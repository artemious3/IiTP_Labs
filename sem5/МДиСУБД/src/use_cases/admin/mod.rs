use chrono::NaiveDateTime;
use sqlx::{Pool, Postgres};
use tabled::{Table, Tabled};

use crate::entities::Money;
use crate::tui::*;
use crate::common::SqlState;

pub mod warehouse;
pub mod producer;
pub mod product;
pub mod user;


pub struct AdminState {
    pub pool : Pool<Postgres>
}

impl SqlState for  AdminState {
    fn pool<'a>(&'a self) -> &'a Pool<Postgres> {
        &self.pool
    }
}


async fn show_journal(s:&mut AdminState)->UserActionResult{
    use tabled::derive::display;
    #[derive(Tabled)]
    #[tabled(display(Option, "display::option", "Unknown"))]
    struct JournalEntry {
       user_id : Option<i64>,
       login : Option<String>,
       message : String,
       success : bool,
       timestamp : NaiveDateTime
   }

   let j = sqlx::query_as!(JournalEntry,r#"
       SELECT user_id,login,message,success,timestamp FROM "Journal"
       "#)
    .fetch_all(&s.pool)
    .await?;

   println!("{}", Table::new(j));

   Ok(())
}


async fn list_order_prices(s : &mut AdminState) -> UserActionResult {


    let view = sqlx::query!(r#"
        SELECT "Order".id,
				COALESCE(SUM("OrderProductRelation".amount *  "Product".price), 0.00::money) as total_price
        FROM "OrderProductRelation"
	JOIN "Product"
			ON "OrderProductRelation".product_id = "Product".id
	RIGHT JOIN "Order"
			ON "OrderProductRelation".order_id = "Order".id
        GROUP BY "Order".id;
        "#)
    .fetch_all(&s.pool)
    .await?;

   for o in view {
       println!("Order {} : {}", o.id, Money::from(o.total_price.unwrap_or_default()));
   }

    Ok(())

}

pub async fn get_count_of_order_per_client(s : &mut AdminState) -> UserActionResult {
    let orders_per_client =  sqlx::query!(r#"
        SELECT  "User".first_name, "User".last_name, COUNT(*) as count
        FROM "Order"
        JOIN "Client" ON "Client".id=client_id
        JOIN "User" ON "User".id="Client".user_id
        GROUP BY "User".id
        ORDER BY count DESC;
        "#)
    .fetch_all(&s.pool)
    .await?;

    for c in orders_per_client {
       println!("{} {} -- total {} orders", c.first_name, c.last_name, c.count.unwrap_or_default());
    }

    Ok(())
}

pub async fn get_orders_with_total_paid(s : &mut AdminState) -> UserActionResult {
    let d = sqlx::query!(r#"
        WITH t AS

        (SELECT "Order".id AS order_id, "Order".client_id,
				COALESCE(SUM("OrderProductRelation".amount *  "Product".price), 0.00::money) as total_price
        FROM "OrderProductRelation"
    	JOIN "Product"
			ON "OrderProductRelation".product_id = "Product".id
    	RIGHT JOIN "Order"
			ON "OrderProductRelation".order_id = "Order".id
        GROUP BY "Order".id)

        SELECT order_id, u.first_name, u.last_name, total_price, SUM(total_price) OVER(PARTITION BY client_id) as client_total_paid
        FROM t
        JOIN "Client" c ON c.id=client_id
        JOIN "User" u  ON u.id=c.user_id
        ;
        "#)
    .fetch_all(&s.pool)
    .await?;

    for i in d {
        println!("Order ID {} | Client {} {} | Order total {} | Client total paid {}",i.order_id, i.first_name, i.last_name,
            Money::from(i.total_price.unwrap_or_default()), Money::from(i.client_total_paid.unwrap_or_default()));
    }

    Ok(())
}


pub fn dispatcher() -> ActionDispatcher<AdminState> {

    let mut dispatcher = ActionDispatcher::new();
    let warehouse = dispatcher.add_action(Box::new(DumbAction::new("Warehouse")));
    let warehouse_children =  vec![
            dispatcher.add_action(Box::new(FnAction::new("List", crate::use_cases::admin::warehouse::list))),
            dispatcher.add_action(Box::new(FnAction::new("Create", crate::use_cases::admin::warehouse::create))),
            dispatcher.add_action(Box::new(FnAction::new("Update", crate::use_cases::admin::warehouse::update))),
            dispatcher.add_action(Box::new(FnAction::new("Delete", crate::use_cases::admin::warehouse::delete))),
        ];
    dispatcher.set_children(warehouse, warehouse_children);

    let producer = dispatcher.add_action(Box::new(DumbAction::new("Producer")));
    let producer_children =  vec![
            dispatcher.add_action(Box::new(FnAction::new("List", crate::use_cases::admin::producer::list))),
            dispatcher.add_action(Box::new(FnAction::new("Create", crate::use_cases::admin::producer::create))),
            dispatcher.add_action(Box::new(FnAction::new("Update", crate::use_cases::admin::producer::update))),
            dispatcher.add_action(Box::new(FnAction::new("Delete", crate::use_cases::admin::producer::delete))),
        ];
    dispatcher.set_children(producer, producer_children);

    let product = dispatcher.add_action(Box::new(DumbAction::new("Product")));
    let product_children =  vec![
            dispatcher.add_action(Box::new(FnAction::new("List", crate::common::list_products))),
            dispatcher.add_action(Box::new(FnAction::new("Create", crate::use_cases::admin::product::create))),
            dispatcher.add_action(Box::new(FnAction::new("Update", crate::use_cases::admin::product::update))),
            dispatcher.add_action(Box::new(FnAction::new("Delete", crate::use_cases::admin::product::delete))),
        ];
    dispatcher.set_children(product, product_children);

    let stats = dispatcher.add_action(Box::new(DumbAction::new("Stats")));
    let stats_children = vec![
        dispatcher.add_action(Box::new(FnAction::new("List prices of orders", list_order_prices))),
        dispatcher.add_action(Box::new(FnAction::new("List count of orders per client ", get_count_of_order_per_client))),
        dispatcher.add_action(Box::new(FnAction::new("List orders and total money, paid by client", get_orders_with_total_paid))),
    ];
    dispatcher.set_children(stats, stats_children);


    let lj = dispatcher.add_action(Box::new(FnAction::new("List JOURNAL", show_journal)));
    let new_user = dispatcher.add_action(Box::new(FnAction::new("New USER", user::register_new_staff)));

    let admin = dispatcher.add_action(Box::new(DumbAction::new("ADMIN")));
    let admin_submenus = vec![
        warehouse,producer,product,lj,new_user,stats
    ];
    dispatcher.set_children(admin, admin_submenus);
    dispatcher.set_root(admin);

    dispatcher

}
