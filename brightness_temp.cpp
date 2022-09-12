/*****************************************************************************
 * Source Code:
 *    brightness_temp.cpp
 * Author: 
 *    Alexander Marvin
 * Summary: 
 *    Calculates blackbody radiation, using Planck's equation
 ****************************************************************************/

#include <iostream>
#include <cstdlib>
#include <cmath>
#include "brightness_temp.h"

using namespace std;

long double planckv(double freq, double temp)
{
  long double B;
  long double c = SPEED_OF_LIGHT;
  long double h = PLANCK_CONST*pow(10,-34);
  long double kB = BOLTZMAN_CONST*pow(10,-23);
  B = 2*h*pow(freq, 3)/pow(c, 2)*1/(exp(h*freq/(kB*temp)) - 1);
  return B;
};

long double planckw(double wave, double temp)
{
  long double B, nu;
  long double c = SPEED_OF_LIGHT;
  long double h = PLANCK_CONST*pow(10,-34);
  long double kB = BOLTZMAN_CONST*pow(10,-23);
  nu = c/wave; 
  //B = 2*h*pow(c,2)/pow(wave,5)*1/(exp(h*c/(wave*kB*temp)) - 1);
  B = 2*h*pow(nu,3)/( pow(c,2)*(exp(h*nu/(kB*temp))) - 1);
  return B;
};
