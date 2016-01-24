# MorseSender
This sketch takes the serial output from the enigma machine engine and flashes it in morse code.

This is part of a project to make the arduino enigma machine simulator able to transmit text via morse code.
The final architecture will be the <a href="http://arduinoenigma.blogspot.com/">Arduino Enigma Machine Simulator</a>, connected via a USB cable to an Arduino Pro Mini with a USB Host Shield.
The Arduino Pro Mini will act as a host and open a serial connection to the Arduino Uno Enigma Machine. When a letter is encoded in the simulator, 
it will be received by the Pro Mini and flashed via the LED.

USB Host Shield:
<a href="https://www.circuitsathome.com/products-page/arduino-shields/usb-host-shield-for-arduino-pro-mini">https://www.circuitsathome.com/products-page/arduino-shields/usb-host-shield-for-arduino-pro-mini</a>

To use this, create a new sketch with two files, MorseSender and MorseFunctions. Save this sketch as MorseSender.

Upload to an Arduino Uno or a Pro Mini.

Connect to it using the Serial Monitor at 9600.

This sketch reads the output of the arduino enigma machine simulator in verbose mode and flashes the output letter. 
It does that by parsing the following string and outputing the letter after the second "Stecker>" string.
The final letter (C) will be flashed when this string is sent.
Q>Stecker>Q>ETW>Q>R1>V>R2>Y>R3>C>R4>Y>UKW>G>R4>U>R3>R>R2>G>R1>C>ETW>C>Stecker>C

To test by hand, the minimal string to send is:
er>er>X
This will flash the letter X

Send the following three lines one at a time.
er>er>S (click Send or press enter)
er>er>O
er>er>S
The built in LED should flash the familiar SOS pattern short short short long long long short short short
