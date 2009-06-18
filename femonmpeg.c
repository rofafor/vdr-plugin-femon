/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include "femontools.h"
#include "femonmpeg.h"

#define IS_MPEG_AUDIO(buf)      (((buf)[0] == 0xFF) && ((buf)[1] & 0xF0))
#define IS_SEQUENCE_HEADER(buf) (((buf)[0] == 0x00) && ((buf)[1] == 0x00) && ((buf)[2] == 0x01) && ((buf)[3] == 0xB3))

static unsigned int bitrates[2][3][16] =
{
  {
    {0,  32,  48,  56,  64,  80,  96, 112, 128, 144, 160, 176, 192, 224, 256, -1}, // MPEG-2 Layer I
    {0,   8,  16,  24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, -1}, // MPEG-2 Layer II/III
    {0,   8,  16,  24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, -1}  // MPEG-2 Layer II/III
  },
  {
    {0,  32,  64,  96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, -1}, // MPEG-1 Layer I
    {0,  32,  48,  56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 384, -1}, // MPEG-1 Layer II
    {0,  32,  40,  48,  56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, -1}  // MPEG-1 Layer III
  }
};

static unsigned int samplerates[2][4] =
{
  {22050, 24000, 16000, -1}, // MPEG-2
  {44100, 48000, 32000, -1}  // MPEG-1
};

static eAudioCodec formats[2][4] =
{
  {AUDIO_CODEC_MPEG2_I, AUDIO_CODEC_MPEG2_II, AUDIO_CODEC_MPEG2_III, AUDIO_CODEC_UNKNOWN}, // MPEG-2
  {AUDIO_CODEC_MPEG1_I, AUDIO_CODEC_MPEG1_II, AUDIO_CODEC_MPEG1_III, AUDIO_CODEC_UNKNOWN}  // MPEG-1
};

bool getMPEGAudioInfo(uint8_t *buf, int len, audio_info_t *info)
{
  // MPEG audio detection, search for syncword
  if ((len < 4) || !IS_MPEG_AUDIO(buf))
     return false;

  int mpegIndex = (buf[1] & 0x08) >> 3;        // MPEG-2=0, MPEG-1=1
  int layerIndex = 3 - ((buf[1] & 0x06) >> 1); // I=11, II=10, III=01 
  int bitrateIndex = (buf[2] & 0xF0) >> 4;
  int frequency = (buf[2] & 0x0C) >> 2;
  int channelMode = (buf[3] & 0xC0) >> 6;

  info->codec = formats[mpegIndex][layerIndex];

  switch (channelMode) {
    case 0:
         info->channelMode = AUDIO_CHANNEL_MODE_STEREO;
         break;
         
    case 1:
         info->channelMode = AUDIO_CHANNEL_MODE_JOINT_STEREO;
         break;

    case 2:
         info->channelMode = AUDIO_CHANNEL_MODE_DUAL;
         break;

    case 3:
         info->channelMode = AUDIO_CHANNEL_MODE_SINGLE;
         break;

    default:
         info->channelMode = AUDIO_CHANNEL_MODE_INVALID;
         break;
  }

  switch (bitrateIndex) {
    case 0:
         info->bitrate = AUDIO_BITRATE_FREE;
         break;
         
    case 0xF:
         info->bitrate = AUDIO_BITRATE_RESERVED;
         break;

    default:
         info->bitrate = 1000 * bitrates[mpegIndex][layerIndex][bitrateIndex];
         break;
  }
  
  switch (frequency) {
    case 3:
         info->samplingFrequency = AUDIO_SAMPLING_FREQUENCY_RESERVED;
         break;

    default:
         info->samplingFrequency = samplerates[mpegIndex][frequency];
         break;
  }

  return true;
}

bool getMPEGVideoInfo(uint8_t *buf, int len, video_info_t *info)
{
  // MPEG-2 video detection, search for sequence header
  if ((len < 7) || !IS_SEQUENCE_HEADER(buf))
     return false;

  // Parse header   
  uint8_t *data = buf + 4;
  info->codec = VIDEO_CODEC_MPEG2;
  info->width = ((data[1] & 0xF0) >> 4) | (data[0] << 4);
  info->height = ((data[1] & 0x0F) << 8) | (data[2]);
  switch ((data[3] & 0xF0) >> 4) {
    case 1:
      info->aspectRatio = VIDEO_ASPECT_RATIO_1_1;
      break;
    case 2:
      info->aspectRatio = VIDEO_ASPECT_RATIO_4_3;
      break;
    case 3:
      info->aspectRatio = VIDEO_ASPECT_RATIO_16_9;
      break;
    case 4:
      info->aspectRatio = VIDEO_ASPECT_RATIO_2_21_1;
      break;
    case 5 ... 15:
    default:
      info->aspectRatio = VIDEO_ASPECT_RATIO_RESERVED;
      break;
    }
  // Video scan should be read from progressive_sequence field in sequence extension
  switch (data[3] & 0x0F) {
    case 1:
      info->frameRate   = 24000 / 1001.0;
      info->scan        = VIDEO_SCAN_PROGRESSIVE;
      info->format      = VIDEO_FORMAT_UNKNOWN;
      break;
    case 2:
      info->frameRate   = 24.0;
      info->scan        = VIDEO_SCAN_PROGRESSIVE;
      info->format      = VIDEO_FORMAT_UNKNOWN;
      break;
    case 3:
      info->frameRate   = 25.0;
      info->scan        = VIDEO_SCAN_UNKNOWN;  // interlaced or progressive
      info->format      = VIDEO_FORMAT_PAL;
      break;
    case 4:
      info->frameRate   = 30000 / 1001.0;
      info->scan        = VIDEO_SCAN_UNKNOWN; // interlaced or progressive
      info->format      = VIDEO_FORMAT_NTSC;
      break;
    case 5:
      info->frameRate   = 30.0;
      info->scan        = VIDEO_SCAN_UNKNOWN;  // interlaced or progressive
      info->format      = VIDEO_FORMAT_NTSC;
      break;
    case 6:
      info->frameRate   = 50.0;
      info->scan        = VIDEO_SCAN_PROGRESSIVE;
      info->format      = VIDEO_FORMAT_PAL;
      break;
    case 7:
      info->frameRate   = 60.0;
      info->scan        = VIDEO_SCAN_PROGRESSIVE;
      info->format      = VIDEO_FORMAT_NTSC;
      break;
    case 8:
      info->frameRate   = 60000 / 1001.0;
      info->scan        = VIDEO_SCAN_PROGRESSIVE;
      info->format      = VIDEO_FORMAT_NTSC;
      break;
    case 9 ... 15:
    default:
      info->frameRate   = 0;
      info->scan        = VIDEO_SCAN_UNKNOWN;
      info->format      = VIDEO_FORMAT_UNKNOWN;
      break;
    }
  info->bitrate = 400.0 * (double)(((data[4] << 10) & 0x0003FC00UL) | ((data[5] << 2) & 0x000003FCUL) | (((data[6] & 0xC0) >> 6) & 0x00000003UL));

  return true;
}
