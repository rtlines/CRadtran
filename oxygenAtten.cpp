/***********************************************************************
 * oxygenAtten.cpp:
 *    Source code for oxygenAtten.cpp
 * Author:
 *    Alexander Marvin
 * Summary:
 *    Calculate the attenuation constant from oxygen
 ************************************************************************/
#include <iostream>
#include <cmath>
#include <cassert>
#include "atten.h"
using namespace std;

/************************************************************************
 * Non-default constructor
 ************************************************************************/
oxygenAtten::oxygenAtten(datafile * d)
{
  //cout << "Constructor running!\n";
  data = NULL;
  data = d;
  //set the values
  setValues();
  
}

/************************************************************************
 * Reset values
 ************************************************************************/
void oxygenAtten::resetValues()
{
  setValues();
}

/************************************************************************
 * Set values: helper function
 ************************************************************************/
void oxygenAtten::setValues()
{
  //cout << "setValues called\n";
  v = data->getFreq(); //GHz
  P = (data->getPressure())*0.750; //Expecting milibar, convert to mmHg
  T = data->getTemperature(); //K
}

double oxygenAtten::getValue()
{
  return C1*P*pow(v,2)/pow(T, 3)*rotationalStateSum();
}

/***********************************************************************
 * rotationalStateSum:
 * Sum all the odd rotational states of oxygen from 1 to 45
 ***********************************************************************/
double oxygenAtten::rotationalStateSum()
{
  //initialize sum
  double sum = 0;
  //loop through all odd states
  for (int N = 1; N <= 45; N += 2)
  {
    sum += rotationalState(N)*exp(-2.06844*N*(N+1)/T);
  }
  return sum;
}

/***********************************************************************
 * rotationalState: 
 * calculate rotational state of oxygen. The rotational state is known 
 * as factor FN with N subscripted in the Meeks and Lilley paper.
 ***********************************************************************/
double oxygenAtten::rotationalState(int N)
{
  // _ underscore denotes subscript in these comments
  //calculate term mu^2_N+
  long double muNPlusSqr = N*(2*N+3)/(N+1);
  //calculate term mu^2_N-
  long double muNMinusSqr = (N+1)*(2*N+1)/N;
  //calculate term mu^2_NO
  long double muNOSqr = 2*(N*N+N+1)*(2*N+1)/(N*(N+1));
  //calculate delta V or line broadening
  long double deltaV = lineWidth()/1000; //convert to Gigacyles
  //get resonant frequency v_N+
  long double VnPlus = getTransitionPlus(N);
  assert(VnPlus != -1);
  //get resonant frequency v_N-
  long double VnMinus = getTransitionMinus(N);
  assert(VnMinus != -1);
  //calculate term F_O 
  long double FO = deltaV/(pow(v,2)+pow(deltaV,2));
  //calculate F_N+
  long double FNPlus = deltaV/(pow(VnPlus-v,2)+pow(deltaV,2));
  FNPlus += deltaV/(pow(VnPlus+v,2)+pow(deltaV,2));
  //calculate F_N-
  long double FNMinus = deltaV/(pow(VnMinus-v,2)+pow(deltaV,2));
  FNMinus += deltaV/(pow(VnMinus+v,2)+pow(deltaV,2));
  //final calculations
  return FNPlus*muNPlusSqr+FNMinus*muNMinusSqr+FO*muNOSqr; //Gigacyles
}

double oxygenAtten::lineWidth()
{
  return 1.95*P*(0.21+0.78*getBeta())*pow(300/T,0.85); //Megacycles
}

double oxygenAtten::getBeta()
{
  //set H1 and H2
  int H1 = 12; //km
  int H2 = 25; //km
  //get altitude
  double h = (data->getAltitude())/1000; //convert to km
  //cout << "Altitude is " << h << endl;
  if (h < H1) 
    return 0.25; //unitless
  else if (H1 <= h && h <= H2)
    return 0.25+0.5*(h - H1)/(H2 - H1); //unitless
  else if (h > H2)
    return 0.75; //unitless
}

double oxygenAtten::getTransitionPlus(int N)
{
  switch(N)
  {
    case 1: return 56.2648;
    case 3: return 58.4466;
    case 5: return 59.5910;
    case 7: return 60.4348;
    case 9: return 61.1506;
    case 11: return 61.8002;
    case 13: return 62.4112;
    case 15: return 62.9980;
    case 17: return 63.5685;
    case 19: return 64.1272;
    case 21: return 64.6779;
    case 23: return 65.2240;
    case 25: return 65.7626;
    case 27: return 66.2978;
    case 29: return 66.8313;
    case 31: return 67.3627;
    case 33: return 67.8923;
    case 35: return 68.4205;
    case 37: return 68.9478;
    case 39: return 69.4741;
    case 41: return 70.0000;
    case 43: return 70.5249;
    case 45: return 71.0497;
    default: return -1;
  }
}

double oxygenAtten::getTransitionMinus(int N)
{
  switch(N)
  {
    case 1: return 118.7505;
    case 3: return 62.4863;
    case 5: return 60.3061;
    case 7: return 59.1642;
    case 9: return 58.3239;
    case 11: return 57.6125;
    case 13: return 56.9682;
    case 15: return 56.3634;
    case 17: return 55.7839;
    case 19: return 55.2214;
    case 21: return 54.6728;
    case 23: return 54.1294;
    case 25: return 53.5960;
    case 27: return 53.0695;
    case 29: return 52.5458;
    case 31: return 52.0259;
    case 33: return 51.5091;
    case 35: return 50.9949;
    case 37: return 50.4830;
    case 39: return 49.9730;
    case 41: return 49.4648;
    case 43: return 48.9582;
    case 45: return 48.4530;
    default: return -1;
  }
}
