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

  typedef struct DAR {
    eVideoAspectRatio dar;
    int               ratio;
  } t_DAR;

  typedef struct SAR {
    int               w;
    int               h;
  } t_SAR;

  cFemonVideoIf    *videoHandlerM;
  uint32_t          widthM;
  uint32_t          heightM;
  eVideoAspectRatio aspectRatioM;
  eVideoFormat      formatM;
  double            frameRateM;
  double            bitRateM;
  eVideoScan        scanM;
  bool              cpbDpbDelaysPresentFlagM;
  bool              picStructPresentFlagM;
  bool              frameMbsOnlyFlagM;
  bool              mbAdaptiveFrameFieldFlagM;
  uint32_t          timeOffsetLengthM;

  void           reset();
  const uint8_t *nextStartCode(const uint8_t *start, const uint8_t *end);
  int            nalUnescape(uint8_t *dst, const uint8_t *src, int len);
  int            parseSPS(const uint8_t *buf, int len);
  int            parseSEI(const uint8_t *buf, int len);

  static const t_SAR             sarS[];
  static const t_DAR             darS[];
  static const eVideoFormat      videoFormatS[];
  static const uint8_t           seiNumClockTsTableS[9];

public:
  cFemonH264(cFemonVideoIf *videoHandlerP);
  virtual ~cFemonH264();

  bool processVideo(const uint8_t *bufP, int lenP);
  };

#endif //__FEMONH264_H
