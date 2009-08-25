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

#define NAL_SEI     0x06 // Supplemental Enhancement Information
#define NAL_SPS     0x07 // Sequence Parameter Set
#define NAL_AUD     0x09 // Access Unit Delimiter
#define NAL_END_SEQ 0x0A // End of Sequence

#define IS_NAL_SEI(buf)     (((buf)[0] == 0x00) && ((buf)[1] == 0x00) && ((buf)[2] == 0x01) && ((buf)[3] == NAL_SEI))
#define IS_NAL_SPS(buf)     (((buf)[0] == 0x00) && ((buf)[1] == 0x00) && ((buf)[2] == 0x01) && ((buf)[3] == NAL_SPS))
#define IS_NAL_AUD(buf)     (((buf)[0] == 0x00) && ((buf)[1] == 0x00) && ((buf)[2] == 0x01) && ((buf)[3] == NAL_AUD))
#define IS_NAL_END_SEQ(buf) (((buf)[0] == 0x00) && ((buf)[1] == 0x00) && ((buf)[2] == 0x01) && ((buf)[3] == NAL_END_SEQ))

// Picture types
#define NO_PICTURE  0
#define I_FRAME     1
#define P_FRAME     2
#define B_FRAME     3

static const eVideoAspectRatio aspect_ratios[] =
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

static const eVideoFormat video_formats[] =
{
  VIDEO_FORMAT_COMPONENT,
  VIDEO_FORMAT_PAL,
  VIDEO_FORMAT_NTSC,
  VIDEO_FORMAT_SECAM,
  VIDEO_FORMAT_MAC,
  VIDEO_FORMAT_UNKNOWN,
  VIDEO_FORMAT_RESERVED
};

typedef struct {
  int width;
  int height;
  eVideoAspectRatio aspect_ratio;
  eVideoFormat format;
} h264_sps_data_t;

typedef struct {
   double frame_rate;
   double bitrate;
   eVideoScan scan;
} h264_sei_data_t;

static bool h264_parse_sps(const uint8_t *buf, int len, h264_sps_data_t *sps)
{
  int profile_idc, pic_order_cnt_type;
  int frame_mbs_only;
  int i, j;
  cBitStream bs(buf, len);

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
  sps->width      = bs.getUeGolomb() + 1;   // mbs
  sps->height     = bs.getUeGolomb() + 1;   // mbs
  frame_mbs_only  = bs.getBit();

  //Dprintf("H.264 SPS: pic_width:  %u mbs", (unsigned int)sps->width);
  //Dprintf("H.264 SPS: pic_height: %u mbs", (unsigned int)sps->height);
  //Dprintf("H.264 SPS: frame only flag: %d", frame_mbs_only);

  sps->width  *= 16;
  sps->height *= 16 * (2 - frame_mbs_only);

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

    sps->width -= 2 * (crop_left + crop_right);
    if (frame_mbs_only)
       sps->height -= 2 * (crop_top + crop_bottom);
    else
       sps->height -= 4 * (crop_top + crop_bottom);
    }

  // VUI parameters
  sps->aspect_ratio = VIDEO_ASPECT_RATIO_INVALID;
  sps->format = VIDEO_FORMAT_INVALID;
  if (bs.getBit()) {                        // vui_parameters_present_flag
     if (bs.getBit()) {                     // aspect_ratio_info_present
        uint32_t aspect_ratio_idc = bs.getU8();
        //Dprintf("H.264 SPS: aspect_ratio_idc %d", aspect_ratio_idc);
        if (aspect_ratio_idc == 255) {      // extended sar
           bs.skipBit();                    // sar_width
           bs.skipBit();                    // sar_height
           sps->aspect_ratio = VIDEO_ASPECT_RATIO_EXTENDED;
           //Dprintf("H.264 SPS: aspect ratio extended");
           }
        else if (aspect_ratio_idc < sizeof(aspect_ratios) / sizeof(aspect_ratios[0])) {
           sps->aspect_ratio = aspect_ratios[aspect_ratio_idc];
           //Dprintf("H.264 SPS: -> aspect ratio %d", sps->aspect_ratio);
           }
        }
     if (bs.getBit())                       // overscan_info_present_flag
        bs.skipBit();                       // overscan_approriate_flag
     if (bs.getBit()) {                     // video_signal_type_present_flag
        uint32_t video_format = bs.getBits(3);
        if (video_format < sizeof(video_formats) / sizeof(video_formats[0])) {
           sps->format = video_formats[video_format];
           //Dprintf("H.264 SPS: -> video format %d", sps->format);
           }
        }
     }

  //Dprintf("H.264 SPS: -> video size %dx%d, aspect %d", sps->width, sps->height, sps->aspect_ratio);

  if (bs.isEOF()) {
     //Dprintf("H.264 SPS: not enough data ?");
     return false;
     }

  return true;
}

static bool h264_parse_sei(const uint8_t *buf, int len, h264_sei_data_t *sei)
{
  int num_referenced_subseqs, i;
  cBitStream bs(buf, len);

  while (!bs.isEOF()) { // sei_message
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

    switch (payloadType) {                             // sei_payload
      //case 1:                                        // pic_timing
      //     ...
      //     switch (bs.getBits(2)) {                    // ct_type
      //       case 0:
      //            sei->scan = VIDEO_SCAN_PROGRESSIVE;
      //            break;
      //       case 1:
      //            sei->scan = VIDEO_SCAN_INTERLACED;
      //            break;
      //       case 2:
      //            sei->scan = VIDEO_SCAN_UNKNOWN;
      //            break;
      //       default:
      //            sei->scan = VIDEO_SCAN_RESERVED;
      //            break;
      //       }
      //     break;

      case 12:                                          // sub_seq_characteristics
           bs.skipUeGolomb();                           // sub_seq_layer_num
           bs.skipUeGolomb();                           // sub_seq_id
           if (bs.getBit())                             // duration_flag
              bs.skipBits(32);                          // sub_seq_duration
           if (bs.getBit()) {                           // average_rate_flag
              bs.skipBit();                             // accurate_statistics_flag
              sei->bitrate = bs.getU16();               // average_bit_rate
              sei->frame_rate = bs.getU16();            // average_frame_rate
              //Dprintf("H.264 SEI: -> stream bitrate %.1f, frame rate %.1f", sei->bitrate, sei->frame_rate);
              }
           num_referenced_subseqs = bs.getUeGolomb();   // num_referenced_subseqs
           for (i = 0; i < num_referenced_subseqs; ++i) {
               bs.skipUeGolomb();                       // ref_sub_seq_layer_num
               bs.skipUeGolomb();                       // ref_sub_seq_id
               bs.getBit();                             // ref_sub_seq_direction
               }
           break;

      default:
           bs.skipBits(payloadSize);
           break;
      }

    // force byte align
    bs.byteAlign();
    }

  return true;
}

static int h264_nal_unescape(uint8_t *dst, const uint8_t *src, int len)
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

static int h264_get_picture_type(const uint8_t *buf, int len)
{
  for (int i = 0; i < (len - 5); ++i) {
      if (buf[i] == 0 && buf[i + 1] == 0 && buf[i + 2] == 1 && buf[i + 3] == NAL_AUD) {
         uint8_t type = (uint8_t)(buf[i + 4] >> 5);
         switch (type) {
           case 0: case 3: case 5: return I_FRAME;
           case 1: case 4: case 6: return P_FRAME;
           case 2: case 7:         return B_FRAME;
           default:;
           }
         }
      }
  return NO_PICTURE;
}

cFemonH264::cFemonH264(cFemonVideoIf *videohandler)
: m_VideoHandler(videohandler)
{
}

cFemonH264::~cFemonH264()
{
}

bool cFemonH264::processVideo(const uint8_t *buf, int len)
{
  bool sps_found = false, sei_found = true; // sei currently disabled

  if (!m_VideoHandler)
     return false;

  // skip PES header
  if (!PesLongEnough(len))
      return false;
  buf += PesPayloadOffset(buf);

  // H.264 detection, search for NAL AUD
  if (!IS_NAL_AUD(buf))
     return false;

  // If I-frame, search for NAL SPS
  if (h264_get_picture_type(buf, len) != I_FRAME)
     return false;

  m_VideoHandler->SetVideoCodec(VIDEO_CODEC_H264);

  // Scan video packet ...
  for (int i = 5; i < len - 4; i++) {
      // ... for sequence parameter set
      if (!sps_found && (buf[i] == 0x00) && (buf[i + 1] == 0x00) && (buf[i + 2] == 0x01) && (buf[i + 3] & 0x1f) == NAL_SPS) {
         uint8_t nal_data[len];
         int nal_len;
         //Dprintf("H.264: Found NAL SPS at offset %d/%d", i, len);
         if (0 < (nal_len = h264_nal_unescape(nal_data, buf + i + 4, len - i - 4))) {
            h264_sps_data_t sps = { 0, 0, VIDEO_ASPECT_RATIO_INVALID, VIDEO_FORMAT_INVALID };
            if (h264_parse_sps(nal_data, nal_len, &sps)) {
               m_VideoHandler->SetVideoFormat(sps.format);
               m_VideoHandler->SetVideoSize(sps.width, sps.height);
               m_VideoHandler->SetVideoAspectRatio(sps.aspect_ratio);
               sps_found = true;
               }
            }
         }
      // ... for supplemental enhancement information
      if (!sei_found && (buf[i] == 0x00) && (buf[i + 1] == 0x00) && (buf[i + 2] == 0x01) && (buf[i + 3] & 0x1f) == NAL_SEI) {
         uint8_t nal_data[len];
         int nal_len;
         //Dprintf("H.264: Found NAL SEI at offset %d/%d", i, len);
         if (0 < (nal_len = h264_nal_unescape(nal_data, buf + i + 4, len - i - 4))) {
            h264_sei_data_t sei = { 0, 0, VIDEO_SCAN_INVALID };
            if (h264_parse_sei(nal_data, nal_len, &sei)) {
               m_VideoHandler->SetVideoFramerate(sei.frame_rate);
               m_VideoHandler->SetVideoBitrate(sei.bitrate);
               m_VideoHandler->SetVideoScan(sei.scan);
               sei_found = true;
               }
            }
         }
      if (sps_found && sei_found)
         break;
      }

  return true;
}
