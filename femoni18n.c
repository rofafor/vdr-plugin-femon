/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#include "femoni18n.h"

const tI18nPhrase Phrases[] = {
  { "DVB Signal Information Monitor (OSD)", // English
    "DVB Signal Informationsanzeige (OSD)", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Portuguκs
    "", // Franηais
    "", // Norsk
    "DVB signaalimittari (OSD)", // suomi
    "", // Polski
    "", // Espaρol
    "", // Ελληνικά
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalΰ
#if VDRVERSNUM >= 10300
    ""  // ΐγααΪΨΩ
#endif
  },
  { "Signal Information", // English
    "Signalinformationen", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Portuguκs
    "", // Franηais
    "", // Norsk
    "Signaalimittari", // suomi
    "", // Polski
    "", // Espaρol
    "", // Ελληνικά
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalΰ
#if VDRVERSNUM >= 10300
    ""  // ΐγααΪΨΩ
#endif
  },
  { "Position", // English
    "Position", // Deutsch
    "", // Slovenski
    "Posizione", // Italiano
    "", // Nederlands
    "", // Portuguκs
    "", // Franηais
    "", // Norsk
    "Sijainti", // suomi
    "", // Polski
    "", // Espaρol
    "", // Ελληνικά
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalΰ
#if VDRVERSNUM >= 10300
    ""  // ΐγααΪΨΩ
#endif
  },
  { "Default Display Mode", // English
    "Standard Anzeigemodus", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Portuguκs
    "", // Franηais 
    "", // Norsk
    "Oletus nδyttφmoodi", // suomi
    "", // Polski   
    "", // Espaρol  
    "", // Ελληνικά
    "", // Svenska
    "", // Romaneste
    "", // Magyar  
    "", // Catalΰ  
#if VDRVERSNUM >= 10300
    ""  // ΐγααΪΨΩ  
#endif
  },
  { "simple", // English
    "einfach", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Portuguκs
    "", // Franηais 
    "", // Norsk
    "suppea", // suomi
    "", // Polski   
    "", // Espaρol  
    "", // Ελληνικά
    "", // Svenska
    "", // Romaneste
    "", // Magyar  
    "", // Catalΰ  
#if VDRVERSNUM >= 10300
    ""  // ΐγααΪΨΩ  
#endif
  },
  { "advanced", // English
    "fortgeschritten", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Portuguκs
    "", // Franηais 
    "", // Norsk
    "laaja", // suomi
    "", // Polski   
    "", // Espaρol  
    "", // Ελληνικά
    "", // Svenska
    "", // Romaneste
    "", // Magyar  
    "", // Catalΰ  
#if VDRVERSNUM >= 10300
    ""  // ΐγααΪΨΩ  
#endif
  },
  { "Hide Mainmenu Entry", // English
    "Hauptmenόeintrag verstecken", // Deutsch
    "", // Slovenski
    "Nascondere Voce Menu", // Italiano
    "", // Nederlands
    "", // Portuguκs
    "", // Franηais
    "", // Norsk
    "Piilota valinta pδδvalikosta", // suomi
    "", // Polski
    "", // Espaρol
    "", // Ελληνικά
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalΰ
#if VDRVERSNUM >= 10300
    ""  // ΐγααΪΨΩ
#endif
  },
  { "OSD Update Interval [0.1s]", // English
    "OSD Updateintervall [0.1s]", // Deutsch
    "", // Slovenski
    "Intervallo di aggiornamento [0.1s]", // Italiano
    "", // Nederlands
    "", // Portuguκs
    "", // Franηais
    "", // Norsk
    "Nδytφn pδivitysvδli [0.1s]", // suomi
    "", // Polski
    "", // Espaρol
    "", // Ελληνικά
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalΰ
#if VDRVERSNUM >= 10300
    ""  // ΐγααΪΨΩ
#endif
  },
  { "Bitrate Calculation", // English
    "Bitratenberechnung", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Portuguκs
    "", // Franηais
    "", // Norsk
    "Bittinopeuden laskenta", // suomi
    "", // Polski
    "", // Espaρol
    "", // Ελληνικά
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalΰ
#if VDRVERSNUM >= 10300
    ""  // ΐγααΪΨΩ
#endif
  },
  { "Calculation Interval [0.1s]", // English
    "Berechnungsintervall [0.1s]", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Portuguκs
    "", // Franηais
    "", // Norsk
    "Laskennan pδivitysvδli [0.1s]", // suomi
    "", // Polski
    "", // Espaρol
    "", // Ελληνικά
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalΰ
#if VDRVERSNUM >= 10300
    ""  // ΐγααΪΨΩ
#endif
  },
  { "Red Limit [%]", // English
    "Grenze Rot [%]", // Deutsch
    "", // Slovenski
    "Limite Rosso [%]", // Italiano
    "", // Nederlands
    "", // Portuguκs
    "", // Franηais
    "", // Norsk
    "Punaisen taso [%]", // suomi
    "", // Polski
    "", // Espaρol
    "", // Ελληνικά
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalΰ
#if VDRVERSNUM >= 10300
    ""  // ΐγααΪΨΩ
#endif
  },
  { "Green Limit [%]", // English
    "Grenze Grόn [%]", // Deutsch
    "", // Slovenski
    "Limite Verde [%]", // Italiano
    "", // Nederlands
    "", // Portuguκs
    "", // Franηais
    "", // Norsk
    "Vihreδn taso [%]", // suomi
    "", // Polski
    "", // Espaρol
    "", // Ελληνικά
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalΰ
#if VDRVERSNUM >= 10300
    ""  // ΐγααΪΨΩ
#endif
  },
  { "Transponder Information", // English
    "Transponderinformation", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Portuguκs
    "", // Franηais
    "", // Norsk   
    "Transponderin tiedot", // suomi
    "", // Polski   
    "", // Espaρol
    "", // Ελληνικά
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalΰ
#if VDRVERSNUM >= 10300
    ""  // ΐγααΪΨΩ
#endif
  },
  { "Satellite Card", // English
    "Satellitenkarte", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Portuguκs
    "", // Franηais
    "", // Norsk
    "Satelliittikortti", // suomi
    "", // Polski
    "", // Espaρol
    "", // Ελληνικά
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalΰ
#if VDRVERSNUM >= 10300
    ""  // ΐγααΪΨΩ
#endif
  },
  { "Cable Card", // English
    "Kabelkarte", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Portuguκs
    "", // Franηais
    "", // Norsk
    "Kaapelikortti", // suomi
    "", // Polski
    "", // Espaρol
    "", // Ελληνικά
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalΰ
#if VDRVERSNUM >= 10300
    ""  // ΐγααΪΨΩ
#endif
  },
  { "Terrestial Card", // English
    "Terrestrische Karte", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Portuguκs
    "", // Franηais
    "", // Norsk
    "Terrestiaalikortti", // suomi
    "", // Polski
    "", // Espaρol
    "", // Ελληνικά
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalΰ
#if VDRVERSNUM >= 10300
    ""  // ΐγααΪΨΩ
#endif
  },
  { NULL }
};

