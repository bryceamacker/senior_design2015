# .. "Copyright (c) 2008 Robert B. Reese, Bryan A. Jones, J. W. Bruce ("AUTHORS")"
#    All rights reserved.
#    (R. Reese, reese_AT_ece.msstate.edu, Mississippi State University)
#    (B. A. Jones, bjones_AT_ece.msstate.edu, Mississippi State University)
#    (J. W. Bruce, jwbruce_AT_ece.msstate.edu, Mississippi State University)
#
#    Permission to use, copy, modify, and distribute this software and its
#    documentation for any purpose, without fee, and without written agreement is
#    hereby granted, provided that the above copyright notice, the following
#    two paragraphs and the authors appear in all copies of this software.
#
#    IN NO EVENT SHALL THE "AUTHORS" BE LIABLE TO ANY PARTY FOR
#    DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
#    OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE "AUTHORS"
#    HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#    THE "AUTHORS" SPECIFICALLY DISCLAIMS ANY WARRANTIES,
#    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
#    AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
#    ON AN "AS IS" BASIS, AND THE "AUTHORS" HAS NO OBLIGATION TO
#    PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
#
#    Please maintain this header in its entirety when copying/modifying
#    these files.
#
#    ********************************************************************
#    FileName: SConstruct.py
#    Proessor: PIC24HJ64GP506A, PIC24HJ128GP506A
#    Compiler: gcc-xc16
#    Company: Mississippi State University/ECE
#
#    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#    MODULE FUNCTION: Build all the PIC code for SECON 2015
#    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#    Author                Date                    Comment
#    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#    Steven Calhoun        9/20/2014               SECON 2015
#    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
#    ********************************************************************

import os
import psutil

# Make sure SCons is recent enough.
EnsureSConsVersion(2, 0)

# ============================================
# USER APP FILES THAT DEFINE THIS APPLICATION
# ============================================
UserAppSources = [
                    'PicCode/pic_game_player.c',
                    'PicCode/pic_navigation.c'
                  ]

LibSources = [
                'lib/src/dataXfer.c',
                'lib/src/dataXferImpl.c',
                'lib/src/pic24_adc.c',
                'lib/src/pic24_clockfreq.c',
                'lib/src/pic24_configbits.c',
                'lib/src/pic24_ecan.c',
                'lib/src/pic24_flash.c',
                'lib/src/pic24_i2c.c',
                'lib/src/pic24_serial.c',
                'lib/src/pic24_spi.c',
                'lib/src/pic24_stdio_uart.c',
                'lib/src/pic24_timer.c',
                'lib/src/pic24_uart.c',
                'lib/src/pic24_util.c',
                'API/servos_API.c',
                'API/photo_cell_API.c',
                'API/ADC_API.c',
                'API/motors_API.c',
                'API/sensor_array_API.c',
                'API/line_follower_API.c',
                'API/SSDisplayAPI.c',
                'Etch/etch_code.c',
                'Rubiks/rubiks_code.c',
                'Simon/simon_code.c',
                'Card/card_code.c',
                'Platform/platform_control.c',
                'GameArm/game_arm_control.c'
              ]

# Create a Microchip XC16 Construction Environment
# ================================================
# Define command-line options to set bootloader.
# The Environment below depends on opts, so this must go here instead of with
# the rest of the `Command-line options`_.
opts = Variables()
opts.Add(EnumVariable('BOOTLDR', 'Determines bootloader type', 'msu',
                    allowed_values=('msu', 'none')))

# Create the environment.
env = Environment(
        # Force SCons to set up with gnu tools to start
        # with reasonable defaults. Note: using platform = 'posix'
        # causes SCons to try to call fork() when executing programs
        # (such as compilers), which errors out on Windows.
        tools = ['gcc', 'gnulink', 'ar', 'zip', 'packaging'],
        options = opts,
        CPPPATH = 'lib/include',
        CC = 'xc16-gcc',
        LIBPATH = '.',
        AR = 'xc16-ar',
        LINK = 'xc16-gcc',
        # Copied and cobbled together from SCons\Tools\cc.py with mods
        CCCOM = '$CC -c -o $TARGET $CFLAGS $CCFLAGS $CPPFLAGS $_CPPDEFFLAGS $_CPPINCFLAGS $SOURCES',
        CCCCOMSTR = 'Compiling $SOURCES',
        # The warnings provide some lint-like checking. Omitted options: -Wstrict-prototypes -Wold-style-definition complains about void foo(), which should be void foo(void), but isn't worth the work to change.
        CCFLAGS = '-mcpu=${MCU} -O1 -msmart-io=1 -omf=elf -Wall -Wextra -Wdeclaration-after-statement -Wlong-long -fdiagnostics-show-option',
        LINKFLAGS = '-mcpu=${MCU} -omf=elf -Wl,--heap=100,$LINKERSCRIPT,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--no-cpp',
        LINKERSCRIPT = '--script="lib/lkr/p${MCU}_bootldr.gld"',
        ARFLAGS = 'rcs',
        ARSTR = 'Create static library: $TARGET',
        OBJSUFFIX = '.o',
        PROGSUFFIX = '.elf',
        # Copy the host envrionment variables for our scons environment
        # so scons can find the build tools and related env vars.
        ENV = os.environ,
  )

# Create a bin2hex builder
# ------------------------
# Add the bin2hex function to the environment as a new builder
# This functions converts a binary (.elf or .cof) file to a hex file.
def bin2hex(
  # The name of the .elf/.cof file to be converted.
  binName,
  # An Environment in which to build these sources.
  buildEnvironment,
  # A string to serve as an alias for this build.
  aliasString):

  f = os.path.splitext(binName)[0]
  myHex = buildEnvironment.Hex(f, f)
  # Add this hex file to a convenient alias
  buildEnvironment.Alias(aliasString, myHex)

b2h = Builder(
        action = 'xc16-bin2hex $SOURCE -a -omf=elf',
        suffix = 'hex',
        src_suffix = 'elf')
env.Append(BUILDERS = {'Hex' : b2h})

# Command-line options
# --------------------
# adjust our default environment based on user command-line requests
dict = env.Dictionary()
if dict['BOOTLDR'] != 'msu':
    env.Replace(LINKERSCRIPT = '--script="p${MCU}.gld"')

env.SetOption('num_jobs', psutil.NUM_CPUS*4)

# generate some command line help for our custom options
Help(opts.GenerateHelpText(env))
Help("""Builds all of the 2015 SECON PIC code.""")


cppdefines = ['HARDWARE_PLATFORM=HARDMAPPED_UART']
debugBuild = False

for key, value in ARGLIST:
  if key == 'define':
    cppdefines.append(value)
    if value == "DEBUG_BUILD":
      debugBuild = True
      # Add the controllers to be built if this is a debug build
      UserAppSources.append("PicCode/Controllers/pic_game_player_controller.c");
      UserAppSources.append("PicCode/Controllers/pic_navigation_controller.c");

if debugBuild:
  print
  print "*******************************************************"
  print "DEBUG BUILD"
  print "*******************************************************"
  print
else:
  print
  print "*******************************************************"
  print "RELEASE BUILD"
  print "*******************************************************"
  print


# Definition of targets
# =====================
# First, set up for defining targets.
env = env.Clone(MCU='24HJ128GP506A',
                CPPDEFINES=cppdefines,
                CPPPATH=[
                  './',
                  'lib/include',
                  'Navigation',
                  'API',
                  'Etch',
                  'Rubiks',
                  'Simon',
                  'Card',
                  'PicCode',
                  'Platform',
                  'GameArm'
                  ]
                )

# Build each source file
for srcFile in UserAppSources:
  env.Program([srcFile, LibSources])
  # Convert it to a .hex
  bin2hex(srcFile, env, 'SECON')
