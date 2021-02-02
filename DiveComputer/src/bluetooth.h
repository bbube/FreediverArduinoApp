#include <ArduinoBLE.h>

int dataLength = 300;

BLEService DiveData("19B10000-E8F3-537E-4F6C-D194768A2214");
BLECharacteristic DataStream( "19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify,300,(1==1));

String testData = "{\"diveID\":1,\"duration\":9438,\"heartrate\":0,\"oxygen\":0,\"confidence\":0,\"depth\":93,\"brightness\":78,\"temperature\":6,\"accX\":0.030151,\"accY\":0.002563,\"accZ\":1.012573,\"gyrX\":1.037598,\"gyrY\":-3.173828,\"gyrZ\":-1.647949}";

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
    DataStream.writeValue(data.c_str());
}

void buildBluetoothConnection() {

  BLEDevice central = BLE.central();  

  if (central) 
  {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      
      sendData(testData);
      delay(200);
    }
  }
  //digitalWrite(LED_BUILTIN, LOW);
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());
}