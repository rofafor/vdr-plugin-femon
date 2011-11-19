/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include "femontools.h"
#include "femonh264.h"

const cFemonH264::t_DAR cFemonH264::s_DAR[] =
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

const cFemonH264::t_SAR cFemonH264::s_SAR[] =
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

const eVideoFormat cFemonH264::s_VideoFormats[] =
{
  VIDEO_FORMAT_COMPONENT,
  VIDEO_FORMAT_PAL,
  VIDEO_FORMAT_NTSC,
  VIDEO_FORMAT_SECAM,
  VIDEO_FORMAT_MAC,
  VIDEO_FORMAT_UNKNOWN,
  VIDEO_FORMAT_RESERVED
};

const uint8_t cFemonH264::s_SeiNumClockTsTable[9] =
{
  1, 1, 1, 2, 2, 3, 3, 2, 3
};

cFemonH264::cFemonH264(cFemonVideoIf *videohandler)
: m_VideoHandler(videohandler),
  m_Width(0),
  m_Height(0),
  m_AspectRatio(VIDEO_ASPECT_RATIO_INVALID),
  m_Format(VIDEO_FORMAT_INVALID),
  m_FrameRate(0),
  m_BitRate(0),
  m_Scan(VIDEO_SCAN_INVALID),
  m_CpbDpbDelaysPresentFlag(false),
  m_PicStructPresentFlag(false),
  m_FrameMbsOnlyFlag(false),
  m_MbAdaptiveFrameFieldFlag(false),
  m_TimeOffsetLength(0)
{
  reset();
}

cFemonH264::~cFemonH264()
{
}

bool cFemonH264::processVideo(const uint8_t *buf, int len)
{
  uint8_t nal_data[len];
  bool aud_found = false, sps_found = false, sei_found = true; // SEI temporarily disabled!
  const uint8_t *start = buf;
  const uint8_t *end = start + len;

  if (!m_VideoHandler)
     return false;

  // skip PES header
  if (!PesLongEnough(len))
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
                       //debug("H.264: Found NAL AUD at offset %d/%d\n", int(buf - start), len);
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
               //debug("H.264: Found NAL SPS at offset %d/%d\n", int(buf - start), len);
               int nal_len = nalUnescape(nal_data, buf + 4, int(end - buf - 4));
               consumed = parseSPS(nal_data, nal_len);
               if (consumed > 0)
                  sps_found = true;
             }
             break;

        case NAL_SEI:
             if (!sei_found) {
               //debug("H.264: Found NAL SEI at offset %d/%d\n", int(buf - start), len);
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
     m_VideoHandler->SetVideoCodec(VIDEO_CODEC_H264);
     if (sps_found) {
        //debug("H.264 SPS: size %dx%d, aspect %d format %d framerate %.2f bitrate %.0f\n", m_Width, m_Height, m_AspectRatio, m_Format, m_FrameRate, m_BitRate);
        m_VideoHandler->SetVideoFormat(m_Format);
        m_VideoHandler->SetVideoSize(m_Width, m_Height);
        m_VideoHandler->SetVideoAspectRatio(m_AspectRatio);
        m_VideoHandler->SetVideoFramerate(m_FrameRate);
        m_VideoHandler->SetVideoBitrate(m_BitRate);
        }
     if (sei_found) {
        //debug("H.264 SEI: scan %d\n", m_Scan);
        m_VideoHandler->SetVideoScan(m_Scan);
        }
  }

  return aud_found;
}

void cFemonH264::reset()
{
  m_CpbDpbDelaysPresentFlag = false;
  m_PicStructPresentFlag = false;
  m_FrameMbsOnlyFlag = false;
  m_MbAdaptiveFrameFieldFlag = false;
  m_TimeOffsetLength = 0;
}

const uint8_t *cFemonH264::nextStartCode(const uint8_t *start, const uint8_t *end)
{
  for (end -= 3; start < end; ++start) {
      if ((start[0] == 0x00) && (start[1] == 0x00) && (start[2] == 0x01))
         return start;
      }
  return (end + 3);
}

int cFemonH264::nalUnescape(uint8_t *dst, const uint8_t *src, int len)
{
  int s = 0, d = 0;

  while (s < len) {
    if (!src[s] && !src[s + 1]) {
       // hit 00 00 xx
       dst[d] = dst[d + 1] = 0;
       s += 2;
       d += 2;
       if (src[s] == 3) {
          s++; // 00 00 03 xx --> 00 00 xx
          if (s >= len)
             return d;
          }
       }
    dst[d++] = src[s++];
    }

  return d;
}

int cFemonH264::parseSPS(const uint8_t *buf, int len)
{
  int profile_idc, level_idc, constraint_set3_flag, pic_order_cnt_type, i, j;
  cFemonBitStream bs(buf, len);

  uint32_t width = m_Width;
  uint32_t height = m_Height;
  eVideoAspectRatio aspect_ratio = m_AspectRatio;
  eVideoFormat format = m_Format;
  double frame_rate = m_FrameRate;
  double bit_rate = m_BitRate;
  bool cpb_dpb_delays_present_flag = m_CpbDpbDelaysPresentFlag;
  bool pic_struct_present_flag = m_PicStructPresentFlag;
  bool frame_mbs_only_flag = m_FrameMbsOnlyFlag;
  bool mb_adaptive_frame_field_flag = m_MbAdaptiveFrameFieldFlag;
  uint32_t time_offset_length = m_TimeOffsetLength;

  profile_idc = bs.GetBits(8);              // profile_idc
  bs.SkipBit();                             // constraint_set0_flag
  bs.SkipBit();                             // constraint_set1_flag
  bs.SkipBit();                             // constraint_set2_flag
  constraint_set3_flag = bs.GetBit();       // constraint_set3_flag
  bs.SkipBits(4);                           // reserved_zero_4bits
  level_idc = bs.GetBits(8);                // level_idc
  bs.SkipUeGolomb();                        // seq_parameter_set_id
  //debug("H.264 SPS: profile_idc %d level_idc %d\n", profile_idc, level_idc);
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
  //debug("H.264 SPS: pic_width:  %u mbs\n", width);
  //debug("H.264 SPS: pic_height: %u mbs\n", height);
  //debug("H.264 SPS: frame only flag: %d\n", frame_mbs_only_flag);
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
     //debug("H.264 SPS: cropping %d %d %d %d\n", crop_left, crop_top, crop_right, crop_bottom);
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
        //debug("H.264 SPS: aspect_ratio_idc %d\n", aspect_ratio_idc);
        if (aspect_ratio_idc == 255) {      // extended sar
           sar_width  = bs.GetBits(16);     // sar_width
           sar_height = bs.GetBits(16);     // sar_height
           }
        else if (aspect_ratio_idc < ELEMENTS(s_SAR)) {
           sar_width  = s_SAR[aspect_ratio_idc].w;
           sar_height = s_SAR[aspect_ratio_idc].h;
           }
        if (sar_width && sar_height) {
           int index = -1, ratio = int(100.0L * sar_width * width / sar_height / height);
           for (unsigned int i = 0; i < ELEMENTS(s_DAR); ++i) {
               if (s_DAR[i].ratio == ratio) {
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
              aspect_ratio = s_DAR[index].dar;
           //debug("H.264 SPS: DAR %dx%d (%d)\n", sar_width, sar_height, aspect_ratio);
           }
        }
     if (bs.GetBit())                       // overscan_info_present_flag
        bs.SkipBit();                       // overscan_approriate_flag
     if (bs.GetBit()) {                     // video_signal_type_present_flag
        uint32_t video_format;
        video_format = bs.GetBits(3);       // video_format
        if (video_format < sizeof(s_VideoFormats) / sizeof(s_VideoFormats[0])) {
           format = s_VideoFormats[video_format];
           //debug("H.264 SPS: video format %d\n", format);
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

  m_Width = width;
  m_Height = height;
  m_AspectRatio = aspect_ratio;
  m_Format = format;
  m_FrameRate = frame_rate;
  m_BitRate = bit_rate;
  m_CpbDpbDelaysPresentFlag = cpb_dpb_delays_present_flag;
  m_PicStructPresentFlag = pic_struct_present_flag;
  m_FrameMbsOnlyFlag = frame_mbs_only_flag;
  m_MbAdaptiveFrameFieldFlag = mb_adaptive_frame_field_flag;
  m_TimeOffsetLength = time_offset_length;

  return (bs.Index() / 8);
}

int cFemonH264::parseSEI(const uint8_t *buf, int len)
{
  int num_referenced_subseqs, i;
  cFemonBitStream bs(buf, len);

  eVideoScan scan = m_Scan;

  while ((bs.Index() * 8 + 16) < len) {               // sei_message
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
           if (m_CpbDpbDelaysPresentFlag) {              // cpb_dpb_delays_present_flag
              bs.SkipUeGolomb();                         // cpb_removal_delay
              bs.SkipUeGolomb();                         // dpb_output_delay
              }
           if (m_PicStructPresentFlag) {                 // pic_struct_present_flag
              uint32_t pic_struct;
              pic_struct = bs.GetBits(4);                // pic_struct
              if (pic_struct >= (sizeof(s_SeiNumClockTsTable)) / sizeof(s_SeiNumClockTsTable[0]))
                 return 0;
              if (m_FrameMbsOnlyFlag && !m_MbAdaptiveFrameFieldFlag)
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
              //debug("H.264 SEI: pic struct %d scan type %d\n", pic_struct, scan);
              for (int i = 0; i < s_SeiNumClockTsTable[pic_struct]; ++i) {
                  if (bs.GetBit()) {                     // clock_timestamp_flag[i]
                     int full_timestamp_flag;
                     switch (bs.GetBits(2)) {            // ct_type
                       case 0:
                            scan = VIDEO_SCAN_PROGRESSIVE;
                            break;
                       case 1:
                            scan = VIDEO_SCAN_INTERLACED;
                            break;
                       case 2:
                            scan = VIDEO_SCAN_UNKNOWN;
                            break;
                       default:
                            scan = VIDEO_SCAN_RESERVED;
                            break;
                       }
                     //debug("H.264 SEI: scan type %d\n", scan);
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
                     if (m_TimeOffsetLength > 0)
                        bs.SkipBits(m_TimeOffsetLength); // time_offset
                     }
                  }
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

  m_Scan = scan;

  return (bs.Index() / 8);
}
