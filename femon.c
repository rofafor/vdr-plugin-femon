/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#include "femoncfg.h"
#include "femoni18n.h"
#include "femonreceiver.h"
#include "femonosd.h"
#include "femon.h"

#if VDRVERSNUM && VDRVERSNUM < 10321
#error "You don't exist! Go away!"
#endif

cPluginFemon::cPluginFemon(void)
{
  // Initialize any member variables here.
  // DON'T DO ANYTHING ELSE THAT MAY HAVE SIDE EFFECTS, REQUIRE GLOBAL
  // VDR OBJECTS TO EXIST OR PRODUCE ANY OUTPUT!
}

cPluginFemon::~cPluginFemon()
{
  // Clean up after yourself!
}

const char *cPluginFemon::CommandLineHelp(void)
{
  // Return a string that describes all known command line options.
  return NULL;
}

bool cPluginFemon::ProcessArgs(int argc, char *argv[])
{
  // Implement command line argument processing here if applicable.
  return true;
}

bool cPluginFemon::Initialize(void)
{
  // Initialize any background activities the plugin shall perform.
  return true;
}

bool cPluginFemon::Start(void)
{
  // Start any background activities the plugin shall perform.
  RegisterI18n(Phrases);
  return true;
}

void cPluginFemon::Stop(void)
{
  // Stop the background activities.
}

void cPluginFemon::Housekeeping(void)
{
  // Perform any cleanup or other regular tasks.
}

cOsdObject *cPluginFemon::MainMenuAction(void)
{
  // Perform the action when selected from the main VDR menu.
  return new cFemonOsd();
}

bool cPluginFemon::SetupParse(const char *Name, const char *Value)
{
  // Parse your own setup parameters and store their values.
  if      (!strcasecmp(Name, "HideMenu"))       femonConfig.hidemenu       = atoi(Value);
  else if (!strcasecmp(Name, "SyslogOutput"))   femonConfig.syslogoutput   = atoi(Value);
  else if (!strcasecmp(Name, "DisplayMode"))    femonConfig.displaymode    = atoi(Value);
  else if (!strcasecmp(Name, "Position"))       femonConfig.position       = atoi(Value);
  else if (!strcasecmp(Name, "OSDHeight"))      femonConfig.osdheight      = atoi(Value);
  else if (!strcasecmp(Name, "OSDOffset"))      femonConfig.osdoffset      = atoi(Value);
  else if (!strcasecmp(Name, "Theme"))          femonConfig.theme          = atoi(Value);
  else if (!strcasecmp(Name, "ShowCASystem"))   femonConfig.showcasystem   = atoi(Value);
  else if (!strcasecmp(Name, "RedLimit"))       femonConfig.redlimit       = atoi(Value);
  else if (!strcasecmp(Name, "GreenLimit"))     femonConfig.greenlimit     = atoi(Value);
  else if (!strcasecmp(Name, "UpdateInterval")) femonConfig.updateinterval = atoi(Value);
  else if (!strcasecmp(Name, "AnalStream"))     femonConfig.analyzestream  = atoi(Value);
  else if (!strcasecmp(Name, "CalcInterval"))   femonConfig.calcinterval   = atoi(Value);
  else
    return false;
  if (femonConfig.displaymode < 0 || femonConfig.displaymode >= eFemonModeMaxNumber) femonConfig.displaymode = 0;

  return true;
}

cMenuFemonSetup::cMenuFemonSetup(void)
{
  dispmodes[eFemonModeBasic]       = tr("basic");
  dispmodes[eFemonModeTransponder] = tr("transponder");
  dispmodes[eFemonModeStream]      = tr("stream");
  dispmodes[eFemonModeAC3]         = tr("AC-3");

  themes[eFemonThemeClassic]       = tr("Classic");
  themes[eFemonThemeElchi]         = tr("Elchi");
  themes[eFemonThemeDeepBlue]      = tr("DeepBlue");

  Setup();
}

void cMenuFemonSetup::Setup(void)
{
  int current = Current();

  Clear();
  Add(new cMenuEditBoolItem(  tr("Hide main menu entry"),        &femonConfig.hidemenu,       tr("no"),            tr("yes")));
  Add(new cMenuEditBoolItem(  tr("Use syslog output"),           &femonConfig.syslogoutput,   tr("no"),            tr("yes")));
  Add(new cMenuEditStraItem(  tr("Default display mode"),        &femonConfig.displaymode,    eFemonModeMaxNumber, dispmodes));
  Add(new cMenuEditStraItem(  tr("Theme"),                       &femonConfig.theme,          eFemonThemeMaxNumber,themes));
  Add(new cMenuEditBoolItem(  tr("Position"),                    &femonConfig.position,       tr("bottom"),        tr("top")));
  Add(new cMenuEditIntItem(   tr("Height"),                      &femonConfig.osdheight,      400,                 500));
  Add(new cMenuEditIntItem(   tr("Horizontal offset"),           &femonConfig.osdoffset,      -50,                 50));
  Add(new cMenuEditBoolItem(  tr("Show CA system"),              &femonConfig.showcasystem,   tr("no"),            tr("yes")));
  Add(new cMenuEditIntItem(   tr("Red limit [%]"),               &femonConfig.redlimit,       1,                   50));
  Add(new cMenuEditIntItem(   tr("Green limit [%]"),             &femonConfig.greenlimit,     51,                  100));
  Add(new cMenuEditIntItem(   tr("OSD update interval [0.1s]"),  &femonConfig.updateinterval, 1,                   100));
  Add(new cMenuEditBoolItem(  tr("Analyze stream"),              &femonConfig.analyzestream,  tr("no"),            tr("yes")));
  if (femonConfig.analyzestream)
     Add(new cMenuEditIntItem(tr("Calculation interval [0.1s]"), &femonConfig.calcinterval,   1,                   100));

  SetCurrent(Get(current));
  Display();
}

void cMenuFemonSetup::Store(void)
{
  SetupStore("HideMenu",       femonConfig.hidemenu);
  SetupStore("SyslogOutput",   femonConfig.syslogoutput);
  SetupStore("DisplayMode",    femonConfig.displaymode);
  SetupStore("Theme",          femonConfig.theme);
  SetupStore("Position",       femonConfig.position);
  SetupStore("OSDHeight",      femonConfig.osdheight);
  SetupStore("OSDOffset",      femonConfig.osdoffset);
  SetupStore("ShowCASystem",   femonConfig.showcasystem);
  SetupStore("RedLimit",       femonConfig.redlimit);
  SetupStore("GreenLimit",     femonConfig.greenlimit);
  SetupStore("UpdateInterval", femonConfig.updateinterval);
  SetupStore("AnalStream",     femonConfig.analyzestream);
  SetupStore("CalcInterval",   femonConfig.calcinterval);
}

eOSState cMenuFemonSetup::ProcessKey(eKeys Key)
{
  int oldAnalyzestream = femonConfig.analyzestream;

  eOSState state = cMenuSetupPage::ProcessKey(Key);

  if (Key != kNone && (femonConfig.analyzestream != oldAnalyzestream)) {
     Setup();
     }

  return state;
}

cMenuSetupPage *cPluginFemon::SetupMenu(void)
{
  // Return a setup menu in case the plugin supports one.
  return new cMenuFemonSetup;
}

VDRPLUGINCREATOR(cPluginFemon); // Don't touch this!
