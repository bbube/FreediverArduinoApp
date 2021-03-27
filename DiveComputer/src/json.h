#include <ArduinoJson.h>

//returns a Json String with values: diveID, time, heartrate, oxygen, confidence, depth, brightness, temperature, accX, accY, accZ, gyrX, gyrY, gyrZ
void createJsonString(int diveID, float duration, int heartrate, int oxygen, int confidence, float depth, int brightness, float waterTemperature,
 float accX, float accY, float accZ, float gyrX, float gyrY, float gyrZ, char* jsonString)
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