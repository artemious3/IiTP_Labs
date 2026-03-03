
use anyhow::{anyhow, bail};

use crate::{common::list_products_by_pattern, tui::{self, UserActionResult, get_bounded, print_flush}};
use super::*;


pub async fn list_all(s : &mut ClientState) -> UserActionResult {
    crate::common::list_products(s).await
}

pub async fn list_in_order(s : &mut ClientState) -> UserActionResult {
    use tabled::{derive::display,Table};
    use crate::entities::Money;
    #[derive(tabled::Tabled)]
    #[tabled(display(Option, "display::option", "Unknown"))]
    struct ProductListView {
        id: i64,
        title: String,
        description: Option<String>,
        mass: i32,
        price: Money,
        producer: String,
    }

    let order_id = get_order_id(s)?;

    let products = sqlx::query_as!(
        ProductListView,
        r#"
        SELECT pr.id, pr.title, pr.description, pr.mass, pr.price as price, p.name as producer
            FROM "Product" as pr
            RIGHT JOIN "OrderProductRelation" as opr ON opr.order_id = $1
            JOIN "Producer" as p ON pr.producer_id = p.id
        "#,
        order_id
    )
    .fetch_all(s.pool())
    .await?;

    let table = Table::new(products);
    println!("{}", table);

    Ok(())
}


pub async fn select_product_from_order(s : &mut ClientState) -> UserActionResult {
   super::order::inspect(s).await?;
   print_flush("Product id:");
   let product_id : i64 = tui::looped(tui::get)?;
   s.product_id = Some(product_id);
   Ok(())
}


pub async fn select_and_add_to_order(s : &mut ClientState) -> UserActionResult {
    list_all(s).await?;
    print_flush("Product id:");
    let product_id : i64 = tui::looped(tui::get)?;
    let order = get_order_id(s)?;

    print_flush("Amount:");
    let amount : i32 = tui::looped(||get_bounded(0, 9999))?;


    sqlx::query!(r#"
        INSERT INTO "OrderProductRelation"
            (order_id, product_id,amount)
        VALUES
            ($1,$2,$3)
        "#, order, product_id, amount)
    .execute(&s.pool)
    .await?;

    Ok(())
}

pub async fn update_in_order(s : &mut ClientState) -> UserActionResult {

    let order = get_order_id(s)?;
    let product = get_product_id(s)?;

    print_flush("New amount:");
    let amount : i32 = tui::looped(||get_bounded(0, 9999))?;


    let rows = sqlx::query!(r#"
        UPDATE "OrderProductRelation"
            SET amount=$1
        WHERE (order_id,product_id) = ($2,$3)
            "#, amount, order, product)
    .execute(&s.pool)
    .await?
    .rows_affected();

    if rows == 0 {
        bail!("Product is not in the order");
    }
    Err(anyhow!(TuiError::Unwind(2)))
}


pub async fn remove_from_order(s : &mut ClientState) -> UserActionResult {
    let order = get_order_id(s)?;
    let product = get_product_id(s)?;

    let rows = sqlx::query!(r#"
        DELETE FROM "OrderProductRelation"
        WHERE (order_id,product_id) = ($1,$2)
            "#, order, product)
    .execute(&s.pool)
    .await?
    .rows_affected();

    if rows == 0 {
        bail!("Product is not in the order");
    }

    Err(anyhow!(TuiError::Unwind(2)))
}

pub async fn search_and_add_to_order(s : &mut ClientState) -> UserActionResult {
    print_flush("Search:");
    let pattern = tui::looped(||tui::get_line(true) )?;
    list_products_by_pattern(s, &pattern).await?;

    let product_id : i64 = tui::looped(tui::get)?;
    let order = get_order_id(s)?;

    print_flush("Amount:");
    let amount : i32 = tui::looped(||get_bounded(0, 9999))?;


    sqlx::query!(r#"
        INSERT INTO "OrderProductRelation"
            (order_id, product_id,amount)
        VALUES
            ($1,$2,$3)
        "#, order, product_id, amount)
    .execute(&s.pool)
    .await?;

    Ok(())
}
