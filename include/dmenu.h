#include <U8g2lib.h>
#include <vector>
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
    std::vector<Preset> presets;
    std::vector<Preset>::iterator currPreset;

    // public:
    DPage *currentPage;
    PedalPage *pedalPage;
    PresetSelector *presetsPage;

    DMenu(U8G2 &display);

    void begin(std::vector<Preset> &_presets, PresetSelector *_presetsPage, PedalPage *_pedalPage);

    void draw();

    void next();
    void prev();
    void back();

    friend class DPage;
    friend class PresetSelector;
    friend class PedalPage;
};

#endif