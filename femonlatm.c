/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include "femontools.h"
#include "femonlatm.h"

int cFemonLATM::bitrateS[3][16] =
{
  {0,  32,  48,  56,  64,  80,  96, 112, 128, 144, 160, 176, 192, 224, 256, -1}, // MPEG-2 Layer I
  {0,   8,  16,  24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, -1}, // MPEG-2 Layer II/III
  {0,   8,  16,  24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, -1}  // MPEG-2 Layer II/III
};

int cFemonLATM::sampleRateS[4] =
{
  22050, 24000, 16000, -1
};

cFemonLATM::cFemonLATM(cFemonAudioIf *audioHandlerP)
: audioHandlerM(audioHandlerP)
{
}

cFemonLATM::~cFemonLATM()
{
}

bool cFemonLATM::processAudio(const uint8_t *bufP, int lenP)
{
  cFemonBitStream bs(bufP, lenP * 8);

  if (!audioHandlerM)
     return false;

  // skip PES header
  if (!PesLongEnough(lenP))
      return false;
  bs.SkipBits(8 * PesPayloadOffset(bufP));

  // MPEG audio detection
  if (bs.GetBits(12) != 0x56E)              // syncword
     return false;

  audioHandlerM->SetAudioCodec(AUDIO_CODEC_LATM);

  if (bs.GetBit() == 0)                     // id: MPEG-1=1, extension to lower sampling frequencies=0
     return true;                           // @todo: lower sampling frequencies support
  int layer = 3 - bs.GetBits(2);            // layer: I=11, II=10, III=01
  bs.SkipBit();                             // protection bit
  int bit_rate_index = bs.GetBits(4);       // bitrate index
  int sampling_frequency = bs.GetBits(2);   // sampling frequency
  bs.SkipBit();                             // padding bit
  bs.SkipBit();                             // private pid
  int mode = bs.GetBits(2);                 // mode

  switch (mode) {
    case 0:
         audioHandlerM->SetAudioChannel(AUDIO_CHANNEL_MODE_STEREO);
         break;

    case 1:
         audioHandlerM->SetAudioChannel(AUDIO_CHANNEL_MODE_JOINT_STEREO);
         break;

    case 2:
         audioHandlerM->SetAudioChannel(AUDIO_CHANNEL_MODE_DUAL);
         break;

    case 3:
         audioHandlerM->SetAudioChannel(AUDIO_CHANNEL_MODE_SINGLE);
         break;

    default:
         audioHandlerM->SetAudioChannel(AUDIO_CHANNEL_MODE_INVALID);
         break;
  }

  if (layer == 3) {
      audioHandlerM->SetAudioBitrate(AUDIO_BITRATE_FREE);
  }
  else {
     switch (bit_rate_index) {
       case 0:
            audioHandlerM->SetAudioBitrate(AUDIO_BITRATE_FREE);
            break;

       case 0xF:
            audioHandlerM->SetAudioBitrate(AUDIO_BITRATE_RESERVED);
            break;

       default:
            audioHandlerM->SetAudioBitrate(1000 * bitrateS[layer][bit_rate_index]);
            break;
     }
  }

  switch (sampling_frequency) {
    case 3:
         audioHandlerM->SetAudioSamplingFrequency(AUDIO_SAMPLING_FREQUENCY_RESERVED);
         break;

    default:
         audioHandlerM->SetAudioSamplingFrequency(sampleRateS[sampling_frequency]);
         break;
  }

  return true;
}
