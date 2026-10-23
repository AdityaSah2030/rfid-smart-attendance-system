# Attendance Logic

## Registration
Register each student in the firmware with their actual card UID and name. Use the UID scanner sketch to obtain the value. Example UIDs in the code are placeholders.

## ESP32 Duplicate Prevention
The ESP32 maintains a Boolean attendance flag for each registered student. It emits the `EXCEL|...` record only when attendance is first marked during the current power-on session.

## Excel Duplicate Prevention
Python checks `attendance.xlsx` for an existing record with the same UID and date. This second check prevents duplicate records if the ESP32 restarts during the same day.

## Timestamp
Python records the date and time from the computer's system clock when it receives the serial record. Keep the computer's date/time correct. If exact scan-time timestamps independent of the PC are required, add a real-time clock or reliable time synchronisation to the firmware.

## Multiple Classes per Day
The current workbook stores one attendance record per UID per calendar day. If students attend multiple classes each day, add a `Session` or `Class ID` column and include it in the duplicate-check rule.

## Security
An RFID UID is an identifier and can sometimes be copied or emulated. This project is appropriate as a basic educational prototype, not as sole authentication for high-security systems.
