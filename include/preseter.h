#include <SPIFFS.h>
#include <EEPROM.h>
#include "pedals.h"
#include <vector>
#include <ArduinoJson.h>

namespace Preseter {

    bool readConfig(std::vector<DPreset> &presets);
    bool saveConfig(std::vector<DPreset> &presets);
}