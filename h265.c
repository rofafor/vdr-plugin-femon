/*
 * h265.c: Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include <math.h>

#include "log.h"
#include "tools.h"
#include "h265.h"

const cFemonH265::t_DAR cFemonH265::darS[] =
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

const cFemonH265::t_SAR cFemonH265::sarS[] =
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

const eVideoFormat cFemonH265::videoFormatS[] =
{
  VIDEO_FORMAT_COMPONENT,
  VIDEO_FORMAT_PAL,
  VIDEO_FORMAT_NTSC,
  VIDEO_FORMAT_SECAM,
  VIDEO_FORMAT_MAC,
  VIDEO_FORMAT_UNKNOWN,
  VIDEO_FORMAT_RESERVED,
  VIDEO_FORMAT_RESERVED
};

cFemonH265::cFemonH265(cFemonVideoIf *videoHandlerP)
: videoHandlerM(videoHandlerP),
  widthM(0),
  heightM(0),
  aspectRatioM(VIDEO_ASPECT_RATIO_INVALID),
  formatM(VIDEO_FORMAT_INVALID),
  frameRateM(0),
  bitRateM(0),
  scanM(VIDEO_SCAN_INVALID),
  frameFieldInfoPresentFlagM(false)
{
  reset();
}

cFemonH265::~cFemonH265()
{
}

bool cFemonH265::processVideo(const uint8_t *bufP, int lenP)
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
                int nal_len = nalUnescape(nal_data, buf + 5, int(end - buf - 5));
                consumed = parseSPS(nal_data, nal_len);
                if (consumed > 0)
                   sps_found = true;
                }
             break;

        case NAL_SEI:
             if (!sei_found) {
               debug2("%s Found NAL SEI at offset %d/%d", __PRETTY_FUNCTION__, int(buf - start), lenP);
               int nal_len = nalUnescape(nal_data, buf + 5, int(end - buf - 5));
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
        videoHandlerM->SetVideoCodec(VIDEO_CODEC_H265);
        if (sps_found) {
           debug2("%s width=%d height=%d, aspect=%d format=%d bitrate=%.0f", __PRETTY_FUNCTION__, widthM, heightM, aspectRatioM, formatM, bitRateM);
           videoHandlerM->SetVideoSize(widthM, heightM);
           videoHandlerM->SetVideoFormat(formatM);
           videoHandlerM->SetVideoAspectRatio(aspectRatioM);
           videoHandlerM->SetVideoBitrate(bitRateM);
           }
        if (sps_found || sei_found) {
           debug2("%s scan=%d framerate=%.2f", __PRETTY_FUNCTION__, scanM, frameRateM);
           videoHandlerM->SetVideoScan(scanM);
           videoHandlerM->SetVideoFramerate(frameRateM);
           }
        }

  return sps_found;
}

void cFemonH265::reset()
{
  frameFieldInfoPresentFlagM = false;
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

int cFemonH265::parseSPS(const uint8_t *bufP, int lenP)
{
  cFemonBitStream bs(bufP, lenP);

  uint32_t width = widthM;
  uint32_t height = heightM;
  eVideoFormat format = formatM;
  eVideoAspectRatio aspect_ratio = aspectRatioM;
  eVideoScan scan = scanM;
  double frame_rate = frameRateM;
  double bit_rate = bitRateM;
  bool frame_field_info_present_flag = frameFieldInfoPresentFlagM;

  const char *profile_name = NULL;
  bool general_tier_flag, conformance_window_flag, sps_sub_layer_ordering_info_present_flag, profilePresentFlag = true;
  bool general_max_12bit_constraint_flag = false, general_max_10bit_constraint_flag = false, general_max_8bit_constraint_flag = false;
  bool general_max_422chroma_constraint_flag = false, general_max_420chroma_constraint_flag = false, general_max_monochrome_constraint_flag = false;
  bool general_intra_constraint_flag = false, general_one_picture_only_constraint_flag = false, general_lower_bit_rate_constraint_flag = false;
  bool general_progressive_source_flag, general_interlaced_source_flag, general_profile_compatibility_flag[32];
  uint32_t chroma_format_idc, log2_max_pic_order_cnt_lsb_minus4, num_short_term_ref_pic_sets, num_long_term_ref_pics_sps;
  uint8_t sps_max_sub_layers_minus1, sub_layer_profile_present_flag[8], sub_layer_level_present_flag[8], general_profile_idc, general_level_idc;

  bs.SkipBits(4);                                                    // sps_video_parameter_set_id
  sps_max_sub_layers_minus1 = bs.GetBits(3);                         // sps_max_sub_layers_minus1
  bs.SkipBit();                                                      // sps_temporal_id_nesting_flag
  // start of profile_tier_level(1, sps_max_sub_layers_minus1)
  if (profilePresentFlag) {
     bs.SkipBits(2);                                                 // general_profile_space
     general_tier_flag = bs.GetBit();                                // general_tier_flag
     general_profile_idc = bs.GetBits(5);                            // general_profile_idc
     for (int i = 0; i < 32; ++i) {
         general_profile_compatibility_flag[i] = bs.GetBit();        // general_profile_compatibility_flag[i]
         }
     general_progressive_source_flag = bs.GetBit();                  // general_progressive_source_flag
     general_interlaced_source_flag = bs.GetBit();                   // general_interlaced_source_flag
     if (general_progressive_source_flag && !general_interlaced_source_flag)
        scan = VIDEO_SCAN_PROGRESSIVE;
     else if (!general_progressive_source_flag && general_interlaced_source_flag)
        scan = VIDEO_SCAN_INTERLACED;
     else if (!general_progressive_source_flag && !general_interlaced_source_flag)
        scan = VIDEO_SCAN_UNKNOWN;
     else
        scan = VIDEO_SCAN_INVALID;
     debug2("%s general_progressive_source_flag=%d general_interlaced_source_flag=%d scan_type=%d", __PRETTY_FUNCTION__, general_progressive_source_flag, general_interlaced_source_flag, scan);
     bs.SkipBit();                                                   // general_non_packed_constraint_flag
     bs.SkipBit();                                                   // general_frame_only_constraint_flag
     if (general_profile_idc == 4 || general_profile_compatibility_flag[4] ||
         general_profile_idc == 5 || general_profile_compatibility_flag[5] ||
         general_profile_idc == 6 || general_profile_compatibility_flag[6] ||
         general_profile_idc == 7 || general_profile_compatibility_flag[7]) {
        // the number of bits in this syntax structure is not affected by this condition
        general_max_12bit_constraint_flag = bs.GetBit();             // general_max_12bit_constraint_flag
        general_max_10bit_constraint_flag = bs.GetBit();             // general_max_10bit_constraint_flag
        general_max_8bit_constraint_flag = bs.GetBit();              // general_max_8bit_constraint_flag
        debug2("%s  general_max_12bit_constraint_flag=%d general_max_10bit_constraint_flag=%d general_max_8bit_constraint_flag=%d", __PRETTY_FUNCTION__, general_max_12bit_constraint_flag, general_max_10bit_constraint_flag, general_max_8bit_constraint_flag);
        general_max_422chroma_constraint_flag = bs.GetBit();         // general_max_422chroma_constraint_flag
        general_max_420chroma_constraint_flag = bs.GetBit();         // general_max_420chroma_constraint_flag
        general_max_monochrome_constraint_flag = bs.GetBit();        // general_max_monochrome_constraint_flag
        debug2("%s general_max_422chroma_constraint_flag=%d general_max_420chroma_constraint_flag=%d general_max_monochrome_constraint_flag=%d", __PRETTY_FUNCTION__, general_max_422chroma_constraint_flag, general_max_420chroma_constraint_flag, general_max_monochrome_constraint_flag);
        general_intra_constraint_flag = bs.GetBit();                 // general_intra_constraint_flag
        general_one_picture_only_constraint_flag = bs.GetBit();      // general_one_picture_only_constraint_flag
        general_lower_bit_rate_constraint_flag = bs.GetBit();        // general_lower_bit_rate_constraint_flag
        debug2("%s general_intra_constraint_flag=%d general_one_picture_only_constraint_flag=%d general_lower_bit_rate_constraint_flag=%d", __PRETTY_FUNCTION__, general_intra_constraint_flag, general_one_picture_only_constraint_flag, general_lower_bit_rate_constraint_flag);
        bs.SkipBits(34);                                             // general_reserved_zero_34bits
        }
     else
        bs.SkipBits(43);                                             // general_reserved_zero_43bits
     // the number of bits in this syntax structure is not affected by this condition
     bs.SkipBit();                                                   // general_reserved_zero_bit
     }
  general_level_idc = bs.GetBits(8);                                 // general_level_idc
  debug2("%s general_profile_idc=%d general_tier_flag=%d general_level_idc=%d", __PRETTY_FUNCTION__, general_profile_idc, general_tier_flag, general_level_idc);
  switch (general_profile_idc) {
    default:
    case 0:
        profile_name = "None";
        break;
    case 1:
        profile_name = "Main";
        break;
    case 2:
        profile_name = "Main 10";
        break;
    case 3:
        profile_name = "Main Still Picture";
        break;
    case 4:
        profile_name = "Format Range Extensions";
        break;
    case 5:
        profile_name = "Format Range Extensions High Throughput";
        break;
    }
  if (general_profile_idc == 1 || general_profile_idc == 2)
     switch (general_level_idc) {
       case 30:  // level 1
            bit_rate = general_tier_flag ? 0 : 128000;
            break;
       case 60:  // level 2
            bit_rate = general_tier_flag ? 0 : 1500000;
            break;
       case 63:  // level 2.1
            bit_rate = general_tier_flag ? 0 : 3000000;
            break;
       case 90:  // level 3
            bit_rate = general_tier_flag ? 0 : 6000000;
            break;
       case 93:  // level 3
            bit_rate = general_tier_flag ? 0 : 10000000;
            break;
       case 120: // level 4
            bit_rate = general_tier_flag ? 30000000 : 12000000;
            break;
       case 123: // level 4.1
            bit_rate = general_tier_flag ? 50000000 : 20000000;
            break;
       case 150: // level 5
            bit_rate = general_tier_flag ? 100000000 : 25000000;
            break;
       case 153: // level 5.1
            bit_rate = general_tier_flag ? 160000000 : 40000000;
            break;
       case 156: // level 5.2
            bit_rate = general_tier_flag ? 240000000 : 60000000;
            break;
       case 180: // level 6
            bit_rate = general_tier_flag ? 240000000 : 60000000;
            break;
       case 183: // level 6.1
            bit_rate = general_tier_flag ? 480000000 : 120000000;
            break;
       case 186: // level 6.2
            bit_rate = general_tier_flag ? 800000000 : 240000000;
            break;
       default:
            bit_rate = 0;
            break;
       }
  else
     bit_rate = 0;
  debug2("%s profile=\"%s@L%.1f@%s\" bit_rate=%.f", __PRETTY_FUNCTION__, profile_name, (double)general_level_idc / 30, general_tier_flag ? "High" : "Main", bit_rate);
  for (int i = 0; i < sps_max_sub_layers_minus1; ++i) {
      sub_layer_profile_present_flag[i] = bs.GetBit();               // sub_layer_profile_present_flag[i]
      sub_layer_level_present_flag[i] = bs.GetBit();                 // sub_layer_level_present_flag[i]
      }
  if (sps_max_sub_layers_minus1 > 0) {
     for (int i = sps_max_sub_layers_minus1; i < 8; ++i)
         bs.SkipBits(2);                                             // reserved_zero_2bits[i]
     }
  for (int i = 0; i < sps_max_sub_layers_minus1; ++i) {
      if (sub_layer_profile_present_flag[i]) {
         bs.SkipBits(2);                                             // sub_layer_profile_space[i]
         bs.SkipBit();                                               // sub_layer_tier_flag[i]
         bs.SkipBits(5);                                             // sub_layer_profile_idc[i]
         bs.SkipBits(32);                                            // sub_layer_profile_compatibility_flag[i][0-31]
         bs.SkipBit();                                               // sub_layer_progressive_source_flag[i]
         bs.SkipBit();                                               // sub_layer_interlaced_source_flag[i]
         bs.SkipBit();                                               // sub_layer_non_packed_constraint_flag[i]
         bs.SkipBit();                                               // sub_layer_frame_only_constraint_flag[i]
         // the number of bits in this syntax structure is not affected by this condition
         bs.SkipBits(43);                                            // sub_layer_reserved_zero_43bits[i]
         // the number of bits in this syntax structure is not affected by this condition
         bs.SkipBit();                                               // sub_layer_reserved_zero_bit[i]
         }
      if (sub_layer_level_present_flag[i])
         bs.SkipBits(8);                                             // sub_layer_level_idc[i]
      }
  // end of profile_tier_level
  bs.SkipUeGolomb();                                                 // sps_seq_parameter_set_id
  chroma_format_idc = bs.GetUeGolomb();                              // chroma_format_idc
  if (chroma_format_idc == 3)
     bs.SkipBit();                                                   // separate_colour_plane_flag
  width = bs.GetUeGolomb();                                          // pic_width_in_luma_samples
  height = bs.GetUeGolomb();                                         // pic_height_in_luma_samples

  conformance_window_flag = bs.GetBit();                             // conformance_window_flag
  if (conformance_window_flag) {
     bs.SkipUeGolomb();                                              // conf_win_left_offset
     bs.SkipUeGolomb();                                              // conf_win_right_offset
     bs.SkipUeGolomb();                                              // conf_win_top_offset
     bs.SkipUeGolomb();                                              // conf_win_bottom_offset
     }
  bs.SkipUeGolomb();                                                 // bit_depth_luma_minus8
  bs.SkipUeGolomb();                                                 // bit_depth_chroma_minus8
  log2_max_pic_order_cnt_lsb_minus4 = bs.GetUeGolomb();              // log2_max_pic_order_cnt_lsb_minus4
  sps_sub_layer_ordering_info_present_flag = bs.GetBit();            // sps_sub_layer_ordering_info_present_flag
  for (int i = (sps_sub_layer_ordering_info_present_flag ? 0 : sps_max_sub_layers_minus1); i <= sps_max_sub_layers_minus1; ++i) {
      bs.SkipUeGolomb();                                             // sps_max_dec_pic_buffering_minus1[i]
      bs.SkipUeGolomb();                                             // sps_max_num_reorder_pics[i]
      bs.SkipUeGolomb();                                             // sps_max_latency_increase_plus1[i]
      }
  bs.SkipUeGolomb();                                                 // log2_min_luma_coding_block_size_minus3
  bs.SkipUeGolomb();                                                 // log2_diff_max_min_luma_coding_block_size
  bs.SkipUeGolomb();                                                 // log2_min_luma_transform_block_size_minus2
  bs.SkipUeGolomb();                                                 // log2_diff_max_min_luma_transform_block_size
  bs.SkipUeGolomb();                                                 // max_transform_hierarchy_depth_inter
  bs.SkipUeGolomb();                                                 // max_transform_hierarchy_depth_intra
  if (bs.GetBit()) {                                                 // scaling_list_enabled_flag
     if (bs.GetBit()) {                                              // sps_scaling_list_data_present_flag
        // start of scaling_list_data()
        for (int sizeId = 0; sizeId < 4; ++sizeId) {
            for (int matrixId = 0; matrixId < 6; matrixId += (sizeId == 3) ? 3 : 1) {
                if (!bs.GetBit())                                    // scaling_list_pred_mode_flag[sizeId][matrixId]
                   bs.SkipUeGolomb();                                // scaling_list_pred_matrix_id_delta[sizeId][matrixId]
                else {
                   int coefNum = min(64, (1 << (4 + (sizeId << 1))));
                   if (sizeId > 1)
                      bs.SkipSeGolomb();                             // scaling_list_dc_coef_minus8[sizeId−2][matrixId]
                   for (int i = 0; i < coefNum; ++i)
                       bs.SkipSeGolomb();                            // scaling_list_delta_coef
                   }
                }
            }
        // end of scaling_list_data()
        }
     }
  bs.SkipBit();                                                      // amp_enabled_flag
  bs.SkipBit();                                                      // sample_adaptive_offset_enabled_flag
  if (bs.GetBit()) {                                                 // pcm_enabled_flag
     bs.SkipBits(4);                                                 // pcm_sample_bit_depth_luma_minus1
     bs.SkipBits(4);                                                 // pcm_sample_bit_depth_chroma_minus1
     bs.SkipUeGolomb();                                              // log2_min_pcm_luma_coding_block_size_minus3
     bs.SkipUeGolomb();                                              // log2_diff_max_min_pcm_luma_coding_block_size
     bs.SkipBit();                                                   // pcm_loop_filter_disabled_flag
     }
  num_short_term_ref_pic_sets = bs.GetUeGolomb();                    // num_short_term_ref_pic_sets
  uint32_t NumDeltaPocs[num_short_term_ref_pic_sets];
  for (uint32_t stRpsIdx = 0; stRpsIdx < num_short_term_ref_pic_sets; ++stRpsIdx) {
      // start of st_ref_pic_set(stRpsIdx)
      bool inter_ref_pic_set_prediction_flag = false;
      if (stRpsIdx != 0)
         inter_ref_pic_set_prediction_flag = bs.GetBit();            // inter_ref_pic_set_prediction_flag
      if (inter_ref_pic_set_prediction_flag) {
         uint32_t RefRpsIdx, delta_idx_minus1 = 0;
         if (stRpsIdx == num_short_term_ref_pic_sets)
            delta_idx_minus1 = bs.GetUeGolomb();                     // delta_idx_minus1
         bs.SkipBit();                                               // delta_rps_sign
         bs.SkipUeGolomb();                                          // abs_delta_rps_minus1
         RefRpsIdx = stRpsIdx - (delta_idx_minus1 + 1);
         NumDeltaPocs[stRpsIdx] = 0;
         for (uint32_t j = 0; j <= NumDeltaPocs[RefRpsIdx]; ++j) {
             if (!bs.GetBit()) {                                     // used_by_curr_pic_flag[j]
                if (bs.GetBit())                                     // use_delta_flag[j]
                   NumDeltaPocs[stRpsIdx]++;
                }
             else
                NumDeltaPocs[stRpsIdx]++;
             }
         }
      else {
         uint32_t num_negative_pics = bs.GetUeGolomb();              // num_negative_pics
         uint32_t num_positive_pics = bs.GetUeGolomb();              // num_positive_pics
         for (uint32_t j = 0; j < num_negative_pics; ++j) {
             bs.SkipUeGolomb();                                      // delta_poc_s0_minus1[i]
             bs.SkipBit();                                           // used_by_curr_pic_s0_flag[i]
             }
         for (uint32_t j = 0; j < num_positive_pics; ++j) {
             bs.SkipUeGolomb();                                      // delta_poc_s1_minus1[i]
             bs.SkipBit();                                           // delta_poc_s1_minus1[i]
             }
         NumDeltaPocs[stRpsIdx] = num_negative_pics + num_positive_pics;
         }
      // end of st_ref_pic_set(stRpsIdx)
      }
  if (bs.GetBit()) {                                                 // long_term_ref_pics_present_flag
     num_long_term_ref_pics_sps = bs.GetUeGolomb();                  // num_long_term_ref_pics_sps
     for (uint32_t i = 0; i < num_long_term_ref_pics_sps; ++i) {
         bs.SkipBits(log2_max_pic_order_cnt_lsb_minus4 + 4);         // lt_ref_pic_poc_lsb_sps[i]
         bs.SkipBit();                                               // used_by_curr_pic_lt_sps_flag[i]
         }
     }
  bs.SkipBit();                                                      // sps_temporal_mvp_enabled_flag
  bs.SkipBit();                                                      // strong_intra_smoothing_enabled_flag
  if (bs.GetBit()) {                                                 // vui_parameters_present_flag
     // start of vui_parameters()
     if (bs.GetBit()) {                                              // aspect_ratio_info_present_flag
        uint32_t sar_width = 0, sar_height = 0;
        uint8_t aspect_ratio_idc = bs.GetBits(8);                    // aspect_ratio_idc
        debug2("%s aspect_ratio_idc=%d", __PRETTY_FUNCTION__, aspect_ratio_idc);
        if (aspect_ratio_idc == 255) {                               // EXTENDED_SAR
           sar_width  = bs.GetBits(16);                              // sar_width
           sar_height = bs.GetBits(16);                              // sar_height
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
     if (bs.GetBit())                                                // overscan_info_present_flag
        bs.SkipBit();                                                // overscan_appropriate_flag
     if (bs.GetBit()) {                                              // video_signal_type_present_flag
        uint32_t video_format = bs.GetBits(3);                       // video_format
        if (video_format < ELEMENTS(videoFormatS)) {
           format = videoFormatS[video_format];
           debug2("%s video_format=%d format=%d", __PRETTY_FUNCTION__, video_format, format);
           }
        bs.SkipBit();                                                // video_full_range_flag
        if (bs.GetBit()) {                                           // colour_description_present_flag
           bs.SkipBits(8);                                           // colour_primaries
           bs.SkipBits(8);                                           // transfer_characteristics
           bs.SkipBits(8);                                           // matrix_coeffs
           }
        }
     if (bs.GetBit()) {                                              // chroma_loc_info_present_flag
        bs.SkipUeGolomb();                                           // chroma_sample_loc_type_top_field
        bs.SkipUeGolomb();                                           // chroma_sample_loc_type_bottom_field
        }
     bs.SkipBit();                                                   // neutral_chroma_indication_flag
     bs.SkipBit();                                                   // field_seq_flag
     frame_field_info_present_flag = bs.GetBit();                    // frame_field_info_present_flag
     debug2("%s frame_field_info_present_flag=%d", __PRETTY_FUNCTION__, frame_field_info_present_flag);
     if (bs.GetBit()) {                                              // default_display_window_flag
        bs.SkipUeGolomb();                                           // def_disp_win_left_offset
        bs.SkipUeGolomb();                                           // def_disp_win_right_offset
        bs.SkipUeGolomb();                                           // def_disp_win_top_offset
        bs.SkipUeGolomb();                                           // def_disp_win_bottom_offset
        }
     if (bs.GetBit()) {                                              // vui_timing_info_present_flag
        uint32_t vui_num_units_in_tick = bs.GetBits(32);             // vui_num_units_in_tick
        uint32_t vui_time_scale = bs.GetBits(32);                    // vui_time_scale
        if (vui_num_units_in_tick > 0) {
           frame_rate = (double)vui_time_scale / vui_num_units_in_tick;
           debug2("%s frame_rate = vui_time_scale(%d) / vui_num_units_in_tick(%d) = %.f", __PRETTY_FUNCTION__, vui_time_scale, vui_num_units_in_tick, frame_rate);
           }
        if (bs.GetBit())                                             // vui_poc_proportional_to_timing_flag
           bs.SkipUeGolomb();                                        // vui_num_ticks_poc_diff_one_minus1
        if (bs.GetBit()) {                                           // vui_hrd_parameters_present_flag
           // start of hrd_parameters(1, sps_max_sub_layers_minus1)
           uint32_t bit_rate_scale = 0;
           bool commonInfPresentFlag = true, nal_hrd_parameters_present_flag = false, vcl_hrd_parameters_present_flag = false, sub_pic_hrd_params_present_flag = false;
           if (commonInfPresentFlag) {
              nal_hrd_parameters_present_flag = bs.GetBit();         // nal_hrd_parameters_present_flag
              vcl_hrd_parameters_present_flag = bs.GetBit();         // vcl_hrd_parameters_present_flag
              if (nal_hrd_parameters_present_flag || vcl_hrd_parameters_present_flag) {
                 sub_pic_hrd_params_present_flag = bs.GetBit();      // sub_pic_hrd_params_present_flag
                 if (sub_pic_hrd_params_present_flag) {
                    bs.SkipBits(8);                                  // tick_divisor_minus2
                    bs.SkipBits(5);                                  // du_cpb_removal_delay_increment_length_minus1
                    bs.SkipBit();                                    // sub_pic_cpb_params_in_pic_timing_sei_flag
                    bs.SkipBits(5);                                  // dpb_output_delay_du_length_minus1
                    }
                 bit_rate_scale = bs.GetBits(4);                     // bit_rate_scale
                 bs.SkipBits(4);                                     // cpb_size_scale
                 if (sub_pic_hrd_params_present_flag)
                    bs.SkipBits(4);                                  // cpb_size_du_scale
                 bs.SkipBits(5);                                     // initial_cpb_removal_delay_length_minus1
                 bs.SkipBits(5);                                     // au_cpb_removal_delay_length_minus1
                 bs.SkipBits(5);                                     // dpb_output_delay_length_minus1
                 }
              }
           for (uint32_t i = 0; i <= sps_max_sub_layers_minus1; ++i) {
               bool fixed_pic_rate_within_cvs_flag = false, low_delay_hrd_flag = false;
               uint32_t cpb_cnt_minus1 = 0;
               if (!bs.GetBit())                                     // fixed_pic_rate_general_flag[i]
                  fixed_pic_rate_within_cvs_flag = bs.GetBit();      // fixed_pic_rate_within_cvs_flag[i]
               if (fixed_pic_rate_within_cvs_flag)
                  bs.SkipUeGolomb();                                 // elemental_duration_in_tc_minus1[i]
               else
                  low_delay_hrd_flag = bs.GetBit();                  // low_delay_hrd_flag[i]
               if (!low_delay_hrd_flag)
                  cpb_cnt_minus1 = bs.GetUeGolomb();                 // cpb_cnt_minus1[i]
               if (nal_hrd_parameters_present_flag) {
                  // start of sub_layer_hrd_parameters(i)
                  for (uint32_t i = 0; i <= cpb_cnt_minus1; ++i) {
                      uint32_t bit_rate_value_minus1;
                      bit_rate_value_minus1 = bs.GetUeGolomb();      // bit_rate_value_minus1[i]
                      bs.SkipUeGolomb();                             // cpb_size_value_minus1[i]
                      if (sub_pic_hrd_params_present_flag) {
                          bs.SkipUeGolomb();                         // cpb_size_du_value_minus1[i]
                          bs.SkipUeGolomb();                         // bit_rate_du_value_minus1[i]
                         }
                      else {
                         double bitrate = (double)((bit_rate_value_minus1 + 1) * pow(2.0, 6 + bit_rate_scale));
                         debug2("%s bit_rate_value_minus1(%u) + 1 * 2 ^ (6 + bit_rate_scale(%u)) = %.f",  __PRETTY_FUNCTION__, bit_rate_value_minus1, bit_rate_scale, bitrate);
                         }
                      bs.SkipBit();                                  // cbr_flag[i]
                      }
                  // end of sub_layer_hrd_parameters(i)
                  }
               if (vcl_hrd_parameters_present_flag) {
                  // start of sub_layer_hrd_parameters(i)
                  for (uint32_t i = 0; i <= cpb_cnt_minus1; ++i) {
                      bs.SkipUeGolomb();                             // bit_rate_value_minus1[i]
                      bs.SkipUeGolomb();                             // cpb_size_value_minus1[i]
                      if (sub_pic_hrd_params_present_flag) {
                          bs.SkipUeGolomb();                         // cpb_size_du_value_minus1[i]
                          bs.SkipUeGolomb();                         // bit_rate_du_value_minus1[i]
                         }
                      bs.SkipBit();                                  // cbr_flag[i]
                      }
                  // end of sub_layer_hrd_parameters(i)
                  }
               }
           // end of hrd_parameters(1, sps_max_sub_layers_minus1)
           }
        }
     if (bs.GetBit()) {                                              // bitstream_restriction_flag
        bs.SkipBit();                                                // tiles_fixed_structure_flag
        bs.SkipBit();                                                // motion_vectors_over_pic_boundaries_flag
        bs.SkipBit();                                                // restricted_ref_pic_lists_flag
        bs.SkipUeGolomb();                                           // min_spatial_segmentation_idc
        bs.SkipUeGolomb();                                           // max_bytes_per_pic_denom
        bs.SkipUeGolomb();                                           // max_bits_per_min_cu_denom
        bs.SkipUeGolomb();                                           // log2_max_mv_length_horizontal
        bs.SkipUeGolomb();                                           // log2_max_mv_length_vertical
        }
     // end of vui_parameters()
     }
  if (bs.GetBit()) {                                                 // sps_extension_present_flag
     bs.SkipBit();                                                   // sps_range_extension_flag
     bs.SkipBit();                                                   // sps_multilayer_extension_flag
     bs.SkipBit();                                                   // sps_3d_extension_flag
     bs.SkipBits(5);                                                 // sps_extension_5bits
     }

  widthM = width;
  heightM = height;
  formatM = format;
  aspectRatioM = aspect_ratio;
  scanM = scan;
  frameRateM = frame_rate;
  bitRateM = bit_rate;
  frameFieldInfoPresentFlagM = frame_field_info_present_flag;

  return (bs.Index() / 8);
}

int cFemonH265::parseSEI(const uint8_t *bufP, int lenP)
{
  cFemonBitStream bs(bufP, lenP);

  eVideoScan scan = scanM;

  while ((bs.Index() * 8 + 16) < lenP) {                             // sei_message
    int len, lastByte, payloadSize = 0, payloadType = 0;

    do {
       lastByte = bs.GetBits(8) & 0xFF;
       payloadType += lastByte;
    } while (lastByte == 0xFF);                                      // last_payload_type_byte

    do {
       lastByte = bs.GetBits(8) & 0xFF;
       payloadSize += lastByte;
    } while (lastByte == 0xFF);                                      // last_payload_size_byte

    switch (payloadType) {                                           // sei_payload
      case 1:                                                        // pic_timing
           len = payloadSize * 8;
           if (frameFieldInfoPresentFlagM) {
              uint8_t pic_struct = bs.GetBits(4);                    // pic_struct
              switch (pic_struct) {
                 case 0:                                             // frame
                 case 7:                                             // frame doubling
                 case 8:                                             // frame tripling
                      scan = VIDEO_SCAN_PROGRESSIVE;
                      break;
                 case 1:                                             // top
                 case 2:                                             // bottom
                 case 3:                                             // top bottom
                 case 4:                                             // bottom top
                 case 5:                                             // top bottom top
                 case 6:                                             // bottom top bottom
                 case 9:                                             // top paired with previous bottom
                 case 10:                                            // bottom paired with previous top
                 case 11:                                            // top paired with next bottom
                 case 12:                                            // bottom paired with next top
                      scan = VIDEO_SCAN_INTERLACED;
                      break;
                 default:
                      scan = VIDEO_SCAN_RESERVED;
                      break;
                 }
              debug2("%s pic_struct=%d scan_type=%d", __PRETTY_FUNCTION__, pic_struct, scan);
              bs.SkipBits(2);                                        // source_scan_type
              bs.SkipBit();                                          // duplicate_flag
              len -= 7;
              }
           bs.SkipBits(len);
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
