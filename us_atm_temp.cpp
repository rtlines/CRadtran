/***********************************************************************
 * us_atm_temp.cpp:
 *    Source code for us_atm_temp
 * Author:
 *    Alexander Marvin
 * Summary:
 *    Calculates temperature by altitude
 ************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include "us_atm_temp.h"
#include "file.h"
using namespace std;

double us_atm_temp(double altitude)
{
  string filename = "/home/amarvin/bin/research/temperatures.csv";
  vector<string> data;
  bool success = readCsvFile(filename.c_str(), data, true);
  return 290.3;
}
