/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#ifndef __FEMONCFG_H
#define __FEMONCFG_H

struct cFemonConfig
{
public:
  cFemonConfig(void);
  int hidemenu;
  int displaymode;
  int position;
  int redlimit;
  int greenlimit;
  int updateinterval;
  int showbitrate;
  int calcinterval;
  int syslogoutput;
};

extern cFemonConfig femonConfig;

#endif // __FEMONCFG_H
