/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __FEMONH264_H
#define __FEMONH264_H

#include "femonvideo.h"

bool getH264VideoInfo(uint8_t *buf, int len, video_info_t *info);

#endif //__FEMONH264_H
