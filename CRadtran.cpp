/***********************************************************************
 * Source code:
 *    main driver
 * Author:
 *    Alexander Marvin
 * Summary:
 *    Calls the other code, and also does tests.
 ***********************************************************************/
#include <iostream>
#include <string>
#include <fstream>
#include "calculate.h"
using namespace std;

//function prototypes
bool readFile(string filename, setup & settings);
bool writeFile(string filename, double data[][2], int length[], double **ppK);

/***********************************************************************
 * MAIN
 ***********************************************************************/
int main(int argc, char** argv)
{
  //initialize stuff
  double radiance = 0;
  double frequency = 0;
  string input_file;
  string output_file;
  setup settings;
  bool ok = true;
  char ans;
  //check the number of command line parameters
  if (argc < 2)
    {
      cout << "**Invalid number of parameters**\n";
      cout << "Usage: CRadtran INPUT [OUTPUT FILE NAME]\n";
      return -1;
    }
  else if(argc == 2)
    {
      input_file = argv[1];
      output_file = "results.txt";
    }
  else
    {
      input_file = argv[1];
      output_file = argv[2];
    }

  //read in file
  ok = readFile(input_file, settings);
  if (!ok)
    return -1;

  //setup array to hold stuff
  int length[2];
  length[0] = (settings.max - settings.min)/settings.step+1;
  length[1] = 0;
  double results[length[0]][2];
  double * ppK[length[0]];

  if (settings.printK)
    length[1] = settings.M;
  //cout << "Number of slices is " << settings.M << endl;
  //loop array
  for (int i = 0; i < length[0]; i++)
    {
      frequency = i*settings.step+settings.min; //frequency is in GHz
      //wavelength comes back in meters, convert to Hz first
      radiance = upwardRadiance(frequency, settings);
      //place in array
      results[i][0] = frequency;
      results[i][1] = radiance;
      /*if (settings.printK)
	{
	  cout << settings.pKappa[8] << endl;;
	}
      */
    }

  //now write results to file
  ok = writeFile(output_file, results, length, ppK);
  //if not defined, compile this part
  //otherwise don't
#ifndef RUNTIME_LIB
  while(!ok)
    {
      cout << "Would you like to attempt to write again(y/n)? ";
      cin >> ans;
      //if no, exit
      if (ans == 'n' || ans == 'N')
	return -1;
      //otherwise continue
      cout << "Do you want to change the output file(y/n)? ";
      cin >> ans;
      if (ans == 'y' || ans == 'Y')
	{
	  cout << "Please enter the name of the new file: ";
	  cin >> output_file;
	}
      else
	{
	  cout << "Attempting to write to file ...\n";
	}
      ok = writeFile(output_file, results, length, ppK);
    }
  cout << "Write to output file successful!\n";
#else
  if (!ok)
    return -2; //return error 
#endif

  return 0;
};

bool readFile(string filename, setup & settings)
{
  //initialization
  ifstream fin;
  string temp;
  string temp2;
  
  //open file
  fin.open(filename.c_str());

  //check to if opened
  if (!fin.is_open())
    {
      //Print error
      cerr << "**Unable to open " << filename << "!**\n";
      return false;
    }

  //read from file
  fin >> temp;

  //check for type of run
  if (temp == "Range")
    { //get range
      fin >> settings.min;
      fin >> settings.max;
      //get stepsize
      fin >> temp;
      if (temp == "Step")
	{
	  fin >> settings.step;
	}
      else //else set to default stepsize of 1
	settings.step = 1;
    }
  else if (temp == "Frequency" || temp == "frequency")
    {
      fin >> settings.min;
      settings.max = settings.min;
      settings.step = 1;
    }
  else
    {
      cerr << "**Invalid file!**\n";
      fin.close();
      return false;
    }

  //read in number of slices
  fin >> temp;
  if (temp == "slices" || temp == "Slices")
    {
      fin >> settings.M;
    }
  else
    {
      cerr << "**Invalid file!**\n";
      fin.close();
      return false;
    }

  //read in surface emissivity
  fin >> temp;
  if (temp == "Emissivity" || temp == "emissivity")
    {
      fin >> settings.epsilon;
    } 
  else
    {
      cerr << "**Invalid file!**\n";
      fin.close();
      return false;
    }

  //read in surface temperature
  fin >> temp;
  fin >> temp2;
  if ((temp=="Surface"||temp=="surface")&&(temp2=="Temperature"||temp2=="temperature"))
    {
      fin >> settings.surfaceTemperature;
    }
  else
    {
      cerr << "**Invalid file!**\n";
      fin.close();
      return false;
    }

  //read in option: Print K?
  fin >> temp;
  fin >> temp2;
  if ((temp=="Print"||temp=="print")&&(temp2=="attenuation"||temp2=="Attenuation"))
    {
      fin >> temp;
      if (temp == "true" || temp == "TRUE" || temp == "True")
	settings.printK = true;
      else
	settings.printK = false;
    }

  //read optional max altitude
  fin >> temp;
  fin >> temp2;
  if ((temp=="Max"||temp=="max")&&(temp2=="Altitude"||temp2=="altitude"))
    {
      fin >> settings.max_altitude;
    }
  else if (fin.eof())
    {
      settings.max_altitude = A0;
    }
  else
    {
      cerr << "**Invalid file**\n";
    }

  if (fin.fail() && !fin.eof() || fin.bad())
    {
      cerr << "**File read failed!**\n";
    }

  //close file
  fin.close();
  
  //return
  return true;
}

bool writeFile(string filename, double data[][2], int length[], double ** ppK)
{
  //initialize
  ofstream fout;

  //open file
  fout.open(filename.c_str());
  //fout.open("example.txt");

  //check that it opens
  if (!fout.is_open())
    {
      cerr << "**Unable to open file!**\n";
      return false;
    }
  //write stuff to file
  fout << "Freq(GHz), Radiance" << endl;
  for (int i = 0; i < length[0]; i++)
    {
      fout << data[i][0] << ", " << data[i][1] << endl;
    }
  /*
  if (length[1])
    {
      for (int i = 0; i < length[0]; i++)
	{
	  fout << "Attenuation at " << data[i][0] << "GHz" << endl;
	  fout << "Slice\tdB/km" << endl;
	  for (int j = 0; j < length[1]; j++)
	    {
	      fout << j << "\t" << ppK[i][j] << endl;
	    }
	}
    }
  */
  //check again
  if (fout.fail())
    {
      cerr << "**Write to file failed!**\n";
      return false;
    }
  
  //close file
  fout.close();

  return true;
}
