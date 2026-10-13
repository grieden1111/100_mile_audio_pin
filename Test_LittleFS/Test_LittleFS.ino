#include <Arduino.h>
#include <LittleFS.h>

#define FORMAT_LITTLEFS_IF_FAILED true

void setup() {
  Serial.begin(115200);
  delay(3000);
  if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)) {
    Serial.println("An error has occured while mounting LittleFS");
    return;
  }
  Serial.println("LittleFS mounted");
  delay(1000);
  File file = LittleFS.open("/data.txt", "r");
  if (!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  Serial.println("File content: ");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}

void loop(){
  //Do nothing.
}