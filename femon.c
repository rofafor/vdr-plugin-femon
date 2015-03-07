/*
 * femon.c: Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include <getopt.h>
#include <vdr/menu.h>
#include <vdr/remote.h>
#include <vdr/player.h>

#include "config.h"
#include "femonservice.h"
#include "log.h"
#include "osd.h"
#include "tools.h"
#include "setup.h"

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
  virtual const char *MainMenuEntry(void) { return (FemonConfig.GetHideMenu() ? NULL : tr(MAINMENUENTRY)); }
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
  debug1("%s", __PRETTY_FUNCTION__);
}

cPluginFemon::~cPluginFemon()
{
  // Clean up after yourself!
  debug1("%s", __PRETTY_FUNCTION__);
}

const char *cPluginFemon::CommandLineHelp(void)
{
  // Return a string that describes all known command line options.
  return "  -t <mode>, --trace=<mode>  set the tracing mode\n";
}

bool cPluginFemon::ProcessArgs(int argc, char *argv[])
{
  // Implement command line argument processing here if applicable.
  static const struct option long_options[] = {
    { "trace",    required_argument, NULL, 't' },
    { NULL,       no_argument,       NULL,  0  }
    };

  cString server;
  int c;
  while ((c = getopt_long(argc, argv, "t:", long_options, NULL)) != -1) {
    switch (c) {
      case 't':
           FemonConfig.SetTraceMode(strtol(optarg, NULL, 0));
           break;
      default:
           return false;
      }
    }
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
  debug1("%s", __PRETTY_FUNCTION__);
  if (cControl::Control() || (Channels.Count() <= 0))
     Skins.Message(mtInfo, tr("Femon not available"));
  else
     return cFemonOsd::Instance(true);
  return NULL;
}

cMenuSetupPage *cPluginFemon::SetupMenu(void)
{
  // Return a setup menu in case the plugin supports one.
  return new cMenuFemonSetup;
}

bool cPluginFemon::SetupParse(const char *nameP, const char *valueP)
{
  // Parse your own setup parameters and store their values.
  if (!strcasecmp(nameP, "HideMenu"))
     FemonConfig.SetHideMenu(atoi(valueP));
  else if (!strcasecmp(nameP, "DisplayMode"))
     FemonConfig.SetDisplayMode(atoi(valueP));
  else if (!strcasecmp(nameP, "Position"))
     FemonConfig.SetPosition(atoi(valueP));
  else if (!strcasecmp(nameP, "Skin"))
     FemonConfig.SetSkin(atoi(valueP));
  else if (!strcasecmp(nameP, "Theme"))
     FemonConfig.SetTheme(atoi(valueP));
  else if (!strcasecmp(nameP, "Downscale"))
     FemonConfig.SetDownscale(atoi(valueP));
  else if (!strcasecmp(nameP, "RedLimit"))
     FemonConfig.SetRedLimit(atoi(valueP));
  else if (!strcasecmp(nameP, "GreenLimit"))
     FemonConfig.SetGreenLimit(atoi(valueP));
  else if (!strcasecmp(nameP, "UpdateInterval"))
     FemonConfig.SetUpdateInterval(atoi(valueP));
  else if (!strcasecmp(nameP, "AnalStream"))
     FemonConfig.SetAnalyzeStream(atoi(valueP));
  else if (!strcasecmp(nameP, "CalcInterval"))
     FemonConfig.SetCalcInterval(atoi(valueP));
  else if (!strcasecmp(nameP, "UseSvdrp"))
     FemonConfig.SetUseSvdrp(atoi(valueP));
  else if (!strcasecmp(nameP, "ServerPort"))
     FemonConfig.SetSvdrpPort(atoi(valueP));
  else if (!strcasecmp(nameP, "ServerIp"))
     FemonConfig.SetSvdrpIp(valueP);
  else
    return false;

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
    "TRAC [ <mode> ]\n"
    "    Gets and/or sets used tracing mode.\n",
    NULL
    };
  return HelpPages;
}

cString cPluginFemon::SVDRPCommand(const char *commandP, const char *optionP, int &replyCodeP)
{
  cDvbDevice *dev = getDvbDevice(cDevice::ActualDevice());
  if (strcasecmp(commandP, "TRAC") == 0) {
     if (optionP && *optionP)
        FemonConfig.SetTraceMode(strtol(optionP, NULL, 0));
     return cString::sprintf("Tracing mode: 0x%04X\n", FemonConfig.GetTraceMode());
     }
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

VDRPLUGINCREATOR(cPluginFemon); // Don't touch this!
