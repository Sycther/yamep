#include <SPIFFS.h>
#include <EEPROM.h>
#include "pedals.h"
#include <vector>
#include <ArduinoJson.h>

namespace Preseter {

    void readConfig(std::vector<Preset> &presets);
    void saveConfig(std::vector<Preset> &presets);
}