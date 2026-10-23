/*
  RFID UID Scanner for ESP32 + MFRC522 / RC522
  Use this sketch first to find the real UID of each student card.
  Serial Monitor: 115200 baud
*/

#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t SS_PIN  = 5;
constexpr uint8_t RST_PIN = 22;

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  delay(500);

  SPI.begin(18, 19, 23, SS_PIN);
  rfid.PCD_Init();
  delay(50);

  Serial.println();
  Serial.println("========================================");
  Serial.println("       RFID CARD UID SCANNER");
  Serial.println("========================================");
  Serial.println("Scan a card or key fob to display its UID.");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("Card UID: ");

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
  Serial.println("Copy this UID into the students array in the attendance sketch.");
  Serial.println("----------------------------------------");

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  delay(800);
}
