#!/bin/sh
#******************************************************************************
#
# ACPICA release generation script for Windows execution
#
#              $Revision: 1.64 $
#
# Requires cygwin be installed http://sources.redhat.com/cygwin/
# and its /bin be *first* in your path
# Also requires zip.
#
# Execute this script from the top level ACPI source directory.
#
# Parameters: notests - do not generate test source code package
#
#******************************************************************************

cp -rf /work/git/acpica/* .

#
# Move and preserve any previous versions of the various release packages
#
mkdir -p release_current
mkdir -p release_archive
rm -f release_current/acpica-release-*.zip
rm -f release_current/acpica-unix-*.tar.gz
rm -f release_current/acpica-win-*.zip
rm -f release_current/iasl-win-*.zip
rm -f release_current/acpitests-unix-*.tar.gz
rm -f release_current/acpitests-win-*.zip

#
# Ensure that the temporary directories are created fresh
#
rm -rf wintemp
rm -rf unixtemp
rm -rf acpica-unix-`date +%Y%m%d`

DOS2UNIX=d2u
OS=`uname`
if [ "${OS:0:5}" == "Linux" ] ; then
DOS2UNIX=dos2unix
fi

#******************************************************************************
#
# Unix source code package
#
#******************************************************************************
echo
echo ACPICA - Unix source code package
echo

#
# Make temp directories
#
mkdir -p unixtemp/generate
mkdir -p unixtemp/tools
mkdir -p unixtemp/os_specific

#
# Copy ACPICA subsystem source code
#
cp -r CHANGES.txt unixtemp/changes.txt
cp -r source/common unixtemp/common
cp -r source/components/* unixtemp
cp -r source/include unixtemp/include

#
# iASL compiler source
#
cp -r source/compiler unixtemp/compiler

#
# ACPICA tools source
#
cp -r source/tools/acpisrc unixtemp/tools
cp -r source/tools/acpiexec unixtemp/tools
cp -r source/tools/acpixtract unixtemp/tools

#
# OS-specific interfaces
#

cp -r source/os_specific/service_layers unixtemp/os_specific/service_layers
cp source/os_specific/service_layers/osunixxf.c unixtemp
cp source/os_specific/service_layers/osunixdir.c unixtemp/tools/acpisrc
cp source/os_specific/service_layers/osunixdir.c unixtemp/tools/acpiexec

#
# Copy UNIX makefiles
#
cp generate/linux/Makefile.iasl unixtemp/compiler/Makefile
cp generate/linux/Makefile.acpisrc unixtemp/tools/acpisrc/Makefile
cp generate/linux/Makefile.acpiexec unixtemp/tools/acpiexec/Makefile
cp generate/linux/Makefile.acpixtract unixtemp/tools/acpixtract/Makefile
cp generate/linux/README.acpica-unix unixtemp/README

#
# Copy Lint directory, delete extraneous files
#
cp -r generate/lint unixtemp/generate/lint
rm -f unixtemp/generate/lint/co*
rm -f unixtemp/generate/lint/env*
rm -f unixtemp/generate/lint/lib*
rm -f unixtemp/generate/lint/LintOut.txt

#
# Delete extraneous files
#
cd unixtemp
find . -name "*.scc"|xargs rm -f
find . -name "CVS"|xargs rm -r -f

#
# Convert all LF/CR pairs to Unix format (LF only)
#
find . -name "*"|xargs $DOS2UNIX
echo Conversion complete
cd ..

#
# Build release package
#
mv unixtemp acpica-unix-`date +%Y%m%d`
tar czf acpica-unix-`date +%Y%m%d`.tar.gz acpica-unix-`date +%Y%m%d`

#
# Move the completed package
#
mv acpica-unix-`date +%Y%m%d`.tar.gz release_current

#
# Remove the temporary directory
#
rm -rf acpica-unix-`date +%Y%m%d`

#******************************************************************************
#
# Windows source code package
#
#******************************************************************************
echo
echo ACPICA - Windows source code package
echo

#
# Make temp directories
#
mkdir wintemp
mkdir wintemp/libraries
mkdir -p wintemp/generate/msvc
mkdir -p wintemp/source/os_specific/service_layers

#
# Copy ACPICA subsystem source code
#
cp -r CHANGES.txt wintemp/changes.txt
cp -r source/common wintemp/source/common
cp -r source/components wintemp/source/
cp -r source/include wintemp/source/include

#
# iASL compiler source
#
cp -r source/compiler wintemp/source/compiler

#
# ACPICA tools source
#
cp -r source/tools wintemp/source/tools

#
# Copy all OS-specific interfaces
#
cp source/os_specific/service_layers/*.c wintemp/source/os_specific/service_layers

#
# Copy project files for MS VC++ (6.0)
#
cp -r generate/msvc/*.dsp wintemp/generate/msvc/
cp -r generate/msvc/*.dsw wintemp/generate/msvc/

#
# Copy Lint directory, delete extraneous files
#
cp -r generate/lint wintemp/generate/lint
rm -f wintemp/generate/lint/co*
rm -f wintemp/generate/lint/env*
rm -f wintemp/generate/lint/lib*
rm -f wintemp/generate/lint/LintOut.txt

#
# Delete extraneous files
#
cd wintemp
find . -name "*.scc"|xargs rm -f
find . -name "*.ncb"|xargs rm -f
find . -name "*.opt"|xargs rm -f
find . -name "CVS"|xargs rm -r -f

#
# Build release package
#
zip -r -9 ../release_current/acpica-win-`date +%Y%m%d`.zip * > /dev/null
cd ..

#
# Remove temporary directory
#
rm -rf wintemp


#******************************************************************************
#
# Windows iASL executable package
#
#******************************************************************************
echo
echo iASL Compiler - Windows executable package
echo

#
# Make temp directory
#
mkdir wintemp

#
# Copy executables and documentation
#
cp -r CHANGES.txt wintemp/changes.txt
cp documents/aslcompiler.pdf wintemp
cp libraries/iasl.exe wintemp
cp libraries/acpixtract.exe wintemp

#
# Build release package
#
cd wintemp
zip -r -9 ../release_current/iasl-win-`date +%Y%m%d`.zip * > /dev/null
cd ..

#
# Remove temporary directory
#
rm -rf wintemp


#******************************************************************************
#
# Source code release (email) package
# Contains: iASL executable, unix source package, and changes.txt file
#
# This zipfile is created with a password so that it can be safely emailed
# without interference from overzealous email scanners.
# Password is acpica
#
#******************************************************************************
echo
echo ACPICA - Source code release package
echo

cp CHANGES.txt release_current/changes.txt
cd release_current
zip -r -9 -P acpica acpica-release-`date +%Y%m%d`.zip changes.txt iasl-win-`date +%Y%m%d`.zip acpica-unix-`date +%Y%m%d`.tar.gz > /dev/null
rm changes.txt
cd ..


#******************************************************************************
#
# Skip to package acpitests if "notests" option specified
#
#******************************************************************************
if [ "$1" != "notests" ]
then
    echo APCICA test suites will not be generated

#******************************************************************************
#
# Unix ASL Test source code package
#
#******************************************************************************
echo
echo ACPICA - Unix ASL Test source code package
echo

#
# Make temp directory
#
mkdir unixtemp

#
# ASL Test source
#
cp -r tests unixtemp/tests

#
# Delete extraneous files
#
cd unixtemp
find . -name "tmp"|xargs rm -r -f
find . -name "CVS"|xargs rm -r -f
cd ..

#
# Convert all LF/CR pairs to Unix format (LF only)
#
cd unixtemp
find . -name "*"|xargs $DOS2UNIX
cd ..

#
# Build release package
#
mv unixtemp acpitests-unix-`date +%Y%m%d`
tar czf acpitests-unix-`date +%Y%m%d`.tar.gz acpitests-unix-`date +%Y%m%d`

#
# Move the completed package
#
mv acpitests-unix-`date +%Y%m%d`.tar.gz release_current

#
# Remove the temporary directory
#
rm -rf acpitests-unix-`date +%Y%m%d`

#******************************************************************************
#
# Windows ASL Test source code package
#
#******************************************************************************
echo
echo ACPICA - Windows ASL Test source code package
echo

#
# Make temp directory
#
mkdir wintemp

#
# ASL Test source
#
cp -r tests wintemp/tests

#
# Delete extraneous files
#
cd wintemp
find . -name "tmp"|xargs rm -r -f
find . -name "CVS"|xargs rm -r -f
cd ..

#
# Build release package
#
cd wintemp
zip -r -9 ../release_current/acpitests-win-`date +%Y%m%d`.zip * > /dev/null
cd ..

#
# Remove temporary directory
#
rm -rf wintemp
fi

#******************************************************************************
#
# Cleanup
#
#******************************************************************************

#
# Save any older versions of the release packages
#
cp -f release_current/acpica-unix-*.tar.gz release_archive
cp -f release_current/acpica-win-*.zip release_archive
cp -f release_current/iasl-win-*.zip release_archive
if [ "$1" != "notests" ] ; then
	cp -f release_current/acpitests-unix-*.tar.gz release_archive
	cp -f release_current/acpitests-win-*.zip release_archive
fi

date
