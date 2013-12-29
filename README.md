arduino-babymcast
=================

Detect your baby over multicast! This project just sends multicast packets
given a specific threshold.

To use it; Connect it to the network, calibrate the sensitivity with
the little gain screw, and listen to the MCAST UDP group:

  * 239.255.47.79:61391

## Dependencies

This project doesn't use the arduino SDK or ino. Just regular makefiles and
the avr* toolset.

### Software

  - avrdude
  - avrlibcgcc
  - picocom

### Hardware parts

  - Arduino UNO, ATmega328p compatible
  - Ethernet shield, W5100+ compatible
  - Microphone with gain control(variable resistor)[SKU135533]

SKU135533: http://dx.com/p/arduino-microphone-sound-detection-sensor-module-red-135533
