#include <Arduino.h>
#include "peripherals.h"
#include "json.h"
#include "bluetooth.h"
#include "metafiles.h"
using namespace std;


float accX, accY, accZ;
float gyrX, gyrY, gyrZ;

String getData() {

  body = bioHub.readBpm();
   
  if(IMU.accelerationAvailable()) {
    IMU.readAcceleration(accX, accY, accZ);
  }
  if(IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gyrX, gyrY, gyrZ);
  }
  return createJsonString(diveID, millis(), body.heartRate, body.oxygen, body.confidence, (random(0, 1000)/10), random(0,255), (random(0,300)/10), accX, accY, accZ, gyrX, gyrY, gyrZ);
}                         

void logData(String data) {

  dataFile = SD.open(logPath, FILE_WRITE);
  if(dataFile){
      dataFile.println(data); //dataString.c_str());
      dataFile.close();
      Serial.println("data logged...");
      delay(50);
  }
  else {
    Serial.println("error opening logfile");
  }
}

void btnDiveClickListener() {

  btnDive.loop(); // must call the loop() function first
  if (btnDive.isPressed() && btnDive.getStateRaw()==0) {
    if (loopStateDive == LOOP_STATE_STOPPED){
        loopStateDive = LOOP_STATE_STARTED;
    }
    else if(loopStateDive == LOOP_STATE_STARTED){
      loopStateDive = LOOP_STATE_STOPPED;
      diveID++;
      setDiveID();
    }
  }
}

void btnBluetoothClickListener() {
  btnBluetooth.loop(); // must call the loop() function first
  if (btnBluetooth.isPressed() && btnBluetooth.getStateRaw()==0) {
    if (loopStateBluetooth == LOOP_STATE_STOPPED){
        loopStateBluetooth = LOOP_STATE_STARTED;
    }
    else if(loopStateBluetooth == LOOP_STATE_STARTED){
      loopStateBluetooth = LOOP_STATE_STOPPED;
    }
  }
}

void setup() {
  
  Serial.begin(9600);
  Wire.begin();
  btnDive.setDebounceTime(5);
  btnBluetooth.setDebounceTime(20);

  initializeBioHubSensor();
  initializeIMU();
  initializeSD();
  initializeMetaData();
  inizializeBLE();
  createDirectory();

  logPath = String("") + directoryPath + "/" + date + ".log";
}

void loop() {

  btnDiveClickListener();
  btnBluetoothClickListener();

  if (loopStateBluetooth == LOOP_STATE_STARTED) {
    buildBluetoothConnection();    
    delay(100);
  } else if(loopStateDive == LOOP_STATE_STARTED) {
    logData(getData());
  }

}




