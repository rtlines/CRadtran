/***********************************************************************
 * Source Code:
 *    Source code for datafile.cpp
 * Author:
 *    Alexander Marvin
 * Summary:
 *    Reads file in, using filename from user, and stores data in local
 *    memory. Data is read from the table.
 ************************************************************************/
#include <iostream>
#include <unistd.h>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include "datafile.h"
using namespace std;

//typedef
typedef map<double,double>::iterator mIter;

datafile::datafile()
{
  char cwd[255]; 
  getcwd(cwd, 255); //get current working directory
  directory = cwd; //place char array in string directory
  altitude = 0;
  frequency = 0.00001;
  //cout << "I've been initialized, yay" << endl;
}

datafile::datafile(string s) : filename(s)
{
  char cwd[255];
  getcwd(cwd, 255);
  directory = cwd;
  altitude = 0;
  frequency = 0.00001;
  //cout << "I've been initialized with some value: " << filename << endl;
}

datafile::datafile(double alt)
{
  char cwd[255];
  getcwd(cwd, 255);
  directory = cwd;
  altitude = alt;
  frequency = 0.00001;
}

datafile::datafile(string s, double alt) : filename(s)
{
  char cwd[255];
  getcwd(cwd, 255);
  directory = cwd;
  altitude = alt;
  frequency = 0.00001;
}

datafile::datafile(string s, string t) : filename(s), directory(t) 
{
  altitude = 0;
  frequency = 0.00001;
};

datafile::datafile(string s, string t, double alt) : filename(s), directory(t)
{
  altitude = alt;
  frequency = 0.00001;
};

string datafile::getFilename()
{
  if (filename.empty())
  {
    cerr << "No filename yet.";
    return filename;
  }
  else
    return filename;
}

string datafile::getFullPath()
{
  if (filename.empty())
    throw("There is no filename");
  return directory + "/" + filename;
}

void datafile::displayData()
{
  map<double, double>::iterator it;
  //display header
  cout << setw(8) << "Altitude ";
  cout << setw(8) << "Temperature ";
  if (!pressure.empty())
    cout << setw(8) << "Pressure ";
  if (!humidity.empty())
    cout << setw(8) << "Humidity";
  cout << endl;
  //loop through values
  for (it = temperature.begin(); it != temperature.end(); it++)
    {
      double temp = it->first;
      cout << setw(8) << it->first << " ";
      cout << setw(8) << it->second << " ";
      if (!pressure.empty())
	cout << " " << setw(8) << pressure[temp];
      if (!humidity.empty())
	cout << " " << setw(8) << humidity[temp];
      cout << endl;
    }
  
}

void datafile::setDirectory(string d)
{
  directory = d;
}

void datafile::setFilename(string f)
{
  filename = f;
}

bool datafile::readFile(bool hasHead)
{
  //check to see if has filename
  if (filename.empty())
    {
      cerr << "There is no filename";
      throw("No filename");
    }
  //initialize
  string filepath;
  double temp;
  double temp2;
  ifstream fin;

  //open file
  filepath = getFullPath();
  fin.open(filepath.c_str());
  //check to see if it opened
  if (fin.fail())
    {
      cout << "File opening has failed\n";
      return false;
    }
  //read file
  //ignore first line if has header
  if (hasHead)
    {
      fin.ignore(256,'\n');
    }

  //read in data
  while (!fin.eof())
    {
      fin >> temp;
      fin >> temp2;
      pressure.insert(std::pair<double,double>(temp2, temp));
      fin >> temp;
      temperature.insert(std::pair<double,double>(temp2, temp));
      fin >> temp;
      humidity.insert(std::pair<double,double>(temp2, temp));
    }
  //close file
  fin.close();
  return true;
}

bool datafile::readCsvFile(bool hasHead )
{
  //check to see if has filename
  if (filename.empty())
    {
      cerr << "There is no filename";
      throw("No filename");
    }
  //initialize
  double temp;
  double temp2;
  string filepath;
  ifstream fin;

  //open file
  filepath = getFullPath();
  fin.open(filepath.c_str());
  //check to see if it opened
  if (fin.fail())
    {
      cout << "File opening has failed" << endl;
      return false;
    }
  //read file
  //ignore first line if has header
  if (hasHead)
    {
      fin.ignore(256,'\n'); //ignore first line
    }
  //read in data
  do
    {
      //read first item on line
      fin >> temp;
      //cout << "temp2: " << temp2 << endl;
      fin.ignore(1,'\n'); //ignore newline character
      //read second item on line
      fin >> temp2;
      //place in map
      temperature.insert(std::pair<double,double>(temp,temp2));
      fin.ignore(1,'\n'); //ignore comma
    }
  while(!fin.eof());
  //close file
  fin.close();
  return true;
}

bool datafile::convertFile(const char filename[] )
{
  cout << "Testing readFile\n";
  //initialization
  char temp = '\0';
  char prev = '\0';
  ifstream fin; 
  ofstream fout;
  char filename2[255];

  //now create file name for output file
  strcpy(filename2, filename);
  convertFileOutName(filename2);
  //cout << "filename: " << filename << endl;
  //cout << "filename2: " << filename2 << endl;

  //open files
  fin.open(filename);
  if (fin.fail())
    {
      cout << "Unable to open your file" << endl;
      return false;
    }

  fout.open(filename2);
  if (fout.fail())
    {
      cout << "Operation has failed" << endl;
    }
  //read file
 
  while (fin.get(temp))
    {
      if (temp == ',' || temp == '"')
	{
	  fout << "\"";
	  fout << temp;
	  fout << "\"";
	}
      else
	{
	  if (endWord(temp, prev, fin.peek()) && fin.tellg() > 1)
	    fout << ",";
	  fout << temp;
	}
      prev = temp;
    }
  //close file
  cout << endl;
  fin.close();
  fout.close();
  return true;
}

double datafile::getTemperature()
{//get value from map

  if (temperature.empty())
    {
      cerr << "File is not read in!\n";
      return -1;
    }
  //first create map iterator
  map<double,double>::iterator it;

  //get temperature
  it = temperature.find(altitude);
  if (it == temperature.end()) //if point not found, return -1
    {
      return interpolate(1);
    }
  else
    {
      return temperature[altitude]; //if found return value
    }
}

double datafile::getPressure()
{ //retrieve value from map
  if (pressure.empty())
    {
      cerr << "File is not read in!\n";
      return -1;
    }
  //allocate iterator
  map<double,double>::iterator it;
  //get pressure
  //cout << "getPressure running!\n";
  it = pressure.find(altitude);
  if (it == pressure.end()) //if point not found, return -1
    return interpolate(2);
  else
    {
      return pressure[altitude]; //if found return value
    }
}

double datafile::getHumidity()
{ //retrieve value from map
  if (humidity.empty())
    {
      cerr << "File is not read in!\n";
      return -1;
    }
  //allocate iterator
  map<double, double>::iterator it;
  //get humidity
  it = humidity.find(altitude);
  if (it == humidity.end()) //if point not found, return -1
    return interpolate(3);
  else
    {
      return humidity[altitude];
    }
}

double datafile::interpolate(int type)
{ //linear interpolation between two points
  //cout << "Interpolate running!\n";
  map<double, double> * mptr;
  //set mptr to right map
  switch(type)
    {
    case 1: //temperature
      mptr = &temperature;
      break;
    case 2: //pressure
      mptr = &pressure;
      break;
    case 3: //humidity
      mptr = &humidity;
      break;
    }
  //do boundary check
  //need to be changed to deal with proper
  //interpolation at boundaries.
  map<double, double>::iterator it;
  
  if (altitude < 0)
    {
      cerr << "Altitude out of range!\n" << endl;
      it = mptr->begin();
      return it->second; 
    }
  it = mptr->end();
  it--;
  
  if (altitude > it->first)
    {
      return it->second;
    }
  
  //find upper and lower points to interpolate
  mIter upper = mptr->upper_bound(altitude);
  mIter lower = mptr->upper_bound(altitude);
  lower--;
  //calculate point
  double xa = lower->first; //x of first point
  double ya = lower->second; //y of first point
  double xb = upper->first; //x of second point
  double yb = upper->second; //y of second point
  //linear interpolation formula from 
  //https://en.wikipedia.org/wiki/Linear_interpolation
  return ya + (yb - ya)*(altitude - xa)/(xb - xa); 
}

bool datafile::endWord(char presChar, char prevChar, char futChar)
{
  if ( presChar == ' ' && prevChar != ' ' && prevChar != '\n')
    return true;
  else
    return false;
}

void datafile::convertFileOutName(char str[])
{
  int length = strlen(str);
  int i;
  for ( i = 0; i < length && str[i] != '.'; i++);
  if (i <= 251 && i != length)
    {
      //cout << "Period is at " << i << endl;
      //cout << "Length is: " << length << endl;
      str[i] = '\0'; //erase ending
    }
  else if (i > 251)
    {
      str[251] = '\0';
    }
  else if (i <= 251)
    {}
    //who cares
  else
    {
      cerr << "An error has occurred";
      throw;
    }
  strcat(str, ".csv");
}
