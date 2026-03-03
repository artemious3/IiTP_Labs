
use sqlx::{Pool,Postgres};
use crate::{common::{SqlState, dumb_action}, tui::{self, ActionDispatcher, FnAction, UserActionResult, get_bounded, get_yes_no, print_flush}};
use crate::entities::OrderState;
use anyhow::*;
use tabled::{Table,Tabled, derive::display};

pub struct LogistitianState{
    pub pool : Pool<Postgres>,
    pub order_id : Option<i64>
}

impl LogistitianState {
    fn get_order(&self) -> Result<i64>{
        self.order_id.ok_or(anyhow!("Select order first"))
    }
}


impl SqlState for LogistitianState {
    fn pool<'a>(&'a self) -> &'a Pool<Postgres> {
        &self.pool
    }
}

pub async fn select_order(s : &mut LogistitianState) -> UserActionResult{

    #[derive(Debug,sqlx::FromRow)]
    #[derive(Tabled)]
    #[tabled(display(Option, "display::option", "Unknown"))]
    pub struct OrderLogisticianView {
        pub idx : i64,
        pub id : i64,
        pub is_paid : bool,
        pub state : OrderState,
        pub dropsite_id : i64,
        pub dropsite_addr : Option<String>,
    }

    let confirmed_orders : Vec<OrderLogisticianView> = sqlx::query_as(
    r#"
        SELECT
            ROW_NUMBER() OVER (ORDER BY o.id) AS idx,
            o.id,
            o.is_paid,
            o.state,
            o.dropsite as dropsite_id,
            l_dropsite.address as dropsite_addr
        FROM "Order" o
        LEFT JOIN "Warehouse" w_dropsite ON o.dropsite = w_dropsite.id
        LEFT JOIN "Location" l_dropsite ON w_dropsite.location_id = l_dropsite.id
        WHERE o.state = 'Confirmed'
    "#
    )
    .fetch_all(&s.pool)
    .await?;

    let len = confirmed_orders.len();

    println!("{}", Table::new(&confirmed_orders));

    print_flush("Index:");
    let idx = tui::looped(||get_bounded(1, len+1))?;


    s.order_id = Some(confirmed_orders[idx-1].id);

    Ok(())
}

pub async  fn inspect_route(s : &mut LogistitianState) -> UserActionResult {
    let order_id = s.get_order()?;

    #[derive(Tabled)]
    struct RoutePointView {
        index: i32,
        warehouse_id: i64,
        address: String,
    }

    let points: Vec<RoutePointView> = sqlx::query_as!(
        RoutePointView,
        r#"
            SELECT
                rp.index,
                w.id as warehouse_id,
                l.address
            FROM "OrderRoutePoints" rp
            JOIN "Warehouse" w ON rp.warehouse_id = w.id
            JOIN "Location" l ON w.location_id = l.id
            WHERE rp.order_id = $1
            ORDER BY rp.index ASC
        "#,
        order_id
    )
    .fetch_all(&s.pool)
    .await?;

    if points.is_empty() {
        println!("Route is empty");
    } else {
        println!("{}", Table::new(points));
    }

    Ok(())
}

pub async fn add_route_point(s : &mut LogistitianState) -> UserActionResult {
    let order_id = s.get_order()?;

    #[derive(sqlx::FromRow, Debug)]
    struct Warehouse {
        id: i64,
        address: String,
    }

    let warehouses: Vec<Warehouse> = sqlx::query_as!(
        Warehouse,
        r#"
            SELECT w.id, l.address
            FROM "Warehouse" w
            JOIN "Location" l ON w.location_id = l.id
        "#
    )
    .fetch_all(&s.pool)
    .await?;

    let selection = tui::looped(||tui::select(&warehouses, |w| format!("W{} - {}", w.id, w.address)))?;

    let selected_warehouse = &warehouses[selection];

    sqlx::query!(
        r#"
            INSERT INTO "OrderRoutePoints" (order_id, warehouse_id)
            VALUES ($1, $2)
        "#,
        order_id,
        selected_warehouse.id,
    )
    .execute(&s.pool)
    .await?;

    println!("Added warehouse {} to route", selected_warehouse.id);

    Ok(())
}

pub async fn remove_route_point(s : &mut LogistitianState) -> UserActionResult {
    let order_id = s.get_order()?;

    inspect_route(s).await?;

    println!("Enter index of route point to remove");
    let index_to_remove = tui::looped(tui::get::<i32>)?;

    let mut tx = s.pool.begin().await?;

    let result = sqlx::query!(
        r#"
            DELETE FROM "OrderRoutePoints"
            WHERE order_id = $1 AND index = $2
        "#,
        order_id,
        index_to_remove
    )
    .execute(&mut *tx)
    .await?;

    if result.rows_affected() == 0 {
        bail!("No such route point index");
    }

    tx.commit().await?;

    println!("Removed route point at index {}", index_to_remove);

    Ok(())
}

pub async fn confirm_route(s : &mut LogistitianState) -> UserActionResult {
    let order_id = s.get_order()?;
    inspect_route(s).await?;

    print_flush("Confirm this route?");
    let yes = tui::looped(get_yes_no)?;
    if !yes{
        return Ok(());
    }

    let mut tx = s.pool.begin().await?;

    sqlx::query!(
        r#"
            UPDATE "Order"
            SET
                state = 'Routed'
            WHERE id = $1
        "#,
        order_id,
    )
    .execute(&mut *tx)
    .await?;

    tx.commit().await?;

    println!("Order {} has been routed", order_id);

    Ok(())
}


pub fn dispatcher() -> ActionDispatcher<LogistitianState> {
    let mut dispatcher = ActionDispatcher::<LogistitianState>::new();
    let root = dispatcher.add_action(Box::new(FnAction::new("Order routing", dumb_action)));
    let sel = dispatcher.add_action(Box::new(FnAction::new("Select order", select_order)));
    let insp = dispatcher.add_action(Box::new(FnAction::new("Inspect route", inspect_route)));
    let add = dispatcher.add_action(Box::new(FnAction::new("Add route point", add_route_point)));
    let rem = dispatcher.add_action(Box::new(FnAction::new("Remove route point", remove_route_point)));
    let conf = dispatcher.add_action(Box::new(FnAction::new("Confirm route", confirm_route)));
    dispatcher.set_children(root, vec![sel, insp, add,rem,conf]);
    dispatcher.set_root(root);
    dispatcher
}
