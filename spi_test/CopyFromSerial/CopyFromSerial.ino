/* Taken and modified from Paul Stoffregen's SerialFlash example code, 'CopyFromSerial.ino,' writting by Wyatt Olson.
  Thank you!!
*/

#include <SerialFlash.h>
#include <SPI.h>

//Buffer sizes
#define USB_BUFFER_SIZE 128
#define FLASH_BUFFER_SIZE 4096

//Max filename length
#define FILENAME_STRING_SIZE 13

//State machine
#define STATE_START 0
#define STATE_SIZE 1
#define STATE_CONTENT 2

//Special byres in the communication protocol
#define BYTE_START 0x7e
#define BYTE_ESCAPE 0x7d
#define BYTE_SEPARATOR 0x7c

//SPI pins (on Arduino Uno R3)
#define MOSI 11
#define MISO 12  
#define SCK 13
#define CS 10


void flushError(){
  uint32_t lastRecieveTime = millis();
  char usbBuffer[USB_BUFFER_SIZE];
  while (Serial.available() || lastRecieveTime + 3000 > millis()) {
    if (Serial.readBytes(usbBuffer, USB_BUFFER_SIZE)) {
      lastRecieveTime = millis();
    }
  }
}

void setup() {
  Serial.begin(115200); //Originaly 9600 in example

  //initialize digital pin LED_BUILTIN as an output. (taken from simple arduino 'Blink' exmaple)
  pinMode(LED_BUILTIN, OUTPUT);

  if (!SerialFlash.begin(CS)) {
    while (1) {
      Serial.println(F("Unable to access SPI Flash chip"));
      delay(1000);
    }
  }

  //We start by formatting the flash...
  uint8_t id[5];
  SerialFlash.readID(id);
  SerialFlash.eraseAll();

  //Flash LED at 1Hz while formatting
  while (!SerialFlash.ready()) {
    //to control digital led pin
    digitalWrite(LED_BUILTIN, HIGH);  // change state of the LED by setting the pin to the HIGH voltage level
    delay(500);                      // wait for a .5 second
    digitalWrite(LED_BUILTIN, LOW);   // change state of the LED by setting the pin to the LOW voltage level
    delay(500);                      // wait for a .5 second
  }

  //Quickly flash LED a few times when completed, then leave the light on solid
  for (uint8_t i = 0; i < 10; i++){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
  digitalWrite(LED_BUILTIN, HIGH);

  //We are now going to wait for the upload program
  while (!Serial.available());

  SerialFlashFile flashFile;

  uint8_t state = STATE_START;
  uint8_t escape = 0;
  uint8_t fileSizeIndex = 0;
  uint32_t fileSize = 0;
  char filename[FILENAME_STRING_SIZE];

  char usbBuffer[USB_BUFFER_SIZE];
  uint8_t flashBuffer[FLASH_BUFFER_SIZE];

  uint16_t flashBufferIndex = 0;
  uint8_t filenameIndex = 0;

  uint32_t lastRecieveTime = millis();

//We assume the serial receive part is finished when we have not received something for 3 seconds
  while(Serial.available() || lastRecieveTime + 3000 > millis()){
    uint16_t available = Serial.readBytes(usbBuffer, USB_BUFFER_SIZE);
    if (available) {
      lastRecieveTime = millis();
    }

    for (uint16_t usbBufferIndex = 0; usbBufferIndex < available; usbBufferIndex++){
      uint8_t b = usbBuffer[usbBufferIndex];
      
      if (state == STATE_START){
        //Start byte.  Repeat start is fine.
        if (b == BYTE_START){
          for (uint8_t i = 0; i < FILENAME_STRING_SIZE; i++){
            filename[i] = 0x00;
          }
          filenameIndex = 0;
        }
        //Valid characters are A-Z, 0-9, comma, period, colon, dash, underscore
        else if ((b >= 'A' && b <= 'Z') || (b >= '0' && b <= '9') || b == '.' || b == ',' || b == ':' || b == '-' || b == '_'){
          filename[filenameIndex++] = b;
          if (filenameIndex >= FILENAME_STRING_SIZE){
            //Error name too long
            flushError();
            return;
          }
        }
        //Filename end character
        else if (b == BYTE_SEPARATOR){
          if (filenameIndex == 0){
            //Error empty filename
            flushError();
            return;
          }
          
          //Change state
          state = STATE_SIZE;
          fileSizeIndex = 0;
          fileSize = 0;
          
        }
        //Invalid character
        else {
          //Error bad filename
          flushError();
          return;
        }
      }
      //We read 4 bytes as a uint32_t for file size
      else if (state == STATE_SIZE){
        if (fileSizeIndex < 4){
          fileSize = (fileSize << 8) + b;
          fileSizeIndex++;
        }
        else if (b == BYTE_SEPARATOR){
          state = STATE_CONTENT;
          flashBufferIndex = 0;
          escape = 0;
          
          if (SerialFlash.exists(filename)){
            SerialFlash.remove(filename);  //It doesn't reclaim the space, but it does let you create a new file with the same name.
          }
          
          //Create a new file and open it for writing
          if (SerialFlash.create(filename, fileSize)) {
            flashFile = SerialFlash.open(filename);
            if (!flashFile) {
              //Error flash file open
              flushError();
              return;
            }
          }
          else {
            //Error flash create (no room left?)
            flushError();
            return;
          }
        }
        else {
          //Error invalid length requested
          flushError();
          return;
        }
      }
      else if (state == STATE_CONTENT){
        //Previous byte was escaped; unescape and add to buffer
        if (escape){
          escape = 0;
          flashBuffer[flashBufferIndex++] = b ^ 0x20;
        }
        //Escape the next byte
        else if (b == BYTE_ESCAPE){
          //Serial.println("esc");
          escape = 1;
        }
        //End of file
        else if (b == BYTE_START){
          //Serial.println("End of file");
          state = STATE_START;
          flashFile.write(flashBuffer, flashBufferIndex);
          flashFile.close();
          flashBufferIndex = 0;
        }
        //Normal byte; add to buffer
        else {
          flashBuffer[flashBufferIndex++] = b;
        }
        
        //The buffer is filled; write to SD card
        if (flashBufferIndex >= FLASH_BUFFER_SIZE){
          flashFile.write(flashBuffer, FLASH_BUFFER_SIZE);
          flashBufferIndex = 0;
        }
      }
    }
  }

  //Success! Turn the light off.
  digitalWrite(LED_BUILTIN, LOW);
}
void loop() {
  //Do nothing.
}

