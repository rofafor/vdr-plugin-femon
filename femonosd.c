/*
 * A Frontend Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#include "femoncfg.h"
#include "femonreceiver.h"
#include "femonosd.h"

#define FE_DEVICE  "/dev/dvb/adapter%d/frontend%d"
#define CHANNELINPUT_TIMEOUT 1000
#define CHANNELINFO_TIMEOUT  5000
#define OSDHEIGHT 5

cFemonOsd::cFemonOsd(void)
#if VDRVERSNUM >= 10300
:cOsdObject(true), cThread("femon plugin")
#else
:cOsdObject(true), cThread()
#endif
{
  //printf("cFemonOsd::cFemonOsd()\n");
  m_Osd = NULL;
  m_Window = -1;
  m_Receiver = NULL;
  m_Frontend = -1;
  m_Active = false;
  m_Number = 0;
  m_InputTime = 0;
  m_InfoTime = 0;
  m_Width = Setup.OSDwidth * cOsd::CellWidth();
  m_Height = OSDHEIGHT * cOsd::LineHeight();
  m_Xpos = (720 - m_Width) / 2;
  m_Ypos = (576 - Setup.OSDheight * cOsd::LineHeight()) / 2 + ( femonConfig.position ? 0 : (Setup.OSDheight - OSDHEIGHT) * cOsd::LineHeight());
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

void cFemonOsd::Action(void)
{
  //printf("cFemonOsd::Action()\n");
  int snr_o, signal_o;
  uint16_t snr, signal;
  uint32_t ber, unc;
  fe_status_t fe_status;
  char buf[128];
  double VRate = 0.0;
  double ARate = 0.0;

#if (VDRVERSNUM < 10300)
	isyslog("femon plugin: thread started (pid = %d)", getpid());
#endif
  m_Active = true;
  while (m_Active) {
    if (m_Frontend != -1) {
       CHECK(ioctl(m_Frontend, FE_READ_STATUS, &fe_status));
       CHECK(ioctl(m_Frontend, FE_READ_SIGNAL_STRENGTH, &signal));
       CHECK(ioctl(m_Frontend, FE_READ_SNR, &snr));
       CHECK(ioctl(m_Frontend, FE_READ_BER, &ber));
       CHECK(ioctl(m_Frontend, FE_READ_UNCORRECTED_BLOCKS, &unc));
       if (m_Osd) {
          m_Osd->Clear(m_Window);
#if (VDRVERSNUM >= 10300) || defined(ELCHIAIOVERSION)
          eDvbFont OldFont = m_Osd->SetFont(fontSml);
#endif
          sprintf(buf, "%d%s%s", m_Number ? m_Number : cDevice::CurrentChannel(), m_Number ? "- " : " ", Channels.GetByNumber(cDevice::CurrentChannel())->Name());
          m_Osd->Fill(0, 0, m_Width, cOsd::LineHeight() - 1, clrWhite, m_Window);
          m_Osd->Text(cOsd::CellWidth(), 0, buf, clrBlack, clrWhite, m_Window);
          if (m_Receiver) {
             // do some averaging to smooth the value
             VRate = (VRate + (8.0 * TS_SIZE * m_Receiver->VideoPacketCount()) / (femonConfig.interval * 102.4 * 1024.0)) / 2.0;
             ARate = (ARate + (8.0 * TS_SIZE * m_Receiver->AudioPacketCount()) / (femonConfig.interval * 102.4)) / 2.0;
             sprintf(buf, "V: %.2f Mbit/s", VRate);
             m_Osd->Text((m_Width - 22 * cOsd::CellWidth()), 0, buf, clrBlack, clrWhite, m_Window);
             sprintf(buf, "A: %.0f kbit/s", ARate);
             m_Osd->Text((m_Width - 10 * cOsd::CellWidth()), 0, buf, clrBlack, clrWhite, m_Window);
             }
          sprintf(buf, "STR: %04x", signal);
          m_Osd->Text(cOsd::CellWidth(), 3 * cOsd::LineHeight(), buf, clrWhite, clrBackground, m_Window);
          sprintf(buf, "SNR: %04x", snr);
          m_Osd->Text(11 * cOsd::CellWidth(), 3 * cOsd::LineHeight(), buf, clrWhite, clrBackground, m_Window);
          sprintf(buf, "BER: %08x", ber);
          m_Osd->Text(21 * cOsd::CellWidth(), 3 * cOsd::LineHeight(), buf, clrWhite, clrBackground, m_Window);
          sprintf(buf, "UNC: %08x", unc);
          m_Osd->Text(35 * cOsd::CellWidth(), 3 * cOsd::LineHeight(), buf, clrWhite, clrBackground, m_Window);
          signal_o = signal / 655;
          sprintf(buf, "STR: %2d%%", signal_o);
          if (signal_o > 0) {
             signal_o = (m_Width - 8 * cOsd::CellWidth()) * signal_o / 100;
             m_Osd->Fill(0, cOsd::LineHeight() + 3, min(((m_Width - 8 * cOsd::CellWidth()) * femonConfig.redlimit / 100), signal_o), 2 * cOsd::LineHeight() - 3, clrRed, m_Window);
             if (signal_o > ((m_Width - 8 * cOsd::CellWidth()) * femonConfig.redlimit / 100)) {
                m_Osd->Fill(((m_Width - 8 * cOsd::CellWidth()) * femonConfig.redlimit / 100), cOsd::LineHeight() + 3, min(((m_Width - 8 * cOsd::CellWidth()) * femonConfig.greenlimit / 100), signal_o), 2 * cOsd::LineHeight() - 3, clrYellow, m_Window);
                }
             if (signal_o > ((m_Width - 8 * cOsd::CellWidth()) * femonConfig.greenlimit / 100)) {
                m_Osd->Fill(((m_Width - 8 * cOsd::CellWidth()) * femonConfig.greenlimit / 100), cOsd::LineHeight() + 3, signal_o, 2 * cOsd::LineHeight() - 3, clrGreen, m_Window);
                }
             m_Osd->Text(m_Width - 8 * cOsd::CellWidth(), cOsd::LineHeight(), buf, clrWhite, clrBackground, m_Window);
             }
          snr_o = snr / 655;
          sprintf(buf, "SNR: %2d%%", snr_o);
          if (snr_o > 0) {
             snr_o = (m_Width - 8 * cOsd::CellWidth()) * snr_o / 100;
             m_Osd->Fill(0, 2 * cOsd::LineHeight() + 3, min(((m_Width - 8 * cOsd::CellWidth()) * femonConfig.redlimit / 100), snr_o), 3 * cOsd::LineHeight() - 3, clrRed, m_Window);
             if (snr_o > ((m_Width - 8 * cOsd::CellWidth()) * femonConfig.redlimit / 100)) {
                m_Osd->Fill(((m_Width - 8 * cOsd::CellWidth()) * femonConfig.redlimit / 100), 2 * cOsd::LineHeight() + 3, min(((m_Width - 8 * cOsd::CellWidth()) * femonConfig.greenlimit / 100), snr_o), 3 * cOsd::LineHeight() - 3, clrYellow, m_Window);
                }
             if (snr_o > ((m_Width - 8 * cOsd::CellWidth()) * femonConfig.greenlimit / 100)) {
                m_Osd->Fill(((m_Width - 8 * cOsd::CellWidth()) * femonConfig.greenlimit / 100), 2 * cOsd::LineHeight() + 3, snr_o, 3 * cOsd::LineHeight() - 3, clrGreen, m_Window);
                }
             m_Osd->Text(m_Width - 8 * cOsd::CellWidth(), 2 * cOsd::LineHeight(), buf, clrWhite, clrBackground, m_Window);
             }
          if (time_ms() - m_InfoTime < CHANNELINFO_TIMEOUT) {
             sprintf(buf, "%s Card #%d - %s", m_FrontendInfo.type == FE_QPSK ? "Satellite" : m_FrontendInfo.type == FE_QAM ? "Cable" : "Terrestial", cDevice::ActualDevice()->CardIndex(), m_FrontendInfo.name);
             m_Osd->Text(cOsd::CellWidth(), 4 * cOsd::LineHeight(), buf, clrWhite, clrBackground, m_Window);
             }
          else {
             m_Osd->Text(  1 * cOsd::CellWidth(), 4 * cOsd::LineHeight(), "LOCK",   (fe_status & FE_HAS_LOCK) ? clrYellow : clrBlack, clrBackground, m_Window);
             m_Osd->Text(  9 * cOsd::CellWidth(), 4 * cOsd::LineHeight(), "SIGNAL", (fe_status & FE_HAS_SIGNAL) ? clrYellow : clrBlack, clrBackground, m_Window);
             m_Osd->Text( 19 * cOsd::CellWidth(), 4 * cOsd::LineHeight(), "CARRIER",(fe_status & FE_HAS_CARRIER) ? clrYellow : clrBlack, clrBackground, m_Window);
             m_Osd->Text( 30 * cOsd::CellWidth(), 4 * cOsd::LineHeight(), "VITERBI",(fe_status & FE_HAS_VITERBI) ? clrYellow : clrBlack, clrBackground, m_Window);
             m_Osd->Text( 40 * cOsd::CellWidth(), 4 * cOsd::LineHeight(), "SYNC",   (fe_status & FE_HAS_SYNC) ? clrYellow : clrBlack, clrBackground, m_Window);
             }
#if (VDRVERSNUM >= 10300) || defined(ELCHIAIOVERSION)
          m_Osd->SetFont(OldFont);
#endif
          m_Osd->Flush();
          }
       }
    usleep(100000L * femonConfig.interval);
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
  m_InfoTime = time_ms();
  m_Osd = cOsd::OpenRaw(m_Xpos, m_Ypos);
  if (m_Osd) {
     //printf("X: %d - Y: %d - W: %d - H: %d\n", m_Xpos, m_Ypos, m_Width, m_Height);
     m_Window = m_Osd->Create(0, 0, m_Width, m_Height, 4);
     m_Osd->AddColor(clrBackground, m_Window);
     m_Osd->AddColor(clrRed, m_Window);
     m_Osd->AddColor(clrGreen, m_Window);
     m_Osd->AddColor(clrYellow, m_Window);
     m_Osd->AddColor(clrWhite, m_Window);
     m_Osd->AddColor(clrBlack, m_Window);
     m_Osd->AddColor(clrTransparent, m_Window);
     m_Osd->Clear(m_Window);
     m_Osd->Flush();
     if (m_Receiver)
        delete m_Receiver;
     m_Receiver = new cFemonReceiver(Channels.GetByNumber(cDevice::CurrentChannel())->Ca(), Channels.GetByNumber(cDevice::CurrentChannel())->Vpid(), Channels.GetByNumber(cDevice::CurrentChannel())->Apid1());
     cDevice::ActualDevice()->AttachReceiver(m_Receiver);
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
  m_Receiver = new cFemonReceiver(Channels.GetByNumber(cDevice::CurrentChannel())->Ca(), Channels.GetByNumber(cDevice::CurrentChannel())->Vpid(), Channels.GetByNumber(cDevice::CurrentChannel())->Apid1());
  cDevice::ActualDevice()->AttachReceiver(m_Receiver);
  m_InfoTime = time_ms();
}

eOSState cFemonOsd::ProcessKey(eKeys Key)
{ 
  eOSState state = cOsdObject::ProcessKey(Key);

  if (state == osUnknown) {
     switch (Key & ~k_Repeat) {
       case k0:
            if (m_Number == 0) {
               // keep the "Toggle channels" function working
               cRemote::Put(Key);
               return osContinue;
               }
       case k1 ... k9:
            if (m_Number >= 0) {
               m_Number = m_Number * 10 + Key - k0;
               if (m_Number > 0) {
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
                  m_Number = 0;
                  m_InputTime = time_ms();
                  }
               }
            break;
       case kOk:
            m_InfoTime = time_ms();
            break;
       default:
            break;
       }
     state = osContinue;
  }
  return state;
}
