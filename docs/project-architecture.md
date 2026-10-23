# Project Architecture

## Data Flow

1. A student presents an RFID card/tag to the reader.
2. The MFRC522 reads the card UID.
3. The ESP32 converts the UID to uppercase hexadecimal text.
4. The program compares the UID with the registered-student list.
5. If a match exists and attendance has not already been marked, the program prints the student name and `PRESENT`.
6. If the UID is unknown, the program reports that attendance was not marked.
7. If the same registered card is scanned again, the program reports that attendance has already been marked for this session.

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
          |
          | SPI
          v
+-------------------+
| ESP32 firmware    |
| - Read UID        |
| - Match student   |
| - Check duplicate |
| - Print result    |
+---------+---------+
          |
          | USB Serial @ 115200 baud
          v
+-------------------+
| Serial Monitor    |
+-------------------+
```

## Pin Configuration

The default sketch uses these ESP32 pins:

- SS/SDA: GPIO 5
- SCK: GPIO 18
- MOSI: GPIO 23
- MISO: GPIO 19
- RST: GPIO 22

These pins can be changed in the sketch if required, but the physical wiring must match the code.
