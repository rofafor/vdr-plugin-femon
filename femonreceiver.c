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
:cReceiver(Ca, -1, 2, Vpid, Apid), cThread("femon receiver")
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
     Cancel(0);
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
  // TS packet length: 188
  if (Length == 188) {
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
  m_Active = true;
  while (m_Active) {
        // TS packet 188 bytes - 4 byte header; MPEG standard defines 1Mbit = 1000000bit
        m_VideoBitrate = (8.0 * 184.0 * m_VideoPacketCount) / (femonConfig.calcinterval * 100000.0);
        m_VideoPacketCount = 0;
        m_AudioBitrate = (8.0 * 184.0 * m_AudioPacketCount) / (femonConfig.calcinterval * 100.0);
        m_AudioPacketCount = 0;
        usleep(100000L * femonConfig.calcinterval);
    }
}
