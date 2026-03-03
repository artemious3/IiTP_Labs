mod entities;
mod use_cases;
mod tui;
mod logger;
mod common;


use anyhow::Ok;
use sqlx::{Postgres, postgres::PgPoolOptions};
use crate::{common::SqlState, entities::RoleEnum, logger::{Logger, SqlLogger}, use_cases::{admin::{self, AdminState}, client, wstaff}};
use crate::use_cases::logistician::{LogistitianState, self};
use crate::use_cases::{auth::AuthState, client::ClientState,wstaff::WStaffState};
use colored::Colorize;

const GREETING : &str = r#"
███    ███ ██ ██      ██████  ██████  ███████ ██████  ██████  ██ ███████ ███████
████  ████ ██ ██      ██   ██ ██   ██ ██      ██   ██ ██   ██ ██ ██      ██
██ ████ ██ ██ ██      ██   ██ ██████  █████   ██████  ██████  ██ █████   ███████
██  ██  ██ ██ ██      ██   ██ ██   ██ ██      ██   ██ ██   ██ ██ ██           ██
██      ██ ██ ███████ ██████  ██████  ███████ ██   ██ ██   ██ ██ ███████ ███████
"#;

pub async fn authenticate(pool : sqlx::Pool<Postgres>, logger : &mut SqlLogger) -> AuthState {
    let mut auth_state = AuthState{pool, user : None};
    use_cases::auth::dispatcher().run(&mut auth_state,&logger).await;
    if let Some(u) = &auth_state.user{
        logger.set_user(u.id,u.login.clone()).await;
    }
    auth_state
}



#[tokio::main]
async fn main() -> anyhow::Result<()> {

    let pool = PgPoolOptions::new()
        .max_connections(5)
        .connect("postgres://myuser:mypassword@localhost:5432/myapp")
        .await?;

    let mut logger = SqlLogger::new();

    println!("{}",GREETING);

    let auth_state = authenticate(pool,&mut logger).await;

    if let Some(user) = auth_state.user {
        println!("Welcome to Mildberries!");
        println!("Input Ctrl+D for cancelling the operation.");
        match user.role {

            RoleEnum::ADMIN => {
                let mut s = AdminState {pool : auth_state.pool};
                admin::dispatcher().run(&mut s,&logger).await;
            }

            RoleEnum::CLIENT => {
                let mut s = ClientState {pool : auth_state.pool,
                                    client_id : user.id,
                                    order_id:None,
                                    product_id:None};
                client::dispatcher().run(&mut s,&logger).await;
            }

            RoleEnum::LOGISTICIAN  => {
                let mut s = LogistitianState{
                    pool: auth_state.pool,
                    order_id: None
                };
                logistician::dispatcher().run(&mut s,&logger).await;
            }

            RoleEnum::WAREHOUSE_STAFF => {
                let mut s =WStaffState{
                    pool: auth_state.pool,
                    current_warehouse : None
                };
                wstaff::dispatcher().run(&mut s,&logger).await;
            }
        }

    } else {
        println!("{}", "Authentication failure".red());
        return Ok(());
    }


    Ok(())
}
