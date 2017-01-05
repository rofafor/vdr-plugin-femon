/*
 * h265.c: Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include "log.h"
#include "tools.h"
#include "h265.h"

cFemonH265::cFemonH265(cFemonVideoIf *videoHandlerP)
: videoHandlerM(videoHandlerP),
  widthM(0),
  heightM(0),
  aspectRatioM(VIDEO_ASPECT_RATIO_INVALID),
  formatM(VIDEO_FORMAT_INVALID),
  frameRateM(0),
  bitRateM(0),
  scanM(VIDEO_SCAN_INVALID)
{
  reset();
}

cFemonH265::~cFemonH265()
{
}

bool cFemonH265::processVideo(const uint8_t *bufP, int lenP)
{
  bool aud_found = false;
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
                aud_found = true;
                debug2("%s Found NAL AUD at offset %d/%d", __PRETTY_FUNCTION__, int(buf - start), lenP);
                }
             break;

        default:
             break;
        }

      if (aud_found)
         break;

      buf += consumed + 4;
      }

  if (aud_found) {
     videoHandlerM->SetVideoCodec(VIDEO_CODEC_H265);
     //videoHandlerM->SetVideoFormat(formatM);
     //videoHandlerM->SetVideoSize(widthM, heightM);
     //videoHandlerM->SetVideoAspectRatio(aspectRatioM);
     //videoHandlerM->SetVideoBitrate(bitRateM);
     //videoHandlerM->SetVideoScan(scanM);
     //videoHandlerM->SetVideoFramerate((scanM == VIDEO_SCAN_PROGRESSIVE) ? (frameRateM / 2) : frameRateM);
     }

  return aud_found;
}

void cFemonH265::reset()
{
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

  while (s < lenP) {
    if (!srcP[s] && !srcP[s + 1]) {
       // hit 00 00 xx
       dstP[d] = dstP[d + 1] = 0;
       s += 2;
       d += 2;
       if (srcP[s] == 3) {
          s++; // 00 00 03 xx --> 00 00 xx
          if (s >= lenP)
             return d;
          }
       }
    dstP[d++] = srcP[s++];
    }

  return d;
}
