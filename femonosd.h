/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#ifndef __FEMONOSD_H
#define __FEMONOSD_H

#include <linux/dvb/frontend.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <vdr/osd.h>
#include <vdr/thread.h>
#include <vdr/status.h>
#include <vdr/channels.h>
#include <vdr/transfer.h>
#include <vdr/tools.h>

class cFemonOsd : public cOsdObject, public cThread, public cStatus {
private:
  cOsd *m_Osd;
  cFemonReceiver *m_Receiver;
  int m_Frontend;
  struct dvb_frontend_info m_FrontendInfo;
  int m_Number;
  int m_OldNumber;
  uint16_t m_SNR;
  uint16_t m_Signal;
  uint32_t m_BER;
  uint32_t m_UNC;
  fe_status_t m_FrontendStatus;
  int m_DisplayMode;
  const cFont *m_Font;
  cTimeMs m_InputTime;
  cMutex* m_Mutex;
  static cBitmap bmStereo, bmMonoLeft, bmMonoRight, bmDD, bmDD20, bmDD51;
  static cBitmap bmZero, bmDevice, bmPAL, bmNTSC, bmOne, bmTwo, bmThree, bmFour, bmFive;
  static cBitmap bmAspectRatio_1_1, bmAspectRatio_16_9, bmAspectRatio_2_21_1, bmAspectRatio_4_3;
  static cBitmap bmLock, bmSignal, bmCarrier, bmViterbi, bmSync;
  void DrawStatusWindow(void);
  void DrawInfoWindow(void);

protected:
  virtual void Action(void);
  virtual void ChannelSwitch(const cDevice * device, int channelNumber);
  virtual void SetAudioTrack(int Index, const char * const *Tracks);

public:
  cFemonOsd(void);
  ~cFemonOsd();

  virtual void Show(void);
  virtual eOSState ProcessKey(eKeys Key);
};

#endif //__FEMONOSD_H

