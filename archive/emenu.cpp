#include "emenu.h"

#pragma region EMENU

EMenu::EMenu(U8G2 *u8g2) : display(u8g2) {}

void EMenu::begin(std::vector<EPage> &presets) {
    this->presets = presets;
    display->begin();
    sel_btn.begin();
    back_btn.begin();

    EEPROM.begin(1);
    int preI = EEPROM.read(0);
    EPage *firstPage;
    try {firstPage = &presets.at(preI);}
    catch (...) {firstPage = &presets[0];}

    presetSel = std::vector<EPage>::iterator(firstPage);
    selectorPage.setPresets(&presets);

    addPageStack(&selectorPage);
    addPageStack(firstPage);

    drawPage();
}

void EMenu::addPageStack(EPage *page) {
    pageStack.push_back(page);
    page->setIterator();
    drawPage();
}

void EMenu::drawPage() {
    if(pageStack.empty()){
        pageStack.push_back(&selectorPage);
        pageStack.push_back(&(*presetSel));
    }
    pageStack.back()->DrawPage(display);
}

void EMenu::backPage(){
    pageStack.pop_back();
    drawPage();
}

void EMenu::HandleEvents(){
    encoder.tick();

    // bool select = sel_btn.pressed();

    RotaryEncoder::Direction dir = encoder.getDirection();
    switch(dir){
        case RotaryEncoder::Direction::COUNTERCLOCKWISE :
            pageStack.back()->increaseItemSel(display);
            Serial.println("NEXT");
            break;
        case RotaryEncoder::Direction::CLOCKWISE :
            pageStack.back()->decreaseItemSel(display);
            Serial.println("PREVIOUS");
            break;
    }
    if(sel_btn.pressed()){
        pageStack.back()->runItemCall(*this);
        Serial.println("SELECT");
    }
    if(back_btn.pressed()){
        backPage();
        Serial.println("BACK");
    }
}


void EPage::runItemCall(EMenu &m){
    (*itemSel).callback();
}


void EPageSelector::runItemCall(EMenu &m){
    m.addPageStack(&(*presetSel));
    m.presetSel=presetSel;
    EEPROM.write(0, (byte)(presetSel-(*presets).begin()));
    EEPROM.commit();
}

#pragma endregion EMENU