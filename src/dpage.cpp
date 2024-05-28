#include "dpage.h"
#include "dmenu.h"
#include <string>
#include <SPI.h>
#include "EEPROM.h"

#define CONFIG_MAX_VALUE 255

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
    selectedPreset = menu->currPreset;
}

#define PS_ITEM_HEIGHT 15

void PresetSelector::draw(U8G2 &display){
    Serial.println("|- PRESET PAGE");
    display.clearBuffer();

    display.setFont(u8g2_font_5x8_mf);
    display.setFontPosTop();

    int x = 12;
    int y = 0;
    for(auto pre = menu->presets.begin(); pre != menu->presets.end(); pre++){
        std::string tmp = "P"+std::to_string(pre-menu->presets.begin());
        display.drawStr(x,y,tmp.c_str());
        display.drawStr(x+20, y,pre->name);
        y+=12;
    }
    x = ITEM_PADDING_WIDTH;
    y = DISPLAY_HEIGTH-TITLE_HEIGHT;
    // for(PedalConfig item : currPedal->configs ) {
    //     drawPedalConfig(display, x,y, item);
    //     x+=ITEM_WIDTH+ITEM_PADDING_WIDTH;
    // }
    display.drawRFrame(x,y,ITEM_WIDTH, TITLE_HEIGHT, EMENU_RADIUS);
    display.drawXBM(x+(ITEM_WIDTH-20)/2, y+(TITLE_HEIGHT-20)/2,20, 20, shuffle_bits);
    x+=ITEM_WIDTH+ITEM_PADDING_WIDTH;
    display.drawRFrame(x,y,ITEM_WIDTH, TITLE_HEIGHT, EMENU_RADIUS);
    x+=ITEM_WIDTH+ITEM_PADDING_WIDTH;
    display.drawRFrame(x,y,ITEM_WIDTH, TITLE_HEIGHT, EMENU_RADIUS);


    x=2;
    y=12*(selectedPreset-menu->presets.begin());
    display.drawStr(x,y,">");

    display.sendBuffer();
}

void PresetSelector::next(MenuDial menuDial, int speed){
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

void PresetSelector::prev(MenuDial menuDial, int speed){
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
            Serial.println("|- SELECTED PRESET");
            menu->currPreset = selectedPreset;
            menu->setPreset();
            menu->setPage(menu->pedalPage);
            EEPROM.write(0, (byte) (selectedPreset-menu->presets.begin()));
            EEPROM.commit();
            Serial.printf("EEPROM: %d\n", (byte) (selectedPreset-menu->presets.begin()));
            break;
        case ItemKnob1:
        case ItemKnob2:
        case ItemKnob3:
            Serial.println("|- SELECTED PRESET MENU OPTION");
            break;
    }
}

#pragma endregion PRESET_SELECTOR

#pragma region PRESET_PAGE

PedalPage::PedalPage(DMenu *menu) {
    this->menu = menu;
    Serial.println("CREATE: PEDAL PAGE");
}

void PedalPage::set(){
    currPedal = menu->pedals.begin();
    menu->resetLevels();
}

void PedalPage::draw(U8G2 &display){
    Serial.println("|- PEDAL PAGE");
    display.clearBuffer();

    //Draw Title
    Serial.println("DRAW: Title");
    display.setFontPosCenter();
    display.drawRFrame(0,0,DISPLAY_WIDTH, TITLE_HEIGHT, EMENU_RADIUS);
    display.setFont(u8g2_font_Pixellari_tf);
    std::string tmp = "P"+std::to_string(menu->currPreset-menu->presets.begin());
    display.drawStr(25, (TITLE_HEIGHT)/2, tmp.c_str());
    // if(menu->currPreset->isChanged())
    //     display.drawStr(18, (TITLE_HEIGHT/2),"*");
    // Sub Title
    Serial.println("DRAW: SubTitle");
    display.setFont(u8g2_font_5x8_mr);
    display.drawStr(55,(TITLE_HEIGHT)/2+5,currPedal->name);
    display.drawStr(55,(TITLE_HEIGHT)/2-5,menu->currPreset->name);
    if(currPedal->name)
        display.drawStr(50, (TITLE_HEIGHT)/2+5, "*");

    //Draw Items
    Serial.println("DRAW: Items");
    display.setFont(u8g2_font_5x8_mr);
    byte x = ITEM_PADDING_WIDTH;
    byte y = TITLE_HEIGHT+ITEM_PADDING_HEIGHT;
    for(auto &item : currPedal->configs ) {
        drawPedalConfig(display, x,y, item);
        x+=ITEM_WIDTH+ITEM_PADDING_WIDTH;
    }

    //Draw Page Arrows
    display.setBitmapMode(1);
    y = TITLE_HEIGHT/2 +1;
    if(currPedal < menu->pedals.end()-1) display.drawXBM(110, y-10, 20,20, next_arrow_bits);
    if(currPedal > menu->pedals.begin()) display.drawXBM(-2, y-10, 20, 20, prev_arrow_bits);

    //TODO: Draw Item Arrows
    // y=
    // if( < items.end()-1) display.drawXBM(x= 110, y-10, 20,20, next_arrow_bits);
    // if(currItem > items.begin()) display.drawXBM(-2, y-10, 20, 20, prev_arrow_bits); 

    //Draw Selection Box
    // x = 26;
    // display.drawRFrame(x-(EITEM_PIC_SIZE+EITEM_PADDING)/2, y-EMENU_ITEM_HEIGHT/2+2, EITEM_PIC_SIZE+EITEM_PADDING, EMENU_ITEM_HEIGHT, EMENU_RADIUS);

    display.sendBuffer();
}

const float dir_x = std::sin(2*3.14*.1);
const float dir_y = std::cos(2*3.14*.1);

void PedalPage::drawPedalConfig(U8G2 &display, int x, int y, PedalConfig &config){
    
    Serial.println("DRAW: Config");
    display.drawRFrame(x,y, ITEM_WIDTH, ITEM_HEIGHT, EMENU_RADIUS);

    display.drawStr(x,y+ITEM_HEIGHT-2, config.name);
    char levelChar[10];

    display.drawCircle(x+ITEM_WIDTH/2, y+ITEM_HEIGHT/2, 7);

    int center_x = x+ITEM_WIDTH/2;
    int center_y = y+ITEM_HEIGHT/2;
    int level_x = center_x-7*std::sin(2*3.14*(.8*config.activeLevel/CONFIG_MAX_VALUE+.1));
    int level_y = center_y+7*std::cos(2*3.14*(.8*config.activeLevel/CONFIG_MAX_VALUE+.1));
    display.drawLine(center_x, center_y, level_x, level_y);

    display.drawLine(center_x-4*dir_x,center_y+4*dir_y, center_x-7*dir_x, center_y+7*dir_y);
    display.drawLine(center_x+4*dir_x,center_y+4*dir_y, center_x+7*dir_x, center_y+7*dir_y);
}

void PedalPage::next(MenuDial menuDial, int speed){
    switch(menuDial){
        case UpperKnob:
            if(currPedal < menu->pedals.end()-1){
                currPedal++;
            }
            // menu->draw();
            draw(menu->display);
            break;
        case LowerKnob:
            break;
        case ItemKnob1:
        case ItemKnob2:
        case ItemKnob3:
            // Serial.printf("ACTIVE VALUE: %d\n", currPedal->configs[menuDial-ItemKnob1].activeLevel);
            PedalConfig& currConfig = currPedal->configs[menuDial-ItemKnob1];
            if(currConfig.activeLevel < CONFIG_MAX_VALUE){
                currConfig.activeLevel = std::min(currConfig.activeLevel+speed, CONFIG_MAX_VALUE);
                currConfig.setDigiPot();
                // menu->setDigiPotLevel(currConfig.pin, currConfig.header, currConfig.activeLevel);
                menu->draw();
            }
            break;
    }
}

void PedalPage::prev(MenuDial menuDial, int speed){
    switch(menuDial){
        case UpperKnob:
            if(currPedal > menu->pedals.begin()) {
                currPedal--;
                draw(menu->display);
            }
            break;
        case LowerKnob:
            break;
        case ItemKnob1:
        case ItemKnob2:
        case ItemKnob3:
            Serial.printf("ACTIVE VALUE: %d\n", currPedal->configs[menuDial-ItemKnob1].activeLevel);
            PedalConfig& currConfig = currPedal->configs[menuDial-ItemKnob1];
            if(currConfig.activeLevel > 0){
                currConfig.activeLevel = std::max(currConfig.activeLevel-speed, 0);
                currConfig.setDigiPot();
                menu->draw();
                // menu->setDigiPotLevel(currConfig.pin, currConfig.header, currConfig.activeLevel);

            }
            break;
    }
}

void PedalPage::select(MenuDial menuDial){
    switch (menuDial) {
        case UpperKnob:
            menu->saveLevels();
            menu->draw();
            break;
        case LowerKnob:
            break;
        case ItemKnob1:
        case ItemKnob2:
        case ItemKnob3:
            currPedal->configs[menuDial-2].resetLevel();
            menu->draw();
            break;
    }
}


#pragma endregion PRESET_PAGE