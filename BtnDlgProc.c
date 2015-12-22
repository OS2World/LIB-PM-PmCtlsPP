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
 *      Button Window Procedure                                                *
\*******************************************************************************/
VOID BtnCreate (HWND hwnd)
    {
    HAB     hab;
    RECTL   rectl;
    CHAR    szText[64];
    ULONG   ulStyle;
    USHORT  usID;

    DebugUL ("BtnCreate", "WM_CREATE, hModule = ", hModule);

    hab  = WinQueryAnchorBlock (hwnd);

    WinQueryWindowRect (hwnd, &rectl);
    WinQueryWindowText (hwnd, 64, szText);
    DebugE ("BtnCreate => window text: ", szText);
    ulStyle = WinQueryWindowULong (hwnd, QWL_STYLE);
    ulStyle = (ulStyle & ~BS_PRIMARYSTYLES) | BS_USERBUTTON | WS_VISIBLE;
    usID    = WinQueryWindowUShort (hwnd, QWS_ID);

    Debug2UL ("BtnCreate", "style=", ulStyle, "id=", (ULONG)usID);
    WinCreateWindow (hwnd,
                     WC_BUTTON,
                     szText,                    // Window text
                     ulStyle,                   // Window style
                     0, 0,
                     rectl.xRight - rectl.xLeft, rectl.yTop - rectl.yBottom,
                     hwnd,
                     HWND_TOP,
                     usID,
                     NULL,
                     NULL);

    DebugE ("BtnCreate", "Create Window done");
    return;
    }

void BtnCommand (HWND hwnd, MPARAM mp2)
    {
    HWND   hwndOwner = WinQueryWindow (hwnd, QW_OWNER);
    USHORT usID      = WinQueryWindowUShort (hwnd, QWS_ID);

    DebugE ("BtnCommand", "WM_COMMAND");
    WinPostMsg (hwndOwner, WM_COMMAND, MPFROMSHORT (usID), mp2);

    return;
    }

VOID BtnControl (HWND hwnd, MPARAM mp1, MPARAM mp2)
    {
    HPS     hPS;
    RECTL   rectl;
    HAB     hab;
    HBITMAP hbm;
    POINTL  aptl[4];
    BITMAPINFOHEADER2 bmpInfo;

    DebugE ("BtnControl", "WM_CONTROL");
    if (SHORT2FROMMP (mp1) == BN_PAINT)
        {
        USERBUTTON *pUsrBtn;

        pUsrBtn = (USERBUTTON *)PVOIDFROMMP (mp2);
        hPS = pUsrBtn->hps;
        DebugUL ("WM_CONTROL", "state", pUsrBtn->fsState);

        hbm = GpiLoadBitmap (hPS, hModule, 1001, 0, 0);
        WinQueryWindowRect (pUsrBtn->hwnd, &rectl);
        bmpInfo.cbFix = sizeof (BITMAPINFOHEADER2);
        GpiQueryBitmapInfoHeader (hbm, &bmpInfo);

        if (pUsrBtn->fsState & BDS_HILITED)
            {
            aptl[0].x = rectl.xLeft;
            aptl[0].y = rectl.yBottom;
            aptl[1].x = rectl.xRight-4;
            aptl[1].y = rectl.yTop-4;
            }
        else
            {
            aptl[0].x = rectl.xLeft+4;
            aptl[0].y = rectl.yBottom+4;
            aptl[1].x = rectl.xRight;
            aptl[1].y = rectl.yTop;
            }
        aptl[2].x = 0;
        aptl[2].y = 0;
        aptl[3].x = bmpInfo.cx;
        aptl[3].y = bmpInfo.cy;

        GpiWCBitBlt (hPS, hbm, 4, aptl, ROP_SRCCOPY, BBO_IGNORE);
        GpiDeleteBitmap (hbm);
        }

    return;
    }

BOOL BtnPaint (HWND hwnd)
    {
    HPS     hPS;
    RECTL   rectl;
    HAB     hab;
    HLIB    hLib;
    HBITMAP hbm;
    POINTL  aptl[4];
    BITMAPINFOHEADER2 bmpInfo;

    hPS = WinBeginPaint (hwnd, NULLHANDLE, NULL);
    DebugUL ("BtnPaint", "hPS = ", hPS);

    hbm = GpiLoadBitmap (hPS, hModule, 1001, 0, 0);
    DebugUL ("BtnPaint", "hbm = ", (ULONG)hbm);
    WinQueryWindowRect (hwnd, &rectl);
    bmpInfo.cbFix = sizeof (BITMAPINFOHEADER2);
    GpiQueryBitmapInfoHeader (hbm, &bmpInfo);

//    if (pUsrBtn->fsState & BDS_HILITED)
//        {
        aptl[0].x = rectl.xLeft;
        aptl[0].y = rectl.yBottom;
        aptl[1].x = rectl.xRight-4;
        aptl[1].y = rectl.yTop-4;
//        }
//    else
//        {
//        aptl[0].x = rectl.xLeft+4;
//        aptl[0].y = rectl.yBottom+4;
//        aptl[1].x = rectl.xRight;
//        aptl[1].y = rectl.yTop;
//        }
    aptl[2].x = 0;
    aptl[2].y = 0;
    aptl[3].x = bmpInfo.cx;
    aptl[3].y = bmpInfo.cy;

    GpiWCBitBlt (hPS, hbm, 4, aptl, ROP_SRCCOPY, BBO_IGNORE);
    GpiDeleteBitmap (hbm);

    return TRUE;
    }


MRESULT EXPENTRY ButtonDialogProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    ULONG ulStyle;

    ulStyle = WinQueryWindowULong (hwnd, QWL_STYLE);

    if ((ulStyle & BS_PRIMARYSTYLES) == BS_PUSHBUTTON &&
        (ulStyle & (BS_TEXT | BS_MINIICON | BS_BITMAP | BS_ICON)) == 0)
        {
        switch (msg)
            {
            case WM_CREATE:
                BtnCreate (hwnd);
                break;

            case WM_COMMAND:
                BtnCommand (hwnd, mp2);
                return 0;

            case WM_CONTROL:
                BtnControl (hwnd, mp1, mp2);
                break;

            case WM_PAINT:
                break;
                Debug2UL ("WM_PAINT", "mp1:", (ULONG)mp1, " mp2:", (ULONG)mp2);
                if (BtnPaint (hwnd))
                    return 0;
                break;
            }
        }

    return (MRESULT)pfnwpOldButtonWndProc (hwnd, msg, mp1, mp2);
    }

