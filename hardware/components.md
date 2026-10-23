# Hardware Components

This project uses an ESP32 and an MFRC522 RFID reader to identify student cards and print attendance information to the Serial Monitor.

| Component | Quantity | Purpose |
|---|---:|---|
| ESP32 DevKit | 1 | Main controller |
| MFRC522 / RC522 RFID reader | 1 | Reads compatible RFID card UIDs |
| 13.56 MHz RFID cards or key fobs | 1 or more | Student IDs |
| Jumper wires | As required | Connect the reader to ESP32 |
| USB data cable | 1 | Upload firmware and view serial output |

## Component Overview

### ESP32 DevKit
Runs the firmware, matches card UIDs to student names, tracks attendance for the current session, and prints results over USB serial.

### MFRC522 / RC522 Reader
Reads supported 13.56 MHz RFID cards and tags. It communicates with the ESP32 over SPI. The RC522 board's pin labelled `SDA` is used as the SPI chip-select pin in this project.

### RFID Cards / Key Fobs
Each card/tag has a UID. The sketch uses this UID to find the matching student in the registered-student list. Use the UID scanner sketch to read the actual UID values.

### Jumper Wires
Connect the RC522 pins to the ESP32. Keep wires short where possible for reliable SPI communication.

## Estimated Cost (India)

| Component | Approximate cost (INR) |
|---|---:|
| ESP32 DevKit | ₹300–₹600 |
| RC522 kit with card/tag | ₹65–₹150 |
| Jumper wires | ₹50–₹150 |

Prices are estimates and vary by seller, quality, and shipping. If you already own the ESP32 and jumper wires, the extra cost may be limited to the RFID kit.
