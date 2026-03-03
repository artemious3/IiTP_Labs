
use async_trait::async_trait;
use sqlx::{Pool, Postgres};

#[async_trait]
pub trait Logger {
    async fn set_user(&mut self,id : i64, login : String);
    async fn log(&self,pool:&Pool<Postgres>,  msg : String, success : bool);
}

pub struct LoggerUser {
    id : i64,
    login : String
}

pub struct SqlLogger{
    user : Option<LoggerUser>
}

impl SqlLogger {
    pub fn new() -> SqlLogger{
        SqlLogger { user: None }
    }

}


#[async_trait]
impl Logger for SqlLogger {
    async fn set_user(&mut self,id : i64, login : String){
        self.user = Some(LoggerUser{
            id,login
        });
    }
    async fn log(&self,pool:&Pool<Postgres>,msg : String, success : bool){
        let id = self.user.as_ref().map(|u|u.id);
        let login = self.user.as_ref().map(|u|u.login.clone());
        let _ = sqlx::query!(
            r#"
            INSERT INTO "Journal"
            (user_id,login,message,success)
            VALUES
            ($1,$2,$3,$4)
            "#,
            id,login, msg, success
        )
        .execute(pool)
        .await;
    }
}
