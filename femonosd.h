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

class cFemonOsd : public cOsdObject, public cThread, public cStatus {
private:
  bool m_Active;
  cOsd *m_Osd;
  cFemonReceiver *m_Receiver;
  int m_Frontend;
  struct dvb_frontend_info m_FrontendInfo;
  int m_Number;
  int m_OldNumber;
  int m_InputTime;
  uint16_t m_SNR;
  uint16_t m_Signal;
  uint32_t m_BER;
  uint32_t m_UNC;
  fe_status_t m_FrontendStatus;
  int m_DisplayMode;
  const cFont *m_Font;
  cMutex* m_Mutex;
  static cBitmap bmDD, bmDD20, bmDD51, bmPAL, bmNTSC, bmAspectRatio_1_1, bmAspectRatio_16_9, bmAspectRatio_2_21_1, bmAspectRatio_4_3;
  void DrawStatusWindow(void);
  void DrawInfoWindow(void);

protected:
  virtual void Action(void);
  virtual void ChannelSwitch(const cDevice * device, int channelNumber);

public:
  cFemonOsd(void);
  ~cFemonOsd();

  virtual void Show(void);
  virtual eOSState ProcessKey(eKeys Key);
};

#endif //__FEMONOSD_H

