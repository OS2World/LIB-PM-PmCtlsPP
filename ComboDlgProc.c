/***************************************************************************
 *
 * PROGRAM NAME: PMCTLSPP
 * -------------
 *
 * MODULE NAME:  COMBODLGPROC
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
 *      InputHookProc
 *      ComboBoxCreate
 *      ComboBoxDialogProc
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

/*******************************************************************\
  local data segment (process specific)
\*******************************************************************/

/*******************************************************************\
  global data segment
\*******************************************************************/
#pragma data_seg(GLOBAL_SEG)

ULONG ulcWindowData;

BOOL APIENTRY ComboBoxRegisterClass (HAB hab, PSZ pszClassName, PFNWP pfnWndProc, ULONG flStyle, ULONG cbWindowData)
    {
    ulcWindowData = cbWindowData + sizeof (ULONG);
    return WinRegisterClass (hab, pszClassName, pfnWndProc, flStyle, ulcWindowData);
    }

/*******************************************************************************\
 *      Combobox Window Procedure                                              *
\*******************************************************************************/
VOID EntryFieldInComboBoxButton1Click (HWND hwnd)
    {
    HWND hwndParent;
    BOOL bShowState;

    hwndParent = (HWND)WinQueryWindow (hwnd, QW_PARENT);
    bShowState = (BOOL)WinSendMsg (hwndParent, CBM_ISLISTSHOWING, MPVOID, MPVOID);
    DebugUL ("EntryFieldInComboBoxButton1Click", "isListShowing = ", bShowState);
    WinSendMsg (hwndParent, CBM_SHOWLIST, MPFROMSHORT (!bShowState), MPVOID);
    WinSendMsg (hwndParent, CBM_HILITE,   MPFROMSHORT (!bShowState), MPVOID);

    return;
    }

MRESULT EXPENTRY EntryFieldInComboBoxDialogProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    HWND  hwndOwner;
    PFNWP pfnwp;

    switch (msg)
        {
        case WM_BUTTON1CLICK:
            EntryFieldInComboBoxButton1Click (hwnd);
            break;
        }

    hwndOwner = WinQueryWindow (hwnd, QW_OWNER);
    pfnwp = (PFNWP)WinQueryWindowULong (hwndOwner, ulcWindowData - sizeof (ULONG));
    return (MRESULT)pfnwp (hwnd, msg, mp1, mp2);
    }

/*******************************************************************************\
 *      Combobox Window Procedure                                              *
\*******************************************************************************/
VOID ComboBoxAdjustWindow (HWND hwnd)
    {
    CHAR  szBuffer[16];
    HWND  hwndTop;
    PFNWP pfnwp;

    // already registered?
    if (WinQueryWindowULong (hwnd, ulcWindowData - sizeof (ULONG)))
        return;

    // The window handle of the EntryField in the ComboBox has to be determined.
    // First the Top-Child will be determined (should be EntryField). Then for
    // security reason its classname is checked.
    hwndTop = (HWND)WinQueryWindow (hwnd, QW_TOP);

    WinQueryClassName (hwndTop, 16, szBuffer);
    if (szBuffer[0] != '#')
        return;
    if (atoi (&szBuffer[1]) != ((ULONG)WC_ENTRYFIELD & 0xFFFF))
        return;

    DebugUL ("ComboBoxWindowPosChanged", "hwnd of entry field = ", hwndTop);

    // If the EntryField has not yet been subclassed, do it now
    pfnwp = WinSubclassWindow (hwndTop, (PFNWP)EntryFieldInComboBoxDialogProc);
    Debug2UL ("ComboBoxWindowPosChanged",
              "old EF-WndProc = ",  (ULONG)pfnwp,
              " new EF-WndProc = ", (ULONG)(PFNWP)EntryFieldInComboBoxDialogProc);

    WinSetWindowULong (hwnd, ulcWindowData - sizeof (ULONG), (ULONG)pfnwp);

    return;
    }

MRESULT EXPENTRY ComboBoxDialogProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    switch (msg)
        {
        case WM_CREATE:
            WinSetWindowULong (hwnd, ulcWindowData - sizeof (ULONG), 0);
            break;

        // This is a message that is reliably arriving after the creation of all
        // ComboBox child windows (CS_ENTRYFIELD and CS_LISTBOX)
        case WM_ADJUSTWINDOWPOS:
            ComboBoxAdjustWindow (hwnd);
            break;
        }

    return (MRESULT)pfnwpOldComboBoxWndProc (hwnd, msg, mp1, mp2);
    }

