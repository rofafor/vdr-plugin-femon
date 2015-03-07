/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include <vdr/menu.h>
#include <vdr/remote.h>
#include <vdr/player.h>
#include "femoncfg.h"
#include "femonreceiver.h"
#include "femonosd.h"
#include "femonservice.h"
#include "femontools.h"

#if defined(APIVERSNUM) && APIVERSNUM < 20200
#error "VDR-2.2.0 API version or greater is required!"
#endif

#ifndef GITVERSION
#define GITVERSION ""
#endif

static const char VERSION[]       = "2.2.1" GITVERSION;
static const char DESCRIPTION[]   = trNOOP("DVB Signal Information Monitor (OSD)");
static const char MAINMENUENTRY[] = trNOOP("Signal Information");

class cPluginFemon : public cPlugin {
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
  virtual void MainThreadHook(void) {}
  virtual cString Active(void) { return NULL; }
  virtual const char *MainMenuEntry(void) { return (FemonConfig.hidemenu ? NULL : tr(MAINMENUENTRY)); }
  virtual cOsdObject *MainMenuAction(void);
  virtual cMenuSetupPage *SetupMenu(void);
  virtual bool SetupParse(const char *nameP, const char *valueP);
  virtual bool Service(const char *idP, void *dataP);
  virtual const char **SVDRPHelpPages(void);
  virtual cString SVDRPCommand(const char *commandP, const char *optionP, int &replyCodeP);
  };

cPluginFemon::cPluginFemon()
{
  // Initialize any member variables here.
  // DON'T DO ANYTHING ELSE THAT MAY HAVE SIDE EFFECTS, REQUIRE GLOBAL
  // VDR OBJECTS TO EXIST OR PRODUCE ANY OUTPUT!
  debug("%s()\n", __PRETTY_FUNCTION__);
}

cPluginFemon::~cPluginFemon()
{
  // Clean up after yourself!
  debug("%s()\n", __PRETTY_FUNCTION__);
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
  debug("%s()\n", __PRETTY_FUNCTION__);
  if (cControl::Control() || (Channels.Count() <= 0))
     Skins.Message(mtInfo, tr("Femon not available"));
  else
     return cFemonOsd::Instance(true);
  return NULL;
}

bool cPluginFemon::SetupParse(const char *nameP, const char *valueP)
{
  // Parse your own setup parameters and store their values.
  if      (!strcasecmp(nameP, "HideMenu"))       FemonConfig.hidemenu       = atoi(valueP);
  else if (!strcasecmp(nameP, "DisplayMode"))    FemonConfig.displaymode    = atoi(valueP);
  else if (!strcasecmp(nameP, "Position"))       FemonConfig.position       = atoi(valueP);
  else if (!strcasecmp(nameP, "Skin"))           FemonConfig.skin           = atoi(valueP);
  else if (!strcasecmp(nameP, "Theme"))          FemonConfig.theme          = atoi(valueP);
  else if (!strcasecmp(nameP, "Downscale"))      FemonConfig.downscale      = atoi(valueP);
  else if (!strcasecmp(nameP, "RedLimit"))       FemonConfig.redlimit       = atoi(valueP);
  else if (!strcasecmp(nameP, "GreenLimit"))     FemonConfig.greenlimit     = atoi(valueP);
  else if (!strcasecmp(nameP, "UpdateInterval")) FemonConfig.updateinterval = atoi(valueP);
  else if (!strcasecmp(nameP, "AnalStream"))     FemonConfig.analyzestream  = atoi(valueP);
  else if (!strcasecmp(nameP, "CalcInterval"))   FemonConfig.calcinterval   = atoi(valueP);
  else if (!strcasecmp(nameP, "UseSvdrp"))       FemonConfig.usesvdrp       = atoi(valueP);
  else if (!strcasecmp(nameP, "ServerPort"))     FemonConfig.svdrpport      = atoi(valueP);
  else if (!strcasecmp(nameP, "ServerIp"))       strn0cpy(FemonConfig.svdrpip, valueP, sizeof(FemonConfig.svdrpip));
  else
    return false;
  if (FemonConfig.displaymode < 0 || FemonConfig.displaymode >= eFemonModeMaxNumber) FemonConfig.displaymode = 0;

  return true;
}

bool cPluginFemon::Service(const char *idP, void *dataP)
{
  if (strcmp(idP, "FemonService-v1.0") == 0) {
     if (dataP) {
        FemonService_v1_0 *data = reinterpret_cast<FemonService_v1_0*>(dataP);
        if (!cDevice::ActualDevice())
           return false;
        cDvbDevice *dev = getDvbDevice(cDevice::ActualDevice());
        data->fe_name = getFrontendName(dev);
        data->fe_status = getFrontendStatus(dev);
        data->fe_snr = getSNR(dev);
        data->fe_signal = getSignal(dev);
        data->fe_ber = getBER(dev);
        data->fe_unc = getUNC(dev);
        data->video_bitrate = cFemonOsd::Instance() ? cFemonOsd::Instance()->GetVideoBitrate() : 0.0;
        data->audio_bitrate = cFemonOsd::Instance() ? cFemonOsd::Instance()->GetAudioBitrate() : 0.0;
        data->dolby_bitrate = cFemonOsd::Instance() ? cFemonOsd::Instance()->GetDolbyBitrate() : 0.0;
        }
     return true;
     }

  return false;
}

const char **cPluginFemon::SVDRPHelpPages(void)
{
  static const char *HelpPages[] = {
    "OPEN\n"
    "    Open femon plugin.",
    "QUIT\n"
    "    Close femon plugin.",
    "NEXT\n"
    "    Switch to next possible device.",
    "PREV\n"
    "    Switch to previous possible device.",
    "INFO <card index>\n"
    "    Print the frontend information.",
    "NAME <card index>\n"
    "    Print the frontend name.",
    "STAT <card index>\n"
    "    Print the frontend status.",
    "STRG <card index>\n"
    "    Print the signal strength.",
    "QUAL <card index>\n"
    "    Print the signal quality.",
    "SGNL <card index>\n"
    "    Print the signal strength from driver.",
    "SNRA <card index>\n"
    "    Print the signal-to-noise ratio from driver.",
    "BERA <card index>\n"
    "    Print the bit error rate.",
    "UNCB <card index>\n"
    "    Print the uncorrected blocks rate.",
    "VIBR\n"
    "    Print the current video bitrate [Mbit/s].",
    "AUBR\n"
    "    Print the current audio bitrate [kbit/s].",
    "DDBR\n"
    "    Print the current dolby bitrate [kbit/s].",
    NULL
    };
  return HelpPages;
}

cString cPluginFemon::SVDRPCommand(const char *commandP, const char *optionP, int &replyCodeP)
{
  cDvbDevice *dev = getDvbDevice(cDevice::ActualDevice());
  if (*optionP && isnumber(optionP)) {
     cDvbDevice *dev2 = dynamic_cast<cDvbDevice*>(cDevice::GetDevice(int(strtol(optionP, NULL, 10))));
     if (dev2)
        dev = dev2;
     }
  if (cReplayControl::NowReplaying() || !dev) {
     replyCodeP = 550; // Requested action not taken
     return cString("Cannot open femon plugin while replaying");
     }
  if (strcasecmp(commandP, "OPEN") == 0) {
     if (!cFemonOsd::Instance())
        cRemote::CallPlugin(Name());
     return cString("Opening femon plugin");
     }
  else if (strcasecmp(commandP, "QUIT") == 0) {
     if (cFemonOsd::Instance())
        cRemote::Put(kBack);
     return cString("Closing femon plugin");
     }
  else if (strcasecmp(commandP, "NEXT") == 0) {
     if (cFemonOsd::Instance())
        return cString::sprintf("Switching to next device: %s", cFemonOsd::Instance()->DeviceSwitch(1) ? "ok" : "failed");
     else
        return cString("Cannot switch device");
     }
  else if (strcasecmp(commandP, "PREV") == 0) {
     if (cFemonOsd::Instance())
        return cString::sprintf("Switching to previous device: %s", cFemonOsd::Instance()->DeviceSwitch(-1) ? "ok" : "failed");
     else
        return cString("Cannot switch device");
     }
  else if (strcasecmp(commandP, "INFO") == 0) {
     return getFrontendInfo(dev);
     }
  else if (strcasecmp(commandP, "NAME") == 0) {
     return getFrontendName(dev);
     }
  else if (strcasecmp(commandP, "STAT") == 0) {
     return getFrontendStatus(dev);
     }
  else if (strcasecmp(commandP, "STRG") == 0) {
     return cString::sprintf("%d on device #%d", dev->SignalStrength(), dev->CardIndex());
     }
  else if (strcasecmp(commandP, "QUAL") == 0) {
     return cString::sprintf("%d on device #%d", dev->SignalQuality(), dev->CardIndex());
     }
  else if (strcasecmp(commandP, "SGNL") == 0) {
     int value = getSignal(dev);
     return cString::sprintf("%04X (%02d%%) on device #%d", value, value / 655, dev->CardIndex());
     }
  else if (strcasecmp(commandP, "SNRA") == 0) {
     int value = getSNR(dev);
     return cString::sprintf("%04X (%02d%%) on device #%d", value, value / 655, dev->CardIndex());
     }
  else if (strcasecmp(commandP, "BERA") == 0) {
     return cString::sprintf("%08X on device #%d", getBER(dev), dev->CardIndex());
     }
  else if (strcasecmp(commandP, "UNCB") == 0) {
     return cString::sprintf("%08X on device #%d", getUNC(dev), dev->CardIndex());
     }
  else if (strcasecmp(commandP, "VIBR") == 0) {
     if (cFemonOsd::Instance())
        return cString::sprintf("%s on device #%d", *getBitrateMbits(cFemonOsd::Instance()->GetVideoBitrate()), cDevice::ActualDevice()->CardIndex());
     else
        return cString::sprintf("--- Mbit/s on device #%d", cDevice::ActualDevice()->CardIndex());
     }
  else if (strcasecmp(commandP, "AUBR") == 0) {
     if (cFemonOsd::Instance())
        return cString::sprintf("%s on device #%d", *getBitrateKbits(cFemonOsd::Instance()->GetAudioBitrate()), cDevice::ActualDevice()->CardIndex());
     else
        return cString::sprintf("--- kbit/s on device #%d", cDevice::ActualDevice()->CardIndex());
     }
  else if (strcasecmp(commandP, "DDBR") == 0) {
     if (cFemonOsd::Instance())
        return cString::sprintf("%s on device #%d", *getBitrateKbits(cFemonOsd::Instance()->GetDolbyBitrate()), cDevice::ActualDevice()->CardIndex());
     else
        return cString::sprintf("--- kbit/s on device #%d", cDevice::ActualDevice()->CardIndex());
     }
  return NULL;
}

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

cMenuSetupPage *cPluginFemon::SetupMenu(void)
{
  // Return a setup menu in case the plugin supports one.
  return new cMenuFemonSetup;
}

VDRPLUGINCREATOR(cPluginFemon); // Don't touch this!
