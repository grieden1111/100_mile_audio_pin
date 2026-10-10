/*
 * Minimal test: read the JEDEC ID from the W25Q64JV flash chip.
 * A correct response proves your SPI wiring (through the level
 * shifter) is actually working, before trusting anything built
 * on top of it (like SerialFlash).
 *
 * Expected result for a genuine W25Q64JV:
 *   Manufacturer ID: 0xEF
 *   Memory Type:     0x40
 *   Capacity:        0x17
 */

#include <SPI.h>

#define FLASH_CS_PIN 10

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  pinMode(FLASH_CS_PIN, OUTPUT);
  digitalWrite(FLASH_CS_PIN, HIGH);  // idle state -- chip ignores the bus

  SPI.begin();

  uint8_t manufacturerID, memoryType, capacity;

  digitalWrite(FLASH_CS_PIN, LOW);   // select the chip -- it's listening now

  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));

  SPI.transfer(0x9F);                   // send the "Read JEDEC ID" command
  manufacturerID = SPI.transfer(0x00);  // send a dummy byte, capture the reply
  memoryType     = SPI.transfer(0x00);
  capacity       = SPI.transfer(0x00);

  SPI.endTransaction();

  digitalWrite(FLASH_CS_PIN, HIGH);  // deselect -- done talking to this chip

  Serial.print("Manufacturer ID: 0x");
  Serial.println(manufacturerID, HEX);
  Serial.print("Memory Type: 0x");
  Serial.println(memoryType, HEX);
  Serial.print("Capacity: 0x");
  Serial.println(capacity, HEX);
}

void loop() {
  // nothing -- this is a one-shot test
}

