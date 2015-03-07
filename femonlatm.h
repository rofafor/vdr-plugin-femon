/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __FEMONLATM_H
#define __FEMONLATM_H

#include "femonaudio.h"

class cFemonLATM {
private:
  cFemonAudioIf *audioHandlerM;

  static int bitrateS[3][16];
  static int sampleRateS[4];

public:
  cFemonLATM(cFemonAudioIf *audioHandlerP);
  virtual ~cFemonLATM();

  bool processAudio(const uint8_t *bufP, int lenP);
  };

#endif //__FEMONLATM_H
