/*
 * A Frontend Monitor plugin for the Video Disk Recorder                                             
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#include "femoncfg.h"

cFemonConfig femonConfig;

cFemonConfig::cFemonConfig(void)
{
  hidemenu   = 0;
  position   = 1;
  interval   = 5;
  redlimit   = 33;
  greenlimit = 66;
}
