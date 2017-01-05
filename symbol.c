/*
 * symbol.c: Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include <vdr/device.h>

#include "log.h"
#include "tools.h"
#include "symbol.h"

#include "symbols/stereo.xpm"
#include "symbols/monoleft.xpm"
#include "symbols/monoright.xpm"
#include "symbols/dolbydigital.xpm"
#include "symbols/dolbydigital20.xpm"
#include "symbols/dolbydigital51.xpm"
#include "symbols/mpeg2.xpm"
#include "symbols/h264.xpm"
#include "symbols/h265.xpm"
#include "symbols/ntsc.xpm"
#include "symbols/pal.xpm"
#include "symbols/encrypted.xpm"
#include "symbols/svdrp.xpm"
#include "symbols/lock.xpm"
#include "symbols/signal.xpm"
#include "symbols/carrier.xpm"
#include "symbols/viterbi.xpm"
#include "symbols/sync.xpm"
#include "symbols/ar11.xpm"
#include "symbols/ar169.xpm"
#include "symbols/ar2211.xpm"
#include "symbols/ar43.xpm"
#include "symbols/device.xpm"
#include "symbols/zero.xpm"
#include "symbols/one.xpm"
#include "symbols/two.xpm"
#include "symbols/three.xpm"
#include "symbols/four.xpm"
#include "symbols/five.xpm"
#include "symbols/six.xpm"
#include "symbols/seven.xpm"
#include "symbols/eight.xpm"
#include "symbols/format1080.xpm"
#include "symbols/format1080i.xpm"
#include "symbols/format1080p.xpm"
#include "symbols/format720.xpm"
#include "symbols/format720i.xpm"
#include "symbols/format720p.xpm"
#include "symbols/format576.xpm"
#include "symbols/format576i.xpm"
#include "symbols/format576p.xpm"
#include "symbols/format480.xpm"
#include "symbols/format480i.xpm"
#include "symbols/format480p.xpm"

static cBitmap bmOnePixel(1, 1, 1);
static cBitmap bmStereo(stereo_xpm);
static cBitmap bmMonoLeft(monoleft_xpm);
static cBitmap bmMonoRight(monoright_xpm);
static cBitmap bmDolbyDigital(dolbydigital_xpm);
static cBitmap bmDolbyDigital20(dolbydigital20_xpm);
static cBitmap bmDolbyDigital51(dolbydigital51_xpm);
static cBitmap bmMpeg2(mpeg2_xpm);
static cBitmap bmH264(h264_xpm);
static cBitmap bmH265(h265_xpm);
static cBitmap bmPal(pal_xpm);
static cBitmap bmNtsc(ntsc_xpm);
static cBitmap bmEncrypted(encrypted_xpm);
static cBitmap bmSvdrp(svdrp_xpm);
static cBitmap bmLock(lock_xpm);
static cBitmap bmSignal(signal_xpm);
static cBitmap bmCarrier(carrier_xpm);
static cBitmap bmViterbi(viterbi_xpm);
static cBitmap bmSync(sync_xpm);
static cBitmap bmAspectRatio11(ar11_xpm);
static cBitmap bmAspectRatio169(ar169_xpm);
static cBitmap bmAspectRatio2211(ar2211_xpm);
static cBitmap bmAspectRatio43(ar43_xpm);
static cBitmap bmDevice(device_xpm);
static cBitmap bmZero(zero_xpm);
static cBitmap bmOne(one_xpm);
static cBitmap bmTwo(two_xpm);
static cBitmap bmThree(three_xpm);
static cBitmap bmFour(four_xpm);
static cBitmap bmFive(five_xpm);
static cBitmap bmSix(six_xpm);
static cBitmap bmSeven(seven_xpm);
static cBitmap bmEight(eight_xpm);
static cBitmap bmFormat1080(format1080_xpm);
static cBitmap bmFormat1080i(format1080i_xpm);
static cBitmap bmFormat1080p(format1080p_xpm);
static cBitmap bmFormat720(format720_xpm);
static cBitmap bmFormat720i(format720i_xpm);
static cBitmap bmFormat720p(format720p_xpm);
static cBitmap bmFormat576(format576_xpm);
static cBitmap bmFormat576i(format576i_xpm);
static cBitmap bmFormat576p(format576p_xpm);
static cBitmap bmFormat480(format480_xpm);
static cBitmap bmFormat480i(format480i_xpm);
static cBitmap bmFormat480p(format480p_xpm);

cFemonSymbolCache femonSymbols;

cFemonSymbolCache::cFemonSymbolCache()
: xFactorM(1.0),
  yFactorM(1.0),
  antiAliasM(false)
{
  Populate();
}

cFemonSymbolCache::~cFemonSymbolCache()
{
  Flush();
}

void cFemonSymbolCache::Refresh()
{
  int width, height;
  double aspect, xfactor, yfactor;
  cDevice::PrimaryDevice()->GetOsdSize(width, height, aspect);
  debug1("%s width=%d height=%d", __PRETTY_FUNCTION__, width, height);
  xfactor = (double)width / DEFAULT_WIDTH;
  yfactor = (double)height / DEFAULT_HEIGHT;
  if (!DoubleEqual(xfactor, xFactorM) || !DoubleEqual(yfactor, yFactorM)) {
     xFactorM = xfactor;
     yFactorM = yfactor;
     Populate();
     }
}

bool cFemonSymbolCache::Populate(void)
{
  debug1("%s xFactor=%.02f yFactor=%.02f", __PRETTY_FUNCTION__, xFactorM, yFactorM);
  if (!DoubleEqual(0.0, xFactorM) || !DoubleEqual(0.0, yFactorM)) {
     Flush();

     // pushing order must follow the enumeration - keep original proportions except for frontend status ones
     cacheM.Append(bmOnePixel.Scaled(yFactorM, yFactorM, antiAliasM));        // SYMBOL_ONEPIXEL
     cacheM.Append(bmStereo.Scaled(yFactorM, yFactorM, antiAliasM));          // SYMBOL_STEREO
     cacheM.Append(bmMonoLeft.Scaled(yFactorM, yFactorM, antiAliasM));        // SYMBOL_MONO_LEFT
     cacheM.Append(bmMonoRight.Scaled(yFactorM, yFactorM, antiAliasM));       // SYMBOL_MONO_RIGHT
     cacheM.Append(bmDolbyDigital.Scaled(yFactorM, yFactorM, antiAliasM));    // SYMBOL_DD
     cacheM.Append(bmDolbyDigital20.Scaled(yFactorM, yFactorM, antiAliasM));  // SYMBOL_DD20
     cacheM.Append(bmDolbyDigital51.Scaled(yFactorM, yFactorM, antiAliasM));  // SYMBOL_DD51
     cacheM.Append(bmMpeg2.Scaled(yFactorM, yFactorM, antiAliasM));           // SYMBOL_MPEG2
     cacheM.Append(bmH264.Scaled(yFactorM, yFactorM, antiAliasM));            // SYMBOL_H264
     cacheM.Append(bmH265.Scaled(yFactorM, yFactorM, antiAliasM));            // SYMBOL_H265
     cacheM.Append(bmPal.Scaled(yFactorM, yFactorM, antiAliasM));             // SYMBOL_PAL
     cacheM.Append(bmNtsc.Scaled(yFactorM, yFactorM, antiAliasM));            // SYMBOL_NTSC
     cacheM.Append(bmEncrypted.Scaled(yFactorM, yFactorM, antiAliasM));       // SYMBOL_ENCRYPTED
     cacheM.Append(bmSvdrp.Scaled(yFactorM, yFactorM, antiAliasM));           // SYMBOL_SVDRP
     cacheM.Append(bmLock.Scaled(xFactorM, yFactorM, antiAliasM));            // SYMBOL_LOCK
     cacheM.Append(bmSignal.Scaled(xFactorM, yFactorM, antiAliasM));          // SYMBOL_SIGNAL
     cacheM.Append(bmCarrier.Scaled(xFactorM, yFactorM, antiAliasM));         // SYMBOL_CARRIER
     cacheM.Append(bmViterbi.Scaled(xFactorM, yFactorM, antiAliasM));         // SYMBOL_VITERBI
     cacheM.Append(bmSync.Scaled(xFactorM, yFactorM, antiAliasM));            // SYMBOL_SYNC
     cacheM.Append(bmAspectRatio11.Scaled(yFactorM, yFactorM, antiAliasM));   // SYMBOL_AR_1_1
     cacheM.Append(bmAspectRatio169.Scaled(yFactorM, yFactorM, antiAliasM));  // SYMBOL_AR_16_9
     cacheM.Append(bmAspectRatio2211.Scaled(yFactorM, yFactorM, antiAliasM)); // SYMBOL_AR_2_21_1
     cacheM.Append(bmAspectRatio43.Scaled(yFactorM, yFactorM, antiAliasM));   // SYMBOL_AR_4_3
     cacheM.Append(bmDevice.Scaled(yFactorM, yFactorM, antiAliasM));          // SYMBOL_DEVICE
     cacheM.Append(bmZero.Scaled(yFactorM, yFactorM, antiAliasM));            // SYMBOL_ZERO
     cacheM.Append(bmOne.Scaled(yFactorM, yFactorM, antiAliasM));             // SYMBOL_ONE
     cacheM.Append(bmTwo.Scaled(yFactorM, yFactorM, antiAliasM));             // SYMBOL_TWO
     cacheM.Append(bmThree.Scaled(yFactorM, yFactorM, antiAliasM));           // SYMBOL_THREE
     cacheM.Append(bmFour.Scaled(yFactorM, yFactorM, antiAliasM));            // SYMBOL_FOUR
     cacheM.Append(bmFive.Scaled(yFactorM, yFactorM, antiAliasM));            // SYMBOL_FIVE
     cacheM.Append(bmSix.Scaled(yFactorM, yFactorM, antiAliasM));             // SYMBOL_SIX
     cacheM.Append(bmSeven.Scaled(yFactorM, yFactorM, antiAliasM));           // SYMBOL_SEVEN
     cacheM.Append(bmEight.Scaled(yFactorM, yFactorM, antiAliasM));           // SYMBOL_EIGHT
     cacheM.Append(bmFormat1080.Scaled(yFactorM, yFactorM, antiAliasM));      // SYMBOL_FORMAT_1080
     cacheM.Append(bmFormat1080i.Scaled(yFactorM, yFactorM, antiAliasM));     // SYMBOL_FORMAT_1080i
     cacheM.Append(bmFormat1080p.Scaled(yFactorM, yFactorM, antiAliasM));     // SYMBOL_FORMAT_1080p
     cacheM.Append(bmFormat720.Scaled(yFactorM, yFactorM, antiAliasM));       // SYMBOL_FORMAT_720
     cacheM.Append(bmFormat720i.Scaled(yFactorM, yFactorM, antiAliasM));      // SYMBOL_FORMAT_720i
     cacheM.Append(bmFormat720p.Scaled(yFactorM, yFactorM, antiAliasM));      // SYMBOL_FORMAT_720p
     cacheM.Append(bmFormat576.Scaled(yFactorM, yFactorM, antiAliasM));       // SYMBOL_FORMAT_576
     cacheM.Append(bmFormat576i.Scaled(yFactorM, yFactorM, antiAliasM));      // SYMBOL_FORMAT_576i
     cacheM.Append(bmFormat576p.Scaled(yFactorM, yFactorM, antiAliasM));      // SYMBOL_FORMAT_576p
     cacheM.Append(bmFormat480.Scaled(yFactorM, yFactorM, antiAliasM));       // SYMBOL_FORMAT_480
     cacheM.Append(bmFormat480i.Scaled(yFactorM, yFactorM, antiAliasM));      // SYMBOL_FORMAT_480i
     cacheM.Append(bmFormat480p.Scaled(yFactorM, yFactorM, antiAliasM));      // SYMBOL_FORMAT_480p

     return true;
     }

  return false;
}

bool cFemonSymbolCache::Flush(void)
{
  debug1("%s", __PRETTY_FUNCTION__);
  for (int i = 0; i < cacheM.Size(); ++i) {
      cBitmap *bmp = cacheM[i];
      DELETENULL(bmp);
      }
  cacheM.Clear();
  return true;
}

cBitmap& cFemonSymbolCache::Get(eSymbols symbolP)
{
  cBitmap *bitmapM = &bmOnePixel;

  if (symbolP < cacheM.Size())
     bitmapM = cacheM[symbolP];
  else
    error("%s (%d) Invalid symbol", __PRETTY_FUNCTION__, symbolP);

  return *bitmapM;
}
