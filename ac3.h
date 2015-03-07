/*
 * ac3.h: Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __FEMON_AC3_H
#define __FEMON_AC3_H

#include "audio.h"

class cFemonAC3 {
private:
  cFemonAC3If *audioHandlerM;

  static int bitrateS[32];
  static int frequencieS[4];
  static int frameS[3][32];

public:
  cFemonAC3(cFemonAC3If *audioHandlerP);
  virtual ~cFemonAC3();

  bool processAudio(const uint8_t *bufP, int lenP);
  };

#endif //__FEMON_AC3_H
