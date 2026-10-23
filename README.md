# RFID-Based Smart Attendance System with Automatic Excel Logging

An RFID attendance system built with an **ESP32**, an **MFRC522 (RC522) RFID reader**, and a **Python serial logger**. A registered student scans their card, the ESP32 identifies the card UID, and Python automatically records the attendance in an Excel workbook (`attendance.xlsx`).

This project keeps the original simple hardware setup. No SD card, extra microcontroller, or Wi-Fi connection is required. The computer must be connected to the ESP32 over USB and the Python logger must be running.

## Features

- Reads supported RFID card/tag UIDs using an MFRC522 reader.
- Matches card UIDs to registered student names.
- Prints attendance and diagnostic information in Arduino IDE Serial Monitor.
- Sends a machine-readable serial record for successful attendance.
- Uses Python to create/update an Excel workbook automatically.
- Stores date, time, student name, UID, and attendance status.
- Rejects unknown cards.
- Prevents duplicate records for the same UID on the same date in Excel.
- Includes a separate RFID UID scanner sketch.
- Includes wiring diagram, setup instructions, troubleshooting, and architecture notes.

> **Before you begin:** The sample card UIDs in the firmware are placeholders. Run the UID scanner sketch and replace them with your actual card UIDs.

---

## Project Folder Structure

```text
rfid-smart-attendance-system-excel/
├── README.md
├── LICENSE
├── .gitignore
├── requirements.txt
├── attendance_to_excel.py
├── docs/
│   ├── attendance-logic.md
│   ├── project-architecture.md
│   ├── setup-guide.md
│   └── troubleshooting.md
├── firmware/
│   ├── rfid_smart_attendance/
│   │   └── rfid_smart_attendance.ino
│   └── rfid_uid_scanner/
│       └── rfid_uid_scanner.ino
└── hardware/
    ├── circuit-diagram.png
    └── components.md
```

The `attendance.xlsx` file is created automatically in the same folder as `attendance_to_excel.py` the first time a valid attendance record is received.

---

## Hardware Components

| Component | Quantity | Purpose |
|---|---:|---|
| ESP32 DevKit (ESP32-WROOM-32 or compatible) | 1 | Main controller |
| MFRC522 / RC522 RFID reader | 1 | Reads RFID card/tag UIDs |
| Compatible 13.56 MHz RFID card or key fob | 1 or more | Student identification |
| Jumper wires | As required | Reader connections |
| USB data cable | 1 | Upload firmware and send serial data to the computer |

### Estimated cost in India

| Component | Approximate cost |
|---|---:|
| ESP32 DevKit | ₹300–₹600 |
| RC522 reader kit with card/tag | ₹65–₹150 |
| Jumper wires (if needed) | ₹50–₹150 |

Approximate total: **₹415–₹900**, depending on seller, shipping, and components already available.

---

## Circuit Diagram

![ESP32 and RC522 wiring diagram](hardware/circuit-diagram.png)

### RC522-to-ESP32 wiring

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

`SDA` on the RC522 board is used as the SPI chip-select pin in this project.

> **Electrical warning:** Power the RC522 from the ESP32's **3V3** pin, never 5V. The RC522 and ESP32 use 3.3V logic. Keep the wires short and connections secure.

---

## Software Requirements

- Arduino IDE 2.x
- ESP32 board support for Arduino IDE
- `MFRC522` library (commonly listed as MFRC522 by GithubCommunity)
- Python 3.10 or newer recommended
- Python packages listed in `requirements.txt`

### Install ESP32 board support

1. Open Arduino IDE → **File → Preferences**.
2. Add this URL to **Additional Boards Manager URLs** if it is not already present:

   `https://espressif.github.io/arduino-esp32/package_esp32_index.json`

3. Open **Tools → Board → Boards Manager**.
4. Search for `esp32` and install **esp32 by Espressif Systems**.
5. Select the correct ESP32 board and COM port.

### Install the RFID library

Open **Sketch → Include Library → Manage Libraries**, search for `MFRC522`, and install it.

### Install Python dependencies

Open Command Prompt or the VS Code terminal in the project folder and run:

```bash
python -m pip install -r requirements.txt
```

If `python` is not recognised on Windows, try:

```bash
py -m pip install -r requirements.txt
```

---

## How to Run the Project

### Step 1 — Read your RFID card UIDs

1. Open `firmware/rfid_uid_scanner/rfid_uid_scanner.ino` in Arduino IDE.
2. Upload it to the ESP32.
3. Open **Tools → Serial Monitor** and set the baud rate to **115200**.
4. Scan each card/tag and copy the UID printed in the monitor.

### Step 2 — Register students

Open `firmware/rfid_smart_attendance/rfid_smart_attendance.ino`. Replace the sample entries in the `students` array with your actual UIDs and student names.

Example only:

```cpp
const Student students[] = {
  {"04 A3 7B 2C", "Anushka"},
  {"D1 82 5F 19", "Rahul"}
};
```

Use the UIDs read from your own cards. The UID format should match the scanner output. Avoid using the `|` character in student names because it separates fields in the serial record.

### Step 3 — Upload the attendance firmware

Upload `rfid_smart_attendance.ino` to the ESP32.

### Step 4 — Close Arduino Serial Monitor

**Close Arduino Serial Monitor before running Python.** Only one program can normally use the ESP32's COM port at a time. The Python logger reads the same USB serial connection.

### Step 5 — Find the COM port

In Arduino IDE, check **Tools → Port** and note the ESP32 port (for example, `COM3`).

Open `attendance_to_excel.py` and update:

```python
SERIAL_PORT = "COM3"
```

Replace `COM3` with your actual port. On Linux, a port may look like `/dev/ttyUSB0` or `/dev/ttyACM0`; on macOS, it may look like `/dev/cu.usbserial-...`.

### Step 6 — Start automatic Excel logging

Run in the project folder:

```bash
python attendance_to_excel.py
```

On Windows, this can also be:

```bash
py attendance_to_excel.py
```

Keep the terminal running and scan a registered card. The script creates or updates `attendance.xlsx` automatically.

### Step 7 — View the Excel workbook

Open `attendance.xlsx` after scanning. Each accepted record includes:

- Date
- Time
- Student Name
- RFID UID
- Status

The date and time are taken from the computer running Python, when the serial record is received.

---

## Example Serial Monitor Output

The ESP32 prints diagnostic messages plus a machine-readable line for Python:

```text
========================================
     RFID SMART ATTENDANCE SYSTEM
========================================
Reader initialized.
System ready. Scan an RFID card...

Card detected
UID: 04 A3 7B 2C
Student: Anushka
Attendance: PRESENT
Result: Attendance marked successfully
EXCEL|04 A3 7B 2C|Anushka|PRESENT
----------------------------------------
```

The sample UID/name above are illustrative. Python listens for lines beginning with `EXCEL|` and ignores other diagnostic messages.

### Example Excel rows

| Date | Time | Student Name | RFID UID | Status |
|---|---|---|---|---|
| 2026-10-23 | 10:15:21 | Anushka | 04 A3 7B 2C | PRESENT |
| 2026-10-23 | 10:16:08 | Rahul | D1 82 5F 19 | PRESENT |

These rows are examples only.

---

## Architecture

```text
RFID Card / Tag
      |
      v
MFRC522 Reader
      |
      | SPI
      v
ESP32 Attendance Firmware
      |
      | USB Serial: EXCEL|UID|NAME|PRESENT
      v
Python Serial Logger
      |
      | openpyxl
      v
attendance.xlsx
(Date, Time, Name, UID, Status)
```

---

## Important Behaviour and Limitations

- **Computer required:** The computer must stay connected to the ESP32 and the Python script must be running to record attendance in Excel.
- **Serial Monitor:** Close Arduino Serial Monitor while Python uses the COM port.
- **Duplicate prevention:** Python checks the UID and date before adding a row. This prevents repeated records for the same card on the same date, including after restarting the ESP32.
- **Session handling:** This version treats attendance as one record per student per calendar day. If you have multiple classes per day, add a session/class identifier to the workbook and duplicate-check logic.
- **Timestamp:** Date/time is from the computer when the record is received, not a hardware clock on the ESP32.
- **Workbook use:** If Excel reports that the file cannot be saved, close `attendance.xlsx` and scan again. The logger reports save errors in the terminal.
- **RAM state:** ESP32's in-memory attendance flags reset on restart. The Python/Excel date-and-UID check is the second layer of duplicate prevention.
- **Security:** An RFID UID is an identifier, not strong authentication. Do not use this basic UID-only design as the sole security control for high-risk systems.
- **Card compatibility:** RC522 typically reads compatible 13.56 MHz cards; many 125 kHz cards will not work.

---

## Troubleshooting

See [`docs/troubleshooting.md`](docs/troubleshooting.md).

## Future Improvements

- Add a session/class ID to allow multiple classes per day.
- Add an LCD/OLED and buzzer.
- Add an online backup or Google Sheets integration.
- Create attendance summaries and percentage reports.
- Add a real-time clock if the system must timestamp scans independently of the computer.

## License

This project is released under the MIT License. See [`LICENSE`](LICENSE).
