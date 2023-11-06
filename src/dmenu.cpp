#include "dmenu.h"

DMenu::DMenu(U8G2 &display) : display(display) {}

void DMenu::begin(std::vector<Preset> &_presets, PresetSelector *_presetsPage, PedalPage *_pedalPage){
    Serial.println("BEGIN: MENU");
    Serial.println(_presets.size());

    this->presets = _presets;
    currPreset = presets.begin();
    Serial.printf("Presets: %d,%d", currPreset, presets.begin());

    Serial.println("MENU: Set Presets");
    presetsPage = _presetsPage;
    pedalPage = _pedalPage;

    Serial.println("MENU: Set Pages");
    currentPage = pedalPage;
    presetsPage->selectedPreset = currPreset;
    pedalPage->currPedal = currPreset->pedals.begin();
    
    Serial.println("MENU: Set Current Page");

    display.begin();
    Serial.println("MENU: Begin Display");
    currentPage->draw(display);
}

void DMenu::draw(){
    currentPage->draw(display);
}

void DMenu::next(){
}

void DMenu::prev(){
}

void DMenu::back(){
    if(currentPage==pedalPage){
        currentPage = presetsPage;
        presetsPage->selectedPreset = currPreset;
        currentPage->draw(display);
    }else{
        currentPage = pedalPage;
        currentPage->draw(display);
    }
}