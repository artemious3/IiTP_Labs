use crate::tui::{self, UserActionResult, get, get_bounded, get_yes_no, print_flush};
use sqlx::postgres::types::PgPoint;
use colored::*;
use tabled::Table;
use super::AdminState;

pub async fn create (state : &mut AdminState) -> UserActionResult{

    print_flush("Warehouse address: ");
    let addres_s  = tui::looped(||tui::get_line(true))?;
    let address = addres_s.trim();
    print_flush("Longtitude: ");
    let lo = tui::looped( || get_bounded::<f64>(-180.0, 180.0) )?;
    print_flush("Latitude: ");
    let la = tui::looped( || get_bounded::<f64>(-90.0, 90.0) )?;


    let pgpoint = PgPoint {x : lo, y : la};

    print_flush("Is dropsite ");
    let is_dropsite = tui::looped ( get_yes_no )?;

    sqlx::query!(r#"
        WITH new_location AS (
            INSERT INTO "Location"
            (coordinates, address)
            VALUES
            ($1, $2)
            RETURNING id
        )
        INSERT INTO "Warehouse"
            (location_id,is_dropsite)
        SELECT id,$3
        FROM new_location
            "#,
            pgpoint, address, is_dropsite)
    .execute(&state.pool)
    .await?;

    Ok(())

}

pub async fn list(s : &mut AdminState)  -> UserActionResult{


    use tabled::derive::display;
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

    let table = Table::new(warehouses);
    println!("{}", table);


    Ok(())

}

pub async fn delete(state : &mut AdminState) -> UserActionResult{
    list(state).await?;
    crate::common::delete(state, "Warehouse").await?;
    Ok(())

}

pub async fn update(state : &mut AdminState) -> UserActionResult{

    list(state).await?;

    print_flush("Update id: ");
    let update_id = tui::looped(get::<i64>)?;

    let warehouse_data = sqlx::query!(r#"SELECT location_id FROM "Warehouse" WHERE id = $1"#, update_id)
        .fetch_optional(&state.pool)
        .await?;

    let location_id = if let Some(data) = warehouse_data {
        data.location_id
    } else {
        println!("{}", "Warehouse with this id not found".red());
        return Ok(());
    };

    print_flush("New warehouse address: ");
    let addres_s  = tui::looped(||tui::get_line(true))?;
    let address = addres_s.trim();
    print_flush("New longtitude: ");
    let lo = tui::looped( || get_bounded::<f64>(-180.0, 180.0) )?;
    print_flush("New latitude: ");
    let la = tui::looped( || get_bounded::<f64>(-90.0, 90.0) )?;


    let pgpoint = PgPoint {x : lo, y : la};

    print_flush("Is dropsite ");
    let is_dropsite = tui::looped ( get_yes_no )?;

    let mut tx = state.pool.begin().await?;

    sqlx::query!(r#"
        UPDATE "Location"
        SET coordinates = $1, address = $2
        WHERE id = $3
        "#,
        pgpoint, address, location_id)
    .execute(&mut *tx)
    .await?;

    sqlx::query!(r#"
        UPDATE "Warehouse"
        SET is_dropsite = $1
        WHERE id = $2
        "#,
        is_dropsite, update_id)
    .execute(&mut *tx)
    .await?;

    tx.commit().await?;

    println!("{}", "Warehouse updated successfully.".green());

    Ok(())

}
