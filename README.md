# Devialet Speaker IR Controller

Production-ready M5StickC Plus2 remote control for Devialet Phantom speakers.

## Features

- **IR Remote**: Samsung TV remotes or custom IR remotes
- **Physical Buttons**: M5StickC Plus2 volume control
- **Auto-Discovery**: mDNS finds all Devialet speakers
- **Multi-Speaker**: Commands sent to all speakers simultaneously
- **Instant UI**: <50ms response with optimistic updates
- **Mute Toggle**: Press once to mute, twice to restore volume
- **Power Saving**: 5-second auto-sleep
- **Auto-Recovery**: Automatically rediscovers speakers if IP addresses change or connections fail
- **Configurable**: Centralized configuration file for all settings

## Display

```
L: 60%
R: 65%
```

Color-coded: Grey (0-34%), Orange (35-69%), Red (70-100%)

## Hardware

- [M5StickC Plus2](https://www.amazon.com/dp/B0F3XQ22XS) (~$35)
- [IR Receiver](https://www.amazon.com/dp/B087ZRD3LH) (~$5) - TL1838/VS1838B
- [Dupont Wires](https://www.amazon.com/dp/B01EV70C78) (~$5)
- TV Remote (Samsung supported)

**Wiring**: IR OUT→GPIO33, VCC→3.3V, GND→GND

## Setup

1. Install [VS Code](https://code.visualstudio.com/) + [PlatformIO](https://platformio.org/install/ide?install=vscode)
2. Copy `include/Secrets.h.example` → `include/Secrets.h` and add WiFi credentials
3. *(Optional)* Customize settings in `include/Config.h`
4. Connect device via USB
5. `Cmd+Shift+B` to build and upload

## Configuration

All configuration options are centralized in `include/Config.h`:

### Hardware Settings
- **IR_PIN** (33): GPIO pin for IR receiver

### IR Remote Settings
- **IR Codes**: Pre-configured Samsung TV codes + custom codes
- **IR_REPEAT_INTERVAL_MS** (125): Time between repeated volume commands (125ms=fast, 250ms=moderate, 500ms=slow)
- **IR_MUTE_REPEAT_INTERVAL_MS** (500): Time between mute button presses (allows toggle detection)
- **IR_MIN_BITS** (8): Minimum valid IR signal bits (filters noise)

### Display Settings
- **DISPLAY_SLEEP_MS** (5000): Auto-sleep timeout in milliseconds
- **VOLUME_COLOR_LOW_THRESHOLD** (35): Volume % for grey/orange transition
- **VOLUME_COLOR_HIGH_THRESHOLD** (70): Volume % for orange/red transition

### Volume Control
- **VOLUME_INCREMENT** (1): Volume change per button press (%)
- **VOLUME_MIN/MAX** (0/100): Volume limits
- **SYNC_SPEAKER_VOLUMES** (true): Sync all speakers to same volume on commands

### Network Settings
- **WIFI_RECONNECT_INTERVAL_MS** (5000): WiFi retry interval
- **HTTP_TIMEOUT_MS** (300): API request timeout

### Adding Custom Remote Codes
1. Set `DEBUG_LOGGING 1` in `Logger.h`
2. Monitor serial output while pressing remote buttons
3. Note the hex codes displayed
4. Add codes to `IRCodes::Custom` namespace in `Config.h`

## Reliability

### Auto-Recovery
The device automatically handles network issues and IP address changes:

- **Lazy Recovery**: Only triggers rediscovery when needed (after a failure)
- **User-Triggered**: Recovery happens on next button press, not in background
- **Smart Detection**: Activates when any speaker fails to respond
- **Zero Overhead**: No performance impact during normal operation

Perfect for long-running deployments where router reboots or DHCP lease renewals might change speaker IP addresses.

## Architecture

- **Non-blocking**: 300ms HTTP timeout per request
- **Optimistic UI**: Instant display updates, verified after POST
- **Throttled repeats**: Prevents rapid command spam
- **Auto-recovery**: Lazy rediscovery when speakers fail (only on user interaction)
- **Centralized config**: All settings in `Config.h` for easy customization
- **Header-only**: Modular design with no .cpp files

## License

MIT
