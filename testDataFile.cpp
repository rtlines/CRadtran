/***********************************************************************
 * source code:
 *    Source code for testDataFile.cpp
 * Author:
 *    Alexander Marvin
 * Summary:
 *    Tests to make sure readFile and writeFile work
 ************************************************************************/
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include "datafile.h"
using namespace std;

int main()
{
  //initialize
  bool success;
  double key;
  double value;
  cout << "testDataFile here\n";
  string directory = "/home/amarvin/bin/research/temperatures.csv";
  string filename = "temperatures.csv";
  datafile data;
  cout << "Current working directory is: " << data.getDirectory() << endl;
  cout << "Is filename empty? " << (!data.hasFileName() ? "Yes" : "No") << endl;
  if (data.hasFileName())
    cout << "Full path is " << data.getFullPath() << endl;
  cout << "Altitude is " << data.getAltitude() << endl;
  cout << "Frequency is " << data.getFreq() << endl;
  datafile data2("temperatures.csv");
  data2.setAltitude(56);
  data2.setFreq(45);
  cout << "Filename is: " << data2.getFilename() << endl;
  cout << "Full path is: " << data2.getFullPath() << endl;
  cout << "Altitude is: " << data2.getAltitude() << endl;
  cout << "Frequency is: " << data2.getFreq() << endl;
  success = data2.readCsvFile(true);
  //success = convertFile(filename.c_str());
  string answer = success ? "true" : "false";
  cout << "File was read successfully: " << answer << endl;
  cout << endl << "Displaying values in our map:" << endl;
  data2.displayData();

  cout << "Now testing retrieval of values:" << endl;
  cout << "Enter a value: ";
  cin >> key;
  //get temperature
  data2.setAltitude(key);
  value = data2.getTemperature();
  if (value != -1)
    cout << "Temperature is: " << value << endl;
  else
    cout << "Temperature is not found!\n";
  //now put filename in data
  data.setFilename("TIGR0001");
  cout << endl << "Now testing setFilename\n";
  cout << "Filename in data is ";
  cout << data.getFilename() << endl;
  cout << "Altitude is " << data.getAltitude() << endl;
  //now read in data
  cout << "Now reading in data from " << data.getFilename() << endl;
  data.readFile(true);
  cout << "Display data:\n";
  data.displayData();
  
  cout << "Now testing retrieval of values:" << endl;
  cout << "Enter a value: ";
  cin >> key;
  //display temperature
  data.setAltitude(key);
  value = data.getTemperature();
  if (value != -1)
    cout << "Temperature is: " << value << endl;
  else
    cout << "Temperature not found!\n";
  //display pressure
  value = data.getPressure();
  if (value != -1)
    cout << "Pressure is: " << value << endl;
  else
    cout << "Pressure is not found!\n";
  //display relative humidity
  value = data.getHumidity();
  if (value != -1)
    cout << "Relative Humidity is: " << value << endl;
  else
    cout << "Humidity is not found!\n";
  //done testing
  cout << "Testing is done!\n";
}
