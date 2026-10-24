# Troubleshooting Guide

## Purpose

This guide outlines common issues encountered during the assembly, flashing, serial communication, and Excel logging phases of the RFID Smart Attendance System, along with practical diagnostic and resolution steps.

---

## Hardware & Wiring Issues

### Reader Not Detecting Cards
- **Symptom:** Cards tapped on the MFRC522 produce no serial response or LED activity.
- **Potential Causes:**
  - Power line connected to 5V instead of 3.3V (causing chip latchup or damage).
  - Loose SPI jumper wires (SDA, SCK, MOSI, or MISO).
  - Incompatible card frequency (e.g., using 125 kHz EM4100 cards instead of 13.56 MHz ISO 14443A MIFARE cards).
- **Resolution:**
  - Verify wiring matches the pin table: SCK=18, MISO=19, MOSI=23, SS/SDA=5, RST=22, 3V3=3.3V, GND=GND.
  - Test jumper wires with a multimeter for continuity.
  - Ensure the cards/key fobs operate at 13.56 MHz.

---

## Firmware & Compilation Issues

### `MFRC522.h: No such file or directory`
- **Symptom:** Compilation fails in Arduino IDE with a missing header error.
- **Potential Causes:**
  - MFRC522 library is not installed in the active Arduino library directory.
- **Resolution:**
  - Open **Sketch → Include Library → Manage Libraries**.
  - Search for `MFRC522` and install the library authored by GithubCommunity.

### ESP32 Upload Hangs / `Failed to connect to ESP32`
- **Symptom:** Arduino IDE displays `Connecting........_____.....` and times out.
- **Potential Causes:**
  - Board is not entering bootloader mode automatically.
  - Defective USB cable (power-only cable lacking data pins).
  - Incorrect COM port selected.
- **Resolution:**
  - Hold the **BOOT** (or IO0) button on the ESP32 while the IDE displays `Connecting...`, then release once uploading starts.
  - Swap the cable for a verified USB data cable.
  - Check Windows Device Manager to confirm the assigned silicon bridge COM port (CP2102, CH340, or FTDI).

---

## Serial Communication Issues

### Serial Port Busy / Access Denied
- **Symptom:** Python throws `SerialException: could not open port 'COMx': PermissionError(13, 'Access is denied.')`.
- **Potential Causes:**
  - Arduino Serial Monitor is open and holding an exclusive lock on the COM port.
  - Another terminal or Python instance is already attached to the port.
- **Resolution:**
  - Close the Arduino IDE Serial Monitor window.
  - Terminate any background Python processes or serial terminals before launching `attendance_to_excel.py`.

### Garbled Characters in Serial Monitor
- **Symptom:** Unreadable characters or question marks appear in the console.
- **Potential Causes:**
  - Baud rate mismatch.
- **Resolution:**
  - Set the Serial Monitor baud rate to **115200 baud** to match `Serial.begin(115200)`.

---

## Python Environment & Excel Issues

### `ModuleNotFoundError: No module named 'serial'`
- **Symptom:** Running the script fails immediately with an import error.
- **Potential Causes:**
  - Dependencies are not installed in the active Python environment, or standard `serial` was installed instead of `pyserial`.
- **Resolution:**
  - Run `python -m pip install -r requirements.txt`.
  - Ensure you are using the same Python interpreter where the packages were installed (`py -m pip ...` or `.venv\Scripts\python`).

### `PermissionError: Cannot save attendance.xlsx`
- **Symptom:** Terminal warns that `attendance.xlsx` cannot be updated.
- **Potential Causes:**
  - Microsoft Excel has the file open with an exclusive write lock.
- **Resolution:**
  - Close Microsoft Excel or the viewing application.
  - Rescan the card; the Python logger will automatically re-attempt the save.

### Duplicate Entries Not Recorded
- **Symptom:** Tapping a card multiple times only produces one row in Excel.
- **Explanation:**
  - This is expected behavior. The dual-layer deduplication engine filters repeated scans on the same calendar date to prevent fraudulent or accidental duplicate marks.

---

## Diagnostic Summary Table

| Issue | Primary Check | Quick Solution |
|-------|---------------|----------------|
| ESP32 won't upload | USB cable & COM port | Hold BOOT button during upload; use data cable |
| Card scan produces no output | 13.56 MHz card & wiring | Check 3.3V connection; ensure tag is ISO 14443A |
| Card reports "Unknown" | Student array mapping | Run `rfid_uid_scanner.ino`, copy UID to firmware |
| Python COM port error | Port locks | Close Arduino Serial Monitor |
| Excel write error | File lock | Close `attendance.xlsx` in Excel |

---

## Conclusion

Most operational issues stem from either exclusive serial port access conflicts or incorrect SPI wiring. Following the diagnostic checklist above ensures dependable day-to-day operation.

---
