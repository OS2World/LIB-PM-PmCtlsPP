# makefile.wat - OpenWatcom 2 Makefile for PmCtlsTest sample
# Builds PmCtlsTest.exe demonstrating the pmctlspp.dll library
#
# Usage: wmake -f makefile.wat
# Run:   set PMCTLSPP=EXTPUSHBUTTON,EXTENTRYFIELD,EXTCOMBOBOX
#        PmCtlsTest.exe

CC     = wcc386
CFLAGS = -bt=os2 -5 -fpi -Oaxt -W3
LIBDIR = ..\..\lib
DLLDIR = ..\..\dll

all: PmCtlsTest.exe pmctlspp.dll

PmCtlsTest.exe: PmCtlsTest.obj
	wlink &
	    system os2v2_pm &
	    name PmCtlsTest.exe &
	    file PmCtlsTest.obj &
	    library $(LIBDIR)\pmctlspp.lib &
	    option map=PmCtlsTest.map

PmCtlsTest.obj: PmCtlsTest.c PmCtlsTest.h
	$(CC) $(CFLAGS) -fo=PmCtlsTest.obj PmCtlsTest.c

pmctlspp.dll: $(DLLDIR)\pmctlspp.dll
	copy $(DLLDIR)\pmctlspp.dll pmctlspp.dll

clean:
	@if exist PmCtlsTest.obj del PmCtlsTest.obj
	@if exist PmCtlsTest.exe del PmCtlsTest.exe
	@if exist PmCtlsTest.map del PmCtlsTest.map
	@if exist pmctlspp.dll del pmctlspp.dll
