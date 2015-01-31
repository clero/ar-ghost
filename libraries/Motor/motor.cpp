/**
 * \file motor.cpp
 * \brief Motor init & control functions
 * \author Thomas Antonini
 * \version 1.2
 * \date 05/12/2014
 *
 * Functions to stop and turn the motor in both ways
 *
 */

#include <Arduino.h>
#include "motor.h"

int gOutForward;
int gOutReverse;

/**
 * \fn void initMotor(int outForward, int outReverse)
 * \brief Initialise the motor
 * \param outForward : number of the Arduino's digital pin going on the input FIn (Forward In) of the motor's driver
 * \param outReverse : number of the Arduino's digital pin going on the input RIn (Reverse In) of the motor's driver
 */
void initMotor(int outForward, int outReverse)
{
	gOutForward = outForward;
	gOutReverse = outReverse;
	pinMode(outForward, OUTPUT);
	pinMode(outReverse, OUTPUT);
	digitalWrite(outForward, LOW);
	digitalWrite(outReverse, LOW);
	
}

/**
 * \fn void forward()
 * \brief Run forward the motor
 */
void forward()
{
  digitalWrite(gOutForward, HIGH);
  digitalWrite(gOutReverse, LOW);
}

/**
 * \fn void reverse()
 * \brief Run reverse the motor
 */
void reverse()
{
  digitalWrite(gOutForward, LOW);
  digitalWrite(gOutReverse, HIGH);
}

/**
 * \fn void stay()
 * \brief Stop the motor
 */
void stay()
{
  digitalWrite(gOutForward, LOW);
  digitalWrite(gOutReverse, LOW);
}