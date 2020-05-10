////////////////////INITIALIZTIONS///////////////////
//IMU intializations
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU9150Lib.h"
#include "CalLib.h"
#include <dmpKey.h>
#include <dmpmap.h>
#include <inv_mpu.h>
#include <inv_mpu_dmp_motion_driver.h>
#include <EEPROM.h>
#define  DEVICE_TO_USE    0
float pitch,roll,yaw;
MPU9150Lib MPU;                                              // the MPU object
//  MPU_UPDATE_RATE defines the rate (in Hz) at which the MPU updates the sensor data and DMP output
#define MPU_UPDATE_RATE  (20)
//  MAG_UPDATE_RATE defines the rate (in Hz) at which the MPU updates the magnetometer data
//  MAG_UPDATE_RATE should be less than or equal to the MPU_UPDATE_RATE

#define MAG_UPDATE_RATE  (10)

//  MPU_MAG_MIX defines the influence that the magnetometer has on the yaw output.
//  The magnetometer itself is quite noisy so some mixing with the gyro yaw can help
//  significantly. Some example values are defined below:

#define  MPU_MAG_MIX_GYRO_ONLY          0                   // just use gyro yaw
#define  MPU_MAG_MIX_MAG_ONLY           1                   // just use magnetometer and no gyro yaw
#define  MPU_MAG_MIX_GYRO_AND_MAG       10                  // a good mix value 
#define  MPU_MAG_MIX_GYRO_AND_SOME_MAG  50                  // mainly gyros with a bit of mag correction 

//  MPU_LPF_RATE is the low pas filter rate and can be between 5 and 188Hz
#define MPU_LPF_RATE   40

//  SERIAL_PORT_SPEED defines the speed to use for the debug serial port
#define  SERIAL_PORT_SPEED  115200




//My Intializations
#define button 12
#define LPFPitch_Beta 0.9   //low pass beta (beta should be fro 0 to 1, if increased it give more reliable but slower readings)
#define LPFYaw_Beta 0.99   //low pass beta (beta should be fro 0 to 1, if increased it give more reliable but slower readings)
#include <Servo.h> 
Servo pitchServo;
Servo yawServo;
int pitchOut,yawOut,i;
float pitchSetPoint,yawSetPoint;
float smoothYaw, smoothPitch;   //variable after low pass filter
void readIMU();
void setPosition();


   
////////////////////SET UP///////////////////
void setup() {
  //IMU setup
  Serial.begin(SERIAL_PORT_SPEED);
 // Serial.print("Arduino9150 starting using device "); Serial.println(DEVICE_TO_USE);
  Wire.begin();
  MPU.selectDevice(DEVICE_TO_USE);                        // only really necessary if using device 1
  MPU.init(MPU_UPDATE_RATE, MPU_MAG_MIX_GYRO_AND_MAG, MAG_UPDATE_RATE, MPU_LPF_RATE);   // start the MPU
  
  //My setup
  
  //pitchServo.attach(9);
  yawServo.attach(10);
  pinMode(button,INPUT);
  for (i=0;i<150; i++)
      readIMU();
  setPosition();    //start the code by setting the set point
  
}




////////////////////LOOP///////////////////
void loop() {

 if (digitalRead(button)==HIGH)
    setPosition();
 for (i=0; i<2; i++)
    readIMU();
 
 Serial.print("Pitch difference\t");
 Serial.println(pitchSetPoint-smoothPitch); //Serial.print(",");
 if (abs(pitchSetPoint-smoothPitch)>1)    //to illimunate movements due to sall variations (inaccuracy)
 {
    pitchOut+=(pitchSetPoint-smoothPitch);  //plus or minus depends on postition of IMU
    pitchOut=constrain(pitchOut,0,180);
    Serial.print("pitchOut\t");
    Serial.println(pitchOut); 
    pitchServo.write(pitchOut);
    delay(15);
 }
 
 
 Serial.print("yaw difference\t");
 Serial.println(yawSetPoint-smoothYaw); //Serial.print(",");
 if (abs(yawSetPoint-smoothYaw)>1)
 {
    yawOut+=yawSetPoint-smoothYaw;  //plus or minus depends on postition of IMU
    yawOut=constrain(yawOut,0,180);
    Serial.print("yawOut\t");
    Serial.println(yawOut); //Serial.print(",");
    yawServo.write(yawOut);
    delay(15);
 }
 
}




