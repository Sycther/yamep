#include "dmenu.h"
#include "main.h"
#include "Button.h"
#include "PCF_Button.h"
#include "RotaryEncoder.h"
#include <Arduino.h>
#include "preseter.h"
#include "PCF8574.h"

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);
DMenu menu(u8g2);

PCF8574 pcf38(0x38);
PCF8574 pcf3d(0x3D);

PCF_Button UD_btn = PCF_Button(pcf3d, 3);
// PCF_Button LD_btn = PCF_Button(LD_SELECT);
PCF_Button back_btn = PCF_Button(pcf3d, 4);

PCF_Button IK1_btn = PCF_Button(pcf3d, 0);
PCF_Button IK2_btn = PCF_Button(pcf3d, 1);
PCF_Button IK3_btn = PCF_Button(pcf3d, 2);

RotaryEncoder UpperDial = RotaryEncoder(pcf38, 6,7, RotaryEncoder::LatchMode::FOUR3);
// RotaryEncoder LowerDial = RotaryEncoder(LD_DT, LD_CLK, RotaryEncoder::LatchMode::FOUR3);
RotaryEncoder Item1 = RotaryEncoder(pcf38, 1,0, RotaryEncoder::LatchMode::FOUR3);
RotaryEncoder Item2 = RotaryEncoder(pcf38, 3,2, RotaryEncoder::LatchMode::FOUR3);
RotaryEncoder Item3 = RotaryEncoder(pcf38, 5,4, RotaryEncoder::LatchMode::FOUR3);

std::vector<Pedal> pedals = {
    {"Distortion", {
        {"LVL", 5, 13,3},
        {"GAIN", 15, 27,3},
        {"SUS", 80, 14,3}
    }},
    {"Reverb", {
        {"EFFECT",0, 5,3},
        {"TONE", 80, 5,3},
        {"VOL", 0, 5,3}
    }},
    {"Fuzz", {
        {"GAIN", 0, 5,3},
        {"SUSTAIN", 0, 5,3},
        {"VOLUME", 0, 5,3}
    }},
    {"WAH", {
        {"LVL", 1, 5,3},
        {"WOO", 1, 5,3}
    }}
};

// std::vector<DPreset> dpresets;

std::vector<DPreset> dpresets = {
    { "Bread Winner 2",{
        {"Distortion", {0,50,0}},
        {"Reverb", {80, 0, 80}},
        {"Fuzz", {10, 20, 40}},
        {"WAH", {1,1}}
    }},
    { "Surface Tension",{
        {"Distortion", {100,10,20}},
        {"Reverb", {8, 0, 200}},
        {"Fuzz", {40, 20, 10}},
        {"WAH", {1,1}}
    }},
    { "Smooth Jazz",{
        {"Distortion", {100,10,200}},
        {"Reverb", {8, 0, 200}},
        {"Fuzz", {40, 20, 10}},
        {"WAH", {1,1}}
    }}
};

// std::vector<Preset> presets = {
// {"Bread Winner",
//     {
//         {"DST",  {
//             {"LVL", 5},
//             {"GAIN", 15},
//             {"SUS", 1}
//         }},
//         {"Reverb", {
//             {"EFFECT",0},
//             {"TONE", 0},
//             {"VOL", 0}
//         }},
//         {"Fuzz", {
//             {"GAIN", 0},
//             {"SUSTAIN", 0},
//             {"VOLUME", 0}
//         }},
//         {"WAH", {
//             {"LVL", 1},
//             {"WOO", 1}
//         }}
//     }
// },{"Smooth Jazz",
//     {
//         {"DST", {
//             {"LVL", 144},
//             {"GAIN", 15},
//             {"SUS", 144}
//         }},
//         {"Reverb", {
//             {"EFFECT",2},
//             {"TONE", 2},
//             {"VOL", 2}
//         }},
//         {"Fuzz", {
//             {"GAIN", 2},
//             {"SUSTAIN", 3},
//             {"VOLUME", 2}
//         }},
//         {"WAH", {
//             {"LVL", 11},
//             {"WOO", 11}
//         }}
//     }
// }};

PresetSelector presetsPage(&menu);
PedalPage pedalPage(&menu);

void setup(){
    Serial.begin(115200);
    Serial.println("START");
    
    Serial.print("MOSI: ");
    Serial.println(MOSI);
    Serial.print("MISO: ");
    Serial.println(MISO);
    Serial.print("SCK: ");
    Serial.println(SCK);
    Serial.print("SS: ");
    Serial.println(SS);  

    Serial.println("BEGIN: ");
    
    // dials = {
    //     {UpperKnob, PCF_Button(pcf3d, 3),RotaryEncoder(pcf38, 6,7, RotaryEncoder::LatchMode::FOUR3)}
    // };

    
    Serial.println("!- PCF I/O EXTENDER");
    pcf38.begin();

    // Serial.println("|- ROTARY ENCODER");
    // IK1_btn.begin();
    // IK2_btn.begin();
    // IK3_btn.begin();


    
    Serial.println("|- SPIFFS");
    if(!SPIFFS.begin(true)){
        Serial.println("COULD NOT START SPIFFS");
    }

    // Preseter::saveConfig(presets);
    // Serial.println("|- PRESETS");
    // if(!Preseter::readConfig(dpresets)){
    //     Serial.println("!! ERROR: READ FAILED !!");
    //     esp_deep_sleep_start();
    // }

    Serial.println("|- MENU");
    if(!menu.begin(dpresets, pedals, &presetsPage, &pedalPage)){
        Serial.println("!! ERROR: COULD NOT START MENU");
    }
    
    Serial.println("FINISHED INITIALIZATION");
}

void dialHandler(RotaryEncoder &dial,PCF_Button &btn, MenuDial menuDial){
    if(btn.pressed()){
        // Serial.printf("SELECT: %d", menuDial);
        // menu.currentPage->select(menuDial);
        menu.select(menuDial);
    }
    dial.tick();
    // if(menuDial == UpperKnob) Serial.printf("POS: %d\n", dial.getPosition());
    switch(dial.getDirection()){
        case RotaryEncoder::Direction::CLOCKWISE:
            // Serial.printf("NEXT: %d, RPM: %d\n", menuDial, dial.getRPM());
            // menu.currentPage->next(menuDial, std::max(1, (int)(dial.getRPM()/7)^3));
            menu.next(menuDial, std::max(1, (int)(dial.getRPM()/7)^3));
            break;
        case RotaryEncoder::Direction::COUNTERCLOCKWISE:
            // Serial.printf("PREV: %d, RPM: %d\n", menuDial, dial.getRPM());
            // menu.currentPage->prev(menuDial, std::max(1, (int)(dial.getRPM()/7)^3));
            menu.prev(menuDial, std::max(1, (int)(dial.getRPM()/7)^3));
            break;
    }
}

void loop(){

    if(back_btn.pressed()){
        menu.back();
    }

    // Serial.printf("PCF 38-0,1: %d, %d\n",pcf38.read(0), pcf38.read(1));

    dialHandler(UpperDial, UD_btn, UpperKnob);
    // dialHandler(LowerDial,LD_btn, LowerKnob);
    dialHandler(Item1, IK1_btn, ItemKnob1);
    dialHandler(Item2, IK2_btn, ItemKnob2);
    dialHandler(Item3, IK3_btn, ItemKnob3);

    // for(auto dial : dials){
    //     // dialSelect(dial.btn, dial.menuDial);
    //     dialHandler(dial.re, dial.btn, dial.menuDial);
    // }
}