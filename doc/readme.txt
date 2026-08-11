Name:     PmCtlsPP
Version:  1.1
Author:   Noller & Breining Software
License:  BSD 3-Clause (see License.txt)
URL:      http://www.os2world.com/wiki/index.php/PmCtlsPP


DESCRIPTION
-----------
PmCtlsPP is an OS/2 Presentation Manager DLL that enhances standard PM
controls with behaviour familiar from other platforms.  Activation is
opt-in per control class via the PMCTLSPP environment variable, so
existing applications are unaffected unless they call _PM_Init().

Enhancements:
  - EXTENTRYFIELD  Double-clicking an entry field selects the word under
                   the cursor instead of the entire contents.
  - EXTCOMBOBOX    Clicking inside the entry field of a combo box toggles
                   the drop-down list open/closed.
  - EXTPUSHBUTTON  Extended push button drawing procedure.


ACTIVATION
----------
Add the desired keywords to CONFIG.SYS:

  SET PMCTLSPP=EXTENTRYFIELD,EXTCOMBOBOX,EXTPUSHBUTTON

Then call _PM_Init() at application startup, after WinInitialize() and
WinCreateMsgQueue(), before any windows are created.  Only the classes
listed in PMCTLSPP are subclassed.


API
---
  ULONG _System _PM_Init(VOID);

  Returns 1 on success.  Link against lib\pmctlspp.lib and declare
  the prototype as shown above.  The function is exported at ordinal 1.


FILE LAYOUT
-----------
  src\              Source files
  dll\              pmctlspp.dll  (built output)
  lib\              pmctlspp.lib  (import library, built output)
  doc\              Documentation
  sample\PmCtlsTest Sample PM application


BUILDING
--------
Requires OpenWatcom 2.0.1 on OS/2 or ArcaOS.

  wmake -f makefile.wat

Outputs: dll\pmctlspp.dll and lib\pmctlspp.lib.

To build the sample:

  cd sample\PmCtlsTest
  wmake -f makefile.wat


VERSION HISTORY
---------------
1.0  2003-06-01  Original release by Noller & Breining Software.
                 Built with IBM Visual Age C++ 3.0.

1.1  2025        OpenWatcom 2.0.1 port.
                 Added sample application (sample\PmCtlsTest).
