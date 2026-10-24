# Hardware Components

This project uses the following hardware components to implement an RFID-based smart attendance system with automatic Excel logging.

| Component | Quantity | Purpose |
|-----------|----------|---------|
| ESP32 DevKit | 1 | Main microcontroller for SPI acquisition and serial transmission |
| MFRC522 (RC522) RFID Reader | 1 | Contactless 13.56 MHz RFID card and tag detection |
| RFID Cards / Key Fobs | 1 or more | Contactless student identification credentials |
| Jumper Wires | As required | Electrical connections between ESP32 and reader |
| USB Data Cable | 1 | Firmware flashing and serial data communication |

---

## Component Overview

### ESP32 DevKit
Acts as the central controller responsible for communicating with the MFRC522 RFID reader over the hardware SPI bus, verifying student registration against internal memory, preventing repeated scans during a session, and streaming structured serial payloads to the host computer.

### MFRC522 (RC522) RFID Reader
Operates at 13.56 MHz and reads passive RFID tags using radio frequency identification. It detects the unique identifier (UID) of MIFARE Classic and compatible ISO/IEC 14443A cards and communicates the data to the ESP32 via SPI.

### RFID Cards and Key Fobs
Assigned to registered individuals to serve as contactless credentials. Each card contains a factory-programmed unique identifier (UID) that maps to a specific student record in the system.

### USB Data Cable
Supplies regulated 5V power to the ESP32 via USB and creates a full-duplex UART serial link with the host computer for firmware flashing and real-time attendance streaming.

### Jumper Wires
Provide reliable electrical interconnects for SPI lines (SCK, MISO, MOSI, SS/SDA), reset control (RST), and 3.3V power rails.

---

## Pin Mapping & Wiring

The MFRC522 reader is connected to the ESP32 DevKit via the hardware VSPI bus interface:

| RC522 Pin | ESP32 Pin | Function |
|-----------|-----------|----------|
| 3.3V | 3V3 | Regulated 3.3V Power Supply |
| GND | GND | Ground Reference |
| RST | GPIO 22 | Hardware Reset |
| SDA / SS | GPIO 5 | SPI Chip Select (Slave Select) |
| SCK | GPIO 18 | SPI Clock |
| MOSI | GPIO 23 | Master Out Slave In |
| MISO | GPIO 19 | Master In Slave Out |
| IRQ | Not Connected | Interrupt Line (unused) |

> **Electrical Notice:** The RC522 module must strictly be powered from the ESP32's **3V3** pin. Connecting the reader to 5V will permanently damage the MFRC522 IC. Ensure wire lengths are kept short to maintain signal integrity across high-speed SPI lines.

---

## Estimated Cost

| Component | Approximate Cost (INR) |
|-----------|------------------------|
| ESP32 DevKit | ₹300–₹550 |
| MFRC522 Reader Kit (with card & fob) | ₹100–₹180 |
| Additional RFID Cards / Fobs | ₹20–₹40 each |
| Jumper Wires | ₹40–₹80 |
| Micro-USB / USB-C Data Cable | ₹80–₹150 |

Total project cost is approximately ₹550–₹950 depending on component source and existing inventory.

---
