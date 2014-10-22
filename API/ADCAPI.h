/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: photoCellAPI.c
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Functions to help read from photo cells
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        10/13/2014              SECON 2015
*********************************************************************/

#include "pic24_all.h"
#include "port_mapping.h"
#include <stdio.h>

#ifndef ADC_API_H_
#define ADC_API_H_

#define YELLOW_LIGHT    1
#define BLUE_LIGHT      0
#define RED_LIGHT       3
#define GREEN_LIGHT     2


/////////////////////////////////////////////// 
//
// ADC config
//
///////////////////////////////////////////////

void adc_init(uint8_t u8_channel);

void adc_change_channel(uint8_t u8_channel);

int16_t adc_read(uint8_t u8_channel);

void adc_print();

#endif