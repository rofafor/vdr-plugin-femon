/*
 * svdrpservice.h: Public interface of the plugin's services
 *
 * See the README file for copyright information and how to reach the author.
 */

#ifndef _SVDRPSERVICE__H
#define _SVDRPSERVICE__H

#include <vdr/tools.h>

class cLine: public cListObject {
private:
  char *lineM;
public:
  const char *Text()      { return lineM; }
  cLine(const char *strP) { lineM = strP ? strdup(strP) : NULL; };
  virtual ~cLine()        { if (lineM) free(lineM); };
};

struct SvdrpConnection_v1_0 {
  // in
  cString        serverIp;
  unsigned short serverPort;
  bool           shared;
  // in+out
  int            handle;
};

struct SvdrpCommand_v1_0 {
  // in
  cString        command;
  int            handle;
  // out
  cList<cLine>   reply;
  unsigned short responseCode;
};

#endif //_SVDRPSERVICE__H
