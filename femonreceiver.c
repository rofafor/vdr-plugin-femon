/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#include <unistd.h>
#include "femoncfg.h"
#include "femonreceiver.h"

cFemonReceiver::cFemonReceiver(int Ca, int Vpid, int Apid)
#if VDRVERSNUM >= 10300
:cReceiver(Ca, -1, 2, Vpid, Apid), cThread("femon receiver")
#else
:cReceiver(Ca, -1, 2, Vpid, Apid)
#endif
{
  //printf("cFemonReceiver::cFemonReceiver()\n");
  m_Active = false;
  m_VideoPid = Vpid;
  m_AudioPid = Apid;
  m_VideoPacketCount = 0;
  m_AudioPacketCount = 0;
  m_VideoBitrate = 0.0;
  m_AudioBitrate = 0.0;
}
 
cFemonReceiver::~cFemonReceiver(void)
{
  //printf("cFemonReceiver::~cFemonReceiver()\n");
  if (m_Active) {
     m_Active = false;
     Cancel(3);
     }
}

void cFemonReceiver::Activate(bool On)
{
  //printf("cFemonReceiver::Activate()\n");
  Start();
}

void cFemonReceiver::Receive(uchar *Data, int Length)
{
  //printf("cFemonReceiver::Receive()\n");
  if (Length == TS_SIZE) {
     int pid = ((Data[1] & 0x1f) << 8) | (Data[2]);
     if (pid == m_VideoPid) {
        m_VideoPacketCount++;
        }
     else if (pid == m_AudioPid) {
        m_AudioPacketCount++;
        }
     }
}

void cFemonReceiver::Action(void)
{
  //printf("cFemonReceiver::Action()\n");
#if (VDRVERSNUM < 10300)
  isyslog("femon receiver: thread started (pid = %d)", getpid());
#endif
  m_Active = true;
  while (m_Active) {
        // should we strip the 4 byte header off from TS packet ?
        m_VideoBitrate = (8.0 * TS_SIZE * m_VideoPacketCount) / (femonConfig.calcinterval * 102.4 * 1024.0);
        m_VideoPacketCount = 0;
        m_AudioBitrate = (8.0 * TS_SIZE * m_AudioPacketCount) / (femonConfig.calcinterval * 102.4);
        m_AudioPacketCount = 0;
        usleep(100000L * femonConfig.calcinterval);
    }
#if (VDRVERSNUM < 10300)
  isyslog("femon receiver: thread stopped (pid = %d)", getpid());
#endif
}
