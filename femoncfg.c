/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#include "femoncfg.h"

cFemonConfig femonConfig;

cFemonConfig::cFemonConfig(void)
{
  hidemenu       = 0;
  displaymode    = 0;
  theme          = 0;
  position       = 1;
  redlimit       = 33;
  greenlimit     = 66;
  updateinterval = 5;
  analyzestream  = 1;
  calcinterval   = 20;
  syslogoutput   = 0;
  showcasystem   = 0;
#ifdef NTSC_SYSTEM
  osdheight      = 420;
#else
  osdheight      = 480;
#endif
  osdoffset      = 0;
}

const cFemonTheme femonTheme[eFemonThemeMaxNumber] =
{
  {
    // eFemonThemeClassic
    0x7F000000, // clrBackground
    0xFFFCFCFC, // clrTitleBackground
    0xFF000000, // clrTitleText
    0xFFFCC024, // clrActiveText
    0xFFFCFCFC, // clrInactiveText
    0xFFFC1414, // clrRed
    0xFFFCC024, // clrYellow
    0xFF24FC24, // clrGreen
  },
  {
    // eFemonThemeElchi
    0xC8000066, // clrBackground
    0xC833AAEE, // clrTitleBackground
    0xFF000000, // clrTitleText
    0xFFCCBB22, // clrActiveText
    0xFFFFFFFF, // clrInactiveText
    0xFFFF0000, // clrRed
    0xFFFFEE00, // clrYellow
    0xFF33CC33, // clrGreen
  },
  {
    // eFemonThemeDeepBlue
    0xC80C0C0C, // clrBackground
    0xC832557A, // clrTitleBackground
    0xFF000000, // clrTitleText
    0xFFCE7B00, // clrActiveText
    0xFF9A9A9A, // clrInactiveText
    0xFF992900, // clrRed
    0xFFCE7B00, // clrYellow
    0xFF336600, // clrGreen
  },
};

