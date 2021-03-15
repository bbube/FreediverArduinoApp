#include <ArduinoBLE.h>
#include <Arduino.h>
#include <SD.h>
#include <vector>
#include <string>

//#include <bits/stdc++.h> 
using namespace std; 

int dataLength = 300;

BLEService DiveData("19B10000-E8F3-537E-4F6C-D194768A2214");
BLECharacteristic DataStream( "19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify,210,(1==1));
 
/*
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
BLEIntCharacteristic   oxygen_saturation("ab9db830-e5b5-4c12-8c9b-845386bf6dc8", BLERead | BLENotify);
BLEIntCharacteristic   luminance("4433820f-7709-4881-ad18-adc063ddc0a1", BLERead | BLENotify); 
BLEIntCharacteristic   ref_dive("b8589826-9f39-445d-901e-699db1cad791", BLERead | BLENotify);
BLEFloatCharacteristic water_temp("3473bdad-806f-419f-875a-0f2472e3fe53", BLERead | BLENotify);
*/

BLEDescriptor nameDescriptor("2187", "1");
char testData[] = "{\"ref_dive\":1,\"duration\":9438,\"heart_freq\":0,\"oxygen_saturation\":0,\"heart_var\":0,\"depth\":93,\"luminance\":78,\"water_temp\":6,\"accelerator_x\":0.030151,\"accelerator_y\":0.002563,\"accelerator_z\":1.012573,\"gyroscope_x\":1.037598,\"gyroscope_y\":-3.173828,\"gyroscope_z\":-1.647949}";
char testData2[] = "{\"rf\":1,\"dur\":9438,\"hf\":0,\"os\":0,\"hv\":0,\"d\":93,\"lu\":78,\"wt\":6,\"ax\":0.030151,\"ay\":0.002563,\"az\":1.012573,\"gx\":1.037598,\"gy\":-3.173828,\"gz\":-1.647949}";
//std::vector<String> sessionsArray;

String directoryPath2 = "logFiles";

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
  /*
  DiveData.addCharacteristic(DataStream);
  BLE.addService(DiveData);
  BLE.setAdvertisedService(DiveData);  
  DataStream.broadcast();
  */
  /*
  accelerator_x.addDescriptor(nameDescriptor);

  
  

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
  */
  DiveData.addCharacteristic(DataStream);
  BLE.addService(DiveData);
  BLE.setAdvertisedService(DiveData);
  DataStream.broadcast();
  /*
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
  */
  BLE.advertise();  
  Serial.println("Bluetooth device active, waiting for connections...");
}

void sendData(
  float _accelerator_x, 
  float _accelerator_y, 
  float _accelerator_z,
  float _depth,
  float _duration,
  float _gyroscope_x,
  float _gyroscope_y,
  float _gyroscope_z,
  int _heart_freq, 
  int _heart_var, 
  int _luminance,
  int _oxygen_saturation,
  int _ref_dive,
  float _water_temp) {
  /*
	accelerator_x.writeValue(_accelerator_x);
  delay(10);
	accelerator_y.writeValue(_accelerator_y);
  delay(10);
	accelerator_z.writeValue(_accelerator_z);
  delay(10);
	depth.writeValue(_depth);
  delay(10);
	duration.writeValue(_duration);
  delay(10);
	gyroscope_x.writeValue(_gyroscope_x);
  delay(10);
	gyroscope_y.writeValue(_gyroscope_y);
	gyroscope_z.writeValue(_gyroscope_z);
	heart_freq.writeValue(_heart_freq);
	heart_var.writeValue(_heart_var);
	luminance.writeValue(_luminance);
	oxygen_saturation.writeValue(_oxygen_saturation);
	ref_dive.writeValue(_ref_dive);
	water_temp.writeValue(_water_temp);
  */
}

void getJsonPart(char* outStr, char input[], float input2)
{  
  snprintf(outStr, 20, input, input2);
}

void getJsonPart(char* outStr, char input[], int input2)
{  
  snprintf(outStr, 20, input, input2);
}

void getJson(char* outStr, float _accelerator_x, float _accelerator_y, float _accelerator_z,
  float _depth, float _duration, float _gyroscope_x, float _gyroscope_y, float _gyroscope_z,
  int _heart_freq, int _heart_var, int _luminance, int _oxygen_saturation, int _ref_dive,
  float _water_temp)
{
    char json[200] = "";
    char jsonPart[20];
    getJsonPart(jsonPart, "{\"1\":%d", _ref_dive);
    strcat(json, jsonPart);
    getJsonPart(jsonPart, ",\"2\":%d", _accelerator_x);
    strcat(json, jsonPart);
    getJsonPart(jsonPart, ",\"3\":%d", _accelerator_y);
    strcat(json, jsonPart);
    getJsonPart(jsonPart, ",\"4\":%d", _accelerator_z);
    strcat(json, jsonPart);
    getJsonPart(jsonPart, ",\"5\":%d", _depth);
    strcat(json, jsonPart);
    getJsonPart(jsonPart, ",\"6\":%d", _duration);
    strcat(json, jsonPart);
    getJsonPart(jsonPart, ",\"7\":%d", _gyroscope_x);
    strcat(json, jsonPart);
    getJsonPart(jsonPart, ",\"8\":%d", _gyroscope_y);
    strcat(json, jsonPart);
    getJsonPart(jsonPart, ",\"9\":%d", _gyroscope_z);
    strcat(json, jsonPart);
    getJsonPart(jsonPart, ",\"10\":%d", _heart_freq);
    strcat(json, jsonPart);
    getJsonPart(jsonPart, ",\"11\":%d", _heart_var);
    strcat(json, jsonPart);
    getJsonPart(jsonPart, ",\"12\":%d", _luminance);
    strcat(json, jsonPart);
    getJsonPart(jsonPart, ",\"13\":%d", _oxygen_saturation);
    strcat(json, jsonPart);
    getJsonPart(jsonPart, ",\"14\":%d", _water_temp);
    strcat(json, jsonPart);    
    strcat(json, "}");
    Serial.print("json in fun: ");
    Serial.println(json);
    outStr = json;
    Serial.println(outStr);
    Serial.print("out in fun: ");
}

String getLastLineAndDeleteLine(String filename)
{
  std::vector<String> sessionArray;
  
  File file = SD.open(filename);
  if(file) {
    while (file.available()) {
      String data = file.readStringUntil('\n');
      sessionArray.push_back(data);
      Serial.println(data);
    }
  }
  Serial.println("Checkpoint 1");
  file.close();
  SD.remove(filename);
  File newFile = SD.open(filename, FILE_WRITE);
  Serial.println("Checkpoint 2");
  
  if(newFile) {
    
    Serial.println("Checkpoint 3");
    if(sessionArray.size() < 1) {
      Serial.println(sessionArray.size());
      Serial.println("es ist passiert");
      return "";
    }
    for (int i = 0; i < sessionArray.size()-1; i++)
    {
      newFile.println(sessionArray[i]);
      
      Serial.println(sessionArray[i]);

    }
    
  }
  newFile.close();
  Serial.println("Checkpoint 2");
  Serial.println(sessionArray[sessionArray.size()-1]);
  return sessionArray[sessionArray.size()-1];
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
    int i = 0;
    while(central.connected()) {
      i++;      
      
      _ref_dive++;
      _heart_freq++;
      char json[200] = "";
      //      
      char jsonPart[20];
      snprintf(jsonPart, 20, "{\"1\":%d", _ref_dive);
      strcat(json, jsonPart);      
      snprintf(jsonPart, 20, ",\"2\":%d", _accelerator_x);
      strcat(json, jsonPart);      
      snprintf(jsonPart, 20, ",\"3\":%d", _accelerator_y);
      strcat(json, jsonPart);      
      snprintf(jsonPart, 20, "\"4\":%d", _accelerator_z);
      strcat(json, jsonPart);      
      snprintf(jsonPart, 20, ",\"5\":%d", _depth);
      strcat(json, jsonPart);      
      snprintf(jsonPart, 20, ",\"6\":%d", _duration);
      strcat(json, jsonPart);      
      snprintf(jsonPart, 20, ",\"7\":%d", _gyroscope_x);
      strcat(json, jsonPart);      
      snprintf(jsonPart, 20, ",\"8\":%d", _gyroscope_y);
      strcat(json, jsonPart);      
      snprintf(jsonPart, 20, ",\"9\":%d", _gyroscope_z);
      strcat(json, jsonPart);      
      snprintf(jsonPart, 20, ",\"10\":%d", _heart_freq);
      strcat(json, jsonPart);      
      snprintf(jsonPart, 20, ",\"11\":%d", _heart_var);
      strcat(json, jsonPart);      
      snprintf(jsonPart, 20, ",\"12\":%d", _luminance);
      strcat(json, jsonPart);      
      snprintf(jsonPart, 20, ",\"13\":%d", _oxygen_saturation);
      strcat(json, jsonPart);      
      snprintf(jsonPart, 20, ",\"14\":%d", _water_temp);
      strcat(json, jsonPart);    
      strcat(json, "}");
      DataStream.writeValue(json);
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

/*
void buildBluetoothConnection() {

  BLEDevice central = BLE.central();  

  if (central) 
  {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    String log;

    int x = 0;
    while(central.connected()) { //central.connected()
  
        log = getLastLineAndDeleteLine("Sessions.log");
        Serial.print("Das ist log: ");
        Serial.println(log);
        if(log.length() != 0) {
          sendData(log);
          File file = SD.open("logfiles/" + log + ".log");
          
          string output2;
          output2.append("logfiles/").append(log.c_str()).append(".log");

          Serial.println("sending Data now...");
          while(true)
            sendData(output2.c_str());

          
          String data;
          if(file) {
            Serial.println("inside if");
            while (file.available()) {
              Serial.println("inside while");
              data = file.readStringUntil('\n');
              Serial.print(data);
              sendData(data);
            }
          }
          
        } 
        else central.disconnect();
        delay(200);
        
        
        x++;
    } 
    
      
    
  }
  
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());
}
*/