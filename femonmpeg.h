/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __FEMONMPEG_H
#define __FEMONMPEG_H

#include "femonaudio.h"
#include "femonvideo.h"

bool getMPEGAudioInfo(uint8_t *buf, int len, audio_info_t *info);
bool getMPEGVideoInfo(uint8_t *buf, int len, video_info_t *info);

#endif //__FEMONMPEG_H
