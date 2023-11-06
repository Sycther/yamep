#include "pedals.h"

#include <vector>
#include <map>
#include <ArduinoJson.h>

JsonObject PedalConfig::toJson(){
    JsonObject obj;
    obj["name"] = name;
    obj["level"] = level;
    return obj;
}

JsonObject Pedal::toJson(){
    JsonObject obj;
    obj["name"] = name;
    JsonArray arry = obj.createNestedArray("configs");
    for(PedalConfig config : configs){
        arry.add(config.toJson());
    }
    return obj;
}

JsonObject Preset::toJson(){
    JsonObject obj;
    obj["name"] = name;
    JsonArray arry = obj.createNestedArray("pedals");
    for(Pedal p : pedals){
        arry.add(p.toJson());
    }
    return obj;
}


