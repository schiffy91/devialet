#pragma once
#include <Arduino.h>

#define DEBUG_LOGGING 1  // Set to 0 for production

class Logger {
public:
  static void begin(unsigned long baudRate = 115200) {
    Serial.begin(baudRate);
  }
  
  // Always-on logging (errors, critical info)
  template<typename... Args>
  static void log(Args... args) {
    Serial.print(args...);
  }
  
  template<typename... Args>
  static void logln(Args... args) {
    Serial.println(args...);
  }
  
  template<typename... Args>
  static void logf(const char* format, Args... args) {
    Serial.printf(format, args...);
  }
  

  static void debugHttpGet(const String& url) {
    #if DEBUG_LOGGING
      Serial.printf("→ GET %s\n", url.c_str());
    #endif
  }
  
  static void debugHttpPost(const String& url, const String& body) {
    #if DEBUG_LOGGING
      Serial.printf("→ POST %s\n  Body: %s\n", url.c_str(), body.c_str());
    #endif
  }
  
  static void debugHttpResponse(int code, const String& body) {
    #if DEBUG_LOGGING
      Serial.printf("← HTTP %d\n  Response: %s\n", code, body.c_str());
    #endif
  }
  
  static void debugIR(const String& protocol, uint64_t value, int bits, bool repeat) {
    #if DEBUG_LOGGING
      Serial.printf("◉ IR: %s 0x%llX (%d bits)%s\n", 
                    protocol.c_str(), (unsigned long long)value, bits, 
                    repeat ? " [REPEAT]" : "");
    #endif
  }

  static void info(const String& msg) { Serial.printf("✓ %s\n", msg.c_str()); }
  static void error(const String& msg) { Serial.printf("✗ %s\n", msg.c_str()); }
  static void command(const String& cmd) { Serial.printf("→ %s\n", cmd.c_str()); }
};
