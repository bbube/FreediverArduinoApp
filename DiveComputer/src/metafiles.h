#include <Arduino.h>
#include <iostream>
#include <string>
#include <SD.h>
using namespace std;

bool sameSession = false;

String directoryPath = "logFiles";
String logPath;
String divePath = "dive.log";
String datePath = "date.log";
String date;

int diveID = 1;

//writes the value of variable sessionID (+ parameter if given) into the sessionID-file
void setDateToFile(String d) {

  File file = SD.open(datePath, FILE_WRITE);
  if(file) {
    file.print(d);
    file.close();
  }
}

//returns the sessionID from the sessionID-file
//returns -1 if failed to open the file
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

//writes the value of variable sessionID into the divenID-file
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

//has to be implemented with the real-time-clock when built in
void setDate() {
  date = to_string(millis()).substr(0, 6).c_str();
}

//-------META files--------
//Check if the meta-Files "sessionID" and "diveID" exist. IF yes their current
//values are saved in sessionID and diveID. IF not they are created.
void initializeMetaData() {

  while(millis() < 3501)
  setDate();
  String oldDate;
  if (SD.exists(datePath)) {
    oldDate = getDateFromFile();
    if(!date.equals(oldDate)) {
      SD.remove(datePath);
      setDateToFile(date);
    } else {
      sameSession = true;
    }
  } else {
    setDateToFile(date);
  }
  Serial.println("date initialized (" + date + ")");

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