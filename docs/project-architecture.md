# Project Architecture

## Overview

The RFID Smart Attendance System is built around a hybrid embedded-and-host processing pipeline. Contactless card data captured by an MFRC522 RFID reader is processed by an ESP32 microcontroller, verified against an in-memory student database, and streamed over serial communication to a Python logger that records and deduplicates attendance in Microsoft Excel.

---

## System Architecture

```text
                ┌─────────────────────────┐
                │    RFID Card / Tag      │
                │       (13.56 MHz)       │
                └────────────┬────────────┘
                             │
                             ▼
                ┌─────────────────────────┐
                │   MFRC522 RFID Reader   │
                └────────────┬────────────┘
                             │ SPI Interface
                             ▼
                ┌─────────────────────────┐
                │      ESP32 DevKit       │
                │                         │
                │  - UID Normalization    │
                │  - Registry Lookup      │
                │  - Session Debounce     │
                └────────────┬────────────┘
                             │ USB UART Serial (115200 baud)
                             ▼
                ┌─────────────────────────┐
                │   Python Serial Logger  │
                │                         │
                │  - Calendar Deduplication│
                │  - Timestamping         │
                │  - openpyxl Formatter   │
                └────────────┬────────────┘
                             │
                             ▼
                ┌─────────────────────────┐
                │     attendance.xlsx     │
                │  (Date, Time, Name, UID)│
                └─────────────────────────┘
```

---

## Data Flow

### 1. Card Detection & UID Acquisition

The MFRC522 reader continuously polls for compatible ISO/IEC 14443A cards within electromagnetic range. When a card is placed near the antenna, the reader extracts its serial number.

Output:

```text
Scanned Card UID (e.g., "04 A3 7B 2C")
```

---

### 2. Firmware-Level Registry Check

The ESP32 converts raw byte arrays into a normalized, uppercase hexadecimal string and evaluates it against an internal array of registered students.

- **Unregistered Card:** Emits a warning to the Serial Monitor; no payload is broadcast.
- **Registered Card:** Retrieves corresponding student name and checks session status.

---

### 3. Session Debounce & Machine Payload

The ESP32 maintains in-memory attendance flags. If the card has already been scanned during the active power cycle, further transmissions are suppressed. If new, the flag is raised and a structured serial message is broadcast:

```text
EXCEL|04 A3 7B 2C|Anushka|PRESENT
```

---

### 4. Host Ingestion & Calendar Deduplication

The Python serial logger listens continuously on the active COM port, parses lines prefixed with `EXCEL|`, and validates data fields:

- Ingests computer system timestamp (YYYY-MM-DD and HH:MM:SS).
- Checks `attendance.xlsx` to ensure the student has not already been marked on the current calendar date.

---

### 5. Spreadsheet Persistence

If validated, the record is appended as a formatted row in `attendance.xlsx`. Header styling, column auto-width, and freeze panes are dynamically applied using `openpyxl`.

```text
[Date]       [Time]     [Student Name]   [RFID UID]       [Status]
2026-10-24   09:30:15   Anushka          04 A3 7B 2C      PRESENT
```

---

## Hardware Responsibilities

### ESP32 DevKit

- Initializes hardware SPI bus and reader registers
- Continuously polls for new RFID tags
- Normalizes UID byte arrays into formatted hex strings
- Cross-references scans against registered student credentials
- Manages in-memory session flags to prevent rapid double-scanning
- Transmits human-readable diagnostics and structured machine records

---

### MFRC522 RFID Reader

- Emits 13.56 MHz electromagnetic carrier field
- Energizes passive RFID transponders via inductive coupling
- Reads anti-collision card identification data
- Communicates serial data to ESP32 over SPI lines

---

### Host Computer (Python Environment)

- Interfaces with ESP32 UART bridge via `pyserial`
- Filters diagnostic noise and isolates machine records
- Generates system timestamps independent of microcontroller state
- Manages workbook creation, header styling, and file locking
- Implements calendar-day deduplication across microcontroller reboots

---

## Attendance Execution Cycle

The system continuously executes the following workflow:

```text
Wait for Card Present
          ↓
Read Card Serial (UID)
          ↓
Normalize Hexadecimal String
          ↓
Match UID with Student Registry
          ↓
Already Marked this Session? ───► Yes ───► Ignore & Debounce
          ↓ No
Set Session Flag to True
          ↓
Emit EXCEL|UID|Name|PRESENT via Serial
          ↓
Python Logger Ingests Record
          ↓
Already Recorded Today in Excel? ──► Yes ───► Skip Workbook Save
          ↓ No
Append Row to attendance.xlsx
          ↓
Return to Card Polling
```

---

## Key Features

- **Dual-Layer Deduplication:** Protects against immediate double-tapping (firmware) and multi-session reboot duplication (host Excel check).
- **Zero Cloud Dependency:** Functions entirely offline with zero latency or cloud connectivity costs.
- **Structured Serial Framing:** Uses clean pipe-delimited packet headers (`EXCEL|...`) allowing coexistence with verbose debugging output.
- **Dynamic Excel Styling:** Creates styled headers, centered alignments, and optimal column widths automatically.
- **Fail-Safe Operation:** Gracefully handles unexpected card removals, disconnected serial links, and locked workbook instances.

---

## Future Enhancements

Potential improvements include:

- On-device OLED/LCD display for real-time visual confirmation
- Audio feedback using an active piezoelectric buzzer
- Dedicated Real-Time Clock (DS3231 RTC) for offline embedded timestamping
- WiFi/MQTT synchronization to Google Sheets or enterprise databases
- Multi-classroom session and subject period tracking
- Web-based attendance reporting and analytics portal

---

## Summary

The project combines contactless RF identification, embedded hardware control, and Python automation to achieve a seamless, paperless attendance tracking system. The modular architecture cleanly separates hardware acquisition from database persistence, ensuring reliable long-term operation.

---
