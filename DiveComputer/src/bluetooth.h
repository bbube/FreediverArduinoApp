#include <ArduinoBLE.h>
#include <Arduino.h>
#include <SD.h>

using namespace std; 

int dataLength = 300;

const char openingBracket[] PROGMEM = "{";
const char closingBracket[] PROGMEM = "}";

BLEService DiveData("19B10000-E8F3-537E-4F6C-D194768A2214");

// the characteristic for a measure point
BLEFloatCharacteristic accelerator_x("094a4ab5-d789-477e-8b3a-fd5fee1971f9", BLERead | BLENotify);
BLEFloatCharacteristic accelerator_y("63e30608-53f1-48fb-a7f1-8363ed609a39", BLERead | BLENotify);
BLEFloatCharacteristic accelerator_z("82dea7b6-2091-4a8a-a788-fcaf0fba84e4", BLERead | BLENotify);
BLEFloatCharacteristic depth("70b263a4-faa9-4940-96a6-ce481cfd5803", BLERead | BLENotify);
BLEFloatCharacteristic duration("21980bb5-887a-4a8f-9598-51bb19b41068", BLERead | BLENotify);
BLEFloatCharacteristic gyroscope_x("b6cf7ab8-525d-4e27-86e9-c4e7fe32223c", BLERead | BLENotify);
BLEFloatCharacteristic gyroscope_y("c4fa00ce-5af9-40b7-b960-1658a8074320", BLERead | BLENotify);
BLEFloatCharacteristic gyroscope_z("5475ba53-bcbd-43de-b410-c06ed3a50142", BLERead | BLENotify);
BLEIntCharacteristic   heart_freq("5fa8fde9-8717-45b8-8d7a-2b4da78b627a", BLERead | BLENotify);
BLEIntCharacteristic   heart_var("af2dc2ff-e565-4183-ae8a-a5bc018967c3", BLERead | BLENotify);
BLEIntCharacteristic   luminance("4433820f-7709-4881-ad18-adc063ddc0a1", BLERead | BLENotify); 
BLEIntCharacteristic   oxygen_saturation("ab9db830-e5b5-4c12-8c9b-845386bf6dc8", BLERead | BLENotify);
BLEIntCharacteristic   ref_dive("b8589826-9f39-445d-901e-699db1cad791", BLERead | BLENotify);
BLEFloatCharacteristic water_temp("3473bdad-806f-419f-875a-0f2472e3fe53", BLERead | BLENotify);

// this is to check if the sendet data
// has been received on the android app
BLEFloatCharacteristic ack("76e8ad5b-fe45-4871-a4c0-04962a35bbed", BLERead | BLEWrite | BLENotify);

// in this characteristic the programm writes to ssignal a new session, a new dive,
// the end of a session or the end of the data transmisson
BLECharacteristic datetime( "496957e5-1b18-41e4-9028-800a68a476b4", BLERead |  BLENotify,20,(1==1) );

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
  
  // adds every characteristic to the ble service
  DiveData.addCharacteristic(accelerator_x);
  DiveData.addCharacteristic(accelerator_y);
  DiveData.addCharacteristic(accelerator_z);
  DiveData.addCharacteristic(depth);
  DiveData.addCharacteristic(duration);
  DiveData.addCharacteristic(gyroscope_x);
  DiveData.addCharacteristic(gyroscope_y);
  DiveData.addCharacteristic(gyroscope_z);
  DiveData.addCharacteristic(heart_freq);
  DiveData.addCharacteristic(heart_var);
  DiveData.addCharacteristic(luminance);
  DiveData.addCharacteristic(oxygen_saturation);
  DiveData.addCharacteristic(ref_dive);
  DiveData.addCharacteristic(water_temp);  
  DiveData.addCharacteristic(ack);
  DiveData.addCharacteristic(datetime);
  BLE.addService(DiveData);
  BLE.setAdvertisedService(DiveData);

  accelerator_x.broadcast();
  accelerator_y.broadcast();
  accelerator_z.broadcast();
  depth.broadcast();
  duration.broadcast();
  gyroscope_x.broadcast();
  gyroscope_y.broadcast();
  gyroscope_z.broadcast();
  heart_freq.broadcast();
  heart_var.broadcast();
  luminance.broadcast();
  oxygen_saturation.broadcast();
  ref_dive.broadcast();
  water_temp.broadcast();
  datetime.broadcast();

  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections..."); //uetooth device active, waiting for connections...");
}

// This function reads the "sessions.log" file to check if there is a session
// that has not been sent yet. A session that has already been sent is marked
// by '*' as the first char. If there is a new one it returns true, writes the 
// name into "result" and marks the line.
bool getFirstLineAndDelete(char filename[], char* result)
{
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

// Function to update the value of every characteristic necessary for a measure point
void sendData(float _accelerator_x, float _accelerator_y, float _accelerator_z, float _depth,
  float _duration, float _gyroscope_x, float _gyroscope_y, float _gyroscope_z, int _heart_freq, 
  int _heart_var, int _luminance, int _oxygen_saturation, int _ref_dive, float _water_temp) {

	accelerator_x.writeValue(_accelerator_x);  
	accelerator_y.writeValue(_accelerator_y);  
	accelerator_z.writeValue(_accelerator_z);  
	depth.writeValue(_depth);  
	duration.writeValue(_duration);  
	gyroscope_x.writeValue(_gyroscope_x);  
	gyroscope_y.writeValue(_gyroscope_y);
	gyroscope_z.writeValue(_gyroscope_z);
	heart_freq.writeValue(_heart_freq);
	heart_var.writeValue(_heart_var);
	luminance.writeValue(_luminance);
	oxygen_saturation.writeValue(_oxygen_saturation);
	ref_dive.writeValue(_ref_dive);
	water_temp.writeValue(_water_temp);
}


void buildBluetoothConnection()
{
  BLEDevice central = BLE.central();  
  DynamicJsonDocument jDocument(256);

  bool ready = false;
  char dateString[25];
  char timeString[25];
  // this is the sign for the android app that the data transmission is complete  
  char terminate[] = "{\"Terminate\":0}";

  if (central) 
  {
    Serial.print("Connected to central: ");
    Serial.println(central.address());    

    char date[10];
    bool fileAvailable = true;
    bool finish = false;
    bool newSession = true;

    File file;    

    while (central.connected()) {      
      
      // if the android app writes into the ack characteristic
      // this becomes true. The divecomupter expects this every
      // it updates the datetime characteristic
      if(ack.written())
      {
        Serial.println("has been written");
        ready = true;
        
        // when no sessions have been send yet or a session is completely send 
        // the programm tries to get the next session file
        // if there is one it updates the datetime characteristic
        if(newSession)
        {
          fileAvailable = getFirstLineAndDelete("sessions.log", date);
          if(fileAvailable)
          {
            snprintf(logfilePath2, 25, "logfiles/%s.log", date);
            snprintf(dateString, sizeof dateString, "{\"Date\":\"%s\"}", date);
            datetime.writeValue(dateString);
            ready = false;
            newSession = false;
            Serial.println(logfilePath2);
          }
        }

        delay(100);

        // if there are no more files to send and the android app
        // send an acknowledge that the data has been received the
        // divecomputer will disconnect from the connection
        if (finish)
        {
          central.disconnect();

          //remove /////////////////////////////////////////////
          if(SD.exists("sessions.log"))
          {
              SD.remove("sessions.log");
          }
          File file2 = SD.open("sessions.log", FILE_WRITE);    
          if (file2)
          {        
              file2.println("27_03_21");
              file2.println("29_03_21");
          }
          file2.close();
          //remove /////////////////////////////////////////////
        }        
      }
      
      // if there are no more files to read the divecomputer sends
      // terminator to the android app
      if (!fileAvailable)
      {        
        datetime.writeValue(terminate);
        Serial.println("finish");
        ack.writeValue(1);    
        finish = true;  
        delay(500);  
      }      
      else if (ready)
      {
        // opens the session file        
        file = SD.open(logfilePath2);
        if(file)
        {
          char data[200];
          // reads out of the file line by line
          // if the line starts with '#' it is the beginning of a new dive
          // and the datetime characteristic will be updated with it
          // otherwise its a measure point in json format that will be split
          // and filled in the measure point characteristic
          while(file.available())
          {
            file.readStringUntil('\n').toCharArray(data,200);

            if(data[0] == '#')
            {
              // '#' will be send with it, that is removed on the anrdoid app
              snprintf(timeString, sizeof timeString, "{\"Time\":\"%s\"}", data);
              datetime.writeValue(timeString);
              while(central.connected() && !ack.written()){}
              Serial.println("dive");
            }
            else
            {
              deserializeJson(jDocument, data);
              accelerator_x.writeValue(jDocument["1"]);              
              accelerator_y.writeValue(jDocument["2"]);              
              accelerator_z.writeValue(jDocument["3"]);             
              depth.writeValue(jDocument["4"]);              
              duration.writeValue(jDocument["5"]);              
              gyroscope_x.writeValue(jDocument["6"]);              
              gyroscope_y.writeValue(jDocument["7"]);
              gyroscope_z.writeValue(jDocument["8"]);
              heart_freq.writeValue(jDocument["9"]);
              heart_var.writeValue(jDocument["10"]);
              luminance.writeValue(jDocument["11"]);
              oxygen_saturation.writeValue(jDocument["12"]);
              ref_dive.writeValue(jDocument["13"]);
              water_temp.writeValue(jDocument["14"]);
              Serial.println("measure");              
            }
          }
          datetime.writeValue("{\"EoS\":1}");          
          newSession = true;
          ready = false;
        }
      }
    }    
  }  
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());
}