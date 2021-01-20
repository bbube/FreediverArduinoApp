#include <Arduino.h>
#include <ezButton.h>
#include <SparkFun_Bio_Sensor_Hub_Library.h>
#include <string>
#include <SPI.h>
#include <SD.h>
#include <iostream>
#include <fstream>
#include <Arduino_LSM6DS3.h>
#include <ArduinoBLE.h>

using namespace std;

#define LOOP_STATE_STOPPED 0
#define LOOP_STATE_STARTED 1

bool diveStarted = false;

int resPin = 6;
int mfioPin = 5;
int btnPressed = -1;
int sessionID = 0;
int diveID = 1;

float accX, accY, accZ;
float gyrX, gyrY, gyrZ;

String sessionPath = "session.log";
String divePath = "dive.log";
String logPath;

const int chipSelect = 4;

SparkFun_Bio_Sensor_Hub bioHub(resPin, mfioPin);
bioData body;

BLEService DiveData("19B10000-E8F3-537E-4F6C-D194768A2214");
BLEUnsignedCharCharacteristic DataStream("2101", BLERead | BLENotify);

File dataFile;

ezButton btnDive(8);
ezButton btnBluetooth(7);
int loopState = LOOP_STATE_STOPPED;


//returns the sessionID from the sessionID-file
//returns -1 if failed to open the file
int getSessionID() {
  File file = SD.open(sessionPath);
  if(file) {
    int x = file.parseInt();
    file.close();
    return x;
  }
  return -1;
}

//returns the diveID from the diveID-file
//returns -1 if failed to open the file
int getDiveID() {
  File file = SD.open(divePath);
  if(file) {
    int x = file.parseInt();
    file.close();
    return x;
  }
  return -1;
}

//writes the value of variable sessionID (+ parameter if given) into the sessionID-file
void setSessionID(int x = 0) {
  File file = SD.open(sessionPath, FILE_WRITE);
  if(file) {
    file.print(sessionID + x);
    file.close();
  }
}

//writes the value of variable sessionID (+ parameter if given) into the sessionID-file
void setDiveID(int x = 0) {
  File file = SD.open(divePath, FILE_WRITE);
  if(file) {
    file.print(diveID + x);
    file.close();
  }
}

//creates a directory with given parameter as name
void createDirectory(int x) {
  SD.mkdir(to_string(x).c_str());
  Serial.println("directory created");
}

//-------biohub sensor--------
//initializes the biohub-sensor
void initializeBioHubSensor() {
  int result = bioHub.begin();
  if (result == 0) // Zero errors!
    Serial.println("Sensor started!");
  else
    Serial.println("Could not communicate with the sensor!!!");
 
  Serial.println("Configuring Sensor...."); 
  int error = bioHub.configBpm(MODE_ONE); // Configuring just the BPM settings. 
  if(error == 0){ // Zero errors!
    Serial.println("Sensor configured");
  }
  else {
    Serial.println("Error configuring sensor.");
    Serial.print("Error: "); 
    Serial.println(error); 
  }
}

//-------IMU--------
//Initialize IMU for gyroscope and accellerator
void initializeIMU() {
  if(!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while(1);
  }
  else {
    Serial.println("IMU initialized");
  }
}

//-------SD card--------
//initializes the SD card
void initializeSD() {
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  SD.begin(chipSelect);
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println(" card initialized.");
}

//-------META files--------
//Check if the meta-Files "sessionID" and "diveID" exist. IF yes their current
//values are saved in sessionID and diveID. IF not they are created.
void initializeMetaFiles() {
  if (SD.exists(sessionPath)) {
    sessionID = getSessionID() + 1;
    SD.remove(sessionPath);
    setSessionID();
  }
  else {
    setSessionID(1);
    sessionID = 1;
  }
  Serial.println("sessionID initialized");

  if (SD.exists(divePath.c_str())) {
    SD.remove(divePath.c_str());
  }
  setDiveID();
  Serial.println("diveID initialized");
}

//-------BLE--------
//initializing the BLE unit
void inizializeBLE() {
  if (!BLE.begin()) 
  {
    Serial.println("starting BLE failed!");
    while (1);
  }

  BLE.setLocalName("DiveComputerV2");
  DiveData.addCharacteristic(DataStream);
  BLE.addService(DiveData);
  BLE.setAdvertisedService(DiveData);  
  DataStream.broadcast();
  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
}

String getData() {
  body = bioHub.readBpm();
   
  if(IMU.accelerationAvailable()) {
    IMU.readAcceleration(accX, accY, accZ);
  }
  if(IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gyrX, gyrY, gyrZ);
  }

  //building the data-string for the log file:
  //time, heartrate, oxygen, confidence, depth, brightness, temperature, acceleration(x,y,z), gyroscope(x,y,z)
  return String("") + millis() + "," + body.heartRate + ","
                    + body.oxygen + "," + body.confidence + "," 
                    + (random(0, 500)/10) + "," + random(0, 255)
                    + "," + (random(0, 300)/10) + "," + accX  + "," + accY
                    + "," + accZ + "," + gyrX + "," + gyrY + "," + gyrZ; 
}

void logData(String data) {
  dataFile = SD.open(logPath, FILE_WRITE);
  if(dataFile){
      dataFile.println(data); //dataString.c_str());
      dataFile.close();
      Serial.println("data logged...");
  }
  else {
    Serial.println("error opining logfile");
  }
}

void btnDiveClickListener() {
  btnDive.loop(); // must call the loop() function first
  if (btnDive.isPressed() && btnDive.getStateRaw()==0) {
    if (loopState == LOOP_STATE_STOPPED){
        loopState = LOOP_STATE_STARTED;
        createDirectory(sessionID);
        logPath = String("") + sessionID + "/" + diveID + ".log";  
      
    }
    else if(loopState == LOOP_STATE_STARTED){
      loopState = LOOP_STATE_STOPPED;
      diveStarted = false;
      dataFile.close();
      diveID++;
    }
  }
}


void setup() {
  
  Serial.begin(9600);
  btnDive.setDebounceTime(5);
  btnBluetooth.setDebounceTime(20);
  Wire.begin();

  initializeBioHubSensor();
  initializeIMU();
  initializeSD();
  initializeMetaFiles();
  inizializeBLE();

  logPath = String("") + sessionID + "/" + getDiveID() + ".log";
  Serial.println(logPath);
  
}

void loop() {

  String dataString = "";

  btnDiveClickListener();

  if (loopState == LOOP_STATE_STARTED) {
    logData(getData());
    delay(100);
  }


}
