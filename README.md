# RFID-Based Smart Attendance System using ESP32

A beginner-friendly RFID attendance project built with an **ESP32 development board** and an **MFRC522 (RC522) RFID reader**. When a registered card is scanned, the ESP32 identifies the student and prints the student's name, card UID, and attendance status in the Arduino IDE Serial Monitor.

This version stores attendance in RAM only. It does not require Wi-Fi, a database, an LCD, or a buzzer.

---

## Features

- Reads RFID card/tag UIDs using the MFRC522 reader.
- Matches scanned UIDs against a list of registered students.
- Prints the UID, student name, and attendance status to Serial Monitor.
- Rejects unregistered cards.
- Prevents duplicate attendance during the current power-on session.
- Includes a separate UID scanner sketch to register cards.
- Includes wiring, setup instructions, troubleshooting, and project notes.

> **Important:** The sample UIDs in the main sketch are placeholders. Scan your own cards using the UID scanner sketch and replace the sample UIDs before testing attendance.

---

## Hardware Components

| Component | Quantity | Purpose |
|---|---:|---|
| ESP32 DevKit (ESP32-WROOM-32 or compatible) | 1 | Main microcontroller |
| MFRC522 / RC522 RFID reader | 1 | Reads RFID card/tag UIDs |
| 13.56 MHz compatible RFID card or key fob | 1 or more | Student identification |
| Jumper wires | As required | Connections |
| USB data cable | 1 | Programming and Serial Monitor |

### Estimated cost (India)

| Component | Approximate cost |
|---|---:|
| ESP32 DevKit board | ₹300–₹600 |
| RC522 reader kit with card/tag | ₹65–₹150 |
| Jumper wires (if not already available) | ₹50–₹150 |

Approximate total: **₹415–₹900**, depending on the board, seller, shipping, and components already available.

---

## Circuit Diagram

![ESP32 and RC522 wiring diagram](hardware/circuit-diagram.png)

### Wiring table

| RC522 pin | ESP32 pin | Notes |
|---|---|---|
| 3.3V | 3V3 | Use 3.3 V, **not 5 V** |
| GND | GND | Common ground |
| RST | GPIO 22 | Reset |
| SDA / SS | GPIO 5 | SPI chip select; this pin is labelled SDA on many RC522 boards |
| SCK | GPIO 18 | SPI clock |
| MOSI | GPIO 23 | SPI controller-to-reader data |
| MISO | GPIO 19 | SPI reader-to-controller data |
| IRQ | Not connected | Not needed for this project |

Pin labels can vary slightly between module manufacturers. The RC522 uses SPI; its pin labelled `SDA` is used as the SPI chip-select pin in this wiring.

> **Electrical safety:** The RC522 is a 3.3 V device. Do not connect its power pin to 5 V. Avoid directly connecting 5 V logic signals to ESP32 GPIO pins.

---

## Software Requirements

1. Arduino IDE 2.x.
2. ESP32 board support installed in Arduino IDE.
3. The **MFRC522** library (commonly listed as `MFRC522` by GithubCommunity).
4. A USB data cable and a working COM port.

### Install the ESP32 board package

In Arduino IDE, open **File → Preferences** and add this Espressif board-manager URL to “Additional Boards Manager URLs” if it is not already present:

`https://espressif.github.io/arduino-esp32/package_esp32_index.json`

Then open **Tools → Board → Boards Manager**, search for `esp32`, and install **esp32 by Espressif Systems**. Select the appropriate ESP32 board and COM port.

### Install the RFID library

Open **Sketch → Include Library → Manage Libraries**, search for `MFRC522`, and install the library. If Arduino IDE asks to install dependencies, accept them.

---

## Run the Project

### Step 1 — Find your card UIDs

1. Open `firmware/rfid_uid_scanner/rfid_uid_scanner.ino`.
2. Upload it to the ESP32.
3. Open **Tools → Serial Monitor** and set the baud rate to **115200**.
4. Hold an RFID card/tag near the RC522 reader.
5. Copy the UID printed in the Serial Monitor. Repeat for every card.

### Step 2 — Register students

Open `firmware/rfid_smart_attendance/rfid_smart_attendance.ino` and edit the `students` list. Replace the example UIDs with the UIDs you scanned. Keep the spaces and uppercase hexadecimal format consistent with the scanner output.

Example:

```cpp
const Student students[] = {
  {"04 A3 7B 2C", "Anushka"},
  {"D1 82 5F 19", "Rahul"}
};
```

The UIDs above are examples only. Use the UIDs printed by your own reader.

### Step 3 — Upload and scan

1. Upload the attendance sketch.
2. Open Serial Monitor at **115200 baud**.
3. Scan a registered card.
4. Check the printed student name and attendance status.

---

## Example Serial Monitor Output

```text
========================================
     RFID SMART ATTENDANCE SYSTEM
========================================
System ready. Scan an RFID card...

Card detected
UID: 04 A3 7B 2C
Student: Anushka
Attendance: PRESENT
Result: Attendance marked successfully
----------------------------------------

Card detected
UID: 04 A3 7B 2C
Student: Anushka
Result: Attendance already marked this session
----------------------------------------

Card detected
UID: 99 88 77 66
UID is not registered.
Attendance: NOT MARKED
----------------------------------------
```

The UIDs and names in this example are illustrative.

---

## Project Architecture

```text
RFID Card / Tag
      |
      | 13.56 MHz radio
      v
MFRC522 Reader
      |
      | SPI (SCK, MOSI, MISO, SS, RST)
      v
ESP32
      |
      | USB Serial
      v
Arduino IDE Serial Monitor
  UID + Student + Attendance
```

---

## Limitations

- Attendance is stored in RAM. It is cleared when the ESP32 restarts or loses power.
- The project does not generate a real date/time timestamp because no real-time clock or network time is configured.
- A card UID is an identifier, not strong authentication. Do not use this simple UID-only design as the sole security mechanism for a high-security access system.
- Only cards compatible with the 13.56 MHz MFRC522 reader can be read; many 125 kHz cards are not compatible.

---

## Troubleshooting

See [`docs/troubleshooting.md`](docs/troubleshooting.md) for common issues.

## Project Files

```text
rfid-smart-attendance-system/
├── README.md
├── LICENSE
├── .gitignore
├── docs/
│   ├── project-architecture.md
│   ├── setup-guide.md
│   ├── troubleshooting.md
│   └── attendance-logic.md
├── firmware/
│   ├── rfid_smart_attendance/
│   │   └── rfid_smart_attendance.ino
│   └── rfid_uid_scanner/
│       └── rfid_uid_scanner.ino
└── hardware/
    ├── circuit-diagram.png
    └── components.md
```

## Future Improvements

- Store attendance in a CSV file or online database.
- Add a real-time clock for date and time.
- Add an OLED/LCD display and buzzer.
- Send attendance to a web dashboard over Wi-Fi.
- Add a reset command or session-management feature.

## License

This project is released under the MIT License. See [`LICENSE`](LICENSE).
