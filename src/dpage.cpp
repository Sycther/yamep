#include "dpage.h"
#include "dmenu.h"
#include <string>

#pragma region DPAGE

DPage::DPage(DMenu *menu) {
    this->menu = menu;
}

#pragma endregion DPAGE

#pragma region PRESET_SELECTOR

PresetSelector::PresetSelector(DMenu *menu){
    this->menu = menu;
    Serial.println("CREATE: PRESET PAGE");
}

void PresetSelector::set(){

}

void PresetSelector::draw(U8G2 &display){
    Serial.println("DRAW: PRESET PAGE");
    display.clearBuffer();

    display.setFont(u8g2_font_5x8_mf);
    display.setFontPosTop();

    int x = 10;
    int y = 0;

    display.drawStr(0, y, "PRESETS");
    for(auto pre = menu->presets.begin(); pre != menu->presets.end(); pre++){
        y+=12;
        std::string tmp = "P"+std::to_string(pre-menu->presets.begin());
        display.drawStr(x,y,tmp.c_str());
        display.drawStr(x+20, y,pre->name);
    }

    //Draw Bottom Options
    display.drawRFrame(0,DISPLAY_HEIGTH-TITLE_HEIGHT,DISPLAY_WIDTH/3, TITLE_HEIGHT, EMENU_RADIUS);

    x=2;
    y=12*(1+selectedPreset-menu->presets.begin());
    display.drawStr(x,y,">");

    display.sendBuffer();
}

void PresetSelector::next(MenuDial menuDial){
    switch(menuDial){
        case UpperKnob:
        case LowerKnob:
            if(selectedPreset < menu->presets.end()-1) {
                selectedPreset++;
                draw(menu->display);
            } else if( selectedPreset == menu->presets.end()){

            }
            break;
        case ItemKnob1:
        case ItemKnob2:
        case ItemKnob3:
            break;
    }
}

void PresetSelector::prev(MenuDial menuDial){
    switch(menuDial){
        case UpperKnob:
        case LowerKnob:
            if(selectedPreset > menu->presets.begin()) {
                selectedPreset--;
                draw(menu->display);
            }
            break;
        case ItemKnob1:
        case ItemKnob2:
        case ItemKnob3:
            break;
    }
}

void PresetSelector::select(MenuDial menuDial){
    switch(menuDial){
        case UpperKnob:
        case LowerKnob:
            Serial.println("SELECTED PRESET");
            menu->currPreset = selectedPreset;
            menu->currentPage = menu->pedalPage;
            menu->draw();
            break;
        case ItemKnob1:
        case ItemKnob2:
        case ItemKnob3:
            break;
    }
}

#pragma endregion PRESET_SELECTOR

#pragma region PRESET_PAGE

PedalPage::PedalPage(DMenu *menu) {
    this->menu = menu;
    Serial.println("CREATE: PEDAL PAGE");}


// PedalPage::setPresets(std::vector<Preset> &_presets){
//     this->presets = _presets;
// }

// PedalPage::setPreset(std::vector<Preset>::iterator &_preset){
//     this->preset = _preset;
//     currPedal = preset.begin();
// }

void PedalPage::set(){
    currPedal = menu->currPreset->pedals.begin();
}

void PedalPage::draw(U8G2 &display){
    Serial.println("DRAW: PEDAL PAGE");
    display.clearBuffer();

    //Draw Title
    display.setFontPosCenter();
    display.drawRFrame(0,0,DISPLAY_WIDTH, TITLE_HEIGHT, EMENU_RADIUS);
    display.setFont(u8g2_font_Pixellari_tf);
    std::string tmp = "P"+std::to_string(menu->currPreset-menu->presets.begin());
    display.drawStr(25, (TITLE_HEIGHT)/2, tmp.c_str());
    // Sub Title
    display.setFont(u8g2_font_5x8_mr);
    display.drawStr(55,(TITLE_HEIGHT)/2+5,currPedal->name);
    display.drawStr(55,(TITLE_HEIGHT)/2-5,menu->currPreset->name);

    //Draw Items
    display.setFont(u8g2_font_5x8_mr);
    byte x = ITEM_PADDING_WIDTH;
    byte y = TITLE_HEIGHT+ITEM_PADDING_HEIGHT;
    for(PedalConfig item : currPedal->configs ) {
        drawPedalConfig(display, x,y, item);
        x+=ITEM_WIDTH+ITEM_PADDING_WIDTH;
    }

    //Draw Page Arrows
    display.setBitmapMode(1);
    y = TITLE_HEIGHT/2 +1;
    if(currPedal < menu->currPreset->pedals.end()-1) display.drawXBM(110, y-10, 20,20, next_arrow_bits);
    if(currPedal > menu->currPreset->pedals.begin()) display.drawXBM(-2, y-10, 20, 20, prev_arrow_bits); 

    if(changedConfig){
        display.drawStr(24, 8, "*");
    }

    //TODO: Draw Item Arrows
    // y=
    // if( < items.end()-1) display.drawXBM(x= 110, y-10, 20,20, next_arrow_bits);
    // if(currItem > items.begin()) display.drawXBM(-2, y-10, 20, 20, prev_arrow_bits); 

    //Draw Selection Box
    // x = 26;
    // display.drawRFrame(x-(EITEM_PIC_SIZE+EITEM_PADDING)/2, y-EMENU_ITEM_HEIGHT/2+2, EITEM_PIC_SIZE+EITEM_PADDING, EMENU_ITEM_HEIGHT, EMENU_RADIUS);

    display.sendBuffer();
}

void PedalPage::drawPedalConfig(U8G2 &display, int x, int y, PedalConfig &config){
    display.drawRFrame(x,y, ITEM_WIDTH, ITEM_HEIGHT, EMENU_RADIUS);
    display.drawStr(x,y+ITEM_HEIGHT-2, config.name);
    char levelChar[10];
    // to_chars(levelChar,  levelChar+4, config.level);
    // Serial.printf("level %d\n", config.level);
    sprintf(levelChar, "%d", config.level);
    // Serial.println(levelChar);
    display.drawStr(x,y+ITEM_HEIGHT/2+5, levelChar);
    display.drawCircle(x+ITEM_WIDTH/2, y+ITEM_HEIGHT/2, 7);
    int x1 = x+ITEM_WIDTH/2;
    int y1 = y+ITEM_HEIGHT/2;
    int x2 = x1-7*std::sin(2*3.14*config.level/16);
    int y2 = y1+7*std::cos(2*3.14*config.level/16);
    display.drawLine(x1,y1, x2,y2);
    // drawGuage(display, x+ITEM_WIDTH/2,y+ITEM_HEIGHT/2, 10, 100, config.level,1, 15, 0, 7);
}

void PedalPage::drawGuage(U8G2 &display, int x, int y, byte radius, byte needlePercent, int value, int min, int max, int s_rad, int e_rad) {
    float n=(radius/100.00)*needlePercent; // calculate needle percent lenght
    for (int a = min; a <= value; a++) {
        float i=((a-min)*(e_rad-s_rad)/(max-min)+s_rad);
        int xp = x+(sin(i) * n);
        int yp = y-(cos(i) * n);
        display.drawLine(x,y,xp,yp);
    }
}

void PedalPage::next(MenuDial menuDial){
    switch(menuDial){
        case UpperKnob:
            if(currPedal < menu->currPreset->pedals.end()-1)currPedal++;
            // menu->draw();
            draw(menu->display);
            break;
        case LowerKnob:
            break;
        case ItemKnob1:
        case ItemKnob2:
        case ItemKnob3:
            Serial.printf("INCREMENT CONFIG[%d]", menuDial-3);
            currPedal->configs[menuDial-3].level++;
            changedConfig=true;
            menu->draw();
            break;
    }
}

void PedalPage::prev(MenuDial menuDial){
    switch(menuDial){
        case UpperKnob:
            if(currPedal > menu->currPreset->pedals.begin()) {
                currPedal--;
                draw(menu->display);
            }
            break;
        case LowerKnob:
            break;
        case ItemKnob1:
        case ItemKnob2:
        case ItemKnob3:
            Serial.printf("DECREMENT CONFIG[%d]", menuDial-3);
            currPedal->configs[menuDial-3].level--;
            changedConfig=true;
            menu->draw();
            break;
    }
}

void PedalPage::select(MenuDial menuDial){
    switch (menuDial) {
    case UpperKnob:
        changedConfig=false;
        menu->draw();
        break;
    }
}


#pragma endregion PRESET_PAGE