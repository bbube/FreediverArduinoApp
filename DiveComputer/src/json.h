#include <ArduinoJson.h>


//returns a Json String with values: diveID, time, heartrate, oxygen, confidence, depth, brightness, temperature, accX, accY, accZ, gyrX, gyrY, gyrZ
String createJsonString(int diveID, float duration, int heartrate, int oxygen, int confidence, float depth, int brightness, float waterTemperature,
 float accX, float accY, float accZ, float gyrX, float gyrY, float gyrZ)
{
  DynamicJsonDocument obj(1024);
  obj["ref_dive"] = diveID;
  obj["duration"] = duration;
  obj["heart_freq"] = heartrate;
  obj["oxygen_saturation"] = oxygen;
  obj["heart_var"] = confidence;
  obj["depth"] = depth;
  obj["luminance"] = brightness;
  obj["water_temp"] = waterTemperature;
  obj["accelerator_x"] = accX;
  obj["accelerator_y"] = accY;
  obj["accelerator_z"] = accZ;
  obj["gyroscope_x"] = gyrX;
  obj["gyroscope_y"] = gyrY;
  obj["gyroscope_z"] = gyrZ;
  
  String output;
  serializeJson(obj, output);
  return output;
}

