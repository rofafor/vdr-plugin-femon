/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include "femontools.h"
#include "femonaac.h"

#define IS_HEAAC_AUDIO(buf) (((buf)[0] == 0xFF) && (((buf)[1] & 0xF6) == 0xF0))

static unsigned int samplerates[16] =
{
  96000, 88200, 64000, 48000, 44100, 32000, 24000, 22050, 16000, 12000, 11025, 8000, -1, -1, -1, -1
};

bool getAACAudioInfo(uint8_t *buf, int len, audio_info_t *info)
{
  // HE-AAC audio detection, search for syncword with layer set to 0
  if ((len < 4) || !IS_HEAAC_AUDIO(buf))
     return false;

  /* ADTS Fixed Header:
   * syncword                 12b  always: '111111111111'
   * id                        1b  0: MPEG-4, 1: MPEG-2
   * layer                     2b  always: '00'
   * protection_absent         1b
   * profile                   2b  0: Main profile AAC MAIN 1: Low Complexity profile (LC) AAC LC 2: Scalable Sample Rate profile (SSR) AAC SSR 3: (reserved) AAC LTP
   * sampling_frequency_index  4b
   * private_bit               1b
   * channel_configuration     3b
   * original/copy             1b
   * home                      1b
   * emphasis                  2b only if ID == 0 (ie MPEG-4)
   */

  int sampling_frequency_index = (buf[2] & 0x03C) >> 2;
  int channel_configuration = ((buf[2] & 0x01) << 2) | ((buf[3] & 0xC0) >> 6);

  info->codec = AUDIO_CODEC_HEAAC;
  info->bitrate = AUDIO_BITRATE_RESERVED;

  switch (channel_configuration) {
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

  switch (sampling_frequency_index) {
    case 0xC ... 0xF:
         info->samplingFrequency = AUDIO_SAMPLING_FREQUENCY_RESERVED;
         break;

    default:
         info->samplingFrequency = samplerates[sampling_frequency_index];
         break;
  }

  return true;
}
