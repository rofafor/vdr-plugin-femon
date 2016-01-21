/*
 * osd.c: Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <ctype.h>
#include <math.h>

#include "config.h"
#include "iptvservice.h"
#include "log.h"
#include "receiver.h"
#include "symbol.h"
#include "tools.h"
#include "osd.h"

#define CHANNELINPUT_TIMEOUT      1000
#define SVDRPPLUGIN               "svdrpservice"

#define OSDWIDTH                  osdWidthM              // in pixels
#define OSDHEIGHT                 osdHeightM             // in pixels
#define OSDROWHEIGHT              fontM->Height()        // in pixels
#define OSDINFOHEIGHT             (OSDROWHEIGHT * 14)     // in pixels (14 rows)
#define OSDSTATUSHEIGHT           (OSDROWHEIGHT * 6)      // in pixels (6 rows)
#define OSDSYMBOL(id)             femonSymbols.Get(id)
#define OSDSPACING                femonSymbols.GetSpacing()
#define OSDROUNDING               femonSymbols.GetRounding()
#define IS_OSDROUNDING            (FemonConfig.GetSkin() == eFemonSkinElchi)
#define IS_OSDRESOLUTION(r1, r2)  (abs(r1 - r2) < 20)
#define OSDINFOWIN_Y(offset)      (FemonConfig.GetPosition() ? (OSDHEIGHT - OSDINFOHEIGHT + offset) : offset)
#define OSDINFOWIN_X(col)         ((col == 4) ? int(round(OSDWIDTH * 0.76)) : \
                                   (col == 3) ? int(round(OSDWIDTH * 0.51)) : \
                                   (col == 2) ? int(round(OSDWIDTH * 0.26)) : \
                                                int(round(OSDWIDTH * 0.025)))
#define OSDSTATUSWIN_Y(offset)    (FemonConfig.GetPosition() ? offset : (OSDHEIGHT - OSDSTATUSHEIGHT + offset))
#define OSDSTATUSWIN_X(col)       ((col == 7) ? int(round(OSDWIDTH * 0.79)) : \
                                   (col == 6) ? int(round(OSDWIDTH * 0.68)) : \
                                   (col == 5) ? int(round(OSDWIDTH * 0.46)) : \
                                   (col == 4) ? int(round(OSDWIDTH * 0.37)) : \
                                   (col == 3) ? int(round(OSDWIDTH * 0.21)) : \
                                   (col == 2) ? int(round(OSDWIDTH * 0.12)) : \
                                                int(round(OSDWIDTH * 0.025)))
#define OSDSTATUSWIN_XSYMBOL(c,w) (c * ((OSDWIDTH - (5 * w)) / 6) + ((c - 1) * w))
#define OSDBARWIDTH(x)            (OSDWIDTH * x / 100)

#define OSDDRAWSTATUSBM(spacing) \
        if (bm) { \
           x -= bm->Width() + spacing; \
           y = (OSDROWHEIGHT - bm->Height()) / 2; \
           if (y < 0) y = 0; \
           osdM->DrawBitmap(x, OSDSTATUSWIN_Y(offset) + y, *bm, FemonTheme[FemonConfig.GetTheme()].clrTitleText, FemonTheme[FemonConfig.GetTheme()].clrTitleBackground); \
           }

#define OSDDRAWSTATUSFRONTEND(column, bitmap, status) \
        osdM->DrawBitmap(OSDSTATUSWIN_XSYMBOL(column, x), OSDSTATUSWIN_Y(offset) + y, bitmap, (frontendStatusM & status) ? FemonTheme[FemonConfig.GetTheme()].clrActiveText : FemonTheme[FemonConfig.GetTheme()].clrRed, FemonTheme[FemonConfig.GetTheme()].clrBackground)

#define OSDDRAWSTATUSVALUES(label1, label2, label3, label4, label5, label6, label7) \
        osdM->DrawText(OSDSTATUSWIN_X(1), OSDSTATUSWIN_Y(offset), label1, FemonTheme[FemonConfig.GetTheme()].clrInactiveText, FemonTheme[FemonConfig.GetTheme()].clrBackground, fontM); \
        osdM->DrawText(OSDSTATUSWIN_X(2), OSDSTATUSWIN_Y(offset), label2, FemonTheme[FemonConfig.GetTheme()].clrInactiveText, FemonTheme[FemonConfig.GetTheme()].clrBackground, fontM); \
        osdM->DrawText(OSDSTATUSWIN_X(3), OSDSTATUSWIN_Y(offset), label3, FemonTheme[FemonConfig.GetTheme()].clrInactiveText, FemonTheme[FemonConfig.GetTheme()].clrBackground, fontM); \
        osdM->DrawText(OSDSTATUSWIN_X(4), OSDSTATUSWIN_Y(offset), label4, FemonTheme[FemonConfig.GetTheme()].clrInactiveText, FemonTheme[FemonConfig.GetTheme()].clrBackground, fontM); \
        osdM->DrawText(OSDSTATUSWIN_X(5), OSDSTATUSWIN_Y(offset), label5, FemonTheme[FemonConfig.GetTheme()].clrInactiveText, FemonTheme[FemonConfig.GetTheme()].clrBackground, fontM); \
        osdM->DrawText(OSDSTATUSWIN_X(6), OSDSTATUSWIN_Y(offset), label6, FemonTheme[FemonConfig.GetTheme()].clrInactiveText, FemonTheme[FemonConfig.GetTheme()].clrBackground, fontM); \
        osdM->DrawText(OSDSTATUSWIN_X(7), OSDSTATUSWIN_Y(offset), label7, FemonTheme[FemonConfig.GetTheme()].clrInactiveText, FemonTheme[FemonConfig.GetTheme()].clrBackground, fontM)

#define OSDDRAWSTATUSBAR(value) \
        if (value > 0) { \
           int barvalue = OSDBARWIDTH(value); \
           osdM->DrawRectangle(0, OSDSTATUSWIN_Y(offset) + 3, min(OSDBARWIDTH(FemonConfig.GetRedLimit()), barvalue), OSDSTATUSWIN_Y(offset) + OSDROWHEIGHT - 3, FemonTheme[FemonConfig.GetTheme()].clrRed); \
           if (barvalue > OSDBARWIDTH(FemonConfig.GetRedLimit())) \
              osdM->DrawRectangle(OSDBARWIDTH(FemonConfig.GetRedLimit()), OSDSTATUSWIN_Y(offset) + 3, min((OSDWIDTH * FemonConfig.GetGreenLimit() / 100), barvalue), OSDSTATUSWIN_Y(offset) + OSDROWHEIGHT - 3, FemonTheme[FemonConfig.GetTheme()].clrYellow); \
           if (barvalue > OSDBARWIDTH(FemonConfig.GetGreenLimit())) \
              osdM->DrawRectangle(OSDBARWIDTH(FemonConfig.GetGreenLimit()), OSDSTATUSWIN_Y(offset) + 3, barvalue, OSDSTATUSWIN_Y(offset) + OSDROWHEIGHT - 3, FemonTheme[FemonConfig.GetTheme()].clrGreen); \
           }

#define OSDDRAWSTATUSTITLEBAR(title) \
        osdM->DrawRectangle(0, OSDSTATUSWIN_Y(offset), OSDWIDTH, OSDSTATUSWIN_Y(offset) + OSDROWHEIGHT - 1, FemonTheme[FemonConfig.GetTheme()].clrTitleBackground); \
        osdM->DrawText(OSDSTATUSWIN_X(1), OSDSTATUSWIN_Y(offset), title, FemonTheme[FemonConfig.GetTheme()].clrTitleText, FemonTheme[FemonConfig.GetTheme()].clrTitleBackground, fontM); \
        if (IS_OSDROUNDING) { \
           osdM->DrawEllipse(0, OSDSTATUSWIN_Y(0), OSDROUNDING, OSDSTATUSWIN_Y(OSDROUNDING), clrTransparent, -2); \
           osdM->DrawEllipse(OSDWIDTH - OSDROUNDING, OSDSTATUSWIN_Y(0), OSDWIDTH, OSDSTATUSWIN_Y(OSDROUNDING), clrTransparent, -1); \
           } \
        osdM->DrawRectangle(0, OSDSTATUSWIN_Y(offset) + OSDROWHEIGHT, OSDWIDTH, OSDSTATUSWIN_Y(offset) + OSDSTATUSHEIGHT - 1, FemonTheme[FemonConfig.GetTheme()].clrBackground)

#define OSDDRAWSTATUSBOTTOMBAR() \
        if (IS_OSDROUNDING) { \
           osdM->DrawEllipse(0, OSDSTATUSWIN_Y(OSDSTATUSHEIGHT) - OSDROUNDING, OSDROUNDING, OSDSTATUSWIN_Y(OSDSTATUSHEIGHT), clrTransparent, -3); \
           osdM->DrawEllipse(OSDWIDTH - OSDROUNDING, OSDSTATUSWIN_Y(OSDSTATUSHEIGHT) - OSDROUNDING, OSDWIDTH, OSDSTATUSWIN_Y(OSDSTATUSHEIGHT), clrTransparent, -4); \
           }

#define OSDCLEARSTATUS() \
        osdM->DrawRectangle(0, OSDSTATUSWIN_Y(0), OSDWIDTH, OSDSTATUSWIN_Y(OSDSTATUSHEIGHT) - 1, clrTransparent)

#define OSDDRAWINFOLEFT(label, value) \
        osdM->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), label, FemonTheme[FemonConfig.GetTheme()].clrInactiveText, FemonTheme[FemonConfig.GetTheme()].clrBackground, fontM); \
        osdM->DrawText(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), value, FemonTheme[FemonConfig.GetTheme()].clrActiveText, FemonTheme[FemonConfig.GetTheme()].clrBackground, fontM)

#define OSDDRAWINFORIGHT(label, value) \
        osdM->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), label, FemonTheme[FemonConfig.GetTheme()].clrInactiveText, FemonTheme[FemonConfig.GetTheme()].clrBackground, fontM); \
        osdM->DrawText(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), value, FemonTheme[FemonConfig.GetTheme()].clrActiveText, FemonTheme[FemonConfig.GetTheme()].clrBackground, fontM)

#define OSDDRAWINFOACTIVE(label, value) \
        osdM->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), label, FemonTheme[FemonConfig.GetTheme()].clrActiveText, FemonTheme[FemonConfig.GetTheme()].clrBackground, fontM); \
        osdM->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), value, FemonTheme[FemonConfig.GetTheme()].clrActiveText, FemonTheme[FemonConfig.GetTheme()].clrBackground, fontM)

#define OSDDRAWINFOINACTIVE(label, value) \
        osdM->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), label, FemonTheme[FemonConfig.GetTheme()].clrInactiveText, FemonTheme[FemonConfig.GetTheme()].clrBackground, fontM); \
        osdM->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), value, FemonTheme[FemonConfig.GetTheme()].clrActiveText, FemonTheme[FemonConfig.GetTheme()].clrBackground, fontM)

#define OSDDRAWINFOLINE(label) \
        osdM->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), label, FemonTheme[FemonConfig.GetTheme()].clrActiveText, FemonTheme[FemonConfig.GetTheme()].clrBackground, fontM)

#define OSDDRAWINFOTITLEBAR(title) \
        osdM->DrawRectangle(0, OSDINFOWIN_Y(offset), OSDWIDTH, OSDINFOWIN_Y(offset) + OSDROWHEIGHT - 1, FemonTheme[FemonConfig.GetTheme()].clrTitleBackground); \
        osdM->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), title, FemonTheme[FemonConfig.GetTheme()].clrTitleText, FemonTheme[FemonConfig.GetTheme()].clrTitleBackground, fontM); \
        if (IS_OSDROUNDING) { \
           osdM->DrawEllipse(0, OSDINFOWIN_Y(0), OSDROUNDING, OSDINFOWIN_Y(OSDROUNDING), clrTransparent, -2); \
           osdM->DrawEllipse(OSDWIDTH - OSDROUNDING, OSDINFOWIN_Y(0), OSDWIDTH, OSDINFOWIN_Y(OSDROUNDING), clrTransparent, -1); \
           } \
        osdM->DrawRectangle(0, OSDINFOWIN_Y(offset) + OSDROWHEIGHT, OSDWIDTH, OSDINFOWIN_Y(offset) + OSDINFOHEIGHT - 1, FemonTheme[FemonConfig.GetTheme()].clrBackground)

#define OSDDRAWINFOBOTTOMBAR() \
        if (IS_OSDROUNDING) { \
           osdM->DrawEllipse(0, OSDINFOWIN_Y(OSDINFOHEIGHT) - OSDROUNDING, OSDROUNDING, OSDINFOWIN_Y(OSDINFOHEIGHT), clrTransparent, -3); \
           osdM->DrawEllipse((OSDWIDTH - OSDROUNDING), OSDINFOWIN_Y(OSDINFOHEIGHT) - OSDROUNDING, OSDWIDTH, OSDINFOWIN_Y(OSDINFOHEIGHT), clrTransparent, -4); \
           }

#define OSDCLEARINFO() \
        osdM->DrawRectangle(0, OSDINFOWIN_Y(0), OSDWIDTH, OSDINFOWIN_Y(OSDINFOHEIGHT) - 1, clrTransparent)

#ifndef MINFONTSIZE
#define MINFONTSIZE 10
#endif

#ifndef MAXFONTSIZE
#define MAXFONTSIZE 64
#endif

class cFemonDummyFont : public cFont {
public:
  virtual int Width(uint cP) const { return 10; }
  virtual int Width(const char *sP) const { return 50; }
  virtual int Height(void) const { return 20; }
  virtual void DrawText(cBitmap *bitmapP, int xP, int yP, const char *sP, tColor colorFgP, tColor colorBgP, int widthP) const {}
  virtual void DrawText(cPixmap *pixmapP, int xP, int yP, const char *sP, tColor colorFgP, tColor colorBgP, int widthP) const {}
};

cFemonOsd *cFemonOsd::pInstanceS = NULL;

cFemonOsd *cFemonOsd::Instance(bool createP)
{
  debug1("%s (%d)", __PRETTY_FUNCTION__, createP);
  if ((pInstanceS == NULL) && createP)
  {
     pInstanceS = new cFemonOsd();
  }
  return (pInstanceS);
}

cFemonOsd::cFemonOsd()
: cOsdObject(true), cThread("femon osd"),
  osdM(NULL),
  receiverM(NULL),
  frontendM(-1),
  svdrpFrontendM(-1),
  svdrpVideoBitRateM(-1),
  svdrpAudioBitRateM(-1),
  svdrpPluginM(NULL),
  numberM(0),
  oldNumberM(0),
  qualityM(0),
  qualityValidM(false),
  strengthM(0),
  strengthValidM(false),
  snrM(0),
  snrValidM(false),
  signalM(0),
  signalValidM(false),
  berM(0),
  berValidM(false),
  uncM(0),
  uncValidM(false),
  frontendNameM(""),
  frontendStatusValidM(false),
  deviceSourceM(DEVICESOURCE_DVBAPI),
  displayModeM(FemonConfig.GetDisplayMode()),
  osdWidthM(cOsd::OsdWidth() * (100 - FemonConfig.GetDownscale()) / 100),
  osdHeightM(cOsd::OsdHeight() * (100 - FemonConfig.GetDownscale()) / 100),
  osdLeftM(cOsd::OsdLeft() + (cOsd::OsdWidth() * FemonConfig.GetDownscale() / 200)),
  osdTopM(cOsd::OsdTop() + (cOsd::OsdHeight() * FemonConfig.GetDownscale() / 200)),
  inputTimeM(0),
  sleepM(),
  mutexM()
{
  int tmp;
  debug1("%s", __PRETTY_FUNCTION__);
  memset(&frontendStatusM, 0, sizeof(frontendStatusM));
  memset(&frontendInfoM, 0, sizeof(frontendInfoM));
  svdrpConnectionM.handle = -1;
  femonSymbols.Refresh();
  fontM = cFont::CreateFont(Setup.FontSml, constrain(Setup.FontSmlSize, MINFONTSIZE, MAXFONTSIZE));
  if (!fontM || !fontM->Height()) {
     fontM = new cFemonDummyFont;
     error("%s Cannot create required font", __PRETTY_FUNCTION__);
     }
  tmp = 5 * OSDSYMBOL(SYMBOL_LOCK).Width() + 6 * OSDSPACING;
  if (OSDWIDTH < tmp) {
     error("%s OSD width (%d) smaller than required (%d).", __PRETTY_FUNCTION__, OSDWIDTH, tmp);
     OSDWIDTH = tmp;
     }
  tmp = OSDINFOHEIGHT + OSDROWHEIGHT + OSDSTATUSHEIGHT;
  if (OSDHEIGHT < tmp) {
     error("%s OSD height (%d) smaller than required (%d).", __PRETTY_FUNCTION__, OSDHEIGHT, tmp);
     OSDHEIGHT = tmp;
     }
}

cFemonOsd::~cFemonOsd(void)
{
  debug1("%s", __PRETTY_FUNCTION__);
  sleepM.Signal();
  if (Running())
     Cancel(3);
  if (svdrpConnectionM.handle >= 0) {
     svdrpPluginM = cPluginManager::GetPlugin(SVDRPPLUGIN);
     if (svdrpPluginM)
        svdrpPluginM->Service("SvdrpConnection-v1.0", &svdrpConnectionM);
     }
  if (receiverM) {
     receiverM->Deactivate();
     DELETENULL(receiverM);
     }
  if (osdM)
     DELETENULL(osdM);
  if (fontM)
     DELETENULL(fontM);
  if (frontendM >= 0) {
     close(frontendM);
     frontendM = -1;
     }
  pInstanceS = NULL;
}

void cFemonOsd::DrawStatusWindow(void)
{
  cMutexLock lock(&mutexM);
  LOCK_CHANNELS_READ;
  const cChannel *channel = Channels->GetByNumber(cDevice::CurrentChannel());

  if (osdM && channel) {
     cBitmap *bm = NULL;
     int offset = 0;
     int x = OSDWIDTH - OSDROUNDING;
     int y = 0;
     eTrackType track = cDevice::PrimaryDevice()->GetCurrentAudioTrack();

     OSDDRAWSTATUSTITLEBAR(*cString::sprintf("%d%s %s", numberM ? numberM : channel->Number(), numberM ? "-" : "", channel->ShortName(true)));
     if (svdrpFrontendM >= 0) {
        bm = &OSDSYMBOL(SYMBOL_SVDRP);
        OSDDRAWSTATUSBM(OSDSPACING);
        }
     switch (cDevice::ActualDevice()->CardIndex()) {
       case 1:  bm = &OSDSYMBOL(SYMBOL_ONE);   break;
       case 2:  bm = &OSDSYMBOL(SYMBOL_TWO);   break;
       case 3:  bm = &OSDSYMBOL(SYMBOL_THREE); break;
       case 4:  bm = &OSDSYMBOL(SYMBOL_FOUR);  break;
       case 5:  bm = &OSDSYMBOL(SYMBOL_FIVE);  break;
       case 6:  bm = &OSDSYMBOL(SYMBOL_SIX);   break;
       case 7:  bm = &OSDSYMBOL(SYMBOL_SEVEN); break;
       case 8:  bm = &OSDSYMBOL(SYMBOL_EIGHT); break;
       default: bm = &OSDSYMBOL(SYMBOL_ZERO);  break;
       }
     OSDDRAWSTATUSBM(OSDSPACING);
     bm = &OSDSYMBOL(SYMBOL_DEVICE);
     OSDDRAWSTATUSBM(0);
     if (IS_AUDIO_TRACK(track)) {
        switch (int(track - ttAudioFirst)) {
           case 1:  bm = &OSDSYMBOL(SYMBOL_ONE);   break;
           case 2:  bm = &OSDSYMBOL(SYMBOL_TWO);   break;
           case 3:  bm = &OSDSYMBOL(SYMBOL_THREE); break;
           case 4:  bm = &OSDSYMBOL(SYMBOL_FOUR);  break;
           case 5:  bm = &OSDSYMBOL(SYMBOL_FIVE);  break;
           case 6:  bm = &OSDSYMBOL(SYMBOL_SIX);   break;
           case 7:  bm = &OSDSYMBOL(SYMBOL_SEVEN); break;
           case 8:  bm = &OSDSYMBOL(SYMBOL_EIGHT); break;
           default: bm = &OSDSYMBOL(SYMBOL_ZERO);  break;
           }
        OSDDRAWSTATUSBM(OSDSPACING);
        switch (cDevice::PrimaryDevice()->GetAudioChannel()) {
           case 1:  bm = &OSDSYMBOL(SYMBOL_MONO_LEFT);  break;
           case 2:  bm = &OSDSYMBOL(SYMBOL_MONO_RIGHT); break;
           default: bm = &OSDSYMBOL(SYMBOL_STEREO);     break;
           }
        OSDDRAWSTATUSBM(0);
        }
     else if (receiverM && receiverM->AC3Valid() && IS_DOLBY_TRACK(track)) {
        if      (receiverM->AC3_5_1()) bm = &OSDSYMBOL(SYMBOL_DD51);
        else if (receiverM->AC3_2_0()) bm = &OSDSYMBOL(SYMBOL_DD20);
        else                            bm = &OSDSYMBOL(SYMBOL_DD);
        OSDDRAWSTATUSBM(OSDSPACING);
        }
     if (receiverM) {
        if (IS_OSDRESOLUTION(receiverM->VideoVerticalSize(), 1080)) {
           switch (receiverM->VideoScan()) {
              case VIDEO_SCAN_INTERLACED:  bm = &OSDSYMBOL(SYMBOL_FORMAT_1080i); break;
              case VIDEO_SCAN_PROGRESSIVE: bm = &OSDSYMBOL(SYMBOL_FORMAT_1080p); break;
              default:                     bm = &OSDSYMBOL(SYMBOL_FORMAT_1080);  break;
              }
           }
        else if (IS_OSDRESOLUTION(receiverM->VideoVerticalSize(), 720)) {
           switch (receiverM->VideoScan()) {
              case VIDEO_SCAN_INTERLACED:  bm = &OSDSYMBOL(SYMBOL_FORMAT_720i); break;
              case VIDEO_SCAN_PROGRESSIVE: bm = &OSDSYMBOL(SYMBOL_FORMAT_720p); break;
              default:                     bm = &OSDSYMBOL(SYMBOL_FORMAT_720);  break;
              }
           }
        else if (IS_OSDRESOLUTION(receiverM->VideoVerticalSize(), 576)) {
           switch (receiverM->VideoScan()) {
              case VIDEO_SCAN_INTERLACED:  bm = &OSDSYMBOL(SYMBOL_FORMAT_576i); break;
              case VIDEO_SCAN_PROGRESSIVE: bm = &OSDSYMBOL(SYMBOL_FORMAT_576p); break;
              default:                     bm = &OSDSYMBOL(SYMBOL_FORMAT_576);  break;
              }
           }
        else if (IS_OSDRESOLUTION(receiverM->VideoVerticalSize(), 480)) {
           switch (receiverM->VideoScan()) {
              case VIDEO_SCAN_INTERLACED:  bm = &OSDSYMBOL(SYMBOL_FORMAT_480i); break;
              case VIDEO_SCAN_PROGRESSIVE: bm = &OSDSYMBOL(SYMBOL_FORMAT_480p); break;
              default:                     bm = &OSDSYMBOL(SYMBOL_FORMAT_480);  break;
              }
           }
        else
           bm = NULL;
        OSDDRAWSTATUSBM(OSDSPACING);
        switch (receiverM->VideoCodec()) {
           case VIDEO_CODEC_MPEG2: bm = &OSDSYMBOL(SYMBOL_MPEG2); break;
           case VIDEO_CODEC_H264:  bm = &OSDSYMBOL(SYMBOL_H264);  break;
           default:                bm = NULL;                     break;
           }
        OSDDRAWSTATUSBM(OSDSPACING);
        switch (receiverM->VideoFormat()) {
           case VIDEO_FORMAT_PAL:  bm = &OSDSYMBOL(SYMBOL_PAL);  break;
           case VIDEO_FORMAT_NTSC: bm = &OSDSYMBOL(SYMBOL_NTSC); break;
           default:                bm = NULL;                    break;
           }
        OSDDRAWSTATUSBM(OSDSPACING);
        switch (receiverM->VideoAspectRatio()) {
           case VIDEO_ASPECT_RATIO_1_1:    bm = &OSDSYMBOL(SYMBOL_AR_1_1);    break;
           case VIDEO_ASPECT_RATIO_4_3:    bm = &OSDSYMBOL(SYMBOL_AR_4_3);    break;
           case VIDEO_ASPECT_RATIO_16_9:   bm = &OSDSYMBOL(SYMBOL_AR_16_9);   break;
           case VIDEO_ASPECT_RATIO_2_21_1: bm = &OSDSYMBOL(SYMBOL_AR_2_21_1); break;
           default:                        bm = NULL;                         break;
           }
        OSDDRAWSTATUSBM(OSDSPACING);
        }
     if (channel->Ca() > 0xFF) {
        bm = &OSDSYMBOL(SYMBOL_ENCRYPTED);
        OSDDRAWSTATUSBM(OSDSPACING);
        }
     offset += OSDROWHEIGHT;
     if (strengthValidM)
        OSDDRAWSTATUSBAR(strengthM);
     offset += OSDROWHEIGHT;
     if (qualityValidM)
        OSDDRAWSTATUSBAR(qualityM);
     offset += OSDROWHEIGHT;
     OSDDRAWSTATUSVALUES("STR:", signalValidM ? *cString::sprintf("%04x", signalM) : "", signalValidM ? *cString::sprintf("(%2d%%)", signalM / 655) : "",
                         "BER:", berValidM ? *cString::sprintf("%08x", berM) : "", *cString::sprintf("%s:", tr("Video")),
                         *getBitrateMbits(receiverM ? receiverM->VideoBitrate() : (svdrpFrontendM >= 0 ? svdrpVideoBitRateM : -1.0)));
     offset += OSDROWHEIGHT;
     OSDDRAWSTATUSVALUES("SNR:", snrValidM ? *cString::sprintf("%04x", snrM) : "", snrValidM ? *cString::sprintf("(%2d%%)", snrM / 655) : "",
                         "UNC:", uncValidM ? *cString::sprintf("%08x", uncM) : "",
                         *cString::sprintf("%s:", (receiverM && receiverM->AC3Valid() && IS_DOLBY_TRACK(track)) ? tr("AC-3") : tr("Audio")),
                         *getBitrateKbits(receiverM ? ((receiverM->AC3Valid() && IS_DOLBY_TRACK(track)) ? receiverM->AC3Bitrate() : receiverM->AudioBitrate()) : (svdrpFrontendM >= 0 ? svdrpAudioBitRateM : -1.0)));
     offset += OSDROWHEIGHT;
     x = OSDSYMBOL(SYMBOL_LOCK).Width();
     y = (OSDROWHEIGHT - OSDSYMBOL(SYMBOL_LOCK).Height()) / 2;
     if (frontendStatusValidM) {
        OSDDRAWSTATUSFRONTEND(1, OSDSYMBOL(SYMBOL_LOCK),    FE_HAS_LOCK);
        OSDDRAWSTATUSFRONTEND(2, OSDSYMBOL(SYMBOL_SIGNAL),  FE_HAS_SIGNAL);
        OSDDRAWSTATUSFRONTEND(3, OSDSYMBOL(SYMBOL_CARRIER), FE_HAS_CARRIER);
        OSDDRAWSTATUSFRONTEND(4, OSDSYMBOL(SYMBOL_VITERBI), FE_HAS_VITERBI);
        OSDDRAWSTATUSFRONTEND(5, OSDSYMBOL(SYMBOL_SYNC),    FE_HAS_SYNC);
        }
     OSDDRAWSTATUSBOTTOMBAR();
     osdM->Flush();
     }
}

void cFemonOsd::DrawInfoWindow(void)
{
  cMutexLock lock(&mutexM);
  LOCK_CHANNELS_READ;
  const cChannel *channel = Channels->GetByNumber(cDevice::CurrentChannel());

  if (osdM && channel) {
     int offset = 0;
     eTrackType track = cDevice::PrimaryDevice()->GetCurrentAudioTrack();

     switch (displayModeM) {
       case eFemonModeTransponder:
            OSDDRAWINFOTITLEBAR(tr("Transponder Information"));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOLEFT( trVDR("Vpid"), *cString::sprintf("%d", channel->Vpid()));
            OSDDRAWINFORIGHT(trVDR("Ppid"), *cString::sprintf("%d", channel->Ppid()));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOLEFT(    tr("Apid"), *getApids(channel));
            OSDDRAWINFORIGHT(   tr("Dpid"), *getDpids(channel));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOLEFT(    tr("Spid"), *getSpids(channel));
            OSDDRAWINFORIGHT(trVDR("Tpid"), *cString::sprintf("%d", channel->Tpid()));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOLEFT( trVDR("Sid"),  *cString::sprintf("%d", channel->Sid()));
            OSDDRAWINFORIGHT(   tr("Nid"),  *cString::sprintf("%d", channel->Nid()));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOLEFT(    tr("Tid"),  *cString::sprintf("%d", channel->Tid()));
            OSDDRAWINFORIGHT(   tr("Rid"),  *cString::sprintf("%d", channel->Rid()));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOLEFT( trVDR("CA"),   *getCAids(channel));
            offset += OSDROWHEIGHT;
            switch (channel->Source() & cSource::st_Mask) {
              case cSource::stSat: {
                   cDvbTransponderParameters dtp(channel->Parameters());
                   OSDDRAWINFOLINE(*cString::sprintf("%s #%d - %s", *getSatelliteSystem(dtp.System()), (svdrpFrontendM >= 0) ? svdrpFrontendM : cDevice::ActualDevice()->CardIndex(), *frontendNameM));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("Frequency"),    *getFrequencyMHz(channel->Frequency()));
                   OSDDRAWINFORIGHT(trVDR("Source"),       *cSource::ToString(channel->Source()));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("Srate"),        *cString::sprintf("%d", channel->Srate()));
                   OSDDRAWINFORIGHT(trVDR("Polarization"), *cString::sprintf("%c", toupper(dtp.Polarization())));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("Inversion"),    *getInversion(dtp.Inversion()));
                   OSDDRAWINFORIGHT(trVDR("CoderateH"),    *getCoderate(dtp.CoderateH()));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("System"),       *getSatelliteSystem(dtp.System()));
                   if (dtp.System()) {
                      OSDDRAWINFORIGHT(trVDR("RollOff"),   *getRollOff(dtp.RollOff()));
                      offset += OSDROWHEIGHT;
                      OSDDRAWINFOLEFT( trVDR("Pilot"),     *getPilot(dtp.Pilot()));
                      }
                   }
                   break;

              case cSource::stCable: {
                   cDvbTransponderParameters dtp(channel->Parameters());
                   OSDDRAWINFOLINE(*cString::sprintf("DVB-C #%d - %s", (svdrpFrontendM >= 0) ? svdrpFrontendM : cDevice::ActualDevice()->CardIndex(), *frontendNameM));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("Frequency"),    *getFrequencyMHz(channel->Frequency()));
                   OSDDRAWINFORIGHT(trVDR("Source"),       *cSource::ToString(channel->Source()));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("Srate"),        *cString::sprintf("%d", channel->Srate()));
                   OSDDRAWINFORIGHT(trVDR("Modulation"),   *getModulation(dtp.Modulation()));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("Inversion"),    *getInversion(dtp.Inversion()));
                   OSDDRAWINFORIGHT(trVDR("CoderateH"),    *getCoderate(dtp.CoderateH()));
                   }
                   break;

              case cSource::stTerr: {
                   cDvbTransponderParameters dtp(channel->Parameters());
                   OSDDRAWINFOLINE(*cString::sprintf("%s #%d - %s", *getTerrestrialSystem(dtp.System()), (svdrpFrontendM >= 0) ? svdrpFrontendM : cDevice::ActualDevice()->CardIndex(), *frontendNameM));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("Frequency"),    *getFrequencyMHz(channel->Frequency()));
                   OSDDRAWINFORIGHT(trVDR("Transmission"), *getTransmission(dtp.Transmission()));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("Bandwidth"),    *getBandwidth(dtp.Bandwidth()));
                   OSDDRAWINFORIGHT(trVDR("Modulation"),   *getModulation(dtp.Modulation()));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("Inversion"),    *getInversion(dtp.Inversion()));
                   OSDDRAWINFORIGHT(tr   ("Coderate"),     *cString::sprintf("%s (H) %s (L)", *getCoderate(dtp.CoderateH()), *getCoderate(dtp.CoderateL())));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("Hierarchy"),    *getHierarchy(dtp.Hierarchy()));
                   OSDDRAWINFORIGHT(trVDR("Guard"),        *getGuard(dtp.Guard()));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("System"),       *getTerrestrialSystem(dtp.System()));
                   if (dtp.System()) {
                      OSDDRAWINFORIGHT(trVDR("StreamId"),  *cString::sprintf("%d", dtp.StreamId()));
                      offset += OSDROWHEIGHT;
                      OSDDRAWINFOLEFT( trVDR("T2SystemId"),*cString::sprintf("%d", dtp.T2SystemId()));
                      OSDDRAWINFORIGHT(trVDR("SISO/MISO"), *cString::sprintf("%d", dtp.SisoMiso()));
                      }
                   }
                   break;

              case stIptv: {
                   OSDDRAWINFOLINE(*cString::sprintf("IPTV #%d - %s", (svdrpFrontendM >= 0) ? svdrpFrontendM : cDevice::ActualDevice()->CardIndex(), *frontendNameM));
                   offset += OSDROWHEIGHT;
                   if (svdrpFrontendM < 0) {
                      cPlugin *p;
                      IptvService_v1_0 data;
                      data.cardIndex = cDevice::ActualDevice()->CardIndex();
                      p = cPluginManager::CallFirstService("IptvService-v1.0", &data);
                      if (p) {
                         OSDDRAWINFOLEFT(tr("Protocol"),   *data.protocol);
                         offset += OSDROWHEIGHT;
                         OSDDRAWINFOLEFT(tr("Bitrate"),    *data.bitrate);
                         }
                      }
                   }
                   break;

              default:
                   break;
              }
            OSDDRAWINFOBOTTOMBAR();
            break;

       case eFemonModeStream:
            OSDDRAWINFOTITLEBAR(tr("Stream Information"));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOACTIVE(  tr("Video Stream"),       *getVideoStream(channel->Vpid()));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOINACTIVE(tr("Codec"),              *getVideoCodec(receiverM ? receiverM->VideoCodec() : VIDEO_CODEC_INVALID));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOINACTIVE(tr("Bitrate"),            *getVideoBitrate(receiverM ? receiverM->VideoBitrate() : 0, receiverM ? receiverM->VideoStreamBitrate() : 0));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOINACTIVE(tr("Aspect Ratio"),       *getAspectRatio(receiverM ? receiverM->VideoAspectRatio() : VIDEO_ASPECT_RATIO_INVALID));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOINACTIVE(tr("Frame Rate"),         *getFrameRate(receiverM ? receiverM->VideoFrameRate() : 0));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOINACTIVE(tr("Video Format"),       *getVideoFormat(receiverM ? receiverM->VideoFormat() : VIDEO_CODEC_INVALID));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOINACTIVE(tr("Resolution"),         *getResolution(receiverM ? receiverM->VideoHorizontalSize() : 0, receiverM ? receiverM->VideoVerticalSize() : 0, receiverM ? receiverM->VideoScan() : VIDEO_SCAN_INVALID));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOACTIVE(  tr("Audio Stream"),       *getAudioStream(track, channel));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOINACTIVE(tr("Codec"),              *getAudioCodec(receiverM ? receiverM->AudioCodec() : AUDIO_CODEC_INVALID));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOINACTIVE(tr("Channel Mode"),       *getAudioChannelMode(receiverM ? receiverM->AudioChannelMode() : AUDIO_CHANNEL_MODE_INVALID));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOINACTIVE(tr("Bitrate"),            *getAudioBitrate(receiverM ? receiverM->AudioBitrate() : 0, receiverM ? receiverM->AudioStreamBitrate() : 0));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOINACTIVE(tr("Sampling Frequency"), *getAudioSamplingFreq(receiverM ? receiverM->AudioSamplingFreq() : AUDIO_SAMPLING_FREQUENCY_INVALID));
            OSDDRAWINFOBOTTOMBAR();
            break;

       case eFemonModeAC3:
            OSDDRAWINFOTITLEBAR(tr("Stream Information"));
            if (receiverM && receiverM->AC3Valid() && IS_DOLBY_TRACK(track)) {
               offset += OSDROWHEIGHT;
               OSDDRAWINFOACTIVE(  tr("AC-3 Stream"),            *getAC3Stream(track, channel));
               offset += OSDROWHEIGHT;
               OSDDRAWINFOINACTIVE(tr("Bitrate"),                *getAudioBitrate(receiverM->AC3Bitrate(), receiverM->AC3StreamBitrate()));
               offset += OSDROWHEIGHT;
               OSDDRAWINFOINACTIVE(tr("Sampling Frequency"),     *getAudioSamplingFreq(receiverM->AC3SamplingFreq()));
               offset += OSDROWHEIGHT;
               OSDDRAWINFOINACTIVE(tr("Bit Stream Mode"),        *getAC3BitStreamMode(receiverM->AC3BitStreamMode(), receiverM->AC3AudioCodingMode()));
               offset += OSDROWHEIGHT;
               OSDDRAWINFOINACTIVE(tr("Audio Coding Mode"),      *getAC3AudioCodingMode(receiverM->AC3AudioCodingMode(), receiverM->AC3BitStreamMode()));
               offset += OSDROWHEIGHT;
               OSDDRAWINFOINACTIVE(tr("Center Mix Level"),       *getAC3CenterMixLevel(receiverM->AC3CenterMixLevel()));
               offset += OSDROWHEIGHT;
               OSDDRAWINFOINACTIVE(tr("Surround Mix Level"),     *getAC3SurroundMixLevel(receiverM->AC3SurroundMixLevel()));
               offset += OSDROWHEIGHT;
               OSDDRAWINFOINACTIVE(tr("Dolby Surround Mode"),    *getAC3DolbySurroundMode(receiverM->AC3DolbySurroundMode()));
               offset += OSDROWHEIGHT;
               OSDDRAWINFOINACTIVE(tr("Low Frequency Effects"),  *cString::sprintf("%s", receiverM->AC3Lfe() ? trVDR("on") : trVDR("off")));
               offset += OSDROWHEIGHT;
               OSDDRAWINFOINACTIVE(tr("Dialogue Normalization"), *getAC3DialogLevel(receiverM->AC3DialogLevel()));
               }
            OSDDRAWINFOBOTTOMBAR();
            break;

       default:
            OSDCLEARINFO();
            break;
       }
     osdM->Flush();
     }
}

void cFemonOsd::Action(void)
{
  debug1("%s", __PRETTY_FUNCTION__);
  cTimeMs t;
  SvdrpCommand_v1_0 cmd;
  cmd.command = cString::sprintf("PLUG %s INFO\r\n", PLUGIN_NAME_I18N);
  while (Running()) {
    t.Set(0);
    svdrpFrontendM = -1;
    svdrpVideoBitRateM = -1.0;
    svdrpAudioBitRateM = -1.0;
    switch (deviceSourceM) {
      case DEVICESOURCE_PVRINPUT:
           qualityM = cDevice::ActualDevice()->SignalStrength();
           qualityValidM = (qualityM >= 0);
           strengthM = cDevice::ActualDevice()->SignalStrength();
           strengthValidM = (strengthM >= 0);
           frontendNameM = cDevice::ActualDevice()->DeviceName();
           frontendStatusM = (fe_status_t)(strengthValidM ? (FE_HAS_LOCK | FE_HAS_SIGNAL | FE_HAS_CARRIER | FE_HAS_VITERBI | FE_HAS_SYNC) : 0);
           frontendStatusValidM = strengthValidM;
           signalM = uint16_t(strengthM * 0xFFFF / 100);
           signalValidM = strengthValidM;
           snrM = 0;
           snrValidM = false;
           berM = 0;
           berValidM = false;
           uncM = 0;
           uncValidM = false;
           break;
      case DEVICESOURCE_IPTV:
           qualityM = cDevice::ActualDevice()->SignalQuality();
           qualityValidM = (qualityM >= 0);
           strengthM = cDevice::ActualDevice()->SignalStrength();
           strengthValidM = (strengthM >= 0);
           frontendNameM = cDevice::ActualDevice()->DeviceName();
           frontendStatusM = (fe_status_t)(strengthValidM ? (FE_HAS_LOCK | FE_HAS_SIGNAL | FE_HAS_CARRIER | FE_HAS_VITERBI | FE_HAS_SYNC) : 0);
           frontendStatusValidM = strengthValidM;
           signalM = uint16_t(strengthM * 0xFFFF / 100);
           signalValidM = strengthValidM;
           snrM = uint16_t(qualityM * 0xFFFF / 100);
           snrValidM = qualityValidM;
           berM = 0;
           berValidM = false;
           uncM = 0;
           uncValidM = false;
           break;
      default:
      case DEVICESOURCE_DVBAPI:
           if (frontendM != -1) {
              qualityM = cDevice::ActualDevice()->SignalQuality();
              qualityValidM = (qualityM >= 0);
              strengthM = cDevice::ActualDevice()->SignalStrength();
              strengthValidM = (strengthM >= 0);
              frontendNameM = cDevice::ActualDevice()->DeviceName();
              frontendStatusValidM = (ioctl(frontendM, FE_READ_STATUS, &frontendStatusM) >= 0);
              signalValidM = (ioctl(frontendM, FE_READ_SIGNAL_STRENGTH, &signalM) >= 0);
              snrValidM = (ioctl(frontendM, FE_READ_SNR, &snrM) >= 0);
              berValidM = (ioctl(frontendM, FE_READ_BER, &berM) >= 0);
              uncValidM = (ioctl(frontendM, FE_READ_UNCORRECTED_BLOCKS, &uncM) >= 0);
              }
           else if (strstr(*cDevice::ActualDevice()->DeviceType(), SATIP_DEVICE)) {
              qualityM = cDevice::ActualDevice()->SignalQuality();
              qualityValidM = (qualityM >= 0);
              strengthM = cDevice::ActualDevice()->SignalStrength();
              strengthValidM = (strengthM >= 0);
              frontendNameM = cDevice::ActualDevice()->DeviceName();
              frontendStatusM = (fe_status_t)(cDevice::ActualDevice()->HasLock() ? (FE_HAS_LOCK | FE_HAS_SIGNAL | FE_HAS_CARRIER | FE_HAS_VITERBI | FE_HAS_SYNC) : 0);
              frontendStatusValidM = strengthValidM;
              signalM = uint16_t(strengthM * 0xFFFF / 100);
              signalValidM = strengthValidM;
              snrM = uint16_t(qualityM * 0xFFFF / 100);
              snrValidM = qualityValidM;
              berM = 0;
              berValidM = false;
              uncM = 0;
              uncValidM = false;
              }
           else if (svdrpConnectionM.handle >= 0) {
              cmd.handle = svdrpConnectionM.handle;
              svdrpPluginM->Service("SvdrpCommand-v1.0", &cmd);
              if (cmd.responseCode == 900) {
                 strengthValidM = false;
                 qualityValidM = false;
                 frontendStatusValidM = false;
                 signalValidM = false;
                 snrValidM = false;
                 berValidM = false;
                 uncValidM = false;
                 for (cLine *line = cmd.reply.First(); line; line = cmd.reply.Next(line)) {
                     const char *s = line->Text();
	             if (!strncasecmp(s, "CARD:", 5))
                        svdrpFrontendM = (int)strtol(s + 5, NULL, 10);
                     else if (!strncasecmp(s, "STRG:", 5)) {
                        strengthM = (int)strtol(s + 5, NULL, 10);
                        strengthValidM = (strengthM >= 0);
                        }
                     else if (!strncasecmp(s, "QUAL:", 5)) {
                        qualityM = (int)strtol(s + 5, NULL, 10);
                        qualityValidM = (qualityM >= 0);
                        }
                     else if (!strncasecmp(s, "TYPE:", 5))
                        frontendInfoM.type = (fe_type_t)strtol(s + 5, NULL, 10);
                     else if (!strncasecmp(s, "NAME:", 5)) {
                        frontendNameM = s + 5;
                        }
                     else if (!strncasecmp(s, "STAT:", 5)) {
                        frontendStatusM = (fe_status_t)strtol(s + 5, NULL, 16);
                        frontendStatusValidM = true;
                        }
                     else if (!strncasecmp(s, "SGNL:", 5)) {
                        signalM = (uint16_t)strtol(s + 5, NULL, 16);
                        signalValidM = true;
                        }
                     else if (!strncasecmp(s, "SNRA:", 5)) {
                        snrM = (uint16_t)strtol(s + 5, NULL, 16);
                        snrValidM = true;
                        }
                     else if (!strncasecmp(s, "BERA:", 5)) {
                        berM = (uint32_t)strtol(s + 5, NULL, 16);
                        berValidM = true;
                        }
                     else if (!strncasecmp(s, "UNCB:", 5)) {
                        uncM = (uint32_t)strtol(s + 5, NULL, 16);
                        uncValidM = true;
                        }
                     else if (!strncasecmp(s, "VIBR:", 5))
                        svdrpVideoBitRateM = (double)strtol(s + 5, NULL, 10);
                     else if (!strncasecmp(s, "AUBR:", 5))
                        svdrpAudioBitRateM = (double)strtol(s + 5, NULL, 10);
                     }
                 }
              }
           break;
      }
    DrawInfoWindow();
    DrawStatusWindow();
    sleepM.Wait(max((int)(100 * FemonConfig.GetUpdateInterval() - t.Elapsed()), 3));
    }
}

void cFemonOsd::Show(void)
{
  debug1("%s", __PRETTY_FUNCTION__);
  eTrackType track = cDevice::PrimaryDevice()->GetCurrentAudioTrack();
  LOCK_CHANNELS_READ;
  const cChannel *channel = Channels->GetByNumber(cDevice::CurrentChannel());

  AttachFrontend();

  osdM = cOsdProvider::NewOsd(osdLeftM, osdTopM);
  if (osdM) {
     tArea Areas1[] = { { 0, 0, OSDWIDTH - 1, OSDHEIGHT - 1, 8 } };
     if (Setup.AntiAlias && osdM->CanHandleAreas(Areas1, sizeof(Areas1) / sizeof(tArea)) == oeOk) {
        osdM->SetAreas(Areas1, sizeof(Areas1) / sizeof(tArea));
        }
     else {
        tArea Areas2[] = { { 0, OSDSTATUSWIN_Y(0),          OSDWIDTH - 1, OSDSTATUSWIN_Y(0) + OSDSTATUSHEIGHT - 1, FemonTheme[FemonConfig.GetTheme()].bpp },
                           { 0, OSDINFOWIN_Y(0),            OSDWIDTH - 1, OSDINFOWIN_Y(0)   + OSDROWHEIGHT    - 1, FemonTheme[FemonConfig.GetTheme()].bpp },
                           { 0, OSDINFOWIN_Y(OSDROWHEIGHT), OSDWIDTH - 1, OSDINFOWIN_Y(0)   + OSDINFOHEIGHT   - 1, 2                                 } };
        osdM->SetAreas(Areas2, sizeof(Areas2) / sizeof(tArea));
        }
     OSDCLEARSTATUS();
     OSDCLEARINFO();
     osdM->Flush();
     if (receiverM) {
        receiverM->Deactivate();
        DELETENULL(receiverM);
        }
     if (FemonConfig.GetAnalyzeStream() && channel) {
        receiverM = new cFemonReceiver(channel, IS_AUDIO_TRACK(track) ? int(track - ttAudioFirst) : 0, IS_DOLBY_TRACK(track) ? int(track - ttDolbyFirst) : 0);
        cDevice::ActualDevice()->AttachReceiver(receiverM);
        }
     Start();
     }
}

bool cFemonOsd::AttachFrontend(void)
{
  debug1("%s", __PRETTY_FUNCTION__);
  LOCK_CHANNELS_READ;
  const cChannel *channel = Channels->GetByNumber(cDevice::CurrentChannel());

  if (frontendM >= 0) {
     close(frontendM);
     frontendM = -1;
     }

  deviceSourceM = DEVICESOURCE_DVBAPI;
  if (channel) {
     if (channel->IsSourceType('I'))
        deviceSourceM = DEVICESOURCE_IPTV;
     else if (channel->IsSourceType('V'))
        deviceSourceM = DEVICESOURCE_PVRINPUT;
     }

  if (deviceSourceM == DEVICESOURCE_DVBAPI) {
     if (!strstr(*cDevice::ActualDevice()->DeviceType(), SATIP_DEVICE)) {
        cDvbDevice *dev = getDvbDevice(cDevice::ActualDevice());
        frontendM = dev ? open(*cString::sprintf(FRONTEND_DEVICE, dev->Adapter(), dev->Frontend()), O_RDONLY | O_NONBLOCK) : -1;
        if (frontendM >= 0) {
           if (ioctl(frontendM, FE_GET_INFO, &frontendInfoM) < 0) {
              if (!FemonConfig.GetUseSvdrp())
                 error("%s Cannot read frontend info", __PRETTY_FUNCTION__);
              close(frontendM);
              frontendM = -1;
              memset(&frontendInfoM, 0, sizeof(frontendInfoM));
              return false;
              }
           }
        else if (FemonConfig.GetUseSvdrp()) {
           if (!SvdrpConnect() || !SvdrpTune())
              return false;
           }
        else {
           error("%s Cannot open frontend device", __PRETTY_FUNCTION__);
           return false;
           }
        }
     }
  else
     frontendM = -1;

  return true;
}

void cFemonOsd::ChannelSwitch(const cDevice * deviceP, int channelNumberP, bool liveViewP)
{
  debug1("%s (%d, %d, %d)", __PRETTY_FUNCTION__, deviceP->DeviceNumber(), channelNumberP, liveViewP);
  eTrackType track = cDevice::PrimaryDevice()->GetCurrentAudioTrack();
  LOCK_CHANNELS_READ;
  const cChannel *channel = Channels->GetByNumber(cDevice::CurrentChannel());

  if (!deviceP || !liveViewP)
     return;

  if (!channelNumberP) {
     if (receiverM) {
        receiverM->Deactivate();
        DELETENULL(receiverM);
        }
     return;
     }

  if (channel && FemonConfig.GetAnalyzeStream() && AttachFrontend()) {
     if (receiverM) {
        receiverM->Deactivate();
        DELETENULL(receiverM);
        }
     receiverM = new cFemonReceiver(channel, IS_AUDIO_TRACK(track) ? int(track - ttAudioFirst) : 0, IS_DOLBY_TRACK(track) ? int(track - ttDolbyFirst) : 0);
     cDevice::ActualDevice()->AttachReceiver(receiverM);
     }
}

void cFemonOsd::SetAudioTrack(int indexP, const char * const *tracksP)
{
  debug1("%s (%d, )", __PRETTY_FUNCTION__, indexP);
  eTrackType track = cDevice::PrimaryDevice()->GetCurrentAudioTrack();
  if (receiverM) {
     receiverM->Deactivate();
     DELETENULL(receiverM);
     }
  if (FemonConfig.GetAnalyzeStream()) {
     LOCK_CHANNELS_READ;
     const cChannel *channel = Channels->GetByNumber(cDevice::CurrentChannel());
     if (channel) {
        receiverM = new cFemonReceiver(channel, IS_AUDIO_TRACK(track) ? int(track - ttAudioFirst) : 0, IS_DOLBY_TRACK(track) ? int(track - ttDolbyFirst) : 0);
        cDevice::ActualDevice()->AttachReceiver(receiverM);
        }
     }
}

bool cFemonOsd::DeviceSwitch(int directionP)
{
  debug1("%s (%d)", __PRETTY_FUNCTION__, directionP);
  int device = cDevice::ActualDevice()->DeviceNumber();
  int direction = sgn(directionP);
  if (device >= 0) {
     LOCK_CHANNELS_READ;
     const cChannel *channel = Channels->GetByNumber(cDevice::CurrentChannel());
     if (channel) {
        for (int i = 0; i < cDevice::NumDevices() - 1; i++) {
            if (direction >= 0) {
               if (++device >= cDevice::NumDevices())
                  device = 0;
               }
            else {
               if (--device < 0)
                  device = cDevice::NumDevices() - 1;
               }
            // Collect the current priorities of all CAM slots that can decrypt the channel:
            int NumCamSlots = CamSlots.Count();
            int SlotPriority[NumCamSlots];
            int NumUsableSlots = 0;
            bool NeedsDetachAllReceivers = false;
            bool InternalCamNeeded = false;
            bool ValidDevice = false;
            cCamSlot *s = NULL;
            cDevice *d = cDevice::GetDevice(device);
            if (channel->Ca() >= CA_ENCRYPTED_MIN) {
               for (cCamSlot *CamSlot = CamSlots.First(); CamSlot; CamSlot = CamSlots.Next(CamSlot)) {
                   SlotPriority[CamSlot->Index()] = MAXPRIORITY + 1; // assumes it can't be used
                   if (CamSlot->ModuleStatus() == msReady) {
                      if (CamSlot->ProvidesCa(channel->Caids())) {
                         if (!ChannelCamRelations.CamChecked(channel->GetChannelID(), CamSlot->SlotNumber())) {
                            SlotPriority[CamSlot->Index()] = CamSlot->Priority();
                            NumUsableSlots++;
                            }
                         }
                      }
                  }
               if (!NumUsableSlots)
                  InternalCamNeeded = true; // no CAM is able to decrypt this channel
               }
            for (int j = 0; j < NumCamSlots || !NumUsableSlots; ++j) {
                if (NumUsableSlots && SlotPriority[j] > MAXPRIORITY)
                   continue; // there is no CAM available in this slot
                bool HasInternalCam = d->HasInternalCam();
                if (InternalCamNeeded && !HasInternalCam)
                   continue; // no CAM is able to decrypt this channel and the device uses vdr handled CAMs
                if (NumUsableSlots && !HasInternalCam && !CamSlots.Get(j)->Assign(d, true))
                   continue; // CAM slot can't be used with this device
                if (d->ProvidesChannel(channel, 0, &NeedsDetachAllReceivers)) { // this device is basically able to do the job
                   debug1("%s (%d) device=%d", __PRETTY_FUNCTION__, direction, device);
                   if (NumUsableSlots && !HasInternalCam && d->CamSlot() && d->CamSlot() != CamSlots.Get(j))
                      NeedsDetachAllReceivers = true; // using a different CAM slot requires detaching receivers
                   if (NumUsableSlots && !HasInternalCam)
                      s = CamSlots.Get(j);
                   ValidDevice = true;
                   break;
                   }
                if (!NumUsableSlots)
                   break; // no CAM necessary, so just one loop over the devices
                }
            // Do the actual switch if valid device found
            if (d && ValidDevice) {
               cControl::Shutdown();
               if (NeedsDetachAllReceivers)
                  d->DetachAllReceivers();
               if (s) {
                  if (s->Device() != d) {
                     if (s->Device())
                        s->Device()->DetachAllReceivers();
                     if (d->CamSlot())
                        d->CamSlot()->Assign(NULL);
                     s->Assign(d);
                     }
                  }
               else if (d->CamSlot() && !d->CamSlot()->IsDecrypting())
                  d->CamSlot()->Assign(NULL);
               d->SwitchChannel(channel, false);
               cControl::Launch(new cTransferControl(d, channel));
               AttachFrontend();
               return true;
               }
            }
        }
     }
   return false;
}

bool cFemonOsd::SvdrpConnect(void)
{
   if (svdrpConnectionM.handle < 0) {
      svdrpPluginM = cPluginManager::GetPlugin(SVDRPPLUGIN);
      if (svdrpPluginM) {
         svdrpConnectionM.serverIp = FemonConfig.GetSvdrpIp();
         svdrpConnectionM.serverPort = (unsigned short)FemonConfig.GetSvdrpPort();
         svdrpConnectionM.shared = true;
         svdrpPluginM->Service("SvdrpConnection-v1.0", &svdrpConnectionM);
         if (svdrpConnectionM.handle >= 0) {
            SvdrpCommand_v1_0 cmd;
            cmd.handle = svdrpConnectionM.handle;
            cmd.command = cString::sprintf("PLUG %s\r\n", PLUGIN_NAME_I18N);
            svdrpPluginM->Service("SvdrpCommand-v1.0", &cmd);
            if (cmd.responseCode != 214) {
               svdrpPluginM->Service("SvdrpConnection-v1.0", &svdrpConnectionM); // close connection
               error("%s Cannot find plugin '%s' on server %s", __PRETTY_FUNCTION__, PLUGIN_NAME_I18N, *svdrpConnectionM.serverIp);
               }
            }
         else
            error("%s Cannot connect to SVDRP server", __PRETTY_FUNCTION__);
         }
      else
         error("%s Cannot find plugin '%s'", __PRETTY_FUNCTION__, SVDRPPLUGIN);
      }
   return svdrpConnectionM.handle >= 0;
}

bool cFemonOsd::SvdrpTune(void)
{
   if (svdrpPluginM && svdrpConnectionM.handle >= 0) {
      LOCK_CHANNELS_READ;
      const cChannel *channel = Channels->GetByNumber(cDevice::CurrentChannel());
      if (channel) {
         SvdrpCommand_v1_0 cmd;
         cmd.handle = svdrpConnectionM.handle;
         cmd.command = cString::sprintf("CHAN %s\r\n", *channel->GetChannelID().ToString());
         svdrpPluginM->Service("SvdrpCommand-v1.0", &cmd);
         if (cmd.responseCode == 250)
            return true;
         error("%s Cannot tune server channel", __PRETTY_FUNCTION__);
         }
      else
         error("%s Invalid channel", __PRETTY_FUNCTION__);
      }
   else
      error("%s Unexpected connection state", __PRETTY_FUNCTION__);
   return false;
}

double cFemonOsd::GetVideoBitrate(void)
{
  debug1("%s", __PRETTY_FUNCTION__);
  double value = 0.0;

  if (receiverM)
     value = receiverM->VideoBitrate();

  return (value);
}

double cFemonOsd::GetAudioBitrate(void)
{
  debug1("%s", __PRETTY_FUNCTION__);
  double value = 0.0;

  if (receiverM)
     value = receiverM->AudioBitrate();

  return (value);
}

double cFemonOsd::GetDolbyBitrate(void)
{
  debug1("%s", __PRETTY_FUNCTION__);
  double value = 0.0;

  if (receiverM)
     value = receiverM->AC3Bitrate();

  return (value);
}

eOSState cFemonOsd::ProcessKey(eKeys keyP)
{
  eOSState state = cOsdObject::ProcessKey(keyP);
  if (state == osUnknown) {
     switch (int(keyP)) {
       case k0:
            if ((numberM == 0) && (oldNumberM != 0)) {
               numberM = oldNumberM;
               oldNumberM = cDevice::CurrentChannel();
               LOCK_CHANNELS_READ;
               Channels->SwitchTo(numberM);
               numberM = 0;
               return osContinue;
               }
       case k1 ... k9:
            if (numberM >= 0) {
               numberM = numberM * 10 + keyP - k0;
               if (numberM > 0) {
                  DrawStatusWindow();
                  LOCK_CHANNELS_READ;
                  const cChannel *ch = Channels->GetByNumber(numberM);
                  inputTimeM.Set(0);
                  // Lets see if there can be any useful further input:
                  int n = ch ? numberM * 10 : 0;
                  while (ch && (ch = Channels->Next(ch)) != NULL) {
                        if (!ch->GroupSep()) {
                           if (n <= ch->Number() && ch->Number() <= n + 9) {
                              n = 0;
                              break;
                              }
                           if (ch->Number() > n)
                              n *= 10;
                           }
                        }
                  if (n > 0) {
                     // This channel is the only one that fits the input, so let's take it right away:
                     oldNumberM = cDevice::CurrentChannel();
                     Channels->SwitchTo(numberM);
                     numberM = 0;
                     }
                  }
               }
            break;
       case kBack:
            return osEnd;
       case kGreen:
            {
            eTrackType types[ttMaxTrackTypes];
            eTrackType CurrentAudioTrack = cDevice::PrimaryDevice()->GetCurrentAudioTrack();
            int numTracks = 0;
            int oldTrack = 0;
            int track = 0;
            for (int i = ttAudioFirst; i <= ttDolbyLast; i++) {
                const tTrackId *TrackId = cDevice::PrimaryDevice()->GetTrack(eTrackType(i));
                if (TrackId && TrackId->id) {
                   types[numTracks] = eTrackType(i);
                   if (i == CurrentAudioTrack)
                      track = numTracks;
                   numTracks++;
                   }
                }
            oldTrack = track;
            if (++track >= numTracks)
               track = 0;
            if (track != oldTrack) {
               cDevice::PrimaryDevice()->SetCurrentAudioTrack(types[track]);
               Setup.CurrentDolby = IS_DOLBY_TRACK(types[track]);
               }
            }
            break;
       case kYellow:
            if (IS_AUDIO_TRACK(cDevice::PrimaryDevice()->GetCurrentAudioTrack())) {
               int audioChannel = cDevice::PrimaryDevice()->GetAudioChannel();
               int oldAudioChannel = audioChannel;
               if (++audioChannel > 2)
                  audioChannel = 0;
               if (audioChannel != oldAudioChannel) {
                  cDevice::PrimaryDevice()->SetAudioChannel(audioChannel);
                  }
               }
            break;
       case kRight:
            DeviceSwitch(1);
            break;
       case kLeft:
            DeviceSwitch(-1);
            break;
       case kUp|k_Repeat:
       case kUp:
       case kDown|k_Repeat:
       case kDown:
            oldNumberM = cDevice::CurrentChannel();
            cDevice::SwitchChannel(NORMALKEY(keyP) == kUp ? 1 : -1);
            numberM = 0;
            break;
       case kNone:
            if (numberM && (inputTimeM.Elapsed() > CHANNELINPUT_TIMEOUT)) {
               LOCK_CHANNELS_READ;
               if (Channels->GetByNumber(numberM)) {
                  oldNumberM = cDevice::CurrentChannel();
                  Channels->SwitchTo(numberM);
                  numberM = 0;
                  }
               else {
                  inputTimeM.Set(0);
                  numberM = 0;
                  }
               }
            break;
       case kOk:
            {
            // toggle between display modes
            LOCK_CHANNELS_READ;
            const cChannel *channel = Channels->GetByNumber(cDevice::CurrentChannel());
            if (++displayModeM == eFemonModeAC3 && channel && !channel->Dpid(0)) displayModeM++;
            if (displayModeM >= eFemonModeMaxNumber) displayModeM = 0;
            DrawInfoWindow();
            }
            break;
       default:
            break;
       }
     state = osContinue;
     }
  return state;
}
