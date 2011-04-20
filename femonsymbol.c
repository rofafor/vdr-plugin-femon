/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include <vdr/device.h>
#include "femontools.h"
#include "femonsymbol.h"

#include "symbols/stereo.xpm"
#include "symbols/monoleft.xpm"
#include "symbols/monoright.xpm"
#include "symbols/dolbydigital.xpm"
#include "symbols/dolbydigital20.xpm"
#include "symbols/dolbydigital51.xpm"
#include "symbols/mpeg2.xpm"
#include "symbols/h264.xpm"
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
#include "symbols/format720.xpm"
#include "symbols/format576.xpm"
#include "symbols/format480.xpm"

static cBitmap bmStereo(stereo_xpm);
static cBitmap bmMonoLeft(monoleft_xpm);
static cBitmap bmMonoRight(monoright_xpm);
static cBitmap bmDolbyDigital(dolbydigital_xpm);
static cBitmap bmDolbyDigital20(dolbydigital20_xpm);
static cBitmap bmDolbyDigital51(dolbydigital51_xpm);
static cBitmap bmMpeg2(mpeg2_xpm);
static cBitmap bmH264(h264_xpm);
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
static cBitmap bmFormat720(format720_xpm);
static cBitmap bmFormat576(format576_xpm);
static cBitmap bmFormat480(format480_xpm);

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
  debug("%s(): %dx%d\n", __PRETTY_FUNCTION__, width, height);
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
  debug("%s(): %.02fx%.02f\n", __PRETTY_FUNCTION__, xFactorM, yFactorM);
  if (!DoubleEqual(0.0, xFactorM) || !DoubleEqual(0.0, yFactorM)) {
     Flush();

     // pushing order must follow the enumeration
     cacheM.Append(bmStereo.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_STEREO
     cacheM.Append(bmMonoLeft.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_MONO_LEFT
     cacheM.Append(bmMonoRight.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_MONO_RIGHT
     cacheM.Append(bmDolbyDigital.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_DD
     cacheM.Append(bmDolbyDigital20.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_DD20
     cacheM.Append(bmDolbyDigital51.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_DD51
     cacheM.Append(bmMpeg2.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_MPEG2
     cacheM.Append(bmH264.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_H264
     cacheM.Append(bmPal.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_PAL
     cacheM.Append(bmNtsc.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_NTSC
     cacheM.Append(bmEncrypted.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_ENCRYPTED
     cacheM.Append(bmSvdrp.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_SVDRP
     cacheM.Append(bmLock.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_LOCK
     cacheM.Append(bmSignal.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_SIGNAL
     cacheM.Append(bmCarrier.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_CARRIER
     cacheM.Append(bmViterbi.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_VITERBI
     cacheM.Append(bmSync.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_SYNC
     cacheM.Append(bmAspectRatio11.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_AR_1_1
     cacheM.Append(bmAspectRatio169.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_AR_16_9
     cacheM.Append(bmAspectRatio2211.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_AR_2_21_1
     cacheM.Append(bmAspectRatio43.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_AR_4_3
     cacheM.Append(bmDevice.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_DEVICE
     cacheM.Append(bmZero.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_ZERO
     cacheM.Append(bmOne.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_ONE
     cacheM.Append(bmTwo.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_TWO
     cacheM.Append(bmThree.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_THREE
     cacheM.Append(bmFour.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_FOUR
     cacheM.Append(bmFive.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_FIVE
     cacheM.Append(bmSix.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_SIX
     cacheM.Append(bmSeven.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_SEVEN
     cacheM.Append(bmEight.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_EIGHT
     cacheM.Append(bmFormat1080.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_FORMAT_1080
     cacheM.Append(bmFormat720.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_FORMAT_720
     cacheM.Append(bmFormat576.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_FORMAT_576
     cacheM.Append(bmFormat480.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_FORMAT_480

     return true;
     }

  return false;
}

bool cFemonSymbolCache::Flush(void)
{
  debug("%s()\n", __PRETTY_FUNCTION__);
  for (int i = 0; i < cacheM.Size(); ++i) {
      cBitmap *bmp = cacheM[i];
      DELETENULL(bmp);
      }
  cacheM.Clear();
  return true;
}

cBitmap& cFemonSymbolCache::Get(eSymbols symbolP)
{
  cBitmap *bitmapM = NULL;

  if (symbolP < cacheM.Size())
     bitmapM = cacheM[symbolP];
  else
    error("%s(): Invalid symbol %d\n", __PRETTY_FUNCTION__, symbolP);

  return *bitmapM;
}
