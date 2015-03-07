/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __FEMONAC3_H
#define __FEMONAC3_H

#include "femonaudio.h"

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

#endif //__FEMONAC3_H
