
use crate::tui::*;
use chrono::NaiveDate;
use crate::entities::RoleEnum;

use argon2::{
    password_hash::{
        rand_core::OsRng,
        PasswordHasher, SaltString
    },
    Argon2
};
use anyhow::*;

use super::AdminState;

pub async fn register_new_staff(s : &mut AdminState) -> UserActionResult{
    let login = looped(|| {
        print_flush("Login: ");
        get_line(true)
    })?;

    let password = looped(|| {
        print_flush("Password: ");
        get_line(true)
    })?;

    let first_name = looped(|| {
        print_flush("First name: ");
        get_line(true)
    })?;

    let last_name = looped(|| {
        print_flush("Last name: ");
        get_line(true)
    })?;

    let phone = looped(|| {
        print_flush("Phone: ");
        get_line(true)
    })?;

    let email = looped(|| {
        print_flush("Email: ");
        get_line(true)
    })?;

    let birth_date : NaiveDate = looped(|| {
        print_flush("Birth date (YYYY-MM-DD): ");
        get()
    })?;

    let roles : Vec<RoleEnum> = vec![RoleEnum::CLIENT,
                                    RoleEnum::LOGISTICIAN,
                                    RoleEnum::WAREHOUSE_STAFF];
    println!("Select role:");
    let role = roles[ select(&roles, |r|format!("{}",r))? ];

    let password_hash = tokio::task::spawn_blocking(move || {
        let salt = SaltString::generate(&mut OsRng);
        let argon2 = Argon2::default();
        argon2.hash_password(password.trim().as_bytes(), &salt)
            .map(|hash| hash.to_string())
            .map_err(|e| anyhow::anyhow!("Failed to hash password: {}", e))
    }).await??;





    let mut tx = s.pool.begin().await?;

    let user_insert_result = sqlx::query!(
        r#"
            INSERT INTO "User" (login, password_hash, first_name, last_name, phone, email, birth_date, role)
            VALUES ($1, $2, $3, $4, $5, $6, $7, $8)
            RETURNING id
        "#,
        login.trim(),
        password_hash,
        first_name.trim(),
        last_name.trim(),
        phone.trim(),
        email.trim(),
        birth_date.and_hms_opt(0, 0, 0).unwrap(),
        role as _
    )
    .fetch_one(&mut *tx)
    .await;

    let user_id = match user_insert_result {
        std::result::Result::Ok(record) => record.id,
        Err(sqlx::Error::Database(db_err)) => {
            if let Some(constraint) = db_err.constraint() {
                 if constraint.ends_with("login_key") {
                    bail!("User with this login already exists");
                } else if constraint.ends_with("phone_key") {
                    bail!("User with this phone number already exists");
                } else if constraint.ends_with("email_key") {
                    bail!("User with this email already exists");
                }
            }
            // For other DB errors, just forward them.
            return Err(db_err.into());
        }
        Err(e) => return Err(e.into()),
    };

    sqlx::query!(
        r#"
            INSERT INTO "Client" (user_id)
            VALUES ($1)
        "#,
        user_id
    )
    .execute(&mut *tx)
    .await?;

    tx.commit().await?;

    println!("Registration successful!");

    Ok(())

}
