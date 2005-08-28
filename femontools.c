/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/dvb/frontend.h>
#include "femontools.h"

cString getFrontendName(int cardIndex)
{
  struct dvb_frontend_info value;
  char *dev = NULL;

  asprintf(&dev, FRONTEND_DEVICE, cardIndex, 0);
  int fe = open(dev, O_RDONLY | O_NONBLOCK);
  free(dev);
  if (fe < 0)
     return NULL;
  ioctl(fe, FE_GET_INFO, &value);
  close(fe);

  return (cString::sprintf("#%d %s", cardIndex, value.name));
}

cString getFrontendStatus(int cardIndex)
{
  fe_status_t value;
  char *dev = NULL;

  asprintf(&dev, FRONTEND_DEVICE, cardIndex, 0);
  int fe = open(dev, O_RDONLY | O_NONBLOCK);
  free(dev);
  if (fe < 0)
     return NULL;
  CHECK(ioctl(fe, FE_READ_STATUS, &value));
  close(fe);

  return (cString::sprintf("%s:%s:%s:%s:%s", (value & FE_HAS_LOCK) ? "LOCKED" : "-", (value & FE_HAS_SIGNAL) ? "SIGNAL" : "-", (value & FE_HAS_CARRIER) ? "CARRIER" : "-", (value & FE_HAS_VITERBI) ? "VITERBI" : "-", (value & FE_HAS_SYNC) ? "SYNC" : "-"));
}

uint16_t getSignal(int cardIndex)
{
  uint16_t value = 0;
  char *dev = NULL;

  asprintf(&dev, FRONTEND_DEVICE, cardIndex, 0);
  int fe = open(dev, O_RDONLY | O_NONBLOCK);
  free(dev);
  if (fe < 0)
     return (value);
  CHECK(ioctl(fe, FE_READ_SIGNAL_STRENGTH, &value));
  close(fe);

  return (value);
}

uint16_t getSNR(int cardIndex)
{
  uint16_t value = 0;
  char *dev = NULL;

  asprintf(&dev, FRONTEND_DEVICE, cardIndex, 0);
  int fe = open(dev, O_RDONLY | O_NONBLOCK);
  free(dev);
  if (fe < 0)
     return (value);
  CHECK(ioctl(fe, FE_READ_SNR, &value));
  close(fe);

  return (value);
}

uint32_t getBER(int cardIndex)
{
  uint32_t value = 0;
  char *dev = NULL;

  asprintf(&dev, FRONTEND_DEVICE, cardIndex, 0);
  int fe = open(dev, O_RDONLY | O_NONBLOCK);
  free(dev);
  if (fe < 0)
     return (value);
  CHECK(ioctl(fe, FE_READ_BER, &value));
  close(fe);

  return (value);
}

uint32_t getUNC(int cardIndex)
{
  uint32_t value = 0;
  char *dev = NULL;

  asprintf(&dev, FRONTEND_DEVICE, cardIndex, 0);
  int fe = open(dev, O_RDONLY | O_NONBLOCK);
  free(dev);
  if (fe < 0)
     return (value);
  CHECK(ioctl(fe, FE_READ_UNCORRECTED_BLOCKS, &value));
  close(fe);

  return (value);
}

double getVideoBitrate(void)
{
  double value = 0.0;

  // Not yet implemented.

  return (value);
}

double getAudioBitrate(void)
{
  double value = 0.0;

  // Not yet implemented.

  return (value);
}
