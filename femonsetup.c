/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include <vdr/menu.h>
#include "femonconfig.h"
#include "femontools.h"
#include "femonsetup.h"

cMenuFemonSetup::cMenuFemonSetup()
{
  debug("%s()\n", __PRETTY_FUNCTION__);
  dispModesM[eFemonModeBasic]       = tr("basic");
  dispModesM[eFemonModeTransponder] = tr("transponder");
  dispModesM[eFemonModeStream]      = tr("stream");
  dispModesM[eFemonModeAC3]         = tr("AC-3");

  skinsM[eFemonSkinClassic]         = tr("Classic");
  skinsM[eFemonSkinElchi]           = tr("Elchi");

  themesM[eFemonThemeClassic]       = tr("Classic");
  themesM[eFemonThemeElchi]         = tr("Elchi");
  themesM[eFemonThemeSTTNG]         = tr("ST:TNG");
  themesM[eFemonThemeDeepBlue]      = tr("DeepBlue");
  themesM[eFemonThemeMoronimo]      = tr("Moronimo");
  themesM[eFemonThemeEnigma]        = tr("Enigma");
  themesM[eFemonThemeEgalsTry]      = tr("EgalsTry");
  themesM[eFemonThemeDuotone]       = tr("Duotone");
  themesM[eFemonThemeSilverGreen]   = tr("SilverGreen");
  themesM[eFemonThemePearlHD]       = tr("PearlHD");

  dataM = FemonConfig;
  SetMenuCategory(mcSetupPlugins);
  Setup();
}

void cMenuFemonSetup::Setup(void)
{
  int current = Current();

  Clear();
  helpM.Clear();

  Add(new cMenuEditBoolItem(tr("Hide main menu entry"), &dataM.hidemenu));
  helpM.Append(tr("Define whether the main menu entry is hidden."));

  Add(new cMenuEditStraItem(tr("Default display mode"), &dataM.displaymode, eFemonModeMaxNumber, dispModesM));
  helpM.Append(tr("Define the default display mode at startup."));

  Add(new cMenuEditStraItem(trVDR("Setup.OSD$Skin"), &dataM.skin, eFemonSkinMaxNumber, skinsM));
  helpM.Append(tr("Define the used OSD skin."));

  Add(new cMenuEditStraItem(trVDR("Setup.OSD$Theme"), &dataM.theme, eFemonThemeMaxNumber, themesM));
  helpM.Append(tr("Define the used OSD theme."));

  Add(new cMenuEditBoolItem(tr("Position"), &dataM.position, trVDR("bottom"), trVDR("top")));
  helpM.Append(tr("Define the position of OSD."));

  Add(new cMenuEditIntItem(tr("Downscale OSD size [%]"), &dataM.downscale, 0, 20));
  helpM.Append(tr("Define the downscale ratio for OSD size."));

  Add(new cMenuEditIntItem(tr("Red limit [%]"), &dataM.redlimit, 1, 50));
  helpM.Append(tr("Define a limit for red bar, which is used to indicate a bad signal."));

  Add(new cMenuEditIntItem(tr("Green limit [%]"), &dataM.greenlimit, 51, 100));
  helpM.Append(tr("Define a limit for green bar, which is used to indicate a good signal."));

  Add(new cMenuEditIntItem(tr("OSD update interval [0.1s]"), &dataM.updateinterval, 1, 100));
  helpM.Append(tr("Define an interval for OSD updates. The smaller interval generates higher CPU load."));

  Add(new cMenuEditBoolItem(tr("Analyze stream"), &dataM.analyzestream));
  helpM.Append(tr("Define whether the DVB stream is analyzed and bitrates calculated."));

  if (FemonConfig.analyzestream) {
     Add(new cMenuEditIntItem(tr("Calculation interval [0.1s]"), &dataM.calcinterval, 1, 100));
     helpM.Append(tr("Define an interval for calculation. The bigger interval generates more stable values."));
     }

  Add(new cMenuEditBoolItem(tr("Use SVDRP service"), &dataM.usesvdrp));
  helpM.Append(tr("Define whether the SVDRP service is used in client/server setups."));

  if (dataM.usesvdrp) {
     Add(new cMenuEditIntItem(tr("SVDRP service port"), &dataM.svdrpport, 1, 65535));
     helpM.Append(tr("Define the port number of SVDRP service."));

     Add(new cMenuEditStrItem(tr("SVDRP service IP"), dataM.svdrpip, sizeof(dataM.svdrpip), ".1234567890"));
     helpM.Append(tr("Define the IP address of SVDRP service."));
     }

  SetCurrent(Get(current));
  Display();
}

void cMenuFemonSetup::Store(void)
{
  debug("%s()\n", __PRETTY_FUNCTION__);
  FemonConfig = dataM;
  SetupStore("HideMenu",       FemonConfig.hidemenu);
  SetupStore("DisplayMode",    FemonConfig.displaymode);
  SetupStore("Skin",           FemonConfig.skin);
  SetupStore("Theme",          FemonConfig.theme);
  SetupStore("Position",       FemonConfig.position);
  SetupStore("Downscale",      FemonConfig.downscale);
  SetupStore("RedLimit",       FemonConfig.redlimit);
  SetupStore("GreenLimit",     FemonConfig.greenlimit);
  SetupStore("UpdateInterval", FemonConfig.updateinterval);
  SetupStore("AnalStream",     FemonConfig.analyzestream);
  SetupStore("CalcInterval",   FemonConfig.calcinterval);
  SetupStore("UseSvdrp",       FemonConfig.usesvdrp);
  SetupStore("ServerPort",     FemonConfig.svdrpport);
  SetupStore("ServerIp",       FemonConfig.svdrpip);
}

eOSState cMenuFemonSetup::ProcessKey(eKeys Key)
{
  int oldUsesvdrp = dataM.usesvdrp;
  int oldAnalyzestream = dataM.analyzestream;

  eOSState state = cMenuSetupPage::ProcessKey(Key);

  if (Key != kNone && (dataM.analyzestream != oldAnalyzestream || dataM.usesvdrp != oldUsesvdrp))
     Setup();

  if ((Key == kInfo) && (state == osUnknown) && (Current() < helpM.Size()))
     return AddSubMenu(new cMenuText(cString::sprintf("%s - %s '%s'", tr("Help"), trVDR("Plugin"), PLUGIN_NAME_I18N), helpM[Current()]));

  return state;
}
