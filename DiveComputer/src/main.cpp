#include <Arduino.h>
#include "peripherals.h"
#include "json.h"
#include "bluetooth.h"
#include "fileSystem.h"
using namespace std;


float accX, accY, accZ;
float gyrX, gyrY, gyrZ;

char jsonString[200];

// Reads data from the sensors and calls the method "createJsonString" with the data as 
// parameter.
void getData(char* jsonString) 
{  
    body = bioHub.readBpm();
    if (IMU.accelerationAvailable()) 
    {
        IMU.readAcceleration(accX, accY, accZ);
    }
    if (IMU.gyroscopeAvailable()) 
    {
        IMU.readGyroscope(gyrX, gyrY, gyrZ);
    }
    createJsonString(
                    accX, 
                    accY, 
                    accZ,
                    (float)random(0, 50),
                    millis(),
                    gyrX, 
                    gyrY, 
                    gyrZ,
                    body.heartRate, 
                    body.confidence, 
                    random(0,255), 
                    body.oxygen, 
                    diveID,
                    (float)random(0,30),                     
                    jsonString);
    Serial.println(body.heartRate);
}                         

// This function saves a String (Json-string with our data) in a file on the SD card.
void logData(char* data) 
{
    dataFile = SD.open(logPath, FILE_WRITE);
    if (dataFile)
    {
        dataFile.println(data);
        dataFile.close();
        Serial.println("data logged...");
        delay(50);
    }
    else 
    {
        Serial.println("error opening logfile");
    }
}

// This function inserts the start time of a dive into the logfile.
void insertSpacer() 
{
    File file = SD.open(logPath, FILE_WRITE);
    if (file) 
    {
        file.println("#12:33:24");
    }
    file.close();
}

// This function is for the "start dive" - button on the test board.
// If its a new session the date is written to "sessions.log". We do this here
// so that we only add a session if there is at least one dive.
void btnDiveClickListener() 
{
    btnDive.loop(); // must call the loop() function first
    if (btnDive.isPressed() && btnDive.getStateRaw()==0)
    {
        if (loopStateDive == LOOP_STATE_STOPPED)
        {
            loopStateDive = LOOP_STATE_STARTED;
            if (!sameSession) 
            {
              writeDateToSessionFile(date);
              Serial.println("sd card crashed");
              sameSession = true;
            }
            insertSpacer();
        }
        else if (loopStateDive == LOOP_STATE_STARTED)
        {
            loopStateDive = LOOP_STATE_STOPPED;
            diveID++;
            setDiveID();
        }
    }
}

//To activate the bluetooth functionality manually with a button.
void btnBluetoothClickListener() 
{
    btnBluetooth.loop(); // must call the loop() function first
    if (btnBluetooth.isPressed() && btnBluetooth.getStateRaw() == 0) 
    {
        if (loopStateBluetooth == LOOP_STATE_STOPPED)
        {
            loopStateBluetooth = LOOP_STATE_STARTED;
        }
        else if (loopStateBluetooth == LOOP_STATE_STARTED)
        {
            loopStateBluetooth = LOOP_STATE_STOPPED;
        }
    }
}

//In the setup method everything is initialized.
void setup() 
{ 
    delay(1000); 
    Serial.begin(1200);
    Serial.flush();
    Wire.begin();
    btnDive.setDebounceTime(5);
    btnBluetooth.setDebounceTime(20);
    
    initializeBioHubSensor();
    initializeIMU();
    initializeSD();
    initializeMetaData();
    inizializeBLE();
    createDirectory();
    Serial.println("beginn testing");
    
    snprintf(logPath, 30, "logfiles/%s.log", date);

    //remove /////////////////////////////////////////////
    if(SD.exists("sessions.log"))
    {
        SD.remove("sessions.log");
    }
    File file2 = SD.open("sessions.log", FILE_WRITE);    
    if (file2)
    {
        file2.println("01_04_21");        
    }
    file2.close();
    //remove /////////////////////////////////////////////
    
}


void loop() 
{
    btnDiveClickListener();
    btnBluetoothClickListener();
    
    if (loopStateBluetooth == LOOP_STATE_STARTED) 
    {
        buildBluetoothConnection();  
        //buildBluetoothConnectionTesting();
        delay(200);
    } 
    else if (loopStateDive == LOOP_STATE_STARTED) 
    {      
        getData(jsonString);      
        logData(jsonString);
    }
}