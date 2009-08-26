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
  enum {
    NAL_SEI     = 0x06, // Supplemental Enhancement Information
    NAL_SPS     = 0x07, // Sequence Parameter Set
    NAL_AUD     = 0x09, // Access Unit Delimiter
    NAL_END_SEQ = 0x0A  // End of Sequence
  };

  cFemonVideoIf *m_VideoHandler;

  const uint8_t *nextStartCode(const uint8_t *start, const uint8_t *end);
  int nalUnescape(uint8_t *dst, const uint8_t *src, int len);
  int parseSPS(const uint8_t *buf, int len);
  int parseSEI(const uint8_t *buf, int len);

  static const eVideoAspectRatio s_AspectRatios[];
  static const eVideoFormat s_VideoFormats[];

public:
  cFemonH264(cFemonVideoIf *videohandler);
  virtual ~cFemonH264();

  bool processVideo(const uint8_t *buf, int len);
  };

#endif //__FEMONH264_H
