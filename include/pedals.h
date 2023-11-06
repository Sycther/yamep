#ifndef PEDAL_H
#define PEDAL_H

#include <vector>
#include <map>
#include <ArduinoJson.h>

struct PedalConfig {
    const char* name;
    int level;

    JsonObject toJson();
};

struct Pedal {
    const char* name; //Bound to 3 Chars
    unsigned char *bitmap;
    std::vector<PedalConfig> configs;

    virtual JsonObject toJson();
};

struct Preset {
    const char* name; //Bound to <11 chars
    // std::map<PedalType, PedalConfig> pedals;
    std::vector<Pedal> pedals;

    JsonObject toJson();
};

#endif