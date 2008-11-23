/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __FEMONRECEIVER_H
#define __FEMONRECEIVER_H

#include <vdr/thread.h>
#include <vdr/receiver.h>

#include "femonaudio.h"
#include "femonvideo.h"

class cFemonReceiver : public cReceiver, public cThread {
private:
  cCondWait     m_Sleep;
  bool          m_Active;

  int           m_VideoPid;
  int           m_VideoPacketCount;
  double        m_VideoBitrate;
  bool          m_VideoValid;
  video_info_t  m_VideoInfo;
  video_info_t  m_VideoInfoBuffer[3];
  unsigned int  m_VideoInfoBufferIndex;

  int           m_AudioPid;
  int           m_AudioPacketCount;
  double        m_AudioBitrate;
  bool          m_AudioValid;
  audio_info_t  m_AudioInfo;
  audio_info_t  m_AudioInfoBuffer[3];
  unsigned int  m_AudioInfoBufferIndex;

  int           m_AC3Pid;
  int           m_AC3PacketCount;
  double        m_AC3Bitrate;
  bool          m_AC3Valid;
  ac3_info_t    m_AC3Info;
  ac3_info_t    m_AC3InfoBuffer[3];
  unsigned int  m_AC3InfoBufferIndex;

  void          GetVideoInfo(uint8_t *buf, int len);
  void          GetAudioInfo(uint8_t *buf, int len);
  void          GetAC3Info(uint8_t *buf, int len);

protected:
  virtual void Activate(bool On);
  virtual void Receive(uchar *Data, int Length);
  virtual void Action(void);

public:
  cFemonReceiver(tChannelID ChannelID, int Ca, int Vpid, int Apid[], int Dpid[]);
  virtual ~cFemonReceiver();
  void Deactivate(void);

  bool   VideoValid(void)           { return m_VideoValid; };                  // boolean
  double VideoBitrate(void)         { return m_VideoBitrate; };                // bit/s
  int    VideoCodec(void)           { return m_VideoInfo.codec; };             // eVideoCodec
  int    VideoFormat(void)          { return m_VideoInfo.format; };            // eVideoFormat
  int    VideoScan(void)            { return m_VideoInfo.scan; };              // eVideoScan
  int    VideoAspectRatio(void)     { return m_VideoInfo.aspectRatio; };       // eVideoAspectRatio
  int    VideoHorizontalSize(void)  { return m_VideoInfo.width; };             // pixels
  int    VideoVerticalSize(void)    { return m_VideoInfo.height; };            // pixels
  double VideoFrameRate(void)       { return m_VideoInfo.frameRate; };         // Hz
  double VideoStreamBitrate(void)   { return m_VideoInfo.bitrate; };           // bit/s

  bool   AudioValid(void)           { return m_AudioValid; };                  // boolean
  double AudioBitrate(void)         { return m_AudioBitrate; };                // bit/s
  int    AudioCodec(void)           { return m_AudioInfo.codec; };             // eAudioCodec
  int    AudioChannelMode(void)     { return m_AudioInfo.channelMode; };       // eAudioChannelMode
  double AudioStreamBitrate(void)   { return m_AudioInfo.bitrate; };           // bit/s or eAudioBitrate
  int    AudioSamplingFreq(void)    { return m_AudioInfo.samplingFrequency; }; // Hz or eAudioSamplingFrequency

  bool   AC3Valid(void)             { return m_AC3Valid; };                    // boolean
  double AC3Bitrate(void)           { return m_AC3Bitrate; };                  // bit/s
  double AC3StreamBitrate(void)     { return m_AC3Info.bitrate; };             // bit/s or eAudioBitrate
  int    AC3SamplingFreq(void)      { return m_AC3Info.samplingFrequency; };   // Hz or eAudioSamplingFrequency
  int    AC3BitStreamMode(void)     { return m_AC3Info.bitstreamMode; };       // 0..7 or eAudioBitstreamMode
  int    AC3AudioCodingMode(void)   { return m_AC3Info.audioCodingMode; };     // 0..7 or eAudioCodingMode
  bool   AC3_2_0(void)              { return m_AC3Info.audioCodingMode == AUDIO_CODING_MODE_2_0; }; // boolean
  bool   AC3_5_1(void)	            { return m_AC3Info.audioCodingMode == AUDIO_CODING_MODE_3_2; }; // boolean
  int    AC3DolbySurroundMode(void) { return m_AC3Info.dolbySurroundMode; };   // eAudioDolbySurroundMode
  int    AC3CenterMixLevel(void)    { return m_AC3Info.centerMixLevel; };      // eAudioCenterMixLevel
  int    AC3SurroundMixLevel(void)  { return m_AC3Info.surroundMixLevel; };    // eAudioSurroundMixLevel
  int    AC3DialogLevel(void)       { return m_AC3Info.dialogLevel; };         // -dB
  bool   AC3Lfe(void)               { return m_AC3Info.lfe; };                 // boolean
  };

#endif //__FEMONRECEIVER_H

