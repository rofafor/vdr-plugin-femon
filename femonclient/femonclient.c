/*
 * femonclient.c: Demo femon service client plugin
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id: $
 */

#include <stdlib.h>
#include <vdr/interface.h>
#include <vdr/plugin.h>
#include "femonservice.h"

static const char *VERSION        = "0.0.1";
static const char *DESCRIPTION    = "Femon client";
static const char *MAINMENUENTRY  = "Show frontend statistic on console";

class cPluginFemonClient : public cPlugin {
public:
  virtual const char *Version(void) { return VERSION; }
  virtual const char *Description(void) { return DESCRIPTION; }
  virtual const char *MainMenuEntry(void) { return MAINMENUENTRY; }
  virtual cOsdObject *MainMenuAction(void);
  };

// --- cPluginFemonClient ----------------------------------------------------------

cOsdObject *cPluginFemonClient::MainMenuAction(void)
{
  FemonService_v1_0 femon;
  cPlugin *p;

  p = cPluginManager::CallFirstService("FemonService-v1.0", &femon);
  if (p) {
     printf("Device : %s\n", *femon.fe_name);
     printf("Status : %s\n", *femon.fe_status);
     printf("Signal : %04X (%2d%%)\n", femon.fe_signal, femon.fe_signal / 655);
     printf("SNR    : %04X (%2d%%)\n", femon.fe_snr, femon.fe_snr / 655);
     printf("BER    : %08X\n", femon.fe_ber);
     printf("UNC    : %08X\n", femon.fe_unc);
     printf("Video  : %.2f Mbit/s\n", femon.video_bitrate);
     printf("Audio  : %.0f kbit/s\n", femon.audio_bitrate);
     }
  else
     printf("\n--- No support for FemonService-v1.0 found ---\n");

  return NULL;
}

VDRPLUGINCREATOR(cPluginFemonClient); // Don't touch this!
