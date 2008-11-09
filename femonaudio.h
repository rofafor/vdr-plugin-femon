/*
 * Frontend Status Monitor plugin for the AUDIO Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __FEMONAUDIO_H
#define __FEMONAUDIO_H

enum eAudioCodec {
  AUDIO_CODEC_INVALID = -1,
  AUDIO_CODEC_UNKNOWN,
  AUDIO_CODEC_MPEG1_I,
  AUDIO_CODEC_MPEG1_II,
  AUDIO_CODEC_MPEG1_III,
  AUDIO_CODEC_MPEG2_I,
  AUDIO_CODEC_MPEG2_II,
  AUDIO_CODEC_MPEG2_III,
  AUDIO_CODEC_HEAAC
  };

enum eAudioChannelMode {
  AUDIO_CHANNEL_MODE_INVALID = -1,
  AUDIO_CHANNEL_MODE_STEREO,
  AUDIO_CHANNEL_MODE_JOINT_STEREO,
  AUDIO_CHANNEL_MODE_DUAL,
  AUDIO_CHANNEL_MODE_SINGLE
  };

enum eAudioBitrate {
  AUDIO_BITRATE_RESERVED = -3,
  AUDIO_BITRATE_FREE = -2,
  AUDIO_BITRATE_INVALID = -1
  };

enum eAudioSamplingFrequency {
  AUDIO_SAMPLING_FREQUENCY_RESERVED = -2,
  AUDIO_SAMPLING_FREQUENCY_INVALID = -1
  };

enum eAudioCenterMixLevel {
  AUDIO_CENTER_MIX_LEVEL_INVALID = -1,
  AUDIO_CENTER_MIX_LEVEL_MINUS_3dB,
  AUDIO_CENTER_MIX_LEVEL_MINUS_4_5dB,
  AUDIO_CENTER_MIX_LEVEL_MINUS_6dB,
  AUDIO_CENTER_MIX_LEVEL_RESERVED
  };

enum eAudioSurroundMixLevel {
  AUDIO_SURROUND_MIX_LEVEL_INVALID = -1,
  AUDIO_SURROUND_MIX_LEVEL_MINUS_3dB,
  AUDIO_SURROUND_MIX_LEVEL_MINUS_6dB,
  AUDIO_SURROUND_MIX_LEVEL_0_dB,
  AUDIO_SURROUND_MIX_LEVEL_RESERVED
  };

enum eAudioDolbySurroundMode {
  AUDIO_DOLBY_SURROUND_MODE_INVALID = -1,
  AUDIO_DOLBY_SURROUND_MODE_NOT_INDICATED,
  AUDIO_DOLBY_SURROUND_MODE_NOT_DOLBYSURROUND,
  AUDIO_DOLBY_SURROUND_MODE_DOLBYSURROUND,
  AUDIO_DOLBY_SURROUND_MODE_RESERVED
  };

enum eAudioBitstreamMode {
  AUDIO_BITSTREAM_MODE_INVALID = -1,
  AUDIO_BITSTREAM_MODE_CM,
  AUDIO_BITSTREAM_MODE_ME,
  AUDIO_BITSTREAM_MODE_VI,
  AUDIO_BITSTREAM_MODE_HI,
  AUDIO_BITSTREAM_MODE_D,
  AUDIO_BITSTREAM_MODE_C,
  AUDIO_BITSTREAM_MODE_E,
  AUDIO_BITSTREAM_MODE_VO_KAR
  };

enum eAudioCodingMode {
  AUDIO_CODING_MODE_INVALID = -1,
  AUDIO_CODING_MODE_1_1,
  AUDIO_CODING_MODE_1_0,
  AUDIO_CODING_MODE_2_0,
  AUDIO_CODING_MODE_3_0,
  AUDIO_CODING_MODE_2_1,
  AUDIO_CODING_MODE_3_1,
  AUDIO_CODING_MODE_2_2,
  AUDIO_CODING_MODE_3_2,
  };

typedef struct audio_info {
  eAudioCodec codec;             // enum
  double      bitrate;           // kbit/s or eAudioBitrate
  int         samplingFrequency; // Hz or eAudioSamplingFrequency
  int         channelMode;       // eAudioChannelMode
} audio_info_t;

typedef struct ac3_info {
  int         bitrate;           // bit/s or eAudioBitrate
  int         samplingFrequency; // Hz or eAudioSamplingFrequency
  int         bitstreamMode;     // 0..7 or eAudioBitstreamMode
  int         audioCodingMode;   // 0..7 or eAudioCodingMode
  int         dolbySurroundMode; // eAudioDolbySurroundMode
  int         centerMixLevel;    // eAudioCenterMixLevel
  int         surroundMixLevel;  // eAudioSurroundMixLevel
  int         dialogLevel;       // -dB
  bool        lfe;               // boolean
} ac3_info_t;

#endif //__FEMONAUDIO_H
