/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include <unistd.h>
#include "femontools.h"
#include "femoncfg.h"
#include "femonmpeg.h"
#include "femonaac.h"
#include "femonac3.h"
#include "femonh264.h"
#include "femonreceiver.h"

#define TS_SIZE       188
#define PAY_START     0x40
#define ADAPT_FIELD   0x20
#define PAYLOAD       0x10
#define PTS_DTS_FLAGS 0xC0

cFemonReceiver::cFemonReceiver(tChannelID ChannelID, int Ca, int Vpid, int Apid[], int Dpid[])
: cReceiver(ChannelID, -1, Vpid, Apid, Dpid, NULL),
  cThread("femon receiver"),
  m_Sleep(),
  m_Active(false),
  m_VideoPid(Vpid),
  m_VideoPacketCount(0),
  m_VideoBitrate(0.0),
  m_VideoValid(false),
  m_VideoInfoBufferIndex(0),
  m_AudioPid(Apid[0]),
  m_AudioPacketCount(0),
  m_AudioBitrate(0.0),
  m_AudioValid(false),
  m_AudioInfoBufferIndex(0),
  m_AC3Pid(Dpid[0]),
  m_AC3PacketCount(0), 
  m_AC3Bitrate(0),
  m_AC3Valid(false),
  m_AC3InfoBufferIndex(0)
{
  Dprintf("%s()\n", __PRETTY_FUNCTION__);

  m_VideoInfo.codec = VIDEO_CODEC_INVALID;
  m_VideoInfo.format = VIDEO_FORMAT_INVALID;
  m_VideoInfo.scan = VIDEO_SCAN_INVALID;
  m_VideoInfo.aspectRatio = VIDEO_ASPECT_RATIO_INVALID;
  m_VideoInfo.width = 0;
  m_VideoInfo.height = 0;
  m_VideoInfo.frameRate = 0;
  m_VideoInfo.bitrate = AUDIO_BITRATE_INVALID;
  for (unsigned int i = 0; i < ELEMENTS(m_VideoInfoBuffer); ++i)
      memcpy(&m_VideoInfoBuffer[i], &m_VideoInfo, sizeof(video_info_t));

  m_AudioInfo.codec = AUDIO_CODEC_UNKNOWN;
  m_AudioInfo.bitrate = AUDIO_BITRATE_INVALID;
  m_AudioInfo.samplingFrequency = AUDIO_SAMPLING_FREQUENCY_INVALID;
  m_AudioInfo.channelMode = AUDIO_CHANNEL_MODE_INVALID;
  for (unsigned int i = 0; i < ELEMENTS(m_AudioInfoBuffer); ++i)
      memcpy(&m_AudioInfoBuffer[i], &m_AudioInfo, sizeof(audio_info_t));

  m_AC3Info.bitrate = AUDIO_BITRATE_INVALID;
  m_AC3Info.samplingFrequency = AUDIO_SAMPLING_FREQUENCY_INVALID;
  m_AC3Info.bitstreamMode = AUDIO_BITSTREAM_MODE_INVALID;
  m_AC3Info.audioCodingMode = AUDIO_CODING_MODE_INVALID;
  m_AC3Info.dolbySurroundMode = AUDIO_DOLBY_SURROUND_MODE_INVALID;
  m_AC3Info.centerMixLevel = AUDIO_CENTER_MIX_LEVEL_INVALID;
  m_AC3Info.surroundMixLevel = AUDIO_SURROUND_MIX_LEVEL_INVALID;
  m_AC3Info.dialogLevel = 0;
  m_AC3Info.lfe = false;
  for (unsigned int i = 0; i < ELEMENTS(m_AC3InfoBuffer); ++i)
      memcpy(&m_AC3InfoBuffer[i], &m_AC3Info, sizeof(ac3_info_t));
}

cFemonReceiver::~cFemonReceiver(void)
{
  Dprintf("%s()\n", __PRETTY_FUNCTION__);
  Deactivate();
}

void cFemonReceiver::Deactivate(void)
{
  Dprintf("%s()\n", __PRETTY_FUNCTION__);
  if (m_Active) {
     m_Active = false;
     m_Sleep.Signal();
     if (Running())
        Cancel(3);
     Detach();
     }
}

void cFemonReceiver::GetVideoInfo(uint8_t *buf, int len)
{
  int c = 0;

  while (c < len) {
    video_info_t tmp;
    uint8_t *b = buf + c;
    if (getMPEGVideoInfo(b, len - c, &tmp) || getH264VideoInfo(b, len - c, &tmp)) {
       bool coherent = true;
       memcpy(&m_VideoInfoBuffer[m_VideoInfoBufferIndex], &tmp, sizeof(video_info_t));
       m_VideoInfoBufferIndex = (m_VideoInfoBufferIndex + 1) % ELEMENTS(m_VideoInfoBuffer);
       for (unsigned int i = 1; i < ELEMENTS(m_VideoInfoBuffer); ++i) {
           if (memcmp(&m_VideoInfoBuffer[0], &m_VideoInfoBuffer[i], sizeof(video_info_t)))
              coherent = false;
              break;
           }
       if (!m_VideoValid || coherent) {
          m_VideoValid = true;
          memcpy(&m_VideoInfo, &m_VideoInfoBuffer[0], sizeof(video_info_t));
          }
       return;
       }
    c++;
    }
}

void cFemonReceiver::GetAudioInfo(uint8_t *buf, int len)
{
  int c = 0;

  while (c < len) {
    audio_info_t tmp;
    uint8_t *b = buf + c;
    if (getAACAudioInfo(b, len - c, &tmp) || getMPEGAudioInfo(b, len - c, &tmp)) {
       bool coherent = true;
       memcpy(&m_AudioInfoBuffer[m_AudioInfoBufferIndex], &tmp, sizeof(audio_info_t));
       m_AudioInfoBufferIndex = (m_AudioInfoBufferIndex + 1) % ELEMENTS(m_AudioInfoBuffer);
       for (unsigned int i = 1; i < ELEMENTS(m_AudioInfoBuffer); ++i) {
           if (memcmp(&m_AudioInfoBuffer[0], &m_AudioInfoBuffer[i], sizeof(audio_info_t)))
              coherent = false;
              break;
           }
       if (!m_AudioValid || coherent) {
          m_AudioValid = true;
          memcpy(&m_AudioInfo, &m_AudioInfoBuffer[0], sizeof(audio_info_t));
          }
       return;
       }
    c++;
    }	
}

void cFemonReceiver::GetAC3Info(uint8_t *buf, int len)
{
  int c = 0;

  while (c < len) {
    ac3_info_t tmp;
    uint8_t *b = buf + c;
    if (getAC3AudioInfo(b, len - c, &tmp)) {
       bool coherent = true;
       memcpy(&m_AC3InfoBuffer[m_AC3InfoBufferIndex], &tmp, sizeof(ac3_info_t));
       m_AC3InfoBufferIndex = (m_AC3InfoBufferIndex + 1) % ELEMENTS(m_AC3InfoBuffer);
       for (unsigned int i = 1; i < ELEMENTS(m_AC3InfoBuffer); ++i) {
           if (memcmp(&m_AC3InfoBuffer[0], &m_AC3InfoBuffer[i], sizeof(ac3_info_t)))
              coherent = false;
              break;
           }
       if (!m_AC3Valid || coherent) {
          m_AC3Valid = true;
          memcpy(&m_AC3Info, &m_AC3InfoBuffer[0], sizeof(ac3_info_t));
          }   
       return;
       }
    c++;
    }
}

void cFemonReceiver::Activate(bool On)
{
  Dprintf("%s(%d)\n", __PRETTY_FUNCTION__, On);
  if (On)
     Start();
  else
     Deactivate();
}

void cFemonReceiver::Receive(uchar *Data, int Length)
{
  // TS packet length: TS_SIZE
  if (Length == TS_SIZE) {
     int pid = ((Data[1] & 0x1f) << 8) | (Data[2]);
     if (pid == m_VideoPid) {
        m_VideoPacketCount++;
        }
     else if (pid == m_AudioPid) {
        m_AudioPacketCount++;
        }
     else if (pid == m_AC3Pid) {
        m_AC3PacketCount++;
        }
     /* the following originates from libdvbmpeg: */
     if (!(Data[3] & PAYLOAD)) {
        return;
        }
     uint8_t off = 0;
     if (Data[3] & ADAPT_FIELD) {
        off = (uint8_t)(Data[4] + 1);
        }
     if (Data[1] & PAY_START) {
        uint8_t *sb = Data + 4 + off;
        if (sb[7] & PTS_DTS_FLAGS) {
           uint8_t *pay = sb + sb[8] + 9; 
           int l = TS_SIZE - 13 - off - sb[8];
           if (pid == m_VideoPid) {
              GetVideoInfo(pay, l);
              }
           else if (pid == m_AudioPid) {
              GetAudioInfo(pay, l);
              }
           else if (pid == m_AC3Pid) {
              GetAC3Info(pay, l);
              }
           }
        }
     /* end */
     }
}

void cFemonReceiver::Action(void)
{
  Dprintf("%s()\n", __PRETTY_FUNCTION__);
  cTimeMs t;
  m_Active = true;
  while (Running() && m_Active) {
        t.Set(0);
        // TS packet 188 bytes - 4 byte header; MPEG standard defines 1Mbit = 1000000bit
        m_VideoBitrate = (10.0 * 8.0 * 184.0 * m_VideoPacketCount) / femonConfig.calcinterval;
        m_VideoPacketCount = 0;
        m_AudioBitrate = (10.0 * 8.0 * 184.0 * m_AudioPacketCount) / femonConfig.calcinterval;
        m_AudioPacketCount = 0;
        m_AC3Bitrate   = (10.0 * 8.0 * 184.0 * m_AC3PacketCount)   / femonConfig.calcinterval;
        m_AC3PacketCount = 0;
        m_Sleep.Wait(max((int)(100 * femonConfig.calcinterval - t.Elapsed()), 3));
    }
}
