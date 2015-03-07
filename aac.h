/*
 * aac.h: Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __FEMON_AAC_H
#define __FEMON_AAC_H

#include "audio.h"

class cFemonAAC {
private:
  cFemonAudioIf *audioHandlerM;

  static int sampleRateS[16];

public:
  cFemonAAC(cFemonAudioIf *audioHandlerP);
  virtual ~cFemonAAC();

  bool processAudio(const uint8_t *bufP, int lenP);
  };

#endif //__FEMON_AAC_H
