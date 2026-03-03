use crate::common::{self};
use crate::tui::{self, UserActionResult, get, print_flush};
use colored::*;
use crate::entities::Money;
use super::AdminState;


pub async fn create(state: &mut AdminState) -> UserActionResult {
    print_flush("Product title: ");
    let title = tui::looped(|| tui::get_line(true))?;

    print_flush("Product description (optional): ");
    let description = tui::get_line(false)?;
    let description = if description.is_empty() {
        None
    } else {
        Some(description)
    };

    print_flush("Product mass (in grams): ");
    let mass = tui::looped(get::<i32>)?;

    print_flush("Product price: ");
    let price = tui::looped(get::<Money>)?;

    super::producer::list(state).await?;
    print_flush("Select producer id for product: ");
    let producer_id = tui::looped(get::<i32>)?;

    sqlx::query!(
        r#"
        INSERT INTO "Product"
            (title, description, mass, price, producer_id)
        VALUES
            ($1, $2, $3, $4, $5)
            "#,
        title,
        description,
        mass,
        price.0,
        producer_id as i64
    )
    .execute(&state.pool)
    .await?;

    println!("{}", "Product created successfully".green());

    Ok(())
}


pub async fn delete(state: &mut AdminState) -> UserActionResult {
    common::list_products(state).await?;
    common::delete(state, "Product").await?;
    Ok(())
}

pub async fn update(state: &mut AdminState) -> UserActionResult {
    common::list_products(state).await?;

    print_flush("Update id: ");
    let update_id = tui::looped(get::<i64>)?;

    let product_data = sqlx::query!(
        r#"SELECT title, description, mass, price, producer_id FROM "Product" WHERE id = $1"#,
        update_id
    )
    .fetch_optional(&state.pool)
    .await?;

    if product_data.is_none() {
        println!("{}", "Product with this id not found".red());
        return Ok(());
    };

    print_flush("New product title: ");
    let title = tui::looped(|| tui::get_line(true))?;

    print_flush("New product description (optional): ");
    let description = tui::get_line(false)?;
    let description = if description.is_empty() {
        None
    } else {
        Some(description)
    };

    print_flush("New product mass (in grams): ");
    let mass = tui::looped(get::<i32>)?;

    print_flush("New product price: ");
    let price = tui::looped(get::<Money>)?;

    super::producer::list(state).await?;
    print_flush("New producer id for product: ");
    let producer_id = tui::looped(get::<i32>)?;

    sqlx::query!(
        r#"
        UPDATE "Product"
        SET title = $1, description = $2, mass = $3, price = $4, producer_id = $5
        WHERE id = $6
        "#,
        title,
        description,
        mass,
        price.0,
        producer_id as i64,
        update_id
    )
    .execute(&state.pool)
    .await?;

    println!("{}", "Product updated successfully.".green());
    
    Ok(())
}
