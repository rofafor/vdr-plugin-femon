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
  int m_VideoHorizontalSize;
  int m_VideoVerticalSize;
  int m_VideoAspectRatio;
  int m_VideoFormat;
  double m_VideoFrameRate;
  double m_VideoStreamBitrate;
  double m_VideoBitrate;
  int m_AudioPacketCount;
  double m_AudioStreamBitrate;
  double m_AudioBitrate;
  int m_AudioSamplingFreq;
  int m_AudioMPEGLayer;
  void GetVideoInfo(uint8_t *mbuf, int count);
  void GetAudioInfo(uint8_t *mbuf, int count);

protected:
  virtual void Activate(bool On);
  virtual void Receive(uchar *Data, int Length);
  virtual void Action(void);

public:
  cFemonReceiver(int Ca, int Vpid, int Apid);
  virtual ~cFemonReceiver();

  int VideoHorizontalSize(void)   { return m_VideoHorizontalSize; }; // pixels
  int VideoVerticalSize(void)     { return m_VideoVerticalSize; };   // pixels
  int VideoAspectRatio(void)      { return m_VideoAspectRatio; };    // 4:3 == 133, ...
  int VideoFormat(void)           { return m_VideoFormat; };         // 0 == unknown, 1 == PAL, 2 == NTSC
  double VideoFrameRate(void)     { return m_VideoFrameRate; };      // Hz
  double VideoStreamBitrate(void) { return m_VideoStreamBitrate; };  // Mbit/s
  double VideoBitrate(void)       { return m_VideoBitrate; };        // Mbit/s

  int AudioMPEGLayer(void)        { return m_AudioMPEGLayer; };      // number
  int AudioSamplingFreq(void)     { return m_AudioSamplingFreq; };   // Hz
  double AudioStreamBitrate(void) { return m_AudioStreamBitrate; };  // kbit/s
  double AudioBitrate(void)       { return m_AudioBitrate; };        // kbit/s
  };

#endif //__FEMONRECEIVER_H

