/*
 * A Frontend Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#include "femon.h"
#include "femonosd.h"
#include "femoni18n.h"

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
  if      (!strcasecmp(Name, "HideMenu"))   femonConfig.hidemenu   = atoi(Value);
  else if (!strcasecmp(Name, "Position"))   femonConfig.position   = atoi(Value);
  else if (!strcasecmp(Name, "Interval"))   femonConfig.interval   = atoi(Value);
  else if (!strcasecmp(Name, "RedLimit"))   femonConfig.redlimit   = atoi(Value);
  else if (!strcasecmp(Name, "GreenLimit")) femonConfig.greenlimit = atoi(Value);
  else
    return false;

  return true;
}

cMenuFemonSetup::cMenuFemonSetup(void)
{
  Add(new cMenuEditBoolItem(tr("Hide Mainmenu Entry"),    &femonConfig.hidemenu,   tr("no"), tr("yes")));
  Add(new cMenuEditBoolItem(tr("Position"),               &femonConfig.position,   tr("bottom"), tr("top")));
  Add(new cMenuEditIntItem( tr("Update Interval [0.1s]"), &femonConfig.interval,   1, 20));
  Add(new cMenuEditIntItem( tr("Red Limit [%]"),          &femonConfig.redlimit,   1, 50));
  Add(new cMenuEditIntItem( tr("Green Limit [%]"),        &femonConfig.greenlimit, 51, 100));
}

void cMenuFemonSetup::Store(void)
{
  SetupStore("HideMenu",   femonConfig.hidemenu);
  SetupStore("Position",   femonConfig.position);
  SetupStore("Interval",   femonConfig.interval);
  SetupStore("RedLimit",   femonConfig.redlimit);
  SetupStore("GreenLimit", femonConfig.greenlimit);
}

cMenuSetupPage *cPluginFemon::SetupMenu(void)
{
  // Return a setup menu in case the plugin supports one.
  return new cMenuFemonSetup;
}

VDRPLUGINCREATOR(cPluginFemon); // Don't touch this!
