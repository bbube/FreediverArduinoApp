#include <ArduinoBLE.h>
#include <Arduino.h>
#include <SD.h>
#include <vector>

//#include <bits/stdc++.h> 
using namespace std; 

int dataLength = 300;

BLEService DiveData("19B10000-E8F3-537E-4F6C-D194768A2214");
BLECharacteristic DataStream( "19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify,350,(1==1));

String testData = "{\"ref_dive\":1,\"duration\":9438,\"heart_freq\":0,\"oxygen_saturation\":0,\"heart_var\":0,\"depth\":93,\"luminance\":78,\"water_temp\":6,\"accelerator_x\":0.030151,\"accelerator_y\":0.002563,\"accelerator_z\":1.012573,\"gyroscope_x\":1.037598,\"gyroscope_y\":-3.173828,\"gyroscope_z\":-1.647949}";
//std::vector<String> sessionsArray;


String sendMetaData() {

}


//-------BLE--------
//initializing the BLE unit
void inizializeBLE() {
  if (!BLE.begin()) 
  {
    Serial.println("starting BLE failed!");
    while (1);
  }
  BLE.setLocalName("DiveComputer");
  DiveData.addCharacteristic(DataStream);
  BLE.addService(DiveData);
  BLE.setAdvertisedService(DiveData);  
  DataStream.broadcast();
  BLE.advertise();  
  Serial.println("Bluetooth device active, waiting for connections...");
}

void sendData(String data) {
  //TODO
  DataStream.writeValue(data.c_str());
}


String getLastLineAndDeleteLine(String filename)
{
  std::vector<String> sessionArray;
  String data;
  File file = SD.open(filename);
  if(file) {
    while (file.available()) {
      data = file.readStringUntil('\n');
      sessionArray.push_back(data);
    }
  }
  file.close();
  SD.remove(filename);
  File newFile = SD.open(filename, FILE_WRITE);
  
  if(newFile) {
    for (int i = 0; i < sessionArray.size()-1; i++)
    {
      newFile.print(sessionArray[i]);
    }
  }
  newFile.close();
  return sessionArray[sessionArray.size()-1];
} 


void buildBluetoothConnection() {

  BLEDevice central = BLE.central();  

  if (central) 
  {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    String log;
    while(central) {
  
        log = getLastLineAndDeleteLine("Sessions.log");
        sendData(log);
        File file = SD.open(log + ".log");
        String data;
        if(file) {
          while (central && file.available()) {
            data = file.readStringUntil('\n');
            Serial.print(data);
            sendData(data);
          }
        }        
        delay(200);
    } 
      
    
  }
  
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());
}