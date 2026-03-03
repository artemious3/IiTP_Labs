use anyhow::{Ok, anyhow, bail};
use chrono::NaiveDate;
use tokio::time::sleep;

use sqlx::{Pool,Postgres};
use crate::{common::SqlState, tui::UserActionResult};
use crate::tui::*;

use tabled::Table;

use argon2::{
    password_hash::{
        rand_core::OsRng,
        PasswordHasher, PasswordVerifier, SaltString
    },
    Argon2
};

use crate::entities;
use crate::common;

pub struct AuthState {
    pub pool : Pool<Postgres>,
    pub user : Option<entities::User>
}

impl SqlState for AuthState {
    fn pool<'a>(&'a self) -> &'a Pool<Postgres> {
        &self.pool
    }
}

pub async fn auth(s : &mut AuthState) -> UserActionResult {
    print_flush("Login:");
    let iusername = get_line(true)?;
    let username = iusername.trim();

    let user = sqlx::query_as!(
        entities::User,
        r#"
            SELECT
            id, login, password_hash, first_name, last_name, phone, email, birth_date, role as "role: entities::RoleEnum"
            FROM "User"
            WHERE login=$1
        "#,
        username)
    .fetch_optional(&s.pool)
    .await?
    .ok_or(anyhow!("User with this login does not exist"))?;

    let hash_str = user.password_hash.clone();

    print_flush("Password:");
    let ipass = get_line(true)?;
    let pass = ipass.trim().to_string();

    let animation = tokio::spawn(async {
        let mut i = 0;
        loop {
            let dots = ".".repeat(i);
            print_flush(format!("\rAuthenticating{}   ", dots).as_str());
            i = (i + 1) % 4;
            sleep(std::time::Duration::from_millis(100)).await;
        }
    });

    let verification_result = tokio::task::spawn_blocking(move || {
        let password_hash = argon2::PasswordHash::new(hash_str.as_str())
            .map_err(|_|anyhow!("Auth data corrupted"))?;
       let pc = password_hash.clone();
        let argon2 = Argon2::default();
        Ok(argon2.verify_password(pass.as_bytes(), &pc).is_err())
    }).await??;

    animation.abort();
    print_flush("\r");

    if verification_result {
        bail!("Invalid password")
    } else {
        use colored::Colorize;
        println!("{}", format!("Authenticated as {} {}", user.first_name, user.last_name).green());

        s.user = Some(user);
        Err(anyhow!(TuiError::Exit))
    }
}

pub async fn register(state : &mut AuthState) -> UserActionResult{
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

    let password_hash = tokio::task::spawn_blocking(move || {
        let salt = SaltString::generate(&mut OsRng);
        let argon2 = Argon2::default();
        argon2.hash_password(password.trim().as_bytes(), &salt)
            .map(|hash| hash.to_string())
            .map_err(|e| anyhow::anyhow!("Failed to hash password: {}", e))
    }).await??;

    let mut tx = state.pool.begin().await?;

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
        entities::RoleEnum::CLIENT as _
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

pub async fn list_clients(state : &mut AuthState) -> UserActionResult{
    let users : Vec<entities::User> = sqlx::query_as!(entities::User,r#"SELECT
        c.id, u.login, u.password_hash, u.first_name, u.last_name, u.phone, u.email, u.birth_date, u.role as "role: entities::RoleEnum"
        FROM "Client" as c
        JOIN "User" u ON u.id = c.user_id
            "#)
    .fetch_all(&state.pool)
    .await?;

    let table = Table::new(users);
    println!("{}",table);
    Ok(())
}

pub fn dispatcher() -> ActionDispatcher<AuthState>{
    let mut authd = ActionDispatcher::<AuthState>::new();
    let root = authd.add_action(Box::new(FnAction::new("Auth", common::dumb_action)));
    let auth = authd.add_action(Box::new(FnAction::new("Authenticate", auth)));
    let register = authd.add_action(Box::new(FnAction::new("Register", register)));
    authd.set_children(root, vec![auth, register]);
    authd.set_root(root);
    authd
}
