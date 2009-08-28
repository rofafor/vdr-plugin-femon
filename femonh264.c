/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * The original NAL SPS parsing and bitstream functions are taken from
 * vdr-xineliboutput plugin by Petri Hintukainen.
 */

#include "femontools.h"
#include "femonh264.h"

const eVideoAspectRatio cFemonH264::s_AspectRatios[] =
{
  VIDEO_ASPECT_RATIO_INVALID,
  VIDEO_ASPECT_RATIO_1_1,
  VIDEO_ASPECT_RATIO_12_11,
  VIDEO_ASPECT_RATIO_10_11,
  VIDEO_ASPECT_RATIO_16_11,
  VIDEO_ASPECT_RATIO_40_33,
  VIDEO_ASPECT_RATIO_24_11,
  VIDEO_ASPECT_RATIO_20_11,
  VIDEO_ASPECT_RATIO_32_11,
  VIDEO_ASPECT_RATIO_80_33,
  VIDEO_ASPECT_RATIO_18_11,
  VIDEO_ASPECT_RATIO_15_11,
  VIDEO_ASPECT_RATIO_64_33,
  VIDEO_ASPECT_RATIO_160_99,
  VIDEO_ASPECT_RATIO_4_3,
  VIDEO_ASPECT_RATIO_3_2,
  VIDEO_ASPECT_RATIO_2_1
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
  m_TimeOffsetLength(0)
{
}

cFemonH264::~cFemonH264()
{
}

bool cFemonH264::processVideo(const uint8_t *buf, int len)
{
  uint8_t nal_data[len];
  bool aud_found = false, sps_found = false, sei_found = false;
  const uint8_t *start = buf;
  const uint8_t *end = start + len;

  if (!m_VideoHandler)
     return false;

  // skip PES header
  if (!PesLongEnough(len))
      return false;
  buf += PesPayloadOffset(buf);
  start = buf;

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
                       //Dprintf("H.264: Found NAL AUD at offset %d/%d", buf - start, len);
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
               //Dprintf("H.264: Found NAL SPS at offset %d/%d", buf - start, len);
               int nal_len = nalUnescape(nal_data, buf + 4, int(end - buf - 4));
               consumed = parseSPS(nal_data, nal_len);
               if (consumed > 0)
                  sps_found = true;
             }
             break;
#if 0
        case NAL_SEI:
             if (!sei_found) {
               //Dprintf("H.264: Found NAL SEI at offset %d/%d", buf - start, len);
               int nal_len = nalUnescape(nal_data, buf + 4, int(end - buf - 4));
               consumed = parseSEI(nal_data, nal_len);
               if (consumed > 0)
                  sei_found = true;
             }
             break;
#endif
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
        //Dprintf("H.264 SPS: -> video size %dx%d, aspect %d format %d", m_Width, m_Height, m_AspectRatio, m_Format);
        m_VideoHandler->SetVideoFormat(m_Format);
        m_VideoHandler->SetVideoSize(m_Width, m_Height);
        m_VideoHandler->SetVideoAspectRatio(m_AspectRatio);
        }
     if (sei_found) {
        //Dprintf("H.264 SEI: -> stream bitrate %.1f, frame rate %.1f scan %d", m_BitRate, m_FrameRate, m_Scan);
        m_VideoHandler->SetVideoFramerate(m_FrameRate);
        m_VideoHandler->SetVideoBitrate(m_BitRate);
        m_VideoHandler->SetVideoScan(m_Scan);
        }
  }

  return aud_found;
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
  int profile_idc, pic_order_cnt_type, frame_mbs_only, i, j;
  cBitStream bs(buf, len);

  unsigned int width = m_Width;
  unsigned int height = m_Height;
  eVideoAspectRatio aspect_ratio = m_AspectRatio;
  eVideoFormat format = m_Format;
  bool cpb_dpb_delays_present_flag = m_CpbDpbDelaysPresentFlag;
  bool pic_struct_present_flag = m_PicStructPresentFlag;
  unsigned int time_offset_length = m_TimeOffsetLength;

  profile_idc = bs.getU8();

  //Dprintf("H.264 SPS: profile_idc %d", profile_idc);

  bs.skipBits(16);
  bs.skipUeGolomb();                        // seq_parameter_set_id
  if (profile_idc >= 100) {
     if (bs.getUeGolomb() == 3)             // chroma_format_idc
        bs.skipBit();                       // residual_colour_transform_flag
     bs.skipUeGolomb();                     // bit_depth_luma - 8
     bs.skipUeGolomb();                     // bit_depth_chroma - 8
     bs.skipBit();                          // transform_bypass
     if (bs.getBit()) {                     // seq_scaling_matrix_present
        for (i = 0; i < 8; i++) {
            if (bs.getBit()) {              // seq_scaling_list_present
               int last = 8, next = 8, size = (i < 6) ? 16 : 64;
               for (j = 0; j < size; j++) {
                   if (next)
                      next = (last + bs.getSeGolomb()) & 0xff;
                   last = next ?: last;
                   }
               }
            }
        }
     }

  bs.skipUeGolomb();                        // log2_max_frame_num - 4
  pic_order_cnt_type = bs.getUeGolomb();
  if (pic_order_cnt_type == 0)
     bs.skipUeGolomb();                     // log2_max_poc_lsb - 4
  else if (pic_order_cnt_type == 1) {
     bs.skipBit();                          // delta_pic_order_always_zero
     bs.skipSeGolomb();                     // offset_for_non_ref_pic
     bs.skipSeGolomb();                     // offset_for_top_to_bottom_field
     j = bs.getUeGolomb();                  // num_ref_frames_in_pic_order_cnt_cycle
     for (i = 0; i < j; i++)
         bs.skipSeGolomb();                 // offset_for_ref_frame[i]
     }
  bs.skipUeGolomb();                        // ref_frames
  bs.skipBit();                             // gaps_in_frame_num_allowed
  width          = bs.getUeGolomb() + 1;    // mbs
  height         = bs.getUeGolomb() + 1;    // mbs
  frame_mbs_only = bs.getBit();

  //Dprintf("H.264 SPS: pic_width:  %u mbs", width);
  //Dprintf("H.264 SPS: pic_height: %u mbs", height);
  //Dprintf("H.264 SPS: frame only flag: %d", frame_mbs_only);

  width  *= 16;
  height *= 16 * (2 - frame_mbs_only);

  if (!frame_mbs_only) {
    if (bs.getBit()) {                      // mb_adaptive_frame_field_flag
       //Dprintf("H.264 SPS: MBAFF");
       }
    }

  bs.skipBit();                             // direct_8x8_inference_flag
  if (bs.getBit()) {                        // frame_cropping_flag
    uint32_t crop_left   = bs.getUeGolomb();
    uint32_t crop_right  = bs.getUeGolomb();
    uint32_t crop_top    = bs.getUeGolomb();
    uint32_t crop_bottom = bs.getUeGolomb();
    //Dprintf("H.264 SPS: cropping %d %d %d %d", crop_left, crop_top, crop_right, crop_bottom);

    width -= 2 * (crop_left + crop_right);
    if (frame_mbs_only)
       height -= 2 * (crop_top + crop_bottom);
    else
       height -= 4 * (crop_top + crop_bottom);
    }

  // VUI parameters
  if (bs.getBit()) {                        // vui_parameters_present_flag
     if (bs.getBit()) {                     // aspect_ratio_info_present
        uint32_t aspect_ratio_idc = bs.getU8();
        //Dprintf("H.264 SPS: aspect_ratio_idc %d", aspect_ratio_idc);
        if (aspect_ratio_idc == 255) {      // extended sar
           bs.skipBit();                    // sar_width
           bs.skipBit();                    // sar_height
           aspect_ratio = VIDEO_ASPECT_RATIO_EXTENDED;
           //Dprintf("H.264 SPS: aspect ratio extended");
           }
        else if (aspect_ratio_idc < sizeof(s_AspectRatios) / sizeof(s_AspectRatios[0])) {
           aspect_ratio = s_AspectRatios[aspect_ratio_idc];
           //Dprintf("H.264 SPS: -> aspect ratio %d", aspect_ratio);
           }
        }
     if (bs.getBit())                       // overscan_info_present_flag
        bs.skipBit();                       // overscan_approriate_flag
     if (bs.getBit()) {                     // video_signal_type_present_flag
        uint32_t video_format;
        video_format = bs.getBits(3);       // video_format
        if (video_format < sizeof(s_VideoFormats) / sizeof(s_VideoFormats[0])) {
           format = s_VideoFormats[video_format];
           //Dprintf("H.264 SPS: -> video format %d", format);
           }
        bs.skipBit();                       // video_full_range_flag
        bs.skipBit();                       // video_full_range_flag
        if (bs.getBit()) {                  // colour_description_present_flag
           bs.skipBits(8);                  // colour_primaries
           bs.skipBits(8);                  // transfer_characteristics
           bs.skipBits(8);                  // matrix_coefficients
           }
        }
     if (bs.getBit()) {                     // chroma_loc_info_present_flag
        bs.skipUeGolomb();                  // chroma_sample_loc_type_top_field
        bs.skipUeGolomb();                  // chroma_sample_loc_type_bottom_field
        }
     if (bs.getBit()) {                     // timing_info_present_flag
        bs.skipBits(32);                    // num_units_in_tick
        bs.skipBits(32);                    // time_scale
        bs.skipBit();                       // fixed_frame_rate_flag
        }
     int nal_hrd_parameters_present_flag = bs.getBit(); // nal_hrd_parameters_present_flag
     if (nal_hrd_parameters_present_flag) {
        int cpb_cnt_minus1;
        cpb_cnt_minus1 = bs.getUeGolomb();  // cpb_cnt_minus1
        bs.skipBits(4);                     // bit_rate_scale
        bs.skipBits(4);                     // cpb_size_scale
        for (int i = 0; i < cpb_cnt_minus1; ++i) {
            bs.skipUeGolomb();              // bit_rate_value_minus1[i]
            bs.skipUeGolomb();              // cpb_size_value_minus1[i]
            bs.skipBit();                   // cbr_flag[i]
            }
        bs.skipBits(5);                     // initial_cpb_removal_delay_length_minus1
        bs.skipBits(5);                     // cpb_removal_delay_length_minus1
        bs.skipBits(5);                     // dpb_output_delay_length_minus1
        time_offset_length = bs.getBits(5); // time_offset_length
        }
     int vlc_hrd_parameters_present_flag = bs.getBit(); // vlc_hrd_parameters_present_flag
     if (vlc_hrd_parameters_present_flag) {
         int cpb_cnt_minus1;
         cpb_cnt_minus1 = bs.getUeGolomb();  // cpb_cnt_minus1
         bs.skipBits(4);                     // bit_rate_scale
         bs.skipBits(4);                     // cpb_size_scale
         for (int i = 0; i < cpb_cnt_minus1; ++i) {
             bs.skipUeGolomb();              // bit_rate_value_minus1[i]
             bs.skipUeGolomb();              // cpb_size_value_minus1[i]
             bs.skipBit();                   // cbr_flag[i]
             }
         bs.skipBits(5);                     // initial_cpb_removal_delay_length_minus1
         bs.skipBits(5);                     // cpb_removal_delay_length_minus1
         bs.skipBits(5);                     // dpb_output_delay_length_minus1
         time_offset_length = bs.getBits(5); // time_offset_length
        }
     cpb_dpb_delays_present_flag = (nal_hrd_parameters_present_flag | vlc_hrd_parameters_present_flag);
     if (cpb_dpb_delays_present_flag)
        bs.skipBit();                       // low_delay_hrd_flag
     pic_struct_present_flag = bs.getBit(); // pic_struct_present_flag
     if (bs.getBit()) {                     // bitstream_restriction_flag
        bs.skipBit();                       // motion_vectors_over_pic_boundaries_flag
        bs.skipUeGolomb();                  // max_bytes_per_pic_denom
        bs.skipUeGolomb();                  // max_bits_per_mb_denom
        bs.skipUeGolomb();                  // log2_max_mv_length_horizontal
        bs.skipUeGolomb();                  // log2_max_mv_length_vertical
        bs.skipUeGolomb();                  // num_reorder_frames
        bs.skipUeGolomb();                  // max_dec_frame_buffering
        }
     }

  m_Width = width;
  m_Height = height;
  m_AspectRatio = aspect_ratio;
  m_Format = format;
  m_CpbDpbDelaysPresentFlag = cpb_dpb_delays_present_flag;
  m_PicStructPresentFlag = pic_struct_present_flag;
  m_TimeOffsetLength = time_offset_length;

  return (bs.getIndex() / 8);
}

int cFemonH264::parseSEI(const uint8_t *buf, int len)
{
  int num_referenced_subseqs, i;
  cBitStream bs(buf, len);

  double frame_rate = m_FrameRate;
  double bit_rate = m_BitRate;
  eVideoScan scan = m_Scan;

  while ((bs.getIndex() * 8 + 16) < len) {               // sei_message
    int lastByte, payloadSize = 0, payloadType = 0;

    // last_payload_type_byte
    do {
       lastByte = bs.getU8() & 0xFF;
       payloadType += lastByte;
    } while (lastByte == 0xFF);

    // last_payload_size_byte
    do {
       lastByte = bs.getU8() & 0xFF;
       payloadSize += lastByte;
    } while (lastByte == 0xFF);

    switch (payloadType) {                               // sei_payload
      case 1:                                            // pic_timing
           if (m_CpbDpbDelaysPresentFlag) {              // cpb_dpb_delays_present_flag
              bs.skipUeGolomb();                         // cpb_removal_delay
              bs.skipUeGolomb();                         // dpb_output_delay
              }
           if (m_PicStructPresentFlag) {                // pic_struct_present_flag
              unsigned int pic_struct = bs.getBits(4);  // pic_struct
              if (pic_struct >= (sizeof(s_SeiNumClockTsTable) ) / sizeof(s_SeiNumClockTsTable[0]))
                 return 0;
              for (int i = 0; i < s_SeiNumClockTsTable[pic_struct]; ++i) {
                  if (bs.getBit()) {                     // clock_timestamp_flag[i]
                     int full_timestamp_flag;
                     switch (bs.getBits(2)) {            // ct_type
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
                     //Dprintf("\nH.264 SEI: -> scan type %d", bit_rate, scan);
                     bs.skipBit();                       // nuit_field_based_flag
                     bs.skipBits(5);                     // counting_type
                     full_timestamp_flag = bs.getBit();  // full_timestamp_flag
                     bs.skipBit();                       // discontinuity_flag
                     bs.skipBit();                       // cnt_dropped_flag
                     bs.skipBits(8);                     // n_frames
                     if (full_timestamp_flag) {
                        bs.skipBits(6);                  // seconds_value
                        bs.skipBits(6);                  // minutes_value
                        bs.skipBits(5);                  // hours_value
                        }
                     else {
                        if (bs.getBit()) {               // seconds_flag
                           bs.skipBits(6);               // seconds_value
                           if (bs.getBit()) {            // minutes_flag
                              bs.skipBits(6);            // minutes_value
                              if (bs.getBit())           // hours_flag
                                  bs.skipBits(5);        // hours_value
                              }
                           }
                        }
                     if (m_TimeOffsetLength > 0)
                        bs.skipBits(m_TimeOffsetLength); // time_offset
                     }
                  }
              }
           break;

      case 12:                                          // sub_seq_characteristics
           bs.skipUeGolomb();                           // sub_seq_layer_num
           bs.skipUeGolomb();                           // sub_seq_id
           if (bs.getBit())                             // duration_flag
              bs.skipBits(32);                          // sub_seq_duration
           if (bs.getBit()) {                           // average_rate_flag
              bs.skipBit();                             // accurate_statistics_flag
              bit_rate   = bs.getU16() / 1048.51;       // average_bit_rate (1000 bit/s -> Mbit/s)
              frame_rate = bs.getU16() / 256.0;         // average_frame_rate (frames/256s)
              //Dprintf("\nH.264 SEI: -> stream bitrate %.1f, frame rate %.1f", bit_rate, frame_rate);
              }
           num_referenced_subseqs = bs.getUeGolomb();   // num_referenced_subseqs
           for (i = 0; i < num_referenced_subseqs; ++i) {
               bs.skipUeGolomb();                       // ref_sub_seq_layer_num
               bs.skipUeGolomb();                       // ref_sub_seq_id
               bs.getBit();                             // ref_sub_seq_direction
               }
           break;

      default:
           bs.skipBits(payloadSize * 8);
           break;
      }

    // force byte align
    bs.byteAlign();
    }

  m_FrameRate = frame_rate;
  m_BitRate = bit_rate;
  m_Scan = scan;

  return (bs.getIndex() / 8);
}
