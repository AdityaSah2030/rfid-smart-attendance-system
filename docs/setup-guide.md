# Setup Guide

## 1. Install Arduino IDE and ESP32 board support
Install Arduino IDE. If needed, add this board URL in File → Preferences:

`https://espressif.github.io/arduino-esp32/package_esp32_index.json`

Use Boards Manager to install `esp32 by Espressif Systems`, then select the board and COM port matching your device.

## 2. Install the MFRC522 library
In Arduino IDE, open Sketch → Include Library → Manage Libraries, search for `MFRC522`, and install it.

## 3. Wire the reader
Connect the RC522 using the table in `hardware/components.md`. Power the reader from ESP32 3V3, not 5V.

## 4. Read card UIDs
Upload `firmware/rfid_uid_scanner/rfid_uid_scanner.ino`, open Serial Monitor at 115200 baud, and scan each card. Copy the exact UID printed.

## 5. Register students
Edit the `students` array in `firmware/rfid_smart_attendance/rfid_smart_attendance.ino`. Replace the placeholder UIDs and names with your real values, then upload the attendance sketch.

## 6. Install Python packages
From the project folder, run:

```bash
python -m pip install -r requirements.txt
```

## 7. Configure the serial port
In `attendance_to_excel.py`, change `SERIAL_PORT = "COM3"` to your ESP32's actual port. Arduino IDE → Tools → Port shows the selected port. Linux and macOS use device paths rather than COM names.

## 8. Start the logger
Close Arduino Serial Monitor. Run:

```bash
python attendance_to_excel.py
```

Keep the terminal open. Scan a registered card. The script creates `attendance.xlsx` in the project folder and adds one row per UID per date.

## 9. View Excel data
Open `attendance.xlsx` after scanning. If the workbook cannot be saved, close it in Excel and scan again. The logger prints save errors in the terminal.
