/*
 * h264.c: Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include "log.h"
#include "tools.h"
#include "h264.h"

const cFemonH264::t_DAR cFemonH264::darS[] =
{
  {  VIDEO_ASPECT_RATIO_1_1,      100  },
  {  VIDEO_ASPECT_RATIO_4_3,      133  },
  {  VIDEO_ASPECT_RATIO_16_9,     177  },
  {  VIDEO_ASPECT_RATIO_2_21_1,   221  },
  {  VIDEO_ASPECT_RATIO_12_11,    109  },
  {  VIDEO_ASPECT_RATIO_10_11,    90   },
  {  VIDEO_ASPECT_RATIO_16_11,    145  },
  {  VIDEO_ASPECT_RATIO_40_33,    121  },
  {  VIDEO_ASPECT_RATIO_24_11,    218  },
  {  VIDEO_ASPECT_RATIO_20_11,    181  },
  {  VIDEO_ASPECT_RATIO_32_11,    290  },
  {  VIDEO_ASPECT_RATIO_80_33,    242  },
  {  VIDEO_ASPECT_RATIO_18_11,    163  },
  {  VIDEO_ASPECT_RATIO_15_11,    136  },
  {  VIDEO_ASPECT_RATIO_64_33,    193  },
  {  VIDEO_ASPECT_RATIO_160_99,   161  },
  {  VIDEO_ASPECT_RATIO_3_2,      150  },
  {  VIDEO_ASPECT_RATIO_2_1,      200  }
};

const cFemonH264::t_SAR cFemonH264::sarS[] =
{
  { 0,   0  }, // VIDEO_ASPECT_RATIO_INVALID
  { 1,   1  }, // VIDEO_ASPECT_RATIO_1_1
  { 12,  11 }, // VIDEO_ASPECT_RATIO_12_11
  { 10,  11 }, // VIDEO_ASPECT_RATIO_10_11
  { 16,  11 }, // VIDEO_ASPECT_RATIO_16_11
  { 40,  33 }, // VIDEO_ASPECT_RATIO_40_33
  { 24,  11 }, // VIDEO_ASPECT_RATIO_24_11
  { 20,  11 }, // VIDEO_ASPECT_RATIO_20_11
  { 32,  11 }, // VIDEO_ASPECT_RATIO_32_11
  { 80,  33 }, // VIDEO_ASPECT_RATIO_80_33
  { 18,  11 }, // VIDEO_ASPECT_RATIO_18_11
  { 15,  11 }, // VIDEO_ASPECT_RATIO_15_11
  { 64,  33 }, // VIDEO_ASPECT_RATIO_64_33
  { 160, 99 }, // VIDEO_ASPECT_RATIO_160_99
  { 4,   3  }, // VIDEO_ASPECT_RATIO_4_3
  { 3,   2  }, // VIDEO_ASPECT_RATIO_3_2
  { 2,   1  }  // VIDEO_ASPECT_RATIO_2_1
};

const eVideoFormat cFemonH264::videoFormatS[] =
{
  VIDEO_FORMAT_COMPONENT,
  VIDEO_FORMAT_PAL,
  VIDEO_FORMAT_NTSC,
  VIDEO_FORMAT_SECAM,
  VIDEO_FORMAT_MAC,
  VIDEO_FORMAT_UNKNOWN,
  VIDEO_FORMAT_RESERVED
};

const uint8_t cFemonH264::seiNumClockTsTableS[9] =
{
  1, 1, 1, 2, 2, 3, 3, 2, 3
};

cFemonH264::cFemonH264(cFemonVideoIf *videoHandlerP)
: videoHandlerM(videoHandlerP),
  widthM(0),
  heightM(0),
  aspectRatioM(VIDEO_ASPECT_RATIO_INVALID),
  formatM(VIDEO_FORMAT_INVALID),
  frameRateM(0),
  bitRateM(0),
  scanM(VIDEO_SCAN_INVALID),
  cpbDpbDelaysPresentFlagM(false),
  picStructPresentFlagM(false),
  frameMbsOnlyFlagM(false),
  mbAdaptiveFrameFieldFlagM(false),
  timeOffsetLengthM(0)
{
  reset();
}

cFemonH264::~cFemonH264()
{
}

bool cFemonH264::processVideo(const uint8_t *bufP, int lenP)
{
  uint8_t nal_data[lenP];
  bool aud_found = false, sps_found = false, sei_found = true; // SEI temporarily disabled!
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

      switch (buf[3] & 0x1F) {
        case NAL_AUD:
             if (!aud_found) {
                 switch (buf[4] >> 5) {
                   case 0: case 3: case 5: // I_FRAME
                       debug2("%s Found NAL AUD at offset %d/%d", __PRETTY_FUNCTION__, int(buf - start), lenP);
                       aud_found = true;
                       break;
                   case 1: case 4: case 6: // P_FRAME;
                   case 2: case 7:         // B_FRAME;
                   default:                // NO_PICTURE;
                        break;
                   }
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

        case NAL_SEI:
             if (!sei_found) {
               debug2("%s Found NAL SEI at offset %d/%d", __PRETTY_FUNCTION__, int(buf - start), lenP);
               int nal_len = nalUnescape(nal_data, buf + 4, int(end - buf - 4));
               consumed = parseSEI(nal_data, nal_len);
               if (consumed > 0)
                  sei_found = true;
             }
             break;

        default:
             break;
        }

      if (aud_found && sps_found && sei_found)
         break;

      buf += consumed + 4;
      }

  if (aud_found) {
     videoHandlerM->SetVideoCodec(VIDEO_CODEC_H264);
     if (sps_found) {
        debug2("%s width=%d height=%d, aspect=%d format=%d bitrate=%.0f", __PRETTY_FUNCTION__, widthM, heightM, aspectRatioM, formatM, bitRateM);
        videoHandlerM->SetVideoFormat(formatM);
        videoHandlerM->SetVideoSize(widthM, heightM);
        videoHandlerM->SetVideoAspectRatio(aspectRatioM);
        videoHandlerM->SetVideoBitrate(bitRateM);
        }
     if (sps_found || sei_found) {
        debug2("%s scan=%d framerate=%.2f", __PRETTY_FUNCTION__, scanM, (scanM == VIDEO_SCAN_PROGRESSIVE) ? (frameRateM / 2) : frameRateM);
        videoHandlerM->SetVideoScan(scanM);
        videoHandlerM->SetVideoFramerate((scanM == VIDEO_SCAN_PROGRESSIVE) ? (frameRateM / 2) : frameRateM);
        }
  }

  return aud_found;
}

void cFemonH264::reset()
{
  cpbDpbDelaysPresentFlagM = false;
  picStructPresentFlagM = false;
  frameMbsOnlyFlagM = false;
  mbAdaptiveFrameFieldFlagM = false;
  timeOffsetLengthM = 0;
}

const uint8_t *cFemonH264::nextStartCode(const uint8_t *startP, const uint8_t *endP)
{
  for (endP -= 3; startP < endP; ++startP) {
      if ((startP[0] == 0x00) && (startP[1] == 0x00) && (startP[2] == 0x01))
         return startP;
      }
  return (endP + 3);
}

int cFemonH264::nalUnescape(uint8_t *dstP, const uint8_t *srcP, int lenP)
{
  int s = 0, d = 0;

  while (s < lenP - 3) {
    if (!srcP[s] && !srcP[s + 1] && srcP[s + 2] == 3) {
       dstP[d++] = srcP[s++];
       dstP[d++] = srcP[s++];
       s++; // skip emulation_prevention_three_byte
       }
    else
       dstP[d++] = srcP[s++];
    }

  while (s < lenP)
    dstP[d++] = srcP[s++];

  return d;
}

int cFemonH264::parseSPS(const uint8_t *bufP, int lenP)
{
  int profile_idc, level_idc, constraint_set3_flag, pic_order_cnt_type, i, j;
  cFemonBitStream bs(bufP, lenP);

  uint32_t width = widthM;
  uint32_t height = heightM;
  eVideoAspectRatio aspect_ratio = aspectRatioM;
  eVideoFormat format = formatM;
  double frame_rate = frameRateM;
  double bit_rate = bitRateM;
  bool cpb_dpb_delays_present_flag = cpbDpbDelaysPresentFlagM;
  bool pic_struct_present_flag = picStructPresentFlagM;
  bool frame_mbs_only_flag = frameMbsOnlyFlagM;
  bool mb_adaptive_frame_field_flag = mbAdaptiveFrameFieldFlagM;
  uint32_t time_offset_length = timeOffsetLengthM;

  profile_idc = bs.GetBits(8);              // profile_idc
  bs.SkipBit();                             // constraint_set0_flag
  bs.SkipBit();                             // constraint_set1_flag
  bs.SkipBit();                             // constraint_set2_flag
  constraint_set3_flag = bs.GetBit();       // constraint_set3_flag
  bs.SkipBits(4);                           // reserved_zero_4bits
  level_idc = bs.GetBits(8);                // level_idc
  bs.SkipUeGolomb();                        // seq_parameter_set_id
  debug2("%s profile_idc=%d level_idc=%d", __PRETTY_FUNCTION__, profile_idc, level_idc);
  switch (profile_idc) {
    case 66:                                // baseline profile
    case 77:                                // main profile
    case 88:                                // extended profile
         switch (level_idc) {
           case 10:                         // level 1.0
                bit_rate = 64000;
                break;
           case 11:                         // level 1b / 1.1
                bit_rate = constraint_set3_flag ? 128000 : 192000;
                break;
           case 12:                         // level 1.2
                bit_rate = 384000;
                break;
           case 13:                         // level 1.3
                bit_rate = 768000;
                break;
           case 20:                         // level 2.0
                bit_rate = 2000000;
                break;
           case 21:                         // level 2.1
                bit_rate = 4000000;
                break;
           case 22:                         // level 2.2
                bit_rate = 4000000;
                break;
           case 30:                         // level 3.0
                bit_rate = 10000000;
                break;
           case 31:                         // level 3.1
                bit_rate = 14000000;
                break;
           case 32:                         // level 3.2
                bit_rate = 20000000;
                break;
           case 40:                         // level 4.0
                bit_rate = 20000000;
                break;
           case 41:                         // level 4.1
                bit_rate = 50000000;
                break;
           case 42:                         // level 4.2
                bit_rate = 50000000;
                break;
           case 50:                         // level 5.0
                bit_rate = 135000000;
                break;
           case 51:                         // level 5.1
                bit_rate = 240000000;
                break;
           default:
                break;
         }
         break;
    case 100:                               // high profile
         switch (level_idc) {
           case 10:                         // level 1.0
                bit_rate = 80000;
                break;
           case 11:                         // level 1b / 1.1
                bit_rate = constraint_set3_flag ? 160000 : 240000;
                break;
           case 12:                         // level 1.2
                bit_rate = 480000;
                break;
           case 13:                         // level 1.3
                bit_rate = 960000;
                break;
           case 20:                         // level 2.0
                bit_rate = 2500000;
                break;
           case 21:                         // level 2.1
                bit_rate = 5000000;
                break;
           case 22:                         // level 2.2
                bit_rate = 5000000;
                break;
           case 30:                         // level 3.0
                bit_rate = 12500000;
                break;
           case 31:                         // level 3.1
                bit_rate = 17500000;
                break;
           case 32:                         // level 3.2
                bit_rate = 25000000;
                break;
           case 40:                         // level 4.0
                bit_rate = 25000000;
                break;
           case 41:                         // level 4.1
                bit_rate = 62500000;
                break;
           case 42:                         // level 4.2
                bit_rate = 62500000;
                break;
           case 50:                         // level 5.0
                bit_rate = 168750000;
                break;
           case 51:                         // level 5.1
                bit_rate = 300000000;
                break;
           default:
                break;
         }
         break;
    case 110:                               // high 10 profile
         switch (level_idc) {
           case 10:                         // level 1.0
                bit_rate = 192000;
                break;
           case 11:                         // level 1b / 1.1
                bit_rate = constraint_set3_flag ? 384000 : 576000;
                break;
           case 12:                         // level 1.2
                bit_rate = 115200;
                break;
           case 13:                         // level 1.3
                bit_rate = 2304000;
                break;
           case 20:                         // level 2.0
                bit_rate = 6000000;
                break;
           case 21:                         // level 2.1
                bit_rate = 12000000;
                break;
           case 22:                         // level 2.2
                bit_rate = 12000000;
                break;
           case 30:                         // level 3.0
                bit_rate = 30000000;
                break;
           case 31:                         // level 3.1
                bit_rate = 42000000;
                break;
           case 32:                         // level 3.2
                bit_rate = 60000000;
                break;
           case 40:                         // level 4.0
                bit_rate = 60000000;
                break;
           case 41:                         // level 4.1
                bit_rate = 150000000;
                break;
           case 42:                         // level 4.2
                bit_rate = 150000000;
                break;
           case 50:                         // level 5.0
                bit_rate = 405000000;
                break;
           case 51:                         // level 5.1
                bit_rate = 720000000;
                break;
           default:
                break;
         }
         break;
    case 122:                               // high 4:2:2 profile
    case 144:                               // high 4:4:4 profile
         switch (level_idc) {
           case 10:                         // level 1.0
                bit_rate = 256000;
                break;
           case 11:                         // level 1b / 1.1
                bit_rate = constraint_set3_flag ? 512000 : 768000;
                break;
           case 12:                         // level 1.2
                bit_rate = 1536000;
                break;
           case 13:                         // level 1.3
                bit_rate = 3072000;
                break;
           case 20:                         // level 2.0
                bit_rate = 8000000;
                break;
           case 21:                         // level 2.1
                bit_rate = 16000000;
                break;
           case 22:                         // level 2.2
                bit_rate = 16000000;
                break;
           case 30:                         // level 3.0
                bit_rate = 40000000;
                break;
           case 31:                         // level 3.1
                bit_rate = 56000000;
                break;
           case 32:                         // level 3.2
                bit_rate = 80000000;
                break;
           case 40:                         // level 4.0
                bit_rate = 80000000;
                break;
           case 41:                         // level 4.1
                bit_rate = 200000000;
                break;
           case 42:                         // level 4.2
                bit_rate = 200000000;
                break;
           case 50:                         // level 5.0
                bit_rate = 540000000;
                break;
           case 51:                         // level 5.1
                bit_rate = 960000000;
                break;
           default:
                break;
         }
         break;
    default:
         break;
  }
  if ((profile_idc == 100) || (profile_idc == 110) || (profile_idc == 122) || (profile_idc == 144)) {
     if (bs.GetUeGolomb() == 3)             // chroma_format_idc
        bs.SkipBit();                       // residual_colour_transform_flag
     bs.SkipUeGolomb();                     // bit_depth_luma_minus8
     bs.SkipUeGolomb();                     // bit_depth_chroma_minus8
     bs.SkipBit();                          // qpprime_y_zero_transform_bypass_flag
     if (bs.GetBit()) {                     // seq_scaling_matrix_present_flag
        for (i = 0; i < 8; ++i) {
            if (bs.GetBit()) {              // seq_scaling_list_present_flag[i]
               int last = 8, next = 8, size = (i < 6) ? 16 : 64;
               for (j = 0; j < size; ++j) {
                   if (next)
                      next = (last + bs.GetSeGolomb()) & 0xff;
                   last = next ?: last;
                   }
               }
            }
        }
     }
  bs.SkipUeGolomb();                        // log2_max_frame_num_minus4
  pic_order_cnt_type = bs.GetUeGolomb();    // pic_order_cnt_type
  if (pic_order_cnt_type == 0)
     bs.SkipUeGolomb();                     // log2_max_pic_order_cnt_lsb_minus4
  else if (pic_order_cnt_type == 1) {
     bs.SkipBit();                          // delta_pic_order_always_zero
     bs.SkipSeGolomb();                     // offset_for_non_ref_pic
     bs.SkipSeGolomb();                     // offset_for_top_to_bottom_field
     j = bs.GetUeGolomb();                  // num_ref_frames_in_pic_order_cnt_cycle
     for (i = 0; i < j; ++i)
         bs.SkipSeGolomb();                 // offset_for_ref_frame[i]
     }
  bs.SkipUeGolomb();                        // num_ref_frames
  bs.SkipBit();                             // gaps_in_frame_num_value_allowed_flag
  width  = bs.GetUeGolomb() + 1;            // pic_width_in_mbs_minus1
  height = bs.GetUeGolomb() + 1;            // pic_height_in_mbs_minus1
  frame_mbs_only_flag = bs.GetBit();        // frame_mbs_only_flag
  debug2("%s pic_width=%u", __PRETTY_FUNCTION__, width);
  debug2("%s pic_height=%u", __PRETTY_FUNCTION__, height);
  debug2("%s frame_mbs_only_flag=%d", __PRETTY_FUNCTION__, frame_mbs_only_flag);
  width  *= 16;
  height *= 16 * (frame_mbs_only_flag ? 1 : 2);
  if (!frame_mbs_only_flag)
     mb_adaptive_frame_field_flag = bs.GetBit(); // mb_adaptive_frame_field_flag
  bs.SkipBit();                             // direct_8x8_inference_flag
  if (bs.GetBit()) {                        // frame_cropping_flag
     uint32_t crop_left, crop_right, crop_top, crop_bottom;
     crop_left   = bs.GetUeGolomb();        // frame_crop_left_offset
     crop_right  = bs.GetUeGolomb();        // frame_crop_rigth_offset
     crop_top    = bs.GetUeGolomb();        // frame_crop_top_offset
     crop_bottom = bs.GetUeGolomb();        // frame_crop_bottom_offset
     debug2("%s crop_left=%d crop_top=%d crop_right=%d crop_bottom=%d", __PRETTY_FUNCTION__, crop_left, crop_top, crop_right, crop_bottom);
     width -= 2 * (crop_left + crop_right);
     if (frame_mbs_only_flag)
        height -= 2 * (crop_top + crop_bottom);
     else
        height -= 4 * (crop_top + crop_bottom);
     }
  // VUI parameters
  if (bs.GetBit()) {                        // vui_parameters_present_flag
     if (bs.GetBit()) {                     // aspect_ratio_info_present
        uint32_t aspect_ratio_idc, sar_width = 0, sar_height = 0;
        aspect_ratio_idc = bs.GetBits(8);   // aspect_ratio_idc
        debug2("%s aspect_ratio_idc=%d", __PRETTY_FUNCTION__, aspect_ratio_idc);
        if (aspect_ratio_idc == 255) {      // extended sar
           sar_width  = bs.GetBits(16);     // sar_width
           sar_height = bs.GetBits(16);     // sar_height
           }
        else if (aspect_ratio_idc < ELEMENTS(sarS)) {
           sar_width  = sarS[aspect_ratio_idc].w;
           sar_height = sarS[aspect_ratio_idc].h;
           }
        if (sar_width && sar_height) {
           int index = -1, ratio = int(100.0L * sar_width * width / sar_height / height);
           for (unsigned int i = 0; i < ELEMENTS(darS); ++i) {
               if (darS[i].ratio == ratio) {
                  index = i;
                  break;
                  }
               }
           if (index < 0) {
              if (aspect_ratio_idc == 255)
                 aspect_ratio = VIDEO_ASPECT_RATIO_EXTENDED;
              else
                 aspect_ratio = VIDEO_ASPECT_RATIO_INVALID;
              }
           else
              aspect_ratio = darS[index].dar;
           debug2("%s sar_width=%d sar_height=%d aspect_ratio=%d", __PRETTY_FUNCTION__, sar_width, sar_height, aspect_ratio);
           }
        }
     if (bs.GetBit())                       // overscan_info_present_flag
        bs.SkipBit();                       // overscan_approriate_flag
     if (bs.GetBit()) {                     // video_signal_type_present_flag
        uint32_t video_format;
        video_format = bs.GetBits(3);       // video_format
        if (video_format < ELEMENTS(videoFormatS)) {
           format = videoFormatS[video_format];
           debug2("%s video_format=%d", __PRETTY_FUNCTION__, format);
           }
        bs.SkipBit();                       // video_full_range_flag
        if (bs.GetBit()) {                  // colour_description_present_flag
           bs.SkipBits(8);                  // colour_primaries
           bs.SkipBits(8);                  // transfer_characteristics
           bs.SkipBits(8);                  // matrix_coefficients
           }
        }
     if (bs.GetBit()) {                     // chroma_loc_info_present_flag
        bs.SkipUeGolomb();                  // chroma_sample_loc_type_top_field
        bs.SkipUeGolomb();                  // chroma_sample_loc_type_bottom_field
        }
     if (bs.GetBit()) {                     // timing_info_present_flag
        uint32_t num_units_in_tick, time_scale;
        num_units_in_tick = bs.GetBits(32); // num_units_in_tick
        time_scale        = bs.GetBits(32); // time_scale
        if (num_units_in_tick > 0)
           frame_rate = time_scale / num_units_in_tick;
        bs.SkipBit();                       // fixed_frame_rate_flag
        }
     int nal_hrd_parameters_present_flag = bs.GetBit(); // nal_hrd_parameters_present_flag
     if (nal_hrd_parameters_present_flag) {
        int cpb_cnt_minus1;
        cpb_cnt_minus1 = bs.GetUeGolomb();  // cpb_cnt_minus1
        bs.SkipBits(4);                     // bit_rate_scale
        bs.SkipBits(4);                     // cpb_size_scale
        for (int i = 0; i < cpb_cnt_minus1; ++i) {
            bs.SkipUeGolomb();              // bit_rate_value_minus1[i]
            bs.SkipUeGolomb();              // cpb_size_value_minus1[i]
            bs.SkipBit();                   // cbr_flag[i]
            }
        bs.SkipBits(5);                     // initial_cpb_removal_delay_length_minus1
        bs.SkipBits(5);                     // cpb_removal_delay_length_minus1
        bs.SkipBits(5);                     // dpb_output_delay_length_minus1
        time_offset_length = bs.GetBits(5); // time_offset_length
        }
     int vlc_hrd_parameters_present_flag = bs.GetBit(); // vlc_hrd_parameters_present_flag
     if (vlc_hrd_parameters_present_flag) {
         int cpb_cnt_minus1;
         cpb_cnt_minus1 = bs.GetUeGolomb(); // cpb_cnt_minus1
         bs.SkipBits(4);                    // bit_rate_scale
         bs.SkipBits(4);                    // cpb_size_scale
         for (int i = 0; i < cpb_cnt_minus1; ++i) {
             bs.SkipUeGolomb();             // bit_rate_value_minus1[i]
             bs.SkipUeGolomb();             // cpb_size_value_minus1[i]
             bs.SkipBit();                  // cbr_flag[i]
             }
         bs.SkipBits(5);                    // initial_cpb_removal_delay_length_minus1
         bs.SkipBits(5);                    // cpb_removal_delay_length_minus1
         bs.SkipBits(5);                    // dpb_output_delay_length_minus1
         time_offset_length = bs.GetBits(5);// time_offset_length
        }
     cpb_dpb_delays_present_flag = (nal_hrd_parameters_present_flag | vlc_hrd_parameters_present_flag);
     if (cpb_dpb_delays_present_flag)
        bs.SkipBit();                       // low_delay_hrd_flag
     pic_struct_present_flag = bs.GetBit(); // pic_struct_present_flag
     if (bs.GetBit()) {                     // bitstream_restriction_flag
        bs.SkipBit();                       // motion_vectors_over_pic_boundaries_flag
        bs.SkipUeGolomb();                  // max_bytes_per_pic_denom
        bs.SkipUeGolomb();                  // max_bits_per_mb_denom
        bs.SkipUeGolomb();                  // log2_max_mv_length_horizontal
        bs.SkipUeGolomb();                  // log2_max_mv_length_vertical
        bs.SkipUeGolomb();                  // num_reorder_frames
        bs.SkipUeGolomb();                  // max_dec_frame_buffering
        }
     }

  widthM = width;
  heightM = height;
  aspectRatioM = aspect_ratio;
  formatM = format;
  scanM = frame_mbs_only_flag ? VIDEO_SCAN_PROGRESSIVE : VIDEO_SCAN_INTERLACED;
  frameRateM = frame_rate;
  bitRateM = bit_rate;
  cpbDpbDelaysPresentFlagM = cpb_dpb_delays_present_flag;
  picStructPresentFlagM = pic_struct_present_flag;
  frameMbsOnlyFlagM = frame_mbs_only_flag;
  mbAdaptiveFrameFieldFlagM = mb_adaptive_frame_field_flag;
  timeOffsetLengthM = time_offset_length;

  return (bs.Index() / 8);
}

int cFemonH264::parseSEI(const uint8_t *bufP, int lenP)
{
  int num_referenced_subseqs, i;
  cFemonBitStream bs(bufP, lenP);

  eVideoScan scan = scanM;

  while ((bs.Index() * 8 + 16) < lenP) {                 // sei_message
    int lastByte, payloadSize = 0, payloadType = 0;

    do {
       lastByte = bs.GetBits(8) & 0xFF;
       payloadType += lastByte;
    } while (lastByte == 0xFF);                          // last_payload_type_byte

    do {
       lastByte = bs.GetBits(8) & 0xFF;
       payloadSize += lastByte;
    } while (lastByte == 0xFF);                          // last_payload_size_byte

    switch (payloadType) {                               // sei_payload
      case 1:                                            // pic_timing
           if (cpbDpbDelaysPresentFlagM) {               // cpb_dpb_delays_present_flag
              bs.SkipUeGolomb();                         // cpb_removal_delay
              bs.SkipUeGolomb();                         // dpb_output_delay
              }
           if (picStructPresentFlagM) {                  // pic_struct_present_flag
              uint32_t pic_struct, ct_type = 0, i = 0;
              pic_struct = bs.GetBits(4);                // pic_struct
              if (pic_struct >= ELEMENTS(seiNumClockTsTableS))
                 return 0;
              if (frameMbsOnlyFlagM && !mbAdaptiveFrameFieldFlagM)
                 scan = VIDEO_SCAN_PROGRESSIVE;
              else {
                 switch (pic_struct) {
                   case 0:                               // frame
                   case 7:                               // frame doubling
                   case 8:                               // frame tripling
                        scan = VIDEO_SCAN_PROGRESSIVE;
                        break;
                   case 1:                               // top
                   case 2:                               // bottom
                   case 3:                               // top bottom
                   case 4:                               // bottom top
                   case 5:                               // top bottom top
                   case 6:                               // bottom top bottom
                        scan = VIDEO_SCAN_INTERLACED;
                        break;
                   default:
                        scan = VIDEO_SCAN_RESERVED;
                        break;
                   }
                }
              debug2("%s pic_struct=%d scan_type=%d", __PRETTY_FUNCTION__, pic_struct, scan);
              for (i = 0; i < seiNumClockTsTableS[pic_struct]; ++i) {
                  if (bs.GetBit()) {                     // clock_timestamp_flag[i]
                     int full_timestamp_flag;
                     ct_type |= (1 << bs.GetBits(2));    // ct_type
                     debug2("%s ct_type=%04X", __PRETTY_FUNCTION__, ct_type);
                     bs.SkipBit();                       // nuit_field_based_flag
                     bs.SkipBits(5);                     // counting_type
                     full_timestamp_flag = bs.GetBit();  // full_timestamp_flag
                     bs.SkipBit();                       // discontinuity_flag
                     bs.SkipBit();                       // cnt_dropped_flag
                     bs.SkipBits(8);                     // n_frames
                     if (full_timestamp_flag) {
                        bs.SkipBits(6);                  // seconds_value
                        bs.SkipBits(6);                  // minutes_value
                        bs.SkipBits(5);                  // hours_value
                        }
                     else {
                        if (bs.GetBit()) {               // seconds_flag
                           bs.SkipBits(6);               // seconds_value
                           if (bs.GetBit()) {            // minutes_flag
                              bs.SkipBits(6);            // minutes_value
                              if (bs.GetBit())           // hours_flag
                                  bs.SkipBits(5);        // hours_value
                              }
                           }
                        }
                     if (timeOffsetLengthM > 0)
                        bs.SkipBits(timeOffsetLengthM);  // time_offset
                     }
                  }
              if (i > 0)
                 scan = (ct_type & (1 << 1)) ? VIDEO_SCAN_INTERLACED : VIDEO_SCAN_PROGRESSIVE;
              }
           break;

      case 12:                                           // sub_seq_characteristics
           bs.SkipUeGolomb();                            // sub_seq_layer_num
           bs.SkipUeGolomb();                            // sub_seq_id
           if (bs.GetBit())                              // duration_flag
              bs.SkipBits(32);                           // sub_seq_duration
           if (bs.GetBit()) {                            // average_rate_flag
              bs.SkipBit();                              // accurate_statistics_flag
              bs.SkipBits(16);                           // average_bit_rate (1000 bit/s)
              bs.SkipBits(16);                           // average_frame_rate (frames per 256s)
              }
           num_referenced_subseqs = bs.GetUeGolomb();    // num_referenced_subseqs
           for (i = 0; i < num_referenced_subseqs; ++i) {
               bs.SkipUeGolomb();                        // ref_sub_seq_layer_num
               bs.SkipUeGolomb();                        // ref_sub_seq_id
               bs.GetBit();                              // ref_sub_seq_direction
               }
           break;

      default:
           bs.SkipBits(payloadSize * 8);
           break;
      }

    // force byte align
    bs.ByteAlign();
    }

  scanM = scan;

  return (bs.Index() / 8);
}
