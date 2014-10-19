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

#include "photoCellAPI.h"

void  photo_cell_init() {  
    printf("Photo cell init\n");
    CONFIG_RB0_AS_ANALOG();
    CONFIG_RB1_AS_ANALOG();
    CONFIG_RB3_AS_ANALOG();
    CONFIG_RB4_AS_ANALOG();

    uint32_t  u32_ticks;

    T3CONbits.TON = 0;
    T2CONbits.TON = 0;
    T2CON = T2_32BIT_MODE_ON | T2_PS_1_1 | T2_SOURCE_INT;
    TMR3 = 0;
    TMR2 = 0;
    u32_ticks = usToU32Ticks(15625, getTimerPrescale(T2CONbits)) - 1;     // # of ticks for 1/64 seconds
    PR3 = u32_ticks>>16;
    PR2 = u32_ticks & 0xFFFF;
    T2CONbits.TON = 1;

    adc_init(RB3_AN, ADC_CH123_POS_SAMPLEB_AN0AN1AN2, ADC_CONV_CLK_10Tcy);
    SET_SAMP_BIT_ADC1();
    
    // CONFIG_RC3_AS_ANALOG();
    // CONFIG_RC4_AS_ANALOG();
}