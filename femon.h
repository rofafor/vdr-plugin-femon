/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#ifndef __FEMON_H
#define __FEMON_H

#include <vdr/plugin.h>

static const char *VERSION        = "0.8.6";
static const char *DESCRIPTION    = "DVB Signal Information Monitor (OSD)";
static const char *MAINMENUENTRY  = "Signal Information";

class cPluginFemon : public cPlugin {
private:
  // Add any member variables or functions you may need here.
public:
  cPluginFemon(void);
  virtual ~cPluginFemon();
  virtual const char *Version(void) { return VERSION; }
  virtual const char *Description(void) { return tr(DESCRIPTION); }
  virtual const char *CommandLineHelp(void);
  virtual bool ProcessArgs(int argc, char *argv[]);
  virtual bool Initialize(void);
  virtual bool Start(void);
  virtual void Stop(void);
  virtual void Housekeeping(void);
  virtual const char *MainMenuEntry(void) { return (femonConfig.hidemenu ? NULL : tr(MAINMENUENTRY)); }
  virtual cOsdObject *MainMenuAction(void);
  virtual cMenuSetupPage *SetupMenu(void);
  virtual bool SetupParse(const char *Name, const char *Value);
  };

class cMenuFemonSetup : public cMenuSetupPage {
  private:
    const char *dispmodes[eFemonModeMaxNumber];
    const char *themes[eFemonThemeMaxNumber];
    virtual void Setup(void);
  protected:
    virtual eOSState ProcessKey(eKeys Key);
    virtual void Store(void);
  public:
    cMenuFemonSetup(void);
  };

#endif //__FEMON_H

