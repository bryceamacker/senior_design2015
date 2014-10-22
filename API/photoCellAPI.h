/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: photoCellAPI.h
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Declerations of functions and constants to help
* read from photo cells
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        10/13/2014              SECON 2015
*********************************************************************/

#include "pic24_all.h"
#include "port_mapping.h"
#include "ADCAPI.h"

#ifndef PHOTO_CELL_API_H_
#define PHOTO_CELL_API_H_

typedef enum {
    YELLOW_TRANS = 0,
    BLUE_TRANS = 1,
    RED_TRANS = 2,
    GREEN_TRANS = 3
} photoTrans;

/////////////////////////////////////////////// 
//
// Photocell config
//
///////////////////////////////////////////////

/**
 * @brief Configure the analog channels and ADC
 */

void  photo_cell_init();

/**
 * @brief Read the value from a photo transistor
 * 
 * @param color Photo transistor to be read
 * @return the value of the photo transistor
 */
int16_t read_photo_transistor(photoTrans color);
void photo_trans_print();
#endif