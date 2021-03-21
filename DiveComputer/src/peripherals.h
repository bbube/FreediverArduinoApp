#include <SparkFun_Bio_Sensor_Hub_Library.h>
#include <Arduino_LSM6DS3.h>
#include <SD.h>
#include <SPI.h>
#include <ezButton.h>

#define LOOP_STATE_STOPPED 0
#define LOOP_STATE_STARTED 1

int loopStateDive = LOOP_STATE_STOPPED;
int loopStateBluetooth = LOOP_STATE_STOPPED;

const int resPin = 6;
const int mfioPin = 5;
const int chipSelect = 4;

File dataFile;

ezButton btnDive(8);
ezButton btnBluetooth(7);

SparkFun_Bio_Sensor_Hub bioHub(resPin, mfioPin);
bioData body;

//-------biohub sensor--------
//initializes the biohub-sensor
void initializeBioHubSensor() 
{
    int result = bioHub.begin();
    if (result == 0) // Zero errors!
      Serial.println("Sensor started!");
    else
      Serial.println("Could not communicate with the sensor!!!");
    
    Serial.println("Configuring Sensor...."); 
    int error = bioHub.configBpm(MODE_ONE); // Configuring just the BPM settings. 
    if(error == 0) // Zero errors!
    { 
      Serial.println("Sensor configured");
    }
    else 
    {
      Serial.println("Error configuring sensor.");
      Serial.print("Error: "); 
      Serial.println(error); 
    }
}

//-------IMU--------
//Initialize IMU for gyroscope and accellerator
void initializeIMU() 
{
    if(!IMU.begin()) 
    {
      Serial.println("Failed to initialize IMU!");
      while(1);
    }
    else 
    {
      Serial.println("IMU initialized");
    }
}

//-------SD card--------
//initializes the SD card
void initializeSD() 
{
    Serial.print("Initializing SD card...");
    // see if the card is present and can be initialized:
    SD.begin(chipSelect);
    if (!SD.begin(chipSelect)) 
    {
      Serial.println("Card failed, or not present");
      // don't do anything more:
      while (1);
    }
    Serial.println(" card initialized.");
}