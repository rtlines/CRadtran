/***********************************************************************
 * atten.cpp:
 *    Source code for atten.cpp
 * Author:
 *    Alexander Marvin
 * Summary:
 *    Calculate the attenuation constant kappa for each layer.
 ************************************************************************/
#include <iostream>
#include "atten.h"
using namespace std;

double atten::getValue(double altitude)
{
  double kappa = 0;
  kappa += calcKappaO2();
  //kappa += calcKappaH2O();
  //future addition of scattering
  //kappa += scattering();
  return kappa;
}
