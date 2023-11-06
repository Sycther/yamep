#include "preseter.h"
#include <SPIFFS.h>

#define FILE_NAME "/presets.json"

void Preseter::saveConfig(std::vector<Preset> &presets){
    StaticJsonDocument<1024> doc;

    doc.add(presets[0].toJson());

    serializeJsonPretty(doc, Serial);
}

void Preseter::readConfig(std::vector<Preset> &presets){
    File file = SPIFFS.open(FILE_NAME, FILE_READ);

    StaticJsonDocument<1024> doc;

    DeserializationError error = deserializeJson(doc, file);
    if(error){
        Serial.println("FAILED TO DESERIALIZE FILE");
    }

    file.close();

    JsonArray presestsArray = doc["presets"];

    for(int i = 0; i<presestsArray.size(); i++){
        Serial.printf("JSON OBJ NAME: %s", presestsArray[i]["name"]);
    }
}