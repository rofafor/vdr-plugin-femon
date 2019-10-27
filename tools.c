/*
 * tools.c: Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "osd.h"
#include "receiver.h"
#include "tools.h"

static cString getCA(int valueP)
{
  // http://www.dvb.org/index.php?id=174
  // http://en.wikipedia.org/wiki/Conditional_access_system
  switch (valueP) {
    case 0x0000:            return cString::sprintf("%s (%X)", trVDR("Free To Air"), valueP);  // Reserved
    case 0x0001 ... 0x009F:
    case 0x00A2 ... 0x00FF: return cString::sprintf("%s (%X)", tr("Fixed"),  valueP); // Standardized systems
    case 0x00A0 ... 0x00A1: return cString::sprintf("%s (%X)", tr("Analog"), valueP); // Analog signals
    case 0x0100 ... 0x01FF: return cString::sprintf("SECA Mediaguard (%X)",  valueP); // Canal Plus
    case 0x0464:            return cString::sprintf("EuroDec (%X)",          valueP); // EuroDec
    case 0x0500 ... 0x05FF: return cString::sprintf("Viaccess (%X)",         valueP); // France Telecom
    case 0x0600 ... 0x06FF: return cString::sprintf("Irdeto (%X)",           valueP); // Irdeto
    case 0x0700 ... 0x07FF: return cString::sprintf("DigiCipher 2 (%X)",     valueP); // Jerrold/GI/Motorola 4DTV
    case 0x0900 ... 0x09FF: return cString::sprintf("NDS Videoguard (%X)",   valueP); // NDS
    case 0x0B00 ... 0x0BFF: return cString::sprintf("Conax (%X)",            valueP); // Norwegian Telekom
    case 0x0D00 ... 0x0DFF: return cString::sprintf("CryptoWorks (%X)",      valueP); // Philips CryptoTec
    case 0x0E00 ... 0x0EFF: return cString::sprintf("PowerVu (%X)",          valueP); // Scientific Atlanta
    case 0x1000:            return cString::sprintf("RAS (%X)",              valueP); // Tandberg Television
    case 0x1200 ... 0x12FF: return cString::sprintf("NagraVision (%X)",      valueP); // BellVu Express
    case 0x1700 ... 0x17FF: return cString::sprintf("VCAS (%X)",             valueP); // Verimatrix Inc. former BetaTechnik
    case 0x1800 ... 0x18FF: return cString::sprintf("NagraVision (%X)",      valueP); // Kudelski SA
    case 0x22F0:            return cString::sprintf("Codicrypt (%X)",        valueP); // Scopus Network Technologies
    case 0x2600:            return cString::sprintf("BISS (%X)",             valueP); // European Broadcasting Union
    case 0x2719:            return cString::sprintf("VanyaCas (%X)",         valueP); // S-Curious Research & Technology Pvt. Ltd.
    case 0x4347:            return cString::sprintf("CryptOn (%X)",          valueP); // CryptOn
    case 0x4800:            return cString::sprintf("Accessgate (%X)",       valueP); // Telemann
    case 0x4900:            return cString::sprintf("China Crypt (%X)",      valueP); // CryptoWorks
    case 0x4A02:            return cString::sprintf("Tongfang (%X)",         valueP); // Tsinghua Tongfang Company
    case 0x4A10:            return cString::sprintf("EasyCas (%X)",          valueP); // EasyCas
    case 0x4A20:            return cString::sprintf("AlphaCrypt (%X)",       valueP); // AlphaCrypt
    case 0x4A60:            return cString::sprintf("SkyCrypt (%X)",         valueP); // @Sky
    case 0x4A61:            return cString::sprintf("Neotioncrypt (%X)",     valueP); // Neotion
    case 0x4A62:            return cString::sprintf("SkyCrypt (%X)",         valueP); // @Sky
    case 0x4A63:            return cString::sprintf("Neotion SHL (%X)",      valueP); // Neotion
    case 0x4A64 ... 0x4A6F: return cString::sprintf("SkyCrypt (%X)",         valueP); // @Sky
    case 0x4A70:            return cString::sprintf("DreamCrypt (%X)",       valueP); // Dream Multimedia
    case 0x4A80:            return cString::sprintf("ThalesCrypt (%X)",      valueP); // Thales Broadcast & Multimedia
    case 0x4AA1:            return cString::sprintf("KeyFly (%X)",           valueP); // SIDSA
    case 0x4ABF:            return cString::sprintf("CTI-CAS (%X)",          valueP); // Beijing Compunicate Technology Inc.
    case 0x4AC1:            return cString::sprintf("Latens (%X)",           valueP); // Latens Systems
    case 0x4AD0 ... 0x4AD1: return cString::sprintf("X-Crypt (%X)",          valueP); // XCrypt Inc.
    case 0x4AD4:            return cString::sprintf("OmniCrypt (%X)",        valueP); // Widevine Technologies, Inc.
    case 0x4AE0 ... 0x4AE1: return cString::sprintf("Z-Crypt (%X)",          valueP); // Digi Raum Electronics Co. Ltd.
    case 0x4AE4:            return cString::sprintf("CoreCrypt (%X)",        valueP); // CoreTrust
    case 0x4AE5:            return cString::sprintf("PRO-Crypt (%X)",        valueP); // IK SATPROF
    case 0x4AEA:            return cString::sprintf("Cryptoguard (%X)",      valueP); // Gryptoguard AB
    case 0x4AEB:            return cString::sprintf("Abel Quintic (%X)",     valueP); // Abel DRM Systems
    case 0x4AF0:            return cString::sprintf("ABV (%X)",              valueP); // Alliance Broadcast Vision
    case 0x5500:            return cString::sprintf("Z-Crypt (%X)",          valueP); // Digi Raum Electronics Co. Ltd.
    case 0x5501:            return cString::sprintf("Griffin (%X)",          valueP); // Nucleus Systems Ltd.
    case 0x5581:            return cString::sprintf("Bulcrypt (%X)",         valueP); // Bulcrypt
    case 0x7BE1:            return cString::sprintf("DRE-Crypt (%X)",        valueP); // DRE-Crypt
    case 0xA101:            return cString::sprintf("RosCrypt-M (%X)",       valueP); // NIIR
    case 0xEAD0:            return cString::sprintf("VanyaCas (%X)",         valueP); // S-Curious Research & Technology Pvt. Ltd.
    default:                break;
    }
  return cString::sprintf("%X", valueP);
}

static const char *getUserString(int valueP, const tDvbParameterMap *mapP)
{
  const tDvbParameterMap *map = mapP;
  while (map && map->userValue != -1) {
        if (map->driverValue == valueP)
           return map->userString ? trVDR(map->userString) : "---";
        map++;
        }
  return "---";
}

cString getFrontendInfo(cDevice *deviceP)
{
  const cChannel *channel;
  int status, valid = DTV_STAT_VALID_NONE;
  cString info = "";
  double signal = 0, cnr = 0, ber = 0, per = 0;

  if (!deviceP)
     return info;

  info = cString::sprintf("CARD:%d\nSTRG:%d\nQUAL:%d\nTYPE:%s\nNAME:%s", deviceP->CardIndex(), deviceP->SignalStrength(), deviceP->SignalQuality(), *deviceP->DeviceType(), *deviceP->DeviceName());
  if (deviceP && deviceP->SignalStats(valid, &signal, &cnr, NULL, &ber, &per, &status)) {
     if (valid & DTV_STAT_VALID_STATUS)
        info = cString::sprintf("%s\nSTAT:%04X", *info, status);
     if (valid & DTV_STAT_VALID_STRENGTH)
        info = cString::sprintf("%s\nSGNL:%s", *info, *dtoa(signal, "%.2f"));
     if (valid & DTV_STAT_VALID_CNR)
        info = cString::sprintf("%s\nCNRA:%s", *info, *dtoa(cnr, "%.2f"));
     if (valid & DTV_STAT_VALID_BERPOST)
        info = cString::sprintf("%s\nBERA:%s", *info, *dtoa(ber, "%.0f"));
     if (valid & DTV_STAT_VALID_PER)
        info = cString::sprintf("%s\nPERA:%s", *info, *dtoa(per, "%.0f"));
     }

  if (cFemonOsd::Instance())
     info = cString::sprintf("%s\nVIBR:%s\nAUBR:%s\nDDBR:%s", *info, *dtoa(cFemonOsd::Instance()->GetVideoBitrate(), "%.0f"), *dtoa(cFemonOsd::Instance()->GetAudioBitrate(), "%.0f"), *dtoa(cFemonOsd::Instance()->GetDolbyBitrate(), "%.0f"));

  LOCK_CHANNELS_READ;
  channel = Channels->GetByNumber(cDevice::CurrentChannel());
  if (channel)
     info = cString::sprintf("%s\nCHAN:%s", *info, *channel->ToText());

  return info;
}

cString getFrontendName(cDevice *deviceP)
{
  if (!deviceP)
     return NULL;

  return (cString::sprintf("%s on deviceP #%d", *deviceP->DeviceName(), deviceP->CardIndex()));
}

cString getFrontendStatus(cDevice *deviceP)
{
  int status;
  int valid = DTV_STAT_VALID_NONE;

  if (deviceP && deviceP->SignalStats(valid, NULL, NULL, NULL, NULL, NULL, &status)) {
     if (valid & DTV_STAT_VALID_STATUS)
        return (cString::sprintf("Status %s:%s:%s:%s:%s on deviceP #%d", (status & DTV_STAT_HAS_LOCK) ? "LOCKED" : "-", (status & DTV_STAT_HAS_SIGNAL) ? "SIGNAL" : "-", (status & DTV_STAT_HAS_CARRIER) ? "CARRIER" : "-", (status & DTV_STAT_HAS_VITERBI) ? "VITERBI" : "-", (status & DTV_STAT_HAS_SYNC) ? "SYNC" : "-", deviceP->CardIndex()));
     }

  return NULL;
}

double getSignal(cDevice *deviceP)
{
  double strength;
  int valid = DTV_STAT_VALID_NONE;

  if (deviceP && deviceP->SignalStats(valid, &strength, NULL, NULL, NULL, NULL, NULL)) {
     if (valid & DTV_STAT_VALID_STRENGTH)
        return strength;
     }

  return 0;
}

double getCNR(cDevice *deviceP)
{
  double cnr;
  int valid = DTV_STAT_VALID_NONE;

  if (deviceP && deviceP->SignalStats(valid, NULL, &cnr, NULL, NULL, NULL, NULL)) {
     if (valid & DTV_STAT_VALID_CNR)
        return cnr;
     }

  return 0;
}

double getBER(cDevice *deviceP)
{
  double ber;
  int valid = DTV_STAT_VALID_NONE;

  if (deviceP && deviceP->SignalStats(valid, NULL, NULL, NULL, &ber, NULL, NULL)) {
     if (valid & DTV_STAT_VALID_BERPOST)
        return ber;
     }

  return 0;
}

double getPER(cDevice *deviceP)
{
  double per;
  int valid = DTV_STAT_VALID_NONE;

  if (deviceP && deviceP->SignalStats(valid, NULL, NULL, NULL, NULL, &per, NULL)) {
     if (valid & DTV_STAT_VALID_PER)
        return per;
     }

  return 0;
}

cString getSignalStrength(double strengthP)
{
  switch (FemonConfig.GetSignalUnit()) {
    case eFemonSignalUnitdBm:  return cString::sprintf("%.2f %s", strengthP, tr("dBm"));
    case eFemonSignalUnitdBuV: return cString::sprintf("%.2f %s", strengthP + (120 - 11.25), tr("dBuV"));
    case eFemonSignalUnitdBV:  return cString::sprintf("%.2f %s", strengthP - 11.25, tr("dBV"));
    default:                   break;
    }
  return cString::sprintf("---");
}

cString getApids(const cChannel *channelP)
{
  int value = 0;
  cString apids = cString::sprintf("%d", channelP->Apid(value));
  while (channelP->Apid(++value) && (value < MAXAPIDS))
    apids = cString::sprintf("%s, %d", *apids, channelP->Apid(value));
  return apids;
}

cString getDpids(const cChannel *channelP)
{
  int value = 0;
  cString dpids = cString::sprintf("%d", channelP->Dpid(value));
  while (channelP->Dpid(++value) && (value < MAXDPIDS))
    dpids = cString::sprintf("%s, %d", *dpids, channelP->Dpid(value));
  return dpids;
}

cString getSpids(const cChannel *channelP)
{
  int value = 0;
  cString spids = cString::sprintf("%d", channelP->Spid(value));
  while (channelP->Spid(++value) && (value < MAXSPIDS))
    spids = cString::sprintf("%s, %d", *spids, channelP->Spid(value));
  return spids;
}

cString getCAids(const cChannel *channelP)
{
  int value = 0;
  cString caids = cString::sprintf("%s", *getCA(channelP->Ca(value)));
  while (channelP->Ca(++value) && (value < MAXCAIDS))
    caids = cString::sprintf("%s, %s", *caids, *getCA(channelP->Ca(value)));
  return caids;
}

cString getVideoStream(int value)
{
  if (value != 0)
     return cString::sprintf("#%d", value);
  return cString::sprintf("---");
}

cString getAudioStream(int valueP, const cChannel *channelP)
{
  int pid = 0;
  if (IS_AUDIO_TRACK(valueP))
     pid = int(valueP - ttAudioFirst);
  if (channelP && channelP->Apid(pid)) {
     if (channelP->Alang(pid))
        return cString::sprintf("#%d (%s)", channelP->Apid(pid), channelP->Alang(pid));
     else
        return cString::sprintf("#%d", channelP->Apid(pid));
     }
  return cString::sprintf("---");
}

cString getAC3Stream(int valueP, const cChannel *channelP)
{
  int pid = 0;
  if (IS_DOLBY_TRACK(valueP))
     pid = int(valueP - ttDolbyFirst);
  if (channelP && channelP->Dpid(pid)) {
     if (channelP->Dlang(pid))
        return cString::sprintf("#%d (%s)", channelP->Dpid(pid), channelP->Dlang(pid));
     else
        return cString::sprintf("#%d", channelP->Dpid(pid));
     }
  return cString::sprintf("---");
}

cString getVideoCodec(int valueP)
{
  switch (valueP) {
    case VIDEO_CODEC_MPEG2: return cString::sprintf("%s", tr("MPEG-2"));
    case VIDEO_CODEC_H264:  return cString::sprintf("%s", tr("H.264"));
    case VIDEO_CODEC_H265:  return cString::sprintf("%s", tr("H.265"));
    default:                break;
    }
  return cString::sprintf("---");
}

cString getAudioCodec(int valueP)
{
  switch (valueP) {
    case AUDIO_CODEC_MPEG1_I:   return cString::sprintf("%s", tr("MPEG-1 Layer I"));
    case AUDIO_CODEC_MPEG1_II:  return cString::sprintf("%s", tr("MPEG-1 Layer II"));
    case AUDIO_CODEC_MPEG1_III: return cString::sprintf("%s", tr("MPEG-1 Layer III"));
    case AUDIO_CODEC_MPEG2_I:   return cString::sprintf("%s", tr("MPEG-2 Layer I"));
    case AUDIO_CODEC_MPEG2_II:  return cString::sprintf("%s", tr("MPEG-2 Layer II"));
    case AUDIO_CODEC_MPEG2_III: return cString::sprintf("%s", tr("MPEG-2 Layer III"));
    case AUDIO_CODEC_HEAAC:     return cString::sprintf("%s", tr("HE-AAC"));
    case AUDIO_CODEC_LATM:      return cString::sprintf("%s", tr("LATM"));
    default:                    break;
    }
  return cString::sprintf("---");
}

cString getAudioChannelMode(int valueP)
{
  switch (valueP) {
    case AUDIO_CHANNEL_MODE_STEREO:       return cString::sprintf("%s", tr("stereo"));
    case AUDIO_CHANNEL_MODE_JOINT_STEREO: return cString::sprintf("%s", tr("joint Stereo"));
    case AUDIO_CHANNEL_MODE_DUAL:         return cString::sprintf("%s", tr("dual"));
    case AUDIO_CHANNEL_MODE_SINGLE:       return cString::sprintf("%s", tr("mono"));
    default:                              break;
    }
  return cString::sprintf("---");
}

cString getCoderate(int valueP)
{
  return cString::sprintf("%s", getUserString(valueP, CoderateValues));
}

cString getTransmission(int valueP)
{
  return cString::sprintf("%s", getUserString(valueP, TransmissionValues));
}

cString getBandwidth(int valueP)
{
  return cString::sprintf("%s", getUserString(valueP, BandwidthValues));
}

cString getInversion(int valueP)
{
  return cString::sprintf("%s", getUserString(valueP, InversionValues));
}

cString getHierarchy(int valueP)
{
  return cString::sprintf("%s", getUserString(valueP, HierarchyValues));
}

cString getGuard(int valueP)
{
  return cString::sprintf("%s", getUserString(valueP, GuardValues));
}

cString getModulation(int valueP)
{
  return cString::sprintf("%s", getUserString(valueP, ModulationValues));
}

cString getTerrestrialSystem(int valueP)
{
  return cString::sprintf("%s", getUserString(valueP, SystemValuesTerr));
}

cString getSatelliteSystem(int valueP)
{
  return cString::sprintf("%s", getUserString(valueP, SystemValuesSat));
}

cString getRollOff(int valueP)
{
  return cString::sprintf("%s", getUserString(valueP, RollOffValues));
}

cString getPilot(int valueP)
{
  return cString::sprintf("%s", getUserString(valueP, PilotValues));
}

cString getResolution(int widthP, int heightP, int scanP)
{
  if ((widthP > 0) && (heightP > 0)) {
     switch (scanP) {
       case VIDEO_SCAN_INTERLACED:  return cString::sprintf("%dx%d %s", widthP, heightP, tr("interlaced"));
       case VIDEO_SCAN_PROGRESSIVE: return cString::sprintf("%dx%d %s", widthP, heightP, tr("progressive"));
       default:                     return cString::sprintf("%dx%d",    widthP, heightP);
       }
     }
  return cString::sprintf("---");
}

cString getAspectRatio(int valueP)
{
  switch (valueP) {
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
    default:                          break;
    }
  return cString::sprintf("---");
}

cString getVideoFormat(int valueP)
{
  switch (valueP) {
    case VIDEO_FORMAT_UNKNOWN:   return cString::sprintf("%s", tr("unknown"));
    case VIDEO_FORMAT_RESERVED:  return cString::sprintf("%s", tr("reserved"));
    case VIDEO_FORMAT_COMPONENT: return cString::sprintf("%s", tr("component"));
    case VIDEO_FORMAT_PAL:       return cString::sprintf("%s", tr("PAL"));
    case VIDEO_FORMAT_NTSC:      return cString::sprintf("%s", tr("NTSC"));
    case VIDEO_FORMAT_SECAM:     return cString::sprintf("%s", tr("SECAM"));
    case VIDEO_FORMAT_MAC:       return cString::sprintf("%s", tr("MAC"));
    default:                     break;
    }
  return cString::sprintf("---");
}

cString getFrameRate(double valueP)
{
  if (valueP > 0)
     return cString::sprintf("%.2f %s", valueP, tr("Hz"));
  return cString::sprintf("---");
}

cString getAC3BitStreamMode(int valueP, int codingP)
{
  switch (valueP) {
    case AUDIO_BITSTREAM_MODE_CM:     return cString::sprintf("%s", tr("Complete Main (CM)"));
    case AUDIO_BITSTREAM_MODE_ME:     return cString::sprintf("%s", tr("Music and Effects (ME)"));
    case AUDIO_BITSTREAM_MODE_VI:     return cString::sprintf("%s", tr("Visually Impaired (VI)"));
    case AUDIO_BITSTREAM_MODE_HI:     return cString::sprintf("%s", tr("Hearing Impaired (HI)"));
    case AUDIO_BITSTREAM_MODE_D:      return cString::sprintf("%s", tr("Dialogue (D)"));
    case AUDIO_BITSTREAM_MODE_C:      return cString::sprintf("%s", tr("Commentary (C)"));
    case AUDIO_BITSTREAM_MODE_E:      return cString::sprintf("%s", tr("Emergency (E)"));
    case AUDIO_BITSTREAM_MODE_VO_KAR: return cString::sprintf("%s", (codingP == 1) ? tr("Voice Over (VO)") : tr("Karaoke"));
    default:                          break;
    }
  return cString::sprintf("---");
}

cString getAC3AudioCodingMode(int valueP, int streamP)
{
  if (streamP != 7) {
     switch (valueP) {
       case AUDIO_CODING_MODE_1_1: return cString::sprintf("1+1 - %s, %s", tr("Ch1"), tr("Ch2"));
       case AUDIO_CODING_MODE_1_0: return cString::sprintf("1/0 - %s", tr("C"));
       case AUDIO_CODING_MODE_2_0: return cString::sprintf("2/0 - %s, %s", tr("L"), tr("R"));
       case AUDIO_CODING_MODE_3_0: return cString::sprintf("3/0 - %s, %s, %s", tr("L"), tr("C"), tr("R"));
       case AUDIO_CODING_MODE_2_1: return cString::sprintf("2/1 - %s, %s, %s", tr("L"), tr("R"), tr("S"));
       case AUDIO_CODING_MODE_3_1: return cString::sprintf("3/1 - %s, %s, %s, %s", tr("L"), tr("C"), tr("R"), tr("S"));
       case AUDIO_CODING_MODE_2_2: return cString::sprintf("2/2 - %s, %s, %s, %s", tr("L"), tr("R"), tr("SL"), tr("SR"));
       case AUDIO_CODING_MODE_3_2: return cString::sprintf("3/2 - %s, %s, %s, %s, %s", tr("L"), tr("C"), tr("R"), tr("SL"), tr("SR"));
       default:                    break;
       }
     }
  return cString::sprintf("---");
}

cString getAC3CenterMixLevel(int valueP)
{
  switch (valueP) {
    case AUDIO_CENTER_MIX_LEVEL_MINUS_3dB:   return cString::sprintf("-3.0 %s", tr("dB"));
    case AUDIO_CENTER_MIX_LEVEL_MINUS_4_5dB: return cString::sprintf("-4.5 %s", tr("dB"));
    case AUDIO_CENTER_MIX_LEVEL_MINUS_6dB:   return cString::sprintf("-6.0 %s", tr("dB"));
    case AUDIO_CENTER_MIX_LEVEL_RESERVED:    return cString::sprintf("%s", tr("reserved"));
    default:                                 break;
    }
  return cString::sprintf("---");
}

cString getAC3SurroundMixLevel(int valueP)
{
  switch (valueP) {
    case AUDIO_SURROUND_MIX_LEVEL_MINUS_3dB: return cString::sprintf("-3 %s", tr("dB"));
    case AUDIO_SURROUND_MIX_LEVEL_MINUS_6dB: return cString::sprintf("-6 %s", tr("dB"));
    case AUDIO_SURROUND_MIX_LEVEL_0_dB:      return cString::sprintf("0 %s", tr("dB"));
    case AUDIO_SURROUND_MIX_LEVEL_RESERVED:  return cString::sprintf("%s", tr("reserved"));
    default:                                 break;
    }
  return cString::sprintf("---");
}

cString getAC3DolbySurroundMode(int valueP)
{
  switch (valueP) {
    case AUDIO_DOLBY_SURROUND_MODE_NOT_INDICATED:     return cString::sprintf("%s", tr("not indicated"));
    case AUDIO_DOLBY_SURROUND_MODE_NOT_DOLBYSURROUND: return cString::sprintf("%s", trVDR("no"));
    case AUDIO_DOLBY_SURROUND_MODE_DOLBYSURROUND:     return cString::sprintf("%s", trVDR("yes"));
    case AUDIO_DOLBY_SURROUND_MODE_RESERVED:          return cString::sprintf("%s", tr("reserved"));
    default:                                          break;
    }
  return cString::sprintf("---");
}

cString getAC3DialogLevel(int valueP)
{
  if (valueP > 0)
     return cString::sprintf("-%d %s", valueP, tr("dB"));
  return cString::sprintf("---");
}

cString getFrequencyMHz(int valueP)
{
  double freq = valueP;
  while (freq > 20000.0) freq /= 1000.0;
  return cString::sprintf("%s %s", *dtoa(freq, "%lg"), tr("MHz"));
}

cString getAudioSamplingFreq(int valueP)
{
  switch (valueP) {
    case AUDIO_SAMPLING_FREQUENCY_INVALID:  return cString::sprintf("---");
    case AUDIO_SAMPLING_FREQUENCY_RESERVED: return cString::sprintf("%s", tr("reserved"));
    default:                                break;
    }
  return cString::sprintf("%d %s", valueP, tr("Hz"));
}

cString getAudioBitrate(double valueP, double streamP)
{
  switch ((int)streamP) {
    case AUDIO_BITRATE_INVALID:  return cString::sprintf("---");
    case AUDIO_BITRATE_RESERVED: return cString::sprintf("%s (%s)", tr("reserved"), *getBitrateKbits(valueP));
    case AUDIO_BITRATE_FREE:     return cString::sprintf("%s (%s)", tr("free"), *getBitrateKbits(valueP));
    default:                     break;
    }
  return cString::sprintf("%s (%s)", *getBitrateKbits(streamP), *getBitrateKbits(valueP));
}

cString getVideoBitrate(double valueP, double streamP)
{
  return cString::sprintf("%s (%s)", *getBitrateMbits(streamP), *getBitrateMbits(valueP));
}

cString getBitrateMbits(double valueP)
{
  if (valueP > 0)
     return cString::sprintf("%.2f %s", valueP / 1000000.0, tr("Mbit/s"));
  return cString::sprintf("---");
}

cString getBitrateKbits(double valueP)
{
  if (valueP > 0)
     return cString::sprintf("%.0f %s", valueP / 1000.0, tr("kbit/s"));
  return cString::sprintf("---");
}

// --- cFemonBitStream -------------------------------------------------------

uint32_t cFemonBitStream::GetUeGolomb()
{
  int n = 0;

  while (!GetBit() && (n < 32))
    n++;

  return (n ? ((1 << n) - 1) + GetBits(n) : 0);
}

int32_t cFemonBitStream::GetSeGolomb()
{
  uint32_t r = GetUeGolomb() + 1;

  return ((r & 1) ? -(r >> 1) : (r >> 1));
}

void cFemonBitStream::SkipGolomb()
{
  int n = 0;

  while (!GetBit() && (n < 32))
    n++;

  SkipBits(n);
}
