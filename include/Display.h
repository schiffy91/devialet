#pragma once
#include <M5Unified.h>
#include <vector>
#include "IRReceiver.h"

struct SpeakerDisplayInfo {
  char role;     // 'M', 'L', 'R'
  int volume;
  String ip;
};

class Display {
private:
  unsigned long _timeout = 0;
  unsigned long _lastActivity = 0;
  bool _screenOn = true;
  bool _setupComplete = false;
  static constexpr unsigned long SLEEP_TIMEOUT_MS = 5000;
  
  uint16_t getVolumeColor(int vol) {
    if (vol < 35) return TFT_DARKGREY;
    if (vol < 70) return TFT_ORANGE;
    return TFT_RED;
  }
  
  void ensureScreenOn() {
    if (!_screenOn) {
      M5.Display.wakeup();
      M5.Display.setBrightness(128);
      _screenOn = true;
    }
  }

public:
  void begin() {
    M5.begin();
    M5.Display.setRotation(M5.Display.width() < M5.Display.height() ? 1 : 3);
    M5.Display.fillScreen(BLACK);
    M5.Display.setTextColor(WHITE, BLACK);
    _lastActivity = millis();
  }
  
  void update() { 
    M5.update();
    
    if (_setupComplete && _screenOn && (millis() - _lastActivity) > SLEEP_TIMEOUT_MS) {
      M5.Display.sleep();
      M5.Display.setBrightness(0);
      _screenOn = false;
    }
  }
  
  void markSetupComplete() {
    _setupComplete = true;
    _lastActivity = millis();
  }
  
  void recordActivity() {
    ensureScreenOn();
    _lastActivity = millis();
  }
  
  void showMainScreen(const std::vector<SpeakerDisplayInfo>& speakers) {
    ensureScreenOn();
    M5.Display.fillScreen(BLACK);
    M5.Display.setTextSize(4);
    M5.Display.setCursor(0, 0);
    
    if (speakers.empty()) {
      M5.Display.setTextSize(2);
      M5.Display.println("No speakers");
    } else {
      for (const auto& sp : speakers) {
        M5.Display.setTextColor(getVolumeColor(sp.volume), BLACK);
        M5.Display.print(sp.role);
        M5.Display.print(": ");
        M5.Display.print(sp.volume);
        M5.Display.println("%");
      }
      
      M5.Display.println();
      M5.Display.setTextSize(1);
      M5.Display.setTextColor(TFT_DARKGREY, BLACK);
      M5.Display.print("(");
      for (size_t i = 0; i < speakers.size(); i++) {
        M5.Display.print(speakers[i].ip);
        if (i < speakers.size() - 1) M5.Display.print(", ");
      }
      M5.Display.print(")");
    }
    _timeout = 0;
    _lastActivity = millis();
  }
  
  void showCommand(const String& text, bool unknown = false) {
    ensureScreenOn();
    M5.Display.fillScreen(BLACK);
    M5.Display.setTextSize(2);
    M5.Display.setCursor(0, 20);
    M5.Display.setTextColor(WHITE, BLACK);
    M5.Display.println(text);
    _timeout = millis() + (unknown ? 3500 : 500);
    _lastActivity = millis();
  }
  
  void showMessage(const String& text) {
    ensureScreenOn();
    M5.Display.fillScreen(BLACK);
    M5.Display.setTextSize(1);
    M5.Display.setCursor(0, 0);
    M5.Display.setTextColor(WHITE, BLACK);
    M5.Display.println(text);
    _lastActivity = millis();
  }
  
  bool shouldRestoreMainScreen() { return _timeout && millis() >= _timeout; }
};
