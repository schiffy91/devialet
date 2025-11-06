#pragma once
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

enum class IRCommand { None, VolumeUp, VolumeDown, Mute };

class IRReceiver {
private:
  IRrecv _recv;
  decode_results _res;
  bool _active = false;
  unsigned long _lastSeen = 0;
  IRCommand _last = IRCommand::None;
  
  struct IRCodeMapping {
    uint64_t code;
    IRCommand command;
  };
  
  IRCommand decodeCommand(uint64_t value) const {
    // LG TV Remote codes
    if (value == 0x20DF40BFULL) return IRCommand::VolumeUp;
    if (value == 0x20DFC03FULL) return IRCommand::VolumeDown;
    if (value == 0x20DF906FULL) return IRCommand::Mute;
    
    // Samsung TV Remote codes (NEC protocol)
    if (value == 0xE0E0E01FULL) return IRCommand::VolumeUp;
    if (value == 0xE0E0D02FULL) return IRCommand::VolumeDown;
    if (value == 0xE0E0F00FULL) return IRCommand::Mute;
    
    // Samsung TV Remote Alt codes (some models)
    if (value == 0x707E01FEULL) return IRCommand::VolumeUp;
    if (value == 0x707E817EULL) return IRCommand::VolumeDown;
    if (value == 0x707E906FULL) return IRCommand::Mute;
    
    return IRCommand::None;
  }

public:
  IRReceiver(uint8_t pin) : _recv(pin) {}
  
  void begin() { _recv.enableIRIn(); _active = true; }
  bool isActive() const { return _active; }
  bool hadRecentActivity(unsigned long ms = 5000) const {
    return _lastSeen && (millis() - _lastSeen) < ms;
  }
  
  IRCommand checkForCommand(String* debug = nullptr) {
    if (!_recv.decode(&_res)) return IRCommand::None;
    
    _lastSeen = millis();
    
    if (debug) {
      *debug = String(typeToString(_res.decode_type)) + " 0x" + 
               String((unsigned long long)_res.value, HEX);
    }
    
    IRCommand cmd = IRCommand::None;
    
    if (_res.repeat && _last != IRCommand::None) {
      cmd = _last;
    } else {
      cmd = decodeCommand(_res.value);
    }
    
    _recv.resume();
    if (cmd != IRCommand::None) _last = cmd;
    return cmd;
  }
  
  static String commandToString(IRCommand cmd) {
    switch (cmd) {
      case IRCommand::VolumeUp: return "VOL UP";
      case IRCommand::VolumeDown: return "VOL DOWN";
      case IRCommand::Mute: return "MUTE";
      default: return "UNKNOWN";
    }
  }
};
