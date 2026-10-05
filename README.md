# 100_mile_audio_pin
This is a wearable, pre-recorded audio playback device that someone can wear for a run! The purpose for this is for my best friend, who is running 100 miles. They will be alone for most of it, so having a voice they can lean on when they need it is the goal!

## Getting Arduino running (debain, flatpak)
Run in the command line: flatpak run cc.arduino.IDE2

## Sources that I am using/found to help bring this project to life
	Storing audio files into flash memory through SPI
	https://forum.arduino.cc/t/how-to-upload-audio-file-in-external-flash-memory-using-arduino-zero-and-play-it/688078/14
	https://forum.arduino.cc/t/using-external-flash-as-an-alternative-to-sd-card/1079126/10
	https://github.com/PaulStoffregen/SerialFlash

## Arduino + SPI pin stuff
	Helpful Links I am using:
	- https://docs.arduino.cc/language-reference/en/functions/communication/SPI/
	- https://www.circuitbasics.com/how-to-set-up-spi-communication-for-arduino/
	- https://electricalflux.com/mcu-coding/spi-for-arduino-beginner-sensor-wiring-guide (very helpful, where i got level shifter knowledge from)	
	
	Hardware I am using:
	- Flash memory(using W25Q64JV, only difference is more storage): https://learn.adafruit.com/adafruit-spi-flash-breakouts/pinouts
	- Generic 4 way logic level shifter
	- Arduino Uno R3
	
	I am using a level shifter in-between the flash memory and the arduino uno. 
	- Pin 10 ->  LV1 -> CS
	- Pin 11 -> LV2 -> MOSI
	- Pin 12 -> LV3 -> MISO
	- Pin 13 -> LV4 -> SCK
	
	Going to start out with a basic transfer/seeing how this works.		

	UART tranfer (most likely the thing to help transfer files from comp to arduino
	- https://docs.arduino.cc/learn/communication/uart/
	
