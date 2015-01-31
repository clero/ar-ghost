/**
 * \file diode.h
 * \brief Diode init & check
 * \author Thomas Antonini
 * \version 1.2
 * \date 09/12/2014
 *
 * Function to check if the light beam between the photodiodes is hidden
 *
 */

#ifndef DIODE_H
#define DIODE_H

const int THRESHOLD = 800; //Threshold for the light detection, between 0 and 1024

/**
 * \fn void initDiode(int inDiode);
 * \brief Initialise the photodiode
 * \param inDiode : number of the Arduino's digital pin going on the photodiode
 */
void initDiode(int inDiode);

/**
 * \fn bool checkDiode()
 * \brief Check if the light beam is hidden or not
 * \return Returns true if the light beam is not hidden
 */
bool checkDiode(); //Returns true if the light is not hidden

#endif