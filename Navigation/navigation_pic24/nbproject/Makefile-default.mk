#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/navigation_pic24.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/navigation_pic24.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../../lib/src/pic24_clockfreq.c ../../lib/src/pic24_configbits.c ../../lib/src/pic24_i2c.c ../../lib/src/pic24_serial.c ../../lib/src/pic24_timer.c ../../lib/src/pic24_uart.c ../../lib/src/pic24_util.c ../lineFollower.c ../pic24_qrt8rc.c C:/senior_design2015/Navigation/motor_pwm.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/957557178/pic24_clockfreq.o ${OBJECTDIR}/_ext/957557178/pic24_configbits.o ${OBJECTDIR}/_ext/957557178/pic24_i2c.o ${OBJECTDIR}/_ext/957557178/pic24_serial.o ${OBJECTDIR}/_ext/957557178/pic24_timer.o ${OBJECTDIR}/_ext/957557178/pic24_uart.o ${OBJECTDIR}/_ext/957557178/pic24_util.o ${OBJECTDIR}/_ext/1472/lineFollower.o ${OBJECTDIR}/_ext/1472/pic24_qrt8rc.o ${OBJECTDIR}/_ext/546171520/motor_pwm.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/957557178/pic24_clockfreq.o.d ${OBJECTDIR}/_ext/957557178/pic24_configbits.o.d ${OBJECTDIR}/_ext/957557178/pic24_i2c.o.d ${OBJECTDIR}/_ext/957557178/pic24_serial.o.d ${OBJECTDIR}/_ext/957557178/pic24_timer.o.d ${OBJECTDIR}/_ext/957557178/pic24_uart.o.d ${OBJECTDIR}/_ext/957557178/pic24_util.o.d ${OBJECTDIR}/_ext/1472/lineFollower.o.d ${OBJECTDIR}/_ext/1472/pic24_qrt8rc.o.d ${OBJECTDIR}/_ext/546171520/motor_pwm.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/957557178/pic24_clockfreq.o ${OBJECTDIR}/_ext/957557178/pic24_configbits.o ${OBJECTDIR}/_ext/957557178/pic24_i2c.o ${OBJECTDIR}/_ext/957557178/pic24_serial.o ${OBJECTDIR}/_ext/957557178/pic24_timer.o ${OBJECTDIR}/_ext/957557178/pic24_uart.o ${OBJECTDIR}/_ext/957557178/pic24_util.o ${OBJECTDIR}/_ext/1472/lineFollower.o ${OBJECTDIR}/_ext/1472/pic24_qrt8rc.o ${OBJECTDIR}/_ext/546171520/motor_pwm.o

# Source Files
SOURCEFILES=../../lib/src/pic24_clockfreq.c ../../lib/src/pic24_configbits.c ../../lib/src/pic24_i2c.c ../../lib/src/pic24_serial.c ../../lib/src/pic24_timer.c ../../lib/src/pic24_uart.c ../../lib/src/pic24_util.c ../lineFollower.c ../pic24_qrt8rc.c C:/senior_design2015/Navigation/motor_pwm.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/navigation_pic24.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24HJ64GP506A
MP_LINKER_FILE_OPTION=,--script="..\..\lib\lkr\p24HJ64GP506A_bootldr.gld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/957557178/pic24_clockfreq.o: ../../lib/src/pic24_clockfreq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/957557178 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_clockfreq.o.d 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_clockfreq.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../lib/src/pic24_clockfreq.c  -o ${OBJECTDIR}/_ext/957557178/pic24_clockfreq.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/957557178/pic24_clockfreq.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O1 -I"../../lib/include" -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/957557178/pic24_clockfreq.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/957557178/pic24_configbits.o: ../../lib/src/pic24_configbits.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/957557178 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_configbits.o.d 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_configbits.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../lib/src/pic24_configbits.c  -o ${OBJECTDIR}/_ext/957557178/pic24_configbits.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/957557178/pic24_configbits.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O1 -I"../../lib/include" -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/957557178/pic24_configbits.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/957557178/pic24_i2c.o: ../../lib/src/pic24_i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/957557178 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_i2c.o.d 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../lib/src/pic24_i2c.c  -o ${OBJECTDIR}/_ext/957557178/pic24_i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/957557178/pic24_i2c.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O1 -I"../../lib/include" -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/957557178/pic24_i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/957557178/pic24_serial.o: ../../lib/src/pic24_serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/957557178 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_serial.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../lib/src/pic24_serial.c  -o ${OBJECTDIR}/_ext/957557178/pic24_serial.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/957557178/pic24_serial.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O1 -I"../../lib/include" -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/957557178/pic24_serial.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/957557178/pic24_timer.o: ../../lib/src/pic24_timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/957557178 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../lib/src/pic24_timer.c  -o ${OBJECTDIR}/_ext/957557178/pic24_timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/957557178/pic24_timer.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O1 -I"../../lib/include" -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/957557178/pic24_timer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/957557178/pic24_uart.o: ../../lib/src/pic24_uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/957557178 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../lib/src/pic24_uart.c  -o ${OBJECTDIR}/_ext/957557178/pic24_uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/957557178/pic24_uart.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O1 -I"../../lib/include" -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/957557178/pic24_uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/957557178/pic24_util.o: ../../lib/src/pic24_util.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/957557178 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_util.o.d 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_util.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../lib/src/pic24_util.c  -o ${OBJECTDIR}/_ext/957557178/pic24_util.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/957557178/pic24_util.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O1 -I"../../lib/include" -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/957557178/pic24_util.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/lineFollower.o: ../lineFollower.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/lineFollower.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/lineFollower.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../lineFollower.c  -o ${OBJECTDIR}/_ext/1472/lineFollower.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/lineFollower.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O1 -I"../../lib/include" -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/lineFollower.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/pic24_qrt8rc.o: ../pic24_qrt8rc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pic24_qrt8rc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pic24_qrt8rc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../pic24_qrt8rc.c  -o ${OBJECTDIR}/_ext/1472/pic24_qrt8rc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/pic24_qrt8rc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O1 -I"../../lib/include" -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pic24_qrt8rc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/546171520/motor_pwm.o: C:/senior_design2015/Navigation/motor_pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/546171520 
	@${RM} ${OBJECTDIR}/_ext/546171520/motor_pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/546171520/motor_pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  C:/senior_design2015/Navigation/motor_pwm.c  -o ${OBJECTDIR}/_ext/546171520/motor_pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/546171520/motor_pwm.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O1 -I"../../lib/include" -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/546171520/motor_pwm.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/957557178/pic24_clockfreq.o: ../../lib/src/pic24_clockfreq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/957557178 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_clockfreq.o.d 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_clockfreq.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../lib/src/pic24_clockfreq.c  -o ${OBJECTDIR}/_ext/957557178/pic24_clockfreq.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/957557178/pic24_clockfreq.o.d"        -g -omf=elf -O1 -I"../../lib/include" -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/957557178/pic24_clockfreq.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/957557178/pic24_configbits.o: ../../lib/src/pic24_configbits.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/957557178 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_configbits.o.d 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_configbits.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../lib/src/pic24_configbits.c  -o ${OBJECTDIR}/_ext/957557178/pic24_configbits.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/957557178/pic24_configbits.o.d"        -g -omf=elf -O1 -I"../../lib/include" -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/957557178/pic24_configbits.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/957557178/pic24_i2c.o: ../../lib/src/pic24_i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/957557178 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_i2c.o.d 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../lib/src/pic24_i2c.c  -o ${OBJECTDIR}/_ext/957557178/pic24_i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/957557178/pic24_i2c.o.d"        -g -omf=elf -O1 -I"../../lib/include" -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/957557178/pic24_i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/957557178/pic24_serial.o: ../../lib/src/pic24_serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/957557178 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_serial.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../lib/src/pic24_serial.c  -o ${OBJECTDIR}/_ext/957557178/pic24_serial.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/957557178/pic24_serial.o.d"        -g -omf=elf -O1 -I"../../lib/include" -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/957557178/pic24_serial.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/957557178/pic24_timer.o: ../../lib/src/pic24_timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/957557178 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../lib/src/pic24_timer.c  -o ${OBJECTDIR}/_ext/957557178/pic24_timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/957557178/pic24_timer.o.d"        -g -omf=elf -O1 -I"../../lib/include" -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/957557178/pic24_timer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/957557178/pic24_uart.o: ../../lib/src/pic24_uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/957557178 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../lib/src/pic24_uart.c  -o ${OBJECTDIR}/_ext/957557178/pic24_uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/957557178/pic24_uart.o.d"        -g -omf=elf -O1 -I"../../lib/include" -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/957557178/pic24_uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/957557178/pic24_util.o: ../../lib/src/pic24_util.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/957557178 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_util.o.d 
	@${RM} ${OBJECTDIR}/_ext/957557178/pic24_util.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../lib/src/pic24_util.c  -o ${OBJECTDIR}/_ext/957557178/pic24_util.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/957557178/pic24_util.o.d"        -g -omf=elf -O1 -I"../../lib/include" -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/957557178/pic24_util.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/lineFollower.o: ../lineFollower.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/lineFollower.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/lineFollower.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../lineFollower.c  -o ${OBJECTDIR}/_ext/1472/lineFollower.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/lineFollower.o.d"        -g -omf=elf -O1 -I"../../lib/include" -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/lineFollower.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/pic24_qrt8rc.o: ../pic24_qrt8rc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/pic24_qrt8rc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pic24_qrt8rc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../pic24_qrt8rc.c  -o ${OBJECTDIR}/_ext/1472/pic24_qrt8rc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/pic24_qrt8rc.o.d"        -g -omf=elf -O1 -I"../../lib/include" -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pic24_qrt8rc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/546171520/motor_pwm.o: C:/senior_design2015/Navigation/motor_pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/546171520 
	@${RM} ${OBJECTDIR}/_ext/546171520/motor_pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/546171520/motor_pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  C:/senior_design2015/Navigation/motor_pwm.c  -o ${OBJECTDIR}/_ext/546171520/motor_pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/546171520/motor_pwm.o.d"        -g -omf=elf -O1 -I"../../lib/include" -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/546171520/motor_pwm.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/navigation_pic24.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../../lib/lkr/p24HJ64GP506A_bootldr.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/navigation_pic24.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -Wl,,--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--heap=100,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library-path="../../lib/lkr",--library-path=".",--no-force-link,--smart-io,-Map="${DISTDIR}/line_follower.X.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/navigation_pic24.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../../lib/lkr/p24HJ64GP506A_bootldr.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/navigation_pic24.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--heap=100,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library-path="../../lib/lkr",--library-path=".",--no-force-link,--smart-io,-Map="${DISTDIR}/line_follower.X.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/navigation_pic24.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
