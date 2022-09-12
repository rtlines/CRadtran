/***********************************************************************
 * Source Code:
 *    Driver for brightness_temp.cpp
 * Author:
 *    Alexander Marvin
 * Summary:
 *    Tests to see if planck function works
 ************************************************************************/

#include <iostream>
#include "brightness_temp.h"

using namespace std;

int main(int argc, char *argv[])
{
  double spect_rad;
  double wavelength, temp;
  cout << "I'm running" << endl;
  //number of arguments
  //cout << argc << endl;
  //ask user for numbers
  cout << "Please enter the wavelength in meters: ";
  cin >> wavelength;
  cout << "Please enter the temperature in Kelvin: ";
  cin >> temp;

  spect_rad = planckw(wavelength, temp);
  cout << spect_rad << endl;
};
