#include <vector>
#include <U8g2lib.h>
#include "pedals.h"
#include "bitmaps.h"

#ifndef DPAGE_H
#define DPAGE_H

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGTH 64

#define TITLE_HEIGHT 26

#define ITEMS_PER_PAGE 3

#define ITEM_WIDTH 40 //DISPLAY_WIDTH
#define ITEM_HEIGHT 30
#define ITEM_PADDING_WIDTH 2
#define ITEM_PADDING_HEIGHT 6

#define EMENU_RADIUS 3

void test();

// using callable = void (*)();

enum MenuDial {
    UpperKnob,
    LowerKnob,
    ItemKnob1,
    ItemKnob2,
    ItemKnob3
};

// class DItem {
//     // callable *callback;
//     char *name;

//     public:
//     DItem(char *name);
//     draw();
// };

class DMenu;

class DPage {
    // std::vector<Preset> presets;
    // std::vector<Preset>::iterator currPreset;

    DMenu *menu;
    
    public:
    DPage() {}
    DPage(DMenu *menu);

    // virtual void setPreset(std::vector<Preset>::iterator &_preset);
    // virtual void setPresets(std::vector<Preset> &_presets);
    virtual void draw(U8G2 &display) = 0;

    virtual void set();
    virtual void next(MenuDial menuDial = UpperKnob, int speed = 1) = 0;
    virtual void prev(MenuDial menuDial = UpperKnob, int speed = 1) = 0;
    virtual void select(MenuDial menuDial = UpperKnob) = 0;

    // virtual void back();

    // bool operator==(const B& lhs, const B& rhs){
    //     Serial.printf("LHS: %d, RHS: %d\n", lhs, rhs);
    //     return false;
    // }
};

class PresetSelector : public DPage {
    public:
    // std::vector<Preset> presets;
    DPreset preset;
    std::vector<DPreset>::iterator selectedPreset;

    DMenu *menu;

    PresetSelector() {}
    PresetSelector(DMenu *menu);

    // virtual void setPreset(std::vector<Preset>::iterator &_preset);
    // virtual void setPresets(std::vector<Preset> &_presets);
    void draw(U8G2 &display);

    void set();
    void next(MenuDial menuDial = UpperKnob, int speed = 1);
    void prev(MenuDial menuDial = UpperKnob, int speed = 1);
    void select(MenuDial menuDial = UpperKnob);

    // void back();
};

class PedalPage : public DPage {
    public:
    // std::vector<Preset> presets;
    // std::vector<Preset>::iterator currPreset;
    std::vector<Pedal>::iterator currPedal;
    // bool changedConfig = false;

    DMenu *menu;

    PedalPage() {}
    PedalPage(DMenu *menu);

    // virtual void setPreset(std::vector<Preset>::iterator &_preset);
    // virtual void setPresets(std::vector<Preset> &_presets);
    void draw(U8G2 &display);

    void set();
    void next(MenuDial menuDial = UpperKnob, int speed = 1);
    void prev(MenuDial menuDial = UpperKnob, int speed = 1);
    void select(MenuDial menuDial = UpperKnob);
    // void back();

    void drawGuage(U8G2 &display, int x, int y, byte radius, byte needlePercent, int value, int min, int max, int s_rad, int e_rad);
    void drawPedalConfig(U8G2 &display, int x, int y, PedalConfig &config);
};

#endif