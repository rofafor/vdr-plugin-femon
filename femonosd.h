/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __FEMONOSD_H
#define __FEMONOSD_H

#include <linux/dvb/frontend.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <vdr/osd.h>
#include <vdr/thread.h>
#include <vdr/status.h>
#include <vdr/plugin.h>
#include <vdr/channels.h>
#include <vdr/transfer.h>
#include <vdr/tools.h>
#include "svdrpservice.h"

#define MAX_BM_NUMBER 8

class cFemonOsd : public cOsdObject, public cThread, public cStatus {
private:
  enum eDeviceSourceType {
    DEVICESOURCE_DVBAPI = 0,
    DEVICESOURCE_IPTV,
    DEVICESOURCE_PVRINPUT,
    DEVICESOURCE_COUNT
    };

  static cFemonOsd *pInstanceS;

  cOsd             *osdM;
  cFemonReceiver   *receiverM;
  int               frontendM;
  int               svdrpFrontendM;
  double            svdrpVideoBitRateM;
  double            svdrpAudioBitRateM;
  SvdrpConnection_v1_0 svdrpConnectionM;
  cPlugin          *svdrpPluginM;
  int               numberM;
  int               oldNumberM;
  int               qualityM;
  bool              qualityValidM;
  int               strengthM;
  bool              strengthValidM;
  uint16_t          snrM;
  bool              snrValidM;
  uint16_t          signalM;
  bool              signalValidM;
  uint32_t          berM;
  bool              berValidM;
  uint32_t          uncM;
  bool              uncValidM;
  cString           frontendNameM;
  fe_status_t       frontendStatusM;
  bool              frontendStatusValidM;
  dvb_frontend_info frontendInfoM;
  eDeviceSourceType deviceSourceM;
  int               displayModeM;
  int               osdWidthM;
  int               osdHeightM;
  int               osdLeftM;
  int               osdTopM;
  cFont            *fontM;
  cTimeMs           inputTimeM;
  cCondWait         sleepM;
  cMutex            mutexM;

  void DrawStatusWindow(void);
  void DrawInfoWindow(void);
  bool SvdrpConnect(void);
  bool SvdrpTune(void);

protected:
  cFemonOsd();
  cFemonOsd(const cFemonOsd&);
  cFemonOsd& operator= (const cFemonOsd&);
  virtual void Action(void);
  virtual void ChannelSwitch(const cDevice *deviceP, int channelNumberP, bool liveViewP);
  virtual void SetAudioTrack(int indexP, const char * const *tracksP);

public:
  static cFemonOsd *Instance(bool createP = false);
  ~cFemonOsd();

  virtual void Show(void);
  virtual eOSState ProcessKey(eKeys keyP);

  bool    DeviceSwitch(int directionP);
  double  GetVideoBitrate(void);
  double  GetAudioBitrate(void);
  double  GetDolbyBitrate(void);
};

#endif //__FEMONOSD_H

