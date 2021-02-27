#include <Arduino.h>
#include <iostream>
#include <string>
#include <SD.h>
#include <fstream>
using namespace std;

bool sameSession = false;

String directoryPath = "logFiles";
String logPath;
String divePath = "dive.log";
String datePath = "date.log";
String sessionsPath = "sessions.log";
String date;

int diveID = 1;

//writes the value of variable date into the date-file
void setDateToFile(String d) {

  File file = SD.open(datePath, FILE_WRITE);
  if(file) {
    file.print(d);
    file.close();
  }
}

//returns the date from the date-file
//returns empty string if failed to open the file
String getDateFromFile() {

  File file = SD.open(datePath);
  if(file) {
    String x = file.readString();
    file.close();
    return x;
  }
  return ""; //maybe error for later checks
}

//returns the diveID from the diveID-file
//returns -1 if failed to open the file
int getDiveID() {

  File file = SD.open(divePath);
  if(file) {
    int x = file.parseInt();
    file.close();
    return x;
  }
  return -1;
}

//writes the value of variable diveID into the diveID-file
void setDiveID() {
  if(SD.exists(divePath)) {
    SD.remove(divePath);
  }
  File file = SD.open(divePath, FILE_WRITE);
  if(file) {
    file.print(diveID);
    file.close();
  }
}

void writeDateToSessionFile() {
  File file = SD.open(sessionsPath, FILE_WRITE);
  Serial.println("File erstellt");
  if(file) {
    Serial.println("date: " + date);
    file.println(date);
    file.close();
  }
}

//has to be implemented with the real-time-clock when built in
void setDate() {
  date = to_string(millis()).substr(0, 6).c_str();
}



//-------META files--------
//sets the date; checks if the meta-files "dive.log" and "date.log"
//exist and creates them if not; if yes the variables "diveID" and
//"date" are set
void initializeMetaData() {

  //just to imitate a date, has to be removed
  while(millis() < 3701)

  setDate();
  String oldDate;
  if (SD.exists(datePath)) {
    oldDate = getDateFromFile();
    if(!date.equals(oldDate)) {
      SD.remove(datePath);
      setDateToFile(date);
      //writeDateToSessionFile();
    } else {
      sameSession = true;
    }
  } else {
    setDateToFile(date);
    //writeDateToSessionFile();
  }

  if(SD.exists(divePath)) {
    if(sameSession) {
      diveID = getDiveID();
    }
    SD.remove(divePath);
  }
  setDiveID();

  
}

//creates a directory with given parameter as name
void createDirectory() {
  SD.mkdir(directoryPath);
  Serial.println("directory created");
}