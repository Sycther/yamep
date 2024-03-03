#include "preseter.h"
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "bitmaps.h"

#define FILE_NAME "/pedals.json"

#define JSON_SIZE 4096

bool Preseter::saveConfig(std::vector<DPreset> &presets) {
    File file = SPIFFS.open(FILE_NAME, FILE_WRITE);
    if(!file) {
        Serial.println("FAILED TO OPEN FILE");
        return false;
    }
    StaticJsonDocument<JSON_SIZE> doc;

    JsonArray json_presets = doc.createNestedArray("presets");
    for(DPreset preset : presets){
        JsonObject preset_obj = json_presets.createNestedObject();
        preset_obj["name"] = preset.name;
        JsonObject preset_confs = preset_obj.createNestedObject("configs");
        for(auto pedal : preset.pedal_presets){
            JsonArray preset_confs_obj = preset_confs.createNestedArray(pedal.first);
            for(auto i : pedal.second){
                preset_confs_obj.add(i);
            }
        }
    }

    

    Serial.printf("TOTAL MEMORY: %d\n", doc.memoryUsage());

    serializeJsonPretty(doc, file);
    file.close();
    return true;
}

bool Preseter::readConfig(std::vector<DPreset> &presets){
    File file = SPIFFS.open(FILE_NAME, FILE_READ);
    if(!file) {
        Serial.println("FAILED TO OPEN FILE");
        return false;
    }

    StaticJsonDocument<JSON_SIZE> doc;

    DeserializationError error = deserializeJson(doc, file);
    if(error){
        Serial.println("FAILED TO DESERIALIZE FILE");
        return false;
    }

    file.close();

    // JsonArray presestsArray = doc["presets"];
    // for(int i = 0; i < presestsArray.size(); i++){
    //     Preset preset = {presestsArray[i]["name"], {}};
    //     // presets[i].name = presestsArray[i]["name"];
    //     JsonArray pedal_array = presestsArray[i]["configs"];
    //     for(int j=0; j<pedal_array.size(); j++){
    //         Pedal pedal = {pedal_array[j]["name"], {}};
    //         // presets[i].pedals[j].name = pedal_array[j]["name"];
    //         JsonArray config_array = pedal_array[j]["configs"];
    //         for(int k=0; k<config_array.size();k++){
    //             PedalConfig config = {config_array[k]["name"], config_array[k]["level"]};
    //             // presets[i].pedals[j].configs[k].name = config_array[k]["name"];
    //             // presets[i].pedals[j].configs[k].level = config_array[k]["level"];
    //             pedal.configs.push_back(config);
    //         }
    //         preset.pedals.push_back(pedal);
    //     }
    //     presets.push_back(preset);
    // }

    // JsonArray json_presets = doc["presets"];
    // for(DPreset preset : presets){
    //     JsonObject preset_obj = json_presets.createNestedObject();
    //     preset_obj["name"] = preset.name;
    //     JsonObject preset_confs = preset_obj.createNestedObject("configs");
    //     for(auto pedal : preset.pedal_presets){
    //         JsonArray preset_confs_obj = preset_confs.createNestedArray(pedal.first);
    //         for(auto i : pedal.second){
    //             preset_confs_obj.add(i);
    //         }
    //     }
    // }

    return true;
}