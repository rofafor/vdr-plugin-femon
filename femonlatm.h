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
  cFemonAudioIf *m_AudioHandler;

  static unsigned int s_Bitrates[3][16];
  static unsigned int s_Samplerates[4];

public:
  cFemonLATM(cFemonAudioIf *audiohandler);
  virtual ~cFemonLATM();

  bool processAudio(const uint8_t *buf, int len);
  };

#endif //__FEMONLATM_H
