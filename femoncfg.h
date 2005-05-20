/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#ifndef __FEMONCFG_H
#define __FEMONCFG_H

#ifdef FEMON_DEBUG
#define Dprintf(x...) printf(x);
#else
#define Dprintf(x...) ;
#endif

enum eFemonModes
{
  eFemonModeBasic,
  eFemonModeTransponder,
  eFemonModeStream,
  eFemonModeAC3,
  eFemonModeMaxNumber
};

struct cFemonConfig
{
public:
  cFemonConfig(void);
  int hidemenu;
  int displaymode;
  int theme;
  int position;
  int redlimit;
  int greenlimit;
  int updateinterval;
  int analyzestream;
  int calcinterval;
  int syslogoutput;
  int showcasystem;
  int osdheight;
  int osdoffset;
};

extern cFemonConfig femonConfig;

enum eFemonThemes
{
  eFemonThemeClassic,
  eFemonThemeElchi,
  eFemonThemeDeepBlue,
  eFemonThemeMoronimo,
  eFemonThemeMaxNumber
};

struct cFemonTheme
{
  int clrBackground;
  int clrTitleBackground;
  int clrTitleText;
  int clrActiveText;
  int clrInactiveText;
  int clrRed;
  int clrYellow;
  int clrGreen;
};

extern const cFemonTheme femonTheme[eFemonThemeMaxNumber];

#endif // __FEMONCFG_H
