"""
Automatic Excel logger for the ESP32 RFID attendance system.

The ESP32 sends a line in this format for each newly marked attendance:
EXCEL|UID|Student Name|PRESENT

Configure SERIAL_PORT below to match your computer. Close Arduino Serial
Monitor before running this script because the serial port is exclusive.
"""

import os
import sys
import time
from datetime import datetime

import serial
from serial import SerialException
from openpyxl import Workbook, load_workbook
from openpyxl.styles import Font, PatternFill, Alignment
from openpyxl.utils import get_column_letter

SERIAL_PORT = "COM3"  # Windows example; change to your actual ESP32 port
BAUD_RATE = 115200
EXCEL_FILE = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                          "attendance.xlsx")
SHEET_NAME = "Attendance"
HEADERS = ["Date", "Time", "Student Name", "RFID UID", "Status"]


def create_workbook_if_needed():
    """Create a workbook and header row if the file does not exist."""
    if not os.path.exists(EXCEL_FILE):
        workbook = Workbook()
        sheet = workbook.active
        sheet.title = SHEET_NAME
        sheet.append(HEADERS)
        format_sheet(sheet)
        workbook.save(EXCEL_FILE)
        workbook.close()
        print(f"Created workbook: {EXCEL_FILE}")


def format_sheet(sheet):
    """Apply simple formatting to the attendance header and columns."""
    header_fill = PatternFill("solid", fgColor="1F4E78")
    header_font = Font(bold=True, color="FFFFFF")
    for cell in sheet[1]:
        cell.fill = header_fill
        cell.font = header_font
        cell.alignment = Alignment(horizontal="center")

    widths = [14, 12, 24, 22, 14]
    for index, width in enumerate(widths, start=1):
        sheet.column_dimensions[get_column_letter(index)].width = width

    sheet.freeze_panes = "A2"
    sheet.auto_filter.ref = f"A1:E{max(sheet.max_row, 1)}"


def parse_attendance_line(line):
    """Return (uid, name, status) for valid machine-readable lines."""
    if not line.startswith("EXCEL|"):
        return None

    parts = line.split("|")
    if len(parts) != 4:
        print(f"Ignoring malformed attendance record: {line!r}")
        return None

    _, uid, name, status = parts
    uid = uid.strip().upper()
    name = name.strip()
    status = status.strip().upper()

    if not uid or not name or status != "PRESENT":
        print(f"Ignoring invalid attendance record: {line!r}")
        return None

    return uid, name, status


def already_recorded(sheet, date_text, uid):
    """Prevent a second record for the same UID on the same date."""
    for row in sheet.iter_rows(min_row=2, values_only=True):
        row_date, _row_time, _name, row_uid, _status = row[:5]
        if row_date == date_text and str(row_uid).strip().upper() == uid:
            return True
    return False


def save_attendance(uid, name, status):
    """Append one record to the workbook, unless that UID is already recorded today."""
    now = datetime.now()
    date_text = now.strftime("%Y-%m-%d")
    time_text = now.strftime("%H:%M:%S")

    create_workbook_if_needed()
    workbook = load_workbook(EXCEL_FILE)

    try:
        if SHEET_NAME in workbook.sheetnames:
            sheet = workbook[SHEET_NAME]
        else:
            sheet = workbook.create_sheet(SHEET_NAME)
            sheet.append(HEADERS)
            format_sheet(sheet)

        if already_recorded(sheet, date_text, uid):
            print(f"Already recorded today: {name} (UID {uid})")
            return False

        sheet.append([date_text, time_text, name, uid, status])
        format_sheet(sheet)
        workbook.save(EXCEL_FILE)
        print(f"Saved: {name} | {uid} | {date_text} {time_text} | {status}")
        return True
    finally:
        workbook.close()


def main():
    create_workbook_if_needed()
    print("RFID attendance Excel logger")
    print(f"Excel file: {EXCEL_FILE}")
    print(f"Serial port: {SERIAL_PORT} at {BAUD_RATE} baud")
    print("Close Arduino Serial Monitor before starting this script.")
    print("Waiting for attendance records. Press Ctrl+C to stop.\n")

    try:
        with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as esp:
            # Opening a serial connection can reset some ESP32 boards.
            time.sleep(2)
            esp.reset_input_buffer()
            print("Connected to ESP32. Scan a registered card.\n")

            while True:
                raw_line = esp.readline()
                if not raw_line:
                    continue

                line = raw_line.decode("utf-8", errors="replace").strip()
                if not line:
                    continue

                # Echo serial output so diagnostics remain visible in the terminal.
                print(f"ESP32: {line}")

                record = parse_attendance_line(line)
                if record is None:
                    continue

                uid, name, status = record
                try:
                    save_attendance(uid, name, status)
                except PermissionError:
                    print(
                        "ERROR: Cannot save attendance.xlsx. Close it in Excel "
                        "and scan the card again."
                    )
                except OSError as error:
                    print(f"ERROR saving workbook: {error}")

    except SerialException as error:
        print(f"\nSerial connection error: {error}")
        print("Check SERIAL_PORT, the USB data cable, and the selected COM port.")
        print("Also close Arduino Serial Monitor or any other app using the port.")
        sys.exit(1)
    except KeyboardInterrupt:
        print("\nAttendance logger stopped.")


if __name__ == "__main__":
    main()
