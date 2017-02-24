/*
 * h265.c: Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include "log.h"
#include "tools.h"
#include "h265.h"

cFemonH265::cFemonH265(cFemonVideoIf *videoHandlerP)
: videoHandlerM(videoHandlerP),
  widthM(0),
  heightM(0),
  aspectRatioM(VIDEO_ASPECT_RATIO_INVALID),
  formatM(VIDEO_FORMAT_INVALID),
  frameRateM(0),
  bitRateM(0),
  scanM(VIDEO_SCAN_PROGRESSIVE)
{
  reset();
}

cFemonH265::~cFemonH265()
{
}

bool cFemonH265::processVideo(const uint8_t *bufP, int lenP)
{
  uint8_t nal_data[lenP];
  bool aud_found = false, sps_found = false;
  const uint8_t *buf = bufP;
  const uint8_t *start = buf;
  const uint8_t *end = start + lenP;

  if (!videoHandlerM)
     return false;

  // skip PES header
  if (!PesLongEnough(lenP))
      return false;
  buf += PesPayloadOffset(buf);
  start = buf;

  reset();

  for (;;) {
      int consumed = 0;

      buf = nextStartCode(buf, end);
      if (buf >= end)
         break;

      switch ((buf[3] >> 1) & 0x3F) {
        case NAL_AUD:
             if (!aud_found) {
                debug2("%s Found NAL AUD at offset %d/%d", __PRETTY_FUNCTION__, int(buf - start), lenP);
                aud_found = true;
                }
             break;

        case NAL_SPS:
             if (!sps_found) {
                debug2("%s Found NAL SPS at offset %d/%d", __PRETTY_FUNCTION__, int(buf - start), lenP);
                int nal_len = nalUnescape(nal_data, buf + 4, int(end - buf - 4));
                consumed = parseSPS(nal_data, nal_len);
                if (consumed > 0)
                   sps_found = true;
                }
             break;

        default:
             break;
        }

      if (aud_found && sps_found)
         break;

      buf += consumed + 4;
      }

     if (aud_found) {
        videoHandlerM->SetVideoCodec(VIDEO_CODEC_H265);
        if (sps_found) {
           debug2("%s width=%d height=%d, aspect=%d format=%d bitrate=%.0f scan=%d framerate=%.2f", __PRETTY_FUNCTION__, widthM, heightM, aspectRatioM, formatM, bitRateM, scanM, frameRateM);
           videoHandlerM->SetVideoSize(widthM, heightM);
           videoHandlerM->SetVideoScan(scanM);
           //videoHandlerM->SetVideoFormat(formatM);
           //videoHandlerM->SetVideoAspectRatio(aspectRatioM);
           //videoHandlerM->SetVideoBitrate(bitRateM);
           //videoHandlerM->SetVideoFramerate(frameRateM);
           }
        }

  return sps_found;
}

void cFemonH265::reset()
{
}

const uint8_t *cFemonH265::nextStartCode(const uint8_t *startP, const uint8_t *endP)
{
  for (endP -= 3; startP < endP; ++startP) {
      if ((startP[0] == 0x00) && (startP[1] == 0x00) && (startP[2] == 0x01))
         return startP;
      }
  return (endP + 3);
}

int cFemonH265::nalUnescape(uint8_t *dstP, const uint8_t *srcP, int lenP)
{
  int s = 0, d = 0;

  while (s < lenP) {
    if (!srcP[s] && !srcP[s + 1]) {
       // hit 00 00 xx
       dstP[d] = dstP[d + 1] = 0;
       s += 2;
       d += 2;
       if (srcP[s] == 3) {
          s++; // 00 00 03 xx --> 00 00 xx
          if (s >= lenP)
             return d;
          }
       }
    dstP[d++] = srcP[s++];
    }

  return d;
}

int cFemonH265::parseSPS(const uint8_t *bufP, int lenP)
{
  cFemonBitStream bs(bufP, lenP);

  uint32_t width = widthM;
  uint32_t height = heightM;
  bool profilePresentFlag = true;
  unsigned int chroma_format_idc;
  uint8_t sps_max_sub_layers_minus1;
  uint8_t sub_layer_profile_present_flag[8];
  uint8_t sub_layer_level_present_flag[8];

  bs.SkipBits(4);                                      // sps_video_parameter_set_id
  sps_max_sub_layers_minus1 = bs.GetBits(3);           // sps_max_sub_layers_minus1
  bs.SkipBit();                                        // sps_temporal_id_nesting_flag
  // start of profile_tier_level(1, sps_max_sub_layers_minus1)
  if (profilePresentFlag) {
     bs.SkipBits(2);                                   // general_profile_space
     bs.SkipBit();                                     // general_profile_space
     bs.SkipBits(5);                                   // general_profile_idc
     bs.SkipBits(32);                                  // general_profile_compatibility_flag[0-31]
     bs.SkipBit();                                     // general_progressive_source_flag
     bs.SkipBit();                                     // general_interlaced_source_flag
     bs.SkipBit();                                     // general_non_packed_constraint_flag
     bs.SkipBit();                                     // general_frame_only_constraint_flag
     // the number of bits in this syntax structure is not affected by this condition
     bs.SkipBits(43);                                  // general_reserved_zero_43bits
     // the number of bits in this syntax structure is not affected by this condition
     bs.SkipBit();                                     // general_reserved_zero_bit
     }
  bs.SkipBits(8);                                      // general_level_idc
  for (int i = 0; i < sps_max_sub_layers_minus1; ++i) {
      sub_layer_profile_present_flag[i] = bs.GetBit(); // sub_layer_profile_present_flag[i]
      sub_layer_level_present_flag[i] = bs.GetBit();   // sub_layer_level_present_flag[i]
      }
  if (sps_max_sub_layers_minus1 > 0) {
     for (int i = sps_max_sub_layers_minus1; i < 8; ++i)
         bs.SkipBits(2);                               // reserved_zero_2bits[i]
     }
  for (int i = 0; i < sps_max_sub_layers_minus1; ++i) {
      if (sub_layer_profile_present_flag[i]) {
         bs.SkipBits(2);                               // sub_layer_profile_space[i]
         bs.SkipBit();                                 // sub_layer_tier_flag[i]
         bs.SkipBits(5);                               // sub_layer_profile_idc[i]
         bs.SkipBits(32);                              // sub_layer_profile_compatibility_flag[i][0-31]
         bs.SkipBit();                                 // sub_layer_progressive_source_flag[i]
         bs.SkipBit();                                 // sub_layer_interlaced_source_flag[i]
         bs.SkipBit();                                 // sub_layer_non_packed_constraint_flag[i]
         bs.SkipBit();                                 // sub_layer_frame_only_constraint_flag[i]
         // the number of bits in this syntax structure is not affected by this condition
         bs.SkipBits(43);                              // sub_layer_reserved_zero_43bits[i]
         // the number of bits in this syntax structure is not affected by this condition
         bs.SkipBit();                                 // sub_layer_reserved_zero_bit[i]
         }
      if (sub_layer_level_present_flag[i])
         bs.SkipBits(8);                               // sub_layer_level_idc[i]
      }
  // end of profile_tier_level
  bs.SkipUeGolomb();                                   // sps_seq_parameter_set_id
  chroma_format_idc = bs.GetUeGolomb();                // chroma_format_idc
  if (chroma_format_idc == 3)
     bs.SkipBit();                                     // separate_colour_plane_flag
  width = bs.GetUeGolomb();                            // pic_width_in_luma_samples
  height = bs.GetUeGolomb();                           // pic_height_in_luma_samples

  widthM = width;
  heightM = height;

  return (bs.Index() / 8);
}
