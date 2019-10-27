/*
 * setup.h: Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __FEMON_SETUP_H
#define __FEMON_SETUP_H

class cMenuFemonSetup : public cMenuSetupPage {
private:
  const char *dispModesM[eFemonModeMaxNumber];
  const char *signalUnitsM[eFemonSignalUnitMaxNumber];
  const char *skinsM[eFemonSkinMaxNumber];
  const char *themesM[eFemonThemeMaxNumber];
  cVector<const char*> helpM;
  int hideMenuM;
  int displayModeM;
  int skinM;
  int themeM;
  int positionM;
  int downscaleM;
  int signalUnitM;
  int redLimitM;
  int greenLimitM;
  int updateIntervalM;
  int analyzeStreamM;
  int calcIntervalM;
  int useSvdrpM;
  int svdrpPortM;
  char svdrpIpM[MaxSvdrpIp + 1]; // must end with additional null
  void Setup(void);

protected:
  virtual eOSState ProcessKey(eKeys Key);
  virtual void Store(void);

public:
  cMenuFemonSetup();
};

#endif // __FEMON_SETUP_H
