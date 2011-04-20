/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __FEMONSYMBOL_H
#define __FEMONSYMBOL_H

#include <vector>
#include <vdr/osd.h>

enum eSymbols {
  SYMBOL_STEREO,
  SYMBOL_MONO_LEFT,
  SYMBOL_MONO_RIGHT,
  SYMBOL_DD,
  SYMBOL_DD20,
  SYMBOL_DD51,
  SYMBOL_MPEG2,
  SYMBOL_H264,
  SYMBOL_PAL,
  SYMBOL_NTSC,
  SYMBOL_ENCRYPTED,
  SYMBOL_SVDRP,
  SYMBOL_LOCK,
  SYMBOL_SIGNAL,
  SYMBOL_CARRIER,
  SYMBOL_VITERBI,
  SYMBOL_SYNC,
  SYMBOL_AR_1_1,
  SYMBOL_AR_16_9,
  SYMBOL_AR_2_21_1,
  SYMBOL_AR_4_3,
  SYMBOL_DEVICE,
  SYMBOL_ZERO,
  SYMBOL_ONE,
  SYMBOL_TWO,
  SYMBOL_THREE,
  SYMBOL_FOUR,
  SYMBOL_FIVE,
  SYMBOL_SIX,
  SYMBOL_SEVEN,
  SYMBOL_EIGHT,
  SYMBOL_FORMAT_1080,
  SYMBOL_FORMAT_720,
  SYMBOL_FORMAT_576,
  SYMBOL_FORMAT_480,
  SYMBOL_MAX_COUNT
  };

class cFemonSymbolCache {
private:
  enum {
    DEFAULT_SPACING  = 5,
    DEFAULT_ROUNDING = 10,
    DEFAULT_HEIGHT   = 576,
    DEFAULT_WIDTH    = 720
  };
  double xFactorM;
  double yFactorM;
  bool antiAliasM;
  std::vector<cBitmap*> cacheM;
  bool Populate(void);
  bool Flush(void);
public:
  cFemonSymbolCache();
  ~cFemonSymbolCache();
  void Refresh();
  cBitmap& Get(unsigned int symbolP);
  int GetSpacing()  { return yFactorM * DEFAULT_SPACING; }
  int GetRounding() { return yFactorM * DEFAULT_ROUNDING; }
};

extern cFemonSymbolCache femonSymbols;

#endif // __FEMONSYMBOL_H
