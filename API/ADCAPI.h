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

#ifndef ADC_API_H_
#define ADC_API_H_

#define LED2    _LATB10
static uint16_t              au16_buffer[8];
static volatile  uint16_t    au16_sum[8];
static volatile  uint8_t     u8_gotData;

/////////////////////////////////////////////// 
//
// ADC config
//
///////////////////////////////////////////////

void adc_init(uint8_t u8_ch0Select, uint16_t u16_ch123SelectMask, uint16_t u16_numTcyMask );

#endif