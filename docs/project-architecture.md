# Project Architecture

## Data Flow

1. Student presents a compatible RFID card/tag.
2. RC522 reads the UID and sends it to the ESP32 over SPI.
3. ESP32 converts the UID to uppercase hexadecimal text.
4. Firmware checks the registered-student list.
5. If the card is registered and not already marked during the current ESP32 session, firmware prints attendance details and sends one machine-readable serial line:
   `EXCEL|UID|Student Name|PRESENT`
6. Python reads the line from USB serial and validates its fields.
7. Python uses the computer's current date/time, checks the workbook for an existing row with the same UID and date, then appends the record.
8. `attendance.xlsx` stores date, time, name, UID, and status.

## Architecture Diagram

```text
+-------------------+
| RFID card / tag   |
+---------+---------+
          |
          v
+-------------------+
| MFRC522 reader    |
+---------+---------+
          | SPI
          v
+-------------------+
| ESP32 firmware    |
| - Read UID        |
| - Match student   |
| - Mark attendance |
| - Print EXCEL row |
+---------+---------+
          | USB Serial
          v
+-------------------+
| Python logger     |
| pyserial          |
| openpyxl          |
+---------+---------+
          |
          v
+-------------------+
| attendance.xlsx   |
+-------------------+
```

## Serial Record Format

`EXCEL|UID|Student Name|PRESENT`

The `EXCEL|` prefix allows Python to ignore ordinary debug messages. Do not use `|` inside student names because it is the field delimiter.

## Important Behaviour

- Excel timestamp is the computer time when Python receives the event.
- The workbook deduplicates by UID and date, so it records one attendance row per card per day.
- The ESP32's in-memory flag prevents repeated marks during one power-on session.
- The PC and logger must be running; this version is not an offline standalone logger.
