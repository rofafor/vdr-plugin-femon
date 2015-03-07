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

#include "femonh264.h"
#include "femonmpeg.h"
#include "femonaac.h"
#include "femonlatm.h"
#include "femonac3.h"
#include "femonaudio.h"
#include "femonvideo.h"
#include "femontools.h"

class cFemonReceiver : public cReceiver, public cThread, public cFemonVideoIf, public cFemonAudioIf, public cFemonAC3If {
private:
  cMutex            mutexM;
  cCondWait         sleepM;
  bool              activeM;

  cFemonH264        detectH264M;
  cFemonMPEG        detectMpegM;
  cFemonAAC         detectAacM;
  cFemonLATM        detectLatmM;
  cFemonAC3         detectAc3M;

  cRingBufferLinear videoBufferM;
  cTsToPes          videoAssemblerM;
  int               videoTypeM;
  int               videoPidM;
  int               videoPacketCountM;
  double            videoBitRateM;
  bool              videoValidM;
  video_info_t      videoInfoM;

  cRingBufferLinear audioBufferM;
  cTsToPes          audioAssemblerM;
  int               audioPidM;
  int               audioPacketCountM;
  double            audioBitRateM;
  bool              audioValidM;
  audio_info_t      audioInfoM;

  cRingBufferLinear ac3BufferM;
  cTsToPes          ac3AssemblerM;
  int               ac3PidM;
  int               ac3PacketCountM;
  double            ac3BitRateM;
  bool              ac3ValidM;
  ac3_info_t        ac3InfoM;

protected:
  virtual void Activate(bool onP);
  virtual void Receive(uchar *dataP, int lengthP);
  virtual void Action(void);

public:
  virtual void SetVideoCodec(eVideoCodec codecP)                   { cMutexLock MutexLock(&mutexM);
                                                                     videoInfoM.codec = codecP; }
  virtual void SetVideoFormat(eVideoFormat formatP)                { cMutexLock MutexLock(&mutexM);
                                                                     videoInfoM.format = formatP; }
  virtual void SetVideoScan(eVideoScan scanP)                      { cMutexLock MutexLock(&mutexM);
                                                                     videoInfoM.scan = scanP; }
  virtual void SetVideoAspectRatio(eVideoAspectRatio aspectRatioP) { cMutexLock MutexLock(&mutexM);
                                                                     videoInfoM.aspectRatio = aspectRatioP; }
  virtual void SetVideoSize(int widthP, int heightP)               { cMutexLock MutexLock(&mutexM);
                                                                     videoInfoM.width  = widthP;
                                                                     videoInfoM.height = heightP; }
  virtual void SetVideoFramerate(double frameRateP)                { cMutexLock MutexLock(&mutexM);
                                                                     videoInfoM.frameRate = frameRateP; }
  virtual void SetVideoBitrate(double bitRateP)                    { cMutexLock MutexLock(&mutexM);
                                                                     videoInfoM.bitrate = bitRateP; }

  virtual void SetAudioCodec(eAudioCodec codecP)        { cMutexLock MutexLock(&mutexM);
                                                          audioInfoM.codec = codecP; }
  virtual void SetAudioBitrate(double bitRateP)         { cMutexLock MutexLock(&mutexM);
                                                          audioInfoM.bitrate = bitRateP; }
  virtual void SetAudioSamplingFrequency(int samplingP) { cMutexLock MutexLock(&mutexM);
                                                          audioInfoM.samplingFrequency = samplingP; }
  virtual void SetAudioChannel(eAudioChannelMode modeP) { cMutexLock MutexLock(&mutexM);
                                                          audioInfoM.channelMode = modeP; }

  virtual void SetAC3Bitrate(int bitRateP)            { cMutexLock MutexLock(&mutexM);
                                                        ac3InfoM.bitrate = bitRateP; }
  virtual void SetAC3SamplingFrequency(int samplingP) { cMutexLock MutexLock(&mutexM);
                                                        ac3InfoM.samplingFrequency = samplingP; }
  virtual void SetAC3Bitstream(int modeP)             { cMutexLock MutexLock(&mutexM);
                                                        ac3InfoM.bitstreamMode = modeP; }
  virtual void SetAC3AudioCoding(int modeP)           { cMutexLock MutexLock(&mutexM);
                                                        ac3InfoM.audioCodingMode = modeP; }
  virtual void SetAC3DolbySurround(int modeP)         { cMutexLock MutexLock(&mutexM);
                                                        ac3InfoM.dolbySurroundMode = modeP; }
  virtual void SetAC3CenterMix(int levelP)            { cMutexLock MutexLock(&mutexM);
                                                        ac3InfoM.centerMixLevel = levelP; }
  virtual void SetAC3SurroundMix(int levelP)          { cMutexLock MutexLock(&mutexM);
                                                        ac3InfoM.surroundMixLevel = levelP; }
  virtual void SetAC3Dialog(int levelP)               { cMutexLock MutexLock(&mutexM);
                                                        ac3InfoM.dialogLevel = levelP; }
  virtual void SetAC3LFE(bool onoffP)                 { cMutexLock MutexLock(&mutexM);
                                                        ac3InfoM.lfe = onoffP; }

public:
  cFemonReceiver(const cChannel* channelP, int aTrackp, int dTrackP);
  virtual ~cFemonReceiver();
  void Deactivate(void);

  bool   VideoValid(void)           { cMutexLock MutexLock(&mutexM);
                                      return videoValidM; };                  // boolean
  double VideoBitrate(void)         { cMutexLock MutexLock(&mutexM);
                                      return videoBitRateM; };                // bit/s
  int    VideoCodec(void)           { cMutexLock MutexLock(&mutexM);
                                      return videoInfoM.codec; };             // eVideoCodec
  int    VideoFormat(void)          { cMutexLock MutexLock(&mutexM);
                                      return videoInfoM.format; };            // eVideoFormat
  int    VideoScan(void)            { cMutexLock MutexLock(&mutexM);
                                      return videoInfoM.scan; };              // eVideoScan
  int    VideoAspectRatio(void)     { cMutexLock MutexLock(&mutexM);
                                      return videoInfoM.aspectRatio; };       // eVideoAspectRatio
  int    VideoHorizontalSize(void)  { cMutexLock MutexLock(&mutexM);
                                      return videoInfoM.width; };             // pixels
  int    VideoVerticalSize(void)    { cMutexLock MutexLock(&mutexM);
                                      return videoInfoM.height; };            // pixels
  double VideoFrameRate(void)       { cMutexLock MutexLock(&mutexM);
                                      return videoInfoM.frameRate; };         // Hz
  double VideoStreamBitrate(void)   { cMutexLock MutexLock(&mutexM);
                                      return videoInfoM.bitrate; };           // bit/s

  bool   AudioValid(void)           { cMutexLock MutexLock(&mutexM);
                                      return audioValidM; };                  // boolean
  double AudioBitrate(void)         { cMutexLock MutexLock(&mutexM);
                                      return audioBitRateM; };                // bit/s
  int    AudioCodec(void)           { cMutexLock MutexLock(&mutexM);
                                      return audioInfoM.codec; };             // eAudioCodec
  int    AudioChannelMode(void)     { cMutexLock MutexLock(&mutexM);
                                      return audioInfoM.channelMode; };       // eAudioChannelMode
  double AudioStreamBitrate(void)   { cMutexLock MutexLock(&mutexM);
                                      return audioInfoM.bitrate; };           // bit/s or eAudioBitrate
  int    AudioSamplingFreq(void)    { cMutexLock MutexLock(&mutexM);
                                      return audioInfoM.samplingFrequency; }; // Hz or eAudioSamplingFrequency

  bool   AC3Valid(void)             { cMutexLock MutexLock(&mutexM);
                                      return ac3ValidM; };                    // boolean
  double AC3Bitrate(void)           { cMutexLock MutexLock(&mutexM);
                                      return ac3BitRateM; };                  // bit/s
  double AC3StreamBitrate(void)     { cMutexLock MutexLock(&mutexM);
                                      return ac3InfoM.bitrate; };             // bit/s or eAudioBitrate
  int    AC3SamplingFreq(void)      { cMutexLock MutexLock(&mutexM);
                                      return ac3InfoM.samplingFrequency; };   // Hz or eAudioSamplingFrequency
  int    AC3BitStreamMode(void)     { cMutexLock MutexLock(&mutexM);
                                      return ac3InfoM.bitstreamMode; };       // 0..7 or eAudioBitstreamMode
  int    AC3AudioCodingMode(void)   { cMutexLock MutexLock(&mutexM);
                                      return ac3InfoM.audioCodingMode; };     // 0..7 or eAudioCodingMode
  bool   AC3_2_0(void)              { cMutexLock MutexLock(&mutexM);
                                      return (ac3InfoM.audioCodingMode == AUDIO_CODING_MODE_2_0); }; // boolean
  bool   AC3_5_1(void)	            { cMutexLock MutexLock(&mutexM);
                                      return (ac3InfoM.audioCodingMode == AUDIO_CODING_MODE_3_2); }; // boolean
  int    AC3DolbySurroundMode(void) { cMutexLock MutexLock(&mutexM);
                                      return ac3InfoM.dolbySurroundMode; };   // eAudioDolbySurroundMode
  int    AC3CenterMixLevel(void)    { cMutexLock MutexLock(&mutexM);
                                      return ac3InfoM.centerMixLevel; };      // eAudioCenterMixLevel
  int    AC3SurroundMixLevel(void)  { cMutexLock MutexLock(&mutexM);
                                      return ac3InfoM.surroundMixLevel; };    // eAudioSurroundMixLevel
  int    AC3DialogLevel(void)       { cMutexLock MutexLock(&mutexM);
                                      return ac3InfoM.dialogLevel; };         // -dB
  bool   AC3Lfe(void)               { cMutexLock MutexLock(&mutexM);
                                      return ac3InfoM.lfe; };                 // boolean
  };

#endif //__FEMONRECEIVER_H

