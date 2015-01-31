/**
 * \file interrupter.h
 * \brief Interrupter init & check
 * \author Thomas Antonini
 * \version 1.2
 * \date 09/12/2014
 *
 * Function to check if the interrupter is pressed or not
 *
 */

#ifndef INTERRUPTER_H
#define INTERRUPTER_H

/**
 * \fn void initSwitch(int inSwitch)
 * \brief Initialise the switch
 * \param inSwitch : number of the Arduino's digital pin going on the photodiode
 */
void initSwitch(int inSwitch);

/**
 * \fn bool checkSwitch()
 * \brief Check if the interrupter is pressed or not
 * \return Returns true if the interrupter is pressed
 */
bool checkSwitch();

#endif;