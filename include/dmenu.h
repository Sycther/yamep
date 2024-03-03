#include <U8g2lib.h>
#include <vector>
#include <SPI.h>
#include "dpage.h"
#include "pedals.h"
#include "bitmaps.h"

#ifndef DMENU_H
#define DMENU_H

// #define DISPLAY_WIDTH 128
// #define DISPLAY_HEIGTH 64

// #define TITLE_HEIGHT 26

// #define ITEMS_PER_PAGE 3

// #define ITEM_WIDTH 40 //DISPLAY_WIDTH
// #define ITEM_HEIGHT 30
// #define ITEM_PADDING_WIDTH 2
// #define ITEM_PADDING_HEIGHT 6

// #define EMENU_RADIUS 3

class DMenu {
    public:
    U8G2 display;
    std::vector<DPreset> presets;
    std::vector<DPreset>::iterator currPreset;
    // DPreset currPreset;
    std::vector<Pedal> pedals;

    // public:
    DPage *currentPage;
    PedalPage *pedalPage;
    PresetSelector *presetsPage;

    DMenu(U8G2 &display);

    bool begin(std::vector<DPreset> &_presets,std::vector<Pedal>& pedals, PresetSelector *_presetsPage, PedalPage *_pedalPage);

    void draw();

    void select(MenuDial menuDial = UpperKnob);
    void setDigiPotLevel(int cs, int header, int level);
    void next(MenuDial menuDial = UpperKnob, int speed = 1);
    void prev(MenuDial menuDial = UpperKnob, int speed = 1);
    void back();

    void setPage(DPage *page);
    void setPreset();
    void saveLevels();
    void resetLevels();
    void setLevel();

    friend class DPage;
    friend class PresetSelector;
    friend class PedalPage;
};

#endif