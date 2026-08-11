# LIB-PM-PmCtlsPP

PmCtlsPP is an OS/2 Presentation Manager DLL that enhances standard PM controls
with behaviour familiar from other platforms: double-clicking an entry field
selects the word under the cursor, clicking inside a combo box entry field
toggles the drop-down list, and push buttons can use an extended drawing
procedure.  Activation is opt-in per class via an environment variable, so
existing applications are unaffected unless they call `_PM_Init()`.

## Version

1.1

## Features

| Enhancement | Env keyword | Control class replaced |
|---|---|---|
| Entry field double-click selects one word | `EXTENTRYFIELD` | `WC_ENTRYFIELD` |
| Combo box click on field toggles list | `EXTCOMBOBOX` | `WC_COMBOBOX` |
| Extended push button drawing | `EXTPUSHBUTTON` | `WC_BUTTON` |

## Activation

Add the desired keywords to `CONFIG.SYS`:

```
SET PMCTLSPP=EXTENTRYFIELD,EXTCOMBOBOX,EXTPUSHBUTTON
```

Then call `_PM_Init()` early in the application's startup, before creating any
windows.  Only the classes listed in `PMCTLSPP` are subclassed; keywords not
present leave the corresponding control unchanged.

## API

```c
ULONG _System _PM_Init(VOID);
```

Returns 1 on success.  Reads `PMCTLSPP`, subclasses the listed window classes,
and returns.  Call once per process, after `WinInitialize` and
`WinCreateMsgQueue`.

The function is exported from `pmctlspp.dll` at ordinal 1.  Link against
`lib\pmctlspp.lib` and declare:

```c
ULONG _System _PM_Init(VOID);
```

## File layout

```
src\              Source files
  pmctlspp.c      DLL entry point and class registration
  BtnDlgProc.c    Enhanced WC_BUTTON window procedure
  EntryDlgProc.c  Enhanced WC_ENTRYFIELD window procedure
  ComboDlgProc.c  Enhanced WC_COMBOBOX window procedure
  PmCtlsPP.h      Public header
  Log.h           Debug stub header (no-op)
  pmctlspp.def    Linker export definition
dll\              Build output: pmctlspp.dll
lib\              Build output: pmctlspp.lib (import library)
doc\              Documentation
sample\
  PmCtlsTest\     Sample PM application demonstrating the library
```

## Building

Requires OpenWatcom 2.0.1 on OS/2 or ArcaOS.

```
wmake -f makefile.wat
```

Outputs: `dll\pmctlspp.dll` and `lib\pmctlspp.lib`.

To build the sample:

```
cd sample\PmCtlsTest
wmake -f makefile.wat
```

The sample makefile copies `pmctlspp.dll` into the sample directory automatically.

## Running the sample

```
SET PMCTLSPP=EXTENTRYFIELD,EXTCOMBOBOX,EXTPUSHBUTTON
PmCtlsTest.exe
```

Without the environment variable the controls behave as standard PM controls.

## License

BSD 3-Clause — see `doc\License.txt`.

## Authors

- Noller & Breining Software (original author, 2001)
- OpenWatcom 2.0.1 port and sample — 2025

## Links

- http://www.os2world.com/wiki/index.php/PmCtlsPP
