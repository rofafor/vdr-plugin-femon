#ifndef __FEMON_H
#define __FEMON_H

#include <vdr/plugin.h>

static const char *VERSION        = "0.0.2c";
static const char *DESCRIPTION    = "DVB Signal Quality Monitor (OSD)";
static const char *MAINMENUENTRY  = "Signal Quality";

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
  virtual void Housekeeping(void);
  virtual const char *MainMenuEntry(void) { return (femonConfig.hidemenu ? NULL : tr(MAINMENUENTRY)); }
  virtual cOsdObject *MainMenuAction(void);
  virtual cMenuSetupPage *SetupMenu(void);
  virtual bool SetupParse(const char *Name, const char *Value);
  };

class cMenuFemonSetup : public cMenuSetupPage {
  public:
    cMenuFemonSetup(void);
  protected:
    virtual void Store(void);
};

#endif //__FEMON_H

