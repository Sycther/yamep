#ifndef PEDAL_H
#define PEDAL_H

#include <vector>
#include <map>
#include <ArduinoJson.h>

enum PedalType {
    Distortion,
    Tremelo,
    Fuzz,
    WAH
};

struct PedalConfig {
    public:
    const char* name;
    int level;
    int pin;
    int addr;

    int activeLevel;

    void setPin(bool on);
    void setDigiPot();
    void resetLevel();
};

struct Pedal {
    const char* name; //Bound to 3 Chars
    std::vector<PedalConfig> configs;

    bool isChanged();
    void setLevel();
};

struct DPreset {
    const char* name;
    std::map<const char*, std::vector<int>> pedal_presets;

    void resetLevels();
    void setLevels();
};

struct Preset {
    const char* name; //Bound to <11 chars
    // std::map<PedalType, PedalConfig> pedals;
    std::vector<Pedal> pedals;

    bool isChanged();

    void resetLevels();
    void setLevels();

    JsonObject toJson();
};

#endif