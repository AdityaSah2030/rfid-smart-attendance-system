/*
 * Project: RFID-Based Smart Attendance System
 *
 * Description:
 * Automated smart attendance system using an ESP32
 * microcontroller, MFRC522 (13.56 MHz) RFID reader,
 * and serial communication for automated Excel logging.
 *
 * Features:
 * - Contactless RFID card detection and UID extraction
 * - In-memory student registry matching
 * - Session-based duplicate scan prevention
 * - Machine-readable serial record generation
 * - Diagnostic feedback via Serial Monitor
 *
 * Hardware:
 * - ESP32 DevKit (ESP32-WROOM-32 compatible)
 * - MFRC522 (RC522) RFID Reader Module
 * - 13.56 MHz RFID Cards / Key Fobs
 *
 * Author: Aditya Kumar Sah
 *
 * NOTE ON SERIAL COMMUNICATION:
 * This sketch communicates over hardware Serial at 115200 baud.
 * Close the Arduino Serial Monitor before running attendance_to_excel.py
 * because only one process can access the USB COM port at a time.
 */

#include <SPI.h>
#include <MFRC522.h>

// ===================
// Hardware Pin Configuration
// ===================
// ESP32 hardware VSPI bus default pins:
// SCK  = GPIO 18
// MISO = GPIO 19
// MOSI = GPIO 23
// SS   = GPIO 5 (configured below)
// RST  = GPIO 22 (configured below)
constexpr uint8_t SS_PIN  = 5;
constexpr uint8_t RST_PIN = 22;

MFRC522 rfid(SS_PIN, RST_PIN);

// ===================
// Student Registry Configuration
// ===================
struct Student {
    const char* uid;
    const char* name;
};

// NOTE: These UIDs are examples. Run rfid_uid_scanner.ino first to
// read your actual physical card UIDs and substitute them below.
// Avoid using the pipe character '|' in student names as it is the serial delimiter.
const Student students[] = {
    {"04 A3 7B 2C", "Anushka"},
    {"D1 82 5F 19", "Rahul"},
    {"AA BB CC DD", "Priya"}
};

constexpr size_t STUDENT_COUNT = sizeof(students) / sizeof(students[0]);

// ===================
// Session State Variables
// ===================
bool attendanceMarked[STUDENT_COUNT] = {false};

// ===================
// Helper Functions
// ===================

// Reads the scanned card's UID and normalizes it into an uppercase hexadecimal string.
String readCardUID() {
    String uid;

    for (byte i = 0; i < rfid.uid.size; i++) {
        if (rfid.uid.uidByte[i] < 0x10) {
            uid += '0';
        }
        uid += String(rfid.uid.uidByte[i], HEX);

        if (i < rfid.uid.size - 1) {
            uid += ' ';
        }
    }

    uid.toUpperCase();
    return uid;
}

void printDivider() {
    Serial.println(F("----------------------------------------"));
}

// Processes card UID, checks registry, and outputs serial records.
void processAttendance(const String& scannedUID) {
    Serial.println();
    Serial.println(F("Card detected"));
    Serial.print(F("UID: "));
    Serial.println(scannedUID);

    bool registered = false;

    for (size_t i = 0; i < STUDENT_COUNT; i++) {
        if (scannedUID.equals(students[i].uid)) {
            registered = true;

            Serial.print(F("Student: "));
            Serial.println(students[i].name);

            if (!attendanceMarked[i]) {
                attendanceMarked[i] = true;
                Serial.println(F("Attendance: PRESENT"));
                Serial.println(F("Result: Attendance marked successfully"));

                // Structured machine-readable record for attendance_to_excel.py
                // Format: EXCEL|<UID>|<Name>|PRESENT
                Serial.print(F("EXCEL|"));
                Serial.print(scannedUID);
                Serial.print(F("|"));
                Serial.print(students[i].name);
                Serial.println(F("|PRESENT"));
            } else {
                Serial.println(F("Result: Attendance already marked this session"));
            }

            break;
        }
    }

    if (!registered) {
        Serial.println(F("UID is not registered."));
        Serial.println(F("Attendance: NOT MARKED"));
    }

    printDivider();
}

// ===================
// Setup Function
// ===================
void setup() {
    Serial.begin(115200);
    delay(500);

    // Initialize hardware SPI bus with custom SS pin
    SPI.begin(18, 19, 23, SS_PIN);
    rfid.PCD_Init();
    delay(50);

    Serial.println();
    Serial.println(F("========================================"));
    Serial.println(F("     RFID SMART ATTENDANCE SYSTEM       "));
    Serial.println(F("========================================"));
    Serial.println(F("Reader initialized."));
    Serial.println(F("System ready. Scan an RFID card..."));
}

// ===================
// Main Execution Loop
// ===================
void loop() {
    // Check for presence of an RFID card
    if (!rfid.PICC_IsNewCardPresent()) {
        return;
    }

    // Read card serial number
    if (!rfid.PICC_ReadCardSerial()) {
        return;
    }

    const String scannedUID = readCardUID();
    processAttendance(scannedUID);

    // Halt card communication and stop cryptographic encryption
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    // Prevent rapid re-reading while the card remains on the antenna
    delay(800);
}
