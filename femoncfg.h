#ifndef __FEMONCFG_H
#define __FEMONCFG_H

struct cFemonConfig
{
public:
  cFemonConfig(void);
  int  hidemenu;
  int  position;
  int  interval;
  int  redlimit;
  int  greenlimit;
};

extern cFemonConfig femonConfig;

#endif // __FEMONCFG_H
