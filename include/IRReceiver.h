#pragma once

#define DECODE_SAMSUNG 1

#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include "Config.h"
#include "Logger.h"

enum class IRCommand { None, VolumeUp, VolumeDown, Mute };

class IRReceiver {
private:
  
  IRrecv _recv;
  decode_results _res;
  IRCommand _last = IRCommand::None;
  unsigned long _lastCommandTime = 0;
  
  IRCommand decode(uint64_t val) const {
    if (val == IRCodes::Samsung::VOL_UP || val == IRCodes::Custom::VOL_UP) return IRCommand::VolumeUp;
    if (val == IRCodes::Samsung::VOL_DOWN || val == IRCodes::Custom::VOL_DOWN) return IRCommand::VolumeDown;
    if (val == IRCodes::Samsung::MUTE || val == IRCodes::Custom::MUTE) return IRCommand::Mute;
    return IRCommand::None;
  }

public:
  IRReceiver(uint8_t pin) : _recv(pin) {}
  
  void begin() { _recv.enableIRIn(); }
  
  IRCommand check() {
    if (!_recv.decode(&_res)) return IRCommand::None;
    
    if (_res.value == 0xFFFFFFFF || _res.value == 0 || _res.bits < IR_MIN_BITS) {
      _recv.resume();
      return IRCommand::None;
    }
    
    Logger::debugIR(typeToString(_res.decode_type), _res.value, _res.bits, _res.repeat);
    
    IRCommand cmd = decode(_res.value);
    if (cmd != IRCommand::None) {
      unsigned long now = millis();
      unsigned long interval = (cmd == IRCommand::Mute) ? IR_MUTE_REPEAT_INTERVAL_MS : IR_REPEAT_INTERVAL_MS;
      if (cmd == _last && now - _lastCommandTime < interval) {
        _recv.resume();
        return IRCommand::None;
      }
      Logger::command(toString(cmd));
      _last = cmd;
      _lastCommandTime = now;
    }
    
    _recv.resume();
    return cmd;
  }
  
  static String toString(IRCommand cmd) {
    switch (cmd) {
      case IRCommand::VolumeUp: return "VOL UP";
      case IRCommand::VolumeDown: return "VOL DOWN";
      case IRCommand::Mute: return "MUTE";
      default: return "UNKNOWN";
    }
  }
};
