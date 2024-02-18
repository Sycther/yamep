

pub trait Page {
    fn draw(&self);
    fn next(&self, &mut menu);
    fn prev(&self, &mut menu);
    fn select(&self, &mut menu);
}

pub struct PedalPage {
    pedal: Pedal;
}

impl Page for PedalPage {
    fn draw(&self) {
        println!("DRAW PEDAL PAGE {}", self.num);
    }
    fn next(&self, &mut menu){
        println!("PEDAL: NEXT");
    }
    fn prev(&self, &mut menu){
        println!("PEDAL: PREV");
    }
    fn select(&self, &mut menu){
        println!("PEDAL: SELECT");
    }
}

pub struct PresetsPage{
    preset : Preset
}

impl Page for PresetsPage {
    fn draw(&self) {
        println!("DRAW PRESET PAGE");
    }
    fn next(&self, &mut menu){
        println!("PRESET: NEXT");
    }
    fn prev(&self){
        println!("PRESET: PREV");
    }
    fn select(&self){
        println!("PRESET: SELECT");
    }
}