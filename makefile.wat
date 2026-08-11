# makefile.wat - OpenWatcom 2 Makefile for OS/2 PMCTLSPP DLL
# Builds PMCTLSPP DLL with OpenWatcom 2 on ArcaOS
# Outputs: dll/pmctlspp.dll, lib/pmctlspp.lib
#
# Usage: wmake -f makefile.wat

CC = wcc386

# Compiler flags for DLL (32-bit OS/2 PM) - include src directory for headers
CFLAGS = -bt=os2 -bd -bm -5 -fpi -Oaxt -W3 -Isrc

all: dll\pmctlspp.dll lib\pmctlspp.lib

# -------------------------------------------------------------------
# PMCTLSPP.DLL
# -------------------------------------------------------------------
dll\pmctlspp.dll: src\pmctlspp.obj src\BtnDlgProc.obj src\EntryDlgProc.obj src\ComboDlgProc.obj src\pmctlspp.def
	wlink system os2v2 dll initinstance name dll\pmctlspp.dll &
	    file { src\pmctlspp.obj src\BtnDlgProc.obj src\EntryDlgProc.obj src\ComboDlgProc.obj } &
	    @src\pmctlspp.def &
	    option map=dll\pmctlspp.map &
	    option implib=lib\pmctlspp.lib

# Object files
src\pmctlspp.obj: src\pmctlspp.c src\pmctlspp.h
	$(CC) $(CFLAGS) -fo=src\pmctlspp.obj src\pmctlspp.c

src\BtnDlgProc.obj: src\BtnDlgProc.c src\pmctlspp.h
	$(CC) $(CFLAGS) -fo=src\BtnDlgProc.obj src\BtnDlgProc.c

src\EntryDlgProc.obj: src\EntryDlgProc.c src\pmctlspp.h
	$(CC) $(CFLAGS) -fo=src\EntryDlgProc.obj src\EntryDlgProc.c

src\ComboDlgProc.obj: src\ComboDlgProc.c src\pmctlspp.h
	$(CC) $(CFLAGS) -fo=src\ComboDlgProc.obj src\ComboDlgProc.c

# -------------------------------------------------------------------
clean:
	@if exist src\pmctlspp.obj del src\pmctlspp.obj
	@if exist src\BtnDlgProc.obj del src\BtnDlgProc.obj
	@if exist src\EntryDlgProc.obj del src\EntryDlgProc.obj
	@if exist src\ComboDlgProc.obj del src\ComboDlgProc.obj
	@if exist dll\pmctlspp.dll del dll\pmctlspp.dll
	@if exist dll\pmctlspp.map del dll\pmctlspp.map
	@if exist lib\pmctlspp.lib del lib\pmctlspp.lib
	@if exist compile.log del compile.log