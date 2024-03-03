mod page;
use page::*;

enum MenuState {
    PresetPage,
    PedalPage,
}

pub struct DMenu {
    state: MenuState,
    current_page: Box<dyn Page>,
    presets: Vec<Preset>,
}

impl DMenu {
    pub fn new() -> Self {
        DMenu {
            state: MenuState::PedalPage,
            current_page: Box::new(PedalPage{num:32}),
        }
    }

    pub fn draw(&self) {
        self.current_page.draw();
    }

    pub fn next(&mut self) {
        self.current_page.next(&mut menu);
    }

    pub fn prev(&mut self) {
        self.current_page.prev(&mut menu);
    }
} 