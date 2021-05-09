# alarm velo

## Simple Bike alarm 

Made with a esp8266 wemos d1 mini and a MPU6050

Libraries extracted from : https://github.com/jrowberg/i2cdevlib
just extract the ic2dev and mpu6050 libraries from the arduino repertory

### Connections for the wemos D1 mini 
* GND to battery negative
* +5V to switch 
* switch to battery positive
* D5 to out of the speaker schematics below

### for the MPU6050
* D1 to SCL
* D2 to SDA
* Connect VCC to VIN
* Connect gnd to GND 

You can use a battery pack to power the alarm. I made a little battery with this cheap circuit from aliexpress [step up module](https://www.aliexpress.com/item/32998389598.html) and a 18650 battery

![Speaker schematics](./speaker.jpg)
