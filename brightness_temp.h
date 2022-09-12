/***********************************************************************
 * Header File:
 *    brightness_temp.h
 * Author:
 *    Alexander Marvin
 * Summary:
 *    Header file for planck functions
 ************************************************************************/
#ifndef _BRIGHTNESS_TEMP_H
#define _BRIGHTNESS_TEMP_H

//now define some stuff (used NIST values)
#define PLANCK_CONST 6.6260755 // *10^-34 Joule-Seconds
#define SPEED_OF_LIGHT 299792458 // meters per second
#define BOLTZMAN_CONST 1.3806568 //*10^-23 Joule per Kelvin

long double planckv(double freq, double temp);
long double planckw(double wavelength, double temp);

#endif // _BRIGHTNESS_TEMP_H
