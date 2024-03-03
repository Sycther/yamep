mod dmenu;
use dmenu::DMenu;

fn main() {
    println!("START");

    let mut dmenu : Box<DMenu> = Box::new(DMenu::new());
}
