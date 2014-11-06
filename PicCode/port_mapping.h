/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: port_mapping.h
* Dependenies: See INCLUDES setion below
* Proessor: PIC33EP512GP806
* Compiler: gcc-pic33
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Port definitions for the game player PIC
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun                                SECON 2015
*********************************************************************/

#define RUBIKS_PLATFORM_PIN         _LATD11
#define RUBIKS_TWIST_PIN            _LATD2

#define SIMON_YELLOW_PIN            _LATD3
#define SIMON_BLUE_PIN              _LATD4
#define SIMON_RED_PIN               _LATD6
#define SIMON_GREEN_PIN             _LATD5

#define ARM_POSITION_PIN            _LATD7
#define ARM_SLIDE_PIN               _LATD8

#define ETCH_VERTICAL_PIN           _LATD9
#define ETCH_HORIZ_PIN              _LATD10

#define YELLOW_DETECTOR_PIN         _RB0
#define BLUE_DETECTOR_PIN           _RB1
#define RED_DETECTOR_PIN            _RB2
#define GREEN_DETECTOR_PIN          _RB3

#define START_PHOTO_RESISTOR_PIN    _RB4