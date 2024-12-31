// Basic demo for accelerometer readings from Adafruit MPU6050

// ESP32 Guide: https://RandomNerdTutorials.com/esp32-mpu-6050-accelerometer-gyroscope-arduino/
// ESP8266 Guide: https://RandomNerdTutorials.com/esp8266-nodemcu-mpu-6050-accelerometer-gyroscope-arduino/
// Arduino Guide: https://RandomNerdTutorials.com/arduino-mpu-6050-accelerometer-gyroscope/
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
float pitch, roll;

float accelXOffset = 0.0;
float accelYOffset = 0.0;
float accelZOffset = 0.0;

float rollOffset = 0.0;
float pitchOffset = 0.0;

/* Get new sensor events with the readings */
sensors_event_t a, g, temp;

const int dangerousNeckAngle = 19;


enum Neck_State{SAFE_STATE, INTERMEDIATE_STATE, DANGEROUS_STATE};
int neck_state = SAFE_STATE;
int danger_timer_start = 0;
float danger_duration = 0;
const int MINIMAL_STRETCH_DURATION = 10*1000; // in ms
int stretch_timer_start = 0;

float convertAccelerationToPitch(sensors_vec_t accel){
  // Calculate pitch and roll angles from accelerometer data
  float accelX = accel.x;
  float accelY = accel.y;
  float accelZ = accel.z;
  float Pitch = atan2(-accelX, sqrt(accelY * accelY + accelZ *accelZ));
  Pitch = Pitch*180/M_PI;
  // Serial.println("KNTL: " +String(Pitch));
  delay(100);
  return Pitch;
}

float convertAccelerationToRoll(sensors_vec_t accel){
  // SerialBT.println(St+ring(pitch*180/M_PI) + "\t" + String(roll*180/M_PI));
  // Calculate pitch and roll angles from accelerometer data
  float accelX = accel.x;
  float accelY = accel.y;
  float accelZ = accel.z;
  float Roll = atan2(accelY, accelZ);
  Roll = Roll*180/M_PI;
  delay(100);
  return Roll;
}

void calibrateAccelerometer() {
  mpu.getEvent(&a, &g, &temp);

  pitchOffset = convertAccelerationToPitch(a.acceleration);
  rollOffset  = convertAccelerationToRoll(a.acceleration);
  Serial.println("pitchOffset: " + String(pitchOffset) + "\trollOffset: " + String(rollOffset));
}


float abs_float(float n){
  if(n < 0){
    return -n;
  }
  return n;
}

