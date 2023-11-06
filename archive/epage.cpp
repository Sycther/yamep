#include "epage.h"

#pragma region EITEM

void test(){
    Serial.println("TEST");
}

EItem::EItem(char* title, char* shortTitle,unsigned char* bitmap)
: title(title), shortTitle(shortTitle), bitmap(bitmap), callback(test)
{}

void EItem::runCallback(){
    try{
        callback();
    } catch (int e){
        Serial.printf("EXCEPTION: %d", e);
    }
}

void EItem::DrawItem(U8G2 *display, byte x, byte y){
    display->drawXBM(x-10,y-13,20,20,bitmap);
    display->drawStr(x-display->getStrWidth(shortTitle)/2,y+14, shortTitle);
}

JsonObject EItem::toJson(){
    JsonObject item;
    Serial.println("item");
    item[title] = shortTitle;
    Serial.println("item title");
    return item;
}

#pragma endregion

#pragma region EPAGE

EPage::EPage(char *title) : title(title) {}

EPage::EPage(char* title, std::vector<EItem> items) 
: title(title), items(items), itemSel(items.begin()) {}

void EPage::setIterator(){
    itemSel = items.begin();
}

void EPage::DrawPage(U8G2 *display){
    Serial.println("PRINT EPAGE");
    display->clearBuffer();
    display->setFontPosCenter();
    //Draw Title Area
    // display->drawStr(1,15, "HELLO ");
    display->drawRFrame(0,0,DISPLAY_WIDTH, EPAGE_TITLE_HEIGHT, EMENU_RADIUS);
    display->setFont(u8g2_font_Pixellari_tf);
    display->drawStr(33, (EPAGE_TITLE_HEIGHT)/2, title);

    //Draw Items
    display->setFont(u8g2_font_5x8_mr);

    byte x = 26;
    byte y = DISPLAY_HEIGTH-(DISPLAY_HEIGTH-EPAGE_TITLE_HEIGHT)/2;

    (*itemSel).DrawItem(display, x,y);
    for(int pedalConf : itemSel->config){
        x+=26;
        display->drawXBM(x-10,y-13,20,20,add_bits);
        display->drawStr(x-display->getStrWidth("CNF")/2,y+14, "CNF");
    }

    // byte itemPage = (itemSel - items.begin())/4;
    // for(byte i = 0; i < 4;i++){
    //     byte index = i+itemPage*4;
    //     if(index > items.size()-1) break;
    //     x += 26;
    //     items[index].DrawItem(display, x, y);
    // }

    ///Draw Next Arrow
    // display->setBitmapMode(true);
    // if(items.size()-itemPage*4 > 4) display->drawXBM(x= 110, y-10, 20,20, next_arrow_bits);
    // if(items.size()-itemPage*4 > 4) display->drawXBM(x= 110, y-10, 20,20, next_arrow_bits);

    // if(itemPage>0) display->drawXBM(-2, y-10, 20, 20, prev_arrow_bits); 

    /// Draw Prev Arrow
    // display->

    //draw Select Box
    x = -1 + 26*(1+(itemSel -items.begin())%4);
    display->drawRFrame(x-(EITEM_PIC_SIZE+EITEM_PADDING)/2, y-EMENU_ITEM_HEIGHT/2+2, EITEM_PIC_SIZE+EITEM_PADDING, EMENU_ITEM_HEIGHT, EMENU_RADIUS);
    
    //Send to Display
    display->sendBuffer();
}

void EPage::increaseItemSel(U8G2 *display){
    if(itemSel < items.end()-1) itemSel++;
    DrawPage(display);
}

void EPage::decreaseItemSel(U8G2 *display){
    if(itemSel != items.begin()) itemSel--;
    DrawPage(display);
}

JsonObject EPage::toJson(){
    JsonObject obj;
    Serial.println("preset");
    obj["title"] = title;
    
    Serial.println("title");
    obj["pedals"].add(items[0].toJson());
    Serial.println("obj");
}

#pragma endregion

#pragma region EPAGESEL

EPageSelector::EPageSelector(char *title) 
: EPage(title) {}

void EPageSelector::DrawPage(U8G2 *display) {
    display->clearBuffer();
    Serial.println("PRINT EPAGESELECTOR");
    display->setFont(u8g2_font_5x8_mf);
    display->setFontPosTop();
    
    int x = 10;
    int y = 0;

    display->drawStr(5, y, title);
    for(EPage i : (*presets)) {
        y+= 12;
        display->drawStr(x,y, i.title);
    }

    x = 2;
    y = 12*(1+presetSel-(*presets).begin());
    display->drawStr(x,y,">");

    display->sendBuffer();
}

void EPageSelector::setIterator(){
    presetSel = (*presets).begin();
}

void EPageSelector::setPresets(std::vector<EPage> *presets){
    this->presets = presets;
    presetSel = (*presets).begin();
}

void EPageSelector::increaseItemSel(U8G2 *display){
    if(presetSel < (*presets).end()-1) {
        presetSel++;
        DrawPage(display);
    }
}

void EPageSelector::decreaseItemSel(U8G2 *display){
    if(presetSel != (*presets).begin()) {
        presetSel--;
        DrawPage(display);
    }
}

#pragma endregion EPAGESEL