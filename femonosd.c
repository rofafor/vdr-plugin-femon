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

#define FE_DEVICE            "/dev/dvb/adapter%d/frontend%d"
#define CHANNELINPUT_TIMEOUT 1000

#define OSDWIDTH        46
#define OSDINFOHEIGHT   11
#define OSDGAPHEIGHT    1
#define OSDSTATUSHEIGHT 6

#define OSDSTATUSCOL1   1
#define OSDSTATUSCOL2   10
#define OSDSTATUSCOL3   17
#define OSDSTATUSCOL4   32

#define OSDSTATUSLOCK   1
#define OSDSTATUSSIGN   9
#define OSDSTATUSCARR   19
#define OSDSTATUSVITE   30
#define OSDSTATUSSYNC   40

#define OSDINFOLCOL1    1
#define OSDINFOLCOL2    14
#define OSDINFOLCOL3    23
#define OSDINFOLCOL4    36

cFemonOsd::cFemonOsd(void)
#if VDRVERSNUM >= 10300
:cOsdObject(true), cThread("femon plugin")
#else
:cOsdObject(true)
#endif
{
  //printf("cFemonOsd::cFemonOsd()\n");
  m_Osd = NULL;
  m_InfoWindow = -1;
  m_StatusWindow = -1;
  m_Receiver = NULL;
  m_Frontend = -1;
  m_Active = false;
  m_Number = 0;
  m_InputTime = 0;
  m_Signal = 0;
  m_SNR = 0;
  m_BER = 0;
  m_UNC = 0;
  m_DisplayMode = femonConfig.displaymode;
  m_Mutex = new cMutex();
}

cFemonOsd::~cFemonOsd(void)
{
  //printf("cFemonOsd::~cFemonOsd()\n");
  if (m_Active) {
     m_Active = false;
     Cancel(5);
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
  int lines = 0;

  if (m_Osd) {
#if (VDRVERSNUM >= 10300) || defined(ELCHIAIOVERSION)
     eDvbFont OldFont = m_Osd->SetFont(fontSml);
#endif
     m_Osd->Clear(m_StatusWindow);
     snprintf(buf, sizeof(buf), "%d%s %s", m_Number ? m_Number : Channels.GetByNumber(cDevice::CurrentChannel())->Number(), m_Number ? "-" : "", Channels.GetByNumber(cDevice::CurrentChannel())->Name());
     m_Osd->Fill(0, femonConfig.position ? 0 : (OSDINFOHEIGHT + OSDGAPHEIGHT) * cOsd::LineHeight(), OSDWIDTH * cOsd::CellWidth(), femonConfig.position ? cOsd::LineHeight() - 1 : (OSDINFOHEIGHT + OSDGAPHEIGHT + 1) * cOsd::LineHeight() - 1, clrWhite, m_StatusWindow);
     m_Osd->Text(OSDSTATUSCOL1 * cOsd::CellWidth(), femonConfig.position ? 0 : (OSDINFOHEIGHT + OSDGAPHEIGHT + 0) * cOsd::LineHeight(), buf, clrBlack, clrWhite, m_StatusWindow);
     lines++;
     if (signal > 0) {
        signal = (OSDWIDTH * cOsd::CellWidth()) * signal / 100;
        m_Osd->Fill(0, femonConfig.position ? lines * cOsd::LineHeight() + 3 : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() + 3, min(((OSDWIDTH * cOsd::CellWidth()) * femonConfig.redlimit / 100), signal), femonConfig.position ? (lines + 1) * cOsd::LineHeight() - 3 : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines + 1) * cOsd::LineHeight() - 3, clrRed, m_StatusWindow);
        if (signal > ((OSDWIDTH * cOsd::CellWidth()) * femonConfig.redlimit / 100)) {
           m_Osd->Fill(((OSDWIDTH * cOsd::CellWidth()) * femonConfig.redlimit / 100), femonConfig.position ? lines * cOsd::LineHeight() + 3 : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() + 3, min(((OSDWIDTH * cOsd::CellWidth()) * femonConfig.greenlimit / 100), signal), femonConfig.position ? (lines + 1) * cOsd::LineHeight() - 3 : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines + 1) * cOsd::LineHeight() - 3, clrYellow, m_StatusWindow);
           }
        if (signal > ((OSDWIDTH * cOsd::CellWidth()) * femonConfig.greenlimit / 100)) {
           m_Osd->Fill(((OSDWIDTH * cOsd::CellWidth()) * femonConfig.greenlimit / 100), femonConfig.position ? lines * cOsd::LineHeight() + 3 : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() + 3, signal, femonConfig.position ? (lines + 1) * cOsd::LineHeight() - 3 : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines + 1) * cOsd::LineHeight() - 3, clrGreen, m_StatusWindow);
           }
        }
     lines++;
     if (snr > 0) {
        snr = (OSDWIDTH * cOsd::CellWidth()) * snr / 100;
        m_Osd->Fill(0, femonConfig.position ? lines * cOsd::LineHeight() + 3 : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() + 3, min(((OSDWIDTH * cOsd::CellWidth()) * femonConfig.redlimit / 100), snr), femonConfig.position ? (lines + 1) * cOsd::LineHeight() - 3 : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines + 1) * cOsd::LineHeight() - 3, clrRed, m_StatusWindow);
        if (snr > ((OSDWIDTH * cOsd::CellWidth()) * femonConfig.redlimit / 100)) {
           m_Osd->Fill(((OSDWIDTH * cOsd::CellWidth()) * femonConfig.redlimit / 100), femonConfig.position ? lines * cOsd::LineHeight() + 3 : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() + 3, min(((OSDWIDTH * cOsd::CellWidth()) * femonConfig.greenlimit / 100), snr), femonConfig.position ? (lines + 1) * cOsd::LineHeight() - 3 : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines + 1) * cOsd::LineHeight() - 3, clrYellow, m_StatusWindow);
           }
        if (snr > ((OSDWIDTH * cOsd::CellWidth()) * femonConfig.greenlimit / 100)) {
           m_Osd->Fill(((OSDWIDTH * cOsd::CellWidth()) * femonConfig.greenlimit / 100),  femonConfig.position ? lines * cOsd::LineHeight() + 3 : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() + 3, snr, femonConfig.position ? (lines + 1) * cOsd::LineHeight() - 3 : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines + 1) * cOsd::LineHeight() - 3, clrGreen, m_StatusWindow);
           }
        }
     lines++;
     snprintf(buf, sizeof(buf), "STR: %04x", m_Signal);
     m_Osd->Text(OSDSTATUSCOL1 * cOsd::CellWidth(), femonConfig.position ? lines * cOsd::LineHeight() : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight(), buf, clrWhite, clrBackground, m_StatusWindow);
     snprintf(buf, sizeof(buf), "(%2d%%)", m_Signal / 655);
     m_Osd->Text(OSDSTATUSCOL2 * cOsd::CellWidth(), femonConfig.position ? lines * cOsd::LineHeight() : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight(), buf, clrWhite /*signal < femonConfig.redlimit ? clrRed : signal < femonConfig.greenlimit ? clrYellow : clrGreen*/, clrBackground, m_StatusWindow);
     snprintf(buf, sizeof(buf), "BER: %08x", m_BER);
     m_Osd->Text(OSDSTATUSCOL3 * cOsd::CellWidth(), femonConfig.position ? lines * cOsd::LineHeight() : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight(), buf, clrWhite, clrBackground, m_StatusWindow);
     if (m_Receiver) snprintf(buf, sizeof(buf), "Video: %.2f Mbit/s",m_Receiver->VideoBitrate());
     else            snprintf(buf, sizeof(buf), "Video: --- Mbit/s");
     m_Osd->Text(OSDSTATUSCOL4 * cOsd::CellWidth(), femonConfig.position ? lines * cOsd::LineHeight() : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight(), buf, clrWhite, clrBackground, m_StatusWindow);
     lines++;
     snprintf(buf, sizeof(buf), "SNR: %04x", m_SNR);
     m_Osd->Text(OSDSTATUSCOL1 * cOsd::CellWidth(), femonConfig.position ? lines * cOsd::LineHeight() : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight(), buf, clrWhite, clrBackground, m_StatusWindow);
     snprintf(buf, sizeof(buf), "(%2d%%)", m_SNR / 655);
     m_Osd->Text(OSDSTATUSCOL2 * cOsd::CellWidth(), femonConfig.position ? lines * cOsd::LineHeight() : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight(), buf, clrWhite /*snr < femonConfig.redlimit ? clrRed : snr < femonConfig.greenlimit ? clrYellow : clrGreen*/, clrBackground, m_StatusWindow);
     snprintf(buf, sizeof(buf), "UNC: %08x", m_UNC);
     m_Osd->Text(OSDSTATUSCOL3 * cOsd::CellWidth(), femonConfig.position ? lines * cOsd::LineHeight() : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight(), buf, clrWhite, clrBackground, m_StatusWindow);
     if (m_Receiver) snprintf(buf, sizeof(buf), "Audio: %.0f kbit/s",m_Receiver->AudioBitrate());
     else            snprintf(buf, sizeof(buf), "Audio: --- kbit/s");
     m_Osd->Text(OSDSTATUSCOL4 * cOsd::CellWidth(), femonConfig.position ? lines * cOsd::LineHeight() : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight(), buf, clrWhite, clrBackground, m_StatusWindow);
     lines++;
     m_Osd->Text(OSDSTATUSLOCK * cOsd::CellWidth(), femonConfig.position ? lines * cOsd::LineHeight() : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight(), "LOCK",   (m_FrontendStatus & FE_HAS_LOCK)   ? clrYellow : clrBlack, clrBackground, m_StatusWindow);
     m_Osd->Text(OSDSTATUSSIGN * cOsd::CellWidth(), femonConfig.position ? lines * cOsd::LineHeight() : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight(), "SIGNAL", (m_FrontendStatus & FE_HAS_SIGNAL) ? clrYellow : clrBlack, clrBackground, m_StatusWindow);
     m_Osd->Text(OSDSTATUSCARR * cOsd::CellWidth(), femonConfig.position ? lines * cOsd::LineHeight() : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight(), "CARRIER",(m_FrontendStatus & FE_HAS_CARRIER)? clrYellow : clrBlack, clrBackground, m_StatusWindow);
     m_Osd->Text(OSDSTATUSVITE * cOsd::CellWidth(), femonConfig.position ? lines * cOsd::LineHeight() : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight(), "VITERBI",(m_FrontendStatus & FE_HAS_VITERBI)? clrYellow : clrBlack, clrBackground, m_StatusWindow);
     m_Osd->Text(OSDSTATUSSYNC * cOsd::CellWidth(), femonConfig.position ? lines * cOsd::LineHeight() : (OSDINFOHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight(), "SYNC",   (m_FrontendStatus & FE_HAS_SYNC)   ? clrYellow : clrBlack, clrBackground, m_StatusWindow);
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
  int lines = 0;
  int value = 0;

  if (m_Osd) {
     if (m_DisplayMode) {
#if (VDRVERSNUM >= 10300) || defined(ELCHIAIOVERSION)
        eDvbFont OldFont = m_Osd->SetFont(fontSml);
#endif
        m_Osd->Clear(m_InfoWindow);
        m_Osd->Fill(0, femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), OSDWIDTH * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines + 1) * cOsd::LineHeight() - 1 : (lines + 1) * cOsd::LineHeight() - 1, clrWhite, m_InfoWindow);
        m_Osd->Text( OSDINFOLCOL1 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines, tr("Transponder Information"), clrBackground, clrWhite, m_InfoWindow);
        lines++;
        m_Osd->Text(OSDINFOLCOL1 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("Vpid"), clrWhite, clrBackground, m_InfoWindow);
        snprintf(buf, sizeof(buf), "%d", Channels.GetByNumber(cDevice::CurrentChannel())->Vpid());
        m_Osd->Text(OSDINFOLCOL2 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
        m_Osd->Text(OSDINFOLCOL3 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("Ppid"), clrWhite, clrBackground, m_InfoWindow);
        snprintf(buf, sizeof(buf), "%d", Channels.GetByNumber(cDevice::CurrentChannel())->Ppid());
        m_Osd->Text(OSDINFOLCOL4 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
        lines++;
        m_Osd->Text(OSDINFOLCOL1 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("Apid1"), clrWhite, clrBackground, m_InfoWindow);
        snprintf(buf, sizeof(buf), "%d", Channels.GetByNumber(cDevice::CurrentChannel())->Apid1());
        m_Osd->Text(OSDINFOLCOL2 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
        m_Osd->Text(OSDINFOLCOL3 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("Tpid"), clrWhite, clrBackground, m_InfoWindow);
        snprintf(buf, sizeof(buf), "%d", Channels.GetByNumber(cDevice::CurrentChannel())->Tpid());
        m_Osd->Text(OSDINFOLCOL4 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
        lines++;
        m_Osd->Text(OSDINFOLCOL1 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("CA"), clrWhite, clrBackground, m_InfoWindow);
        snprintf(buf, sizeof(buf), "%d", Channels.GetByNumber(cDevice::CurrentChannel())->Ca());
        m_Osd->Text(OSDINFOLCOL2 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
        m_Osd->Text(OSDINFOLCOL3 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("Sid"), clrWhite, clrBackground, m_InfoWindow);
        snprintf(buf, sizeof(buf), "%d", Channels.GetByNumber(cDevice::CurrentChannel())->Sid());
        m_Osd->Text(OSDINFOLCOL4 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
        lines++;
#if (VDRVERSNUM >= 10300)
        m_Osd->Text(OSDINFOLCOL1 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), "Nid", clrWhite, clrBackground, m_InfoWindow);
        snprintf(buf, sizeof(buf), "%d", Channels.GetByNumber(cDevice::CurrentChannel())->Nid());
        m_Osd->Text(OSDINFOLCOL2 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
        m_Osd->Text(OSDINFOLCOL3 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), "Tid" /*tr("Tid")*/, clrWhite, clrBackground, m_InfoWindow);
        snprintf(buf, sizeof(buf), "%d", Channels.GetByNumber(cDevice::CurrentChannel())->Tid());
        m_Osd->Text(OSDINFOLCOL4 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
        lines++;
        m_Osd->Text(OSDINFOLCOL1 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), "Rid" /*tr("Rid")*/, clrWhite, clrBackground, m_InfoWindow);
        snprintf(buf, sizeof(buf), "%d", Channels.GetByNumber(cDevice::CurrentChannel())->Rid());
        m_Osd->Text(OSDINFOLCOL2 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
        lines++;
#endif
        switch (m_FrontendInfo.type) {
          case FE_QPSK:
               snprintf(buf, sizeof(buf), "%s #%d - %s", tr("Satellite Card"), cDevice::ActualDevice()->CardIndex(), m_FrontendInfo.name);
               m_Osd->Text(OSDINFOLCOL1 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               lines++;
               m_Osd->Text(OSDINFOLCOL1 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("Frequency"), clrWhite, clrBackground, m_InfoWindow);
               snprintf(buf, sizeof(buf), "%d kHz", Channels.GetByNumber(cDevice::CurrentChannel())->Frequency());
               m_Osd->Text(OSDINFOLCOL2 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               m_Osd->Text(OSDINFOLCOL3 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("Source"), clrWhite, clrBackground, m_InfoWindow);
               snprintf(buf, sizeof(buf), "%s", cSource::ToString(Channels.GetByNumber(cDevice::CurrentChannel())->Source()));
               m_Osd->Text(OSDINFOLCOL4 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               lines++;
               m_Osd->Text(OSDINFOLCOL1 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("Srate"), clrWhite, clrBackground, m_InfoWindow);
               snprintf(buf, sizeof(buf), "%d", Channels.GetByNumber(cDevice::CurrentChannel())->Srate());
               m_Osd->Text(OSDINFOLCOL2 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               m_Osd->Text(OSDINFOLCOL3 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("Polarization"), clrWhite, clrBackground, m_InfoWindow);
               snprintf(buf, sizeof(buf), "%c", toupper(Channels.GetByNumber(cDevice::CurrentChannel())->Polarization()));
               m_Osd->Text(OSDINFOLCOL4 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               lines++;
               m_Osd->Text(OSDINFOLCOL1 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("Inversion"), clrWhite, clrBackground, m_InfoWindow);
               value = Channels.GetByNumber(cDevice::CurrentChannel())->Inversion();
               if      (value == 0) snprintf(buf, sizeof(buf), "Off");
               else if (value == 1) snprintf(buf, sizeof(buf), "On");
               else                 snprintf(buf, sizeof(buf), "Auto");
               m_Osd->Text(OSDINFOLCOL2 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               m_Osd->Text(OSDINFOLCOL3 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("CoderateH"), clrWhite, clrBackground, m_InfoWindow);
               value = Channels.GetByNumber(cDevice::CurrentChannel())->CoderateH();
               if      (value == 0) snprintf(buf, sizeof(buf), "None");
               else if (value == 1) snprintf(buf, sizeof(buf), "1/2");
               else if (value == 2) snprintf(buf, sizeof(buf), "2/3");
               else if (value == 3) snprintf(buf, sizeof(buf), "3/4");
               else if (value == 4) snprintf(buf, sizeof(buf), "4/5");
               else if (value == 5) snprintf(buf, sizeof(buf), "5/6");
               else if (value == 6) snprintf(buf, sizeof(buf), "6/7");
               else if (value == 7) snprintf(buf, sizeof(buf), "7/8");
               else if (value == 8) snprintf(buf, sizeof(buf), "8/9");
               else                 snprintf(buf, sizeof(buf), "Auto");
               m_Osd->Text(OSDINFOLCOL4 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               break;

          case FE_QAM:
               snprintf(buf, sizeof(buf), "%s #%d - %s", tr("Cable Card"), cDevice::ActualDevice()->CardIndex(), m_FrontendInfo.name);
               m_Osd->Text(OSDINFOLCOL1 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               lines++;
               m_Osd->Text(OSDINFOLCOL1 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("Frequency"), clrWhite, clrBackground, m_InfoWindow);
               snprintf(buf, sizeof(buf), "%d kHz", Channels.GetByNumber(cDevice::CurrentChannel())->Frequency());
               m_Osd->Text(OSDINFOLCOL2 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               m_Osd->Text(OSDINFOLCOL3 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("Source"), clrWhite, clrBackground, m_InfoWindow);
               snprintf(buf, sizeof(buf), "%s", cSource::ToString(Channels.GetByNumber(cDevice::CurrentChannel())->Source()));
               m_Osd->Text(OSDINFOLCOL4 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               lines++;
               m_Osd->Text(OSDINFOLCOL1 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("Srate"), clrWhite, clrBackground, m_InfoWindow);
               snprintf(buf, sizeof(buf), "%d", Channels.GetByNumber(cDevice::CurrentChannel())->Srate());
               m_Osd->Text(OSDINFOLCOL2 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               m_Osd->Text(OSDINFOLCOL3 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("Modulation"), clrWhite, clrBackground, m_InfoWindow);
               value = Channels.GetByNumber(cDevice::CurrentChannel())->Modulation();
               if      (value == 0) snprintf(buf, sizeof(buf), "QPSK");
               else if (value == 1) snprintf(buf, sizeof(buf), "QAM 16");
               else if (value == 2) snprintf(buf, sizeof(buf), "QAM 32");
               else if (value == 3) snprintf(buf, sizeof(buf), "QAM 64");
               else if (value == 4) snprintf(buf, sizeof(buf), "QAM 128");
               else if (value == 5) snprintf(buf, sizeof(buf), "QAM 256");
               else                 snprintf(buf, sizeof(buf), "QAM Auto");
               m_Osd->Text(OSDINFOLCOL4 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               lines++;
               m_Osd->Text(OSDINFOLCOL1 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("Inversion"), clrWhite, clrBackground, m_InfoWindow);
               value = Channels.GetByNumber(cDevice::CurrentChannel())->Inversion();
               if      (value == 0) snprintf(buf, sizeof(buf), "Off");
               else if (value == 1) snprintf(buf, sizeof(buf), "On");
               else                 snprintf(buf, sizeof(buf), "Auto");
               m_Osd->Text(OSDINFOLCOL2 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               m_Osd->Text(OSDINFOLCOL3 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("CoderateH"), clrWhite, clrBackground, m_InfoWindow);
               value = Channels.GetByNumber(cDevice::CurrentChannel())->CoderateH();
               if      (value == 0) snprintf(buf, sizeof(buf), "None");
               else if (value == 1) snprintf(buf, sizeof(buf), "1/2");
               else if (value == 2) snprintf(buf, sizeof(buf), "2/3");
               else if (value == 3) snprintf(buf, sizeof(buf), "3/4");
               else if (value == 4) snprintf(buf, sizeof(buf), "4/5");
               else if (value == 5) snprintf(buf, sizeof(buf), "5/6");
               else if (value == 6) snprintf(buf, sizeof(buf), "6/7");
               else if (value == 7) snprintf(buf, sizeof(buf), "7/8");
               else if (value == 8) snprintf(buf, sizeof(buf), "8/9");
               else                 snprintf(buf, sizeof(buf), "Auto");
               m_Osd->Text(OSDINFOLCOL4 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               break;

          default:
               snprintf(buf, sizeof(buf), "%s #%d - %s", tr("Terrestial Card"), cDevice::ActualDevice()->CardIndex(), m_FrontendInfo.name);
               m_Osd->Text(OSDINFOLCOL1 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               lines++;
               m_Osd->Text(OSDINFOLCOL1 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("Frequency"), clrWhite, clrBackground, m_InfoWindow);
               snprintf(buf, sizeof(buf), "%d kHz", Channels.GetByNumber(cDevice::CurrentChannel())->Frequency() / 1000);
               m_Osd->Text(OSDINFOLCOL2 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               m_Osd->Text(OSDINFOLCOL3 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("Transmission"), clrWhite, clrBackground, m_InfoWindow);
               value = Channels.GetByNumber(cDevice::CurrentChannel())->Transmission();
               if      (value == 0) snprintf(buf, sizeof(buf), "2K");
               else if (value == 1) snprintf(buf, sizeof(buf), "8K");
               else                 snprintf(buf, sizeof(buf), "Auto");
               m_Osd->Text(OSDINFOLCOL4 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               lines++;
               m_Osd->Text( OSDINFOLCOL1 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("Bandwidth"), clrWhite, clrBackground, m_InfoWindow);
               value = Channels.GetByNumber(cDevice::CurrentChannel())->Bandwidth();
               if      (value == 0) snprintf(buf, sizeof(buf), "6 MHz");
               else if (value == 1) snprintf(buf, sizeof(buf), "7 MHz");
               else if (value == 2) snprintf(buf, sizeof(buf), "8 MHz");
               else                 snprintf(buf, sizeof(buf), "Auto");
               m_Osd->Text(OSDINFOLCOL2 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               m_Osd->Text(OSDINFOLCOL3 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("Modulation"), clrWhite, clrBackground, m_InfoWindow);
               value = Channels.GetByNumber(cDevice::CurrentChannel())->Modulation();
               if      (value == 0) snprintf(buf, sizeof(buf), "QPSK");
               else if (value == 1) snprintf(buf, sizeof(buf), "QAM 16");
               else if (value == 2) snprintf(buf, sizeof(buf), "QAM 32");
               else if (value == 3) snprintf(buf, sizeof(buf), "QAM 64");
               else if (value == 4) snprintf(buf, sizeof(buf), "QAM 128");
               else if (value == 5) snprintf(buf, sizeof(buf), "QAM 256");
               else                 snprintf(buf, sizeof(buf), "QAM Auto");
               m_Osd->Text(OSDINFOLCOL4 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               lines++;
               m_Osd->Text(OSDINFOLCOL1 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("Inversion"), clrWhite, clrBackground, m_InfoWindow);
               value = Channels.GetByNumber(cDevice::CurrentChannel())->Inversion();
               if      (value == 0) snprintf(buf, sizeof(buf), "Off");
               else if (value == 1) snprintf(buf, sizeof(buf), "On");
               else                 snprintf(buf, sizeof(buf), "Auto");
               m_Osd->Text(OSDINFOLCOL2 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               m_Osd->Text(OSDINFOLCOL3 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("CoderateH"), clrWhite, clrBackground, m_InfoWindow);
               value = Channels.GetByNumber(cDevice::CurrentChannel())->CoderateH();
               if      (value == 0) snprintf(buf, sizeof(buf), "None");
               else if (value == 1) snprintf(buf, sizeof(buf), "1/2");
               else if (value == 2) snprintf(buf, sizeof(buf), "2/3");
               else if (value == 3) snprintf(buf, sizeof(buf), "3/4");
               else if (value == 4) snprintf(buf, sizeof(buf), "4/5");
               else if (value == 5) snprintf(buf, sizeof(buf), "5/6");
               else if (value == 6) snprintf(buf, sizeof(buf), "6/7");
               else if (value == 7) snprintf(buf, sizeof(buf), "7/8");
               else if (value == 8) snprintf(buf, sizeof(buf), "8/9");
               else                 snprintf(buf, sizeof(buf), "Auto");
               value = Channels.GetByNumber(cDevice::CurrentChannel())->CoderateL();
               if      (value == 0) snprintf(buf2, sizeof(buf2), " - None");
               else if (value == 1) snprintf(buf2, sizeof(buf2), " - 1/2");
               else if (value == 2) snprintf(buf2, sizeof(buf2), " - 2/3");
               else if (value == 3) snprintf(buf2, sizeof(buf2), " - 3/4");
               else if (value == 4) snprintf(buf2, sizeof(buf2), " - 4/5");
               else if (value == 5) snprintf(buf2, sizeof(buf2), " - 5/6");
               else if (value == 6) snprintf(buf2, sizeof(buf2), " - 6/7");
               else if (value == 7) snprintf(buf2, sizeof(buf2), " - 7/8");
               else if (value == 8) snprintf(buf2, sizeof(buf2), " - 8/9");
               else                 snprintf(buf2, sizeof(buf2), " - Auto");
               strncat(buf, buf2, sizeof(buf));
               m_Osd->Text(OSDINFOLCOL4 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               lines++;
               m_Osd->Text(OSDINFOLCOL1 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("Hierarchy"), clrWhite, clrBackground, m_InfoWindow);
               value = Channels.GetByNumber(cDevice::CurrentChannel())->Hierarchy();
               if      (value == 0) snprintf(buf, sizeof(buf), "None");
               else if (value == 1) snprintf(buf, sizeof(buf), "1");
               else if (value == 2) snprintf(buf, sizeof(buf), "2");
               else                 snprintf(buf, sizeof(buf), "Auto");
               m_Osd->Text(OSDINFOLCOL2 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               m_Osd->Text(OSDINFOLCOL3 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), tr("Guard"), clrWhite, clrBackground, m_InfoWindow);
               value = Channels.GetByNumber(cDevice::CurrentChannel())->Guard();
               if      (value == 0) snprintf(buf, sizeof(buf), "1/4");
               else if (value == 1) snprintf(buf, sizeof(buf), "1/8");
               else if (value == 2) snprintf(buf, sizeof(buf), "1/16");
               else if (value == 3) snprintf(buf, sizeof(buf), "1/32");
               else                 snprintf(buf, sizeof(buf), "Auto");
               m_Osd->Text(OSDINFOLCOL4 * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT + lines) * cOsd::LineHeight() : lines * cOsd::LineHeight(), buf, clrYellow, clrBackground, m_InfoWindow);
               break;
          }
#if (VDRVERSNUM >= 10300) || defined(ELCHIAIOVERSION)
        m_Osd->SetFont(OldFont);
#endif
        }
     else {
        m_Osd->Fill(0, femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT) * cOsd::LineHeight() : 0, OSDWIDTH * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDINFOHEIGHT + OSDGAPHEIGHT) * cOsd::LineHeight() : OSDINFOHEIGHT * cOsd::LineHeight(), clrTransparent, m_InfoWindow);
        }
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
  asprintf(&dev, FE_DEVICE, cDevice::ActualDevice()->CardIndex(), 0);
  m_Frontend = open(dev, O_RDONLY | O_NONBLOCK);
  free(dev);
  if (m_Frontend < 0) {
     isyslog("cFemonOsd::Show() cannot open frontend device.");
     m_Frontend = -1;
     return;
     }
  if (ioctl(m_Frontend, FE_GET_INFO, &m_FrontendInfo) < 0) {
     isyslog("cFemonOsd::Show() cannot read frontend info.");
     m_Frontend = -1;
     close(m_Frontend);
     return;
     }
  m_Osd = cOsd::OpenRaw((720 - OSDWIDTH * cOsd::CellWidth()) / 2, (576 - (OSDSTATUSHEIGHT + OSDINFOHEIGHT + OSDGAPHEIGHT) * cOsd::LineHeight()) / 2);
  if (m_Osd) {
     m_StatusWindow = m_Osd->Create(0, femonConfig.position ? 0 : (OSDINFOHEIGHT + OSDGAPHEIGHT) * cOsd::LineHeight(), OSDWIDTH * cOsd::CellWidth(), OSDSTATUSHEIGHT * cOsd::LineHeight(), 4);
     m_InfoWindow = m_Osd->Create(0, femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT) * cOsd::LineHeight() : 0, OSDWIDTH * cOsd::CellWidth(), OSDINFOHEIGHT * cOsd::LineHeight(), 2);
     m_Osd->Clear();
     m_Osd->Fill(0, femonConfig.position ? (OSDSTATUSHEIGHT + OSDGAPHEIGHT) * cOsd::LineHeight() : 0, OSDWIDTH * cOsd::CellWidth(), femonConfig.position ? (OSDSTATUSHEIGHT + OSDINFOHEIGHT + OSDGAPHEIGHT) * cOsd::LineHeight() : OSDINFOHEIGHT * cOsd::LineHeight(), clrTransparent, m_InfoWindow);
     m_Osd->Flush();
     if (m_Receiver)
        delete m_Receiver;
     if (femonConfig.showbitrate) {
        m_Receiver = new cFemonReceiver(Channels.GetByNumber(cDevice::CurrentChannel())->Ca(), Channels.GetByNumber(cDevice::CurrentChannel())->Vpid(), Channels.GetByNumber(cDevice::CurrentChannel())->Apid1());
        cDevice::ActualDevice()->AttachReceiver(m_Receiver);
        }
     Start();
     }
}

void cFemonOsd::ChannelSwitch(const cDevice * device, int channelNumber)
{
  //printf("cFemonOsd::ChannelSwitch()\n");
  char *dev = NULL;
  close(m_Frontend);
  asprintf(&dev, FE_DEVICE, cDevice::ActualDevice()->CardIndex(), 0);
  m_Frontend = open(dev, O_RDONLY | O_NONBLOCK);
  free(dev);
  if (m_Frontend < 0) {
     isyslog("cFemonOsd::ChannelSwitch() cannot open frontend device.");
     m_Frontend = -1;
     return;
     }
  if (ioctl(m_Frontend, FE_GET_INFO, &m_FrontendInfo) < 0) {
     isyslog("cFemonOsd::ChannelSwitch() cannot read frontend info.");
     m_Frontend = -1;
     close(m_Frontend);
     return;
     }
  if (m_Receiver)
     delete m_Receiver;
  if (femonConfig.showbitrate) {
     m_Receiver = new cFemonReceiver(Channels.GetByNumber(cDevice::CurrentChannel())->Ca(), Channels.GetByNumber(cDevice::CurrentChannel())->Vpid(), Channels.GetByNumber(cDevice::CurrentChannel())->Apid1());
     cDevice::ActualDevice()->AttachReceiver(m_Receiver);
     }
}

eOSState cFemonOsd::ProcessKey(eKeys Key)
{ 
  eOSState state = cOsdObject::ProcessKey(Key);

  if (state == osUnknown) {
     switch (Key & ~k_Repeat) {
       case k0:
            if (m_Number == 0) {
               // keep the "Toggle channels" function working - however it isn't working now :)
               cRemote::Put(Key);
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
                     Channels.SwitchTo(m_Number);
                     m_Number = 0;
                     }
                  }
               }
            break;
       case kBack:
            return osEnd;
       case kUp:
       case kDown:
            cDevice::SwitchChannel(NORMALKEY(Key) == kUp ? 1 : -1);
            break;
       case kNone:
            if (m_Number && (time_ms() - m_InputTime > CHANNELINPUT_TIMEOUT)) {
               if (Channels.GetByNumber(m_Number)) {
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
            m_DisplayMode ^= 1; // toggle between advanced and simple display mode
            DrawInfoWindow();
            break;
       default:
            break;
       }
     state = osContinue;
  }
  return state;
}
