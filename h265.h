/*
 * h265.h: Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __FEMON_H265_H
#define __FEMON_H265_H

#include "video.h"

class cFemonH265 {
private:
  enum {
    NAL_VPS = 32, // Video Parameter Set
    NAL_SPS = 33, // Sequence Parameter Set
    NAL_PPS = 34, // Picture Parameter Set
    NAL_AUD = 35, // Access Unit Delimiter
    NAL_EOS = 36, // End of Sequence
    NAL_EOB = 37, // End of Bitstream
  };

  cFemonVideoIf    *videoHandlerM;
  uint32_t          widthM;
  uint32_t          heightM;
  eVideoAspectRatio aspectRatioM;
  eVideoFormat      formatM;
  double            frameRateM;
  double            bitRateM;
  eVideoScan        scanM;

  void           reset();
  const uint8_t *nextStartCode(const uint8_t *start, const uint8_t *end);
  int            nalUnescape(uint8_t *dst, const uint8_t *src, int len);

public:
  cFemonH265(cFemonVideoIf *videoHandlerP);
  virtual ~cFemonH265();

  bool processVideo(const uint8_t *bufP, int lenP);
  };

#endif //__FEMON_H265_H
