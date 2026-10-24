# Development Notes

## Project Objective

The goal of this project was to design and develop an automated, contactless attendance recording system that leverages RFID credentials and an ESP32 microcontroller to log attendance records directly into Microsoft Excel in real time.

The project aimed to eliminate manual paper roll calls while providing a zero-cost, offline-first architecture that requires no external cloud subscriptions or complex server setups.

---

## Design Goals

- Rapid contactless card scanning with sub-second feedback
- Automated Excel spreadsheet creation and formatted record appending
- Dual-layer duplicate scan prevention (firmware debouncing and host-side date checking)
- Clean, modular code structure separating embedded firmware from desktop logging logic
- Resilient serial communication handling port conflicts and workbook locking
- Affordable, readily accessible hardware components

---

## Development Process

### Phase 1: Hardware Integration & SPI Configuration

The initial phase involved establishing reliable SPI communication between the ESP32 DevKit and the MFRC522 RFID reader.

Key focus areas:
- Mapping the hardware VSPI bus lines (GPIO 18 SCK, GPIO 19 MISO, GPIO 23 MOSI, GPIO 5 SS/SDA, and GPIO 22 RST).
- Verifying 3.3V logic supply rails to ensure the MFRC522 IC is protected from damaging overvoltage.
- Ensuring robust breadboard interconnects to minimize signal reflections on high-frequency SPI lines.

---

### Phase 2: RFID UID Scanner Implementation

Before developing the full attendance system, a dedicated diagnostic scanner utility (`rfid_uid_scanner.ino`) was developed to:
- Test PICC card polling and anti-collision loops.
- Extract raw UID byte sequences from MIFARE Classic 1K cards and key fobs.
- Format UIDs into consistent, zero-padded uppercase hexadecimal strings for database mapping.

---

### Phase 3: Attendance Firmware & Session Debounce

The core attendance firmware (`rfid_smart_attendance.ino`) was developed to:
- Maintain an internal student registry pairing physical card UIDs with student names.
- Track in-memory session flags (`attendanceMarked[]`) to prevent repeated scans while a card rests on the reader.
- Format human-readable diagnostics for the Serial Monitor alongside structured machine payloads.

---

### Phase 4: Python Serial Logger & Excel Integration

The host desktop integration (`attendance_to_excel.py`) was implemented to:
- Establish a resilient serial listener over the ESP32 UART bridge.
- Parse machine records using the `EXCEL|UID|Name|Status` protocol format.
- Ingest computer-synchronized timestamps (date and time).
- Verify calendar-day uniqueness against existing spreadsheet records.
- Automatically format the workbook with styled navy headers, auto-adjusted column widths, and freeze panes using `openpyxl`.

---

## Challenges Faced

### 3.3V Logic Requirements
The MFRC522 is strictly a 3.3V logic device. Using a 5V supply risks destroying the reader's input protection diodes. Powering the module strictly from the ESP32's onboard 3.3V LDO regulator ensured signal safety without requiring external bidirectional logic level shifters.

### Card Bouncing & Repeated Reads
Passive RFID cards held near the antenna can trigger multiple read cycles within milliseconds. This was resolved through an 800 ms polling delay combined with firmware-level boolean session tracking.

### Serial Port Contention
On Windows, serial COM ports cannot be shared concurrently by multiple processes. Opening the Arduino Serial Monitor while running the Python logger resulted in `PermissionError(13, 'Access is denied.')`. Clear documentation and exception handling were introduced to guide users to release the port.

### Excel File Locking
When Microsoft Excel opens an `.xlsx` file, it places an exclusive filesystem write lock on it. If an attendance record arrives while the workbook is open, `openpyxl` raises a `PermissionError`. The script was engineered to catch this exception gracefully, report a clear warning to the operator, and preserve execution without crashing.

---

## Key Learnings

Through this project, the following embedded and software concepts were explored and refined:
- High-speed SPI peripheral initialization on the ESP32 architecture
- Contactless RFID protocol fundamentals (ISO/IEC 14443A and MIFARE anti-collision)
- Stream-based serial packet framing and synchronization delimiters
- Dual-layer deduplication architectures across distributed hardware and software boundaries
- Desktop Python automation, serial stream parsing, and spreadsheet styling with `openpyxl`

---

## Future Scope

Potential areas for future exploration include:
- Integrating an I2C OLED (SSD1306) or 16x2 LCD for standalone visual feedback
- Adding a piezoelectric buzzer to provide distinct audio chirps for successful vs. duplicate scans
- Integrating an external RTC module (DS3231) to allow standalone logging to an onboard SD card without needing a host PC
- Implementing WiFi/MQTT telemetry to synchronize records to cloud databases (Google Sheets, Firebase)
- Multi-classroom session tracking and lecture timetable management

---

## Conclusion

This project demonstrates the effective combination of embedded microcontroller programming, contactless RF sensing, and host-level Python automation. The resulting system delivers a modern, paperless, and dependable attendance tracking solution with zero manual data entry.

---
