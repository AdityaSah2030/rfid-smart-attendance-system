# Troubleshooting

## Serial Monitor shows nothing
- Set baud rate to 115200.
- Confirm the correct board and COM port.
- Use a USB data cable.
- Press the ESP32 reset/EN button.

## Python reports a serial connection error
- Check `SERIAL_PORT` in `attendance_to_excel.py`.
- Close Arduino Serial Monitor and other apps using the same COM port.
- Confirm the USB cable supports data.
- On Windows, inspect Device Manager for the COM port.

## `No module named serial`
Install dependencies in the same Python environment used to run the script:

```bash
python -m pip install -r requirements.txt
```

## `MFRC522.h: No such file or directory`
Install the MFRC522 library in Arduino IDE Library Manager.

## Reader does not detect cards
- Power RC522 from 3V3, not 5V.
- Check GND, SS/SDA, SCK, MOSI, MISO, and RST wires.
- Verify that the card is compatible with the 13.56 MHz MFRC522 reader; many 125 kHz cards are incompatible.

## Card appears as unknown
Run the UID scanner, copy the exact UID, and update the `students` array. Recompile and upload the attendance sketch.

## Excel file cannot be saved
Close `attendance.xlsx` in Excel, then scan again. Check that the project folder is writable.

## Attendance is not being written
- Confirm Python logger is running before scanning.
- Confirm the Arduino firmware prints the line beginning with `EXCEL|`.
- Check the terminal for serial or workbook errors.
- Keep student names free of the `|` character.

## Upload fails
- Check the selected board and port.
- Try another USB data cable or USB port.
- Some ESP32 boards need the BOOT button held while upload begins.
