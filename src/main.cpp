#include "dmenu.h"
#include "Button.h"
#include <RotaryEncoder.h>
#include <Arduino.h>
#include "preseter.h"

#define PIN_SELECT 23

#define PAGE_DIAL_DT 18
#define PAGE_DIAL_CLK 19
#define ITEM_DT 16
#define ITEM_CLK 17

#define INCREMENT_SPEED 1

#define D1_DT 1
#define D1_CLK 3
#define D2_DT 2
#define D2_CLK 4
#define D3_DT 33
#define D3_CLK 32

#define PIN_BACK 5

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);
DMenu menu(u8g2);

Button sel_btn = Button(PIN_SELECT);
Button back_btn = Button(PIN_BACK);

RotaryEncoder UpperDial = RotaryEncoder(PAGE_DIAL_DT, PAGE_DIAL_CLK, RotaryEncoder::LatchMode::FOUR3);
RotaryEncoder LowerDial = RotaryEncoder(ITEM_DT, ITEM_CLK, RotaryEncoder::LatchMode::FOUR3);
RotaryEncoder Item1 = RotaryEncoder(D1_DT, D1_CLK, RotaryEncoder::LatchMode::FOUR3);
RotaryEncoder Item2 = RotaryEncoder(D2_DT, D2_CLK, RotaryEncoder::LatchMode::FOUR3);
RotaryEncoder Item3 = RotaryEncoder(D3_DT, D3_CLK, RotaryEncoder::LatchMode::FOUR3);

// std::vector<std::pair<MenuDial, RotaryEncoder>> dials = {
//     {UpperKnob, RotaryEncoder(PAGE_DIAL_DT, PAGE_DIAL_CLK, RotaryEncoder::LatchMode::FOUR3)},
//     // {LowerKnob, RotaryEncoder(ITEM_DT, ITEM_CLK, RotaryEncoder::LatchMode::FOUR3)},
//     // {ItemKnob1, RotaryEncoder(D1_DT, D1_CLK, RotaryEncoder::LatchMode::FOUR3)},
//     // {ItemKnob2, RotaryEncoder(D2_DT, D2_CLK, RotaryEncoder::LatchMode::FOUR3)},
//     // {ItemKnob3, RotaryEncoder(D3_DT, D3_CLK, RotaryEncoder::LatchMode::FOUR3)}
// };

// std::vector<Preset> presets;
std::vector<Preset> presets = {
{"Bread Winner",
    {
        {"DST", dst_bits, {
            {"LVL", 5},
            {"GAIN", 15},
            {"SUS", 1}
        }},
        {"Reverb", reverb_bits, {
            {"EFFECT",0},
            {"TONE", 0},
            {"VOL", 0}
        }},
        {"Fuzz", fuzz_bits, {
            {"GAIN", 0},
            {"SUSTAIN", 0},
            {"VOLUME", 0}
        }},
        {"WAH", fuzz_bits, {
            {"LVL", 1},
            {"WOO", 1}
        }}
    }
},{"Smooth Jazz",
    {
        {"DST", dst_bits, {
            {"LVL", 12},
            {"GAIN", 15},
            {"SUS", 12}
        }},
        {"Reverb", reverb_bits, {
            {"EFFECT",2},
            {"TONE", 2},
            {"VOL", 2}
        }},
        {"Fuzz", fuzz_bits, {
            {"GAIN", 2},
            {"SUSTAIN", 3},
            {"VOLUME", 2}
        }},
        {"WAH", fuzz_bits, {
            {"LVL", 11},
            {"WOO", 11}
        }}
    }
}};

PresetSelector presetsPage(&menu);
PedalPage pedalPage(&menu);

void setup(){
    Serial.begin(115200);
    Serial.println("START");

    sel_btn.begin();
    back_btn.begin();

    Preseter::saveConfig(presets);
    // Preseter::readConfig(presets);

    menu.begin(presets, &presetsPage, &pedalPage);
}

void dialHandler(RotaryEncoder &dial, MenuDial menuDial){
    dial.tick();
    switch(dial.getDirection()){
        case RotaryEncoder::Direction::CLOCKWISE:
            Serial.printf("NEXT: %d\n", menuDial);
            menu.currentPage->next(menuDial);
            break;
        case RotaryEncoder::Direction::COUNTERCLOCKWISE:
            Serial.printf("PREV: %d\n", menuDial);
            menu.currentPage->prev(menuDial);
            break;
    }
}

void loop(){

    if(sel_btn.pressed()){
        Serial.println("SELECT");
        menu.currentPage->select();
    }
    if(back_btn.pressed()){
        Serial.println("BACK");
        menu.back();
    }

    dialHandler(UpperDial, UpperKnob);
    dialHandler(LowerDial, LowerKnob);
    dialHandler(Item1, ItemKnob1);
    dialHandler(Item2, ItemKnob2);
    dialHandler(Item3, ItemKnob3);

    // for(auto dial : dials) {
    //     dial.second.tick();
    //     switch(dial.second.getDirection()){
    //         case RotaryEncoder::Direction::CLOCKWISE:
    //             Serial.printf("NEXT: %d\n", dial.first);
    //             menu.currentPage->next(dial.first);
    //             break;
    //         case RotaryEncoder::Direction::COUNTERCLOCKWISE:
    //             Serial.printf("PREV: %d\n", dial.first);
    //             menu.currentPage->prev(dial.first);
    //             break;
    //     }
    // }
}