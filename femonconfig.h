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

class cFemonConfig
{
private:
  unsigned int traceModeM;
  int hideMenuM;
  int displayModeM;
  int skinM;
  int themeM;
  int positionM;
  int downscaleM;
  int redLimitM;
  int greenLimitM;
  int updateIntervalM;
  int analyzeStreamM;
  int calcIntervalM;
  int useSvdrpM;
  int svdrpPortM;
  char svdrpIpM[MaxSvdrpIp + 1]; // must end with additional null

public:
  enum eTraceMode {
    eTraceModeNormal  = 0x0000,
    eTraceModeDebug1  = 0x0001,
    eTraceModeDebug2  = 0x0002,
    eTraceModeDebug3  = 0x0004,
    eTraceModeDebug4  = 0x0008,
    eTraceModeDebug5  = 0x0010,
    eTraceModeDebug6  = 0x0020,
    eTraceModeDebug7  = 0x0040,
    eTraceModeDebug8  = 0x0080,
    eTraceModeDebug9  = 0x0100,
    eTraceModeDebug10 = 0x0200,
    eTraceModeDebug11 = 0x0400,
    eTraceModeDebug12 = 0x0800,
    eTraceModeDebug13 = 0x1000,
    eTraceModeDebug14 = 0x2000,
    eTraceModeDebug15 = 0x4000,
    eTraceModeDebug16 = 0x8000,
    eTraceModeMask    = 0xFFFF
  };
  cFemonConfig();
  unsigned int GetTraceMode(void) const { return traceModeM; }
  bool IsTraceMode(eTraceMode modeP) const { return (traceModeM & modeP); }
  int GetHideMenu(void) const { return hideMenuM; }
  int GetDisplayMode(void) const { return displayModeM; }
  int GetSkin(void) const { return skinM; }
  int GetTheme(void) const { return themeM; }
  int GetPosition(void) const { return positionM; }
  int GetDownscale(void) const { return downscaleM; }
  int GetRedLimit(void) const { return redLimitM; }
  int GetGreenLimit(void) const { return greenLimitM; }
  int GetUpdateInterval(void) const { return updateIntervalM; }
  int GetAnalyzeStream(void) const { return analyzeStreamM; }
  int GetCalcInterval(void) const { return calcIntervalM; }
  int GetUseSvdrp(void) const { return useSvdrpM; }
  int GetSvdrpPort(void) const { return svdrpPortM; }
  const char *GetSvdrpIp(void) const { return svdrpIpM; }

  void SetTraceMode(unsigned int modeP) { traceModeM = (modeP & eTraceModeMask); }
  void SetHideMenu(int hideMenuP) { hideMenuM = hideMenuP; }
  void SetDisplayMode(int displayModeP) { if (displayModeM < 0 || displayModeM >= eFemonModeMaxNumber) displayModeM = 0; else displayModeM = displayModeP; }
  void SetSkin(int skinP) { skinM = skinP; }
  void SetTheme(int themeP) { themeM = themeP; }
  void SetPosition(int positionP) { positionM = positionP; }
  void SetDownscale(int downscaleP) { downscaleM = downscaleP; }
  void SetRedLimit(int redLimitP) { redLimitM = redLimitP; }
  void SetGreenLimit(int greenLimitP) { greenLimitM = greenLimitP; }
  void SetUpdateInterval(int updateIntervalP) { updateIntervalM = updateIntervalP; }
  void SetAnalyzeStream(int analyzeStreamP) { analyzeStreamM = analyzeStreamP; }
  void SetCalcInterval(int calcIntervalP) { calcIntervalM = calcIntervalP; }
  void SetUseSvdrp(int useSvdrpP) { useSvdrpM = useSvdrpP; }
  void SetSvdrpPort(int svdrpPortP) { svdrpPortM = svdrpPortP; }
  void SetSvdrpIp(const char *strP);
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
