#include <ArduinoBLE.h>
#include <Arduino.h>
#include <SD.h>

using namespace std; 

int dataLength = 300;

const char openingBracket[] PROGMEM = "{";
const char closingBracket[] PROGMEM = "}";

const char serviceID[] PROGMEM = "19B10000-E8F3-537E-4F6C-D194768A2214";
const char characteristicID[] PROGMEM = "19B10001-E8F2-537E-4F6C-D104768A1214";

BLEService DiveDataService(serviceID);
BLECharacteristic DiveDataCharacteristic( characteristicID, BLERead | BLENotify,512,(1==1));

const char directoryPath2[] PROGMEM = "logFiles";
char logfilePath2[30];

//-------BLE--------
//initializing the BLE unit
void inizializeBLE() 
{
    if (!BLE.begin()) 
    {
        Serial.println("starting BLE failed!");
        while (1);
    }
    BLE.setLocalName("DiveComputer");
    
    DiveDataService.addCharacteristic(DiveDataCharacteristic);
    BLE.addService(DiveDataService);
    BLE.setAdvertisedService(DiveDataService);
    DiveDataCharacteristic.broadcast();
    
    BLE.setConnectionInterval(0xF0, 0x0c80);
    BLE.advertise();  
    Serial.println("BLE active"); //uetooth device active, waiting for connections...");
}

//this function did cost a LOT of time and nerves (traditionally)
bool getFirstLineAndDelete(char filename[], char* result)
{
    Serial.println(filename);
    File file = SD.open(filename, FILE_WRITE_TRAD);
    if(file) 
    {
        file.seek(0);
        int i = 0;
        while (file.available()) 
        {
            char c[2];
            file.read(c, 2);
            if(c[0] != '*') 
            {
                file.seek(i);
                file.readStringUntil('\n').toCharArray(result, 9);
                file.seek(i);
                file.print('*');
                file.close();
                return true;
            } 
            else 
            {
                file.readStringUntil('\n');
            }
            i+=10;
        } 
    }  
    return false;
} 

void addBracketsToDate(char* result, int resultSize, const char* b1, char date[], const char* b2) 
{
    snprintf(result, resultSize, "%s%s%s", b1, date, b2);
}

void buildBluetoothConnectionTesting() {

  BLEDevice central = BLE.central();
  float _accelerator_x = 0.04f;
  float _accelerator_y = 0.02f;
  float _accelerator_z = 100.83f;
  float _depth = 34.92f;
  float _duration = 232023.f;
  float _gyroscope_x = 0.324f;
  float _gyroscope_y = 0.367f;
  float _gyroscope_z = 0.546f;
  int _heart_freq = 1;
  int _heart_var = 80;
  int _luminance = 120;
  int _oxygen_saturation = 98;
  int _ref_dive = 1;
  float _water_temp = 23.75;

  if(central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    
    while(central.connected()) {
      
      char json[500] = "";
      for (size_t u = 0; u < 3; u++)
      { 
        _ref_dive++;
        _heart_freq++;
             
        char jsonPart[20];              
        snprintf(jsonPart, 20, "{\"1\":%.4f", _accelerator_x);
        strcat(json, jsonPart);      
        snprintf(jsonPart, 20, ",\"2\":%.4f", _accelerator_y);
        strcat(json, jsonPart);      
        snprintf(jsonPart, 20, ",\"3\":%.4f", _accelerator_z);
        strcat(json, jsonPart);      
        snprintf(jsonPart, 20, ",\"4\":%.4f", _depth);
        strcat(json, jsonPart);      
        snprintf(jsonPart, 20, ",\"5\":%.4f", _duration);
        strcat(json, jsonPart);      
        snprintf(jsonPart, 20, ",\"6\":%.4f", _gyroscope_x);
        strcat(json, jsonPart);      
        snprintf(jsonPart, 20, ",\"7\":%.4f", _gyroscope_y);
        strcat(json, jsonPart);      
        snprintf(jsonPart, 20, ",\"8\":%.4f", _gyroscope_z);
        strcat(json, jsonPart);      
        snprintf(jsonPart, 20, ",\"9\":%d", _heart_freq);
        strcat(json, jsonPart);      
        snprintf(jsonPart, 20, ",\"10\":%d", _heart_var);
        strcat(json, jsonPart);      
        snprintf(jsonPart, 20, ",\"11\":%d", _luminance);
        strcat(json, jsonPart);      
        snprintf(jsonPart, 20, ",\"12\":%d", _oxygen_saturation);        
        strcat(json, jsonPart);      
        snprintf(jsonPart, 20, ",\"13\":%d", _ref_dive);
        strcat(json, jsonPart);
        snprintf(jsonPart, 20, ",\"14\":%.4f", _water_temp);
        strcat(json, jsonPart);    
        strcat(json, "}");
      }
      
      DiveDataCharacteristic.writeValue(json);
      //
		  //getJson(json, _accelerator_x, _accelerator_y, _accelerator_z, _depth, _duration, _gyroscope_x, _gyroscope_y, _gyroscope_z, _heart_freq, _heart_var, _luminance, _oxygen_saturation, _ref_dive, _water_temp);
		  Serial.print("json in ble: ");
      Serial.println(json);
      delay(200);
    }
    central.disconnect();
  }
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());
}

void bluetoothTraditionell()
{
  BLEDevice central = BLE.central();  

  if(central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    
    while(central.connected()) {
      
      char json[500] = "";
      char path[] = "logfiles/45_03_21.log";
      char jsonPart[190]  = "";

      //snprintf(path, 30, "logfiles/45_03_21.log");

      File mFile = SD.open(path);
      Serial.println("ja");
      if (mFile)
      {
        Serial.println("nein");
        while (mFile.available())
        {
          for (size_t u = 0; u < 3; u++)
          { 
            mFile.readStringUntil('\n').toCharArray(jsonPart, 160);
            Serial.println(jsonPart);
            delay(100);
            strcat(json, jsonPart);          
          }
          DiveDataCharacteristic.writeValue(json);
          Serial.print("json in ble: ");
          Serial.println(json);
          delay(200);
        }
      }
    }
    central.disconnect();
  }
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());
}

void buildBluetoothConnection() 
{
  BLEDevice central = BLE.central();  
  
  if (central) 
  {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    char date[10];
    bool fileAvailable;
    
    int count = 0;
    while(central.connected())
    { 
      count++;

      fileAvailable = getFirstLineAndDelete("Sessions.log", date);
      Serial.print("Das ist log: ");
      Serial.println(date);
      
      if(true) 
      {
        char dateString[12];
        addBracketsToDate(dateString, sizeof dateString, openingBracket, date, closingBracket);
        DiveDataCharacteristic.writeValue(dateString);
        
        Serial.print("Aktueller dateString: ");
        Serial.println(dateString);
        
        //snprintf(logfilePath2, 30, "logfiles/%s.log", date);
        
        snprintf(logfilePath2, 25, "logfiles/%s.log", date);
        char testPath[] = "logfiles/44_03_21.log";

        Serial.print("Aktueller logpath: ");
        Serial.println(logfilePath2);      

        File file = SD.open(testPath);

        if(file) 
        {
          Serial.println("file traditionell geöffnet");
          int i = 0;
          char data[512] PROGMEM;
          char tmp[170] PROGMEM;
          while (file.available()) 
          {
            if(i % 3 == 0)
            {
              file.readStringUntil('\n').toCharArray(data, 512);
              // Serial.print("modulo 3 = 0: ");
              // Serial.println(data);
            } 
            else if(i % 3 == 1)
            {
              file.readStringUntil('\n').toCharArray(tmp, 170);
              strcat(data, tmp);
              //snprintf(data, 420, tmp);
              memset(tmp, 0, 170);
              // Serial.print("modulo 3 = 1 ");
              // Serial.println(data);
            }
            else if(i % 3 == 2)
            {
              file.readStringUntil('\n').toCharArray(tmp,170);
              strcat(data, tmp);
              //snprintf(data, 512, tmp);              
              Serial.println("sending: ");
              DiveDataCharacteristic.writeValue(data);
              memset(data, 0, 512);
            }
            i++;
          }
        }
      }
      else 
      {
        //central.disconnect();     
      }
      delay(666);
    }
    //central.disconnect();
  }
  
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());
}