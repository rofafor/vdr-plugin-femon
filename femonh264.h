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
  unsigned int m_Width;
  unsigned int m_Height;
  eVideoAspectRatio m_AspectRatio;
  eVideoFormat m_Format;
  double m_FrameRate;
  double m_BitRate;
  eVideoScan m_Scan;
  bool m_CpbDpbDelaysPresentFlag;
  bool m_PicStructPresentFlag;
  unsigned int m_TimeOffsetLength;

  void reset();
  const uint8_t *nextStartCode(const uint8_t *start, const uint8_t *end);
  int nalUnescape(uint8_t *dst, const uint8_t *src, int len);
  int parseSPS(const uint8_t *buf, int len);
  int parseSEI(const uint8_t *buf, int len);

  static const eVideoAspectRatio s_AspectRatios[];
  static const eVideoFormat s_VideoFormats[];
  static const uint8_t s_SeiNumClockTsTable[9];

public:
  cFemonH264(cFemonVideoIf *videohandler);
  virtual ~cFemonH264();

  bool processVideo(const uint8_t *buf, int len);
  };

#endif //__FEMONH264_H
