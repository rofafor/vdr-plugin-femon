/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * AC3 Audio Header: http://www.atsc.org/standards/a_52a.pdf
 */

#include "femontools.h"
#include "femonac3.h"

#define IS_AC3_DATA(buf) (((buf)[0] == 0x0b) && ((buf)[1] == 0x77))

static unsigned int ac3_bitrates[32] =
{
  32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384, 448, 512, 576, 640, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static unsigned int ac3_freq[4] =
{
  480, 441, 320, 0
};

//static unsigned int ac3_frames[3][32] =
//{
//  {64, 80,  96,  112, 128, 160, 192, 224, 256, 320, 384, 448, 512, 640, 768,  896,  1024, 1152, 1280, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  {69, 87,  104, 121, 139, 174, 208, 243, 278, 348, 417, 487, 557, 696, 835,  975,  1114, 1253, 1393, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  {96, 120, 144, 168, 192, 240, 288, 336, 384, 480, 576, 672, 768, 960, 1152, 1344, 1536, 1728, 1920, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
//};

bool getAC3AudioInfo(uint8_t *buf, int len, ac3_info_t *info)
{
  if (!IS_AC3_DATA(buf) || (len < 8))
     return false;

  uint8_t *data = buf + 2;
  uint8_t frame = (uint8_t)(data[2] & 0x3f);
  info->bitrate = 1000 * ac3_bitrates[frame >> 1];
  uint8_t fr = (data[2] & 0xc0 ) >> 6;
  //uint8_t sz = ac3_frames[fr][frame >> 1];
  //if ((frame & 1) && (fr == 1))
  //   sz++;
  //sz <<= 1;
  info->samplingFrequency = 100 * ac3_freq[fr];
  info->bitstreamMode = (data[3] & 7);
  int acm = (data[4] & 0xE0) >> 5;
  info->audioCodingMode = acm;
  if ((acm & 0x01) && (acm != 0x01)) {
     // 3 front channels
     info->centerMixLevel = (data[4] & 0x18) >> 3;
     if (acm & 0x04) {
        // a surround channel exists
        info->surroundMixLevel = (data[4] & 0x06) >> 1;
        if (acm == 0x02) {
           // if in 2/0 mode
           info->dolbySurroundMode = ((data[4] & 0x01) << 1) | ((data[5] & 0x80) >> 7);
           info->lfe = (data[5] & 0x40) >> 6;
           info->dialogLevel = (data[5] & 0x3e) >> 1;
           }
        else {
           info->dolbySurroundMode = AUDIO_DOLBY_SURROUND_MODE_INVALID;
           info->lfe = (data[4] & 0x01);
           info->dialogLevel = (data[5] & 0xF8) >> 3;
           }
        }
     else {
        info->surroundMixLevel = AUDIO_SURROUND_MIX_LEVEL_INVALID;
        if (acm == 0x02) {
           // if in 2/0 mode
            info->dolbySurroundMode = (data[4] & 0x06) >> 1;
            info->lfe = (data[4] & 0x01);
            info->dialogLevel = (data[5] & 0xF8) >> 3;
           }
        else {
           info->dolbySurroundMode = AUDIO_DOLBY_SURROUND_MODE_INVALID;
           info->lfe = (data[4] & 0x04) >> 2;
           info->dialogLevel = (data[4] & 0x03) << 3 | ((data[5] & 0xE0) >> 5);
           }
        }
     }
  else {
     info->centerMixLevel = AUDIO_CENTER_MIX_LEVEL_INVALID;
     if (acm & 0x04) {
        // a surround channel exists
        info->surroundMixLevel = (data[4] & 0x18) >> 3;
        if (acm == 0x02) {
           // if in 2/0 mode
           info->dolbySurroundMode = (data[4] & 0x06) >> 1;
           info->lfe = (data[4] & 0x01);
           info->dialogLevel = (data[5] & 0xF8) >> 3;
           }
        else {
           info->dolbySurroundMode = AUDIO_DOLBY_SURROUND_MODE_INVALID;
           info->lfe = (data[4] & 0x04) >> 2;
           info->dialogLevel = (data[4] & 0x03) << 3 | ((data[5] & 0xE0) >> 5);
           }
        }
     else {
        info->surroundMixLevel = AUDIO_SURROUND_MIX_LEVEL_INVALID;
        if (acm == 0x02) {
           // if in 2/0 mode
           info->dolbySurroundMode = (data[4] & 0x18) >> 3;
           info->lfe = (data[4] & 0x04) >> 2;
           info->dialogLevel = (data[4] & 0x03) << 3 | ((data[5] & 0xE0) >> 5);
           }
        else {
           info->dolbySurroundMode = AUDIO_DOLBY_SURROUND_MODE_INVALID;
           info->lfe = (data[4] & 0x10) >> 4;
           info->dialogLevel = ((data[4] & 0x0F) << 1) | ((data[5] & 0x80) >> 7);
           }
        }
     }
     
  return true;
}
