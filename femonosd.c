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

#if (VDRVERSNUM < 10300) && !defined(ELCHIAIOVERSION)
#warning You should consider using the small fonts!
#endif

#define FRONTEND_DEVICE          "/dev/dvb/adapter%d/frontend%d"
#define CHANNELINPUT_TIMEOUT     1000

#define SCREENWIDTH              720 // in pixels
#define SCREENHEIGHT             576 // in pixels
#define OSDWIDTH                 600 // in pixels
#define OSDHEIGHT                480 // in pixels
#define OSDINFOHEIGHT            ((cOsd::LineHeight() - 2) * 11) // in pixels (11 rows)
#define OSDSTATUSHEIGHT          ((cOsd::LineHeight() - 2) * 6)  // in pixels (6 rows)

#define OSDINFOWIN_Y(offset)     (femonConfig.position ? (OSDHEIGHT - OSDINFOHEIGHT + offset) : offset)
#define OSDINFOWIN_X(col)        ((col == 4) ? 470 : (col == 3) ? 300 : (col==2) ? 180 : 15)
#define OSDSTATUSWIN_Y(offset)   (femonConfig.position ? offset : (OSDHEIGHT - OSDSTATUSHEIGHT + offset))
#define OSDSTATUSWIN_X(col)      ((col == 7) ? 475 : (col == 6) ? 410 : (col == 5) ? 275 : (col == 4) ? 220 : (col == 3) ? 125 : (col==2) ? 70 : 15)
#define OSDSTATUSWIN_XC(col,txt) (((col - 1) * SCREENWIDTH / 6) + ((SCREENWIDTH / 6 - cOsd::WidthInCells(txt) * cOsd::CellWidth()) / 2))
#define BARWIDTH(x)              (OSDWIDTH * x / 100)

cFemonOsd::cFemonOsd(void)
#if VDRVERSNUM >= 10300
:cOsdObject(true), cThread("femon osd")
#else
:cOsdObject(true)
#endif
{
  //printf("cFemonOsd::cFemonOsd()\n");
  m_Osd = NULL;
  m_Receiver = NULL;
  m_Frontend = -1;
  m_Active = false;
  m_Number = 0;
  m_OldNumber = 0;
  m_InputTime = 0;
  m_Signal = 0;
  m_SNR = 0;
  m_BER = 0;
  m_UNC = 0;
  m_DisplayMode = femonConfig.displaymode;
#if (VDRVERSNUM >= 10300) || defined(ELCHIAIOVERSION)
  m_Font = fontSml;
#else
  m_Font = fontOsd;
#endif
  m_Mutex = new cMutex();
}

cFemonOsd::~cFemonOsd(void)
{
  //printf("cFemonOsd::~cFemonOsd()\n");
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
  //printf("cFemonOsd::DrawStatusWindow()\n");
  char buf[128];
  int snr = m_SNR / 655;
  int signal = m_Signal / 655;
  int offset = 0;
  cChannel *channel = Channels.GetByNumber(cDevice::CurrentChannel());

  if (m_Osd) {
#if (VDRVERSNUM >= 10300) || defined(ELCHIAIOVERSION)
     eDvbFont OldFont = m_Osd->SetFont(m_Font);
#endif
     m_Osd->Fill(0, OSDSTATUSWIN_Y(0), OSDWIDTH, OSDSTATUSWIN_Y(OSDSTATUSHEIGHT), clrBackground, m_StatusWindow);
     snprintf(buf, sizeof(buf), "%d%s %s", m_Number ? m_Number : channel->Number(), m_Number ? "-" : "", channel->Name());
     m_Osd->Fill(0, OSDSTATUSWIN_Y(offset), OSDWIDTH, OSDSTATUSWIN_Y(offset+cOsd::LineHeight()-1), clrWhite, m_StatusWindow);
     m_Osd->Text(OSDSTATUSWIN_X(1), OSDSTATUSWIN_Y(offset), buf, clrBlack, clrWhite, m_StatusWindow);
     offset += cOsd::LineHeight();
     if (signal > 0) {
        signal = BARWIDTH(signal);
        m_Osd->Fill(0, OSDSTATUSWIN_Y(offset+3), min(BARWIDTH(femonConfig.redlimit), signal), OSDSTATUSWIN_Y(offset+cOsd::LineHeight()-3), clrRed, m_StatusWindow);
        if (signal > BARWIDTH(femonConfig.redlimit)) {
           m_Osd->Fill(BARWIDTH(femonConfig.redlimit), OSDSTATUSWIN_Y(offset+3), min((OSDWIDTH * femonConfig.greenlimit / 100), signal), OSDSTATUSWIN_Y(offset+cOsd::LineHeight()-3), clrYellow, m_StatusWindow);
           }
        if (signal > BARWIDTH(femonConfig.greenlimit)) {
           m_Osd->Fill(BARWIDTH(femonConfig.greenlimit), OSDSTATUSWIN_Y(offset+3), signal, OSDSTATUSWIN_Y(offset+cOsd::LineHeight()-3), clrGreen, m_StatusWindow);
           }
        }
     offset += cOsd::LineHeight() - 2;
     if (snr > 0) {
        snr = BARWIDTH(snr);
        m_Osd->Fill(0, OSDSTATUSWIN_Y(offset+3), min(BARWIDTH(femonConfig.redlimit), snr), OSDSTATUSWIN_Y(offset+cOsd::LineHeight()-3), clrRed, m_StatusWindow);
        if (snr > BARWIDTH(femonConfig.redlimit)) {
           m_Osd->Fill(BARWIDTH(femonConfig.redlimit), OSDSTATUSWIN_Y(offset+3), min(BARWIDTH(femonConfig.greenlimit), snr), OSDSTATUSWIN_Y(offset+cOsd::LineHeight()-3), clrYellow, m_StatusWindow);
           }
        if (snr > BARWIDTH(femonConfig.greenlimit)) {
           m_Osd->Fill(BARWIDTH(femonConfig.greenlimit),  OSDSTATUSWIN_Y(offset+3), snr, OSDSTATUSWIN_Y(offset+cOsd::LineHeight()-3), clrGreen, m_StatusWindow);
           }
        }
     offset += cOsd::LineHeight() - 2;
     m_Osd->Text(OSDSTATUSWIN_X(1), OSDSTATUSWIN_Y(offset), "STR:", clrWhite, clrBackground, m_StatusWindow);
     snprintf(buf, sizeof(buf), "%04x", m_Signal);
     m_Osd->Text(OSDSTATUSWIN_X(2), OSDSTATUSWIN_Y(offset), buf, clrWhite, clrBackground, m_StatusWindow);
     snprintf(buf, sizeof(buf), "(%2d%%)", m_Signal / 655);
     m_Osd->Text(OSDSTATUSWIN_X(3), OSDSTATUSWIN_Y(offset), buf, clrWhite, clrBackground, m_StatusWindow);
     m_Osd->Text(OSDSTATUSWIN_X(4), OSDSTATUSWIN_Y(offset), "BER:", clrWhite, clrBackground, m_StatusWindow);
     snprintf(buf, sizeof(buf), "%08x", m_BER);
     m_Osd->Text(OSDSTATUSWIN_X(5), OSDSTATUSWIN_Y(offset), buf, clrWhite, clrBackground, m_StatusWindow);
     snprintf(buf, sizeof(buf), "%s:", tr("Video"));
     m_Osd->Text(OSDSTATUSWIN_X(6), OSDSTATUSWIN_Y(offset), buf, clrWhite, clrBackground, m_StatusWindow);
     if (m_Receiver) snprintf(buf, sizeof(buf), "%.2f %s", m_Receiver->VideoBitrate(), tr("Mbit/s"));
     else            snprintf(buf, sizeof(buf), "--- %s", tr("Mbit/s"));
     m_Osd->Text(OSDSTATUSWIN_X(7), OSDSTATUSWIN_Y(offset), buf, clrWhite, clrBackground, m_StatusWindow);
     offset += cOsd::LineHeight() - 2;
     m_Osd->Text(OSDSTATUSWIN_X(1), OSDSTATUSWIN_Y(offset), "SNR:", clrWhite, clrBackground, m_StatusWindow);
     snprintf(buf, sizeof(buf), "%04x", m_SNR);
     m_Osd->Text(OSDSTATUSWIN_X(2), OSDSTATUSWIN_Y(offset), buf, clrWhite, clrBackground, m_StatusWindow);
     snprintf(buf, sizeof(buf), "(%2d%%)", m_SNR / 655);
     m_Osd->Text(OSDSTATUSWIN_X(3), OSDSTATUSWIN_Y(offset), buf, clrWhite, clrBackground, m_StatusWindow);
     m_Osd->Text(OSDSTATUSWIN_X(4), OSDSTATUSWIN_Y(offset), "UNC:", clrWhite, clrBackground, m_StatusWindow);
     snprintf(buf, sizeof(buf), "%08x", m_UNC);
     m_Osd->Text(OSDSTATUSWIN_X(5), OSDSTATUSWIN_Y(offset), buf, clrWhite, clrBackground, m_StatusWindow);
     snprintf(buf, sizeof(buf), "%s:", tr("Audio"));
     m_Osd->Text(OSDSTATUSWIN_X(6), OSDSTATUSWIN_Y(offset), buf, clrWhite, clrBackground, m_StatusWindow);
     if (m_Receiver) snprintf(buf, sizeof(buf), "%.0f %s", m_Receiver->AudioBitrate(), tr("kbit/s"));
     else            snprintf(buf, sizeof(buf), "--- %s", tr("kbit/s"));
     m_Osd->Text(OSDSTATUSWIN_X(7), OSDSTATUSWIN_Y(offset), buf, clrWhite, clrBackground, m_StatusWindow);
     offset += cOsd::LineHeight() - 2;
     m_Osd->Text(OSDSTATUSWIN_XC(1,tr("LOCK")),    OSDSTATUSWIN_Y(offset), tr("LOCK"),   (m_FrontendStatus & FE_HAS_LOCK)   ? clrYellow : clrBlack, clrBackground, m_StatusWindow);
     m_Osd->Text(OSDSTATUSWIN_XC(2,tr("SIGNAL")),  OSDSTATUSWIN_Y(offset), tr("SIGNAL"), (m_FrontendStatus & FE_HAS_SIGNAL) ? clrYellow : clrBlack, clrBackground, m_StatusWindow);
     m_Osd->Text(OSDSTATUSWIN_XC(3,tr("CARRIER")), OSDSTATUSWIN_Y(offset), tr("CARRIER"),(m_FrontendStatus & FE_HAS_CARRIER)? clrYellow : clrBlack, clrBackground, m_StatusWindow);
     m_Osd->Text(OSDSTATUSWIN_XC(4,tr("VITERBI")), OSDSTATUSWIN_Y(offset), tr("VITERBI"),(m_FrontendStatus & FE_HAS_VITERBI)? clrYellow : clrBlack, clrBackground, m_StatusWindow);
     m_Osd->Text(OSDSTATUSWIN_XC(5,tr("SYNC")),    OSDSTATUSWIN_Y(offset), tr("SYNC"),   (m_FrontendStatus & FE_HAS_SYNC)   ? clrYellow : clrBlack, clrBackground, m_StatusWindow);
#if (VDRVERSNUM >= 10300) || defined(ELCHIAIOVERSION)
     m_Osd->SetFont(OldFont);
#endif
     m_Osd->Flush();
     }
}

void cFemonOsd::DrawInfoWindow(void)
{
  cMutexLock lock(m_Mutex);
  //printf("cFemonOsd::DrawInfoWindow()\n");
  char buf[128];
  char buf2[20];
  int offset = 0;
  int value = 0;
  double dvalue = 0.0;
  cChannel *channel = Channels.GetByNumber(cDevice::CurrentChannel());

  if (m_Osd) {
#if (VDRVERSNUM >= 10300) || defined(ELCHIAIOVERSION)
     eDvbFont OldFont = m_Osd->SetFont(m_Font);
#endif
     if (m_DisplayMode == modeTransponder) {
        m_Osd->Fill(0, OSDINFOWIN_Y(0), OSDWIDTH, OSDINFOWIN_Y(OSDINFOHEIGHT), clrBackground, m_InfoWindow);
        m_Osd->Fill(0, OSDINFOWIN_Y(offset), OSDWIDTH, OSDINFOWIN_Y(offset+cOsd::LineHeight()-1), clrWhite, m_InfoWindow);
        m_Osd->Text( OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Transponder Information"), clrBackground, clrWhite, m_InfoWindow);
        offset += cOsd::LineHeight();
        m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Vpid"), clrWhite, clrBackground, m_InfoWindow);
        snprintf(buf, sizeof(buf), "%d", channel->Vpid());
        m_Osd->Text(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
        m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Ppid"), clrWhite, clrBackground, m_InfoWindow);
        snprintf(buf, sizeof(buf), "%d", channel->Ppid());
        m_Osd->Text(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
        offset += cOsd::LineHeight() - 2;
        m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Apid1"), clrWhite, clrBackground, m_InfoWindow);
        value = channel->Apid2();
        if (value) snprintf(buf, sizeof(buf), "%d, %d", channel->Apid1(), value);
        else       snprintf(buf, sizeof(buf), "%d", channel->Apid1());
        m_Osd->Text(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
        m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Dpid1"), clrWhite, clrBackground, m_InfoWindow);
        value = channel->Dpid2();
        if (value) snprintf(buf, sizeof(buf), "%d, %d", channel->Dpid1(), value);
        else       snprintf(buf, sizeof(buf), "%d", channel->Dpid1());
        m_Osd->Text(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
        offset += cOsd::LineHeight() - 2;
        m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("CA"), clrWhite, clrBackground, m_InfoWindow);
        snprintf(buf, sizeof(buf), "%d", channel->Ca());
        m_Osd->Text(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
        m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Tpid"), clrWhite, clrBackground, m_InfoWindow);
        snprintf(buf, sizeof(buf), "%d", channel->Tpid());
        m_Osd->Text(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
        offset += cOsd::LineHeight() - 2;
        m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Sid"), clrWhite, clrBackground, m_InfoWindow);
        snprintf(buf, sizeof(buf), "%d", channel->Sid());
        m_Osd->Text(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
#if (VDRVERSNUM >= 10300)
        m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Nid"), clrWhite, clrBackground, m_InfoWindow);
        snprintf(buf, sizeof(buf), "%d", channel->Nid());
        m_Osd->Text(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
        offset += cOsd::LineHeight() - 2;
        m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Tid"), clrWhite, clrBackground, m_InfoWindow);
        snprintf(buf, sizeof(buf), "%d", channel->Tid());
        m_Osd->Text(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
        m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Rid"), clrWhite, clrBackground, m_InfoWindow);
        snprintf(buf, sizeof(buf), "%d", channel->Rid());
        m_Osd->Text(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
#endif
        offset += cOsd::LineHeight() - 2;
        switch (m_FrontendInfo.type) {
          case FE_QPSK:
               snprintf(buf, sizeof(buf), "%s #%d - %s", tr("Satellite Card"), cDevice::ActualDevice()->CardIndex(), m_FrontendInfo.name);
               m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               offset += cOsd::LineHeight() - 2;
               m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Frequency"), clrWhite, clrBackground, m_InfoWindow);
               value = channel->Frequency();
               while (value > 20000) value /= 1000;
               snprintf(buf, sizeof(buf), "%d %s", value, tr("MHz"));
               m_Osd->Text(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Source"), clrWhite, clrBackground, m_InfoWindow);
               snprintf(buf, sizeof(buf), "%s", cSource::ToString(channel->Source()));
               m_Osd->Text(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               offset += cOsd::LineHeight() - 2;
               m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Srate"), clrWhite, clrBackground, m_InfoWindow);
               snprintf(buf, sizeof(buf), "%d", channel->Srate());
               m_Osd->Text(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Polarization"), clrWhite, clrBackground, m_InfoWindow);
               snprintf(buf, sizeof(buf), "%c", toupper(channel->Polarization()));
               m_Osd->Text(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               offset += cOsd::LineHeight() - 2;
               m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Inversion"), clrWhite, clrBackground, m_InfoWindow);
               value = channel->Inversion();
               if      (value == INVERSION_OFF)   snprintf(buf, sizeof(buf), tr("Off"));
               else if (value == INVERSION_ON)    snprintf(buf, sizeof(buf), tr("On"));
               else            /*INVERSION_AUTO*/ snprintf(buf, sizeof(buf), tr("Auto"));
               m_Osd->Text(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("CoderateH"), clrWhite, clrBackground, m_InfoWindow);
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
               m_Osd->Text(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               break;

          case FE_QAM:
               snprintf(buf, sizeof(buf), "%s #%d - %s", tr("Cable Card"), cDevice::ActualDevice()->CardIndex(), m_FrontendInfo.name);
               m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               offset += cOsd::LineHeight() - 2;
               m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Frequency"), clrWhite, clrBackground, m_InfoWindow);
               value = channel->Frequency();
               while (value > 20000) value /= 1000;
               snprintf(buf, sizeof(buf), "%d %s", value, tr("MHz"));
               m_Osd->Text(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Source"), clrWhite, clrBackground, m_InfoWindow);
               snprintf(buf, sizeof(buf), "%s", cSource::ToString(channel->Source()));
               m_Osd->Text(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               offset += cOsd::LineHeight() - 2;
               m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Srate"), clrWhite, clrBackground, m_InfoWindow);
               snprintf(buf, sizeof(buf), "%d", channel->Srate());
               m_Osd->Text(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Modulation"), clrWhite, clrBackground, m_InfoWindow);
               value = channel->Modulation();
               if      (value == QPSK)      snprintf(buf, sizeof(buf), "QPSK");
               else if (value == QAM_16)    snprintf(buf, sizeof(buf), "QAM 16");
               else if (value == QAM_32)    snprintf(buf, sizeof(buf), "QAM 32");
               else if (value == QAM_64)    snprintf(buf, sizeof(buf), "QAM 64");
               else if (value == QAM_128)   snprintf(buf, sizeof(buf), "QAM 128");
               else if (value == QAM_256)   snprintf(buf, sizeof(buf), "QAM 256");
               else            /*QAM_AUTO*/ snprintf(buf, sizeof(buf), "QAM %s", tr("Auto"));
               m_Osd->Text(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               offset += cOsd::LineHeight() - 2;
               m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Inversion"), clrWhite, clrBackground, m_InfoWindow);
               value = channel->Inversion();
               if      (value == INVERSION_OFF)   snprintf(buf, sizeof(buf), tr("Off"));
               else if (value == INVERSION_ON)    snprintf(buf, sizeof(buf), tr("On"));
               else            /*INVERSION_AUTO*/ snprintf(buf, sizeof(buf), tr("Auto"));
               m_Osd->Text(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("CoderateH"), clrWhite, clrBackground, m_InfoWindow);
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
               m_Osd->Text(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               break;

          default:
               snprintf(buf, sizeof(buf), "%s #%d - %s", tr("Terrestial Card"), cDevice::ActualDevice()->CardIndex(), m_FrontendInfo.name);
               m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               offset += cOsd::LineHeight() - 2;
               m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Frequency"), clrWhite, clrBackground, m_InfoWindow);
               value = channel->Frequency();
               while (value > 20000) value /= 1000;
               snprintf(buf, sizeof(buf), "%d %s", value, tr("MHz"));
               m_Osd->Text(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Transmission"), clrWhite, clrBackground, m_InfoWindow);
               value = channel->Transmission();
               if      (value == TRANSMISSION_MODE_2K)    snprintf(buf, sizeof(buf), "2K");
               else if (value == TRANSMISSION_MODE_8K)    snprintf(buf, sizeof(buf), "8K");
               else            /*TRANSMISSION_MODE_AUTO*/ snprintf(buf, sizeof(buf), tr("Auto"));
               m_Osd->Text(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               offset += cOsd::LineHeight() - 2;
               m_Osd->Text( OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Bandwidth"), clrWhite, clrBackground, m_InfoWindow);
               value = channel->Bandwidth();
               if      (value == BANDWIDTH_8_MHZ) snprintf(buf, sizeof(buf), "8 %s", tr("MHz"));
               else if (value == BANDWIDTH_7_MHZ) snprintf(buf, sizeof(buf), "7 %s", tr("MHz"));
               else if (value == BANDWIDTH_6_MHZ) snprintf(buf, sizeof(buf), "6 %s", tr("MHz"));
               else            /*BANDWIDTH_AUTO*/ snprintf(buf, sizeof(buf), tr("Auto"));
               m_Osd->Text(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Modulation"), clrWhite, clrBackground, m_InfoWindow);
               value = channel->Modulation();
               if      (value == QPSK)      snprintf(buf, sizeof(buf), "QPSK");
               else if (value == QAM_16)    snprintf(buf, sizeof(buf), "QAM 16");
               else if (value == QAM_32)    snprintf(buf, sizeof(buf), "QAM 32");
               else if (value == QAM_64)    snprintf(buf, sizeof(buf), "QAM 64");
               else if (value == QAM_128)   snprintf(buf, sizeof(buf), "QAM 128");
               else if (value == QAM_256)   snprintf(buf, sizeof(buf), "QAM 256");
               else            /*QAM_AUTO*/ snprintf(buf, sizeof(buf), "QAM %s", tr("Auto"));
               m_Osd->Text(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               offset += cOsd::LineHeight() - 2;
               m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Inversion"), clrWhite, clrBackground, m_InfoWindow);
               value = channel->Inversion();
               if      (value == INVERSION_OFF)   snprintf(buf, sizeof(buf), tr("Off"));
               else if (value == INVERSION_ON)    snprintf(buf, sizeof(buf), tr("On"));
               else            /*INVERSION_AUTO*/ snprintf(buf, sizeof(buf), tr("Auto"));
               m_Osd->Text(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("CoderateH"), clrWhite, clrBackground, m_InfoWindow);
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
               m_Osd->Text(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               offset += cOsd::LineHeight() - 2;
               m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Hierarchy"), clrWhite, clrBackground, m_InfoWindow);
               value = channel->Hierarchy();
               if      (value == HIERARCHY_NONE)  snprintf(buf, sizeof(buf), tr("None"));
               else if (value == HIERARCHY_1)     snprintf(buf, sizeof(buf), "1");
               else if (value == HIERARCHY_2)     snprintf(buf, sizeof(buf), "2");
               else if (value == HIERARCHY_4)     snprintf(buf, sizeof(buf), "4");
               else            /*HIERARCHY_AUTO*/ snprintf(buf, sizeof(buf), tr("Auto"));
               m_Osd->Text(OSDINFOWIN_X(2), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), tr("Guard"), clrWhite, clrBackground, m_InfoWindow);
               value = channel->Guard();
               if      (value == GUARD_INTERVAL_1_32)  snprintf(buf, sizeof(buf), "1/32");
               else if (value == GUARD_INTERVAL_1_16)  snprintf(buf, sizeof(buf), "1/16");
               else if (value == GUARD_INTERVAL_1_8)   snprintf(buf, sizeof(buf), "1/8");
               else if (value == GUARD_INTERVAL_1_4)   snprintf(buf, sizeof(buf), "1/4");
               else            /*GUARD_INTERVAL_AUTO*/ snprintf(buf, sizeof(buf), tr("Auto"));
               m_Osd->Text(OSDINFOWIN_X(4), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
               break;
          }
        }
     else if (m_DisplayMode == modeStream) {
        m_Osd->Fill(0, OSDINFOWIN_Y(0), OSDWIDTH, OSDINFOWIN_Y(OSDINFOHEIGHT), clrBackground);
        m_Osd->Fill(0, OSDINFOWIN_Y(offset), OSDWIDTH, OSDINFOWIN_Y(offset+cOsd::LineHeight()-1), clrWhite);
        m_Osd->Text( OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Stream Information"), clrBackground, clrWhite, m_InfoWindow);
        offset += cOsd::LineHeight();
        m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Video Stream"), clrYellow, clrBackground, m_InfoWindow);
        snprintf(buf, sizeof(buf), "#%d", channel->Vpid());
        m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
        offset += cOsd::LineHeight() - 2;
        m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Bitrate"), clrWhite, clrBackground, m_InfoWindow);
        if (m_Receiver) snprintf(buf, sizeof(buf), "%.2f %s (%.2f %s)", m_Receiver->VideoStreamBitrate(), tr("Mbit/s"), m_Receiver->VideoBitrate(), tr("Mbit/s"));
        else            snprintf(buf, sizeof(buf), "---");
        m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
        offset += cOsd::LineHeight() - 2;
        m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Aspect Ratio"), clrWhite, clrBackground, m_InfoWindow);
        if (m_Receiver) {
           value = m_Receiver->VideoAspectRatio();
           if      (value == AR_1_1)    snprintf(buf, sizeof(buf), "1:1");
           else if (value == AR_4_3)    snprintf(buf, sizeof(buf), "4:3");
           else if (value == AR_16_9)   snprintf(buf, sizeof(buf), "16:9");
           else if (value == AR_2_21_1) snprintf(buf, sizeof(buf), "2.21:1");
           else                         snprintf(buf, sizeof(buf), "%s", tr("reserved"));
           }
        else                            snprintf(buf, sizeof(buf), "---");
        m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
        offset += cOsd::LineHeight() - 2;
        m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Frame Rate"), clrWhite, clrBackground, m_InfoWindow);
        if (m_Receiver) snprintf(buf, sizeof(buf), "%.2f %s", m_Receiver->VideoFrameRate(), tr("Hz"));
        else            snprintf(buf, sizeof(buf), "---");
        m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
        offset += cOsd::LineHeight() - 2;
        m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Video Format"), clrWhite, clrBackground, m_InfoWindow);
        if (m_Receiver) {
           value = m_Receiver->VideoFormat();
           if      (value == VF_PAL)  snprintf(buf, sizeof(buf), "%s", tr("PAL"));
           else if (value == VF_NTSC) snprintf(buf, sizeof(buf), "%s", tr("NTSC"));
           else                       snprintf(buf, sizeof(buf), "%s", tr("unknown"));
           }
        else                          snprintf(buf, sizeof(buf), "---");
        m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
        offset += cOsd::LineHeight() - 2;
        m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Resolution"), clrWhite, clrBackground, m_InfoWindow);
        if (m_Receiver) snprintf(buf, sizeof(buf), "%d x %d", m_Receiver->VideoHorizontalSize(), m_Receiver->VideoVerticalSize());
        else            snprintf(buf, sizeof(buf), "---");
        m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
        offset += cOsd::LineHeight() - 2;
        m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Audio Stream"), clrYellow, clrBackground, m_InfoWindow);
        snprintf(buf, sizeof(buf), "#%d", channel->Apid1());
        m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
        offset += cOsd::LineHeight() - 2;
        m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Bitrate"), clrWhite, clrBackground, m_InfoWindow);
        if (m_Receiver) {
           dvalue = m_Receiver->AudioStreamBitrate();
           if      (dvalue == (double)FR_RESERVED) snprintf(buf, sizeof(buf), "%s (%.0f %s)", tr("reserved"), m_Receiver->AudioBitrate(), tr("kbit/s"));
           else if (dvalue == (double)FR_FREE)     snprintf(buf, sizeof(buf), "%s (%.0f %s)", tr("free"), m_Receiver->AudioBitrate(), tr("kbit/s"));
           else                                    snprintf(buf, sizeof(buf), "%.0f %s (%.0f %s)", dvalue, tr("kbit/s"), m_Receiver->AudioBitrate(), tr("kbit/s"));
           }
        else                                       snprintf(buf, sizeof(buf), "---");
        m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
        offset += cOsd::LineHeight() - 2;
        m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("MPEG Layer"), clrWhite, clrBackground, m_InfoWindow);
        if (m_Receiver) snprintf(buf, sizeof(buf), "%d", m_Receiver->AudioMPEGLayer());
        else            snprintf(buf, sizeof(buf), "---");
        m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
        offset += cOsd::LineHeight() - 2;
        m_Osd->Text(OSDINFOWIN_X(1), OSDINFOWIN_Y(offset), tr("Sampling Frequency"), clrWhite, clrBackground, m_InfoWindow);
        if (m_Receiver) {
           value =  m_Receiver->AudioSamplingFreq();
           if (value == FR_RESERVED) snprintf(buf, sizeof(buf), "%s", tr("reserved"));
           else                      snprintf(buf, sizeof(buf), "%.1f %s", (value / 1000.0), tr("kHz"));
           }
        else                         snprintf(buf, sizeof(buf), "---");
        m_Osd->Text(OSDINFOWIN_X(3), OSDINFOWIN_Y(offset), buf, clrYellow, clrBackground, m_InfoWindow);
        }
     else /* modeBasic */ {
        m_Osd->Fill(0, OSDINFOWIN_Y(0), OSDWIDTH, OSDINFOWIN_Y(OSDINFOHEIGHT), clrTransparent, m_InfoWindow);
        }
#if (VDRVERSNUM >= 10300) || defined(ELCHIAIOVERSION)
     m_Osd->SetFont(OldFont);
#endif
     m_Osd->Flush();
     }
}

void cFemonOsd::Action(void)
{
  //printf("cFemonOsd::Action()\n");
#if (VDRVERSNUM < 10300)
	isyslog("femon plugin: thread started (pid = %d)", getpid());
#endif
  m_Active = true;
  while (m_Active) {
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
    usleep(100000L * femonConfig.updateinterval);
    }
#if (VDRVERSNUM < 10300)
	isyslog("femon plugin: thread stopped (pid = %d)", getpid());
#endif
}

void cFemonOsd::Show(void)
{
  //printf("cFemonOsd::Show()\n");
  char *dev = NULL;
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
  m_Osd = cOsd::OpenRaw((SCREENWIDTH - OSDWIDTH) / 2, (SCREENHEIGHT - OSDHEIGHT) / 2);
  if (m_Osd) {
#if (VDRVERSNUM >= 10300) || defined(ELCHIAIOVERSION)
     eDvbFont OldFont = m_Osd->SetFont(m_Font);
#endif
     m_StatusWindow = m_Osd->Create(0, OSDSTATUSWIN_Y(0), OSDWIDTH, OSDSTATUSHEIGHT, 4);
     m_InfoWindow   = m_Osd->Create(0, OSDINFOWIN_Y(0),   OSDWIDTH, OSDINFOHEIGHT,   2);
     m_Osd->Clear();
     m_Osd->Fill(0, OSDINFOWIN_Y(0), OSDWIDTH, OSDINFOWIN_Y(OSDINFOHEIGHT), clrTransparent, m_InfoWindow);
#if (VDRVERSNUM >= 10300) || defined(ELCHIAIOVERSION)
     m_Osd->SetFont(OldFont);
#endif
     m_Osd->Flush();
     if (m_Receiver)
        delete m_Receiver;
     if (femonConfig.analyzestream) {
        int channelNumber = cDevice::CurrentChannel();
        m_Receiver = new cFemonReceiver(Channels.GetByNumber(channelNumber)->Ca(), Channels.GetByNumber(channelNumber)->Vpid(), Channels.GetByNumber(channelNumber)->Apid1());
        cDevice::ActualDevice()->AttachReceiver(m_Receiver);
        }
     Start();
     }
}

void cFemonOsd::ChannelSwitch(const cDevice * device, int channelNumber)
{
  //printf("cFemonOsd::ChannelSwitch()\n");
  char *dev = NULL;
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
     channelNumber = cDevice::CurrentChannel();
     m_Receiver = new cFemonReceiver(Channels.GetByNumber(channelNumber)->Ca(), Channels.GetByNumber(channelNumber)->Vpid(), Channels.GetByNumber(channelNumber)->Apid1());
     cDevice::ActualDevice()->AttachReceiver(m_Receiver);
     }
}

eOSState cFemonOsd::ProcessKey(eKeys Key)
{ 
  eOSState state = cOsdObject::ProcessKey(Key);

  if (state == osUnknown) {
     switch (Key & ~k_Repeat) {
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
                  m_InputTime = time_ms();
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
       case kUp|k_Repeat:
       case kUp:
       case kDown|k_Repeat:
       case kDown:
            m_OldNumber = cDevice::CurrentChannel();
            cDevice::SwitchChannel(NORMALKEY(Key) == kUp ? 1 : -1);
            m_Number = 0;
            break;
       case kNone:
            if (m_Number && (time_ms() - m_InputTime > CHANNELINPUT_TIMEOUT)) {
               if (Channels.GetByNumber(m_Number)) {
                  m_OldNumber = cDevice::CurrentChannel();
                  Channels.SwitchTo(m_Number);
                  m_Number = 0;
                  }
               else {
                  m_InputTime = time_ms();
                  m_Number = 0;
                  }
               }
            break;
       case kOk:
            if (++m_DisplayMode >= modeMaxNumber) m_DisplayMode = 0; // toggle between display modes
            DrawInfoWindow();
            break;
       default:
            break;
       }
     state = osContinue;
  }
  return state;
}
