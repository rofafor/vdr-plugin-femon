/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#ifndef __FEMONRECEIVER_H
#define __FEMONRECEIVER_H

#include <vdr/thread.h>
#include <vdr/receiver.h>

class cFemonReceiver : public cReceiver, public cThread {
private:
  bool m_Active;
  int m_VideoPid;
  int m_AudioPid;
  int m_VideoPacketCount;
  int m_AudioPacketCount;
  double m_VideoBitrate;
  double m_AudioBitrate;

protected:
  virtual void Activate(bool On);
  virtual void Receive(uchar *Data, int Length);
  virtual void Action(void);

public:
  cFemonReceiver(int Ca, int Vpid, int Apid);
  virtual ~cFemonReceiver();

  double VideoBitrate(void) { return m_VideoBitrate; }; // Mbit/s
  double AudioBitrate(void) { return m_AudioBitrate; }; // kbit/s
  };

#endif //__FEMONRECEIVER_H

