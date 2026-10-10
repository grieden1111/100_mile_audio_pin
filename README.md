# 100_mile_audio_pin
This is a wearable, pre-recorded audio playback device that someone can wear for a run! The purpose for this is for my best friend, who is running 100 miles. They will be alone for most of it, so having a voice they can lean on when they need it is the goal!

## Getting Arduino running (debain, flatpak)
Run in the command line: flatpak run cc.arduino.IDE2


## New Direction using XIAO ESP32-S3 chip
	For some reason, I could not understand the MISO line in the SPI protocol. Fortunately, after searching
	for a chip that has onboard memory, the XIAO ESP32-S3 was incredibly compact and a better option overall!
	So yay for failing in understanding MISO, as I found a much better chip for the audio pin.

	Helpful Links I am using for XIAO ESP32-S3 chip:
	- https://wiki.seeedstudio.com/SeeedStudio_XIAO_Series_Introduction/
	- (Very useful) https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/
	
	New plan for audio pin:
	XIAO ESP32-S3 is the microcontroller and storage device for audio files. Will use the MAX 98357 amp
	to power a 8 ohm speaker. A 3.7 li-ion or LiPo battery will power the project. I will solder all together, and use
	hydrophobic audio mesh all around a 3D printed case to help ensure that any sweat/water will not take out the pin.	

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
	
