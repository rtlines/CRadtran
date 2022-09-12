/***********************************************************************
 * Header File:
 *    Header file for calculate.cpp
 * Author:
 *    Alexander Marvin
 * Summary:
 *    Contains function headers
 ************************************************************************/
#ifndef _CALCULATE_H
#define _CALCULATE_H

//declare setup class
class setup
{
 public:
  setup()
    {
      pKappa = NULL;
      printK = false;
    };
  double min;
  double max;
  double step;
  int M;
  double epsilon;
  double surfaceTemperature;
  double max_altitude;
  bool printK;
  double * pKappa;
  ~setup()
    {
      pKappa = NULL;
    }
};

double sliceTrans(double k, double deltaZ);
//double calcDensity(double temperature, double pressure);
double sum(double *pArray, int length);
double atmosphereTerm(double tau[], double brightTemp[], int length);
double reflectedTerm(double tau[], double brightness[], int length);
double freqToWavelength(double freq);
double upwardRadiance(double frequency, setup & settings);
double calcPressure(double altitude);
double wavelengthToFreq(double wavelength);

//constants
#define GRAV 9.81 //meters per second squared
#define A0 100000 //meters, Karman line
#define IDEAL_GAS_CONST 0.286 // kJ/ kg/k
#define LAP_RATE 0.0065 // K/m (temperature lapse rate)
#define P0 101.325 //kPa (sea level atm pressure)
#define T0 288.15 //K (sea level standard temperature)

#endif // _CALCULATE_H
