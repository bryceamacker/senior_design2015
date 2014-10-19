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
# ****************************************************************
# SConstruct.py - Build all libraries and examples over many chips
# ****************************************************************
#  #. Install SCons.
#  #. Install the Microchip compiler. Make sure your path
#     includes the directories in which the compiler binaries
#     exist.
#   From the command line, change to the directory in which
#     this file lies.
#   Execute ``SCons``, which builds everything. Optionally use :doc:`runscons.bat <runscons.bat>` to filter through the resulting warnings.
#
#  The build process can be modified by passing options to
#  SCons. See ``SCons --help`` for options specific
#  to this build and ``SCons -H`` for generic SCons
#  options.
#
# .. contents::

import os
import psutil

# Make sure SCons is recent enough.
EnsureSConsVersion(2, 0)

# ============================================
# USER APP FILES THAT DEFINE THIS APPLICATION
# ============================================
UserAppSources = [
                    'I2C/pic_code.c',
                    'PicCode/pic.c'
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
                'API/sensorArrayAPI.c',
                'API/servosAPI.c',
                'API/photoCellAPI.c',
                'API/ADCAPI.c',
                'API/pic_config.c',
                'Etch/etch_code.c',
                'Rubiks/rubiks_code.c',
                'Simon/simon_code.c',
                'Platform/platform_control.c'
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

# Functions used to build an application with library sources
# ===============================================================
# This function builds a program which includes the PIC24+ESOS library
def buildProgramWithCommonSources(
  # A list of source files to be built into one program.
  sourceFileList,
  # A list of source files upon which all sources
  # in the sourceFileList depend. Wildcards are not
  # supported.
  commonSources,
  # An Environment in which to build these sources.
  buildEnvironment,
  # A string to serve as an alias for this build.  commonSources,
  aliasString):

  be = buildEnvironment
  be.Program(sourceFileList + commonSources)
  # Pick the name of the target to be the first c file in the list
  bin2hex(sourceFileList[0], be, aliasString)

# Command-line options
# --------------------
# adjust our default environment based on user command-line requests
dict = env.Dictionary()
if dict['BOOTLDR'] != 'msu':
    env.Replace(LINKERSCRIPT = '--script="p${MCU}.gld"')

# By default, run number_of_cpus*4 jobs at once. This only works if the --no-cpp option is passed to the linker; otherwise, the linker produces a temporary file in the root build directory, which gets overwritten and confused when multiple builds run. There's some nice examples and explanation for this in the `SCons user guide <http://www.scons.org/doc/production/HTML/scons-user/c2092.html#AEN2183>`_.
#
# Some results from running on my 8-core PC:, gathered from the Total build time returned by the --debug=time scons command-line option:
#
# ==  ==========  ===============  ============
# -j  Time (sec)  Time (hh:mm:ss)  Speedup
# ==  ==========  ===============  ============
# 32   303        0:05:03          11.66006601
# 16   348.7      0:05:49          10.13191855
#  8   510.9      0:08:31           6.915247602
#  4   916        0:15:16           3.8569869
#  2  1777        0:29:37           1.98818233
#  1  3533        0:58:53           1
# ==  ==========  ===============  ============

env.SetOption('num_jobs', psutil.NUM_CPUS*4)
print("Running with -j %d." % GetOption('num_jobs'))

# generate some command line help for our custom options
Help(opts.GenerateHelpText(env))
Help("""Additional targets:
  template-build: Build all .c/.h files which are produced by templates.""")

# A DEBUG STATEMENT to see what the scons build envrionment (env) has defined
#print   env.Dump()
#
#
# Definition of targets
# =====================
# First, set up for defining targets.
#
env = env.Clone(MCU='24HJ64GP506A',
                CPPDEFINES=['HARDWARE_PLATFORM=HARDMAPPED_UART'],
                CPPPATH=[
                  './',
                  'lib/include',
                  'Navigation',
                  'API',
                  'Etch',
                  'Rubiks',
                  'Simon',
                  'PicCode',
                  'Platform'
                  ]
                )

#VariantDir('build_EMBEDDEDF14_33EP512GP806','../../pic24lib_all')

# Inform SCons about the dependencies in the template-based files
# SConscript('templates/SConscript.py', 'env')


for srcFile in UserAppSources:
  env.Program([srcFile, LibSources])
  # Convert it to a .hex
  bin2hex(srcFile, env, 'esos')

