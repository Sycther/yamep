#include <vector>
#include <list>
#include <U8g2lib.h>
#include "bitmaps.h"
#include <EEPROM.h>
#include <ArduinoJson.h>

#ifndef EPAGE_H
#define EPAGE_H

// #define PIN_UP 17
// #define PIN_DOWN 16

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGTH 64

#define EPAGE_TITLE_HEIGHT 22

#define EITEM_PIC_SIZE 20
#define EITEM_PADDING 4
#define EITEM_BUFFER 2
#define EMENU_ITEM_HEIGHT 34

#define EMENU_RADIUS 3

class EMenu;

using callable = void (*)();

void test();

// struct Pedal {
//     const char name[3]; //Bound to 3 Chars
//     std::vector<int> configs;
// };

// struct Preset {
//     const char* name; //Bound to <11 chars
//     std::vector<Pedal> pedals;
// };

struct EItem {
    callable callback;
    unsigned char *bitmap;
    int config[3];

    EItem(char* title, char* shortTitle ,unsigned char* bitmap = dst_bits);

    void runCallback();

    void DrawItem(U8G2 *display, byte x, byte y);
    void DrawSelected();

    void enterPage();
    JsonObject toJson();
};

struct EPage {
    char *title;
    std::vector<EItem> items = {
        EItem("Distortion", "DST", dst_bits),
        EItem("Reverb", "RVB", reverb_bits),
        EItem("Fuzz", "FZZ", fuzz_bits),
        EItem("WAH", "WAH", dst_bits)
        // EItem("ADD ITEM", "ADD" , add_bits)
    };

    
    // byte itemSel;
    std::vector<EItem>::iterator itemSel;

    EPage(char* title);
    EPage(char* title, std::vector<EItem> items);

    virtual void DrawPage(U8G2 *display);
    // virtual void runItemCall(EMenu &m);
    virtual void runItemCall(EMenu &m);
    virtual void increaseItemSel(U8G2 *display);
    virtual void decreaseItemSel(U8G2 *display);
    virtual void setIterator();

    JsonObject toJson();
    void addToJsonDoc(JsonObject &obj);
};

struct EPresetPage : EPage{
};

struct EPedalPage : EPage{
};

struct EPageSelector : EPage {
    std::vector<EPage> *presets;
    std::vector<EPage>::iterator presetSel;

    EPageSelector(char *title);

    void setPresets(std::vector<EPage> *presets);

    void DrawPage(U8G2 *display);
    void runItemCall(EMenu &m);
    void increaseItemSel(U8G2 *display);
    void decreaseItemSel(U8G2 *display);
    virtual void setIterator();
};

#endif