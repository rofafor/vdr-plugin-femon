/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#include <ctype.h>
#include "femoncfg.h"
#include "femoni18n.h"
#include "femonreceiver.h"
#include "femonosd.h"

#include "symbols/device.xpm"
#include "symbols/stereo.xpm"
#include "symbols/monoleft.xpm"
#include "symbols/monoright.xpm"
#include "symbols/zero.xpm"
#include "symbols/one.xpm"
#include "symbols/two.xpm"
#include "symbols/three.xpm"
#include "symbols/four.xpm"
#include "symbols/five.xpm"
#include "symbols/ar11.xpm"
#include "symbols/ar169.xpm"
#include "symbols/ar2211.xpm"
#include "symbols/ar43.xpm"
#include "symbols/ntsc.xpm"
#include "symbols/pal.xpm"
#include "symbols/dolbydigital.xpm"
#include "symbols/dolbydigital20.xpm"
#include "symbols/dolbydigital51.xpm"

#define FRONTEND_DEVICE          "/dev/dvb/adapter%d/frontend%d"
#define CHANNELINPUT_TIMEOUT     1000

#define OSDHEIGHT                femonConfig.osdheight   // in pixels
#define OSDWIDTH                 600                     // in pixels
#define OSDINFOHEIGHT            (m_Font->Height() * 11) // in pixels (11 rows)
#define OSDSTATUSHEIGHT          (m_Font->Height() * 6)  // in pixels (6 rows)

#define OSDINFOWIN_Y(offset)     (femonConfig.position ? (OSDHEIGHT - OSDINFOHEIGHT + offset) : offset)
#define OSDINFOWIN_X(col)        ((col == 4) ? 455 : (col == 3) ? 305 : (col == 2) ? 155 : 15)
#define OSDSTATUSWIN_Y(offset)   (femonConfig.position ? offset : (OSDHEIGHT - OSDSTATUSHEIGHT + offset))
#define OSDSTATUSWIN_X(col)      ((col == 7) ? 475 : (col == 6) ? 410 : (col == 5) ? 275 : (col == 4) ? 220 : (col == 3) ? 125 : (col == 2) ? 70 : 15)
#define OSDSTATUSWIN_XC(col,txt) (((col - 1) * OSDWIDTH / 5) + ((OSDWIDTH / 5 - m_Font->Width(txt)) / 2))
#define BARWIDTH(x)              (OSDWIDTH * x / 100)
#define SPACING                  5

#define clrBackground            clrGray50 // this should be tied somehow into current theme

cBitmap cFemonOsd::bmDevice(device_xpm);
cBitmap cFemonOsd::bmStereo(stereo_xpm);
cBitmap cFemonOsd::bmMonoLeft(monoleft_xpm);
cBitmap cFemonOsd::bmMonoRight(monoright_xpm);
cBitmap cFemonOsd::bmZero(zero_xpm);
cBitmap cFemonOsd::bmOne(one_xpm);
cBitmap cFemonOsd::bmTwo(two_xpm);
cBitmap cFemonOsd::bmThree(three_xpm);
cBitmap cFemonOsd::bmFour(four_xpm);
cBitmap cFemonOsd::bmFive(five_xpm);
cBitmap cFemonOsd::bmAspectRatio_1_1(ar11_xpm);
cBitmap cFemonOsd::bmAspectRatio_16_9(ar169_xpm);
cBitmap cFemonOsd::bmAspectRatio_2_21_1(ar2211_xpm);
cBitmap cFemonOsd::bmAspectRatio_4_3(ar43_xpm);
cBitmap cFemonOsd::bmPAL(pal_xpm);
cBitmap cFemonOsd::bmNTSC(ntsc_xpm);
cBitmap cFemonOsd::bmDD(dolbydigital_xpm);
cBitmap cFemonOsd::bmDD20(dolbydigital20_xpm);
cBitmap cFemonOsd::bmDD51(dolbydigital51_xpm);

cFemonOsd::cFemonOsd(void)
:cOsdObject(true), cThread("femon osd")
{
  debug(printf("cFemonOsd::cFemonOsd()\n"));
  m_Osd = NULL;
  m_Receiver = NULL;
  m_Frontend = -1;
  m_Active = false;
  m_Number = 0;
  m_OldNumber = 0;
  m_Signal = 0;
  m_SNR = 0;
  m_BER = 0;
  m_UNC = 0;
  m_DisplayMode = femonConfig.displaymode;
  m_InputTime.Set(0);
  m_Mutex = new cMutex();
  if (Setup.UseSmallFont == 0) {
     // Dirty hack to force the small fonts...
     Setup.UseSmallFont = 1;
     m_Font = cFont::GetFont(fontSml);
     Setup.UseSmallFont = 0;
     }
  else
     m_Font = cFont::GetFont(fontSml);
}

cFemonOsd::~cFemonOsd(void)
{
  debug(printf("cFemonOsd::~cFemonOsd()\n"));
  if (m_Active) {
     m_Active = false;
     Cancel(3);
     }
  if (m_Receiver)
     delete m_Receiver;
  if (m_Osd)
     delete m_Osd;
}

void cFemonOsd::DrawStatusWindow(void)
{
  cMutexLock lock(m_Mutex);
  debug(printf("cFemonOsd::DrawStatusWindow()\n"));
  char buf[128];
  int snr = m_SNR / 655;
  int signal = m_Signal / 655;
  int offset = 0;
  int x = OSDWIDTH;
  int y = 0;
  int value = 0;
  eTrackType track = cDevice::PrimaryDevice()->GetCurrentAudioTrack();
  cChannel *channel = Channels.GetByNumber(cDevice::CurrentChannel());

  if (m_Osd) {
     m_Osd->DrawRectangle(0, OSDSTATUSWIN_Y(0), OSDWIDTH, OSDSTATUSWIN_Y(OSDSTATUSHEIGHT), clrBackground);
     snprintf(buf, sizeof(buf), "%d%s %s (%s)", m_Number ? m_Number : channel->Number(), m_Number ? "-" : "", channel->ShortName(true), channel->Provider());
     m_Osd->DrawRectangle(0, OSDSTATUSWIN_Y(offset), OSDWIDTH, OSDSTATUSWIN_Y(offset+m_Font->Height()-1), clrWhite);
     m_Osd->DrawText(OSDSTATUSWIN_X(1), OSDSTATUSWIN_Y(offset), buf, clrBlack, clrWhite, m_Font);
     if (m_Receiver) {
        value = cDevice::ActualDevice()->CardIndex();
        if (value == 1) {
           x -= bmOne.Width() + SPACING;
           y = (m_Font->Height() - bmOne.Height()) / 2;
           if (y < 0) y = 0;
           m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmOne, clrBlack, clrWhite);
           }
        else if (value == 2) {
           x -= bmTwo.Width() + SPACING;
           y = (m_Font->Height() - bmTwo.Height()) / 2;
           if (y < 0) y = 0;
           m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmTwo, clrBlack, clrWhite);
           }
        else if (value == 3) {
           x -= bmThree.Width() + SPACING;
           y = (m_Font->Height() - bmThree.Height()) / 2;
           if (y < 0) y = 0;
           m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmThree, clrBlack, clrWhite);
           }
        else {
           x -= bmZero.Width() + SPACING;
           y = (m_Font->Height() - bmZero.Height()) / 2;
           if (y < 0) y = 0;
           m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmZero, clrBlack, clrWhite);
           }
        x -= bmDevice.Width();
        y = (m_Font->Height() - bmDevice.Height()) / 2;
        if (y < 0) y = 0;
        m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmDevice, clrBlack, clrWhite);
        if (IS_AUDIO_TRACK(track)) {
           value = int(track - ttAudioFirst);
           if (value == 1) {
              x -= bmOne.Width() + SPACING;
              y = (m_Font->Height() - bmOne.Height()) / 2;
              if (y < 0) y = 0;
              m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmOne, clrBlack, clrWhite);
              }
           else if (value == 2) {
              x -= bmTwo.Width() + SPACING;
              y = (m_Font->Height() - bmTwo.Height()) / 2;
              if (y < 0) y = 0;
              m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmTwo, clrBlack, clrWhite);
              }
           else if (value == 3) {
              x -= bmThree.Width() + SPACING;
              y = (m_Font->Height() - bmThree.Height()) / 2;
              if (y < 0) y = 0;
              m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmThree, clrBlack, clrWhite);
              }
           else if (value == 4) {
              x -= bmFour.Width() + SPACING;
              y = (m_Font->Height() - bmFour.Height()) / 2;
              if (y < 0) y = 0;
              m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmFour, clrBlack, clrWhite);
              }
           else if (value == 5) {
              x -= bmFive.Width() + SPACING;
              y = (m_Font->Height() - bmFive.Height()) / 2;
              if (y < 0) y = 0;
              m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmFive, clrBlack, clrWhite);
              }
           else {
              x -= bmZero.Width() + SPACING;
              y = (m_Font->Height() - bmZero.Height()) / 2;
              if (y < 0) y = 0;
              m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmZero, clrBlack, clrWhite);
              }
           value = cDevice::PrimaryDevice()->GetAudioChannel();
           if (value == 1) {
              x -= bmMonoLeft.Width();
              y = (m_Font->Height() - bmMonoLeft.Height()) / 2;
              if (y < 0) y = 0;
              m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmMonoLeft, clrBlack, clrWhite);
              }
           else if (value == 2) {
              x -= bmMonoRight.Width();
              y = (m_Font->Height() - bmMonoRight.Height()) / 2;
              if (y < 0) y = 0;
              m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmMonoRight, clrBlack, clrWhite);
              }
           else {
              x -= bmStereo.Width();
              y = (m_Font->Height() - bmStereo.Height()) / 2;
              if (y < 0) y = 0;
              m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmStereo, clrBlack, clrWhite);
              }
           }
        else if (m_Receiver->AC3Valid() && IS_DOLBY_TRACK(track)) {
           if (m_Receiver->AC3_5_1()) {
              x -= bmDD51.Width() + SPACING;
              y = (m_Font->Height() - bmDD51.Height()) / 2;
              if (y < 0) y = 0;
              m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmDD51, clrBlack, clrWhite);
              }
           else if (m_Receiver->AC3_2_0()) {
              x -= bmDD20.Width() + SPACING;
              y = (m_Font->Height() - bmDD20.Height()) / 2;
              if (y < 0) y = 0;
              m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmDD20, clrBlack, clrWhite);
              }
           else {
              x -= bmDD.Width() + SPACING;
              y = (m_Font->Height() - bmDD.Height()) / 2;
              if (y < 0) y = 0;
              m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmDD, clrBlack, clrWhite);
              }
           }
        value = m_Receiver->VideoFormat();
        if (value == VF_PAL) {
           x -= bmPAL.Width() + SPACING;
           y = (m_Font->Height() - bmPAL.Height()) / 2;
           if (y < 0) y = 0;
           m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmPAL, clrBlack, clrWhite);
           }
        else if (value == VF_NTSC) {
           x -= bmNTSC.Width() + SPACING;
           y = (m_Font->Height() - bmNTSC.Height()) / 2;
           if (y < 0) y = 0;
           m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmNTSC, clrBlack, clrWhite);
           }
        value = m_Receiver->VideoAspectRatio();
        if (value == AR_1_1) {
           x -= bmAspectRatio_1_1.Width() + SPACING;
           y = (m_Font->Height() - bmAspectRatio_1_1.Height()) / 2;
           if (y < 0) y = 0;
           m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmAspectRatio_1_1, clrBlack, clrWhite);
           }
        else if (value == AR_4_3) {
           x -= bmAspectRatio_4_3.Width() + SPACING;
           y = (m_Font->Height() - bmAspectRatio_4_3.Height()) / 2;
           if (y < 0) y = 0;
           m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmAspectRatio_4_3, clrBlack, clrWhite);
           }
        else if (value == AR_16_9) {
           x -= bmAspectRatio_16_9.Width() + SPACING;
           y = (m_Font->Height() - bmAspectRatio_16_9.Height()) / 2;
           if (y < 0) y = 0;
           m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmAspectRatio_16_9, clrBlack, clrWhite);
           }
        else if (value == AR_2_21_1) {
           x -= bmAspectRatio_2_21_1.Width() + SPACING;
           y = (m_Font->Height() - bmAspectRatio_2_21_1.Height()) / 2;
           if (y < 0) y = 0;
           m_Osd->DrawBitmap(x, OSDSTATUSWIN_Y(offset+y), bmAspectRatio_2_21_1, clrBlack, clrWhite);
           }
        }
     offset += m_Font->Height();
     if (signal > 0) {
        signal = BARWIDTH(signal);
        m_Osd->DrawRectangle(0, OSDSTATUSWIN_Y(offset+3), min(BARWIDTH(femonConfig.redlimit), signal), OSDSTATUSWIN_Y(offset+m_Font->Height()-3), clrRed);
        if (signal > BARWIDTH(femonConfig.redlimit)) {
           m_Osd->DrawRectangle(BARWIDTH(femonConfig.redlimit), OSDSTATUSWIN_Y(offset+3), min((OSDWIDTH * femonConfig.greenlimit / 100), signal), OSDSTATUSWIN_Y(offset+m_Font->Height()-3), clrYellow);
           }
        if (signal > BARWIDTH(femonConfig.greenlimit)) {
           m_Osd->DrawRectangle(BARWIDTH(femonConfig.greenlimit), OSDSTATUSWIN_Y(offset+3), signal, OSDSTATUSWIN_Y(offset+m_Font->Height()-3), clrGreen);
           }
        }
     offset += m_Font->Height();
     if (snr > 0) {
        snr = BARWIDTH(snr);
        m_Osd->DrawRectangle(0, OSDSTATUSWIN_Y(offset+3), min(BARWIDTH(femonConfig.redlimit), snr), OSDSTATUSWIN_Y(offset+m_Font->Height()-3), clrRed);
        if (snr > BARWIDTH(femonConfig.redlimit)) {
           m_Osd->DrawRectangle(BARWIDTH(femonConfig.redlimit), OSDSTATUSWIN_Y(offset+3), min(BARWIDTH(femonConfig.greenlimit), snr), OSDSTATUSWIN_Y(offset+m_Font->Height()-3), clrYellow);
           }
        if (snr > BARWIDTH(femonConfig.greenlimit)) {
           m_Osd->DrawRectangle(BARWIDTH(femonConfig.greenlimit),  OSDSTATUSWIN_Y(offset+3), snr, OSDSTATUSWIN_Y(offset+m_Font->Height()-3), clrGreen);
           }
        }
     offset += m_Font->Height();
     m_Osd->DrawText(OSDSTATUSWIN_X(1), OSDSTATUSWIN_Y(offset), "STR:", clrWhite, clrBackground, m_Font);
     snprintf(buf, sizeof(buf), "%04x", m_Signal);
     m_Osd->DrawText(OSDSTATUSWIN_X(2), OSDSTATUSWIN_Y(offset), buf, clrWhite, clrBackground, m_Font);
     snprintf(buf, sizeof(buf), "(%2d%%)", m_Signal / 655);
     m_Osd->DrawText(OSDSTATUSWIN_X(3), OSDSTATUSWIN_Y(offset), buf, clrWhite, clrBackground, m_Font);
     m_Osd->DrawText(OSDSTATUSWIN_X(4), OSDSTATUSWIN_Y(offset), "BER:", clrWhite, clrBackground, m_Font);
     snprintf(buf, sizeof(buf), "%08x", m_BER);
     m_Osd->DrawText(OSDSTATUSWIN_X(5), OSDSTATUSWIN_Y(offset), buf, clrWhite, clrBackground, m_Font);
     snprintf(buf, sizeof(buf), "%s:", tr("Video"));
     m_Osd->DrawText(OSDSTATUSWIN_X(6), OSDSTATUSWIN_Y(offset), buf, clrWhite, clrBackground, m_Font);
     if (m_Receiver) snprintf(buf, sizeof(buf), "%.2f %s", m_Receiver->VideoBitrate(), tr("Mbit/s"));
     else            snprintf(buf, sizeof(buf), "--- %s", tr("Mbit/s"));
     m_Osd->DrawText(OSDSTATUSWIN_X(7), OSDSTATUSWIN_Y(offset), buf, clrWhite, clrBackground, m_Font);
     offset += m_Font->Height();
     m_Osd->DrawText(OSDSTATUSWIN_X(1), OSDSTATUSWIN_Y(offset), "SNR:", clrWhite, clrBackground, m_Font);
     snprintf(buf, sizeof(buf), "%04x", m_SNR);
     m_Osd->DrawText(OSDSTATUSWIN_X(2), OSDSTATUSWIN_Y(offset), buf, clrWhite, clrBackground, m_Font);
     snprintf(buf, sizeof(buf), "(%2d%%)", m_SNR / 655);
     m_Osd->DrawText(OSDSTATUSWIN_X(3), OSDSTATUSWIN_Y(offset), buf, clrWhite, clrBackground, m_Font);
     m_Osd->DrawText(OSDSTATUSWIN_X(4), OSDSTATUSWIN_Y(offset), "UNC:", clrWhite, clrBackground, m_Font);
     snprintf(buf, sizeof(buf), "%08x", m_UNC);
     m_Osd->DrawText(OSDSTATUSWIN_X(5), OSDSTATUSWIN_Y(offset), buf, clrWhite, clrBackground, m_Font);
     snprintf(buf, sizeof(buf), "%s:", (m_Receiver && m_Receiver->AC3Valid() && IS_DOLBY_TRACK(track)) ? tr("AC-3") : tr("Audio"));
     m_Osd->DrawText(OSDSTATUSWIN_X(6), OSDSTATUSWIN_Y(offset), buf, clrWhite, clrBackground, m_Font);
     if (m_Receiver) snprintf(buf, sizeof(buf), "%.0f %s", (m_Receiver->AC3Valid() && IS_DOLBY_TRACK(track)) ? m_Receiver->AC3Bitrate() : m_Receiver->AudioBitrate(), tr("kbit/s"));
     else            snprintf(buf, sizeof(buf), "--- %s", tr("kbit/s"));
     m_Osd->DrawText(OSDSTATUSWIN_X(7), OSDSTATUSWIN_Y(offset), buf, clrWhite, clrBackground, m_Font);
     offset += m_Font->Height();
     m_Osd->DrawText(OSDSTATUSWIN_XC(1,tr("LOCK")),    OSDSTATUSWIN_Y(offset), tr("LOCK"),   (m_FrontendStatus & FE_HAS_LOCK)   ? clrYellow : clrBlack, clrBackground, m_Font);
     m_Osd->DrawText(OSDSTATUSWIN_XC(2,tr("SIGNAL")),  OSDSTATUSWIN_Y(offset), tr("SIGNAL"), (m_FrontendStatus & FE_HAS_SIGNAL) ? clrYellow : clrBlack, clrBackground, m_Font);
     m_Osd->DrawText(OSDSTATUSWIN_XC(3,tr("CARRIER")), OSDSTATUSWIN_Y(offset), tr("CARRIER"),(m_FrontendStatus & FE_HAS_CARRIER)? clrYellow : clrBlack, clrBackground, m_Font);
     m_Osd->DrawText(OSDSTATUSWIN_XC(4,tr("VITERBI")), OSDSTATUSWIN_Y(offset), tr("VITERBI"),(m_FrontendStatus & FE_HAS_VITERBI)? clrYellow : clrBlack, clrBackground, m_Font);
     m_Osd->DrawText(OSDSTATUSWIN_XC(5,tr("SYNC")),    OSDSTATUSWIN_Y(offset), tr("SYNC"),   (m_FrontendStatus & FE_HAS_SYNC)   ? clrYellow : clrBlack, clrBackground, m_Font);
     m_Osd->Flush();
     }
}

void cFemonOsd::DrawInfoWindow(void)
{
  cMutexLock lock(m_Mutex);
  debug(printf("cFemonOsd::DrawInfoWindow()\n"));
  char buf[128];
  char buf2[20];
  int offset = 0;
  int value = 0;
  double dvalue = 0.0;
  cChannel *channel = Channels.GetByNumber(cDevice::CurrentChannel());
  eTrackType track = cDevice::PrimaryDevice()->GetCurrentAudioTrack();

  if (m_Osd) {
     if (m_DisplayMode == modeTransponder) {
        m_Osd->DrawRectangle(0, OSDINFOWIN_Y(0), OSDWIDTH, OSDINFOWIN_Y(OSDINFOHEIGHT), clrBackground);
        m_Osd->DrawRectangle(0, OSDINFOWIN_Y(offset), OSDWIDTH, OSDINFOWIN_Y(offset+m_Font->Height()-1), clrWhite);
        m_Osd->DrawText( OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Transponder Information"), clrBackground, clrWhite, m_Font);
        offset += m_Font->Height();
        m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Vpid"), clrWhite, clrBackground, m_Font);
        snprintf(buf, sizeof(buf), "%d", channel->Vpid());
        m_Osd->DrawText(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
        m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Ppid"), clrWhite, clrBackground, m_Font);
        snprintf(buf, sizeof(buf), "%d", channel->Ppid());
        m_Osd->DrawText(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
        offset += m_Font->Height();
        m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Apid"), clrWhite, clrBackground, m_Font);
        snprintf(buf, sizeof(buf), "%d", channel->Apid(0));
        value = 1;
        while (channel->Apid(value) && (value < MAXAPIDS)) {
           snprintf(buf2, sizeof(buf2), ", %d", channel->Apid(value++));
           strncat(buf, buf2, sizeof(buf));
           }
        m_Osd->DrawText(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
        m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Dpid"), clrWhite, clrBackground, m_Font);
        snprintf(buf, sizeof(buf), "%d", channel->Dpid(0));
        value = 1;
        while (channel->Dpid(value) && (value < MAXAPIDS)) {
           snprintf(buf2, sizeof(buf2), ", %d", channel->Dpid(value++));
           strncat(buf, buf2, sizeof(buf));
           }
        m_Osd->DrawText(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
        offset += m_Font->Height();
        m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("CA"), clrWhite, clrBackground, m_Font);
        value = channel->Ca();
        if (femonConfig.showcasystem) {
           /* http://www.dvb.org/index.php?id=174 */
           switch (value) {
             case 0x0000:
                  /* Reserved */
                  snprintf(buf, sizeof(buf), "%s", tr("Free to Air"));
                  break;
             case 0x0001 ... 0x00FF:
                  /* Standardized systems */
                  snprintf(buf, sizeof(buf), "%s", tr("Fixed"));
                  break;
             case 0x0100 ... 0x01FF:
                  /* Canal Plus */
                  snprintf(buf, sizeof(buf), "%s", tr("SECA/Mediaguard"));
                  break;
             case 0x0500 ... 0x05FF:
                  /* France Telecom */
                  snprintf(buf, sizeof(buf), "%s", tr("Viaccess"));
                  break;
             case 0x0600 ... 0x06FF:
                  /* Irdeto */
                  snprintf(buf, sizeof(buf), "%s", tr("Irdeto"));
                  break;
             case 0x0900 ... 0x09FF:
                  /* News Datacom */
                  snprintf(buf, sizeof(buf), "%s", tr("NDS/Videoguard"));
                  break;
             case 0x0B00 ... 0x0BFF:
                  /* Norwegian Telekom */
                  snprintf(buf, sizeof(buf), "%s", tr("Conax"));
                  break;
             case 0x0D00 ... 0x0DFF:
                  /* Philips */
                  snprintf(buf, sizeof(buf), "%s", tr("CryptoWorks"));
                  break;
             case 0x0E00 ... 0x0EFF:
                  /* Scientific Atlanta */
                  snprintf(buf, sizeof(buf), "%s", tr("PowerVu"));
                  break;
             case 0x1200 ... 0x12FF:
                  /* BellVu Express */
                  snprintf(buf, sizeof(buf), "%s", tr("NagraVision"));
                  break;
             case 0x1700 ... 0x17FF:
                  /* BetaTechnik */
                  snprintf(buf, sizeof(buf), "%s", tr("BetaCrypt"));
                  break;
             case 0x1800 ... 0x18FF:
                  /* Kudelski SA */
                  snprintf(buf, sizeof(buf), "%s", tr("NagraVision"));
                  break;
             case 0x4A60 ... 0x4A6F:
                  /* @Sky */
                  snprintf(buf, sizeof(buf), "%s", tr("SkyCrypt"));
                  break;
             default:
                  snprintf(buf, sizeof(buf), "%X", value);
                  break;
             }
           }
        else {
           snprintf(buf, sizeof(buf), "%X", value);
           value = 1;
           while (channel->Ca(value) && (value < MAXCAIDS)) {
             snprintf(buf2, sizeof(buf2), ", %X", channel->Ca(value++));
             strncat(buf, buf2, sizeof(buf));
             }
           }
        m_Osd->DrawText(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
        m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Tpid"), clrWhite, clrBackground, m_Font);
        snprintf(buf, sizeof(buf), "%d", channel->Tpid());
        m_Osd->DrawText(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
        offset += m_Font->Height();
        m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Sid"), clrWhite, clrBackground, m_Font);
        snprintf(buf, sizeof(buf), "%d", channel->Sid());
        m_Osd->DrawText(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
        m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Nid"), clrWhite, clrBackground, m_Font);
        snprintf(buf, sizeof(buf), "%d", channel->Nid());
        m_Osd->DrawText(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
        offset += m_Font->Height();
        m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Tid"), clrWhite, clrBackground, m_Font);
        snprintf(buf, sizeof(buf), "%d", channel->Tid());
        m_Osd->DrawText(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
        m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Rid"), clrWhite, clrBackground, m_Font);
        snprintf(buf, sizeof(buf), "%d", channel->Rid());
        m_Osd->DrawText(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
        offset += m_Font->Height();
        switch (m_FrontendInfo.type) {
          case FE_QPSK:
               snprintf(buf, sizeof(buf), "%s #%d - %s", tr("Satellite Card"), cDevice::ActualDevice()->CardIndex(), m_FrontendInfo.name);
               m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               offset += m_Font->Height();
               m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Frequency"), clrWhite, clrBackground, m_Font);
               value = channel->Frequency();
               while (value > 20000) value /= 1000;
               snprintf(buf, sizeof(buf), "%d %s", value, tr("MHz"));
               m_Osd->DrawText(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Source"), clrWhite, clrBackground, m_Font);
               snprintf(buf, sizeof(buf), "%s", *cSource::ToString(channel->Source()));
               m_Osd->DrawText(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               offset += m_Font->Height();
               m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Srate"), clrWhite, clrBackground, m_Font);
               snprintf(buf, sizeof(buf), "%d", channel->Srate());
               m_Osd->DrawText(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Polarization"), clrWhite, clrBackground, m_Font);
               snprintf(buf, sizeof(buf), "%c", toupper(channel->Polarization()));
               m_Osd->DrawText(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               offset += m_Font->Height();
               m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Inversion"), clrWhite, clrBackground, m_Font);
               value = channel->Inversion();
               if      (value == INVERSION_OFF)   snprintf(buf, sizeof(buf), tr("Off"));
               else if (value == INVERSION_ON)    snprintf(buf, sizeof(buf), tr("On"));
               else            /*INVERSION_AUTO*/ snprintf(buf, sizeof(buf), tr("Auto"));
               m_Osd->DrawText(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("CoderateH"), clrWhite, clrBackground, m_Font);
               value = channel->CoderateH();
               if      (value == FEC_NONE)  snprintf(buf, sizeof(buf), tr("None"));
               else if (value == FEC_1_2)   snprintf(buf, sizeof(buf), "1/2");
               else if (value == FEC_2_3)   snprintf(buf, sizeof(buf), "2/3");
               else if (value == FEC_3_4)   snprintf(buf, sizeof(buf), "3/4");
               else if (value == FEC_4_5)   snprintf(buf, sizeof(buf), "4/5");
               else if (value == FEC_5_6)   snprintf(buf, sizeof(buf), "5/6");
               else if (value == FEC_6_7)   snprintf(buf, sizeof(buf), "6/7");
               else if (value == FEC_7_8)   snprintf(buf, sizeof(buf), "7/8");
               else if (value == FEC_8_9)   snprintf(buf, sizeof(buf), "8/9");
               else            /*FEC_AUTO*/ snprintf(buf, sizeof(buf), tr("Auto"));
               m_Osd->DrawText(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               break;

          case FE_QAM:
               snprintf(buf, sizeof(buf), "%s #%d - %s", tr("Cable Card"), cDevice::ActualDevice()->CardIndex(), m_FrontendInfo.name);
               m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               offset += m_Font->Height();
               m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Frequency"), clrWhite, clrBackground, m_Font);
               value = channel->Frequency();
               while (value > 20000) value /= 1000;
               snprintf(buf, sizeof(buf), "%d %s", value, tr("MHz"));
               m_Osd->DrawText(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Source"), clrWhite, clrBackground, m_Font);
               snprintf(buf, sizeof(buf), "%s", *cSource::ToString(channel->Source()));
               m_Osd->DrawText(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               offset += m_Font->Height();
               m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Srate"), clrWhite, clrBackground, m_Font);
               snprintf(buf, sizeof(buf), "%d", channel->Srate());
               m_Osd->DrawText(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Modulation"), clrWhite, clrBackground, m_Font);
               value = channel->Modulation();
               if      (value == QPSK)      snprintf(buf, sizeof(buf), "QPSK");
               else if (value == QAM_16)    snprintf(buf, sizeof(buf), "QAM 16");
               else if (value == QAM_32)    snprintf(buf, sizeof(buf), "QAM 32");
               else if (value == QAM_64)    snprintf(buf, sizeof(buf), "QAM 64");
               else if (value == QAM_128)   snprintf(buf, sizeof(buf), "QAM 128");
               else if (value == QAM_256)   snprintf(buf, sizeof(buf), "QAM 256");
               else            /*QAM_AUTO*/ snprintf(buf, sizeof(buf), "QAM %s", tr("Auto"));
               m_Osd->DrawText(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               offset += m_Font->Height();
               m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Inversion"), clrWhite, clrBackground, m_Font);
               value = channel->Inversion();
               if      (value == INVERSION_OFF)   snprintf(buf, sizeof(buf), tr("Off"));
               else if (value == INVERSION_ON)    snprintf(buf, sizeof(buf), tr("On"));
               else            /*INVERSION_AUTO*/ snprintf(buf, sizeof(buf), tr("Auto"));
               m_Osd->DrawText(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("CoderateH"), clrWhite, clrBackground, m_Font);
               value = channel->CoderateH();
               if      (value == FEC_NONE)  snprintf(buf, sizeof(buf), tr("None"));
               else if (value == FEC_1_2)   snprintf(buf, sizeof(buf), "1/2");
               else if (value == FEC_2_3)   snprintf(buf, sizeof(buf), "2/3");
               else if (value == FEC_3_4)   snprintf(buf, sizeof(buf), "3/4");
               else if (value == FEC_4_5)   snprintf(buf, sizeof(buf), "4/5");
               else if (value == FEC_5_6)   snprintf(buf, sizeof(buf), "5/6");
               else if (value == FEC_6_7)   snprintf(buf, sizeof(buf), "6/7");
               else if (value == FEC_7_8)   snprintf(buf, sizeof(buf), "7/8");
               else if (value == FEC_8_9)   snprintf(buf, sizeof(buf), "8/9");
               else            /*FEC_AUTO*/ snprintf(buf, sizeof(buf), tr("Auto"));
               m_Osd->DrawText(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               break;

          default:
               snprintf(buf, sizeof(buf), "%s #%d - %s", tr("Terrestial Card"), cDevice::ActualDevice()->CardIndex(), m_FrontendInfo.name);
               m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               offset += m_Font->Height();
               m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Frequency"), clrWhite, clrBackground, m_Font);
               value = channel->Frequency();
               while (value > 20000) value /= 1000;
               snprintf(buf, sizeof(buf), "%d %s", value, tr("MHz"));
               m_Osd->DrawText(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Transmission"), clrWhite, clrBackground, m_Font);
               value = channel->Transmission();
               if      (value == TRANSMISSION_MODE_2K)    snprintf(buf, sizeof(buf), "2K");
               else if (value == TRANSMISSION_MODE_8K)    snprintf(buf, sizeof(buf), "8K");
               else            /*TRANSMISSION_MODE_AUTO*/ snprintf(buf, sizeof(buf), tr("Auto"));
               m_Osd->DrawText(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               offset += m_Font->Height();
               m_Osd->DrawText( OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Bandwidth"), clrWhite, clrBackground, m_Font);
               value = channel->Bandwidth();
               if      (value == BANDWIDTH_8_MHZ) snprintf(buf, sizeof(buf), "8 %s", tr("MHz"));
               else if (value == BANDWIDTH_7_MHZ) snprintf(buf, sizeof(buf), "7 %s", tr("MHz"));
               else if (value == BANDWIDTH_6_MHZ) snprintf(buf, sizeof(buf), "6 %s", tr("MHz"));
               else            /*BANDWIDTH_AUTO*/ snprintf(buf, sizeof(buf), tr("Auto"));
               m_Osd->DrawText(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Modulation"), clrWhite, clrBackground, m_Font);
               value = channel->Modulation();
               if      (value == QPSK)      snprintf(buf, sizeof(buf), "QPSK");
               else if (value == QAM_16)    snprintf(buf, sizeof(buf), "QAM 16");
               else if (value == QAM_32)    snprintf(buf, sizeof(buf), "QAM 32");
               else if (value == QAM_64)    snprintf(buf, sizeof(buf), "QAM 64");
               else if (value == QAM_128)   snprintf(buf, sizeof(buf), "QAM 128");
               else if (value == QAM_256)   snprintf(buf, sizeof(buf), "QAM 256");
               else            /*QAM_AUTO*/ snprintf(buf, sizeof(buf), "QAM %s", tr("Auto"));
               m_Osd->DrawText(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               offset += m_Font->Height();
               m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Inversion"), clrWhite, clrBackground, m_Font);
               value = channel->Inversion();
               if      (value == INVERSION_OFF)   snprintf(buf, sizeof(buf), tr("Off"));
               else if (value == INVERSION_ON)    snprintf(buf, sizeof(buf), tr("On"));
               else            /*INVERSION_AUTO*/ snprintf(buf, sizeof(buf), tr("Auto"));
               m_Osd->DrawText(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("CoderateH"), clrWhite, clrBackground, m_Font);
               value = channel->CoderateH();
               if      (value == FEC_NONE)  snprintf(buf, sizeof(buf), tr("None"));
               else if (value == FEC_1_2)   snprintf(buf, sizeof(buf), "1/2");
               else if (value == FEC_2_3)   snprintf(buf, sizeof(buf), "2/3");
               else if (value == FEC_3_4)   snprintf(buf, sizeof(buf), "3/4");
               else if (value == FEC_4_5)   snprintf(buf, sizeof(buf), "4/5");
               else if (value == FEC_5_6)   snprintf(buf, sizeof(buf), "5/6");
               else if (value == FEC_6_7)   snprintf(buf, sizeof(buf), "6/7");
               else if (value == FEC_7_8)   snprintf(buf, sizeof(buf), "7/8");
               else if (value == FEC_8_9)   snprintf(buf, sizeof(buf), "8/9");
               else            /*FEC_AUTO*/ snprintf(buf, sizeof(buf), tr("Auto"));
               value = channel->CoderateL();
               if      (value == FEC_NONE)  snprintf(buf2, sizeof(buf2), " - %s", tr("None"));
               else if (value == FEC_1_2)   snprintf(buf2, sizeof(buf2), " - 1/2");
               else if (value == FEC_2_3)   snprintf(buf2, sizeof(buf2), " - 2/3");
               else if (value == FEC_3_4)   snprintf(buf2, sizeof(buf2), " - 3/4");
               else if (value == FEC_4_5)   snprintf(buf2, sizeof(buf2), " - 4/5");
               else if (value == FEC_5_6)   snprintf(buf2, sizeof(buf2), " - 5/6");
               else if (value == FEC_6_7)   snprintf(buf2, sizeof(buf2), " - 6/7");
               else if (value == FEC_7_8)   snprintf(buf2, sizeof(buf2), " - 7/8");
               else if (value == FEC_8_9)   snprintf(buf2, sizeof(buf2), " - 8/9");
               else            /*FEC_AUTO*/ snprintf(buf2, sizeof(buf2), " - %s", tr("Auto"));
               strncat(buf, buf2, sizeof(buf));
               m_Osd->DrawText(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               offset += m_Font->Height();
               m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Hierarchy"), clrWhite, clrBackground, m_Font);
               value = channel->Hierarchy();
               if      (value == HIERARCHY_NONE)  snprintf(buf, sizeof(buf), tr("None"));
               else if (value == HIERARCHY_1)     snprintf(buf, sizeof(buf), "1");
               else if (value == HIERARCHY_2)     snprintf(buf, sizeof(buf), "2");
               else if (value == HIERARCHY_4)     snprintf(buf, sizeof(buf), "4");
               else            /*HIERARCHY_AUTO*/ snprintf(buf, sizeof(buf), tr("Auto"));
               m_Osd->DrawText(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Guard"), clrWhite, clrBackground, m_Font);
               value = channel->Guard();
               if      (value == GUARD_INTERVAL_1_32)  snprintf(buf, sizeof(buf), "1/32");
               else if (value == GUARD_INTERVAL_1_16)  snprintf(buf, sizeof(buf), "1/16");
               else if (value == GUARD_INTERVAL_1_8)   snprintf(buf, sizeof(buf), "1/8");
               else if (value == GUARD_INTERVAL_1_4)   snprintf(buf, sizeof(buf), "1/4");
               else            /*GUARD_INTERVAL_AUTO*/ snprintf(buf, sizeof(buf), tr("Auto"));
               m_Osd->DrawText(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
               break;
          }
        }
     else if (m_DisplayMode == modeStream) {
        m_Osd->DrawRectangle(0, OSDINFOWIN_Y(0), OSDWIDTH, OSDINFOWIN_Y(OSDINFOHEIGHT), clrBackground);
        m_Osd->DrawRectangle(0, OSDINFOWIN_Y(offset), OSDWIDTH, OSDINFOWIN_Y(offset+m_Font->Height()-1), clrWhite);
        m_Osd->DrawText( OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Stream Information"), clrBackground, clrWhite, m_Font);
        offset += m_Font->Height();
        m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Video Stream"), clrYellow, clrBackground, m_Font);
        snprintf(buf, sizeof(buf), "#%d", channel->Vpid());
        m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
        offset += m_Font->Height();
        m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Bitrate"), clrWhite, clrBackground, m_Font);
        if (m_Receiver) snprintf(buf, sizeof(buf), "%.2f %s (%.2f %s)", m_Receiver->VideoStreamBitrate(), tr("Mbit/s"), m_Receiver->VideoBitrate(), tr("Mbit/s"));
        else            snprintf(buf, sizeof(buf), "---");
        m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
        offset += m_Font->Height();
        m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Aspect Ratio"), clrWhite, clrBackground, m_Font);
        if (m_Receiver) {
           value = m_Receiver->VideoAspectRatio();
           if      (value == AR_1_1)    snprintf(buf, sizeof(buf), "1:1");
           else if (value == AR_4_3)    snprintf(buf, sizeof(buf), "4:3");
           else if (value == AR_16_9)   snprintf(buf, sizeof(buf), "16:9");
           else if (value == AR_2_21_1) snprintf(buf, sizeof(buf), "2.21:1");
           else                         snprintf(buf, sizeof(buf), "%s", tr("reserved"));
           }
        else                            snprintf(buf, sizeof(buf), "---");
        m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
        offset += m_Font->Height();
        m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Frame Rate"), clrWhite, clrBackground, m_Font);
        if (m_Receiver) snprintf(buf, sizeof(buf), "%.2f %s", m_Receiver->VideoFrameRate(), tr("Hz"));
        else            snprintf(buf, sizeof(buf), "---");
        m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
        offset += m_Font->Height();
        m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Video Format"), clrWhite, clrBackground, m_Font);
        if (m_Receiver) {
           value = m_Receiver->VideoFormat();
           if      (value == VF_PAL)  snprintf(buf, sizeof(buf), "%s", tr("PAL"));
           else if (value == VF_NTSC) snprintf(buf, sizeof(buf), "%s", tr("NTSC"));
           else                       snprintf(buf, sizeof(buf), "%s", tr("unknown"));
           }
        else                          snprintf(buf, sizeof(buf), "---");
        m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
        offset += m_Font->Height();
        m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Resolution"), clrWhite, clrBackground, m_Font);
        if (m_Receiver) snprintf(buf, sizeof(buf), "%d x %d", m_Receiver->VideoHorizontalSize(), m_Receiver->VideoVerticalSize());
        else            snprintf(buf, sizeof(buf), "---");
        m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
        offset += m_Font->Height();
        m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Audio Stream"), clrYellow, clrBackground, m_Font);
        snprintf(buf, sizeof(buf), "#%d %s", IS_AUDIO_TRACK(track) ? channel->Apid(int(track - ttAudioFirst)) : channel->Apid(0), IS_AUDIO_TRACK(track) ? channel->Alang(int(track - ttAudioFirst)) : channel->Alang(0));
        m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
        offset += m_Font->Height();
        m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Bitrate"), clrWhite, clrBackground, m_Font);
        if (m_Receiver) {
           dvalue = m_Receiver->AudioStreamBitrate();
           if      (dvalue == (double)FR_RESERVED) snprintf(buf, sizeof(buf), "%s (%.0f %s)", tr("reserved"), m_Receiver->AudioBitrate(), tr("kbit/s"));
           else if (dvalue == (double)FR_FREE)     snprintf(buf, sizeof(buf), "%s (%.0f %s)", tr("free"), m_Receiver->AudioBitrate(), tr("kbit/s"));
           else                                    snprintf(buf, sizeof(buf), "%.0f %s (%.0f %s)", dvalue, tr("kbit/s"), m_Receiver->AudioBitrate(), tr("kbit/s"));
           }
        else                                       snprintf(buf, sizeof(buf), "---");
        m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
        offset += m_Font->Height();
        m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("MPEG Layer"), clrWhite, clrBackground, m_Font);
        if (m_Receiver) snprintf(buf, sizeof(buf), "%d", m_Receiver->AudioMPEGLayer());
        else            snprintf(buf, sizeof(buf), "---");
        m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
        offset += m_Font->Height();
        m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Sampling Frequency"), clrWhite, clrBackground, m_Font);
        if (m_Receiver) {
           value = m_Receiver->AudioSamplingFreq();
           if (value == FR_RESERVED) snprintf(buf, sizeof(buf), "%s", tr("reserved"));
           else                      snprintf(buf, sizeof(buf), "%.1f %s", (value / 1000.0), tr("kHz"));
           }
        else                         snprintf(buf, sizeof(buf), "---");
        m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
        offset += m_Font->Height();
        }
     else if (m_DisplayMode == modeAC3) {
        m_Osd->DrawRectangle(0, OSDINFOWIN_Y(0), OSDWIDTH, OSDINFOWIN_Y(OSDINFOHEIGHT), clrBackground);
        m_Osd->DrawRectangle(0, OSDINFOWIN_Y(offset), OSDWIDTH, OSDINFOWIN_Y(offset+m_Font->Height()-1), clrWhite);
        snprintf(buf, sizeof(buf), "%s - %s #%d %s", tr("Stream Information"), tr("AC-3 Stream"), IS_DOLBY_TRACK(track) ? channel->Dpid(int(track - ttDolbyFirst)) : channel->Dpid(0), IS_DOLBY_TRACK(track) ? channel->Dlang(int(track - ttDolbyFirst)) : channel->Dlang(0));
        m_Osd->DrawText( OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), buf, clrBackground, clrWhite, m_Font);
        offset += m_Font->Height();
        if (m_Receiver && m_Receiver->AC3Valid() && IS_DOLBY_TRACK(track)) {
           m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Bitrate"), clrWhite, clrBackground, m_Font);
           snprintf(buf, sizeof(buf), "%.0f %s (%0.f %s)", m_Receiver->AC3StreamBitrate(), tr("kbit/s"), m_Receiver->AC3Bitrate(), tr("kbit/s"));
           m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
           offset += m_Font->Height();
           m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Sampling Frequency"), clrWhite, clrBackground, m_Font);
           snprintf(buf, sizeof(buf), "%.1f %s", m_Receiver->AC3SamplingFreq() / 1000., tr("kHz"));
           m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
           offset += m_Font->Height();
           m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Frame Size"), clrWhite, clrBackground, m_Font);
           snprintf(buf, sizeof(buf), "%d", m_Receiver->AC3FrameSize());
           m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
           offset += m_Font->Height();
           m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Bit Stream Mode"), clrWhite, clrBackground, m_Font);
           switch (m_Receiver->AC3BitStreamMode()) {
             case 0: snprintf(buf, sizeof(buf), tr("Complete Main (CM)"));     break;
             case 1: snprintf(buf, sizeof(buf), tr("Music and Effects (ME)")); break;
             case 2: snprintf(buf, sizeof(buf), tr("Visually Impaired (VI)")); break;
             case 3: snprintf(buf, sizeof(buf), tr("Hearing Impaired (HI)"));  break;
             case 4: snprintf(buf, sizeof(buf), tr("Dialogue (D)"));           break;
             case 5: snprintf(buf, sizeof(buf), tr("Commentary (C)"));         break;
             case 6: snprintf(buf, sizeof(buf), tr("Emergency (E)"));          break;
             case 7: (m_Receiver->AC3AudioCodingMode() == 1) ? snprintf(buf, sizeof(buf), tr("Voice Over (VO)")) : snprintf(buf, sizeof(buf), tr("Karaoke")); break;
             default: snprintf(buf, sizeof(buf), "---");
             }
           m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
           offset += m_Font->Height();
           m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Audio Coding Mode"), clrWhite, clrBackground, m_Font);
           if (m_Receiver->AC3BitStreamMode() != 7) {
              switch (m_Receiver->AC3AudioCodingMode()) {
                case 0:  snprintf(buf, sizeof(buf), "1+1 - %s, %s",             tr("Ch1"), tr("Ch2"));                           break;
                case 1:  snprintf(buf, sizeof(buf), "1/0 - %s",                 tr("C"));                                        break;
                case 2:  snprintf(buf, sizeof(buf), "2/0 - %s, %s",             tr("L"), tr("R"));                               break;
                case 3:  snprintf(buf, sizeof(buf), "3/0 - %s, %s, %s",         tr("L"), tr("C"), tr("R"));                      break;
                case 4:  snprintf(buf, sizeof(buf), "2/1 - %s, %s, %s",         tr("L"), tr("R"), tr("S"));                      break;
                case 5:  snprintf(buf, sizeof(buf), "3/1 - %s, %s, %s, %s",     tr("L"), tr("C"), tr("R"),  tr("S"));            break;
                case 6:  snprintf(buf, sizeof(buf), "2/2 - %s, %s, %s, %s",     tr("L"), tr("R"), tr("SL"), tr("SR"));           break;
                case 7:  snprintf(buf, sizeof(buf), "3/2 - %s, %s, %s, %s, %s", tr("L"), tr("C"), tr("R"),  tr("SL"), tr("SR")); break;
                default: snprintf(buf, sizeof(buf), "---");
                }
             }
	   else snprintf(buf, sizeof(buf), "---");
           m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
           offset += m_Font->Height();
           m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Center Mix Level"), clrWhite, clrBackground, m_Font);
           switch (m_Receiver->AC3CenterMixLevel()) {
             case CML_MINUS_3dB:   snprintf(buf, sizeof(buf), "-3.0 %s", tr("dB"));  break;
             case CML_MINUS_4_5dB: snprintf(buf, sizeof(buf), "-4.5 %s", tr("dB"));  break;
             case CML_MINUS_6dB:   snprintf(buf, sizeof(buf), "-6.0 %s", tr("dB"));  break;
             case CML_RESERVED:    snprintf(buf, sizeof(buf), "%s", tr("reserved")); break;
             default:              snprintf(buf, sizeof(buf), "---");
             }
           m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
           offset += m_Font->Height();
           m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Surround Mix Level"), clrWhite, clrBackground, m_Font);
           switch (m_Receiver->AC3SurroundMixLevel()) {
             case SML_MINUS_3dB: snprintf(buf, sizeof(buf), "-3 %s", tr("dB"));    break;
             case SML_MINUS_6dB: snprintf(buf, sizeof(buf), "-6 %s", tr("dB"));    break;
             case SML_0_dB:      snprintf(buf, sizeof(buf), "0 %s", tr("dB"));     break;
             case SML_RESERVED:  snprintf(buf, sizeof(buf), "%s", tr("reserved")); break;
             default:            snprintf(buf, sizeof(buf), "---");
             }
           m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
           offset += m_Font->Height();
           m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Dolby Surround Mode"), clrWhite, clrBackground, m_Font);
           switch (m_Receiver->AC3DolbySurroundMode()) {
             case DSM_NOT_INDICATED:     snprintf(buf, sizeof(buf), "%s", tr("not indicated")); break;
             case DSM_NOT_DOLBYSURROUND: snprintf(buf, sizeof(buf), "%s", tr("no"));            break;
             case DSM_DOLBYSURROUND:     snprintf(buf, sizeof(buf), "%s", tr("yes"));           break;
             case DSM_RESERVED:          snprintf(buf, sizeof(buf), "%s", tr("reserved"));      break;
             default:                    snprintf(buf, sizeof(buf), "---");
             }
           m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
           offset += m_Font->Height();
           m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Low Frequency Effects"), clrWhite, clrBackground, m_Font);
           snprintf(buf, sizeof(buf), "%s", m_Receiver->AC3LfeOn() ? tr("On") : tr("Off"));
           m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
           offset += m_Font->Height();
           m_Osd->DrawText(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Dialogue Normalization"), clrWhite, clrBackground, m_Font);
           value = m_Receiver->AC3DialogLevel();
           if (value > 0) snprintf(buf, sizeof(buf), "-%d %s", value, tr("dB"));
           else           snprintf(buf, sizeof(buf), "---");
           m_Osd->DrawText(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_Font);
           offset += m_Font->Height();
           }
        }
     else /* modeBasic */ {
        m_Osd->DrawRectangle(0, OSDINFOWIN_Y(0), OSDWIDTH, OSDINFOWIN_Y(OSDINFOHEIGHT), clrTransparent);
        }
     m_Osd->Flush();
     }
}

void cFemonOsd::Action(void)
{
  debug(printf("cFemonOsd::Action()\n"));
  cTimeMs t;
  m_Active = true;
  while (m_Active) {
    t.Set(0);
    if (m_Frontend != -1) {
       CHECK(ioctl(m_Frontend, FE_READ_STATUS, &m_FrontendStatus));
       CHECK(ioctl(m_Frontend, FE_READ_SIGNAL_STRENGTH, &m_Signal));
       CHECK(ioctl(m_Frontend, FE_READ_SNR, &m_SNR));
       CHECK(ioctl(m_Frontend, FE_READ_BER, &m_BER));
       CHECK(ioctl(m_Frontend, FE_READ_UNCORRECTED_BLOCKS, &m_UNC));
       DrawInfoWindow();
       DrawStatusWindow();
       if (femonConfig.syslogoutput) {
          isyslog("Card #%d (%s) STR: %04x SNR: %04x BER: %08x UNC: %08x |%c|%c|%c|%c|%c|", cDevice::ActualDevice()->CardIndex(), m_FrontendInfo.name, m_Signal, m_SNR, m_BER, m_UNC, (m_FrontendStatus & FE_HAS_LOCK) ? 'L' : ' ', (m_FrontendStatus & FE_HAS_SIGNAL) ? 'S' : ' ', (m_FrontendStatus & FE_HAS_CARRIER) ? 'C' : ' ', (m_FrontendStatus & FE_HAS_VITERBI) ? 'V' : ' ', (m_FrontendStatus & FE_HAS_SYNC) ? 'Z' : ' ');
          }
       }
    cCondWait::SleepMs(100 * femonConfig.updateinterval - t.Elapsed());
    }
}

void cFemonOsd::Show(void)
{
  debug(printf("cFemonOsd::Show()\n"));
  int apid = 0, dpid = 0;
  char *dev = NULL;
  eTrackType track = cDevice::PrimaryDevice()->GetCurrentAudioTrack();
  asprintf(&dev, FRONTEND_DEVICE, cDevice::ActualDevice()->CardIndex(), 0);
  m_Frontend = open(dev, O_RDONLY | O_NONBLOCK);
  free(dev);
  if (m_Frontend < 0) {
     isyslog("cFemonOsd::Show() cannot open frontend device.");
     m_Frontend = -1;
     return;
     }
  if (ioctl(m_Frontend, FE_GET_INFO, &m_FrontendInfo) < 0) {
     isyslog("cFemonOsd::Show() cannot read frontend info.");
     close(m_Frontend);
     m_Frontend = -1;
     return;
     }
  m_Osd = cOsdProvider::NewOsd(((Setup.OSDWidth - OSDWIDTH) / 2) + Setup.OSDLeft, ((Setup.OSDHeight - OSDHEIGHT) / 2) + Setup.OSDTop);
  if (m_Osd) {
     tArea Areas[] = { { 0, 0, OSDWIDTH, OSDHEIGHT, 4 } };
     if (m_Osd->CanHandleAreas(Areas, sizeof(Areas) / sizeof(tArea)) == oeOk) {
        m_Osd->SetAreas(Areas, sizeof(Areas) / sizeof(tArea));
        }
     else {
        tArea Areas[] = { { 0, OSDSTATUSWIN_Y(0), (OSDWIDTH-1), OSDSTATUSWIN_Y(OSDSTATUSHEIGHT-1), 4 },
                          { 0, OSDINFOWIN_Y(0),   (OSDWIDTH-1), OSDINFOWIN_Y(OSDINFOHEIGHT-1),     2 }
                        };
        m_Osd->SetAreas(Areas, sizeof(Areas) / sizeof(tArea));
        }
     m_Osd->DrawRectangle(0, OSDINFOWIN_Y(0), OSDWIDTH, OSDINFOWIN_Y(OSDINFOHEIGHT), clrTransparent);
     m_Osd->Flush();
     if (m_Receiver)
        delete m_Receiver;
     if (femonConfig.analyzestream) {
        if (IS_AUDIO_TRACK(track)) apid = int(track - ttAudioFirst);
        else if (IS_DOLBY_TRACK(track)) dpid = int(track - ttDolbyFirst);
        cChannel *channel = Channels.GetByNumber(cDevice::CurrentChannel());
        m_Receiver = new cFemonReceiver(channel->Ca(), channel->Vpid(), channel->Apid(apid), channel->Dpid(dpid));
        cDevice::ActualDevice()->AttachReceiver(m_Receiver);
        }
     Start();
     }
}

void cFemonOsd::ChannelSwitch(const cDevice * device, int channelNumber)
{
  debug(printf("cFemonOsd::ChannelSwitch()\n"));
  int apid = 0, dpid = 0;
  char *dev = NULL;
  eTrackType track = cDevice::PrimaryDevice()->GetCurrentAudioTrack();
  if (!device->IsPrimaryDevice() || !channelNumber || cDevice::PrimaryDevice()->CurrentChannel() != channelNumber)
     return;
  close(m_Frontend);
  asprintf(&dev, FRONTEND_DEVICE, cDevice::ActualDevice()->CardIndex(), 0);
  m_Frontend = open(dev, O_RDONLY | O_NONBLOCK);
  free(dev);
  if (m_Frontend < 0) {
     isyslog("cFemonOsd::ChannelSwitch() cannot open frontend device.");
     m_Frontend = -1;
     return;
     }
  if (ioctl(m_Frontend, FE_GET_INFO, &m_FrontendInfo) < 0) {
     isyslog("cFemonOsd::ChannelSwitch() cannot read frontend info.");
     close(m_Frontend);
     m_Frontend = -1;
     return;
     }
  if (m_Receiver)
     delete m_Receiver;
  if (femonConfig.analyzestream) {
     if (IS_AUDIO_TRACK(track)) apid = int(track - ttAudioFirst);
     else if (IS_DOLBY_TRACK(track)) dpid = int(track - ttDolbyFirst);
     cChannel *channel = Channels.GetByNumber(cDevice::CurrentChannel());
     m_Receiver = new cFemonReceiver(channel->Ca(), channel->Vpid(), channel->Apid(apid), channel->Dpid(dpid));
     cDevice::ActualDevice()->AttachReceiver(m_Receiver);
     }
}

void cFemonOsd::SetAudioTrack(int Index, const char * const *Tracks)
{
  debug(printf("cFemonOsd::SetAudioTrack()\n"));
  int apid = 0, dpid = 0;
  eTrackType track = cDevice::PrimaryDevice()->GetCurrentAudioTrack();
  if (m_Receiver)
     delete m_Receiver;
  if (femonConfig.analyzestream) {
     if (IS_AUDIO_TRACK(track)) apid = int(track - ttAudioFirst);
     else if (IS_DOLBY_TRACK(track)) dpid = int(track - ttDolbyFirst);
     cChannel *channel = Channels.GetByNumber(cDevice::CurrentChannel());
     m_Receiver = new cFemonReceiver(channel->Ca(), channel->Vpid(), channel->Apid(apid), channel->Dpid(dpid));
     cDevice::ActualDevice()->AttachReceiver(m_Receiver);
     }
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
       case kLeft:
            {
            int device = cDevice::ActualDevice()->DeviceNumber();
            if (device >= 0) {
               cChannel *channel = Channels.GetByNumber(cDevice::CurrentChannel());
               for (int i = 0; i < cDevice::NumDevices() - 1; i++) {
                   if (NORMALKEY(Key) == kBlue) {
                      if (++device >= cDevice::NumDevices()) device = 0;
                      }
                   else {
                      if (--device < 0) device = cDevice::NumDevices() - 1;
                      }
                   if (cDevice::GetDevice(device)->ProvidesChannel(channel)) {
                      //cStatus::MsgChannelSwitch(cDevice::GetDevice(device), 0);
                      //implement some tuning mechanism here
                      //cControl::Launch(new cTransferControl(cDevice::GetDevice(device), channel->Vpid(), channel->Apid(0), channel->Apid(1), channel->Dpid(0), channel->Dpid(1)));
                      //cStatus::MsgChannelSwitch(cDevice::GetDevice(device), channel->Number());
                      break;
                      }
                   }
               }
            }
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
            // toggle between display modes
            if (++m_DisplayMode == modeAC3 && !Channels.GetByNumber(cDevice::CurrentChannel())->Dpid(0)) m_DisplayMode++;
            if (m_DisplayMode >= modeMaxNumber) m_DisplayMode = 0;
            DrawInfoWindow();
            break;
       default:
            break;
       }
     state = osContinue;
  }
  return state;
}
