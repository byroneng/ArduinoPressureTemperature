# Arduino Pressure and Temperature Sensor
## Equipment needed:
- Arduino Feather 32u4 
- Adafruit Featherwing OLED 
- Adafruit MPL3115A2

This code displays a simple running average (10 seconds, sampled at 1 Hz) for pressure, and displays temperature at 1 Hz. The A and B buttons are used to cycle up & down through 3 sets of units:
- 1: Pressure in Pa, Temperature in F
- 2: Pressure in mmHg, Temperature in C
- 3: Pressure in inHg, Temperature in K

*Note: Button C was disabled due to hardware failure.
