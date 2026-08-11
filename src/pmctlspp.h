#ifndef PMCTLSPP_H
#define PMCTLSPP_H

/*******************************************************************\
  Compiler compatibility macros
\*******************************************************************/
#if defined(__WATCOMC__)
  #define EXPENTRY    _System
  #define APIENTRY    _System
  #define _Optlink    _Optlink
  #define _System     _System
  #define _cdecl      _cdecl
  #define _stdcall    _stdcall
  #define PASCAL      _System
  #ifndef FAR
  #define FAR         far
  #endif
  #ifndef NEAR
  #define NEAR        near
  #endif
  #define EXPORT      __export
#elif defined(__IBMC__) || defined(__IBMCPP__)
  /* IBM Visual Age C++ - keep existing definitions */
  #ifndef EXPENTRY
  #define EXPENTRY    _System
  #endif
  #ifndef APIENTRY
  #define APIENTRY    _System
  #endif
#else
  /* Generic fallback */
  #ifndef EXPENTRY
  #define EXPENTRY
  #endif
  #ifndef APIENTRY
  #define APIENTRY
  #endif
#endif

/*******************************************************************\
  type definitions an function prototypes
\*******************************************************************/
typedef struct _STRUCTCLASS
    {
    PSZ     pszClassName;       // name of class to be replaced as used in WinRegisterClass
    PSZ     pszClassString;     // name of class in readable form
    PSZ     pszEnvName;         // name to be used for activation in config.sys
    BOOL    bRegister;          // used by parser to identify classes to be instantiated
    PFN     pPrivRegisterClass; // private class registration, if not NULL
    PFNWP   pWndProc;           // new window procedure
    PFNWP  *ppOldWndProc;       // original window procedure
    } STRUCTCLASS, *PSTRUCTCLASS;

MRESULT EXPENTRY ButtonDialogProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY EntryFieldDialogProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY ComboBoxDialogProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

BOOL APIENTRY ComboBoxRegisterClass (HAB hab, PSZ pszClassName, PFNWP pfnWndProc, ULONG flStyle, ULONG cbWindowData);

/*******************************************************************\
  local data segment (process specific)
\*******************************************************************/
extern HMODULE hModule;

/*******************************************************************\
  global data segment
\*******************************************************************/
extern PFNWP pfnwpOldButtonWndProc;
extern PFNWP pfnwpOldEntryFieldWndProc;
extern PFNWP pfnwpOldComboBoxWndProc;

#endif /* PMCTLSPP_H */
