# Devialet Speaker IR Controller

M5StickC Plus2 remote control for Devialet Phantom speakers via IR and WiFi.

## Features

- **IR Remote Control**: Works with LG and Samsung TV remotes (volume up/down, mute)
- **Physical Buttons**: Volume control via M5StickC buttons
- **Auto-Discovery**: Finds Devialet speakers via mDNS at startup
- **Synchronized Control**: Commands are sent to all discovered speakers
- **Instant Feedback**: Color-coded volume display updates immediately
- **Power Saving**: Screen turns off after 5 seconds of inactivity, wakes on any button/IR input
- **No Polling**: Only queries speakers at startup to avoid network spam

## Display

Large, easy-to-read format showing speaker volumes:

```
L: 50%
R: 55%

(192.168.1.10, 192.168.1.11)
```

- **L**: Left speaker
- **R**: Right speaker  
- **M**: Mono speaker
- **Colors**: Grey (0-34%), Orange (35-69%), Red (70-100%)
- **IPs**: Debug info in small grey text at bottom

## Hardware

### Required Parts

- **[M5StickC Plus2](https://www.amazon.com/dp/B0F3XQ22XS)** - ESP32 IoT controller with display, IR, and WiFi
- **[IR Receiver Module](https://www.amazon.com/dp/B087ZRD3LH)** - KY-022/TL1838/VS1838B universal IR sensor
- **[Dupont Wires](https://www.amazon.com/dp/B01EV70C78)** - Male-to-female jumper cables for connections
- **TV Remote** - Compatible with LG and Samsung remotes (or any remote - see customization below)

### Wiring

Connect IR receiver to M5StickC Plus2:
- IR OUT → GPIO 26
- IR VCC → 3.3V
- IR GND → GND

## Setup

### Prerequisites

1. Install [VS Code](https://code.visualstudio.com/)
2. Install the [PlatformIO IDE extension](https://platformio.org/install/ide?install=vscode) for VS Code

### Build and Upload

1. Clone the repository
2. Open the project folder in VS Code
3. Copy `include/Secrets.h.example` to `include/Secrets.h`
4. Edit `include/Secrets.h` and add your WiFi credentials
5. Connect your M5StickC Plus2 via USB
6. Use VS Code tasks or PlatformIO toolbar:
   - **Build**: `Cmd+Shift+B` (or Terminal → Run Build Task)
   - **Upload**: Terminal → Run Task → Upload
   - **Upload and Monitor**: Terminal → Run Task → Upload and Monitor
   - **Monitor Serial**: Terminal → Run Task → Monitor Serial
   - **Clean**: Terminal → Run Task → Clean

**Note:** `Secrets.h` is gitignored to protect your credentials.

### Available Tasks

All tasks are preconfigured in `.vscode/tasks.json`:
- **Build** - Compile the project (default task: `Cmd+Shift+B`)
- **Upload** - Upload firmware to device
- **Build & Upload** - Compile and upload in one step
- **Upload and Monitor** - Upload and start serial monitor
- **Monitor Serial** - Open serial monitor (115200 baud)
- **Clean** - Remove build artifacts
- **Full Clean** - Complete rebuild (removes all cached files)

## Controls

- **Button A**: Volume Up (+5%)
- **Button B**: Volume Down (-5%)
- **IR Remote**: Volume Up/Down/Mute

### Supported Remotes

Out of the box, the following remotes are supported:
- **LG TV Remotes** - Standard LG IR protocol
- **Samsung TV Remotes** - Including Insignia replacement remotes (NS-RMTSAM21)

### Adding Your Remote

If your remote isn't recognized:

1. **Upload and Monitor** to see serial output
2. Press volume buttons on your remote
3. Note the protocol and hex codes shown (e.g., `SAMSUNG 0xE0E0E01F`)
4. Edit `include/IRReceiver.h` in the `decodeCommand()` method and add your codes:
   ```cpp
   if (value == 0xYOURCODEHERE) return IRCommand::VolumeUp;
   if (value == 0xYOURCODEHERE) return IRCommand::VolumeDown;
   if (value == 0xYOURCODEHERE) return IRCommand::Mute;
   ```
5. Rebuild and upload

The code automatically handles repeat signals, so holding down a button will work correctly.

## Code Structure

```
include/
  ButtonHandler.h  - Input handling
  Connectivity.h   - WiFi & mDNS
  DevialetAPI.h    - HTTP API client
  Display.h        - Screen rendering
  IRReceiver.h     - IR decoding
  Logger.h         - Serial logging
  Speaker.h        - Speaker management
src/
  main.cpp         - Main program
```

Clean, minimal, header-only architecture with strong encapsulation.

## License

MIT
