/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include <ctype.h>
#include "femoncfg.h"
#include "femonreceiver.h"
#include "femontools.h"
#include "femonsymbol.h"
#include "femonosd.h"

#define CHANNELINPUT_TIMEOUT      1000
#define SVDRPPLUGIN               "svdrpservice"

#define OSDHEIGHT                 femonConfig.osdheight   // in pixels
#define OSDWIDTH                  600                     // in pixels
#define OSDROWHEIGHT              m_Font->Height()        // in pixels
#define OSDINFOHEIGHT             (OSDROWHEIGHT * 13)     // in pixels (13 rows)
#define OSDSTATUSHEIGHT           (OSDROWHEIGHT * 6)      // in pixels (6 rows)
#define OSDSPACING                5
#define OSDROUNDING               10
#define IS_OSDROUNDING            (femonConfig.skin == eFemonSkinElchi)

#define OSDINFOWIN_Y(offset)      (femonConfig.position ? (OSDHEIGHT - OSDINFOHEIGHT + offset) : offset)
#define OSDINFOWIN_X(col)         ((col == 4) ? 455 : (col == 3) ? 305 : (col == 2) ? 155 : 15)
#define OSDSTATUSWIN_Y(offset)    (femonConfig.position ? offset : (OSDHEIGHT - OSDSTATUSHEIGHT + offset))
#define OSDSTATUSWIN_X(col)       ((col == 7) ? 475 : (col == 6) ? 410 : (col == 5) ? 275 : (col == 4) ? 220 : (col == 3) ? 125 : (col == 2) ? 70 : 15)
#define OSDSTATUSWIN_XSYMBOL(c,w) (c * ((OSDWIDTH - (5 * w)) / 6) + ((c - 1) * w))
#define OSDBARWIDTH(x)            (OSDWIDTH * x / 100)

#define OSDDRAWSTATUSBM(spacing) \
        if (bm) { \
           x -= bm->Width() + spacing; \
           y = (OSDROWHEIGHT - bm->Height()) / 2; \
           if (y < 0) y = 0; \
           m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset) + y, *bm, femonTheme[femonConfig.theme].clrTitleText, femonTheme[femonConfig.theme].clrTitleBackground); \
           }

#define OSDDRAWSTATUSFRONTEND(column, bitmap, status) \
        m_Osd->DrawBitmap(OSDSTATUSWIN_XSYMBOL(column, x), OSDSTATUSWIN_Y(offset) + y, bitmap, (m_FrontendStatus & status) ? femonTheme[femonConfig.theme].clrActiveText : femonTheme[femonConfig.theme].clrRed, femonTheme[femonConfig.theme].clrBackground)

#define OSDDRAWSTATUSVALUES(label1, label2, label3, label4, label5, label6, label7) \
        m_Osd->DrawText(OSDSTATUSWIN_X(1), OSDSTATUSWIN_Y(offset), label1, femonTheme[femonConfig.theme].clrInactiveText, femonTheme[femonConfig.theme].clrBackground, m_Font); \
        m_Osd->DrawText(OSDSTATUSWIN_X(2), OSDSTATUSWIN_Y(offset), label2, femonTheme[femonConfig.theme].clrInactiveText, femonTheme[femonConfig.theme].clrBackground, m_Font); \
        m_Osd->DrawText(OSDSTATUSWIN_X(3), OSDSTATUSWIN_Y(offset), label3, femonTheme[femonConfig.theme].clrInactiveText, femonTheme[femonConfig.theme].clrBackground, m_Font); \
        m_Osd->DrawText(OSDSTATUSWIN_X(4), OSDSTATUSWIN_Y(offset), label4, femonTheme[femonConfig.theme].clrInactiveText, femonTheme[femonConfig.theme].clrBackground, m_Font); \
        m_Osd->DrawText(OSDSTATUSWIN_X(5), OSDSTATUSWIN_Y(offset), label5, femonTheme[femonConfig.theme].clrInactiveText, femonTheme[femonConfig.theme].clrBackground, m_Font); \
        m_Osd->DrawText(OSDSTATUSWIN_X(6), OSDSTATUSWIN_Y(offset), label6, femonTheme[femonConfig.theme].clrInactiveText, femonTheme[femonConfig.theme].clrBackground, m_Font); \
        m_Osd->DrawText(OSDSTATUSWIN_X(7), OSDSTATUSWIN_Y(offset), label7, femonTheme[femonConfig.theme].clrInactiveText, femonTheme[femonConfig.theme].clrBackground, m_Font)

#define OSDDRAWSTATUSBAR(value) \
        if (value > 0) { \
           value = OSDBARWIDTH(value); \
           m_Osd->DrawRectangle(0, OSDSTATUSWIN_Y(offset) + 3, min(OSDBARWIDTH(femonConfig.redlimit), value), OSDSTATUSWIN_Y(offset) + OSDROWHEIGHT - 3, femonTheme[femonConfig.theme].clrRed); \
           if (value > OSDBARWIDTH(femonConfig.redlimit)) \
              m_Osd->DrawRectangle(OSDBARWIDTH(femonConfig.redlimit), OSDSTATUSWIN_Y(offset) + 3, min((OSDWIDTH * femonConfig.greenlimit / 100), value), OSDSTATUSWIN_Y(offset) + OSDROWHEIGHT - 3, femonTheme[femonConfig.theme].clrYellow); \
           if (value > OSDBARWIDTH(femonConfig.greenlimit)) \
              m_Osd->DrawRectangle(OSDBARWIDTH(femonConfig.greenlimit), OSDSTATUSWIN_Y(offset) + 3, value, OSDSTATUSWIN_Y(offset) + OSDROWHEIGHT - 3, femonTheme[femonConfig.theme].clrGreen); \
           }

#define OSDDRAWSTATUSTITLEBAR(title) \
        m_Osd->DrawRectangle(0, OSDSTATUSWIN_Y(offset), OSDWIDTH, OSDSTATUSWIN_Y(offset) + OSDROWHEIGHT - 1, femonTheme[femonConfig.theme].clrTitleBackground); \
        m_Osd->DrawText(OSDSTATUSWIN_X(1), OSDSTATUSWIN_Y(offset), title, femonTheme[femonConfig.theme].clrTitleText, femonTheme[femonConfig.theme].clrTitleBackground, m_Font); \
        if (IS_OSDROUNDING) { \
           m_Osd->DrawEllipse(0, OSDSTATUSWIN_Y(0), OSDROUNDING, OSDSTATUSWIN_Y(OSDROUNDING), clrTransparent, -2); \
           m_Osd->DrawEllipse(OSDWIDTH - OSDROUNDING, OSDSTATUSWIN_Y(0), OSDWIDTH, OSDSTATUSWIN_Y(OSDROUNDING), clrTransparent, -1); \
           } \
        m_Osd->DrawRectangle(0, OSDSTATUSWIN_Y(offset) + OSDROWHEIGHT, OSDWIDTH, OSDSTATUSWIN_Y(offset) + OSDSTATUSHEIGHT - 1, femonTheme[femonConfig.theme].clrBackground)

#define OSDDRAWSTATUSBOTTOMBAR() \
        if (IS_OSDROUNDING) { \
           m_Osd->DrawEllipse(0, OSDSTATUSWIN_Y(OSDSTATUSHEIGHT) - OSDROUNDING, OSDROUNDING, OSDSTATUSWIN_Y(OSDSTATUSHEIGHT), clrTransparent, -3); \
           m_Osd->DrawEllipse(OSDWIDTH - OSDROUNDING, OSDSTATUSWIN_Y(OSDSTATUSHEIGHT) - OSDROUNDING, OSDWIDTH, OSDSTATUSWIN_Y(OSDSTATUSHEIGHT), clrTransparent, -4); \
           }

#define OSDCLEARSTATUS() \
        m_Osd->DrawRectangle(0, OSDSTATUSWIN_Y(0), OSDWIDTH, OSDSTATUSWIN_Y(OSDSTATUSHEIGHT) - 1, clrTransparent)

#define OSDDRAWINFOLEFT(label, value) \
        m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), label, femonTheme[femonConfig.theme].clrInactiveText, femonTheme[femonConfig.theme].clrBackground, m_Font); \
        m_Osd->DrawText(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), value, femonTheme[femonConfig.theme].clrActiveText, femonTheme[femonConfig.theme].clrBackground, m_Font)

#define OSDDRAWINFORIGHT(label, value) \
        m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), label, femonTheme[femonConfig.theme].clrInactiveText, femonTheme[femonConfig.theme].clrBackground, m_Font); \
        m_Osd->DrawText(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), value, femonTheme[femonConfig.theme].clrActiveText, femonTheme[femonConfig.theme].clrBackground, m_Font)

#define OSDDRAWINFOACTIVE(label, value) \
        m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), label, femonTheme[femonConfig.theme].clrActiveText, femonTheme[femonConfig.theme].clrBackground, m_Font); \
        m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), value, femonTheme[femonConfig.theme].clrActiveText, femonTheme[femonConfig.theme].clrBackground, m_Font)

#define OSDDRAWINFOINACTIVE(label, value) \
        m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), label, femonTheme[femonConfig.theme].clrInactiveText, femonTheme[femonConfig.theme].clrBackground, m_Font); \
        m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), value, femonTheme[femonConfig.theme].clrActiveText, femonTheme[femonConfig.theme].clrBackground, m_Font)

#define OSDDRAWINFOLINE(label) \
        m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), label, femonTheme[femonConfig.theme].clrActiveText, femonTheme[femonConfig.theme].clrBackground, m_Font)

#define OSDDRAWINFOTITLEBAR(title) \
        m_Osd->DrawRectangle(0, OSDINFOWIN_Y(offset), OSDWIDTH, OSDINFOWIN_Y(offset) + OSDROWHEIGHT - 1, femonTheme[femonConfig.theme].clrTitleBackground); \
        m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), title, femonTheme[femonConfig.theme].clrTitleText, femonTheme[femonConfig.theme].clrTitleBackground, m_Font); \
        if (IS_OSDROUNDING) { \
           m_Osd->DrawEllipse(0, OSDINFOWIN_Y(0), OSDROUNDING, OSDINFOWIN_Y(OSDROUNDING), clrTransparent, -2); \
           m_Osd->DrawEllipse(OSDWIDTH - OSDROUNDING, OSDINFOWIN_Y(0), OSDWIDTH, OSDINFOWIN_Y(OSDROUNDING), clrTransparent, -1); \
           } \
        m_Osd->DrawRectangle(0, OSDINFOWIN_Y(offset) + OSDROWHEIGHT, OSDWIDTH, OSDINFOWIN_Y(offset) + OSDINFOHEIGHT - 1, femonTheme[femonConfig.theme].clrBackground)

#define OSDDRAWINFOBOTTOMBAR() \
        if (IS_OSDROUNDING) { \
           m_Osd->DrawEllipse(0, OSDINFOWIN_Y(OSDINFOHEIGHT) - OSDROUNDING, OSDROUNDING, OSDINFOWIN_Y(OSDINFOHEIGHT), clrTransparent, -3); \
           m_Osd->DrawEllipse((OSDWIDTH - OSDROUNDING), OSDINFOWIN_Y(OSDINFOHEIGHT) - OSDROUNDING, OSDWIDTH, OSDINFOWIN_Y(OSDINFOHEIGHT), clrTransparent, -4); \
           }

#define OSDCLEARINFO() \
        m_Osd->DrawRectangle(0, OSDINFOWIN_Y(0), OSDWIDTH, OSDINFOWIN_Y(OSDINFOHEIGHT) - 1, clrTransparent)

cFemonOsd *cFemonOsd::pInstance = NULL;

cFemonOsd *cFemonOsd::Instance(bool create)
{
  Dprintf("%s()\n", __PRETTY_FUNCTION__);
  if (pInstance == NULL && create)
  {
     pInstance = new cFemonOsd();
  }
  return (pInstance);
}

cFemonOsd::cFemonOsd()
: cOsdObject(true), cThread("femon osd"),
  m_Osd(NULL),
  m_Receiver(NULL),
  m_Frontend(-1),
  m_SvdrpFrontend(-1),
  m_SvdrpVideoBitrate(-1),
  m_SvdrpAudioBitrate(-1),
  m_SvdrpPlugin(NULL),
  m_Number(0),
  m_OldNumber(0),
  m_SNR(0),
  m_Signal(0),
  m_BER(0),
  m_UNC(0),
  m_DisplayMode(femonConfig.displaymode),
  m_InputTime(0),
  m_Sleep(),
  m_Mutex()
{
  Dprintf("%s()\n", __PRETTY_FUNCTION__);
  m_SvdrpConnection.handle = -1;
  if (Setup.UseSmallFont == 0) {
     // Dirty hack to force the small fonts...
     Setup.UseSmallFont = 1;
     m_Font = cFont::GetFont(fontSml);
     Setup.UseSmallFont = 0;
     }
  else
     m_Font = cFont::GetFont(fontSml);
  if (OSDHEIGHT < (OSDINFOHEIGHT + OSDROWHEIGHT + OSDSTATUSHEIGHT))
     OSDHEIGHT = (OSDINFOHEIGHT + OSDROWHEIGHT + OSDSTATUSHEIGHT);
}

cFemonOsd::~cFemonOsd(void)
{
  Dprintf("%s()\n", __PRETTY_FUNCTION__);
  m_Sleep.Signal();
  if (Running())
     Cancel(3);
  if (m_SvdrpConnection.handle >= 0) {
     m_SvdrpPlugin = cPluginManager::GetPlugin(SVDRPPLUGIN);
     if (m_SvdrpPlugin)
        m_SvdrpPlugin->Service("SvdrpConnection-v1.0", &m_SvdrpConnection);
     }
  if (m_Receiver) {
     m_Receiver->Deactivate();
     DELETENULL(m_Receiver);
     }
  if (m_Osd)
     DELETENULL(m_Osd);
  pInstance = NULL;
}

void cFemonOsd::DrawStatusWindow(void)
{
  cMutexLock lock(&m_Mutex);
  cBitmap *bm = NULL;
  int snr = m_SNR / 655;
  int signal = m_Signal / 655;
  int offset = 0;
  int x = OSDWIDTH - OSDROUNDING;
  int y = 0;
  eTrackType track = cDevice::PrimaryDevice()->GetCurrentAudioTrack();
  cChannel *channel = Channels.GetByNumber(cDevice::CurrentChannel());

  if (m_Osd && channel) {
     OSDDRAWSTATUSTITLEBAR(*cString::sprintf("%d%s %s", m_Number ? m_Number : channel->Number(), m_Number ? "-" : "", channel->ShortName(true)));
     if (m_SvdrpFrontend >= 0) {
        bm = &bmSymbol[SYMBOL_SVDRP];
        OSDDRAWSTATUSBM(OSDSPACING);
        }
     switch (cDevice::ActualDevice()->CardIndex()) {
       case 1:  bm = &bmSymbol[SYMBOL_ONE];   break;
       case 2:  bm = &bmSymbol[SYMBOL_TWO];   break;
       case 3:  bm = &bmSymbol[SYMBOL_THREE]; break;
       case 4:  bm = &bmSymbol[SYMBOL_FOUR];  break;
       case 5:  bm = &bmSymbol[SYMBOL_FIVE];  break;
       case 6:  bm = &bmSymbol[SYMBOL_SIX];   break;
       case 7:  bm = &bmSymbol[SYMBOL_SEVEN]; break;
       default: bm = &bmSymbol[SYMBOL_ZERO];  break;
       }
     OSDDRAWSTATUSBM(OSDSPACING);
     bm = &bmSymbol[SYMBOL_DEVICE];
     OSDDRAWSTATUSBM(0);
     if (IS_AUDIO_TRACK(track)) {
        switch (int(track - ttAudioFirst)) {
           case 1:  bm = &bmSymbol[SYMBOL_ONE];   break;
           case 2:  bm = &bmSymbol[SYMBOL_TWO];   break;
           case 3:  bm = &bmSymbol[SYMBOL_THREE]; break;
           case 4:  bm = &bmSymbol[SYMBOL_FOUR];  break;
           case 5:  bm = &bmSymbol[SYMBOL_FIVE];  break;
           case 6:  bm = &bmSymbol[SYMBOL_SIX];   break;
           case 7:  bm = &bmSymbol[SYMBOL_SEVEN]; break;
           default: bm = &bmSymbol[SYMBOL_ZERO];  break;
           }
        OSDDRAWSTATUSBM(OSDSPACING);
        switch (cDevice::PrimaryDevice()->GetAudioChannel()) {
           case 1:  bm = &bmSymbol[SYMBOL_MONO_LEFT];  break;
           case 2:  bm = &bmSymbol[SYMBOL_MONO_RIGHT]; break;
           default: bm = &bmSymbol[SYMBOL_STEREO];     break;
           }
        OSDDRAWSTATUSBM(0);
        }
     else if (m_Receiver && m_Receiver->AC3Valid() && IS_DOLBY_TRACK(track)) {
        if      (m_Receiver->AC3_5_1()) bm = &bmSymbol[SYMBOL_DD51];
        else if (m_Receiver->AC3_2_0()) bm = &bmSymbol[SYMBOL_DD20];
        else                            bm = &bmSymbol[SYMBOL_DD];
        OSDDRAWSTATUSBM(OSDSPACING);
        }
     if (m_Receiver) {
        switch (m_Receiver->VideoCodec()) {
           case VIDEO_CODEC_MPEG2: bm = &bmSymbol[SYMBOL_MPEG2]; break;
           case VIDEO_CODEC_H264:  bm = &bmSymbol[SYMBOL_H264];  break;
           default:                bm = NULL;                    break;
           }
        OSDDRAWSTATUSBM(OSDSPACING);
        switch (m_Receiver->VideoFormat()) {
           case VIDEO_FORMAT_PAL:  bm = &bmSymbol[SYMBOL_PAL];  break;
           case VIDEO_FORMAT_NTSC: bm = &bmSymbol[SYMBOL_NTSC]; break;
           default:                bm = NULL;                   break;
           }
        OSDDRAWSTATUSBM(OSDSPACING);
        switch (m_Receiver->VideoAspectRatio()) {
           case VIDEO_ASPECT_RATIO_1_1:    bm = &bmSymbol[SYMBOL_AR_1_1];    break;
           case VIDEO_ASPECT_RATIO_4_3:    bm = &bmSymbol[SYMBOL_AR_4_3];    break;
           case VIDEO_ASPECT_RATIO_16_9:   bm = &bmSymbol[SYMBOL_AR_16_9];   break;
           case VIDEO_ASPECT_RATIO_2_21_1: bm = &bmSymbol[SYMBOL_AR_2_21_1]; break;
           default:                        bm = NULL;                        break;
           }
        OSDDRAWSTATUSBM(OSDSPACING);
        }
     if (channel->Ca() > 0xFF) {
        bm = &bmSymbol[SYMBOL_ENCRYPTED];
        OSDDRAWSTATUSBM(OSDSPACING);
        }
     offset += OSDROWHEIGHT;
     OSDDRAWSTATUSBAR(signal);
     offset += OSDROWHEIGHT;
     OSDDRAWSTATUSBAR(snr);
     offset += OSDROWHEIGHT;
     OSDDRAWSTATUSVALUES("STR:", *cString::sprintf("%04x", m_Signal), *cString::sprintf("(%2d%%)", m_Signal / 655), "BER:", *cString::sprintf("%08x", m_BER),
                         *cString::sprintf("%s:", tr("Video")), *getBitrateMbits(m_Receiver ? m_Receiver->VideoBitrate() : (m_SvdrpFrontend >= 0 ? m_SvdrpVideoBitrate : -1.0)));
     offset += OSDROWHEIGHT;
     OSDDRAWSTATUSVALUES("SNR:", *cString::sprintf("%04x", m_SNR), *cString::sprintf("(%2d%%)", m_SNR / 655), "UNC:", *cString::sprintf("%08x", m_UNC),
                         *cString::sprintf("%s:", (m_Receiver && m_Receiver->AC3Valid() && IS_DOLBY_TRACK(track)) ? tr("AC-3") : tr("Audio")),
                         *getBitrateKbits(m_Receiver ? ((m_Receiver->AC3Valid() && IS_DOLBY_TRACK(track)) ? m_Receiver->AC3Bitrate() : m_Receiver->AudioBitrate()) : (m_SvdrpFrontend >= 0 ? m_SvdrpAudioBitrate : -1.0)));
     offset += OSDROWHEIGHT;
     x = bmSymbol[SYMBOL_LOCK].Width();
     y = (OSDROWHEIGHT - bmSymbol[SYMBOL_LOCK].Height()) / 2;
     OSDDRAWSTATUSFRONTEND(1, bmSymbol[SYMBOL_LOCK],    FE_HAS_LOCK);
     OSDDRAWSTATUSFRONTEND(2, bmSymbol[SYMBOL_SIGNAL],  FE_HAS_SIGNAL);
     OSDDRAWSTATUSFRONTEND(3, bmSymbol[SYMBOL_CARRIER], FE_HAS_CARRIER);
     OSDDRAWSTATUSFRONTEND(4, bmSymbol[SYMBOL_VITERBI], FE_HAS_VITERBI);
     OSDDRAWSTATUSFRONTEND(5, bmSymbol[SYMBOL_SYNC],    FE_HAS_SYNC);
     OSDDRAWSTATUSBOTTOMBAR();
     m_Osd->Flush();
     }
}

void cFemonOsd::DrawInfoWindow(void)
{
  cMutexLock lock(&m_Mutex);
  int offset = 0;
  cChannel *channel = Channels.GetByNumber(cDevice::CurrentChannel());
  eTrackType track = cDevice::PrimaryDevice()->GetCurrentAudioTrack();

  if (m_Osd && channel) {
     switch (m_DisplayMode) {
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
              case cSource::stSat:
                   OSDDRAWINFOLINE(*cString::sprintf("DVB-S%s #%d - %s", (m_FrontendInfo.caps & 0x10000000) ? "2" : "", (m_SvdrpFrontend >= 0) ? m_SvdrpFrontend : cDevice::ActualDevice()->CardIndex(), m_FrontendInfo.name));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("Frequency"),    *getFrequencyMHz(channel->Frequency()));
                   OSDDRAWINFORIGHT(trVDR("Source"),       *cSource::ToString(channel->Source()));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("Srate"),        *cString::sprintf("%d", channel->Srate()));
                   OSDDRAWINFORIGHT(trVDR("Polarization"), *cString::sprintf("%c", toupper(channel->Polarization())));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("Inversion"),    *getInversion(channel->Inversion()));
                   OSDDRAWINFORIGHT(trVDR("CoderateH"),    *getCoderate(channel->CoderateH()));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("System"),       *getSystem(channel->System()));
                   OSDDRAWINFORIGHT(trVDR("RollOff"),      *getRollOff(channel->RollOff()));
                   break;

              case cSource::stCable:
                   OSDDRAWINFOLINE(*cString::sprintf("DVB-C #%d - %s", (m_SvdrpFrontend >= 0) ? m_SvdrpFrontend : cDevice::ActualDevice()->CardIndex(), m_FrontendInfo.name));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("Frequency"),    *getFrequencyMHz(channel->Frequency()));
                   OSDDRAWINFORIGHT(trVDR("Source"),       *cSource::ToString(channel->Source()));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("Srate"),        *cString::sprintf("%d", channel->Srate()));
                   OSDDRAWINFORIGHT(trVDR("Modulation"),   *getModulation(channel->Modulation()));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("Inversion"),    *getInversion(channel->Inversion()));
                   OSDDRAWINFORIGHT(trVDR("CoderateH"),    *getCoderate(channel->CoderateH()));
                   break;

              case cSource::stTerr:
                   OSDDRAWINFOLINE(*cString::sprintf("DVB-T #%d - %s", (m_SvdrpFrontend >= 0) ? m_SvdrpFrontend : cDevice::ActualDevice()->CardIndex(), m_FrontendInfo.name));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("Frequency"),    *getFrequencyMHz(channel->Frequency()));
                   OSDDRAWINFORIGHT(trVDR("Transmission"), *getTransmission(channel->Transmission()));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("Bandwidth"),    *getBandwidth(channel->Bandwidth()));
                   OSDDRAWINFORIGHT(trVDR("Modulation"),   *getModulation(channel->Modulation()));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("Inversion"),    *getInversion(channel->Inversion()));
                   OSDDRAWINFORIGHT(tr   ("Coderate"),     *cString::sprintf("%s (H) %s (L)", *getCoderate(channel->CoderateH()), *getCoderate(channel->CoderateL())));
                   offset += OSDROWHEIGHT;
                   OSDDRAWINFOLEFT( trVDR("Hierarchy"),    *getHierarchy(channel->Hierarchy()));
                   OSDDRAWINFORIGHT(trVDR("Guard"),        *getGuard(channel->Guard()));
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
            OSDDRAWINFOINACTIVE(tr("Codec"),              *getVideoCodec(m_Receiver ? m_Receiver->VideoCodec() : VIDEO_CODEC_INVALID));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOINACTIVE(tr("Bitrate"),            *getVideoBitrate(m_Receiver ? m_Receiver->VideoBitrate() : 0, m_Receiver ? m_Receiver->VideoStreamBitrate() : 0));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOINACTIVE(tr("Aspect Ratio"),       *getAspectRatio(m_Receiver ? m_Receiver->VideoAspectRatio() : VIDEO_ASPECT_RATIO_INVALID));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOINACTIVE(tr("Frame Rate"),         *getFrameRate(m_Receiver ? m_Receiver->VideoFrameRate() : 0));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOINACTIVE(tr("Video Format"),       *getVideoFormat(m_Receiver ? m_Receiver->VideoFormat() : VIDEO_CODEC_INVALID));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOINACTIVE(tr("Resolution"),         *getResolution(m_Receiver ? m_Receiver->VideoHorizontalSize() : 0, m_Receiver ? m_Receiver->VideoVerticalSize() : 0, m_Receiver ? m_Receiver->VideoScan() : VIDEO_SCAN_INVALID));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOACTIVE(  tr("Audio Stream"),       *getAudioStream(track, channel));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOINACTIVE(tr("Codec"),              *getAudioCodec(m_Receiver ? m_Receiver->AudioCodec() : AUDIO_CODEC_INVALID));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOINACTIVE(tr("Channel Mode"),       *getAudioChannelMode(m_Receiver ? m_Receiver->AudioChannelMode() : AUDIO_CHANNEL_MODE_INVALID));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOINACTIVE(tr("Bitrate"),            *getAudioBitrate(m_Receiver ? m_Receiver->AudioBitrate() : 0, m_Receiver ? m_Receiver->AudioStreamBitrate() : 0));
            offset += OSDROWHEIGHT;
            OSDDRAWINFOINACTIVE(tr("Sampling Frequency"), *getAudioSamplingFreq(m_Receiver ? m_Receiver->AudioSamplingFreq() : AUDIO_SAMPLING_FREQUENCY_INVALID));
            OSDDRAWINFOBOTTOMBAR();
            break;

       case eFemonModeAC3:
            OSDDRAWINFOTITLEBAR(tr("Stream Information"));
            if (m_Receiver && m_Receiver->AC3Valid() && IS_DOLBY_TRACK(track)) {
               offset += OSDROWHEIGHT;
               OSDDRAWINFOACTIVE(  tr("AC-3 Stream"),            *getAC3Stream(track, channel));
               offset += OSDROWHEIGHT;
               OSDDRAWINFOINACTIVE(tr("Bitrate"),                *getAudioBitrate(m_Receiver->AC3Bitrate(), m_Receiver->AC3StreamBitrate()));
               offset += OSDROWHEIGHT;
               OSDDRAWINFOINACTIVE(tr("Sampling Frequency"),     *getAudioSamplingFreq(m_Receiver->AC3SamplingFreq()));
               offset += OSDROWHEIGHT;
               OSDDRAWINFOINACTIVE(tr("Bit Stream Mode"),        *getAC3BitStreamMode(m_Receiver->AC3BitStreamMode(), m_Receiver->AC3AudioCodingMode()));
               offset += OSDROWHEIGHT;
               OSDDRAWINFOINACTIVE(tr("Audio Coding Mode"),      *getAC3AudioCodingMode(m_Receiver->AC3AudioCodingMode(), m_Receiver->AC3BitStreamMode()));
               offset += OSDROWHEIGHT;
               OSDDRAWINFOINACTIVE(tr("Center Mix Level"),       *getAC3CenterMixLevel(m_Receiver->AC3CenterMixLevel()));
               offset += OSDROWHEIGHT;
               OSDDRAWINFOINACTIVE(tr("Surround Mix Level"),     *getAC3SurroundMixLevel(m_Receiver->AC3SurroundMixLevel()));
               offset += OSDROWHEIGHT;
               OSDDRAWINFOINACTIVE(tr("Dolby Surround Mode"),    *getAC3DolbySurroundMode(m_Receiver->AC3DolbySurroundMode()));
               offset += OSDROWHEIGHT;
               OSDDRAWINFOINACTIVE(tr("Low Frequency Effects"),  *cString::sprintf("%s", m_Receiver->AC3Lfe() ? tr("on") : tr("off")));
               offset += OSDROWHEIGHT;
               OSDDRAWINFOINACTIVE(tr("Dialogue Normalization"), *getAC3DialogLevel(m_Receiver->AC3DialogLevel()));
               }
            OSDDRAWINFOBOTTOMBAR();
            break;

       default:
            OSDCLEARINFO();
            break;
       }
     m_Osd->Flush();
     }
}

void cFemonOsd::Action(void)
{
  Dprintf("%s()\n", __PRETTY_FUNCTION__);
  cTimeMs t;
  SvdrpCommand_v1_0 cmd;
  cmd.command = cString::sprintf("PLUG %s INFO\r\n", PLUGIN_NAME_I18N);
  while (Running()) {
    t.Set(0);
    m_SvdrpFrontend = -1;
    m_SvdrpVideoBitrate = -1.0;
    m_SvdrpAudioBitrate = -1.0;
    if (m_Frontend != -1) {
       CHECK(ioctl(m_Frontend, FE_READ_STATUS, &m_FrontendStatus));
       CHECK(ioctl(m_Frontend, FE_READ_SIGNAL_STRENGTH, &m_Signal));
       CHECK(ioctl(m_Frontend, FE_READ_SNR, &m_SNR));
       CHECK(ioctl(m_Frontend, FE_READ_BER, &m_BER));
       CHECK(ioctl(m_Frontend, FE_READ_UNCORRECTED_BLOCKS, &m_UNC));
       DrawInfoWindow();
       DrawStatusWindow();
       }
    else if (m_SvdrpConnection.handle >= 0) {
       cmd.handle = m_SvdrpConnection.handle; 
       m_SvdrpPlugin->Service("SvdrpCommand-v1.0", &cmd);
       if (cmd.responseCode == 900) {
          for (cLine *line = cmd.reply.First(); line; line = cmd.reply.Next(line)) {
              const char *s = line->Text();
	      if (!strncasecmp(s, "CARD:", 5))
                 m_SvdrpFrontend = strtol(s + 5, NULL, 10);
              else if (!strncasecmp(s, "TYPE:", 5))
                 m_FrontendInfo.type = (fe_type_t) strtol(s + 5, NULL, 10);
              else if (!strncasecmp(s, "NAME:", 5))
                 strn0cpy(m_FrontendInfo.name, s + 5, sizeof(m_FrontendInfo.name));
              else if (!strncasecmp(s, "STAT:", 5))
                 m_FrontendStatus = (fe_status_t) strtol(s + 5, NULL, 16);
              else if (!strncasecmp(s, "SGNL:", 5))
                 m_Signal = strtol(s + 5, NULL, 16);
              else if (!strncasecmp(s, "SNRA:", 5))
                 m_SNR = strtol(s + 5, NULL, 16);
              else if (!strncasecmp(s, "BERA:", 5))
                 m_BER = strtol(s + 5, NULL, 16);
              else if (!strncasecmp(s, "UNCB:", 5))
                 m_UNC = strtol(s + 5, NULL, 16);
              else if (!strncasecmp(s, "VIBR:", 5))
                 m_SvdrpVideoBitrate = strtol(s + 5, NULL, 10);
              else if (!strncasecmp(s, "AUBR:", 5))
                 m_SvdrpAudioBitrate = strtol(s + 5, NULL, 10);
              }
          }
       DrawInfoWindow();
       DrawStatusWindow();
       }
    m_Sleep.Wait(max((int)(100 * femonConfig.updateinterval - t.Elapsed()), 3));
    }
}

void cFemonOsd::Show(void)
{
  Dprintf("%s()\n", __PRETTY_FUNCTION__);
  int apid[2] = {0, 0};
  int dpid[2] = {0, 0};
  eTrackType track = cDevice::PrimaryDevice()->GetCurrentAudioTrack();
  cString dev = cString::sprintf(FRONTEND_DEVICE, cDevice::ActualDevice()->CardIndex(), 0);
  m_Frontend = open(dev, O_RDONLY | O_NONBLOCK);
  if (m_Frontend >= 0) {
     if (ioctl(m_Frontend, FE_GET_INFO, &m_FrontendInfo) < 0) {
        esyslog("ERROR: cFemonOsd::Show() cannot read frontend info.");
        close(m_Frontend);
        m_Frontend = -1;
        return;
        }
     }
  else if (femonConfig.usesvdrp) {
     if (!SvdrpConnect() || !SvdrpTune())
        return;
     }
  else {
     esyslog("ERROR: cFemonOsd::Show() cannot open frontend device.");
     return;
     }

  m_Osd = cOsdProvider::NewOsd(((cOsd::OsdWidth() - OSDWIDTH) / 2) + cOsd::OsdLeft() + femonConfig.osdoffset, ((cOsd::OsdHeight() - OSDHEIGHT) / 2) + cOsd::OsdTop());
  if (m_Osd) {
     tArea Areas1[] = { { 0, 0, OSDWIDTH - 1, OSDHEIGHT - 1, 8 } };
     if (Setup.AntiAlias && m_Osd->CanHandleAreas(Areas1, sizeof(Areas1) / sizeof(tArea)) == oeOk) {
        m_Osd->SetAreas(Areas1, sizeof(Areas1) / sizeof(tArea));
        }
     else {
        tArea Areas2[] = { { 0, OSDSTATUSWIN_Y(0),          OSDWIDTH - 1, OSDSTATUSWIN_Y(0) + OSDSTATUSHEIGHT - 1, femonTheme[femonConfig.theme].bpp },
                           { 0, OSDINFOWIN_Y(0),            OSDWIDTH - 1, OSDINFOWIN_Y(0)   + OSDROWHEIGHT    - 1, femonTheme[femonConfig.theme].bpp },
                           { 0, OSDINFOWIN_Y(OSDROWHEIGHT), OSDWIDTH - 1, OSDINFOWIN_Y(0)   + OSDINFOHEIGHT   - 1, 2                                 } };
        m_Osd->SetAreas(Areas2, sizeof(Areas2) / sizeof(tArea));
        }
     OSDCLEARSTATUS();
     OSDCLEARINFO();
     m_Osd->Flush();
     if (m_Receiver) {
        m_Receiver->Deactivate();
        DELETENULL(m_Receiver);
        }
     if (femonConfig.analyzestream) {
        cChannel *channel = Channels.GetByNumber(cDevice::CurrentChannel());
        if (channel) {
           IS_AUDIO_TRACK(track) ? apid[0] = channel->Apid(int(track - ttAudioFirst)) : apid[0] = channel->Apid(0);
           IS_DOLBY_TRACK(track) ? dpid[0] = channel->Dpid(int(track - ttDolbyFirst)) : dpid[0] = channel->Dpid(0);
           m_Receiver = new cFemonReceiver(channel->GetChannelID(), channel->Ca(), channel->Vpid(), apid, dpid);
           cDevice::ActualDevice()->AttachReceiver(m_Receiver);
           }
        }
     Start();
     }
}

void cFemonOsd::ChannelSwitch(const cDevice * device, int channelNumber)
{
  Dprintf("%s(%d,%d)\n", __PRETTY_FUNCTION__, device->DeviceNumber(), channelNumber);
  int apid[2] = {0, 0};
  int dpid[2] = {0, 0};
  eTrackType track = cDevice::PrimaryDevice()->GetCurrentAudioTrack();
  if (!device->IsPrimaryDevice() || !channelNumber || cDevice::PrimaryDevice()->CurrentChannel() != channelNumber)
     return;
  close(m_Frontend);
  cString dev = cString::sprintf(FRONTEND_DEVICE, cDevice::ActualDevice()->CardIndex(), 0);
  m_Frontend = open(dev, O_RDONLY | O_NONBLOCK);
  if (m_Frontend >= 0) {
     if (ioctl(m_Frontend, FE_GET_INFO, &m_FrontendInfo) < 0) {
        esyslog("ERROR: cFemonOsd::ChannelSwitch() cannot read frontend info.");
        close(m_Frontend);
        m_Frontend = -1;
        return;
        }
     }
  else if (femonConfig.usesvdrp) {
     if (!SvdrpConnect() || !SvdrpTune())
        return;
     }
  else {
     esyslog("ERROR: cFemonOsd::ChannelSwitch() cannot open frontend device.");
     return;
     }

  if (m_Receiver) {
     m_Receiver->Deactivate();
     DELETENULL(m_Receiver);
     }
  if (femonConfig.analyzestream) {
     cChannel *channel = Channels.GetByNumber(cDevice::CurrentChannel());
     if (channel) {
        IS_AUDIO_TRACK(track) ? apid[0] = channel->Apid(int(track - ttAudioFirst)) : apid[0] = channel->Apid(0);
        IS_DOLBY_TRACK(track) ? dpid[0] = channel->Dpid(int(track - ttDolbyFirst)) : dpid[0] = channel->Dpid(0);
        m_Receiver = new cFemonReceiver(channel->GetChannelID(), channel->Ca(), channel->Vpid(), apid, dpid);
        cDevice::ActualDevice()->AttachReceiver(m_Receiver);
        }
     }
}

void cFemonOsd::SetAudioTrack(int Index, const char * const *Tracks)
{
  Dprintf("%s()\n", __PRETTY_FUNCTION__);
  int apid[2] = {0, 0};
  int dpid[2] = {0, 0};
  eTrackType track = cDevice::PrimaryDevice()->GetCurrentAudioTrack();
  if (m_Receiver) {
     m_Receiver->Deactivate();
     DELETENULL(m_Receiver);
     }
  if (femonConfig.analyzestream) {
     cChannel *channel = Channels.GetByNumber(cDevice::CurrentChannel());
     if (channel) {
        IS_AUDIO_TRACK(track) ? apid[0] = channel->Apid(int(track - ttAudioFirst)) : apid[0] = channel->Apid(0);
        IS_DOLBY_TRACK(track) ? dpid[0] = channel->Dpid(int(track - ttDolbyFirst)) : dpid[0] = channel->Dpid(0);
        m_Receiver = new cFemonReceiver(channel->GetChannelID(), channel->Ca(), channel->Vpid(), apid, dpid);
        cDevice::ActualDevice()->AttachReceiver(m_Receiver);
        }
     }
}

bool cFemonOsd::DeviceSwitch(int direction)
{
  Dprintf("%s()\n", __PRETTY_FUNCTION__);
  int device = cDevice::ActualDevice()->DeviceNumber();
  direction = sgn(direction);
  if (device >= 0) {
     cChannel *channel = Channels.GetByNumber(cDevice::CurrentChannel());
     if (channel) {
        for (int i = 0; i < cDevice::NumDevices() - 1; i++) {
           if (direction) {
              if (++device >= cDevice::NumDevices())
                 device = 0;
              }
           else {
              if (--device < 0)
                 device = cDevice::NumDevices() - 1;
              }
           if (cDevice::GetDevice(device)->ProvidesChannel(channel, 0)) {
              Dprintf("%s(%d) device(%d)\n", __PRETTY_FUNCTION__, direction, device);
              cStatus::MsgChannelSwitch(cDevice::PrimaryDevice(), 0);
              cControl::Shutdown();
              cDevice::GetDevice(device)->SwitchChannel(channel, true);
              if (cDevice::GetDevice(device) == cDevice::PrimaryDevice())
                 cDevice::GetDevice(device)->ForceTransferMode();
              cControl::Launch(new cTransferControl(cDevice::GetDevice(device), channel->GetChannelID(), channel->Vpid(), channel->Apids(), channel->Dpids(), channel->Spids()));
              cStatus::MsgChannelSwitch(cDevice::PrimaryDevice(), channel->Number());
              return (true);
              }
           }
        }
     }
   return (false);
}

bool cFemonOsd::SvdrpConnect(void)
{
   if (m_SvdrpConnection.handle < 0) {
      m_SvdrpPlugin = cPluginManager::GetPlugin(SVDRPPLUGIN);
      if (m_SvdrpPlugin) {
         m_SvdrpConnection.serverIp = femonConfig.svdrpip;
         m_SvdrpConnection.serverPort = femonConfig.svdrpport;
         m_SvdrpConnection.shared = true;
         m_SvdrpPlugin->Service("SvdrpConnection-v1.0", &m_SvdrpConnection);
         if (m_SvdrpConnection.handle >= 0) {
            SvdrpCommand_v1_0 cmd;
            cmd.handle = m_SvdrpConnection.handle; 
            cmd.command = cString::sprintf("PLUG %s\r\n", PLUGIN_NAME_I18N);
            m_SvdrpPlugin->Service("SvdrpCommand-v1.0", &cmd);
            if (cmd.responseCode != 214) {
               m_SvdrpPlugin->Service("SvdrpConnection-v1.0", &m_SvdrpConnection); // close connection
               esyslog("ERROR: cFemonOsd::SvdrpConnect() cannot find plugin '%s' on server %s.", PLUGIN_NAME_I18N, *m_SvdrpConnection.serverIp);
               }
            }
         else
            esyslog("ERROR: cFemonOsd::SvdrpConnect() cannot connect to SVDRP server.");
         }
      else
         esyslog("ERROR: cFemonOsd::SvdrpConnect() cannot find plugin '%s'.", SVDRPPLUGIN);
      }
   return m_SvdrpConnection.handle >= 0;
}

bool cFemonOsd::SvdrpTune(void)
{
   if (m_SvdrpPlugin && m_SvdrpConnection.handle >= 0) {
      cChannel *channel = Channels.GetByNumber(cDevice::CurrentChannel());
      if (channel) {
         SvdrpCommand_v1_0 cmd;
         cmd.handle = m_SvdrpConnection.handle; 
         cmd.command = cString::sprintf("CHAN %s\r\n", *channel->GetChannelID().ToString());
         m_SvdrpPlugin->Service("SvdrpCommand-v1.0", &cmd);
         if (cmd.responseCode == 250)
            return true;
         esyslog("ERROR: cFemonOsd::SvdrpTune() cannot tune server channel.");
         }
      else
         esyslog("ERROR: cFemonOsd::SvdrpTune() invalid channel.");
      }
   else
      esyslog("ERROR: cFemonOsd::SvdrpTune() unexpected connection state.");
   return false;
}

double cFemonOsd::GetVideoBitrate(void)
{
  Dprintf("%s()\n", __PRETTY_FUNCTION__);
  double value = 0.0;

  if (m_Receiver)
     value = m_Receiver->VideoBitrate();

  return (value);
}

double cFemonOsd::GetAudioBitrate(void)
{
  Dprintf("%s()\n", __PRETTY_FUNCTION__);
  double value = 0.0;

  if (m_Receiver)
     value = m_Receiver->AudioBitrate();

  return (value);
}

double cFemonOsd::GetDolbyBitrate(void)
{
  Dprintf("%s()\n", __PRETTY_FUNCTION__);
  double value = 0.0;

  if (m_Receiver)
     value = m_Receiver->AC3Bitrate();

  return (value);
}

eOSState cFemonOsd::ProcessKey(eKeys Key)
{ 
  eOSState state = cOsdObject::ProcessKey(Key);
  if (state == osUnknown) {
     switch (Key) {
       case k0:
            if ((m_Number == 0) && (m_OldNumber != 0)) {
               m_Number = m_OldNumber;
               m_OldNumber = cDevice::CurrentChannel();
               Channels.SwitchTo(m_Number);
               m_Number = 0;
               return osContinue;
               }
       case k1 ... k9:
            if (m_Number >= 0) {
               m_Number = m_Number * 10 + Key - k0;
               if (m_Number > 0) {
                  DrawStatusWindow();
                  cChannel *ch = Channels.GetByNumber(m_Number);
                  m_InputTime.Set(0);
                  // Lets see if there can be any useful further input:
                  int n = ch ? m_Number * 10 : 0;
                  while (ch && (ch = Channels.Next(ch)) != NULL) {
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
                     m_OldNumber = cDevice::CurrentChannel();
                     Channels.SwitchTo(m_Number);
                     m_Number = 0;
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
            m_OldNumber = cDevice::CurrentChannel();
            cDevice::SwitchChannel(NORMALKEY(Key) == kUp ? 1 : -1);
            m_Number = 0;
            break;
       case kNone:
            if (m_Number && (m_InputTime.Elapsed() > CHANNELINPUT_TIMEOUT)) {
               if (Channels.GetByNumber(m_Number)) {
                  m_OldNumber = cDevice::CurrentChannel();
                  Channels.SwitchTo(m_Number);
                  m_Number = 0;
                  }
               else {
                  m_InputTime.Set(0);
                  m_Number = 0;
                  }
               }
            break;
       case kOk:
            {
            // toggle between display modes
            cChannel *channel = Channels.GetByNumber(cDevice::CurrentChannel());
            if (++m_DisplayMode == eFemonModeAC3 && channel && !channel->Dpid(0)) m_DisplayMode++;
            if (m_DisplayMode >= eFemonModeMaxNumber) m_DisplayMode = 0;
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
