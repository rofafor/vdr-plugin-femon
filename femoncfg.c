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
}
