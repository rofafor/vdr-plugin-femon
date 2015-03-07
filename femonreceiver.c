/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include <unistd.h>
#include "femontools.h"
#include "femoncfg.h"
#include "femonreceiver.h"

cFemonReceiver::cFemonReceiver(const cChannel *channelP, int aTrackP, int dTrackP)
: cReceiver(channelP),
  cThread("femon receiver"),
  mutexM(),
  sleepM(),
  activeM(false),
  detectH264M(this),
  detectMpegM(this, this),
  detectAacM(this),
  detectLatmM(this),
  detectAc3M(this),
  videoBufferM(KILOBYTE(512), TS_SIZE, false, "Femon video"),
  videoTypeM(channelP ? channelP->Vtype(): 0),
  videoPidM(channelP ? channelP->Vpid() : 0),
  videoPacketCountM(0),
  videoBitRateM(0.0),
  videoValidM(false),
  audioBufferM(KILOBYTE(256), TS_SIZE, false, "Femon audio"),
  audioPidM(channelP ? channelP->Apid(aTrackP) : 0),
  audioPacketCountM(0),
  audioBitRateM(0.0),
  audioValidM(false),
  ac3BufferM(KILOBYTE(256), TS_SIZE, false, "Femon AC3"),
  ac3PidM(channelP ? channelP->Dpid(dTrackP) : 0),
  ac3PacketCountM(0),
  ac3BitRateM(0),
  ac3ValidM(false)
{
  debug("%s()\n", __PRETTY_FUNCTION__);

  SetPids(NULL);
  AddPid(videoPidM);
  AddPid(audioPidM);
  AddPid(ac3PidM);

  videoBufferM.SetTimeouts(0, 100);
  audioBufferM.SetTimeouts(0, 100);
  ac3BufferM.SetTimeouts(0, 100);

  videoInfoM.codec = VIDEO_CODEC_INVALID;
  videoInfoM.format = VIDEO_FORMAT_INVALID;
  videoInfoM.scan = VIDEO_SCAN_INVALID;
  videoInfoM.aspectRatio = VIDEO_ASPECT_RATIO_INVALID;
  videoInfoM.width = 0;
  videoInfoM.height = 0;
  videoInfoM.frameRate = 0;
  videoInfoM.bitrate = AUDIO_BITRATE_INVALID;
  audioInfoM.codec = AUDIO_CODEC_UNKNOWN;
  audioInfoM.bitrate = AUDIO_BITRATE_INVALID;
  audioInfoM.samplingFrequency = AUDIO_SAMPLING_FREQUENCY_INVALID;
  audioInfoM.channelMode = AUDIO_CHANNEL_MODE_INVALID;
  ac3InfoM.bitrate = AUDIO_BITRATE_INVALID;
  ac3InfoM.samplingFrequency = AUDIO_SAMPLING_FREQUENCY_INVALID;
  ac3InfoM.bitstreamMode = AUDIO_BITSTREAM_MODE_INVALID;
  ac3InfoM.audioCodingMode = AUDIO_CODING_MODE_INVALID;
  ac3InfoM.dolbySurroundMode = AUDIO_DOLBY_SURROUND_MODE_INVALID;
  ac3InfoM.centerMixLevel = AUDIO_CENTER_MIX_LEVEL_INVALID;
  ac3InfoM.surroundMixLevel = AUDIO_SURROUND_MIX_LEVEL_INVALID;
  ac3InfoM.dialogLevel = 0;
  ac3InfoM.lfe = false;
}

cFemonReceiver::~cFemonReceiver(void)
{
  debug("%s()\n", __PRETTY_FUNCTION__);
  Deactivate();
}

void cFemonReceiver::Deactivate(void)
{
  debug("%s()\n", __PRETTY_FUNCTION__);
  Detach();
  if (activeM) {
     activeM = false;
     sleepM.Signal();
     if (Running())
        Cancel(3);
     }
}

void cFemonReceiver::Activate(bool onP)
{
  debug("%s(%d)\n", __PRETTY_FUNCTION__, onP);
  if (onP)
     Start();
  else
     Deactivate();
}

void cFemonReceiver::Receive(uchar *dataP, int lengthP)
{
  // TS packet length: TS_SIZE
  if (Running() && (*dataP == TS_SYNC_BYTE) && (lengthP == TS_SIZE)) {
     int len, pid = TsPid(dataP);
     if (pid == videoPidM) {
        ++videoPacketCountM;
        len = videoBufferM.Put(dataP, lengthP);
        if (len != lengthP) {
           videoBufferM.ReportOverflow(lengthP - len);
           videoBufferM.Clear();
           }
        }
     else if (pid == audioPidM) {
        ++audioPacketCountM;
        len = audioBufferM.Put(dataP, lengthP);
        if (len != lengthP) {
           audioBufferM.ReportOverflow(lengthP - len);
           audioBufferM.Clear();
           }
        }
     else if (pid == ac3PidM) {
        ++ac3PacketCountM;
        len = ac3BufferM.Put(dataP, lengthP);
        if (len != lengthP) {
           ac3BufferM.ReportOverflow(lengthP - len);
           ac3BufferM.Clear();
           }
        }
     }
}

void cFemonReceiver::Action(void)
{
  debug("%s()\n", __PRETTY_FUNCTION__);
  cTimeMs calcPeriod(0);
  activeM = true;

  while (Running() && activeM) {
    uint8_t *Data;
    double timeout;
    int len, Length;
    bool processed = false;

    // process available video data
    while ((Data = videoBufferM.Get(Length))) {
      if (!activeM || (Length < TS_SIZE))
         break;
      Length = TS_SIZE;
      if (*Data != TS_SYNC_BYTE) {
         for (int i = 1; i < Length; ++i) {
             if (Data[i] == TS_SYNC_BYTE) {
                Length = i;
                break;
                }
             }
         videoBufferM.Del(Length);
         continue;
         }
      processed = true;
      if (TsPayloadStart(Data)) {
         while (const uint8_t *p = videoAssemblerM.GetPes(len)) {
           if (videoTypeM == 0x1B) { // MPEG4
              if (detectH264M.processVideo(p, len)) {
                 videoValidM = true;
                 break;
                 }
              }
           else {
              if (detectMpegM.processVideo(p, len)) {
                 videoValidM = true;
                 break;
                 }
              }
           }
         videoAssemblerM.Reset();
         }
      videoAssemblerM.PutTs(Data, Length);
      videoBufferM.Del(Length);
      }

    // process available audio data
    while ((Data = audioBufferM.Get(Length))) {
      if (!activeM || (Length < TS_SIZE))
         break;
      Length = TS_SIZE;
      if (*Data != TS_SYNC_BYTE) {
         for (int i = 1; i < Length; ++i) {
             if (Data[i] == TS_SYNC_BYTE) {
                Length = i;
                break;
                }
             }
         audioBufferM.Del(Length);
         continue;
         }
      processed = true;
      if (const uint8_t *p = audioAssemblerM.GetPes(len)) {
         if (detectAacM.processAudio(p, len) || detectLatmM.processAudio(p, len) || detectMpegM.processAudio(p, len))
            audioValidM = true;
         audioAssemblerM.Reset();
         }
      audioAssemblerM.PutTs(Data, Length);
      audioBufferM.Del(Length);
      }

    // process available dolby data
    while ((Data = ac3BufferM.Get(Length))) {
      if (!activeM || (Length < TS_SIZE))
         break;
      Length = TS_SIZE;
      if (*Data != TS_SYNC_BYTE) {
         for (int i = 1; i < Length; ++i) {
             if (Data[i] == TS_SYNC_BYTE) {
                Length = i;
                break;
                }
             }
         ac3BufferM.Del(Length);
         continue;
         }
      processed = true;
      if (const uint8_t *p = ac3AssemblerM.GetPes(len)) {
         if (detectAc3M.processAudio(p, len))
            ac3ValidM = true;
         ac3AssemblerM.Reset();
         }
      ac3AssemblerM.PutTs(Data, Length);
      ac3BufferM.Del(Length);
      }

    // calculate bitrates
    timeout = double(calcPeriod.Elapsed());
    if (activeM && (timeout >= (100.0 * FemonConfig.calcinterval))) {
       // TS packet 188 bytes - 4 byte header; MPEG standard defines 1Mbit = 1000000bit
       // PES headers should be compensated!
       videoBitRateM     = (1000.0 * 8.0 * 184.0 * videoPacketCountM) / timeout;
       videoPacketCountM = 0;
       audioBitRateM     = (1000.0 * 8.0 * 184.0 * audioPacketCountM) / timeout;
       audioPacketCountM = 0;
       ac3BitRateM       = (1000.0 * 8.0 * 184.0 * ac3PacketCountM)   / timeout;
       ac3PacketCountM   = 0;
       calcPeriod.Set(0);
       }

    if (!processed)
       sleepM.Wait(10); // to avoid busy loop and reduce cpu load
    }
}
