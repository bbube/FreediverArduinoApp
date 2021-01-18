#include <Arduino.h>
#include <ezButton.h>
#include <SparkFun_Bio_Sensor_Hub_Library.h>
#include <string>
#include <SPI.h>
#include <SD.h>
#include <iostream>
#include <fstream>
#include <Arduino_LSM6DS3.h>

using namespace std;

#define LOOP_STATE_STOPPED 0
#define LOOP_STATE_STARTED 1

bool directoryCreated = false;

int resPin = 6;
int mfioPin = 5;
int btnPressed = -1;
int sessionID = 0;
int diveID = 1;

float accX, accY, accZ;
float gyrX, gyrY, gyrZ;

String sessionPath = "session.txt";
String divePath = "dive.txt";
String logPath;

const int chipSelect = 4;

SparkFun_Bio_Sensor_Hub bioHub(resPin, mfioPin);

bioData body;

File dataFile;

//<button>
ezButton button(8);  // create ezButton object that attach to pin 7;
int loopState = LOOP_STATE_STOPPED;
//</button>


int getSessionID() {
  File file = SD.open(sessionPath);
  if(file) {
    int x = file.parseInt();
    file.close();
    return x;
  }
  return -1;
}

int getDiveID() {
  File file = SD.open(divePath);
  if(file) {
    int x = file.parseInt();
    file.close();
    return x;
  }
  return -1;
}

void setSessionID(int x = 0) {
  File file = SD.open(sessionPath, FILE_WRITE);
  if(file) {
    file.print(sessionID + x);
    file.close();
  }
}

void setDiveID(int x = 0) {
  File file = SD.open(divePath, FILE_WRITE);
  if(file) {
    file.print(diveID + x);
    file.close();
  }
}

void createDirectory(int x) {
  SD.mkdir(to_string(x).c_str());
  Serial.println("directory created");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  button.setDebounceTime(50);

  // ---------- Hearbeat etc -----------
  Wire.begin();
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

  // Data lags a bit behind the sensor, if you're finger is on the sensor when
  // it's being configured this delay will give some time for the data to catch
  // up. 
  Serial.println("Loading up the buffer with data");
  //delay(4000);

  //Initialize IMU for gyroscope and accellerator
  if(!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while(1);
  }
  else {
    Serial.println("IMU initialized");
  }

  //-------SD----------
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  SD.begin(chipSelect);
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println(" card initialized.");

    
    //File sessionFile = SD.open(sessionPath.c_str(),FILE_WRITE);
    //sessionFile.println(to_string(420).c_str());
    //sessionFile.close();

  
  //-------META files---------
  //Check if the Meta-Files "sessionID" and "diveID" exist. IF yes their current
  //values are saved in sessionID and diveID. IF not they are created.
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
  

  logPath = String("") + sessionID + "/" + getDiveID() + ".txt";
  Serial.println(logPath);
  
}

void loop() {

  button.loop(); // MUST call the loop() function first
  String dataString = "";

  if (button.isPressed()) {
    if (loopState == LOOP_STATE_STOPPED){
      loopState = LOOP_STATE_STARTED;
      createDirectory(sessionID);
      logPath = String("") + sessionID + "/" + diveID + ".txt";
    }
    else if(loopState == LOOP_STATE_STARTED){
      loopState = LOOP_STATE_STOPPED;
      dataFile.close();
      diveID++;
    }
  }

  
  
  if (loopState == LOOP_STATE_STARTED) {

    body = bioHub.readBpm();
   
    if(IMU.accelerationAvailable()) {
      IMU.readAcceleration(accX, accY, accZ);
    }
    if(IMU.gyroscopeAvailable()) {
      IMU.readGyroscope(gyrX, gyrY, gyrZ);
    }

    

    //building the data-string for the log file:
    //time, heartrate, oxygen, confidence, depth, brightness, 
    dataString = String("") + millis() + "," + body.heartRate + ","
                  + body.oxygen + "," + body.confidence + "," 
                  + (random(0, 500)/10) + "," + random(0, 255)
                  + "," + (random(0, 300)/10) + "," + accX  + "," + accY
                  + "," + accZ + "," + gyrX + "," + gyrY + "," + gyrZ; 
    
    dataFile = SD.open(logPath, FILE_WRITE);
    if(dataFile){
        dataFile.println(dataString); //dataString.c_str());
        dataFile.close();
        Serial.println("data logged...");
    }
    else {
      Serial.println("error opining logfile");
    }

    delay(100);
  }


}
