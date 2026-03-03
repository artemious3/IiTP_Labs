use std::collections::HashMap;
use std::fmt::Display;
use std::marker::PhantomData;
use std::{io::Write, str::FromStr};
use std::result::Result;
use anyhow::{Ok};
use colored::Colorize;
use thiserror::Error;
use crate::common::SqlState;
use crate::logger::{Logger, SqlLogger};



#[derive(Error, Debug)]
pub enum TuiError {
    #[error("Input output error")]
    IOError,

    #[error("Invalid input")]
    ParseError,

    #[error("Value can't be empty")]
    EmptyError,

    #[error("Cancelled")]
    Cancelled,

    #[error("Exit")]
    Exit,

    #[error("Unwind")]
    Unwind(u32),

    #[error("Invalid value : {0}")]
    InvalidError(String)
}


pub fn get_line(non_empty : bool) -> Result<String,TuiError>{
    use std::result::Result;

   let mut s = String::new();
   let input_result = std::io::stdin().read_line(&mut s);

   match input_result {
       Result::Ok(0) => {
           println!(""); //ctrl+d does not create new line
           Result::Err(TuiError::Cancelled)
       },
       Result::Ok(_) => {
           if non_empty && s.trim().is_empty(){
               Result::Err(TuiError::EmptyError)
           } else {
               Result::Ok(s)
           }
       },
       Err(_) => Result::Err(TuiError::IOError)
   }
}

pub fn print_flush(s : &str){
    print!("{}",s);
    std::io::stdout().flush().unwrap();
}

pub fn get<T>() -> Result<T,TuiError>
where T:FromStr,
{
    use std::result::Result;

    print_flush("> ");
    let res = get_line(false)?.trim().parse::<T>();
    match res {
        Err(_) => Err(TuiError::ParseError),
        Result::Ok(val) => std::result::Result::Ok(val)
    }
}

pub fn get_bounded<T>(min : T, max : T) -> Result<T,TuiError>
where T:FromStr+PartialOrd+Display {

    use std::result::Result;

    let res = get::<T>()?;
    if res < min || res >= max{
        println!("Value should be between {} and {}", min, max);
        Err(TuiError::ParseError)
    } else {
        Result::Ok(res)
    }
}

pub fn get_yes_no() -> Result<bool,TuiError> {
    use std::result::Result;

    print_flush("[y/n]");
    let s = get_line(true)?;
    let y_or_n = s.trim();
    match y_or_n {
        "y" => {
            return Result::Ok(true);
        },
        "n" => {
            return Result::Ok(false);
        },
        _ => {
            eprintln!("{}", "please input `y` or `n`".red());
            return Err(TuiError::ParseError);
        }
    }
}

pub fn looped<T,F>(f : F)->Result<T,TuiError>
where F : Fn()->Result<T,TuiError>
{
    loop {
        let oval = f();
        match oval{
            Result::Ok(val) => return Result::Ok(val),
            Err(TuiError::Cancelled) => return Result::Err(TuiError::Cancelled),
            Err(e) => println!("{} : {}", "Error".red(), e.to_string().red()),
        }
    }

}


pub fn select<T, ToStringCall>(options : &[T], f: ToStringCall) -> Result<usize,TuiError>
where ToStringCall : Fn(&T)->String
{
    use std::result::Result;

    for (idx,t) in options.iter().enumerate(){
        println!("{} : {}", idx+1, f(t));
    }

    let selection = get::<usize>()?;
    if selection != 0 && selection <= options.len(){
        Result::Ok(selection-1)
    } else {
        Result::Err(TuiError::InvalidError(format!("{}{}", "Input the value from 1 to ".red(), options.len().to_string().red())))
    }
}



// #[derive(Clone)]
// pub enum UserActionResult
// {
//     Ok,
//     Err(String),
// }

pub type UserActionResult = anyhow::Result<()>;

#[async_trait::async_trait(?Send)]
pub trait UserAction{
    type State;
    fn name(&self) -> &str;
    async fn invoke(&self, _ : &mut Self::State) -> UserActionResult;
}

pub struct DumbAction<T>{
    name : String,
    pd : PhantomData<T>,
}

impl<T> DumbAction<T>
where T : Sync
{
    pub fn new(name : &str) -> Self{
        DumbAction{
            name : name.into(),
            pd : PhantomData,
        }
    }
}

#[async_trait::async_trait(?Send)]
impl<T> UserAction for DumbAction<T>
where T : Sync
{
    type State = T;
    fn name(&self) -> &str {
        return self.name.as_str();
    }
    async fn invoke(&self, _ : &mut T) -> UserActionResult {
        Ok(())
    }
}


pub struct FnAction<T,F>
where
    F : AsyncFn(&mut T) -> UserActionResult,
    T : Sync
{
    name : String,
    func : F,
    pd : PhantomData<T>
}

impl<T,F> FnAction<T,F>
where
    F : AsyncFn(&mut T) -> UserActionResult,
    T : Sync
{
    pub fn new(name : &str, func : F)->Self{
        Self { name:name.into(), func, pd: PhantomData }
    }

}

#[async_trait::async_trait(?Send)]
impl<T,F> UserAction for FnAction<T,F>
where
    F : AsyncFn(&mut T) -> UserActionResult,
    T : Sync
{
    type State = T;
    fn name(&'_ self) -> &str {
        &self.name.as_str()
    }
    async fn invoke(&self, state : &mut Self::State) -> UserActionResult {
        (self.func)(state).await
    }

}



type UserActionBox<T> = Box<dyn UserAction<State = T>>;

pub struct ActionDispatcher<T>
where T:SqlState{
    actions : Vec<UserActionBox<T>>,
    subactions : HashMap<usize, Vec<usize>>,
    root : Option<usize>,
}

enum SelectSubactionResult {
    Up,
    Subaction(usize),
    Invalid
}

impl<T> ActionDispatcher<T>
where T:SqlState{

    pub fn new() -> Self {
        ActionDispatcher {
            actions : Vec::new(),
            subactions : HashMap::new(),
            root: None,
        }
    }


    pub fn add_action(&mut self, action : UserActionBox<T>) -> usize {
        self.actions.push(action);
        self.actions.len()-1
    }

    pub fn set_root(&mut self, root : usize){
        assert!(root < self.actions.len());
        self.root = Some(root);
    }

    pub fn set_children(&mut self, parent_id : usize, children_ids : Vec<usize>){
        assert!(parent_id < self.actions.len(), "the parent id is not valid");
        for child_id in &children_ids {
            assert!(*child_id < self.actions.len());
        }
        //ignore previous children, if any
        let _ = self.subactions.insert(parent_id, children_ids);
    }

    pub fn append_children(&mut self, parent_id : usize, mut children_ids : Vec<usize>){
        assert!(parent_id < self.actions.len(), "the parent id is not valid");
        for child_id in &children_ids {
            assert!(*child_id < self.actions.len());
        }
        if let Some(entry) = self.subactions.get_mut(&parent_id){
            entry.append(&mut children_ids);
        } else {
            self.subactions.insert(parent_id, children_ids);
        }
    }



    fn select_subaction_or_up(&self, action : usize) -> SelectSubactionResult{

        let osubactions = self.subactions.get(&action);
        if let Some(subactions_src) = osubactions {

            // subactions array with added option to go up
            let mut subactions = subactions_src.clone();

            if !subactions.is_empty(){

                const GO_UP_OPTION : usize = std::usize::MAX;
                subactions.push(GO_UP_OPTION);

                let oindex = select(subactions.as_slice(), |val|{
                    if *val == GO_UP_OPTION{
                       "EXIT".to_string()
                    } else {
                        //PANICS : never, subactions slice is valid
                        self.actions.get(*val).unwrap().name().to_string()
                    }
                });

                match oindex {
                    Result::Ok(index) => {
                        if subactions[index] == GO_UP_OPTION{
                            return SelectSubactionResult::Up;
                        } else {
                            return SelectSubactionResult::Subaction(subactions[index])
                        }
                    },
                    Err(e) => {
                        println!("{}", e.to_string().red());
                        return SelectSubactionResult::Invalid;
                    }
                }

            } else {
                //subactions are initialized, but empty for some reason
                return SelectSubactionResult::Up;
            }
        } else {
            //subactions are empty, return back
            return SelectSubactionResult::Up;

        }
    }


    pub async fn run(&self, state : &mut T,logger : &SqlLogger) {
        let mut stack : Vec<usize> = Vec::new();
        // PANIC : only if the root was not set (programmer's mistake)
        stack.push(self.root.expect("Set root for the action dispatcher"));

        loop {
            let ocurrent = stack.last();
            if let Some(current) = ocurrent{

                // invoke action
                // PANICS : never, we push only valid actions onto stack
                let action = self.actions.get(*current).unwrap();
                println!("{:_^30}", "");
                println!("{: ^30}", action.name());
                println!("{:^^30}", "");
                let result = action.invoke(state).await;
                if let Err(e) = result {
                    let o_tui_err = e.downcast_ref::<TuiError>();
                    if let Some(TuiError::Exit) = o_tui_err{
                        return;
                    } else if let Some(TuiError::Cancelled) = o_tui_err{
                        println!("{}", "Cancelled".yellow());
                    } else if let Some(TuiError::Unwind(level)) = o_tui_err {
                        for _ in 0..*level {
                            stack.pop();
                        }
                        continue;
                    } else {
                        eprintln!("{} : {}", "Error".red(), e.to_string().red());
                        eprintln!("{:?}",e);
                        stack.pop();
                        logger.log(state.pool(),format!("Action `{}` returned error `{}`",action.name(),e), false).await;
                        continue;
                    }

                } else {
                    logger.log(state.pool(),format!("Action `{}` returned success",action.name()), true).await;
                }

                let subaction_selection = self.select_subaction_or_up(*current);
                match subaction_selection {
                    SelectSubactionResult::Up => {let _ = stack.pop();},
                    SelectSubactionResult::Subaction(action) => stack.push(action),
                    SelectSubactionResult::Invalid => ()
                }


            } else {
                return;
            }


        };
    }
}
