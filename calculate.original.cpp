/***********************************************************************
 * calculate.cpp:
 *    Source code for calculations
 * Author:
 *    Alexander Marvin
 * Summary:
 *    This calculates and does integration
 ***********************************************************************/
#include <iostream>
#include <cmath>
#include <cassert>
#include "calculate.h"
#include "brightness_temp.h"
#include "datafile.h"
#include "atten.h"

using namespace std;

double upwardRadiance(double frequency, setup & settings)
{
  //intialization
  int M = settings.M; //M is the number of slices
  //cout << "M = "<< M << endl;

  cout << frequency << " GHz" << endl;

  //create an array
  double tau[M];
  double brightTemp[M];
  double k[M];
  settings.pKappa = k;

  //intialize everything else
  //cout << settings.max_altitude << endl;
  double deltaZ = settings.max_altitude/((double)M); //length of slice
  cout << "dz = " << deltaZ << " meters\n";
  double epsilon = settings.epsilon; //emissivity constant
  //surface temperature in Kelvin
  double surfaceTemp = settings.surfaceTemperature; 
  double bright_Temp_Surface, firstTerm, secondTerm, thirdTerm, pressure, 
    temperature, totalTrans;
  
  //set up datafile object
  datafile data("TIGR0001");
  //read our data in
  data.readFile(true);

  //setup oxgenAtten object
  oxygenAtten kappa(&data);
  //calculations
  //calculate transmission for each slice
  //cout << "Altitude\tAttenuation" << endl;
  for (int i = 0; i < M; i++)
    {
      //set our altitude
      data.setAltitude(i*deltaZ);

      //set frequency
      data.setFreq(frequency);
      
      //reset kappa
      kappa.resetValues();
      
      //calculate attenuation constant
      k[i] = kappa.getValue();
      //cout <<" kappa value " << k[i] << endl;

      //get pressure for slice
      pressure = data.getPressure();
      assert(pressure >= 0);
      
      //get temperature for slice
      temperature = data.getTemperature();
      
      //calculate optical length for slice
      tau[i] = sliceTrans(k[i], deltaZ);
      assert(tau[i] >= 0);
      //cout <<" tau value " << tau[i] << endl;
      
      //calculate Brightness temp for slice
      brightTemp[i] = planckv(frequency*1e9, temperature);
      //cout << "temp "<< temperature << "freq " << frequency << "plank " <<brightTemp[i] << endl;
    }
      
  //now calculate first term
  bright_Temp_Surface = planckv(frequency*1e9, surfaceTemp);
  //cout << "Value of surface is " << bright_Temp_Surface << endl; 
  totalTrans = sum(tau,M);
  //cout << "Value of optical length is " << totalTrans << endl;
  //cout << "Value of T is " << exp(-totalTrans) << endl;
  //cout << "Value of epsilon " << epsilon << endl;
  firstTerm = epsilon*bright_Temp_Surface*exp(-totalTrans);

  //calculate second term
  secondTerm = atmosphereTerm(tau, brightTemp, M);

  //calculate third and last term
  thirdTerm = (1-epsilon)*reflectedTerm(tau, brightTemp, M);

  //debugging
  
  cout << "Value of the first term is " << firstTerm << endl;
  cout << "Value of the second term is " << secondTerm << endl;
  cout << "Value of the third term is " << thirdTerm << endl;
  
  //add terms and return value
  return firstTerm + secondTerm + thirdTerm;
};

/***********************************************************************
 * Function: sliceTrans
 * Parameters: double k, double deltaZ
 * Returns: double tau
 * Calculates the optical depth of slice of air
 ***********************************************************************/
double sliceTrans(double k, double deltaZ)
{
  //calculate optical depth
  return 0.23025*k*deltaZ/1000;
};

/***********************************************************************
 * Function: sum
 * Parameters: double array 
 * Returns: double total
 ***********************************************************************/
double sum(double *pArray, int length)
{
  long double total = 0;
  for ( int i = 0; i < length; i++)
  {
    total += pArray[i];
  }
  return total;
};

/***********************************************************************
 * Function: atmosphereTerm
 * Parameters: array tau, array brightness, integer length
 * Returns: double result
 * Summary: calculates value for second term in general radiative 
 * transfer equation
 ***********************************************************************/
double atmosphereTerm(double tau[], double brightness[], int length)
{
  double value = 0;
  double sum1 = 0;
  double sum2 = tau[length - 1];
  for (int i = length - 2; i > 0; i--)
    { //sum up
      sum1 += tau[i+1];
      sum2 += tau[i];
      //cout << "E term" << (exp(-sum1) - exp(-sum2)) << endl;
      value += brightness[i]*(exp(-sum1) - exp(-sum2));
    }
  //cout << "Sum1 = " << sum1 << endl;
  //cout << "Sum2 = " << sum2 << endl;
  //cout << exp(-sum2) << endl;
  return value;
};

/**********************************************************************
 * Function: reflectedTerm
 * Parameters: array tau, array brightness, integer length
 * Returns: double result
 * Summary: calculates value for third term in general radiative 
 * transfer equation
 **********************************************************************/
double reflectedTerm(double tau[], double brightness[], int length)
{
  double value = 0;
  double sum1 = tau[0];
  double sum2 = 0;
  for ( int i = 1; i < length-1; i++)
    {
      sum1 += tau[i];
      sum2 += tau[i-1];
      value += brightness[i]*(exp(-sum1) - exp(-sum2));
    }
  return value;
}

/***********************************************************************
 * Function: frequency to wavelength
 * Parameters: double frequency
 * Returns: wavelength
 ***********************************************************************/
double freqToWavelength(double freq)
{
  return SPEED_OF_LIGHT/freq;
}

/***********************************************************************
 * Function: wavelength to frequency
 * Parameters: double wavelength
 * Returns: frequency
 ***********************************************************************/
double wavelengthToFreq(double wavelength)
{
  return SPEED_OF_LIGHT/wavelength;
}
