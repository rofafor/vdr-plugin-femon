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
    "Visualizza le informazioni sul segnale DVB", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "DVB signaalimittari (OSD)", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
    "¼ŞİØâŞà ÚĞçÕáâÒĞ áØÓİĞÛĞ", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
  },
  { "Signal Information", // English
    "Signalinformationen", // Deutsch
    "", // Slovenski
    "Informazioni sul segnale", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Signaalimittari", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
    "ÁØÓİĞÛ", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
  },
  { "Position", // English
    "Position", // Deutsch
    "", // Slovenski
    "Posizione", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Sijainti", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
    "ÀĞ×ÜÕéÕİØÕ ŞÚİĞ", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
  },
  { "Default Display Mode", // English
    "Standard Anzeigemodus", // Deutsch
    "", // Slovenski
    "Modo di visualizzazione standard", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français 
    "", // Norsk
    "Oletus näyttömoodi", // suomi
    "", // Polski   
    "", // Español  
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar  
    "", // Català  
    "ÀÕÖØÜ ßŞ ãÜŞÛçĞİØî", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
  },
  { "simple", // English
    "einfach", // Deutsch
    "", // Slovenski
    "semplice", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français 
    "", // Norsk
    "suppea", // suomi
    "", // Polski   
    "", // Español  
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar  
    "", // Català  
    "ßàŞáâŞÙ", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
  },
  { "advanced", // English
    "fortgeschritten", // Deutsch
    "", // Slovenski
    "avanzato", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français 
    "", // Norsk
    "laaja", // suomi
    "", // Polski   
    "", // Español  
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar  
    "", // Català  
    "àĞáèØàÕİİëÙ", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
  },
  { "Hide Mainmenu Entry", // English
    "Hauptmenüeintrag verstecken", // Deutsch
    "", // Slovenski
    "Nascondi voce menù", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Piilota valinta päävalikosta", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
    "ÁÚàëâì ÚŞÜĞİÔã Ò ÓÛĞÒİŞÜ ÜÕİî", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
  },
  { "OSD Update Interval [0.1s]", // English
    "OSD Updateintervall [0.1s]", // Deutsch
    "", // Slovenski
    "Intervallo di aggiornamento [0.1s]", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Näytön päivitysväli [0.1s]", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
    "ÇĞáâŞâĞ ŞÑİŞÒÛÕİØï (0,1 áÕÚ)", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
  },
  { "Bitrate Calculation", // English
    "Bitratenberechnung", // Deutsch
    "", // Slovenski
    "Calcolo Bitrate", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Bittinopeuden laskenta", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
    "ÀĞáçñâ áÚŞàŞáâØ ßŞâŞÚĞ ÔĞİİëå", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
  },
  { "Calculation Interval [0.1s]", // English
    "Berechnungsintervall [0.1s]", // Deutsch
    "", // Slovenski
    "Intervallo di calcolo [0.1s]", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Laskennan päivitysväli [0.1s]", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
    "ÇĞáâŞâĞ ßÕàÕáçñâĞ (0,1 áÕÚ)", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
  },
  { "Red Limit [%]", // English
    "Grenze Rot [%]", // Deutsch
    "", // Slovenski
    "Limite rosso [%]", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Punaisen taso [%]", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
    "ºàĞáİĞï ×ŞİĞ áÛĞÑŞÓŞ áØÓİĞÛĞ ÔŞ (%)", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
  },
  { "Green Limit [%]", // English
    "Grenze Grün [%]", // Deutsch
    "", // Slovenski
    "Limite verde [%]", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Vihreän taso [%]", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
    "·ÕÛñİĞï ×ŞİĞ áØÛìİŞÓŞ áØÓİĞÛĞ Şâ (%)", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
  },
  { "Transponder Information", // English
    "Transponderinformation", // Deutsch
    "", // Slovenski
    "Informazioni transponder", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk   
    "Transponderin tiedot", // suomi
    "", // Polski   
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
    "ÁÒÕÔÕİØï Ş ÚĞİĞÛÕ", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
  },
  { "Satellite Card", // English
    "Satellitenkarte", // Deutsch
    "", // Slovenski
    "Scheda satellitare", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Satelliittikortti", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
    "¿ÛĞâĞ áßãâİØÚŞÒŞÓŞ ßàØñÜĞ", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
  },
  { "Cable Card", // English
    "Kabelkarte", // Deutsch
    "", // Slovenski
    "Scheda via cavo", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Kaapelikortti", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
    "¿ÛĞâĞ ÚĞÑÕÛìİŞÓŞ ßàØñÜĞ", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
  },
  { "Terrestial Card", // English
    "Terrestrische Karte", // Deutsch
    "", // Slovenski
    "Scheda terrestre", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Terrestiaalikortti", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
    "¿ÛĞâĞ íäØàİŞÓŞ ßàØñÜĞ", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
  },
  { "Use Syslog Output", // English
    "Benutze syslog Ausgabe", // Deutsch
    "", // Slovenski
    "Registra su syslog", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Kirjaa systeemilokiin", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
    "¸áßŞÛì×ŞÒĞâì áÛãÖÑã syslog", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
  },
  { "LOCK", // English
    "LOCK", // Deutsch
    "LOCK", // Slovenski
    "LOCK", // Italiano
    "LOCK", // Nederlands
    "LOCK", // Português
    "LOCK", // Français
    "LOCK", // Norsk
    "LOCK", // suomi
    "LOCK", // Polski
    "LOCK", // Español
    "LOCK", // ÅëëçíéêÜ (Greek)
    "LOCK", // Svenska
    "LOCK", // Romaneste
    "LOCK", // Magyar
    "LOCK", // Català
    "·°Å²°Â", // ÀãááÚØÙ (Russian)
    "LOCK", // Hrvatski (Croatian)
  },
  { "SIGNAL", // English
    "SIGNAL", // Deutsch
    "SIGNAL", // Slovenski
    "SIGNAL", // Italiano
    "SIGNAL", // Nederlands
    "SIGNAL", // Português
    "SIGNAL", // Français
    "SIGNAL", // Norsk
    "SIGNAL", // suomi
    "SIGNAL", // Polski
    "SIGNAL", // Español
    "SIGNAL", // ÅëëçíéêÜ (Greek)
    "SIGNAL", // Svenska
    "SIGNAL", // Romaneste
    "SIGNAL", // Magyar
    "SIGNAL", // Català
    "Á¸³½°»", // ÀãááÚØÙ (Russian)
    "SIGNAL", // Hrvatski (Croatian)
  },
  { "CARRIER", // English
    "CARRIER", // Deutsch
    "CARRIER", // Slovenski
    "CARRIER", // Italiano
    "CARRIER", // Nederlands
    "CARRIER", // Português
    "CARRIER", // Français
    "CARRIER", // Norsk
    "CARRIER", // suomi
    "CARRIER", // Polski
    "CARRIER", // Español
    "CARRIER", // ÅëëçíéêÜ (Greek)
    "CARRIER", // Svenska
    "CARRIER", // Romaneste
    "CARRIER", // Magyar
    "CARRIER", // Català
    "½µÁÃÉ°Ï", // ÀãááÚØÙ (Russian)
    "CARRIER", // Hrvatski (Croatian)
  },
  { "VITERBI", // English
    "VITERBI", // Deutsch
    "VITERBI", // Slovenski
    "VITERBI", // Italiano
    "VITERBI", // Nederlands
    "VITERBI", // Português
    "VITERBI", // Français
    "VITERBI", // Norsk
    "VITERBI", // suomi
    "VITERBI", // Polski
    "VITERBI", // Español
    "VITERBI", // ÅëëçíéêÜ
    "VITERBI", // Svenska
    "VITERBI", // Romaneste
    "VITERBI", // Magyar
    "VITERBI", // Català
    "²¸ÂµÀ±¸", // ÀãááÚØÙ (Russian)
    "VITERBI", // Hrvatski (Croatian)
  },
  { "SYNC", // English
    "SYNC", // Deutsch
    "SYNC", // Slovenski
    "SYNC", // Italiano
    "SYNC", // Nederlands
    "SYNC", // Português
    "SYNC", // Français
    "SYNC", // Norsk
    "SYNC", // suomi
    "SYNC", // Polski
    "SYNC", // Español
    "SYNC", // ÅëëçíéêÜ (Greek)
    "SYNC", // Svenska
    "SYNC", // Romaneste
    "SYNC", // Magyar
    "SYNC", // Català
    "Á¸½ÅÀ", // ÀãááÚØÙ (Russian)
    "SYNC", // Hrvatski (Croatian)
  },
  { "Audio", // English
    "Audio", // Deutsch
    "Audio", // Slovenski
    "Audio", // Italiano
    "Audio", // Nederlands
    "Audio", // Português
    "Audio", // Français
    "Audio", // Norsk
    "Audio", // suomi
    "Audio", // Polski
    "Audio", // Español
    "Audio", // ÅëëçíéêÜ (Greek)
    "Audio", // Svenska
    "Audio", // Romaneste
    "Audio", // Magyar
    "Audio", // Català
    "°ãÔØŞ", // ÀãááÚØÙ (Russian)
    "Audio", // Hrvatski (Croatian)
  },
  { "Video", // English
    "Video", // Deutsch
    "Video", // Slovenski
    "Video", // Italiano
    "Video", // Nederlands
    "Video", // Português
    "Video", // Français
    "Video", // Norsk
    "Video", // suomi
    "Video", // Polski
    "Video", // Español
    "Video", // ÅëëçíéêÜ (Greek)
    "Video", // Svenska
    "Video", // Romaneste
    "Video", // Magyar
    "Video", // Català
    "²ØÔÕŞ", // ÀãááÚØÙ (Russian)
    "Video", // Hrvatski (Croatian)
  },
  { "Auto", // English
    "Auto", // Deutsch
    "Auto", // Slovenski
    "Auto", // Italiano
    "Auto", // Nederlands
    "Auto", // Português
    "Auto", // Français
    "Auto", // Norsk
    "Auto", // suomi
    "Auto", // Polski
    "Auto", // Español
    "Auto", // ÅëëçíéêÜ (Greek)
    "Auto", // Svenska
    "Auto", // Romaneste
    "Auto", // Magyar
    "Auto", // Català
    "ĞÒâŞ", // ÀãááÚØÙ (Russian)
    "Auto", // Hrvatski (Croatian)
  },
  { "None", // English
    "None", // Deutsch
    "None", // Slovenski
    "None", // Italiano
    "None", // Nederlands
    "None", // Português
    "None", // Français
    "None", // Norsk
    "None", // suomi
    "None", // Polski
    "None", // Español
    "None", // ÅëëçíéêÜ (Greek)
    "None", // Svenska
    "None", // Romaneste
    "None", // Magyar
    "None", // Català
    "İÕâ", // ÀãááÚØÙ (Russian)
    "None", // Hrvatski (Croatian)
  },
  { "Off", // English
    "Off", // Deutsch
    "Off", // Slovenski
    "Off", // Italiano
    "Off", // Nederlands
    "Off", // Português
    "Off", // Français
    "Off", // Norsk
    "Off", // suomi
    "Off", // Polski
    "Off", // Español
    "Off", // ÅëëçíéêÜ (Greek)
    "Off", // Svenska
    "Off", // Romaneste
    "Off", // Magyar
    "Off", // Català
    "ÒëÚÛ", // ÀãááÚØÙ (Russian)
    "Off", // Hrvatski (Croatian)
  },
  { "On", // English
    "On", // Deutsch
    "On", // Slovenski
    "On", // Italiano
    "On", // Nederlands
    "On", // Português
    "On", // Français
    "On", // Norsk
    "On", // suomi
    "On", // Polski
    "On", // Español
    "On", // ÅëëçíéêÜ (Greek)
    "On", // Svenska
    "On", // Romaneste
    "On", // Magyar
    "On", // Català
    "ÒÚÛ", // ÀãááÚØÙ (Russian)
    "On", // Hrvatski (Croatian)
  },
  { "MHz", // English
    "MHz", // Deutsch
    "MHz", // Slovenski
    "MHz", // Italiano
    "MHz", // Nederlands
    "MHz", // Português
    "MHz", // Français
    "MHz", // Norsk
    "MHz", // suomi
    "MHz", // Polski
    "MHz", // Español
    "MHz", // ÅëëçíéêÜ (Greek)
    "MHz", // Svenska
    "MHz", // Romaneste
    "MHz", // Magyar
    "MHz", // Català
    "¼³æ", // ÀãááÚØÙ (Russian)
    "MHz", // Hrvatski (Croatian)
  },
  { "Mbit/s", // English
    "Mbit/s", // Deutsch
    "Mbit/s", // Slovenski
    "Mbit/s", // Italiano
    "Mbit/s", // Nederlands
    "Mbit/s", // Português
    "Mbit/s", // Français
    "Mbit/s", // Norsk
    "Mbit/s", // suomi
    "Mbit/s", // Polski
    "Mbit/s", // Español
    "Mbit/s", // ÅëëçíéêÜ (Greek)
    "Mbit/s", // Svenska
    "Mbit/s", // Romaneste
    "Mbit/s", // Magyar
    "Mbit/s", // Català
    "¼ÑØâ/á", // ÀãááÚØÙ (Russian)
    "Mbit/s", // Hrvatski (Croatian)
  },
  { "kbit/s", // English
    "kbit/s", // Deutsch
    "kbit/s", // Slovenski
    "kbit/s", // Italiano
    "kbit/s", // Nederlands
    "kbit/s", // Português
    "kbit/s", // Français
    "kbit/s", // Norsk
    "kbit/s", // suomi
    "kbit/s", // Polski
    "kbit/s", // Español
    "kbit/s", // ÅëëçíéêÜ (Greek)
    "kbit/s", // Svenska
    "kbit/s", // Romaneste
    "kbit/s", // Magyar
    "kbit/s", // Català
    "ÚÑØâ/á", // ÀãááÚØÙ (Russian)
    "kbit/s", // Hrvatski (Croatian)
  },
  { NULL }
};
