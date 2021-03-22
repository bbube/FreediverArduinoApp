#include <Arduino.h>
#include <iostream>
#include <string>
#include <SD.h>
#include <fstream>

#define dateLength 10


using namespace std;

bool sameSession = false;

const char directoryPath[] PROGMEM = "logFiles/";
const char divePath[] PROGMEM = "dive.log";
const char datePath[] PROGMEM = "date.log";
const char sessionsPath[] PROGMEM = "sessions.log";
char date[10] PROGMEM;
char logPath[25] PROGMEM;

int diveID = 1;

//writes the value of variable date into the date-file
void setDateToFile(char d[]) 
{
    File file = SD.open(datePath, FILE_WRITE);
    if(file) 
    {
        file.print(d);
        file.close();
    }
}

//returns the date from the date-file
//returns empty string if failed to open the file
void getDateFromFile(char* result) 
{
    File file = SD.open(datePath);
    
    if(file) 
    {
        file.readString().toCharArray(result, dateLength);	
        file.close();
    }
}

//returns the diveID from the diveID-file
//returns -1 if failed to open the file
int getDiveID() 
{
    File file = SD.open(divePath);

    if(file) 
    {
        int x = file.parseInt();
        file.close();
        return x;
    }
    return -1;
}

//writes the value of variable diveID into the diveID-file
void setDiveID() 
{
    if(SD.exists(divePath))
    {
        SD.remove(divePath);
    }
    File file = SD.open(divePath, FILE_WRITE);
    if(file) 
    {
        file.print(diveID);
        file.close();
    }
}

void writeDateToSessionFile(char* date) 
{
    File file = SD.open(sessionsPath, FILE_WRITE);    
    if(file) 
    {
        file.println(date);
        file.close();
    }
}

//has to be implemented with the real-time-clock when built in
void setDate() 
{	
    //char out[20];
    //snprintf(out, sizeof out, "%id", millis());
    //strncpy ( date, out, 6);
    
    char currDate[] = "45_03_21";
    snprintf(date, sizeof date, "%s", currDate);
}

//-------META files--------
//sets the date; checks if the meta-files "dive.log" and "date.log"
//exist and creates them if not; if yes the variables "diveID" and
//"date" are set
void initializeMetaData() 
{
    //just to imitate a date, has to be removed
    //while(millis() < 3701)
    setDate();
    char oldDate[dateLength];
    
    if (SD.exists(datePath)) 
    {
        getDateFromFile(oldDate);
        if (strcmp(date,oldDate) != 0) 
        {
            SD.remove(datePath);
            setDateToFile(date);
            //writeDateToSessionFile();
        } 
        else 
        {
            sameSession = true;
        }
    } 
    else 
    {
        setDateToFile(date);
        //writeDateToSessionFile();
    }
    
    if(SD.exists(divePath)) 
    {
        if(sameSession) 
        {
            diveID = getDiveID();
        }
        //SD.remove(divePath);
    }
    setDiveID();  
}

//creates a directory with given parameter as name
void createDirectory() 
{
    SD.mkdir(directoryPath);
}