/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#ifndef __FEMONTOOLS_H
#define __FEMONTOOLS_H

#include <stdint.h>
#include <vdr/tools.h>

#ifdef DEBUG
#define Dprintf(x...) printf(x);
#else
#define Dprintf(x...) ;
#endif

#define FRONTEND_DEVICE "/dev/dvb/adapter%d/frontend%d"

cString getFrontendName(int cardIndex = 0);
cString getFrontendStatus(int cardIndex = 0);
uint16_t getSNR(int cardIndex = 0);
uint16_t getSignal(int cardIndex = 0);
uint32_t getBER(int cardIndex = 0);
uint32_t getUNC(int cardIndex = 0);

#endif // __FEMONTOOLS_H
