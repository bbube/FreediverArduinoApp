#include <ArduinoJson.h>


//returns a Json String with values: diveID, time, heartrate, oxygen, confidence, depth, brightness, temperature, accX, accY, accZ, gyrX, gyrY, gyrZ
String createJsonString(int diveID, float duration, int heartrate, int oxygen, int confidence, float depth, int brightness, float waterTemperature,
 float accX, float accY, float accZ, float gyrX, float gyrY, float gyrZ)
{
  DynamicJsonDocument obj(1024);
  obj["diveID"] = diveID;
  obj["duration"] = duration;
  obj["heartrate"] = heartrate;
  obj["oxygen"] = oxygen;
  obj["confidence"] = confidence;
  obj["depth"] = depth;
  obj["brightness"] = brightness;
  obj["temperature"] = waterTemperature;
  obj["accX"] = accX;
  obj["accY"] = accY;
  obj["accZ"] = accZ;
  obj["gyrX"] = gyrX;
  obj["gyrY"] = gyrY;
  obj["gyrZ"] = gyrZ;
  
  String output;
  serializeJson(obj, output);
  return output;
}