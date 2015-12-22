# makefile
# Erstellt von IBM WorkFrame/2 MakeMake um 12:18:28 am 2 Mai 1998
#
# Diese Make-Datei enth„lt folgende Aktionen:
#  Compile::C++ Compiler
#  Link::Linker

.SUFFIXES:

.SUFFIXES: \
    .c .obj

# /M<p|s|c|t>       /Mp: Set the default calling convention to _Optlink.
#                   Default: /Mp
#                   /Ms: Set the default calling convention to _System.
#                   /Mc: Set the default calling convention to __cdecl.
#                   /Mt: Set the default linkage to __stdcall.
# /Gd<[+]|->        /Gd+: Use the version of the runtime library that is
#                         dynamically linked.
#                   /Gd-: Use the version of the runtime library that is
#                         statically linked.  Default: /Gd-
# /Gm<[+]|->        Link with multithread runtime libraries.  Default:  /Gm-
# /Ge<[+]|->        /Ge+: Use the version of the runtime library that assumes an
#                         EXE is being built.
#                   /Ge-: Use the version of the runtime library that assumes a
#                         DLL is being built.  Default: /Ge+
# /R<e|n>           /Re: Produce code to run with a complete runtime
#                        environment.  Default: /Re
#                   /Rn: Produce code to run with no runtime environment.

ICCOPTSRT=/Ss /Q /Mp /Gm+ /Gd- /Ge- /G5 /C
ICCOPTSSS=/Ss /Q /Mp /Rn  /Gd- /Ge- /G5 /C

.c.obj:
    @echo " Compile::C++ Compiler "
    icc.exe  $(CDB) $(ICCOPTSSS) %s
#    icc.exe /Ss $(CDB) /Q /Rn /Mp /Gd- /Ge- /G5 /C %s

{D:\work\c\buttondll}.c.obj:
    @echo " Compile::C++ Compiler "
    icc.exe  $(CDB) $(ICCOPTSSS) %s

all: \
    .\pmctlspp.dll

.\pmctlspp.dll: \
    .\pmctlspp.obj \
    .\BtnDlgProc.obj \
    .\ComboDlgProc.obj \
    .\EntryDlgProc.obj \
    {$(LIB)}pmctlspp.def
    @echo " Link::Linker "
    icc.exe @<<
     /Q /B" $(LDB) /noe"
     /Fepmctlspp.dll $(TDB)
     /Fmpmctlspp.map
     pmctlspp.def
     .\pmctlspp.obj
     .\BtnDlgProc.obj
     .\ComboDlgProc.obj
     .\EntryDlgProc.obj
<<

!include "makefile.DEP"
