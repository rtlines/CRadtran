/***********************************************************************
 * Header File:
 *    atten.h attenuation class
 * Author:
 *    Alexander Marvin
 * Summary:
 *    Calculates attenuation constant kappa for different slices of atmosphere.
 ************************************************************************/
#ifndef ATTEN_H
#define ATTEN_H

#include "datafile.h"

class atten
{
 public:
  atten();
  double getValue(double altitude);
 private:
  //friend oxygenAtten;
};

#define C1 2.6742 //decibels per kilometer

class oxygenAtten
{
 public:
  oxygenAtten(datafile * d); //receive pointer for datafile
  double getValue();
  void resetValues();
 private:
  //variables
  double P; //pressure
  double v; //frequency
  double T; //temperature
  datafile * data; //pointer to data
  //functions
  void setValues();
  double rotationalStateSum();
  double rotationalState(int N);
  double lineWidth();
  double getBeta();
  double getTransitionPlus(int N);
  double getTransitionMinus(int N);
};

class waterVaporAtten
{
  waterVaporAtten(datafile & d);
};

#endif // ATTEN_H
