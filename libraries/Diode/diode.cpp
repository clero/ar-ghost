/**
 * \file diode.cpp
 * \brief Diode init & check
 * \author Thomas Antonini
 * \version 1.2
 * \date 09/12/2014
 *
 * Function to check if the light beam between the photodiodes is hidden
 *
 */

#include <Arduino.h>
#include "diode.h"

int gInDiode;

/**
 * \fn void initDiode(int inDiode)
 * \brief Initialise the photodiode
 * \param inDiode : number of the Arduino's digital pin going on the photodiode
 */
void initDiode(int inDiode)
{
	pinMode(inDiode, INPUT);
	gInDiode = inDiode;
}

/**
 * \fn bool checkDiode()
 * \brief Check if the light beam is hidden or not
 * \return Returns true if the light beam is not hidden
 */
bool checkDiode()
{
	bool check;
	
	if (analogRead(gInDiode) < THRESHOLD)
	{
		check = true;
	}
	else
	{
		check = false;
	}
	return check;
}