/***************************************************************************
 *
 * Log.h - Minimal debug logging header for OpenWatcom compatibility
 * Provides stubs for Debug and Trace functions used in PMCTLSPP
 *
 ******************************************************************************/
#ifndef LOG_H
#define LOG_H

#include <os2.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Trace initialization - no-op */
#define TraceInit(filename)

/* Debug output functions - no-op */
#define DebugE(tag, msg)
#define DebugUL(tag, msg, val)
#define Debug2UL(tag, msg1, val1, msg2, val2)

/* If you want to enable debugging, uncomment the following and link with a debug library */
/*
VOID _System TraceInit(PSZ pszFilename);
VOID _System DebugE(PSZ pszTag, PSZ pszMsg);
VOID _System DebugUL(PSZ pszTag, PSZ pszMsg, ULONG ulVal);
VOID _System Debug2UL(PSZ pszTag, PSZ pszMsg1, ULONG ulVal1, PSZ pszMsg2, ULONG ulVal2);
*/

#ifdef __cplusplus
}
#endif

#endif /* LOG_H */
