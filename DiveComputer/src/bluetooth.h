#include <ArduinoBLE.h>
#include <Arduino.h>
#include <SD.h>

using namespace std; 

int dataLength = 300;

char openingBracket[] = "{";
char closingBracket[] = "}";

BLEService DiveDataService("19B10000-E8F3-537E-4F6C-D194768A2214");
BLECharacteristic DiveDataCharacteristic( "19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify,512,(1==1));

char directoryPath2[] = "logFiles";
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
    Serial.println("Bluetooth device active, waiting for connections...");
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

void addBracketsToDate(char* result, int resultSize, char b1[], char date[], char b2[]) 
{
    snprintf(result, resultSize, "%s%s%s", b1, date, b2);
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
    while(count < 5) //central.connected()
    { 
      count++;

      fileAvailable = getFirstLineAndDelete("Sessions.log", date);
      Serial.print("Das ist log: ");
      Serial.println(date);
      if(fileAvailable) 
      {
        char dateString[12];
        addBracketsToDate(dateString, sizeof dateString, openingBracket, date, closingBracket);
        DiveDataCharacteristic.writeValue(dateString);
        
        Serial.print("Aktueller dateString: ");
        Serial.println(dateString);
        
        //snprintf(logfilePath2, 30, "logfiles/%s.log", date);
        
        //snprintf(logfilePath2, 25, "logfiles/%s.log", date);
        char testPath[] = "logfiles/35_03_21.log";

        Serial.print("Aktueller logpath: ");
        Serial.println(logfilePath2);      

        File file = SD.open(testPath);

        if(file) 
        {
          Serial.println("file traditionell geöffnet");
          while (file.available()) 
          {           
            char line[200];
            char package[480];

            file.readStringUntil('\n').toCharArray(line, 200);
            strcat(package, line);
            memset(line, 0, sizeof(line));

            file.readStringUntil('\n').toCharArray(line, 200);
			      strcat(package, line);
			      memset(line, 0, sizeof(line));

            file.readStringUntil('\n').toCharArray(line, 200);
            strcat(package, line);
            memset(line, 0, sizeof(line));

            
            Serial.println("############################");

            //Da C eine traditionelle Sprache ist, sollte das funktionieren
            //snprintf(package, sizeof(package), "%s%s%s", data1, data2, data3);
            //addBracketsToDate(package, sizeof package, data1, data2, data3);
			     Serial.println(package);
				 memset(package, 0, sizeof package);

				 //memset(data1, 0, sizeof(data1));
				 //memset(data2, 0, sizeof(data2));
				 //memset(data3, 0, sizeof(data3));
				 //sendData(package);
				 //sendData(data);
          }
        }
      }
      else 
      {
        central.disconnect();     
      }
      delay(666);
    }
    central.disconnect();
  }
  
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());
}