/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __FEMONAAC_H
#define __FEMONAAC_H

#include "femonaudio.h"

bool getAACAudioInfo(uint8_t *buf, int len, audio_info_t *info);

#endif //__FEMONAAC_H
