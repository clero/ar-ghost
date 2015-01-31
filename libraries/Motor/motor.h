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

#ifndef MOTOR_H
#define MOTOR_H

/**
 * \fn void initMotor(int outForward, int outReverse)
 * \brief Initialise the motor
 * \param outForward : number of the Arduino's digital pin going on the input FIn (Forward In) of the motor's driver
 * \param outReverse : number of the Arduino's digital pin going on the input RIn (Reverse In) of the motor's driver
 */
void initMotor(int outForward, int outReverse);

/**
 * \fn void forward()
 * \brief Run forward the motor
 */
void forward();

/**
 * \fn void reverse()
 * \brief Run reverse the motor
 */
void reverse();

/**
 * \fn void stay()
 * \brief Stop the motor
 */
void stay();

#endif