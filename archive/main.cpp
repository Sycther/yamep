#include "emenu.h"
#include "bitmaps.h"
#include <SPIFFS.h>
#include <ArduinoJson.h>


U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);
EMenu menu(&u8g2);

std::vector<EPage> presets = {
  EPage("Punk Rock"),
  EPage("Mellow Jazz"),
  EPage("HARD JAZZ")
};

const char *FILE_NAME = "/presets.json";

bool saveConfig(){
    File file = SPIFFS.open(FILE_NAME, FILE_WRITE);

    StaticJsonDocument<1024> doc;
    Serial.println("doc");

    serializeJsonPretty(doc, Serial);
    Serial.println("serial");
    file.close();
    return true;
}

bool readConfig(){
  File file = SPIFFS.open(FILE_NAME);
  if(!file) return false;
  String content = "";
  while(file.available()){
    content = file.readString();
  }
  file.close();
  StaticJsonDocument<1024> doc;
  auto error = deserializeJson(doc, content);
  if(error) {
    Serial.println("ERROR interpreting config file");
    return false;
  }

  Serial.println(content);
}

void setup(){
  Serial.begin(115200);
  Serial.println("START");
  
  if(!SPIFFS.begin(true)){
    Serial.println("ERROR: COULD NOT MOUNT SPIFFS");
    return;
  }

  if(saveConfig()){
    Serial.println("Spiffs something did it");
  }

  menu.begin(presets);
}

void loop(){

  menu.HandleEvents();
}