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
  debug("cFemonSymbolCache::Refresh(): %dx%d", width, height);
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
  debug("cFemonSymbolCache::Populate(): %.02fx%.02f ", xFactorM, yFactorM);
  if (!DoubleEqual(0.0, xFactorM) || !DoubleEqual(0.0, yFactorM)) {
     Flush();

     // pushing order must follow the enumeration
     cacheM.push_back(bmStereo.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_STEREO
     cacheM.push_back(bmMonoLeft.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_MONO_LEFT
     cacheM.push_back(bmMonoRight.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_MONO_RIGHT
     cacheM.push_back(bmDolbyDigital.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_DD
     cacheM.push_back(bmDolbyDigital20.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_DD20
     cacheM.push_back(bmDolbyDigital51.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_DD51
     cacheM.push_back(bmMpeg2.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_MPEG2
     cacheM.push_back(bmH264.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_H264
     cacheM.push_back(bmPal.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_PAL
     cacheM.push_back(bmNtsc.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_NTSC
     cacheM.push_back(bmEncrypted.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_ENCRYPTED
     cacheM.push_back(bmSvdrp.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_SVDRP
     cacheM.push_back(bmLock.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_LOCK
     cacheM.push_back(bmSignal.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_SIGNAL
     cacheM.push_back(bmCarrier.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_CARRIER
     cacheM.push_back(bmViterbi.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_VITERBI
     cacheM.push_back(bmSync.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_SYNC
     cacheM.push_back(bmAspectRatio11.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_AR_1_1
     cacheM.push_back(bmAspectRatio169.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_AR_16_9
     cacheM.push_back(bmAspectRatio2211.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_AR_2_21_1
     cacheM.push_back(bmAspectRatio43.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_AR_4_3
     cacheM.push_back(bmDevice.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_DEVICE
     cacheM.push_back(bmZero.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_ZERO
     cacheM.push_back(bmOne.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_ONE
     cacheM.push_back(bmTwo.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_TWO
     cacheM.push_back(bmThree.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_THREE
     cacheM.push_back(bmFour.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_FOUR
     cacheM.push_back(bmFive.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_FIVE
     cacheM.push_back(bmSix.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_SIX
     cacheM.push_back(bmSeven.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_SEVEN
     cacheM.push_back(bmEight.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_EIGHT
     cacheM.push_back(bmFormat1080.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_FORMAT_1080
     cacheM.push_back(bmFormat720.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_FORMAT_720
     cacheM.push_back(bmFormat576.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_FORMAT_576
     cacheM.push_back(bmFormat480.Scaled(xFactorM, yFactorM, antiAliasM)); // SYMBOL_FORMAT_480

     return true;
     }

  return false;
}

bool cFemonSymbolCache::Flush(void)
{
  debug("cFemonSymbolCache::Flush()");
  if (!cacheM.empty()) {
     for (unsigned int i = 0; i < cacheM.size(); ++i) {
         cBitmap *bmp = cacheM[i];
         DELETENULL(bmp);
         }
     cacheM.clear();
     }
  return true;
}

cBitmap& cFemonSymbolCache::Get(unsigned int symbolP)
{
  cBitmap *bitmapM = NULL;

  if (symbolP < cacheM.size())
     bitmapM = cacheM[symbolP];

  return *bitmapM;
}
