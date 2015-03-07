/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __FEMONSETUP_H
#define __FEMONSETUP_H

class cMenuFemonSetup : public cMenuSetupPage {
private:
  const char *dispModesM[eFemonModeMaxNumber];
  const char *skinsM[eFemonSkinMaxNumber];
  const char *themesM[eFemonThemeMaxNumber];
  cFemonConfig dataM;
  cVector<const char*> helpM;
  void Setup(void);

protected:
  virtual eOSState ProcessKey(eKeys Key);
  virtual void Store(void);

public:
  cMenuFemonSetup();
};

#endif // __FEMONSETUP_H
