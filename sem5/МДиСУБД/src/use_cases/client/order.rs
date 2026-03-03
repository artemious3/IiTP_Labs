use colored::Colorize;
use sqlx::postgres::types::PgMoney;
use tabled::Tabled;

use crate::tui::{self, UserActionResult, print_flush};
use crate::entities::{Money,OrderState,Order};
use anyhow::{anyhow, bail};
use tabled::{Table,derive::display};
use super::*;


pub async fn create(s : &mut ClientState) -> UserActionResult {

    let client_id = get_client_id(s);
    let new_id = sqlx::query_scalar!(r#"
        INSERT INTO "Order"
            (client_id)
        VALUES
            ($1)
        RETURNING id
    "#, client_id)
    .fetch_one(&s.pool)
    .await?;

    s.order_id = Some(new_id);

    println!("{}",format!("Successfully created and selected order {}", new_id).green());

    Ok(())
}

pub async fn delete_selected(s : &mut ClientState) -> UserActionResult {
    let order_id = get_order_id(s)?;


    let o = sqlx::query!(r#"
        SELECT o.state as "state: OrderState"
        FROM "Order" o
        WHERE o.id=$1
        "#, order_id)
    .fetch_one(&s.pool)
    .await?;

    match o.state {
        OrderState::Created => {
            print_flush(format!("Are you sure you want to delete order {}?", order_id).as_str());
            let yes = looped(get_yes_no)?;
            if !yes {
                return Err(anyhow!(TuiError::Cancelled));
            }

            sqlx::query(r#"
                DELETE FROM "Order" o
                WHERE o.id = $1
                "#)
            .bind(order_id)
            .execute(&s.pool)
            .await?;
            println!("{}","Deleted order successully".green());
        }
        _ => {
            println!("{}","You can't delete confirmed orders from application.".red());
        }
    }



    Ok(())
}



pub async fn list(state : &mut ClientState) -> UserActionResult {

    let client_id = get_client_id(state);

    // TODO : think of why state does not work
    let orders : Vec<Order> = sqlx::query_as(
        r#"
            SELECT
                o.id,
                o.is_paid,
                o.state,
                l_dropsite.address as dropsite_addr,
                l_current.address as current_addr,
                l_target.address as target_addr
            FROM "Order" o
            LEFT JOIN "Warehouse" w_dropsite ON o.dropsite = w_dropsite.id
            LEFT JOIN "Location" l_dropsite ON w_dropsite.location_id = l_dropsite.id
            LEFT JOIN "Warehouse" w_current ON o.current_warehouse = w_current.id
            LEFT JOIN "Location" l_current ON w_current.location_id = l_current.id
            LEFT JOIN "Warehouse" w_target ON o.target_warehouse = w_target.id
            LEFT JOIN "Location" l_target ON w_target.location_id = l_target.id
            WHERE o.client_id = $1
        "#
    )
    .bind(client_id)
    .fetch_all(&state.pool)
    .await?;

    if orders.is_empty() {
        println!("No orders found.");
    } else {
        let table = Table::new(orders);
        println!("{}", table);
    }
    Ok(())
}

pub async fn select_order(s : &mut ClientState) -> UserActionResult {
    list(s).await?;
    print_flush("Order id:");
    let order_id = tui::looped(||tui::get::<i64>())?;

    let client_id = sqlx::query_scalar!(
        r#"SELECT client_id FROM "Order" WHERE id=$1"#,
        order_id
    )
    .fetch_optional(&s.pool)
    .await?;

    match client_id {
        Some(id) => {
            if id != s.client_id {
                bail!("Invalid order");
            }
        }
        None => bail!("Invalid order"),
    }

    s.order_id = Some(order_id);

    Ok(())
}


pub async fn inspect(s : &mut ClientState) -> UserActionResult {
    use tabled::Table;
    #[derive(Tabled, sqlx::FromRow)]
    struct ProductView {
        pub id : i64,
        pub name : String,
        pub producer : String,
        pub price : Money,
        pub amount : i32,
        pub total : Money
    }

    let order_id = get_order_id(s)?;

    println!("Printing ORDER {}", order_id);

    let mut tx = s.pool.begin().await?;
    let products = sqlx::query_as!(
        ProductView,
        r#"
            SELECT
                p.id,
                p.title as name,
                pr.name as producer,
                p.price as "price!",
                opr.amount,
                (p.price * opr.amount) as "total!"
            FROM "OrderProductRelation" opr
            JOIN "Product" p ON opr.product_id = p.id
            JOIN "Producer" pr ON p.producer_id = pr.id
            WHERE opr.order_id = $1
        "#,
        order_id
    )
    .fetch_all(&mut *tx)
    .await?;

    let total : PgMoney = sqlx::query_scalar(
        r#"
        SELECT
            COALESCE(SUM(p.price * opr.amount), 0::money)
        FROM
            "Order" o
        LEFT JOIN
            "OrderProductRelation" opr ON o.id = opr.order_id
        LEFT JOIN
            "Product" p ON opr.product_id = p.id
        WHERE
            o.id = $1
        GROUP BY o.id;
        "#
    )
    .bind(order_id)
    .fetch_one(&mut *tx)
    .await?;

    if products.is_empty() {
        println!("No products found for this order.");
    } else {
        let table = Table::new(products);
        println!("{}", table);
        println!("TOTAL:{: ^20}", Money::from(total));
    }
    tx.commit().await?;

    Ok(())

}

pub async fn pay(s : &mut ClientState) -> UserActionResult {
  let order_id = get_order_id(s)?;
  inspect(s).await?;
  print_flush("Pay for this order?");
  let yes = looped(get_yes_no)?;

  if yes {

    let ra = sqlx::query(r#"
      UPDATE "Order" o
      SET is_paid=TRUE
      WHERE o.id=$1 AND o.is_paid=FALSE
      "#)
    .bind(order_id)
    .execute(&s.pool)
    .await?
    .rows_affected();

    if ra == 0 {
      println!("{}", "You've already paid for the order".yellow())
      ;
    } else {
      println!("{}", "Paid successfully".green());
    }

  }

  Ok(())
}


pub async fn confirm(s : &mut ClientState) -> UserActionResult{
    inspect(s).await?;

    let order_id = get_order_id(s)?;

    #[derive(tabled::Tabled)]
    #[tabled(display(Option, "display::option", "Unknown"))]
    struct WarehouseListView {
        id : i64,
        address : String,
        x : Option<f64>,
        y : Option<f64>
    }
    let dropsites = sqlx::query_as!(WarehouseListView, r#"
        SELECT w.id, l.address, (l.coordinates)[0] AS x, (l.coordinates)[1] AS y
            FROM "Warehouse" as w
            JOIN "Location" as l ON w.location_id = l.id
            WHERE w.is_dropsite=TRUE
        "#)
    .fetch_all(&s.pool)
    .await?;
    println!("{}", Table::new(dropsites));

    print_flush("Select dropsite:");
    let dropsite_id : i64 = tui::looped(tui::get)?;

    print_flush(format!("Do you want to confirm order {}?",order_id).as_str());
    let yes = get_yes_no()?;
    if yes {
        sqlx::query!(r#"
            UPDATE "Order"
            SET dropsite = $1,
    		state = 'Confirmed'
            WHERE id = $2;
            "#, dropsite_id, order_id)
        .execute(&s.pool)
        .await?;
        println!("{}", "Confirmed successfully".green());
    }

    Ok(())
}
