#include <U8g2lib.h>
#include <vector>
#include <list>
#include "bitmaps.h"
#include "Button.h"
#include <RotaryEncoder.h>
#include "epage.h"

#ifndef EMENU_H
#define EMENU_H

#define PIN_SELECT 23
#define PIN_NEXT 18
#define PIN_PREV 19
#define PIN_BACK 16


class EMenu {
    public:
    U8G2 *display;
    std::vector<EPage> presets;
    std::vector<EPage>::iterator presetSel;
    EPageSelector selectorPage = EPageSelector("Presets");
    std::vector<EPage*> pageStack;

    Button sel_btn = Button(PIN_SELECT);
    Button back_btn = Button(D1_DT);
    RotaryEncoder encoder = RotaryEncoder(PIN_NEXT, D1_CLK, RotaryEncoder::LatchMode::FOUR3);

    EMenu(U8G2 *u8g2);

    void begin(std::vector<EPage> &presets);
    void addPageStack(EPage *page);
    void drawPage();
    void backPage();

    void HandleEvents();
};

#endif