/*
 * A Frontend Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#include "femonreceiver.h"

#define TS_SIZE 188

cFemonReceiver::cFemonReceiver(int Ca, int Vpid, int Apid)
:cReceiver(Ca, -1, 2, Vpid, Apid)
{
  //printf("cFemonReceiver::cFemonReceiver()\n");
  m_VPid = Vpid;
  m_APid = Apid;
  m_VideoCount = 0;
  m_AudioCount = 0;
}
 
cFemonReceiver::~cFemonReceiver(void)
{
  //printf("cFemonReceiver::~cFemonReceiver()\n");
}

void cFemonReceiver::Activate(bool On)
{
  //printf("cFemonReceiver::Activate()\n");
}

void cFemonReceiver::Receive(uchar *Data, int Length)
{
  //printf("cFemonReceiver::Receive()\n");
  if (Length == TS_SIZE) {
     int pid = ((Data[1]&0x1f) << 8) | (Data[2]);
     if (pid == m_VPid)
        m_VideoCount++;
     if (pid == m_APid)
        m_AudioCount++;
     }
}

int cFemonReceiver::VideoPacketCount(void)
{
  //printf("cFemonReceiver::VideoPacketCount()\n");
  int count = m_VideoCount;
  m_VideoCount = 0;
  return count;
}

int cFemonReceiver::AudioPacketCount(void)
{
  //printf("cFemonReceiver::AudioPacketCount()\n");
  int count = m_AudioCount;
  m_AudioCount = 0;
  return count;
}
