/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __FEMONCONFIG_H
#define __FEMONCONFIG_H

#define MaxSvdrpIp 15 // xxx.xxx.xxx.xxx

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
  int skin;
  int theme;
  int position;
  int downscale;
  int redlimit;
  int greenlimit;
  int updateinterval;
  int analyzestream;
  int calcinterval;
  int usesvdrp;
  int svdrpport;
  char svdrpip[MaxSvdrpIp + 1]; // must end with additional null
};

extern cFemonConfig FemonConfig;

enum eFemonSkins
{
  eFemonSkinClassic,
  eFemonSkinElchi,
  eFemonSkinMaxNumber
};

enum eFemonThemes
{
  eFemonThemeClassic,
  eFemonThemeElchi,
  eFemonThemeSTTNG,
  eFemonThemeDeepBlue,
  eFemonThemeMoronimo,
  eFemonThemeEnigma,
  eFemonThemeEgalsTry,
  eFemonThemeDuotone,
  eFemonThemeSilverGreen,
  eFemonThemePearlHD,
  eFemonThemeMaxNumber
};

struct cFemonTheme
{
  int bpp;
  unsigned int clrBackground;
  unsigned int clrTitleBackground;
  unsigned int clrTitleText;
  unsigned int clrActiveText;
  unsigned int clrInactiveText;
  unsigned int clrRed;
  unsigned int clrYellow;
  unsigned int clrGreen;
};

extern const cFemonTheme FemonTheme[eFemonThemeMaxNumber];

#endif // __FEMONCONFIG_H
