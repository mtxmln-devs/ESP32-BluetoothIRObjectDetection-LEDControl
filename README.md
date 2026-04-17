# ESP32-BluetoothIRObjectDetection-LEDControl
This system use ESP32 to Detect Object using BlueTooth and has LED Control.

# 🔵 ESP32 Bluetooth IR Object Detection & LED Control

Remotely control an LED via Bluetooth **and** detect nearby objects using an IR sensor — all in one ESP32 sketch.

[![Platform](https://img.shields.io/badge/Platform-ESP32-E7352C?style=flat&logo=espressif)](https://www.espressif.com/)
[![Framework](https://img.shields.io/badge/Framework-Arduino-00979D?style=flat&logo=arduino)](https://www.arduino.cc/)
[![Protocol](https://img.shields.io/badge/Protocol-Bluetooth%20Classic-0082FC?style=flat&logo=bluetooth)](https://www.bluetooth.com/)
[![Sensor](https://img.shields.io/badge/Sensor-IR%20Analog-yellow?style=flat)](#)
[![Language](https://img.shields.io/badge/Language-C%2B%2B-00599C?style=flat&logo=c%2B%2B)](https://isocpp.org/)

---

## 📌 Overview

This project combines two independent systems into a single ESP32 sketch:

1. **Bluetooth LED Control** — Send `ON` or `OFF` commands from any Bluetooth-capable device (phone, laptop, Serial terminal) to toggle an LED wirelessly.
2. **IR Object Detection** — Continuously reads an analog IR sensor and broadcasts detection status (`OBJECT DETECTED` / `CLEAR`) back over Bluetooth every 500ms.

The ESP32 advertises itself as **`BlewToT`** over Bluetooth Classic, making it discoverable and pairable from any standard Bluetooth Serial app.

---

## ✨ Features

- 📱 **Wireless LED control** via Bluetooth Classic Serial commands
- 🔴 **Real-time IR object detection** streamed over Bluetooth
- 🔁 **Two-way Bluetooth communication** — send commands, receive sensor data
- 📟 **Simple command interface** — just type `ON` or `OFF`
- ⏱️ **Polling interval** — sensor updates every 500ms
- 🔌 **Minimal wiring** — just an LED and an IR sensor module

---

## 🛠️ Hardware Required

| Component | Quantity |
|-----------|----------|
| ESP32 Development Board | 1 |
| Analog IR Sensor Module | 1 |
| LED | 1 |
| 220Ω Resistor (for LED) | 1 |
| Jumper Wires | As needed |
| Breadboard | 1 |
| USB Cable | 1 |
| Smartphone or PC (Bluetooth) | 1 |

> **Note:** Use an **analog** IR sensor (with an `AO` / analog output pin), not a purely digital one — the code reads the sensor via `analogRead()` on an ADC pin.

---

## 📐 Pin Configuration

| GPIO Pin | Function | Component |
|----------|----------|-----------|
| GPIO 23 | Digital Output | LED (via 220Ω resistor) |
| GPIO 36 | Analog Input (ADC) | IR Sensor Analog Output |

> GPIO 36 (`VP`) is an input-only ADC pin on most ESP32 boards — perfect for analog sensor readings.

### Wiring Diagram

**LED:**
```
GPIO 23 ──── [220Ω Resistor] ──── LED (+) ──── GND
```

**IR Sensor Module:**
```
IR Module VCC  ──── 3.3V or 5V
IR Module GND  ──── GND
IR Module AO   ──── GPIO 36
```

> Some IR modules also have a digital output (`DO`) pin — this project only uses the **analog output (`AO`)** for adjustable sensitivity via the analog threshold.

---

## ⚙️ Software Setup

### Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/software) (1.8.x or 2.x)
- ESP32 board package installed

### Installing the ESP32 Board Package

1. Open Arduino IDE → **File** → **Preferences**
2. Add this URL to *Additional Board Manager URLs*:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Go to **Tools** → **Board** → **Boards Manager**
4. Search for `esp32` and install the package by **Espressif Systems**

### Libraries Used

| Library | Purpose | Install |
|---------|---------|---------|
| `BluetoothSerial.h` | Bluetooth Classic Serial communication | Built-in (ESP32 Arduino core) |

> `BluetoothSerial` is included with the ESP32 Arduino core — no manual installation required. Note: this library is only available on ESP32 chips with **Bluetooth Classic** support (not ESP32-S2, which lacks it).

---

## 🚀 Flashing Instructions

### Step 1 — Open the sketch

Open `BluetoothIRObjectDetectLEDControl.ino` in Arduino IDE.

### Step 2 — Select board and port

- **Tools → Board** → `ESP32 Dev Module` (or your specific model)
- **Tools → Port** → Select the correct COM port

### Step 3 — Enable Bluetooth

Make sure Bluetooth is enabled in the ESP32 partition scheme:
- **Tools → Partition Scheme** → `Default` or `Huge APP (3MB No OTA)`

### Step 4 — Upload

Click **Upload** (→) and wait for the flash to complete.

### Step 5 — Pair via Bluetooth

1. On your phone or PC, scan for Bluetooth devices
2. Pair with **`BlewToT`**
3. Open a **Bluetooth Serial Terminal** app (e.g., *Serial Bluetooth Terminal* on Android, or *PuTTY* / *CoolTerm* on PC)
4. Connect to `BlewToT`

---

## 📲 Bluetooth Commands

Once connected to **`BlewToT`** via a Bluetooth Serial terminal:

| Command | Action | Response |
|---------|--------|----------|
| `ON` | Turns the LED ON | `LED ON` |
| `OFF` | Turns the LED OFF | `LED OFF` |

> Commands are **case-sensitive**. Send `ON` not `on`. Make sure your terminal sends a newline (`\n`) after each command — the sketch uses `readStringUntil('\n')`.

### Automatic Sensor Output

Every **500ms**, the ESP32 automatically sends one of the following over Bluetooth regardless of commands:

| Output | Meaning |
|--------|---------|
| `IR: OBJECT DETECTED` | IR sensor analog value < 2000 (object nearby) |
| `IR: CLEAR` | IR sensor analog value ≥ 2000 (no object detected) |

---

## 🔍 How It Works

```
┌─────────────────────────────────────────────────────────┐
│                        loop()                           │
│                                                         │
│  ┌─────────────────────────────────┐                    │
│  │   Bluetooth Serial Available?   │                    │
│  │   ┌─────┐        ┌──────┐       │                    │
│  │   │ "ON"│──────► │LED ON│       │  ◄── Phone/PC      │
│  │   └─────┘        └──────┘       │      sends cmd     │
│  │   ┌──────┐       ┌───────┐      │                    │
│  │   │"OFF" │──────►│LED OFF│       │                    │
│  │   └──────┘       └───────┘      │                    │
│  └─────────────────────────────────┘                    │
│                                                         │
│  ┌─────────────────────────────────┐                    │
│  │   analogRead(GPIO 36)           │                    │
│  │                                 │                    │
│  │   irValue < 2000?               │                    │
│  │   YES ──► "IR: OBJECT DETECTED" │──► Bluetooth       │
│  │   NO  ──► "IR: CLEAR"           │    Terminal        │
│  └─────────────────────────────────┘                    │
│                                                         │
│  delay(500ms)                                           │
└─────────────────────────────────────────────────────────┘
```

### IR Threshold Explained

The IR sensor returns an analog value from `0` to `4095` (12-bit ADC on ESP32):

| Condition | Analog Value | Status |
|-----------|-------------|--------|
| Object close / detected | `< 2000` | `OBJECT DETECTED` |
| No object / clear | `≥ 2000` | `CLEAR` |

> The threshold of `2000` may need adjustment depending on your IR sensor model and environment. Lower values = stricter detection (object must be closer); higher values = more sensitive.

---

## 📁 File Structure

```
📦 esp32-bluetooth-ir-led/
├── BluetoothIRObjectDetectLEDControl.ino   # Main Arduino sketch
└── README.md                               # This file
```

---

## 🐛 Troubleshooting

| Problem | Likely Cause | Solution |
|---------|-------------|----------|
| `BlewToT` not appearing in Bluetooth scan | Board not powered or sketch not flashed | Re-upload sketch; check power |
| Can't connect after pairing | Previous ghost connection | Forget the device on your phone and re-pair |
| `ON`/`OFF` commands not working | No newline sent | Enable "newline" in terminal settings |
| LED stays OFF after `ON` command | Wiring issue | Check GPIO 23 → resistor → LED → GND |
| IR always shows `OBJECT DETECTED` | Threshold too high or sensor too sensitive | Lower the `2000` threshold in code |
| IR always shows `CLEAR` | Threshold too low or bad wiring | Raise the `2000` threshold; check AO pin connection |
| Bluetooth library not found | Wrong ESP32 board variant | Use a standard ESP32 (not ESP32-S2/S3 which lack BT Classic) |
| Flooding terminal output | 500ms delay too short | Increase `delay(500)` to `delay(1000)` |

---

## 🔧 Possible Improvements

- [ ] Add threshold calibration command over Bluetooth (e.g., `THRESH:1500`)
- [ ] Only send IR status when state **changes** (not every 500ms) to reduce Bluetooth traffic
- [ ] Add a second LED as a dedicated IR indicator
- [ ] Log IR detection events with timestamps via NTP
- [ ] Add BLE (Bluetooth Low Energy) support for phone app compatibility
- [ ] Build a custom Android/iOS app with toggle buttons and live IR status
- [ ] Add buzzer that beeps on object detection

---

## 📄 License

This project is open-source and available under the [MIT License](LICENSE).

---

## 🙋 Author

Built with ❤️ using ESP32 and Arduino.  
Feel free to fork, modify, and build on this project!
