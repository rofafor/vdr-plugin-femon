/*
 * mpeg.h: Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __FEMON_MPEG_H
#define __FEMON_MPEG_H

#include "video.h"
#include "audio.h"

class cFemonMPEG {
private:
  cFemonVideoIf *videoHandlerM;
  cFemonAudioIf *audioHandlerM;

  static int bitrateS[2][3][16];
  static int sampleRateS[2][4];
  static eAudioCodec formatS[2][4];

public:
  cFemonMPEG(cFemonVideoIf *videoHandlerP, cFemonAudioIf *audioHandlerP);
  virtual ~cFemonMPEG();

  bool processVideo(const uint8_t *bufP, int lenP);
  bool processAudio(const uint8_t *bufP, int lenP);
  };

#endif //__FEMON_MPEG_H
