
use sqlx::{Pool,Postgres};
use crate::{common::SqlState, tui::{self, ActionDispatcher, FnAction, UserActionResult, print_flush}};
use crate::common::dumb_action;
use anyhow::{Result,anyhow,bail};
use tabled::{Table, derive::display};
use crate::entities::Order;

pub struct WStaffState{
    pub pool : Pool<Postgres>,
    pub current_warehouse : Option<i64>
}

impl WStaffState {
    fn get_warehouse(&self) -> Result<i64>{
       self.current_warehouse.ok_or(anyhow!("Select warehouse first"))
    }
}

async fn forbid_if_not_dropsite(s : &mut WStaffState) -> UserActionResult{
    let wh_id = s.get_warehouse()?;

    let is_dropsite = sqlx::query_scalar!(
        r#"SELECT is_dropsite FROM "Warehouse" WHERE id = $1"#,
        wh_id
    )
    .fetch_one(&s.pool)
    .await?;

    if !is_dropsite {
        bail!("This warehouse is not a dropsite.");
    }
    Ok(())
}

impl SqlState for WStaffState {
    fn pool<'a>(&'a self) -> &'a Pool<Postgres> {
        &self.pool
    }
}

pub async fn select_warehouse(s : &mut WStaffState) -> UserActionResult {
    #[derive(tabled::Tabled)]
    #[tabled(display(Option, "display::option", "Unknown"))]
    struct WarehouseListView {
        id : i64,
        address : String,
        x : Option<f64>,
        y : Option<f64>
    }

    let warehouses : Vec<WarehouseListView> = sqlx::query_as!(
        WarehouseListView,
        r#"
        SELECT w.id, l.address, (l.coordinates)[0] AS x, (l.coordinates)[1] AS y
            FROM "Warehouse" as w
            JOIN "Location" as l ON w.location_id = l.id
        "#)
    .fetch_all(&s.pool)
    .await?;

    println!("Warehouse index:");
    let wh_idx = tui::looped(||tui::select(&warehouses, |w|{
        format!("W{} | {}", w.id, w.address)
    }))?;

    s.current_warehouse = Some(warehouses[wh_idx].id);

    let table = Table::new(warehouses);
    println!("{}", table);
    Ok(())
}

pub async fn list_coming(s : &mut WStaffState) -> UserActionResult {
   let wh_id = s.get_warehouse()?;
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
            WHERE o.target_warehouse = $1
        "#
    )
    .bind(wh_id)
    .fetch_all(&s.pool)
    .await?;

    if orders.is_empty() {
        println!("No coming orders found.");
    } else {
        let table = Table::new(orders);
        println!("{}", table);
    }
    Ok(())
}

pub async fn list_stored(s : &mut WStaffState) -> UserActionResult {
    let wh_id = s.get_warehouse()?;
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
             WHERE o.current_warehouse = $1
         "#
     )
     .bind(wh_id)
     .fetch_all(&s.pool)
     .await?;

     if orders.is_empty() {
         println!("No stored orders found.");
     } else {
         let table = Table::new(orders);
         println!("{}", table);
     }
     Ok(())
}

pub async fn confirm_receive(s : &mut WStaffState) -> UserActionResult {
    let wh_id = s.get_warehouse()?;
    list_coming(s).await?;
    println!("Enter order id to confirm receive");
    let order_id = tui::looped(tui::get::<i64>)?;

    let rows = sqlx::query!(
        r#"
            UPDATE "Order"
            SET current_warehouse = target_warehouse, target_warehouse = NULL
            WHERE id = $1 AND target_warehouse = $2
        "#,
        order_id,
        wh_id
    )
    .execute(&s.pool)
    .await?
    .rows_affected();

    if rows > 0 {
        println!("Order {} received", order_id);
    } else {
        println!("Order {} not found or not coming to this warehouse", order_id);
    }

    Ok(())
}

pub async fn confirm_send(s : &mut WStaffState) -> UserActionResult {
    let wh_id = s.get_warehouse()?;
    list_stored(s).await?;
    println!("Enter order id to send");
    let order_id = tui::looped(tui::get::<i64>)?;

    let mut tx = s.pool.begin().await?;

    let current_route_point = sqlx::query!(
        r#"
            SELECT index FROM "OrderRoutePoints"
            WHERE order_id = $1 AND warehouse_id = $2
        "#,
        order_id,
        wh_id
    )
    .fetch_optional(&mut *tx)
    .await?;

    let Some(current_route_point) = current_route_point else {
        bail!("Order {} is not at this warehouse", order_id);
    };

    let next_route_point = sqlx::query!(
        r#"
            SELECT warehouse_id FROM "OrderRoutePoints"
            WHERE order_id = $1 AND index = $2
        "#,
        order_id,
        current_route_point.index + 1
    )
    .fetch_optional(&mut *tx)
    .await?;

    let Some(next_route_point) = next_route_point else {
        bail!("This is the last warehouse in the route. The order should be completed at the dropsite.");
    };

    sqlx::query!(
        r#"
            UPDATE "Order"
            SET target_warehouse = $1, current_warehouse = NULL
            WHERE id = $2
        "#,
        next_route_point.warehouse_id,
        order_id
    )
    .execute(&mut *tx)
    .await?;

    tx.commit().await?;

    println!("Order {} sent to warehouse {}", order_id, next_route_point.warehouse_id);

    Ok(())
}

pub async fn confirm_order_completed(s : &mut WStaffState) -> UserActionResult {
    forbid_if_not_dropsite(s).await?;
    let wh_id = s.get_warehouse()?;

    println!("Orders arriving at or stored in this dropsite:");
    list_coming(s).await?;
    list_stored(s).await?;

    println!("Enter order id to complete");
    let order_id = tui::looped(tui::get::<i64>)?;

    let mut tx = s.pool.begin().await?;

    // let order = sqlx::query!(
    //     r#"SELECT is_paid, dropsite FROM "Order" WHERE id = $1"#,
    //     order_id
    // )
    // .fetch_optional(&mut *tx)
    // .await?
    // .ok_or_else(|| anyhow!("Order {} not found", order_id))?;

    // // TODO : TRIGGER
    // if !order.is_paid {
    //     bail!("Order {} is not paid for. Cannot complete.", order_id);
    // }

    // if order.dropsite != Some(wh_id) {
    //     bail!("Order {} is not designated for this dropsite.", order_id);
    // }

    sqlx::query!(
        r#"
            UPDATE "Order"
            SET state = 'Completed', current_warehouse = NULL, target_warehouse = NULL
            WHERE id = $1
        "#,
        order_id
    )
    .execute(&mut *tx)
    .await?;

    tx.commit().await?;

    println!("Order {} completed.", order_id);

    Ok(())
}


pub fn dispatcher() -> ActionDispatcher<WStaffState> {
    let mut dispatcher = ActionDispatcher::<WStaffState>::new();
    let root = dispatcher.add_action(Box::new(FnAction::new("Warehouse manager", dumb_action)));
    let sel = dispatcher.add_action(Box::new(FnAction::new("Select warehouse", select_warehouse)));
    let lcom = dispatcher.add_action(Box::new(FnAction::new("List coming orders", list_coming)));
    let lstor = dispatcher.add_action(Box::new(FnAction::new("List stored orders", list_stored)));
    let crec = dispatcher.add_action(Box::new(FnAction::new("Confirm order received", confirm_receive)));
    let csend = dispatcher.add_action(Box::new(FnAction::new("Confirm order send", confirm_send)));
    let ccompl = dispatcher.add_action(Box::new(FnAction::new("Confirm order completed (only DROPSITE)", confirm_order_completed)));
    dispatcher.set_children(root, vec![sel,lcom,lstor,crec,csend,ccompl]);
    dispatcher.set_root(root);
    dispatcher
}
