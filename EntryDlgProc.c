/***************************************************************************
 *
 * PROGRAM NAME: PMCTLSPP
 * -------------
 *
 * MODULE NAME:  ENTRYDLGPROC
 * ------------
 *
 * VERSION:      1.0
 * --------
 *
 * DESCRIPTION:
 * ------------
 *      dialog window procedure for extended entry field
 *
 * FUNCTIONS:
 * ----------
 *      EntryFieldButton1DblClk
 *      EntryFieldDialogProc
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

/*******************************************************************************\
 *      Entry Field Window Procedure                                           *
\*******************************************************************************/
BOOL EntryFieldButton1DblClk (HWND hwnd)
    {
    ULONG ulCursor;
    LONG  lTextLength;
    PCHAR pc;
    PCHAR pc1, pc2;

    // Determine cursor position
    ulCursor = (ULONG)WinSendMsg (hwnd, EM_QUERYSEL, MPVOID, MPVOID);
    if ((ulCursor & 0xFFFF) != (ulCursor >> 16))
        return TRUE;

    ulCursor &= 0xFFFF;

    // Get entry field contents
    lTextLength = WinQueryWindowTextLength (hwnd) + 1;
    if (DosAllocMem ((PPVOID)&pc, lTextLength, PAG_COMMIT | PAG_READ | PAG_WRITE) != NO_ERROR)
        return TRUE;

    WinQueryWindowText (hwnd, lTextLength, pc);

    // Determine new selection
    for (pc1 = pc + ulCursor; pc1 > pc; pc1--)
        {
        if (*(pc1-1) == ' ')
            break;
        }

    for (pc2 = pc + ulCursor; pc2 < pc + lTextLength; pc2++)
        {
        if (*pc2 == ' ')
            break;
        }

    // Set selection
    WinSendMsg (hwnd, EM_SETSEL, MPFROM2SHORT ((pc1-pc), (pc2-pc)), MPVOID);
    DosFreeMem (pc);

    return TRUE;
    }

MRESULT EXPENTRY EntryFieldDialogProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    switch (msg)
        {
        case WM_BUTTON1DBLCLK:
            return (MRESULT)EntryFieldButton1DblClk (hwnd);
        }

    return (MRESULT)pfnwpOldEntryFieldWndProc (hwnd, msg, mp1, mp2);
    }

