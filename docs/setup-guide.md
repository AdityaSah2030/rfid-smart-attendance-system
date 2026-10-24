# Setup and Configuration Guide

## Purpose

This document outlines the step-by-step setup procedure required to configure the hardware, flash firmware to the ESP32, set up the Python host logging environment, and verify automated attendance recording into Microsoft Excel.

---

## Prerequisites

### Hardware Requirements
- ESP32 DevKit (ESP32-WROOM-32 or compatible)
- MFRC522 (RC522) RFID Reader Module
- 13.56 MHz RFID Cards / Key Fobs
- Breadboard & Jumper Wires
- Micro-USB or USB-C Data Cable (must support data transfer, not charge-only)

### Software Requirements
- Arduino IDE 2.x
- ESP32 Board Core (by Espressif Systems)
- `MFRC522` Library (by GithubCommunity)
- Python 3.10 or newer
- Virtual environment or terminal with `pip` access

---

## Step 1: Hardware Wiring

Connect the MFRC522 RFID reader to the ESP32 DevKit using the hardware VSPI bus interface:

| MFRC522 Pin | ESP32 Pin | Function |
|-------------|-----------|----------|
| 3.3V | 3V3 | Regulated 3.3V Power |
| GND | GND | Common Ground |
| RST | GPIO 22 | Reset Control Line |
| SDA / SS | GPIO 5 | SPI Chip Select |
| SCK | GPIO 18 | SPI Clock |
| MOSI | GPIO 23 | Master Out Slave In |
| MISO | GPIO 19 | Master In Slave Out |
| IRQ | Not Connected | Unused |

> **Important:** Connect the power pin strictly to **3V3**. Do not use 5V or VIN, as overvoltage will damage the MFRC522 chip.

---

## Step 2: Arduino IDE & ESP32 Board Core Setup

1. Open Arduino IDE and navigate to **File → Preferences**.
2. Add the official ESP32 package index URL to **Additional Boards Manager URLs**:
   ```text
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
   ```
3. Open **Tools → Board → Boards Manager**, search for `esp32`, and install **esp32 by Espressif Systems**.
4. Open **Sketch → Include Library → Manage Libraries**, search for `MFRC522`, and install the library.

---

## Step 3: Read RFID Card UIDs

Before student names can be recognized, each card's unique physical identifier (UID) must be scanned:

1. Open `firmware/rfid_uid_scanner/rfid_uid_scanner.ino` in Arduino IDE.
2. Select your ESP32 board under **Tools → Board** (e.g., `ESP32 Dev Module`).
3. Select the active COM port under **Tools → Port**.
4. Click **Upload**.
5. Once uploaded, open **Tools → Serial Monitor** and set the baud rate to **115200**.
6. Tap each RFID card or key fob on the reader. Note down each printed UID string (e.g., `04 A3 7B 2C`).

---

## Step 4: Register Students in Attendance Firmware

1. Open `firmware/rfid_smart_attendance/rfid_smart_attendance.ino`.
2. Locate the `students` array definition:
   ```cpp
   const Student students[] = {
     {"04 A3 7B 2C", "Anushka"},
     {"D1 82 5F 19", "Rahul"},
     {"AA BB CC DD", "Priya"}
   };
   ```
3. Replace the placeholder values with your actual scanned card UIDs and corresponding student names.
4. Upload `rfid_smart_attendance.ino` to the ESP32.
5. Verify in the Serial Monitor that the greeting banner displays:
   ```text
   ========================================
        RFID SMART ATTENDANCE SYSTEM
   ========================================
   Reader initialized.
   System ready. Scan an RFID card...
   ```
6. **Crucial:** Close the Arduino Serial Monitor window before proceeding to the next step.

---

## Step 5: Configure Python Host Environment

1. Open a terminal or Command Prompt in the `rfid-smart-attendance-system/` root directory.
2. (Optional) Create and activate a virtual environment:
   ```bash
   python -m venv .venv
   .venv\Scripts\activate
   ```
3. Install required Python packages:
   ```bash
   python -m pip install -r requirements.txt
   ```
4. Open `attendance_to_excel.py` and verify `SERIAL_PORT` matches your ESP32 COM port:
   ```python
   SERIAL_PORT = "COM3"  # Update with your port (e.g., COM4, COM5, or /dev/ttyUSB0)
   ```

---

## Step 6: Run Automatic Excel Logger

1. Launch the Python logger from your terminal:
   ```bash
   python attendance_to_excel.py
   ```
2. The logger will establish a serial connection to the ESP32 and display:
   ```text
   RFID attendance Excel logger
   Excel file: ...\attendance.xlsx
   Serial port: COM3 at 115200 baud
   Close Arduino Serial Monitor before starting this script.
   Waiting for attendance records. Press Ctrl+C to stop.
   
   Connected to ESP32. Scan a registered card.
   ```
3. Tap a registered card against the reader.
4. The terminal will log the transaction and confirm the Excel write:
   ```text
   ESP32: Card detected
   ESP32: UID: 04 A3 7B 2C
   ESP32: Student: Anushka
   ESP32: Attendance: PRESENT
   ESP32: EXCEL|04 A3 7B 2C|Anushka|PRESENT
   Saved: Anushka | 04 A3 7B 2C | 2026-10-24 09:30:15 | PRESENT
   ```
5. Open `attendance.xlsx` to view the formatted attendance records.

---

## Performance Evaluation Checklist

Use the following checklist during initial system commissioning:

- [ ] ESP32 boots and initializes the MFRC522 reader without SPI errors
- [ ] Card UID scanner successfully reads 13.56 MHz RFID tags
- [ ] Registered students are successfully matched in firmware
- [ ] Unregistered cards are recognized and rejected without logging
- [ ] Arduino Serial Monitor is closed prior to launching the Python daemon
- [ ] Python script successfully binds to the designated COM port
- [ ] `attendance.xlsx` is created automatically with formatted navy headers
- [ ] Subsequent scans of the same card on the same day are deduplicated
- [ ] Script handles open workbook locks gracefully with user warnings

---

## Conclusion

Following this setup process establishes a robust, automated attendance logging pipeline. The dual-layer architecture guarantees accurate record keeping directly inside Microsoft Excel with zero manual data entry.

---
