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

#define TS_SIZE       188
#define PAY_START     0x40
#define ADAPT_FIELD   0x20
#define PAYLOAD       0x10
#define PTS_DTS_FLAGS 0xC0

cFemonReceiver::cFemonReceiver(int Ca, int Vpid, int Apid)
:cReceiver(Ca, -1, 2, Vpid, Apid), cThread("femon receiver")
{
  //printf("cFemonReceiver::cFemonReceiver()\n");
  m_Active = false;
  m_VideoPid = Vpid;
  m_AudioPid = Apid;
  m_VideoPacketCount = 0;
  m_VideoHorizontalSize = 0;
  m_VideoVerticalSize = 0;
  m_VideoAspectRatio = AR_RESERVED;
  m_VideoFormat = VF_UNKNOWN;
  m_VideoFrameRate = 0.0;
  m_VideoStreamBitrate = 0.0;
  m_VideoBitrate = 0.0;
  m_AudioPacketCount = 0;
  m_AudioStreamBitrate = -2.0;
  m_AudioBitrate = 0.0;
  m_AudioSamplingFreq = -1;
  m_AudioMPEGLayer = 0;
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

/* the following function originates from libdvbmpeg: */
void cFemonReceiver::GetVideoInfo(uint8_t *mbuf, int count)
{
  //printf("cFemonReceiver::GetVideoInfo()\n");
  uint8_t *headr;
  int found = 0;
  int c = 0;
  while (found < 4 && c + 4 < count) {
    uint8_t *b;
    b = mbuf + c;
    if (b[0] == 0x00 && b[1] == 0x00 && b[2] == 0x01 && b[3] == 0xb3)
       found = 4;
    else
       c++;
    }
  if (!found) return;
  c += 4;
  if (c + 12 >= count) return;
  headr = mbuf + c;
  m_VideoHorizontalSize = ((headr[1] &0xF0) >> 4) | (headr[0] << 4);
  m_VideoVerticalSize = ((headr[1] &0x0F) << 8) | (headr[2]);
  int sw = (int)((headr[3] & 0xF0) >> 4);
  switch( sw ){
    case 1:
      m_VideoAspectRatio = AR_1_1;
      break;
    case 2:
      m_VideoAspectRatio = AR_4_3;
      break;
    case 3:
      m_VideoAspectRatio = AR_16_9;
      break;
    case 4:
      m_VideoAspectRatio = AR_2_21_1;
      break;
    case 5 ... 15:
      m_VideoAspectRatio = AR_RESERVED;
      break;
    default:
      return;
    }
  sw = (int)(headr[3] & 0x0F);
  switch ( sw ) {
    case 1:
      m_VideoFrameRate = 24000/1001.0;
      m_VideoFormat = VF_UNKNOWN;
      break;
    case 2:
      m_VideoFrameRate = 24.0;
      m_VideoFormat = VF_UNKNOWN;
      break;
    case 3:
      m_VideoFrameRate = 25.0;
      m_VideoFormat = VF_PAL;
      break;
    case 4:
      m_VideoFrameRate = 30000/1001.0;
      m_VideoFormat = VF_NTSC;
      break;
    case 5:
      m_VideoFrameRate = 30.0;
      m_VideoFormat = VF_NTSC;
      break;
    case 6:
      m_VideoFrameRate = 50.0;
      m_VideoFormat = VF_PAL;
      break;
    case 7:
      m_VideoFrameRate = 60.0;
      m_VideoFormat = VF_NTSC;
      break;
    }
  m_VideoStreamBitrate = 400 * (((headr[4] << 10) & 0x0003FC00UL) | ((headr[5] << 2) & 0x000003FCUL) | (((headr[6] & 0xC0) >> 6) & 0x00000003UL)) / 1000000.0;
}

static unsigned int bitrates[3][16] =
{
  {0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 0},
  {0, 32, 48, 56, 64,  80,  96,  112, 128, 160, 192, 224, 256, 320, 384, 0},
  {0, 32, 40, 48, 56,  64,  80,  96,  112, 128, 160, 192, 224, 256, 320, 0}
};

static unsigned int samplerates[4] = {441, 480, 320, 0};

/* the following function originates from libdvbmpeg: */
void cFemonReceiver::GetAudioInfo(uint8_t *mbuf, int count)
{
  //printf("cFemonReceiver::GetAudioInfo()\n");
  uint8_t *headr;
  int found = 0;
  int c = 0;
  int tmp = 0;
  while (!found && c < count) {
    uint8_t *b = mbuf + c;
    if (b[0] == 0xff && (b[1] & 0xf8) == 0xf8)
       found = 1;
    else
       c++;
    }	
  if (!found) return;
  if (c + 3 >= count) return;
  headr = mbuf + c;
  m_AudioMPEGLayer = 4 - ((headr[1] & 0x06) >> 1);
  tmp = bitrates[(3 - ((headr[1] & 0x06) >> 1))][(headr[2] >> 4)] * 1000;
  if (tmp == 0)
     m_AudioStreamBitrate = (double)FR_FREE;
  else if (tmp == 0xf)
     m_AudioStreamBitrate = (double)FR_RESERVED;
  else
     m_AudioStreamBitrate = tmp / 1000.0;
  tmp = samplerates[((headr[2] & 0x0c) >> 2)] * 100;
  if (tmp == 3)
     m_AudioSamplingFreq = FR_RESERVED;
  else
     m_AudioSamplingFreq = tmp;
}

void cFemonReceiver::Activate(bool On)
{
  //printf("cFemonReceiver::Activate()\n");
  Start();
}

void cFemonReceiver::Receive(uchar *Data, int Length)
{
  //printf("cFemonReceiver::Receive()\n");
  // TS packet length: TS_SIZE
  if (Length == TS_SIZE) {
     int pid = ((Data[1] & 0x1f) << 8) | (Data[2]);
     if (pid == m_VideoPid) {
        m_VideoPacketCount++;
        }
     else if (pid == m_AudioPid) {
        m_AudioPacketCount++;
        }
     /* the following originates from libdvbmpeg: */
     if (!(Data[3] & PAYLOAD)) {
        return;
        }
     uint8_t off = 0;
     if (Data[3] & ADAPT_FIELD) {
        off = Data[4] + 1;
        }
     if (Data[1] & PAY_START) {
        uint8_t *sb = Data + 4 + off;
        if (sb[7] & PTS_DTS_FLAGS) {
           uint8_t *pay = sb + sb[8] + 9; 
           int l = TS_SIZE - 13 - off - sb[8];
           if (pid == m_VideoPid) {
              GetVideoInfo(pay, l);
              }
           if (pid == m_AudioPid) {
              GetAudioInfo(pay, l);
              }
           }
        }
     /* end */
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
