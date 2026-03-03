use crate::tui::{self, UserActionResult, get, print_flush};
use crate::common;
use colored::*;
use tabled::Table;

use super::AdminState;

pub async fn create (state : &mut AdminState) -> UserActionResult{

    print_flush("Producer name: ");
    let name  = tui::looped(||tui::get_line(true))?;

    super::warehouse::list(state).await?;
    print_flush("Select warehouse id for producer: ");
    let warehouse_id = tui::looped(get::<i64>)?;


    sqlx::query!(r#"
        INSERT INTO "Producer"
            (name, warehouse_id)
        VALUES
            ($1, $2)
            "#,
            name, warehouse_id)
    .execute(&state.pool)
    .await?;

    println!("{}", "Producer created successfully".green());

    Ok(())

}

pub async fn list(s : &mut AdminState)  -> UserActionResult{

    #[derive(tabled::Tabled)]
    struct ProducerListView {
        id : i32,
        name : String,
        warehouse_id : i64,
        address : String,
    }

    let producers : Vec<ProducerListView> = sqlx::query_as!(
        ProducerListView,
        r#"
        SELECT p.id, p.name, w.id as warehouse_id, l.address
            FROM "Producer" as p
            JOIN "Warehouse" as w ON p.warehouse_id = w.id
            JOIN "Location" as l ON w.location_id = l.id
        "#)
    .fetch_all(&s.pool)
    .await?;

    let table = Table::new(producers);
    println!("{}", table);


    Ok(())

}

pub async fn delete(state : &mut AdminState) -> UserActionResult{

    list(state).await?;
    common::delete(state, "Producer").await?;
    Ok(())

}

pub async fn update(state : &mut AdminState) -> UserActionResult{

    list(state).await?;

    print_flush("Update id: ");
    let update_id = tui::looped(get::<i32>)?;

    let producer_data = sqlx::query!(r#"SELECT name, warehouse_id FROM "Producer" WHERE id = $1"#, update_id)
        .fetch_optional(&state.pool)
        .await?;

    if producer_data.is_none() {
        println!("{}", "Producer with this id not found".red());
        return Ok(());
    };

    print_flush("New producer name: ");
    let name  = tui::looped(||tui::get_line(true))?;

    super::warehouse::list(state).await?;
    print_flush("New warehouse id: ");
    let warehouse_id = tui::looped(get::<i64>)?;


    sqlx::query!(r#"
        UPDATE "Producer"
        SET name = $1, warehouse_id = $2
        WHERE id = $3
        "#,
        name, warehouse_id, update_id)
    .execute(&state.pool)
    .await?;


    println!("{}", "Producer updated successfully.".green());

    Ok(())

}
