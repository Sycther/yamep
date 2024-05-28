#include "pedals.h"

#include <vector>
#include <map>
#include <ArduinoJson.h>
#include <SPI.h>

void PedalConfig::resetLevel(){
    activeLevel = level;
}

void PedalConfig::setPin(bool on){
    digitalWrite(pin, on);
}

#define MCP_WRITE 0b00010000

void PedalConfig::setDigiPot(){ 
    // switch(addr){
    //     case 0:
    //         break;
    //     case 1:
    //     case 2:
    //         SPI.transfer(MCP_WRITE+addr);
    //         SPI.transfer(activeLevel);
    //         break;
    //     case 3:
    //         if(activeLevel>125){
    //             SPI.transfer(MCP_WRITE+1);
    //             SPI.transfer(255);
    //             SPI.transfer(MCP_WRITE+2);
    //             SPI.transfer(activeLevel*2);
    //         }else{
    //             SPI.transfer(MCP_WRITE+1);
    //             SPI.transfer(activeLevel*2);
    //             SPI.transfer(MCP_WRITE+2);
    //             SPI.transfer(0);
    //         }
    //         break;
    // }

    digitalWrite(pin, LOW);
    SPI.transfer(MCP_WRITE + addr);
    Serial.printf("SPI: DIGIPOT: %d,%d:%d\n", pin,addr, level);
    SPI.transfer(activeLevel);
    digitalWrite(pin, HIGH);
}

bool Pedal::isChanged(){
    for(auto& conf : configs){
        if(conf.activeLevel != conf.level) return true;
    }
    return false;
}

// void Pedal::setLevels(std::vector<int> &configs){

// }

//OLD PRESET

void Preset::setLevels(){
    for(auto& pedal: pedals){
        for(auto& config : pedal.configs){
            config.level = config.activeLevel;
        }
    }
}

void Preset::resetLevels(){
    for(auto& pedal: pedals){
        for(auto& conf : pedal.configs){
            conf.activeLevel = conf.level;
        }
    }
}

bool Preset::isChanged(){
    for(auto& pedal: pedals){
        if(pedal.isChanged()) return true;
    }
    return false;
}