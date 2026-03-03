
use sqlx::{Pool,Postgres};
use crate::tui::*;
use anyhow::{Result, bail};


pub trait SqlState {
    fn pool<'a>(&'a self) -> &'a Pool<Postgres>;
}

pub async fn delete<'a,T>(s : &mut T, model_name : &str) -> Result<()>
where T : SqlState
{

    print_flush("Remove id");
    let remove_id = get::<i64>()?;
    let affected_rows = sqlx::query(format!(r#"DELETE FROM "{}" WHERE id=$1"#,model_name).as_str())
        .bind(remove_id)
        .execute(s.pool())
        .await?
        .rows_affected();

    match affected_rows {
        0 => bail!("The id does not exist. Nothing was removed"),
        _ => {
            println!("Deleted successfully");
            Ok(())
        }
    }

}


pub async fn list_products<T>(s: &mut T) -> UserActionResult
where T : SqlState
{

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

    let products = sqlx::query_as!(
        ProductListView,
        r#"
        SELECT pr.id, pr.title, pr.description, pr.mass, pr.price as price, p.name as producer
            FROM "Product" as pr
            JOIN "Producer" as p ON pr.producer_id = p.id
        "#
    )
    .fetch_all(s.pool())
    .await?;

    let table = Table::new(products);
    println!("{}", table);

    Ok(())
}

pub async fn list_products_by_pattern<T>(s: &mut T, pattern :  &str) -> UserActionResult
where T : SqlState
{

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

    let products = sqlx::query_as!(
        ProductListView,
        r#"
        SELECT pr.id, pr.title, pr.description, pr.mass, pr.price as price, p.name as producer
        FROM "Product" as pr
        JOIN "Producer" as p ON pr.producer_id = p.id
        WHERE to_tsvector('english', title || ' ' || description) @@ plainto_tsquery('english', $1);
        "#,
        pattern
    )
    .fetch_all(s.pool())
    .await?;

    let table = Table::new(products);
    println!("{}", table);

    Ok(())
}






pub async fn dumb_action<T>(_ : &mut T)->UserActionResult{
    Ok(())
}
