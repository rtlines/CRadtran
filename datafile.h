/***********************************************************************
 * Header File:
 *    datafile.h
 * Author:
 *    Alexander Marvin
 * Summary:
 *    Header for datafile.cpp
 ************************************************************************/
#ifndef DATA_FILE_H
#define DATA_FILE_H

#include <map>
#include <string>

//typedef
typedef std::map<double,double>::iterator mIter;

class datafile
{
 public:
  //constructors
  datafile();
  datafile(std::string s);
  datafile(double alt);
  datafile(std::string s, double alt);
  datafile(std::string s, std::string t);
  datafile(std::string s, std::string t, double alt);
  //getters
  std::string getDirectory() { return directory; };
  std::string getFilename();
  std::string getFullPath();
  double getAltitude() { return altitude; }; //altitude in meters
  double getFreq() { return frequency; }; //frequency in GHz
  //setters
  void setDirectory(std::string d);
  void setFilename(std::string f);
  void setAltitude(double alt) { altitude = alt; }; //altitude in meters
  void setFreq(double freq) {frequency = freq; }; //frequency in GHz
  //useful functions
  bool hasFileName() { return !filename.empty(); };
  bool readFile(bool hasHead);
  bool readCsvFile(bool hasHead);
  bool convertFile(const char filename[]);
  double getTemperature(); //in Kelvin
  double getPressure(); //in milibars
  double getHumidity();
  void displayData();
 private:
  bool endWord(char presChar, char prevChar, char futChar);
  void convertFileOutName(char str[]);
  double interpolate(int type);
  std::string directory; //working directory of our object
  std::string filename; //filename
  std::map<double,double> temperature;
  std::map<double,double> pressure;
  std::map<double,double> humidity;
  double altitude; //altitude in meters?
  double frequency; //frequency in GHz
};

#endif // DATA_FILE_H
