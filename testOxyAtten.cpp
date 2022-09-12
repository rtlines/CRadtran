/***********************************************************************
 * testOxyAtten.cpp:
 *    Driver for oxygenAtten class
 * Author:
 *    Alexander Marvin
 * Summary:
 *    Test oxygenAtten class
 ************************************************************************/
#include "atten.h"
#include "datafile.h"
#include <iostream>
#include <cmath>
using namespace std;

void loopFreq(datafile & data, oxygenAtten & test);

int main()
{ 
  cout << "testOxyAtten running\n";
  //initialize
  datafile data("TIGR0001");
  //read in file
  data.readFile(true);
  //set altitude
  data.setAltitude(0);
  cout << "Altitude is set to 0 km\n";
  //pass pointer to oxygenAtten class
  oxygenAtten test(&data);
  //loop through frequencies
  loopFreq(data, test);

  //now do for another altitude
  data.setAltitude(8e+3);
  cout << endl;
  cout << "Altitude is set to 8 km\n";
  //reset values
  test.resetValues();
  //now loop through Frequencies
  loopFreq(data, test);

  //set for third altitude
  data.setAltitude(30e+3);
  cout << endl;
  cout << "Altitude is set to 30 km\n";
  //now loop through Frequencies
  loopFreq(data, test);
}

void loopFreq(datafile & data, oxygenAtten & test)
{
  double testValue;
  int start = 50;
  int end = 70;
  double stepsize = 1;
  int numValues = (end - start)/stepsize + 1;
  cout << "Frequency\tAttenuation\n";
  for (int i = 0; i < numValues; i++)
    {
      //set frequency
      double frequency = start+i*stepsize;
      data.setFreq(frequency);
      //reset values
      test.resetValues();
      //testing code
      testValue = test.getValue();
      cout << frequency << "\t" << testValue << " db/km" << endl;
    }
}
