# Setup Guide

## 1. Install Arduino IDE
Install Arduino IDE 2.x on your computer.

## 2. Install ESP32 board support
In Arduino IDE, open **File → Preferences**. Add the following URL to “Additional Boards Manager URLs”:

`https://espressif.github.io/arduino-esp32/package_esp32_index.json`

Open **Tools → Board → Boards Manager**, search for `esp32`, and install **esp32 by Espressif Systems**. Select the board matching your hardware and select the correct port.

## 3. Install MFRC522 library
Open **Sketch → Include Library → Manage Libraries**, search for `MFRC522`, and install it.

## 4. Wire the RC522 reader
Follow the table and diagram in [`../hardware/components.md`](../hardware/components.md) and the root README. Power the reader from the ESP32's 3V3 pin, not 5V.

## 5. Read the UIDs
Open and upload `firmware/rfid_uid_scanner/rfid_uid_scanner.ino`. Open Serial Monitor at 115200 baud and scan each card.

## 6. Register student IDs
Copy each UID into the `students` array in `firmware/rfid_smart_attendance/rfid_smart_attendance.ino`, together with the student's name. Use the exact UID printed by the scanner.

## 7. Run attendance
Upload the attendance sketch, open Serial Monitor at 115200 baud, and scan registered cards. A student's attendance is marked once per power-on session.
