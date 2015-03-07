/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * AC3 Audio Header: http://www.atsc.org/standards/a_52a.pdf
 */

#include "femontools.h"
#include "femonac3.h"

int cFemonAC3::bitrateS[32] =
{
  32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384, 448, 512, 576, 640, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int cFemonAC3::frequencieS[4] =
{
  480, 441, 320, 0
};

int cFemonAC3::frameS[3][32] =
{
  {64, 80,  96,  112, 128, 160, 192, 224, 256, 320, 384, 448, 512, 640, 768,  896,  1024, 1152, 1280, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {69, 87,  104, 121, 139, 174, 208, 243, 278, 348, 417, 487, 557, 696, 835,  975,  1114, 1253, 1393, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {96, 120, 144, 168, 192, 240, 288, 336, 384, 480, 576, 672, 768, 960, 1152, 1344, 1536, 1728, 1920, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

cFemonAC3::cFemonAC3(cFemonAC3If *audioHandlerP)
: audioHandlerM(audioHandlerP)
{
}

cFemonAC3::~cFemonAC3()
{
}

bool cFemonAC3::processAudio(const uint8_t *bufP, int lenP)
{
  int fscod, frmsizcod, bsmod, acmod;
  int centermixlevel = AUDIO_CENTER_MIX_LEVEL_INVALID;
  int surroundmixlevel = AUDIO_SURROUND_MIX_LEVEL_INVALID;
  int dolbysurroundmode = AUDIO_DOLBY_SURROUND_MODE_INVALID;
  cFemonBitStream bs(bufP, lenP * 8);

  if (!audioHandlerM)
     return false;

  // skip PES header
  if (!PesLongEnough(lenP))
      return false;
  bs.SkipBits(8 * PesPayloadOffset(bufP));

  // http://rmworkshop.com/dvd_info/related_info/ac3hdr.htm

  // AC3 audio detection
  if (bs.GetBits(16) != 0x0B77)     // syncword
     return false;

  bs.SkipBits(16);                  // CRC1

  fscod = bs.GetBits(2);            // sampling rate values
  frmsizcod = bs.GetBits(6);        // frame size code

  bs.SkipBits(5);                   // bitstream id
  bsmod = bs.GetBits(3);            // bitstream mode
  acmod = bs.GetBits(3);            // audio coding mode

  // 3 front channels
  if ((acmod & 0x01) && (acmod != 0x01))
     centermixlevel = bs.GetBits(2);

  // if a surround channel exists
  if (acmod & 0x04)
     surroundmixlevel = bs.GetBits(2);

  // if in 2/0 mode
  if (acmod == 0x02)
      dolbysurroundmode = bs.GetBits(2);

  audioHandlerM->SetAC3Bitrate(1000 * bitrateS[frmsizcod >> 1]);
  audioHandlerM->SetAC3SamplingFrequency(100 * frequencieS[fscod]);
  audioHandlerM->SetAC3Bitstream(bsmod);
  audioHandlerM->SetAC3AudioCoding(acmod);
  audioHandlerM->SetAC3CenterMix(centermixlevel);
  audioHandlerM->SetAC3SurroundMix(surroundmixlevel);
  audioHandlerM->SetAC3DolbySurround(dolbysurroundmode);

  audioHandlerM->SetAC3LFE(bs.GetBit());       // low frequency effects on
  audioHandlerM->SetAC3Dialog(bs.GetBits(5));  // dialog normalization

  return true;
}
