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
      //cout << "temperature" << temperature << endl; 
      //calculate optical length for slice/cout
      //
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
  //cout << "Value of the optical depth is " << totalTrans << endl;
  //cout << "Value of T is " << exp(-totalTrans) << endl;
  //cout << "Value of epsilon " << epsilon << endl;
  //cout << "Value of surface exitance is " << bright_Temp_Surface << endl;
  firstTerm = epsilon*bright_Temp_Surface*exp(-totalTrans);

  //calculate second term
  secondTerm = atmosphereTerm(tau, brightTemp, M);
   //    cout << "second term "<< secondTerm << " in TB " << secondTerm*3.2548e39/pow((frequency*1e9),2)<<endl;
  //calculate third and last term
  //thirdTerm = (1-epsilon)*reflectedTerm(tau, brightTemp, M);

  //debugging
  
  //cout << "Value of the first term is " << firstTerm << " or " << firstTerm*3.2548e39/pow((frequency*1e9),2)<< endl;
  //cout << "Value of the second term is " << secondTerm <<  " or " << secondTerm*3.2548e39/pow((frequency*1e9),2)<< endl;
;
  //cout << "Value of the third term is " << thirdTerm << endl;
  
  //add terms and return value
  cout << "final result in TB " << (firstTerm + secondTerm + thirdTerm)*3.2548e39/pow((frequency*1e9),2)<<endl;

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
  double trans_surf_to_z[length - 1];  //GAM5
  double trans_z_to_toa[length - 1];   //GAM6
  double sum1 = 0;
  double sum2 = tau[length - 1];
  int ik;

  // start with a transmission from surf to first layer of 1
  trans_surf_to_z[0] = 1;
  //cout << "length = " << length << endl;
  //cout << "trans_surf_to_z[" <<0 <<"], " << trans_surf_to_z[0] << endl;
  // now do the transmission from surf to z layer for the rest of 
  // the atmpsphere
  
  for (int i =  0; i < length -1; i++)    
    {
    sum1 += tau[i];
    trans_surf_to_z[i+1]=exp(-sum1);
    //cout << "trans_surf_to_z[" <<i+1 <<"], " << trans_surf_to_z[i+1] << endl;
    }

   // now do the transmission from layer z to the TOA 
   //   take the first layer as a special case
     if (trans_surf_to_z[0] != 0) 
        {
         trans_z_to_toa[0] = trans_surf_to_z[length-1] / trans_surf_to_z[0];
        }
     else
       {
        trans_z_to_toa[0] = 0.0;
       }
     //cout << "trans_z_to_toa["<<0<<"], "<< trans_z_to_toa[0] << endl;
   // now do the rest of the atmosphere
   for (int i =  0; i < length -1; i++)    
     {
     ik=i+1; 
     if (trans_surf_to_z[ik] != 0) 
        {
         trans_z_to_toa[ik] = trans_surf_to_z[length-1] / trans_surf_to_z[ik];
        }
     else
       {
        trans_z_to_toa[ik] = 0.0;
       }
     //cout << "trans_z_to_toa["<<ik<<"], "<< trans_z_to_toa[ik] << endl;
     }
  // now calculate the upward intensity
  sum1=0.0;
  for (int i = 0; i < length - 2;  i++)
    { //sum up
      sum1 += brightness[i]*(trans_z_to_toa[i + 1] - trans_z_to_toa[i]);
     // cout << "planck["<<i<<"], "<< "(trans_z_to_toa["<<i + 1<<"] - trans_z_to_toa["<<i<<"]) "<<(trans_z_to_toa[i + 1] - trans_z_to_toa[i])<<endl;
    }
   value=sum1;
   cout << " upward intensity  " << value << endl;
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
