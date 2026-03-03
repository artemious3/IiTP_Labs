pub mod order;
pub mod product;

use sqlx::{Pool,Postgres};
use crate::tui::*;
use anyhow::{Result,anyhow};

use crate::{common::SqlState, tui::ActionDispatcher};

pub struct ClientState {
    pub pool : Pool<Postgres>,
    pub client_id : i64,
    pub order_id : Option<i64>,
    pub product_id : Option<i64>
}

impl SqlState for ClientState {
    fn pool<'a>(&'a self) -> &'a Pool<Postgres> {
        &self.pool
    }
}

fn get_client_id(state : &ClientState) -> i64{
    state.client_id
}

fn get_order_id(state : &ClientState) ->Result<i64>{
    state.order_id.ok_or(anyhow!("Select order first"))
}

fn get_product_id(s : &ClientState) -> Result<i64> {
    s.product_id.ok_or(anyhow!("Select product first"))
}

async fn show_info(s : &mut ClientState) -> UserActionResult{
    match get_order_id(s) {
        Ok(id) => {
            println!("SELECTED ORDER : {}", id);
        }
        Err(_) => {
            println!("SELECTED ORDER : None")
        }
    }
    Ok(())
}

pub fn dispatcher() -> ActionDispatcher<ClientState> {
    let mut dispatcher = ActionDispatcher::new();

    let client = dispatcher.add_action(Box::new(FnAction::new("CLIENT",show_info)));
    let sel = dispatcher.add_action(Box::new(FnAction::new("Select ORDER",order::select_order)));
    let insp = dispatcher.add_action(Box::new(FnAction::new("Inspect ORDER",order::inspect)));
    let cr = dispatcher.add_action(Box::new(FnAction::new("Create ORDER",order::create)));
    let del = dispatcher.add_action(Box::new(FnAction::new("Delete ORDER",order::delete_selected)));
    let pay = dispatcher.add_action(Box::new(FnAction::new("Pay for ORDER",order::pay)));

    let ed =dispatcher.add_action(Box::new(FnAction::new("Edit ORDER",product::select_product_from_order)));
    let ed_upd =  dispatcher.add_action(Box::new(FnAction::new("Update amount",product::update_in_order)));
    let ed_rem =  dispatcher.add_action(Box::new(FnAction::new("Remove",product::remove_from_order)));
    dispatcher.set_children(ed, vec![ed_upd,ed_rem]);
    let conf = dispatcher.add_action(Box::new(FnAction::new("Confirm ORDER",order::confirm)));

    let list_add = dispatcher.add_action(Box::new(FnAction::new("List and add PRODUCT",product::select_and_add_to_order)));
    let search_add = dispatcher.add_action(Box::new(FnAction::new("Search and add PRODUCT",product::search_and_add_to_order)));

    dispatcher.set_children(client, vec![sel, cr, del,insp, pay,ed, list_add, search_add, conf]);
    dispatcher.set_root(client);

    dispatcher
}
