#include <ArduinoJson.h>

//returns a Json String with values: diveID, time, heartrate, oxygen, confidence, depth, brightness, temperature, accX, accY, accZ, gyrX, gyrY, gyrZ
void createJsonString(float accX, float accY, float accZ, float depth, long duration, float gyrX, float gyrY, float gyrZ, int heartrate, int confidence,
 						int brightness, int oxygen, int diveID, float waterTemperature, char* jsonString)
{
	DynamicJsonDocument obj(256);
	obj["1"] = accX;
	obj["2"] = accY;
	obj["3"] = accZ;
	obj["4"] = depth;
	obj["5"] = duration;
	obj["6"] = gyrX;
	obj["7"] = gyrY;
	obj["8"] = gyrZ;
	obj["9"] = heartrate;
	obj["10"] = confidence;
	obj["11"] = brightness;
	obj["12"] = oxygen;
	obj["13"] = diveID;
	obj["14"] = waterTemperature;
	
	serializeJson(obj, jsonString, 200);
}