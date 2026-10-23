# Hardware Components

| Component | Quantity | Purpose |
|---|---:|---|
| ESP32 DevKit | 1 | Reads card data, matches students, and sends attendance over USB serial |
| MFRC522 / RC522 reader | 1 | Reads compatible 13.56 MHz RFID cards/tags |
| RFID card or key fob | 1 or more | Student identifier |
| Jumper wires | As required | Connects the reader to ESP32 |
| USB data cable | 1 | Uploads firmware and transfers serial records to the computer |

## Wiring

| RC522 pin | ESP32 pin |
|---|---|
| 3.3V | 3V3 |
| GND | GND |
| RST | GPIO 22 |
| SDA / SS | GPIO 5 |
| SCK | GPIO 18 |
| MOSI | GPIO 23 |
| MISO | GPIO 19 |
| IRQ | Not connected |

**Do not power the RC522 from 5V.** Use 3V3 and 3.3V logic.

## How Excel is connected

No additional hardware is required for Excel integration. A Python script running on the computer reads a machine-readable record from the ESP32 USB serial connection and writes it into `attendance.xlsx`. The computer and Python logger must be running during scans.
