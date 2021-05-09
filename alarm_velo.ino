/*
 * Alarm made with esp8266 wemos and MPU6050. 
 * Simple on and off switch
 * the thief won't know how to disable it ....
 * Will ring more and more each time you move
 * reset every minute 
 * 
 * 
*/

// Libraries extracted from arduino repertory in https://github.com/jrowberg/i2cdevlib
#include "I2Cdev.h"
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 accelgyro;

int16_t ax, ay, az, dx, dy, dz ;
int16_t gx, gy, gz, hx, hy, hz, jilt;

int buzzPin=14; // D5 on the wemos mini

int start = 0;
int total = 0;
bool sound = false ;// Set to true to really make noise (debug without noise is better...)
bool tentativeVol= false;
int deplacement = 0;
unsigned long myTime;
unsigned long delayTime;

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    Serial.begin(115200);
    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();
    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // configure buzzPin
     pinMode(buzzPin,OUTPUT);
     digitalWrite(buzzPin, LOW);

     // delay before alarm is on
     delay(2000);
}

void loop() {
    // read raw accel/gyro measurements from device
    dx=ax; dy=ay; dz=az; hx=gx; hy=gy; hz=gz;
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    dx = dx-ax; dy=dy-ay; dz=dz-az;
    hx=hx-gx; hy=hy-gy; hz=hz-gz;

    // Mean of the different accelerations....
    jilt = abs(dx/40)+abs(dy/40)+abs(dz/40)+ (abs(hx/6)+abs(hy/6)+abs(hz/6));

    Serial.print("a/g:\t");
    Serial.print(dx); Serial.print("\t"); Serial.print(dy); Serial.print("\t"); Serial.print(dz); Serial.print("\t");
    Serial.print(hx); Serial.print("\t"); Serial.print(hy); Serial.print("\t"); Serial.print(hz); Serial.print("\t");
    Serial.print(jilt); Serial.println("");

   // Threshold for mouvment 
   if (jilt>80) {
      if (start%2 != 0) {   
        total++;
        alarm();
        if (start > 10) start = 0;  
      }
      start++;
  }
  // delay between 2 measurements
  else delay(200);
}


void alarm () {
  if (!tentativeVol) {
    //first time 
    tentativeVol = true;
    myTime = millis();
    deplacement = 1;
    Serial.println("First time ");
    beep();
    
  } 
  else {
    deplacement++;
    delayTime = millis()-myTime;
    if (delayTime > 60000) {
      Serial.println(" First movment after 1 minute");
      beep();
      tentativeVol = true;
      deplacement= 1;
      myTime= millis();
      
    }
    else {
      Serial.print (" deplacement number :");
      Serial.println(deplacement);
      int i;
      for (i = 0; i< deplacement;i++) {
        //  will ring more and more within a minute
        beep();
        
      }
    }
  }
}  
void beep () {
  int j;
  Serial.print("Beeeep  start = ");
  Serial.println(start);
  if (sound) {
   for (j = 0; j < 12; j++){
    note (2000,15);
    note (4000,15);
    note (6000,15);
    note (4000,15);
    note (2000,15);
    note (4000,15);
    note (6000,15);
    note (4000,15);
    }
  }
  else {
    delay(1340);
  }
}
void note( int ton, int duree) {
  tone(buzzPin, ton);
  delay(duree);
  noTone(buzzPin);         
}
