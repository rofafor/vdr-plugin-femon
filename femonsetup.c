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
: hideMenuM(FemonConfig.GetHideMenu()),
  displayModeM(FemonConfig.GetDisplayMode()),
  skinM(FemonConfig.GetSkin()),
  themeM(FemonConfig.GetTheme()),
  positionM(FemonConfig.GetPosition()),
  downscaleM(FemonConfig.GetDownscale()),
  redLimitM(FemonConfig.GetRedLimit()),
  greenLimitM(FemonConfig.GetGreenLimit()),
  updateIntervalM(FemonConfig.GetUpdateInterval()),
  analyzeStreamM(FemonConfig.GetAnalyzeStream()),
  calcIntervalM(FemonConfig.GetCalcInterval()),
  useSvdrpM(FemonConfig.GetUseSvdrp()),
  svdrpPortM(FemonConfig.GetSvdrpPort())
{
  debug("%s()\n", __PRETTY_FUNCTION__);
  strn0cpy(svdrpIpM, FemonConfig.GetSvdrpIp(), sizeof(svdrpIpM));

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

  SetMenuCategory(mcSetupPlugins);
  Setup();
}

void cMenuFemonSetup::Setup(void)
{
  int current = Current();

  Clear();
  helpM.Clear();

  Add(new cMenuEditBoolItem(tr("Hide main menu entry"), &hideMenuM));
  helpM.Append(tr("Define whether the main menu entry is hidden."));

  Add(new cMenuEditStraItem(tr("Default display mode"), &displayModeM, eFemonModeMaxNumber, dispModesM));
  helpM.Append(tr("Define the default display mode at startup."));

  Add(new cMenuEditStraItem(trVDR("Setup.OSD$Skin"), &skinM, eFemonSkinMaxNumber, skinsM));
  helpM.Append(tr("Define the used OSD skin."));

  Add(new cMenuEditStraItem(trVDR("Setup.OSD$Theme"), &themeM, eFemonThemeMaxNumber, themesM));
  helpM.Append(tr("Define the used OSD theme."));

  Add(new cMenuEditBoolItem(tr("Position"), &positionM, trVDR("bottom"), trVDR("top")));
  helpM.Append(tr("Define the position of OSD."));

  Add(new cMenuEditIntItem(tr("Downscale OSD size [%]"), &downscaleM, 0, 20));
  helpM.Append(tr("Define the downscale ratio for OSD size."));

  Add(new cMenuEditIntItem(tr("Red limit [%]"), &redLimitM, 1, 50));
  helpM.Append(tr("Define a limit for red bar, which is used to indicate a bad signal."));

  Add(new cMenuEditIntItem(tr("Green limit [%]"), &greenLimitM, 51, 100));
  helpM.Append(tr("Define a limit for green bar, which is used to indicate a good signal."));

  Add(new cMenuEditIntItem(tr("OSD update interval [0.1s]"), &updateIntervalM, 1, 100));
  helpM.Append(tr("Define an interval for OSD updates. The smaller interval generates higher CPU load."));

  Add(new cMenuEditBoolItem(tr("Analyze stream"), &analyzeStreamM));
  helpM.Append(tr("Define whether the DVB stream is analyzed and bitrates calculated."));

  if (analyzeStreamM) {
     Add(new cMenuEditIntItem(tr("Calculation interval [0.1s]"), &calcIntervalM, 1, 100));
     helpM.Append(tr("Define an interval for calculation. The bigger interval generates more stable values."));
     }

  Add(new cMenuEditBoolItem(tr("Use SVDRP service"), &useSvdrpM));
  helpM.Append(tr("Define whether the SVDRP service is used in client/server setups."));

  if (useSvdrpM) {
     Add(new cMenuEditIntItem(tr("SVDRP service port"), &svdrpPortM, 1, 65535));
     helpM.Append(tr("Define the port number of SVDRP service."));

     Add(new cMenuEditStrItem(tr("SVDRP service IP"), svdrpIpM, sizeof(svdrpIpM), ".1234567890"));
     helpM.Append(tr("Define the IP address of SVDRP service."));
     }

  SetCurrent(Get(current));
  Display();
}

void cMenuFemonSetup::Store(void)
{
  debug("%s()\n", __PRETTY_FUNCTION__);
  // Store values into setup.conf
  SetupStore("HideMenu",       hideMenuM);
  SetupStore("DisplayMode",    displayModeM);
  SetupStore("Skin",           skinM);
  SetupStore("Theme",          themeM);
  SetupStore("Position",       positionM);
  SetupStore("Downscale",      downscaleM);
  SetupStore("RedLimit",       redLimitM);
  SetupStore("GreenLimit",     greenLimitM);
  SetupStore("UpdateInterval", updateIntervalM);
  SetupStore("AnalStream",     analyzeStreamM);
  SetupStore("CalcInterval",   calcIntervalM);
  SetupStore("UseSvdrp",       useSvdrpM);
  SetupStore("ServerPort",     svdrpPortM);
  SetupStore("ServerIp",       svdrpIpM);
  // Update global config
  FemonConfig.SetHideMenu(hideMenuM);
  FemonConfig.SetDisplayMode(displayModeM);
  FemonConfig.SetSkin(skinM);
  FemonConfig.SetTheme(themeM);
  FemonConfig.SetPosition(positionM);
  FemonConfig.SetDownscale(downscaleM);
  FemonConfig.SetRedLimit(redLimitM);
  FemonConfig.SetGreenLimit(greenLimitM);
  FemonConfig.SetUpdateInterval(updateIntervalM);
  FemonConfig.SetAnalyzeStream(analyzeStreamM);
  FemonConfig.SetCalcInterval(calcIntervalM);
  FemonConfig.SetUseSvdrp(useSvdrpM);
  FemonConfig.SetSvdrpPort(svdrpPortM);
  FemonConfig.SetSvdrpIp(svdrpIpM);
}

eOSState cMenuFemonSetup::ProcessKey(eKeys keyP)
{
  int oldUseSvdrp = useSvdrpM;
  int oldAnalyzeStream = analyzeStreamM;

  eOSState state = cMenuSetupPage::ProcessKey(keyP);

  if (keyP != kNone && (analyzeStreamM != oldAnalyzeStream || useSvdrpM != oldUseSvdrp))
     Setup();

  if ((keyP == kInfo) && (state == osUnknown) && (Current() < helpM.Size()))
     return AddSubMenu(new cMenuText(cString::sprintf("%s - %s '%s'", tr("Help"), trVDR("Plugin"), PLUGIN_NAME_I18N), helpM[Current()]));

  return state;
}
