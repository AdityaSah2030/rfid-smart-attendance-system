/*
 * Project: RFID Card UID Scanner Utility
 *
 * Description:
 * Diagnostic utility for the ESP32 and MFRC522 RFID reader.
 * Scans passive RFID tags and cards (13.56 MHz) and outputs
 * their normalized physical unique identifiers (UID) to the
 * Serial Monitor.
 *
 * Hardware:
 * - ESP32 DevKit (ESP32-WROOM-32 compatible)
 * - MFRC522 (RC522) RFID Reader Module
 *
 * Author: Aditya Kumar Sah
 *
 * Usage:
 * Upload this sketch first to obtain the exact UID of each card.
 * Copy and paste the resulting UIDs into the 'students' array in
 * the main attendance sketch (rfid_smart_attendance.ino).
 */

#include <SPI.h>
#include <MFRC522.h>

// ===================
// Hardware Pin Configuration
// ===================
constexpr uint8_t SS_PIN  = 5;
constexpr uint8_t RST_PIN = 22;

MFRC522 rfid(SS_PIN, RST_PIN);

// ===================
// Setup Function
// ===================
void setup() {
    Serial.begin(115200);
    delay(500);

    // Initialize SPI bus: SCK=18, MISO=19, MOSI=23, SS=5
    SPI.begin(18, 19, 23, SS_PIN);
    rfid.PCD_Init();
    delay(50);

    Serial.println();
    Serial.println(F("========================================"));
    Serial.println(F("        RFID CARD UID SCANNER           "));
    Serial.println(F("========================================"));
    Serial.println(F("Scan an RFID card or key fob to display its UID."));
    Serial.println(F("----------------------------------------"));
}

// ===================
// Main Execution Loop
// ===================
void loop() {
    // Check for new card present
    if (!rfid.PICC_IsNewCardPresent()) {
        return;
    }

    // Read card serial
    if (!rfid.PICC_ReadCardSerial()) {
        return;
    }

    Serial.print(F("Card UID: "));

    for (byte i = 0; i < rfid.uid.size; i++) {
        if (rfid.uid.uidByte[i] < 0x10) {
            Serial.print('0');
        }
        Serial.print(rfid.uid.uidByte[i], HEX);

        if (i < rfid.uid.size - 1) {
            Serial.print(' ');
        }
    }

    Serial.println();
    Serial.println(F("Copy this UID into the 'students' array in rfid_smart_attendance.ino"));
    Serial.println(F("----------------------------------------"));

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    delay(800);
}
