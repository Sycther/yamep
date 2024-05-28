#include "dmenu.h"
#include <SPI.h>
#include "EEPROM.h"

// const SPISettings digipotSettings(2000000, MSBFIRST, SPI_MODE0);
DMenu::DMenu(U8G2 &display) : display(display) {}

bool DMenu::begin(std::vector<DPreset> &_presets, std::vector<Pedal>& pedals, PresetSelector *_presetsPage, PedalPage *_pedalPage){
    
    Serial.println(" -- Setting EEPROM");
    EEPROM.begin(1);
    int index = EEPROM.read(0);

    if(index >= 24)
        index = 0;

    Serial.println(" -- Setting Presets");
    // currPreset = presets.begin() + index;

    SPI.begin();
    for(auto pedal : pedals){
        for(auto config : pedal.configs){
            pinMode(config.pin, OUTPUT);
            config.activeLevel = config.level;
        }
    }
    
    this->presets = _presets;
    this->pedals = pedals;
    currPreset = presets.begin()+index;
    setPreset();

    Serial.println(" -- Setting Pages");
    presetsPage = _presetsPage;
    pedalPage = _pedalPage;

    Serial.println(" -- Begin Display");
    display.begin();

    Serial.println(" -- Setting Current Page");
    setPage(pedalPage);
    // currentPage = pedalPage;
    // pedalPage->currPedal = currPreset->pedals.begin();
    

    currentPage->draw(display);
    return true;
}

void DMenu::draw(){
    Serial.println("DRAW: ");
    currentPage->draw(display);
}

void DMenu::setDigiPotLevel(int cs, int header, int level){
    // SPI.beginTransaction(digipotSettings);
    digitalWrite(cs, LOW);
    SPI.transfer(header);
    Serial.printf("SPI: DIGIPOT: %d,%d:%d\n", cs,header, level);
    SPI.transfer(level);
    digitalWrite(cs, HIGH);
    // SPI.endTransaction();
};

void DMenu::next(MenuDial menuDial, int speed){
    Serial.printf("NEXT: Dial-%d, Speed-%d\n", menuDial, speed);
    currentPage->next(menuDial, speed);
}

void DMenu::prev(MenuDial menuDial, int speed){
    Serial.printf("PREV: Dial-%d, Speed-%d\n", menuDial, speed);
    currentPage->prev(menuDial, speed);
}

void DMenu::select(MenuDial menuDial){
    Serial.printf("SELECT: Dial-%d\n", menuDial);
    currentPage->select(menuDial);
}

void DMenu::setPage(DPage *page){
    Serial.println("SET PAGE: ");
    currentPage = page;
    currentPage->set();
    currentPage->draw(display);
}

void DMenu::setPreset(){
    Serial.println("SET PRESET");
    for(auto& pedal: pedals) {
        std::vector<int> preset_configs = currPreset->pedal_presets[pedal.name];
        for(int i = 0; i < pedal.configs.size(); i++){
            pedal.configs[i].level = currPreset->pedal_presets[pedal.name][i];
            pedal.configs[i].activeLevel = pedal.configs[i].level;
            pedal.configs[i].setDigiPot();
        }
    }
}

void DMenu::saveLevels(){
    for(auto& pedal: pedals){
        for(auto& config : pedal.configs){
            config.level = config.activeLevel;
        }
    }
}

void DMenu::resetLevels(){
    for(auto& pedal: pedals){
        for(auto& conf : pedal.configs){
            conf.activeLevel = conf.level;
        }
    }
}

void DMenu::setLevel(){

}

void DMenu::back(){
    Serial.println("BACK: ");
    if(currentPage==pedalPage){
        currentPage = presetsPage;
        presetsPage->selectedPreset = currPreset;
    }else{
        currentPage = pedalPage;
    }
    currentPage->draw(display);
}