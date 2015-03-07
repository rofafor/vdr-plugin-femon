/*
 * log.h: Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __FEMON_LOG_H
#define __FEMON_LOG_H

#include "femonconfig.h"

#define error(x...)   esyslog("FEMON-ERROR: " x)
#define info(x...)    isyslog("FEMON: " x)
// 0x0001: Generic call stack
#define debug1(x...)  void( FemonConfig.IsTraceMode(cFemonConfig::eTraceModeDebug1)  ? dsyslog("FEMON1: " x)  : void() )
// 0x0002: H.264
#define debug2(x...)  void( FemonConfig.IsTraceMode(cFemonConfig::eTraceModeDebug2)  ? dsyslog("FEMON2: " x)  : void() )
// 0x0004: TBD
#define debug3(x...)  void( FemonConfig.IsTraceMode(cFemonConfig::eTraceModeDebug3)  ? dsyslog("FEMON3: " x)  : void() )
// 0x0008: TBD
#define debug4(x...)  void( FemonConfig.IsTraceMode(cFemonConfig::eTraceModeDebug4)  ? dsyslog("FEMON4: " x)  : void() )
// 0x0010: TBD
#define debug5(x...)  void( FemonConfig.IsTraceMode(cFemonConfig::eTraceModeDebug5)  ? dsyslog("FEMON5: " x)  : void() )
// 0x0020: TBD
#define debug6(x...)  void( FemonConfig.IsTraceMode(cFemonConfig::eTraceModeDebug6)  ? dsyslog("FEMON6: " x)  : void() )
// 0x0040: TBD
#define debug7(x...)  void( FemonConfig.IsTraceMode(cFemonConfig::eTraceModeDebug7)  ? dsyslog("FEMON7: " x)  : void() )
// 0x0080: TBD
#define debug8(x...)  void( FemonConfig.IsTraceMode(cFemonConfig::eTraceModeDebug8)  ? dsyslog("FEMON8: " x)  : void() )
// 0x0100: TBD
#define debug9(x...)  void( FemonConfig.IsTraceMode(cFemonConfig::eTraceModeDebug9)  ? dsyslog("FEMON9: " x)  : void() )
// 0x0200: TBD
#define debug10(x...) void( FemonConfig.IsTraceMode(cFemonConfig::eTraceModeDebug10) ? dsyslog("FEMON10: " x) : void() )
// 0x0400: TBD
#define debug11(x...) void( FemonConfig.IsTraceMode(cFemonConfig::eTraceModeDebug11) ? dsyslog("FEMON11: " x) : void() )
// 0x0800: TBD
#define debug12(x...) void( FemonConfig.IsTraceMode(cFemonConfig::eTraceModeDebug12) ? dsyslog("FEMON12: " x) : void() )
// 0x1000: TBD
#define debug13(x...) void( FemonConfig.IsTraceMode(cFemonConfig::eTraceModeDebug13) ? dsyslog("FEMON13: " x) : void() )
// 0x2000: TBD
#define debug14(x...) void( FemonConfig.IsTraceMode(cFemonConfig::eTraceModeDebug14) ? dsyslog("FEMON14: " x) : void() )
// 0x4000: TBD
#define debug15(x...) void( FemonConfig.IsTraceMode(cFemonConfig::eTraceModeDebug15) ? dsyslog("FEMON15: " x) : void() )
// 0x8000; Extra call stack
#define debug16(x...) void( FemonConfig.IsTraceMode(cFemonConfig::eTraceModeDebug16) ? dsyslog("FEMON16: " x) : void() )

#endif // __FEMON_LOG_H
