# Troubleshooting

## Serial Monitor shows nothing
- Set the baud rate to **115200**.
- Check that the correct ESP32 board and COM port are selected.
- Use a USB cable that supports data, not charging only.
- Press the ESP32 reset/EN button after opening Serial Monitor.

## Reader is not detected or cards are not read
- Confirm the RC522 is powered from **3V3**, not 5V.
- Recheck GND, SS/SDA, SCK, MOSI, MISO, and RST connections.
- Keep jumper wires short and secure.
- Confirm the card/tag is compatible with a 13.56 MHz MFRC522 reader. Many 125 kHz cards will not work.

## `MFRC522.h: No such file or directory`
Install the `MFRC522` library using Arduino IDE Library Manager.

## Card is detected but marked unknown
- Upload the UID scanner sketch and scan the card.
- Copy the exact UID printed by the scanner into the `students` array.
- Keep each UID byte separated by one space and use uppercase hexadecimal characters.
- Recompile and upload the attendance sketch.

## Upload fails
- Check the selected COM port.
- Try another USB data cable or USB port.
- Some ESP32 boards require holding the BOOT button while upload begins.

## Reader behaves inconsistently
- Verify all SPI wires and common ground.
- Avoid powering motors or other high-current loads from the ESP32 3V3 pin.
- If using a different ESP32 variant, confirm the chosen GPIO pins are available on that board.
