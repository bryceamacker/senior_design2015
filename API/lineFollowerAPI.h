/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName:
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Bryce Amacker        10/01/2014              SECON 2015
*********************************************************************/

#include "pic24_all.h"
#include "sensorArrayAPI.h"
#include "motorsAPI.h"

float getLine(uint16_t* pau16_sensorValues);

void followLine(uint16_t* pau16_sensorValues, float f_maxSpeed);

void followLineBack(uint16_t* pau16_sensorValues, float f_maxSpeed);

void printLineData(uint16_t* pau16_sensorValues);