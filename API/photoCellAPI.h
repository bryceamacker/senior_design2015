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

/////////////////////////////////////////////// 
//
// Photocell config
//
///////////////////////////////////////////////

void  photo_cell_init();

#endif