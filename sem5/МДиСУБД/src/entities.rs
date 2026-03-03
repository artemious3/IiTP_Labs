use sqlx::postgres::types::PgMoney;
use tabled::{Tabled,derive::display};
use std::{fmt::{self, Display}, str::FromStr};


#[derive(Clone, Copy,Debug, sqlx::Type)]
#[sqlx(type_name = "role_enum")]
pub enum RoleEnum { CLIENT, ADMIN, LOGISTICIAN, WAREHOUSE_STAFF }

#[derive(Clone, Debug, sqlx::Type)]
#[sqlx(type_name = "order_state")]
pub enum OrderState {Created, Confirmed, Routed, Completed}


impl Display for RoleEnum {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f,"{}", match self {
            Self::CLIENT => "CLIENT",
            Self::ADMIN => "ADMIN",
            Self::LOGISTICIAN => "LOGISTICIAN",
            Self::WAREHOUSE_STAFF => "WAREHOUSE STAFF"
        })
    }
}

impl Display for OrderState {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f,"{}", match self {
            Self::Created => "Created",
            Self::Confirmed => "Confirmed",
            Self::Routed => "Routed",
            Self::Completed => "Completed"
        })
    }
}

#[derive(Debug,sqlx::FromRow)]
#[derive(Tabled)]
pub struct User{
    pub id : i64,
    pub login : String,
    pub password_hash : String,
    pub first_name : String,
    pub last_name : String,
    pub phone : String,
    pub email : String,
    pub birth_date : chrono::NaiveDateTime,
    pub role : RoleEnum
}

pub struct Client{
    pub id : i64,
    pub user_id : i64,
}



pub struct Money(pub PgMoney);

impl From<PgMoney> for Money {
    fn from(value: PgMoney) -> Self {
        Money(value)
    }
}

impl fmt::Display for Money {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}.{}", self.0.0/100, self.0.0%100)
    }
}

impl FromStr for Money {
    type Err = anyhow::Error;
    fn from_str(s: &str) -> Result<Self, Self::Err> {

        let mut was_neg = false;
        let mut was_dot = false;
        let mut whole : i64 = 0;
        let mut frac : i64 = 0;

        for (i,c) in s.chars().enumerate() {
            if c == '-' {
                if i != 0 || was_neg {
                    anyhow::bail!("Bad money");
                }
                was_neg = true;
            } else if c == '.' {
                if was_dot {
                    anyhow::bail!("Bad money");
                }
                was_dot = true;
            } else if let Some(dig) = c.to_digit(10) {
                if was_dot {
                    frac = frac*10 + (dig as i64);
                } else {
                    whole = whole * 10 + (dig as i64);
                }
            } else {
                anyhow::bail!("Bad money");
            }
        }
        if frac > 100 {
            anyhow::bail!("Bad fractional part");
        }
        Ok(Money(PgMoney( if was_neg { -1 } else {1} * whole*100 + frac)))

    }

}

#[derive(Debug,sqlx::FromRow)]
#[derive(Tabled)]
#[tabled(display(Option, "display::option", "Unknown"))]
pub struct Order {
    pub id : i64,
    pub is_paid : bool,
    pub state : OrderState,
    pub dropsite_addr : Option<String>,
    pub current_addr : Option<String>,
    pub target_addr : Option<String>,
}
