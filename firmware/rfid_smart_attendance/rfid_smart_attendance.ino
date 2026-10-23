/*
  RFID-Based Smart Attendance System with Automatic Excel Logging
  Board: ESP32 DevKit (ESP32-WROOM-32 compatible)
  Reader: MFRC522 / RC522
  Serial Monitor / Python logger: 115200 baud

  Install the "MFRC522" library before compiling.
  Replace the sample UIDs with your actual card UIDs.

  For each newly marked attendance, the firmware emits:
  EXCEL|UID|Student Name|PRESENT

  Python reads that line over USB serial and saves the record to attendance.xlsx.
*/

#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t SS_PIN  = 5;
constexpr uint8_t RST_PIN = 22;

MFRC522 rfid(SS_PIN, RST_PIN);

struct Student {
  const char* uid;
  const char* name;
};

// IMPORTANT: These UIDs are examples only.
// Use rfid_uid_scanner.ino to read your actual card UIDs.
// Do not use the pipe character '|' in student names.
const Student students[] = {
  {"04 A3 7B 2C", "Anushka"},
  {"D1 82 5F 19", "Rahul"},
  {"AA BB CC DD", "Priya"}
};

constexpr size_t STUDENT_COUNT = sizeof(students) / sizeof(students[0]);
bool attendanceMarked[STUDENT_COUNT] = {false};

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
  Serial.println("----------------------------------------");
}

void processAttendance(const String& scannedUID) {
  Serial.println();
  Serial.println("Card detected");
  Serial.print("UID: ");
  Serial.println(scannedUID);

  bool registered = false;

  for (size_t i = 0; i < STUDENT_COUNT; i++) {
    if (scannedUID.equals(students[i].uid)) {
      registered = true;

      Serial.print("Student: ");
      Serial.println(students[i].name);

      if (!attendanceMarked[i]) {
        attendanceMarked[i] = true;
        Serial.println("Attendance: PRESENT");
        Serial.println("Result: Attendance marked successfully");

        // Machine-readable record consumed by attendance_to_excel.py.
        // Keep this format exactly: EXCEL|UID|Name|PRESENT
        Serial.print("EXCEL|");
        Serial.print(scannedUID);
        Serial.print("|");
        Serial.print(students[i].name);
        Serial.println("|PRESENT");
      } else {
        Serial.println("Result: Attendance already marked this session");
        // No EXCEL record is sent for this duplicate scan.
      }

      break;
    }
  }

  if (!registered) {
    Serial.println("UID is not registered.");
    Serial.println("Attendance: NOT MARKED");
    // Unknown cards never produce an EXCEL record.
  }

  printDivider();
}

void setup() {
  Serial.begin(115200);
  delay(500);

  // ESP32 VSPI pins: SCK=18, MISO=19, MOSI=23, SS=5
  SPI.begin(18, 19, 23, SS_PIN);
  rfid.PCD_Init();
  delay(50);

  Serial.println();
  Serial.println("========================================");
  Serial.println("     RFID SMART ATTENDANCE SYSTEM");
  Serial.println("========================================");
  Serial.println("Reader initialized.");
  Serial.println("System ready. Scan an RFID card...");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  const String scannedUID = readCardUID();
  processAttendance(scannedUID);

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  // Brief pause to reduce repeated reads while a card remains on the reader.
  delay(800);
}
