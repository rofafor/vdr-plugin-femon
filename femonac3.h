/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __FEMONAC3_H
#define __FEMONAC3_H

#include "femonaudio.h"

bool getAC3AudioInfo(uint8_t *buf, int len, ac3_info_t *info);

#endif //__FEMONAC3_H
