/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/dvb/frontend.h>
#include "femonreceiver.h"
#include "femonosd.h"
#include "femontools.h"

cString getFrontendInfo(int cardIndex)
{
  cString info;
  struct dvb_frontend_info value;
  fe_status_t status;
  uint16_t signal = 0;
  uint16_t snr = 0;
  uint32_t ber = 0;
  uint32_t unc = 0;
  cChannel *channel = Channels.GetByNumber(cDevice::CurrentChannel());
  cString dev = cString::sprintf(FRONTEND_DEVICE, cardIndex, 0);

  int fe = open(dev, O_RDONLY | O_NONBLOCK);
  if (fe < 0)
     return NULL;
  CHECK(ioctl(fe, FE_GET_INFO, &value));
  CHECK(ioctl(fe, FE_READ_STATUS, &status));
  CHECK(ioctl(fe, FE_READ_SIGNAL_STRENGTH, &signal));
  CHECK(ioctl(fe, FE_READ_SNR, &snr));
  CHECK(ioctl(fe, FE_READ_BER, &ber));
  CHECK(ioctl(fe, FE_READ_UNCORRECTED_BLOCKS, &unc));
  close(fe);

  info = cString::sprintf("CARD:%d\nTYPE:%d\nNAME:%s\nSTAT:%02X\nSGNL:%04X\nSNRA:%04X\nBERA:%08X\nUNCB:%08X", cardIndex, value.type, value.name, status, signal, snr, ber, unc);

  if (cFemonOsd::Instance())
     info = cString::sprintf("%s\nVIBR:%.0f\nAUBR:%.0f\nDDBR:%.0f", *info, cFemonOsd::Instance()->GetVideoBitrate(), cFemonOsd::Instance()->GetAudioBitrate(), cFemonOsd::Instance()->GetDolbyBitrate());

  if (channel)
     info  = cString::sprintf("%s\nCHAN:%s", *info, *channel->ToText());

  return info;
}

cString getFrontendName(int cardIndex)
{
  struct dvb_frontend_info value;
  cString dev = cString::sprintf(FRONTEND_DEVICE, cardIndex, 0);

  int fe = open(dev, O_RDONLY | O_NONBLOCK);
  if (fe < 0)
     return NULL;
  CHECK(ioctl(fe, FE_GET_INFO, &value));
  close(fe);

  return (cString::sprintf("%s on device #%d", value.name, cardIndex));
}

cString getFrontendStatus(int cardIndex)
{
  fe_status_t value;
  cString dev = cString::sprintf(FRONTEND_DEVICE, cardIndex, 0);

  int fe = open(dev, O_RDONLY | O_NONBLOCK);
  if (fe < 0)
     return NULL;
  CHECK(ioctl(fe, FE_READ_STATUS, &value));
  close(fe);

  return (cString::sprintf("Status %s:%s:%s:%s:%s on device #%d", (value & FE_HAS_LOCK) ? "LOCKED" : "-", (value & FE_HAS_SIGNAL) ? "SIGNAL" : "-", (value & FE_HAS_CARRIER) ? "CARRIER" : "-", (value & FE_HAS_VITERBI) ? "VITERBI" : "-", (value & FE_HAS_SYNC) ? "SYNC" : "-", cardIndex));
}

uint16_t getSignal(int cardIndex)
{
  uint16_t value = 0;
  cString dev = cString::sprintf(FRONTEND_DEVICE, cardIndex, 0);

  int fe = open(dev, O_RDONLY | O_NONBLOCK);
  if (fe < 0)
     return (value);
  CHECK(ioctl(fe, FE_READ_SIGNAL_STRENGTH, &value));
  close(fe);

  return (value);
}

uint16_t getSNR(int cardIndex)
{
  uint16_t value = 0;
  cString dev = cString::sprintf(FRONTEND_DEVICE, cardIndex, 0);

  int fe = open(dev, O_RDONLY | O_NONBLOCK);
  if (fe < 0)
     return (value);
  CHECK(ioctl(fe, FE_READ_SNR, &value));
  close(fe);

  return (value);
}

uint32_t getBER(int cardIndex)
{
  uint32_t value = 0;
  cString dev = cString::sprintf(FRONTEND_DEVICE, cardIndex, 0);

  int fe = open(dev, O_RDONLY | O_NONBLOCK);
  if (fe < 0)
     return (value);
  CHECK(ioctl(fe, FE_READ_BER, &value));
  close(fe);

  return (value);
}

uint32_t getUNC(int cardIndex)
{
  uint32_t value = 0;
  cString dev = cString::sprintf(FRONTEND_DEVICE, cardIndex, 0);

  int fe = open(dev, O_RDONLY | O_NONBLOCK);
  if (fe < 0)
     return (value);
  CHECK(ioctl(fe, FE_READ_UNCORRECTED_BLOCKS, &value));
  close(fe);

  return (value);
}

cString getApids(const cChannel *channel)
{
  int value = 0;
  cString apids = cString::sprintf("%d", channel->Apid(value));
  while (channel->Apid(++value) && (value < MAXAPIDS))
    apids = cString::sprintf("%s, %d", *apids, channel->Apid(value));
  return apids;
}

cString getDpids(const cChannel *channel)
{
  int value = 0;
  cString dpids = cString::sprintf("%d", channel->Dpid(value));
  while (channel->Dpid(++value) && (value < MAXDPIDS))
    dpids = cString::sprintf("%s, %d", *dpids, channel->Dpid(value));
  return dpids;
}

cString getSpids(const cChannel *channel)
{
  int value = 0;
  cString spids = cString::sprintf("%d", channel->Spid(value));
  while (channel->Spid(++value) && (value < MAXSPIDS))
    spids = cString::sprintf("%s, %d", *spids, channel->Spid(value));
  return spids;
}

cString getCAids(const cChannel *channel, bool identify)
{
  cString caids;
  int value = 0;

  if (identify) {
     caids = cString::sprintf("%s", *getCA(channel->Ca(value)));
     while (channel->Ca(++value) && (value < MAXCAIDS))
       caids = cString::sprintf("%s, %s", *caids, *getCA(channel->Ca(value)));
     }
  else {
     caids = cString::sprintf("%04x", channel->Ca(value));
     while (channel->Ca(++value) && (value < MAXCAIDS))
       caids = cString::sprintf("%s, %04x", *caids, channel->Ca(value));
     }
  return caids;
}

cString getCA(int value)
{
  /* http://www.dvb.org/index.php?id=174 */
  switch (value) {
    case 0x0000:            return cString::sprintf("%s", trVDR("Free To Air"));  // Reserved
    case 0x0001 ... 0x009F:
    case 0x00A2 ... 0x00FF: return cString::sprintf("%s", tr("Fixed"));           // Standardized systems
    case 0x00A0 ... 0x00A1: return cString::sprintf("%s", tr("Analog"));          // Analog signals
    case 0x0100 ... 0x01FF: return cString::sprintf("%s", tr("SECA/Mediaguard")); // Canal Plus
    case 0x0500 ... 0x05FF: return cString::sprintf("%s", tr("Viaccess"));        // France Telecom
    case 0x0600 ... 0x06FF: return cString::sprintf("%s", tr("Irdeto"));          // Irdeto
    case 0x0900 ... 0x09FF: return cString::sprintf("%s", tr("NDS/Videoguard"));  // News Datacom
    case 0x0B00 ... 0x0BFF: return cString::sprintf("%s", tr("Conax"));           // Norwegian Telekom
    case 0x0D00 ... 0x0DFF: return cString::sprintf("%s", tr("CryptoWorks"));     // Philips
    case 0x0E00 ... 0x0EFF: return cString::sprintf("%s", tr("PowerVu"));         // Scientific Atlanta
    case 0x1200 ... 0x12FF: return cString::sprintf("%s", tr("NagraVision"));     // BellVu Express
    case 0x1700 ... 0x17FF: return cString::sprintf("%s", tr("BetaCrypt"));       // BetaTechnik
    case 0x1800 ... 0x18FF: return cString::sprintf("%s", tr("NagraVision"));     // Kudelski SA
    case 0x4A60 ... 0x4A6F: return cString::sprintf("%s", tr("SkyCrypt"));        // @Sky
    }
  return cString::sprintf("%X", value);
}

cString getVideoStream(int value)
{
  if (value != 0)
     return cString::sprintf("#%d", value);
  return cString::sprintf("---");
}

cString getAudioStream(int value, const cChannel *channel)
{
  int pid = 0;
  if (IS_AUDIO_TRACK(value))
     pid = int(value - ttAudioFirst);
  if (channel && channel->Apid(pid)) {
     if (channel->Alang(pid))
        return cString::sprintf("#%d (%s)", channel->Apid(pid), channel->Alang(pid));
     else
        return cString::sprintf("#%d", channel->Apid(pid));
     }
  return cString::sprintf("---");
}

cString getVideoCodec(int value)
{
  switch (value) {
    case VIDEO_CODEC_MPEG2: return cString::sprintf("%s", tr("MPEG-2"));
    case VIDEO_CODEC_H264:  return cString::sprintf("%s", tr("H.264"));
    }
  return cString::sprintf("---");
}

cString getAudioCodec(int value)
{
  switch (value) {
    case AUDIO_CODEC_MPEG1_I:   return cString::sprintf("%s", tr("MPEG-1 Layer I"));
    case AUDIO_CODEC_MPEG1_II:  return cString::sprintf("%s", tr("MPEG-1 Layer II"));
    case AUDIO_CODEC_MPEG1_III: return cString::sprintf("%s", tr("MPEG-1 Layer III"));
    case AUDIO_CODEC_MPEG2_I:   return cString::sprintf("%s", tr("MPEG-2 Layer I"));
    case AUDIO_CODEC_MPEG2_II:  return cString::sprintf("%s", tr("MPEG-2 Layer II"));
    case AUDIO_CODEC_MPEG2_III: return cString::sprintf("%s", tr("MPEG-2 Layer III"));
    case AUDIO_CODEC_HEAAC:     return cString::sprintf("%s", tr("HE-AAC"));
    }
  return cString::sprintf("---");
}

cString getAudioChannelMode(int value)
{
  switch (value) {
    case AUDIO_CHANNEL_MODE_STEREO:       return cString::sprintf("%s", tr("stereo"));
    case AUDIO_CHANNEL_MODE_JOINT_STEREO: return cString::sprintf("%s", tr("joint Stereo"));
    case AUDIO_CHANNEL_MODE_DUAL:         return cString::sprintf("%s", tr("dual"));
    case AUDIO_CHANNEL_MODE_SINGLE:       return cString::sprintf("%s", tr("mono"));
    }
  return cString::sprintf("---");
}

cString getCoderate(int value)
{
  switch (value) {
    case FEC_NONE: return cString::sprintf("%s", tr("none"));
    case FEC_1_2:  return cString::sprintf("1/2");
    case FEC_2_3:  return cString::sprintf("2/3");
    case FEC_3_4:  return cString::sprintf("3/4");
    case FEC_4_5:  return cString::sprintf("4/5");
    case FEC_5_6:  return cString::sprintf("5/6");
    case FEC_6_7:  return cString::sprintf("6/7");
    case FEC_7_8:  return cString::sprintf("7/8");
    case FEC_8_9:  return cString::sprintf("8/9");
    case FEC_AUTO: return cString::sprintf("%s", tr("auto"));
    }
  return cString::sprintf("---");
}

cString getTransmission(int value)
{
  switch (value) {
    case TRANSMISSION_MODE_2K:   return cString::sprintf("2K");
    case TRANSMISSION_MODE_8K:   return cString::sprintf("8K");
    case TRANSMISSION_MODE_AUTO: return cString::sprintf("%s", tr("auto"));
    }
  return cString::sprintf("---");
}
 
cString getBandwidth(int value)
{
  switch (value) {
    case BANDWIDTH_8_MHZ: return cString::sprintf("8 %s", tr("MHz"));
    case BANDWIDTH_7_MHZ: return cString::sprintf("7 %s", tr("MHz"));
    case BANDWIDTH_6_MHZ: return cString::sprintf("6 %s", tr("MHz"));
    case BANDWIDTH_AUTO:  return cString::sprintf("%s", tr("auto"));
    }
  return cString::sprintf("---");
}

cString getInversion(int value)
{
  switch (value) {
    case INVERSION_OFF:  return cString::sprintf("%s", tr("off"));
    case INVERSION_ON:   return cString::sprintf("%s", tr("on"));
    case INVERSION_AUTO: return cString::sprintf("%s", tr("auto"));
    }
  return cString::sprintf("---");
}

cString getHierarchy(int value)
{
  switch (value) {
    case HIERARCHY_NONE: return cString::sprintf("%s", tr("none"));
    case HIERARCHY_1:    return cString::sprintf("1");
    case HIERARCHY_2:    return cString::sprintf("2");
    case HIERARCHY_4:    return cString::sprintf("4");
    case HIERARCHY_AUTO: cString::sprintf("%s", tr("auto"));
    }
  return cString::sprintf("---");
}

cString getGuard(int value)
{
  switch (value) {
    case GUARD_INTERVAL_1_32: return cString::sprintf("1/32");
    case GUARD_INTERVAL_1_16: return cString::sprintf("1/16");
    case GUARD_INTERVAL_1_8:  return cString::sprintf("1/8");
    case GUARD_INTERVAL_1_4:  return cString::sprintf("1/4");
    case GUARD_INTERVAL_AUTO: cString::sprintf("%s", tr("auto"));
    }
  return cString::sprintf("---");
}

cString getModulation(int value)
{
  switch (value) {
    case QPSK:     return cString::sprintf("QPSK");
    case QAM_16:   return cString::sprintf("QAM 16");
    case QAM_32:   return cString::sprintf("QAM 32");
    case QAM_64:   return cString::sprintf("QAM 64");
    case QAM_128:  return cString::sprintf("QAM 128");
    case QAM_256:  return cString::sprintf("QAM 256");
    case QAM_AUTO: return cString::sprintf("QAM %s", tr("auto"));
    }
  return cString::sprintf("---");
}

cString getAlpha(int value)
{
  return cString::sprintf("---");
}

cString getPriority(int value)
{
  return cString::sprintf("---");
}

cString getSystem(int value)
{
  return cString::sprintf("---");
}

cString getRollOff(int value)
{
  return cString::sprintf("---");
}

cString getResolution(int width, int height, int scan)
{
  if ((width > 0) && (height > 0)) {
     switch (scan) {
       case VIDEO_SCAN_INTERLACED:  return cString::sprintf("%dx%d %s", width, height, tr("interlaced"));
       case VIDEO_SCAN_PROGRESSIVE: return cString::sprintf("%dx%d %s", width, height, tr("progressive"));
       default:                     return cString::sprintf("%dx%d",    width, height);
       }
     }
  return cString::sprintf("---");
}

cString getAspectRatio(int value)
{
  switch (value) {
    case VIDEO_ASPECT_RATIO_RESERVED: return cString::sprintf("%s", tr("reserved"));
    case VIDEO_ASPECT_RATIO_EXTENDED: return cString::sprintf("%s", tr("extended"));
    case VIDEO_ASPECT_RATIO_1_1:      return cString::sprintf("1:1");
    case VIDEO_ASPECT_RATIO_4_3:      return cString::sprintf("4:3");
    case VIDEO_ASPECT_RATIO_16_9:     return cString::sprintf("16:9");
    case VIDEO_ASPECT_RATIO_2_21_1:   return cString::sprintf("2.21:1");
    case VIDEO_ASPECT_RATIO_12_11:    return cString::sprintf("12:11");
    case VIDEO_ASPECT_RATIO_10_11:    return cString::sprintf("10:11");
    case VIDEO_ASPECT_RATIO_16_11:    return cString::sprintf("16:11");
    case VIDEO_ASPECT_RATIO_40_33:    return cString::sprintf("40:33");
    case VIDEO_ASPECT_RATIO_24_11:    return cString::sprintf("24:11");
    case VIDEO_ASPECT_RATIO_20_11:    return cString::sprintf("20:11");
    case VIDEO_ASPECT_RATIO_32_11:    return cString::sprintf("32:11");
    case VIDEO_ASPECT_RATIO_80_33:    return cString::sprintf("80:33");
    case VIDEO_ASPECT_RATIO_18_11:    return cString::sprintf("18:11");
    case VIDEO_ASPECT_RATIO_15_11:    return cString::sprintf("15:11");
    case VIDEO_ASPECT_RATIO_64_33:    return cString::sprintf("64:33");
    case VIDEO_ASPECT_RATIO_160_99:   return cString::sprintf("160:99");
    case VIDEO_ASPECT_RATIO_3_2:      return cString::sprintf("3:2");
    case VIDEO_ASPECT_RATIO_2_1:      return cString::sprintf("2:1");
    }
  return cString::sprintf("---");
}

cString getVideoFormat(int value)
{
  switch (value) {
    case VIDEO_FORMAT_UNKNOWN:   return cString::sprintf("%s", tr("unknown"));
    case VIDEO_FORMAT_RESERVED:  return cString::sprintf("%s", tr("reserved"));
    case VIDEO_FORMAT_COMPONENT: return cString::sprintf("%s", tr("component"));
    case VIDEO_FORMAT_PAL:       return cString::sprintf("%s", tr("PAL"));
    case VIDEO_FORMAT_NTSC:      return cString::sprintf("%s", tr("NTSC"));
    case VIDEO_FORMAT_SECAM:     return cString::sprintf("%s", tr("SECAM"));
    case VIDEO_FORMAT_MAC:       return cString::sprintf("%s", tr("MAC"));
    }
  return cString::sprintf("---");
}

cString getFrameRate(double value)
{
  if (value > 0)
     return cString::sprintf("%.2f %s", value, tr("Hz"));
  return cString::sprintf("---");
}

cString getAC3BitStreamMode(int value, int coding)
{
  switch (value) {
    case AUDIO_BITSTREAM_MODE_CM:     return cString::sprintf("%s", tr("Complete Main (CM)"));
    case AUDIO_BITSTREAM_MODE_ME:     return cString::sprintf("%s", tr("Music and Effects (ME)"));
    case AUDIO_BITSTREAM_MODE_VI:     return cString::sprintf("%s", tr("Visually Impaired (VI)"));
    case AUDIO_BITSTREAM_MODE_HI:     return cString::sprintf("%s", tr("Hearing Impaired (HI)"));
    case AUDIO_BITSTREAM_MODE_D:      return cString::sprintf("%s", tr("Dialogue (D)"));
    case AUDIO_BITSTREAM_MODE_C:      return cString::sprintf("%s", tr("Commentary (C)"));
    case AUDIO_BITSTREAM_MODE_E:      return cString::sprintf("%s", tr("Emergency (E)"));
    case AUDIO_BITSTREAM_MODE_VO_KAR: return cString::sprintf("%s", (coding == 1) ? tr("Voice Over (VO)") : tr("Karaoke"));
    }
  return cString::sprintf("---");
}

cString getAC3AudioCodingMode(int value, int stream)
{
  if (stream != 7) {
     switch (value) {
       case AUDIO_CODING_MODE_1_1: return cString::sprintf("1+1 - %s, %s", tr("Ch1"), tr("Ch2"));
       case AUDIO_CODING_MODE_1_0: return cString::sprintf("1/0 - %s", tr("C"));
       case AUDIO_CODING_MODE_2_0: return cString::sprintf("2/0 - %s, %s", tr("L"), tr("R"));
       case AUDIO_CODING_MODE_3_0: return cString::sprintf("3/0 - %s, %s, %s", tr("L"), tr("C"), tr("R"));
       case AUDIO_CODING_MODE_2_1: return cString::sprintf("2/1 - %s, %s, %s", tr("L"), tr("R"), tr("S"));
       case AUDIO_CODING_MODE_3_1: return cString::sprintf("3/1 - %s, %s, %s, %s", tr("L"), tr("C"), tr("R"), tr("S"));
       case AUDIO_CODING_MODE_2_2: return cString::sprintf("2/2 - %s, %s, %s, %s", tr("L"), tr("R"), tr("SL"), tr("SR"));
       case AUDIO_CODING_MODE_3_2: return cString::sprintf("3/2 - %s, %s, %s, %s, %s", tr("L"), tr("C"), tr("R"), tr("SL"), tr("SR"));
       }
     }
  return cString::sprintf("---");
}

cString getAC3CenterMixLevel(int value)
{
  switch (value) {
    case AUDIO_CENTER_MIX_LEVEL_MINUS_3dB:   return cString::sprintf("-3.0 %s", tr("dB"));
    case AUDIO_CENTER_MIX_LEVEL_MINUS_4_5dB: return cString::sprintf("-4.5 %s", tr("dB"));
    case AUDIO_CENTER_MIX_LEVEL_MINUS_6dB:   return cString::sprintf("-6.0 %s", tr("dB"));
    case AUDIO_CENTER_MIX_LEVEL_RESERVED:    return cString::sprintf("%s", tr("reserved"));
    }
  return cString::sprintf("---");
}

cString getAC3SurroundMixLevel(int value)
{
  switch (value) {
    case AUDIO_SURROUND_MIX_LEVEL_MINUS_3dB: return cString::sprintf("-3 %s", tr("dB"));
    case AUDIO_SURROUND_MIX_LEVEL_MINUS_6dB: return cString::sprintf("-6 %s", tr("dB"));
    case AUDIO_SURROUND_MIX_LEVEL_0_dB:      return cString::sprintf("0 %s", tr("dB"));
    case AUDIO_SURROUND_MIX_LEVEL_RESERVED:  return cString::sprintf("%s", tr("reserved"));
    }
  return cString::sprintf("---");
}

cString getAC3DolbySurroundMode(int value)
{
  switch (value) {
    case AUDIO_DOLBY_SURROUND_MODE_NOT_INDICATED:     return cString::sprintf("%s", tr("not indicated"));
    case AUDIO_DOLBY_SURROUND_MODE_NOT_DOLBYSURROUND: return cString::sprintf("%s", trVDR("no"));
    case AUDIO_DOLBY_SURROUND_MODE_DOLBYSURROUND:     return cString::sprintf("%s", trVDR("yes"));
    case AUDIO_DOLBY_SURROUND_MODE_RESERVED:          return cString::sprintf("%s", tr("reserved"));
    }
  return cString::sprintf("---");
}

cString getAC3DialogLevel(int value)
{
  if (value > 0)
     return cString::sprintf("-%d %s", value, tr("dB"));
  return cString::sprintf("---");
}

cString getFrequencyMHz(int value)
{
   while (value > 20000) value /= 1000;
   return cString::sprintf("%d %s", value, tr("MHz"));
}

cString getAudioSamplingFreq(int value)
{
  switch (value) {
    case AUDIO_SAMPLING_FREQUENCY_INVALID:  return cString::sprintf("---");
    case AUDIO_SAMPLING_FREQUENCY_RESERVED: return cString::sprintf("%s", tr("reserved"));
    }
  return cString::sprintf("%d %s", value, tr("Hz"));
}

cString getAudioBitrate(double value, double stream)
{
  switch ((int)stream) {
    case AUDIO_BITRATE_INVALID:  return cString::sprintf("---");
    case AUDIO_BITRATE_RESERVED: return cString::sprintf("%s (%s)", tr("reserved"), *getBitrateKbits(value));
    case AUDIO_BITRATE_FREE:     return cString::sprintf("%s (%s)", tr("free"), *getBitrateKbits(value));
    }
  return cString::sprintf("%s (%s)", *getBitrateKbits(stream), *getBitrateKbits(value));
}

cString getVideoBitrate(double value, double stream)
{
  return cString::sprintf("%s (%s)", *getBitrateMbits(stream), *getBitrateMbits(value));
}

cString getBitrateMbits(double value)
{
  if (value > 0)
     return cString::sprintf("%.2f %s", value / 1000000.0, tr("Mbit/s"));
  return cString::sprintf("---");
}

cString getBitrateKbits(double value)
{
  if (value > 0) 
     return cString::sprintf("%.0f %s", value / 1000.0, tr("kbit/s"));
  return cString::sprintf("---");
}