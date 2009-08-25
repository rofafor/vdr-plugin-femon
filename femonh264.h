/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __FEMONH264_H
#define __FEMONH264_H

#include "femonvideo.h"

class cFemonH264 {
private:
  cFemonVideoIf *m_VideoHandler;

public:
  cFemonH264(cFemonVideoIf *videohandler);
  virtual ~cFemonH264();

  bool processVideo(const uint8_t *buf, int len);
  };

#endif //__FEMONH264_H
