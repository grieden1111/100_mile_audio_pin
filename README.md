# 100_mile_audio_pin
This is a wearable, pre-recorded audio playback device that someone can wear for a run! The purpose for this is for my best friend, who is running 100 miles. They will be alone for most of it, so having a voice they can lean on when they need it is the goal!

## Getting Arduino running (debain, flatpak)
Run in the command line: flatpak run cc.arduino.IDE2


## New Direction using XIAO ESP32-S3 chip
	For some reason, I could not understand the MISO line in the SPI protocol. Fortunately, after searching
	for a chip that has onboard memory, the XIAO ESP32-S3 was incredibly compact and a better option overall!
	So yay for failing in understanding MISO, as I found a much better chip for the audio pin.
	
	New plan for audio pin:
	XIAO ESP32-S3 is the microcontroller and storage device for audio files. Will use the MAX 98357 amp
	to power a 8 ohm speaker. A 3.7 li-ion or LiPo battery will power the project. I will solder all together, and use
	hydrophobic audio mesh all around a 3D printed case to help ensure that any sweat/water will not take out the pin.	

## Setting up LittleFS (Arduino)
	LittleFS will be storing all audio files
	- Seems that the esp32 by espressif on arduino ide installed LittleFS already as part of its [libraries](https://github.com/espressif/arduino-esp32)

	Using LittleFs with ESP-IDF
	- [Step 1](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/index.html)
	- [Step 2](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/linux-setup.html#debian-based-linux-installation-via-apt)
	- Can go back to step 1, which shows you how to create your first project 
		- Github page project with [ESP32-S3, LittleFS and ESP-IDF](https://github.com/alfredochac/ESP32-Projects_LittleFS/tree/main/ESP32S3_LittleFS_Demo)

### Issues I encountered with LittleFS, Arduino and Hardware
	- My Arduino IDE did not have a 'Sketch Upload,' so checkout this [github](https://github.com/earlephilhower/arduino-littlefs-upload)
	- Download the .vsix file
	- In your CLI: 
		- Go to ~/.arduinoIDE/plugins/ (might have to create 'plugins' directory
		- Move .vsix file into 'plugins' directory 
	- Restart Arduino IDE and launch again
	- In the Arduino IDE, press CRTL + SHIFT + P and type LittleFS
	- Should have "Upload... LittleFS...", hit enter key
	
	- Hardware: Exchanging USB cables dealth with the error: "ERROR" No port specified, check IDE menus."

## Understanding Hardware
	- XIAO ESP32-S3
		- https://wiki.seeedstudio.com/SeeedStudio_XIAO_Series_Introduction/
		- https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/
	
	- LittleFS Partition
		- [Espressif's guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/partition-tables.html)
		- electricalflux, from [Robert Kim](https://electricalflux.com/mcu-general/set-littlefs-partition-size-arduino-esp32)
		
	- MAX98357 audio amp
		- [Adafruit's guide](https://learn.adafruit.com/adafruit-max98357-i2s-class-d-mono-amp/overview)
		- [In-depth Youtube video](https://www.youtube.com/watch?v=HOoPKGAMr0I)
		- Cirkit Designer's [quick guide with project examples](https://docs.cirkitdesigner.com/component/3600f762-9978-4d93-b74b-f431e1fc831c/max98357a)
		- [Makerguides](https://www.makerguides.com/playing-audio-with-esp32-and-max98357/)

### Libraries I am using
	- [Espressifs's arduino-esp32](https://github.com/espressif/arduino-esp32)
	- Earlephilhower's:
		- [ESP8266Audio](https://github.com/earlephilhower/ESP8266Audio/tree/master)
		- [arduino-littlfs-upload](https://github.com/earlephilhower/arduino-littlefs-upload)



















## Will maybe follow up with Flash Memory, but could not figure out why MISO pin was not working properly

## (Will maybe revisit) Sources that I am using/found to help bring this project to life
        Storing audio files into flash memory through SPI
        https://forum.arduino.cc/t/how-to-upload-audio-file-in-external-flash-memory-using-arduino-ze>
        https://forum.arduino.cc/t/using-external-flash-as-an-alternative-to-sd-card/1079126/10
        https://github.com/PaulStoffregen/SerialFlash


## (Will maybe revisit) Arduino + SPI pin stuff
	Helpful Links I am using:
	- https://docs.arduino.cc/language-reference/en/functions/communication/SPI/
	- https://www.circuitbasics.com/how-to-set-up-spi-communication-for-arduino/
	- https://electricalflux.com/mcu-coding/spi-for-arduino-beginner-sensor-wiring-guide (very helpful, where i got level shifter knowledge from)	
	
	Hardware I am using:
	- Flash memory(using W25Q64JV, only difference is more storage): https://learn.adafruit.com/adafruit-spi-flash-breakouts/pinouts
	- Generic 4 way logic level shifter
	- Arduino Uno R3
	
	I am using a level shifter in-between the flash memory and the arduino uno. 
	- Pin 10 -> HV4 -> LV4 -> CS
	- Pin 11 -> HV3 -> LV3 -> MOSI
	- Pin 12 -> HV2 -> LV2 -> MISO
	- Pin 13 -> HV1 -> LV1 -> SCK
	
	Going to start out with a basic transfer/seeing how this works.		

	UART tranfer (most likely the thing to help transfer files from comp to arduino
	- https://docs.arduino.cc/learn/communication/uart/	
