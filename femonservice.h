/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __FEMONSERVICE_H
#define __FEMONSERVICE_H

struct FemonService_v1_1 {
  cString fe_name;
  cString fe_status;
  double fe_cnr;
  double fe_signal;
  double fe_ber;
  double fe_per;
  double video_bitrate;
  double audio_bitrate;
  double dolby_bitrate;
  };

#endif //__FEMONSERVICE_H

