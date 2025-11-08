#pragma once

// =============================================================================
// HARDWARE PINS
// =============================================================================

constexpr uint8_t IR_PIN = 33;  // GPIO pin for IR receiver (TL1838/VS1838B)

// =============================================================================
// IR REMOTE CODES
// =============================================================================

// Samsung TV remote codes
namespace IRCodes {
  namespace Samsung {
    static constexpr uint32_t VOL_UP   = 0xE0E0E01F;
    static constexpr uint32_t VOL_DOWN = 0xE0E0D02F;
    static constexpr uint32_t MUTE     = 0xE0E0F00F;
  }
  
  // Add custom remote codes here (use DEBUG_LOGGING to discover codes)
  namespace Custom {
    static constexpr uint64_t VOL_UP   = 0x0;
    static constexpr uint64_t VOL_DOWN = 0x0;
    static constexpr uint64_t MUTE     = 0x0;
  }
}

// =============================================================================
// IR RECEIVER SETTINGS
// =============================================================================

// Minimum time between repeated IR commands (prevents spam)
// 125ms = fast, 250ms = moderate, 500ms = slow
constexpr unsigned long IR_REPEAT_INTERVAL_MS = 125;

// Minimum time between mute button presses (allows toggle detection)
constexpr unsigned long IR_MUTE_REPEAT_INTERVAL_MS = 500;

// Minimum number of bits required for valid IR signal (filters noise)
constexpr int IR_MIN_BITS = 8;

// =============================================================================
// DISPLAY SETTINGS
// =============================================================================

// Auto-sleep timeout after last user interaction
constexpr unsigned long DISPLAY_SLEEP_MS = 5000;

// Volume color thresholds (percentage)
constexpr int VOLUME_COLOR_LOW_THRESHOLD = 35;   // Below this: Grey
constexpr int VOLUME_COLOR_HIGH_THRESHOLD = 70;  // Above this: Red, between: Orange

// =============================================================================
// VOLUME CONTROL
// =============================================================================

// Volume change per button press (percentage)
constexpr int VOLUME_INCREMENT = 1;

// Volume limits
constexpr int VOLUME_MIN = 0;
constexpr int VOLUME_MAX = 100;

// Sync volume across all speakers (true = all speakers set to same volume)
constexpr bool SYNC_SPEAKER_VOLUMES = true;

// =============================================================================
// NETWORK SETTINGS
// =============================================================================

// Time between WiFi reconnection attempts
constexpr unsigned long WIFI_RECONNECT_INTERVAL_MS = 5000;

// HTTP request timeout for Devialet API calls
constexpr int HTTP_TIMEOUT_MS = 300;
