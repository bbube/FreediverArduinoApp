#include <Arduino.h>
#include "peripherals.h"
#include "json.h"
#include "bluetooth.h"
#include "fileSystem.h"
using namespace std;


float accX, accY, accZ;
float gyrX, gyrY, gyrZ;

char jsonString[200];

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
    createJsonString(diveID, millis(), body.heartRate, body.oxygen, body.confidence, (random(0, 1000)/10), random(0,255), (random(0,300)/10), accX, accY, accZ, gyrX, gyrY, gyrZ, jsonString);
}                         

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

void insertSpacer() 
{
    File file = SD.open(logPath, FILE_WRITE);
    if (file) 
    {
        file.println("#Uhrzeit");
    }
    file.close();
}

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
              Serial.println("bin traditionell drinne in der olga");
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

void setup() 
{ 
    delay(1000); 
    Serial.begin(9600);
    //Serial.flush();
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
    
    /*
    File file2 = SD.open("sessions.log");    
    if (file2)
    {
        char x[10];
        while(file2.available())
        {
            file2.readStringUntil('\n').toCharArray(x, 10);
            Serial.println(x);
        }
    }
    file2.close();
    
    
    //logPath = String("") + directoryPath + "/" + date + ".log";
    //snprintf(logPath, 30, "logfiles/%s.log", date);
    // Serial.println(logPath);


    char ch1[] = "equal";
    char ch2[] = "equal";

    Serial.print("is equal: ");
    Serial.println(strcmp(ch1,ch2));

    char ch3[] = "equal";
    char ch4[] = "notequal";

    Serial.print("is unequal: ");
    Serial.println(strcmp(ch3,ch4));
    */
}

void loop() 
{
  btnDiveClickListener();
  btnBluetoothClickListener();

  if (loopStateBluetooth == LOOP_STATE_STARTED) 
  {
      //buildBluetoothConnection();  
      bluetoothTraditionell();
      delay(200);
  } 
  else if (loopStateDive == LOOP_STATE_STARTED) 
  {
	  getData(jsonString);
	  logData(jsonString);
  }
}