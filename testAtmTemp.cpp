/***********************************************************************
 * testAtmTemp.cpp:
 *    Source code for driver for us_atm_temp.cpp
 * Author:
 *    Alexander Marvin
 * Summary:
 *    Driver that tests us_atm_temp.cpp
 ************************************************************************/

#include <iostream>
#include "us_atm_temp.h"
using namespace std;

int main()
{
  cout << "testAtmTemp working." << endl;
  cout << "Value of us_atm_temp: " << us_atm_temp(100) << endl;
}
