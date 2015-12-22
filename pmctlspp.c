/***************************************************************************
 *
 * PROGRAM NAME: PMCTLSPP
 * -------------
 *
 * MODULE NAME:  PMCTLSPP
 * ------------
 *
 * VERSION:      1.0
 * --------
 *
 * DESCRIPTION:
 * ------------
 *      Initialisation module
 *
 * FUNCTIONS:
 * ----------
 *      LoadModule
 *      getClassIndex
 *      CheckEnvironment
 *      _DLL_InitTerm
 *      _PM_Init
 *      RegisterPublicWindowClasses
 *
 * COMPILER:
 * ---------
 *      IBM Visual Age C++ 3.0
 *
 * LIBRARIES:
 * ----------
 *      Subsystem libraries (static)
 *
 * CHANGE LOG:
 * -----------
 *  Ver.    Date      Comment
 *  ----    --------  -------
 *  1.00    30.09.01  First Release
 *
 *  Copyright (C) 2001 noller & breining software
 *
 ******************************************************************************/
#define  INCL_DOS
#define  INCL_WIN
#define  INCL_GPI
#define  INCL_ERRORS
#include <os2.h>

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include <Log.h>
#include <PmCtlsPP.h>

#define ENVIRONMENTNAME     "PMCTLSPP"

/*******************************************************************\
  type definitions an function prototypes
\*******************************************************************/
int _rmem_init (void);
int _rmem_term (void);
unsigned long _System _DLL_InitTerm (unsigned long hModule, unsigned long ulFlag);

ULONG RegisterPublicWindowClasses (VOID);
VOID LoadModule (VOID);

/*******************************************************************\
  local data segment (process specific)
\*******************************************************************/
HMODULE hModule = NULLHANDLE;

/*******************************************************************\
  global data segment
\*******************************************************************/
#pragma data_seg(GLOBAL_SEG)

BOOL  bIsRegistered = FALSE;

PFNWP pfnwpOldButtonWndProc               = NULL;
PFNWP pfnwpOldEntryFieldWndProc           = NULL;
PFNWP pfnwpOldComboBoxWndProc             = NULL;

STRUCTCLASS arClasses[] =
    {
        {WC_BUTTON,     "PushButton", "EXTPUSHBUTTON", FALSE, NULL,                       ButtonDialogProc,     &pfnwpOldButtonWndProc},
        {WC_ENTRYFIELD, "EntryField", "EXTENTRYFIELD", FALSE, NULL,                       EntryFieldDialogProc, &pfnwpOldEntryFieldWndProc},
        {WC_COMBOBOX,   "ComboBox",   "EXTCOMBOBOX",   FALSE, (PFN)ComboBoxRegisterClass, ComboBoxDialogProc,   &pfnwpOldComboBoxWndProc},
        {NULL,          NULL,         NULL,            FALSE, NULL,                       NULL,                 NULL}
    };

/*******************************************************************************\
 *      Load Resource Module                                                   *
\*******************************************************************************/
VOID LoadModule (VOID)
    {
    CHAR buffer[64];

    DebugUL ("LoadModule", "entered; current value: ", hModule);
    if (hModule == NULLHANDLE)
        {
        if (DosQueryModuleHandle ("btnres.dll", &hModule))
            {
            DebugE ("LoadModule", "Could not query module; try to load");
            if (DosLoadModule (buffer, 64, "btnres.dll", &hModule))
                DebugE ("LoadModule => DLL Load Error: ", buffer);
            }
        }

    DebugUL ("LoadModule", "hModule = ", hModule);
    return;
    }

LONG getClassIndex (PSZ pszClassName)
    {
    LONG lc;

    for (lc = 0; arClasses[lc].pszClassName != NULL; lc++)
        {
        if (arClasses[lc].pszClassName == pszClassName)
            break;
        }

    if (arClasses[lc].pszClassName == NULL)
        lc = -1;

    return lc;
    }

BOOL CheckEnvironment (VOID)
    {
    PSZ  pszValue = NULL;
    PSZ  pc, pc1, pc2;
    LONG lIndex, lSize;
    COUNTRYCODE cc = {0, 0};

    if (DosScanEnv (ENVIRONMENTNAME, &pszValue) != NO_ERROR)
        return FALSE;

    lSize = strlen (pszValue) + 1;
    if (DosAllocMem ((PPVOID)&pc, lSize, PAG_COMMIT | PAG_READ | PAG_WRITE) != NO_ERROR)
        return FALSE;

    strcpy (pc, pszValue);
    DosMapCase (lSize, &cc, pc);
    for (pc1 = pc2 = pc; lSize > 0; pc2++, lSize--)
        {
        if ((*pc2 == ',') || (*pc2 == ';'))
            {
            *pc2 = '\0';
            }
        if (*pc2 == '\0')
            {
            for (lIndex = 0; arClasses[lIndex].pszClassName != NULL; lIndex++)
                if (strcmp (pc1, arClasses[lIndex].pszEnvName) == 0)
                    {
                    arClasses[lIndex].bRegister = TRUE;
                    break;
                    }
            pc1 = pc2 + 1;
            }
        }

    DosFreeMem ((PVOID)pc);
    return TRUE;
    }

/*******************************************************************\
    _DLL_InitTerm is the function that gets called by the operating
    system loader when it loads and frees this DLL for each process
    that accesses this DLL.  However, it only gets called the first
    time the DLL is loaded and the last time it is freed for a
    particular process.  The system linkage convention must be used
    because the operating system loader is calling this function.
    Input:  hModule: module handle of this dll
            ulFlag:  initialization / termination
    Return: 0:       unsuccessul
            other:   successul
\*******************************************************************/
#pragma linkage (_DLL_InitTerm, system)
unsigned long _DLL_InitTerm (unsigned long hModule, unsigned long ulFlag)
    {
    static BOOL bIsInitialized = FALSE;
    size_t      i;

    switch (ulFlag)
        {
        case 0:         // initialization
            /* The C run-time environment initialization function must   */
            /* be called before any calls to C run-time functions that   */
            /* are not inlined.                                          */
            if (!bIsInitialized)
                {
                _rmem_init ();
                bIsInitialized = TRUE;
                }

            TraceInit ("d:\\$$Btn$$.dbg");
            DebugE ("_DLL_InitTerm", "Trace was initialized");

            LoadModule ();

            break;

        case 1:         // termination
            _rmem_term ();
            break;

        default:
            return 0;
        }

    /* A nonzero value must be returned to indicate success. */
    return 1;
    }

ULONG EXPENTRY _PM_Init (VOID)
    {
    DebugE ("_PM_Init", "called");

    if (!bIsRegistered)
        {
        bIsRegistered = TRUE;

        // register all classes in class array
        if (CheckEnvironment ())
            RegisterPublicWindowClasses ();
        }

    return 1;
    }

ULONG RegisterPublicWindowClasses (VOID)
    {
    BOOL      rc = FALSE;
    HAB       hab;
    LONG      lIndex;
    CLASSINFO ci;

    hab = WinQueryAnchorBlock (HWND_DESKTOP);

    for (lIndex = 0; arClasses[lIndex].pszClassName != NULL; lIndex++)
        {
        if (arClasses[lIndex].bRegister == FALSE)
            continue;

        DebugUL ("Register", "class name = ", (ULONG)arClasses[lIndex].pszClassName);
        WinQueryClassInfo (hab, arClasses[lIndex].pszClassName, &ci);
        DebugE ("Register", arClasses[lIndex].pszClassString);
        DebugUL ("Register", "flClassStyle: ", ci.flClassStyle);
        Debug2UL ("Register", "pfnWndProc:   ", ci.pfnWindowProc, "; new: ", arClasses[lIndex].pWndProc);
        DebugUL ("Register", "cbWindowData: ", ci.cbWindowData);

        *(arClasses[lIndex].ppOldWndProc) = ci.pfnWindowProc;

        if (arClasses[lIndex].pPrivRegisterClass)
            rc |= arClasses[lIndex].pPrivRegisterClass (hab,
                                                        arClasses[lIndex].pszClassName,
                                                        arClasses[lIndex].pWndProc,
                                                        ci.flClassStyle,
                                                        ci.cbWindowData);
        else
            rc |= WinRegisterClass (hab,
                                    arClasses[lIndex].pszClassName,
                                    arClasses[lIndex].pWndProc,
                                    ci.flClassStyle,
                                    ci.cbWindowData);

        DebugUL ("Register", "return of WinRegisterClass: ", rc);
        }

    // return 1 if at least 1 class was registered
    return rc ? 1 : 0;
    }

