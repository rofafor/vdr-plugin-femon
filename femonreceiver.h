#ifndef __FEMONRECEIVER_H
#define __FEMONRECEIVER_H

#include <vdr/receiver.h>

class cFemonReceiver : public cReceiver {
private:
  int m_VPid;
  int m_APid;
  int m_VideoCount;
  int m_AudioCount;

protected:
  virtual void Activate(bool On);
  virtual void Receive(uchar *Data, int Length);

public:
  cFemonReceiver(int Ca, int Vpid, int Apid);
  virtual ~cFemonReceiver();

  int VideoPacketCount(void);
  int AudioPacketCount(void);
  };

#endif //__FEMONRECEIVER_H

