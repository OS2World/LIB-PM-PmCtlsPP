#define INCL_WIN
#define INCL_DOS
#define INCL_ERRORS
#include <os2.h>
#include <string.h>
#include "PmCtlsTest.h"

/* Public API from pmctlspp.dll */
ULONG _System _PM_Init(VOID);

HAB  hab;

static MRESULT EXPENTRY ClientWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    switch (msg)
    {
    case WM_CREATE:
        {
            PSZ  pszEnv  = NULL;
            PCSZ pszStatus = "(not set)";

            if (DosScanEnv("PMCTLSPP", &pszEnv) == NO_ERROR && pszEnv && *pszEnv)
                pszStatus = pszEnv;

            /* status line */
            WinCreateWindow(hwnd, WC_STATIC, "PMCTLSPP=",
                            SS_TEXT | WS_VISIBLE,
                            10, 230, 90, 20,
                            hwnd, HWND_TOP, (ULONG)-1, NULL, NULL);
            WinCreateWindow(hwnd, WC_STATIC, pszStatus,
                            SS_TEXT | WS_VISIBLE,
                            105, 230, 440, 20,
                            hwnd, HWND_TOP, IDC_STATUS, NULL, NULL);

            /* entry field */
            WinCreateWindow(hwnd, WC_STATIC,
                            "Entry field  (double-click selects one word):",
                            SS_TEXT | WS_VISIBLE,
                            10, 200, 450, 20,
                            hwnd, HWND_TOP, (ULONG)-1, NULL, NULL);
            WinCreateWindow(hwnd, WC_ENTRYFIELD,
                            "The quick brown fox jumps over the lazy dog",
                            ES_AUTOSCROLL | WS_VISIBLE | WS_TABSTOP,
                            10, 175, 550, 24,
                            hwnd, HWND_TOP, IDC_EF, NULL, NULL);

            /* combo box */
            WinCreateWindow(hwnd, WC_STATIC,
                            "Combo box  (click field to toggle list):",
                            SS_TEXT | WS_VISIBLE,
                            10, 145, 400, 20,
                            hwnd, HWND_TOP, (ULONG)-1, NULL, NULL);
            {
                HWND hwndCB = WinCreateWindow(hwnd, WC_COMBOBOX, "",
                                CBS_DROPDOWN | WS_VISIBLE | WS_TABSTOP,
                                10, 40, 300, 105,
                                hwnd, HWND_TOP, IDC_CB, NULL, NULL);
                WinSendMsg(hwndCB, LM_INSERTITEM, MPFROMSHORT(LIT_END), MPFROMP("Alpha"));
                WinSendMsg(hwndCB, LM_INSERTITEM, MPFROMSHORT(LIT_END), MPFROMP("Beta"));
                WinSendMsg(hwndCB, LM_INSERTITEM, MPFROMSHORT(LIT_END), MPFROMP("Gamma"));
                WinSendMsg(hwndCB, LM_INSERTITEM, MPFROMSHORT(LIT_END), MPFROMP("Delta"));
                WinSendMsg(hwndCB, LM_SELECTITEM, MPFROMSHORT(0), MPFROMSHORT(TRUE));
            }

            /* button */
            WinCreateWindow(hwnd, WC_BUTTON, "Click Me",
                            BS_PUSHBUTTON | WS_VISIBLE | WS_TABSTOP,
                            10, 10, 100, 24,
                            hwnd, HWND_TOP, IDC_TESTBTN, NULL, NULL);
        }
        return (MRESULT)0;

    case WM_COMMAND:
        if (SHORT1FROMMP(mp1) == IDC_TESTBTN)
            WinMessageBox(HWND_DESKTOP, hwnd,
                          "Push button clicked.\n\n"
                          "Set PMCTLSPP=EXTPUSHBUTTON in CONFIG.SYS\n"
                          "to activate the enhanced button class.",
                          "PmCtlsPP Test", 0, MB_OK | MB_INFORMATION);
        return (MRESULT)0;

    case WM_ERASEBACKGROUND:
        return (MRESULT)TRUE;

    case WM_CLOSE:
        WinPostMsg(hwnd, WM_QUIT, 0L, 0L);
        return (MRESULT)0;

    default:
        return WinDefWindowProc(hwnd, msg, mp1, mp2);
    }
    return (MRESULT)0;
}

int main(void)
{
    HMQ  hmq;
    HWND hwndFrame, hwndClient;
    QMSG qmsg;
    ULONG flCreate;

    hab = WinInitialize(0);
    if (hab == NULLHANDLE)
        return 1;

    hmq = WinCreateMsgQueue(hab, 0);
    if (hmq == NULLHANDLE)
    {
        WinTerminate(hab);
        return 1;
    }

    _PM_Init();

    WinRegisterClass(hab, (PCSZ)"PmCtlsTest", ClientWndProc, CS_SIZEREDRAW, 0);

    flCreate = FCF_TITLEBAR | FCF_SYSMENU | FCF_SIZEBORDER | FCF_MINMAX | FCF_SHELLPOSITION;
    hwndFrame = WinCreateStdWindow(HWND_DESKTOP, 0L,
                                   &flCreate,
                                   (PCSZ)"PmCtlsTest",
                                   (PCSZ)"PmCtlsPP Test",
                                   0L, NULLHANDLE, 0L, &hwndClient);

    if (hwndFrame != NULLHANDLE)
    {
        WinSetWindowPos(hwndFrame, HWND_TOP,
                        100, 100, 580, 310,
                        SWP_SIZE | SWP_MOVE | SWP_SHOW | SWP_ACTIVATE);

        while (WinGetMsg(hab, &qmsg, NULLHANDLE, 0, 0))
            WinDispatchMsg(hab, &qmsg);

        WinDestroyWindow(hwndFrame);
    }

    WinDestroyMsgQueue(hmq);
    WinTerminate(hab);
    return 0;
}
