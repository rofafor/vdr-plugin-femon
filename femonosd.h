#ifndef __FEMONOSD_H
#define __FEMONOSD_H

#include <linux/dvb/frontend.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <vdr/osd.h>
#include <vdr/thread.h>
#include <vdr/status.h>
#include <vdr/channels.h>
#include <vdr/font.h>
#include "femonreceiver.h"

class cFemonOsd : public cOsdObject, public cThread, public cStatus {
private:
  bool m_Active;
  cOsdBase *m_Osd;
  tWindowHandle m_Window;
  cFemonReceiver *m_Receiver;
  int m_Frontend;
  struct dvb_frontend_info m_FrontendInfo;
  int m_Number;
  int m_InputTime;
  int m_InfoTime;
  int m_Width;
  int m_Height;
  int m_Xpos;
  int m_Ypos;

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

