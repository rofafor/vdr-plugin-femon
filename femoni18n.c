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
#if VDRVERSNUM >= 10302
    "¼ŞİØâŞà ÚĞçÕáâÒĞ áØÓİĞÛĞ", // ÀãááÚØÙ (Russian)
#endif
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
#if VDRVERSNUM >= 10302
    "ÁØÓİĞÛ", // ÀãááÚØÙ (Russian)
#endif
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
#if VDRVERSNUM >= 10302
    "ÀĞ×ÜÕéÕİØÕ ŞÚİĞ", // ÀãááÚØÙ (Russian)
#endif
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
#if VDRVERSNUM >= 10302
    "ÀÕÖØÜ ßŞ ãÜŞÛçĞİØî", // ÀãááÚØÙ (Russian)
#endif
  },
  { "basic", // English
    "Standard", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français 
    "", // Norsk
    "perus", // suomi
    "", // Polski   
    "", // Español  
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar  
    "", // Català  
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "transponder", // English
    "Transponder", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français 
    "", // Norsk
    "transponderi", // suomi
    "", // Polski   
    "", // Español  
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar  
    "", // Català  
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "stream", // English
    "Stream", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français 
    "", // Norsk
    "lähete", // suomi
    "", // Polski   
    "", // Español  
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar  
    "", // Català  
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
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
#if VDRVERSNUM >= 10302
    "ÁÚàëâì ÚŞÜĞİÔã Ò ÓÛĞÒİŞÜ ÜÕİî", // ÀãááÚØÙ (Russian)
#endif
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
#if VDRVERSNUM >= 10302
    "ÇĞáâŞâĞ ŞÑİŞÒÛÕİØï (0,1 áÕÚ)", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Analyze Stream", // English
    "Stream analysieren", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Lähetteen analysointi", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "ÀĞáçñâ áÚŞàŞáâØ ßŞâŞÚĞ ÔĞİİëå", // ÀãááÚØÙ (Russian)
#endif
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
#if VDRVERSNUM >= 10302
    "ÇĞáâŞâĞ ßÕàÕáçñâĞ (0,1 áÕÚ)", // ÀãááÚØÙ (Russian)
#endif
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
#if VDRVERSNUM >= 10302
    "ºàĞáİĞï ×ŞİĞ áÛĞÑŞÓŞ áØÓİĞÛĞ ÔŞ (%)", // ÀãááÚØÙ (Russian)
#endif
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
#if VDRVERSNUM >= 10302
    "·ÕÛñİĞï ×ŞİĞ áØÛìİŞÓŞ áØÓİĞÛĞ Şâ (%)", // ÀãááÚØÙ (Russian)
#endif
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
#if VDRVERSNUM >= 10302
    "ÁÒÕÔÕİØï Ş ÚĞİĞÛÕ", // ÀãááÚØÙ (Russian)
#endif
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
#if VDRVERSNUM >= 10302
    "¿ÛĞâĞ áßãâİØÚŞÒŞÓŞ ßàØñÜĞ", // ÀãááÚØÙ (Russian)
#endif
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
#if VDRVERSNUM >= 10302
    "¿ÛĞâĞ ÚĞÑÕÛìİŞÓŞ ßàØñÜĞ", // ÀãááÚØÙ (Russian)
#endif
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
#if VDRVERSNUM >= 10302
    "¿ÛĞâĞ íäØàİŞÓŞ ßàØñÜĞ", // ÀãááÚØÙ (Russian)
#endif
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
#if VDRVERSNUM >= 10302
    "¸áßŞÛì×ŞÒĞâì áÛãÖÑã syslog", // ÀãááÚØÙ (Russian)
#endif
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
#if VDRVERSNUM >= 10302
    "·°Å²°Â", // ÀãááÚØÙ (Russian)
#endif
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
#if VDRVERSNUM >= 10302
    "Á¸³½°»", // ÀãááÚØÙ (Russian)
#endif
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
#if VDRVERSNUM >= 10302
    "½µÁÃÉ°Ï", // ÀãááÚØÙ (Russian)
#endif
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
#if VDRVERSNUM >= 10302
    "²¸ÂµÀ±¸", // ÀãááÚØÙ (Russian)
#endif
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
#if VDRVERSNUM >= 10302
    "Á¸½ÅÀ", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Audio", // English
    "Audio", // Deutsch
    "Audio", // Slovenski
    "Audio", // Italiano
    "Audio", // Nederlands
    "Audio", // Português
    "Audio", // Français
    "Audio", // Norsk
    "Ääni", // suomi
    "Audio", // Polski
    "Audio", // Español
    "Audio", // ÅëëçíéêÜ (Greek)
    "Audio", // Svenska
    "Audio", // Romaneste
    "Audio", // Magyar
    "Audio", // Català
#if VDRVERSNUM >= 10302
    "°ãÔØŞ", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Video", // English
    "Video", // Deutsch
    "Video", // Slovenski
    "Video", // Italiano
    "Video", // Nederlands
    "Video", // Português
    "Video", // Français
    "Video", // Norsk
    "Kuva", // suomi
    "Video", // Polski
    "Video", // Español
    "Video", // ÅëëçíéêÜ (Greek)
    "Video", // Svenska
    "Video", // Romaneste
    "Video", // Magyar
    "Video", // Català
#if VDRVERSNUM >= 10302
    "²ØÔÕŞ", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Auto", // English
    "Auto", // Deutsch
    "Auto", // Slovenski
    "Auto", // Italiano
    "Auto", // Nederlands
    "Auto", // Português
    "Auto", // Français
    "Auto", // Norsk
    "auto", // suomi
    "Auto", // Polski
    "Auto", // Español
    "Auto", // ÅëëçíéêÜ (Greek)
    "Auto", // Svenska
    "Auto", // Romaneste
    "Auto", // Magyar
    "Auto", // Català
#if VDRVERSNUM >= 10302
    "ĞÒâŞ", // ÀãááÚØÙ (Russian)
#endif
  },
  { "None", // English
    "None", // Deutsch
    "None", // Slovenski
    "None", // Italiano
    "None", // Nederlands
    "None", // Português
    "None", // Français
    "None", // Norsk
    "ei", // suomi
    "None", // Polski
    "None", // Español
    "None", // ÅëëçíéêÜ (Greek)
    "None", // Svenska
    "None", // Romaneste
    "None", // Magyar
    "None", // Català
#if VDRVERSNUM >= 10302
    "İÕâ", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Off", // English
    "Off", // Deutsch
    "Off", // Slovenski
    "Off", // Italiano
    "Off", // Nederlands
    "Off", // Português
    "Off", // Français
    "Off", // Norsk
    "poissa", // suomi
    "Off", // Polski
    "Off", // Español
    "Off", // ÅëëçíéêÜ (Greek)
    "Off", // Svenska
    "Off", // Romaneste
    "Off", // Magyar
    "Off", // Català
#if VDRVERSNUM >= 10302
    "ÒëÚÛ", // ÀãááÚØÙ (Russian)
#endif
  },
  { "On", // English
    "On", // Deutsch
    "On", // Slovenski
    "On", // Italiano
    "On", // Nederlands
    "On", // Português
    "On", // Français
    "On", // Norsk
    "päällä", // suomi
    "On", // Polski
    "On", // Español
    "On", // ÅëëçíéêÜ (Greek)
    "On", // Svenska
    "On", // Romaneste
    "On", // Magyar
    "On", // Català
#if VDRVERSNUM >= 10302
    "ÒÚÛ", // ÀãááÚØÙ (Russian)
#endif
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
#if VDRVERSNUM >= 10302
    "¼³æ", // ÀãááÚØÙ (Russian)
#endif
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
#if VDRVERSNUM >= 10302
    "¼ÑØâ/á", // ÀãááÚØÙ (Russian)
#endif
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
#if VDRVERSNUM >= 10302
    "ÚÑØâ/á", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Stream Information", // English
    "Streaminformation", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Lähetteen tiedot", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "MPEG Layer", // English
    "MPEG Layer", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "MPEG-taso", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Video Stream", // English
    "Video Stream", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Kuvaraita", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Bitrate", // English
    "Bitrate", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Bittinopeus", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar 
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Frame Rate", // English
    "Bildrate", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Ruudunpäivitysnopeus", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Hz", // English
    "Hz", // Deutsch
    "Hz", // Slovenski
    "Hz", // Italiano
    "Hz", // Nederlands
    "Hz", // Português
    "Hz", // Français
    "Hz", // Norsk
    "Hz", // suomi
    "Hz", // Polski
    "Hz", // Español
    "Hz", // ÅëëçíéêÜ (Greek)
    "Hz", // Svenska
    "Hz", // Romaneste
    "Hz", // Magyar
    "Hz", // Català
#if VDRVERSNUM >= 10302
    "³æ", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Aspect Ratio", // English
    "Seitenverhältnis", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Kuvasuhde", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "reserved", // English
    "belegt", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "varattu", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "free", // English
    "frei", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "vapaa", // suomi
    "", // Polski   
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Video Format", // English
    "Bildformat", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Kuvaformaatti", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "PAL", // English
    "PAL", // Deutsch
    "PAL", // Slovenski
    "PAL", // Italiano
    "PAL", // Nederlands
    "PAL", // Português
    "PAL", // Français
    "PAL", // Norsk
    "PAL", // suomi
    "PAL", // Polski
    "PAL", // Español
    "PAL", // ÅëëçíéêÜ (Greek)
    "PAL", // Svenska
    "PAL", // Romaneste
    "PAL", // Magyar
    "PAL", // Català
#if VDRVERSNUM >= 10302
    "PAL", // ÀãááÚØÙ (Russian)
#endif
  },
  { "NTSC", // English
    "NTSC", // Deutsch
    "NTSC", // Slovenski
    "NTSC", // Italiano
    "NTSC", // Nederlands
    "NTSC", // Português
    "NTSC", // Français
    "NTSC", // Norsk
    "NTSC", // suomi
    "NTSC", // Polski
    "NTSC", // Español
    "NTSC", // ÅëëçíéêÜ (Greek)
    "NTSC", // Svenska
    "NTSC", // Romaneste
    "NTSC", // Magyar
    "NTSC", // Català
#if VDRVERSNUM >= 10302
    "NTSC", // ÀãááÚØÙ (Russian)
#endif
  },
  { "unknown", // English
    "unbekannt", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "tuntematon", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Resolution", // English
    "Auflösung", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Resoluutio", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Audio Stream", // English
    "Audio Stream", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Ääniraita", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Sampling Frequency", // English
    "Abtastrate", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Näytteenottotaajuus", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "kHz", // English
    "kHz", // Deutsch
    "kHz", // Slovenski
    "kHz", // Italiano
    "kHz", // Nederlands
    "kHz", // Português
    "kHz", // Français
    "kHz", // Norsk
    "kHz", // suomi
    "kHz", // Polski   
    "kHz", // Español
    "kHz", // ÅëëçíéêÜ (Greek)
    "kHz", // Svenska
    "kHz", // Romaneste
    "kHz", // Magyar
    "kHz", // Català
#if VDRVERSNUM >= 10302
    "Ú³æ", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Nid", // English
    "Nid", // Deutsch
    "Nid", // Slovenski
    "Nid", // Italiano
    "Nid", // Nederlands
    "Nid", // Português
    "Nid", // Français
    "Nid", // Norsk
    "Verkko ID", // suomi
    "Nid", // Polski
    "Nid", // Español
    "Nid", // ÅëëçíéêÜ (Greek)
    "Nid", // Svenska
    "Nid", // Romaneste
    "Nid", // Magyar
    "Nid", // Català
#if VDRVERSNUM >= 10302
    "Nid", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Tid", // English
    "Tid", // Deutsch
    "Tid", // Slovenski
    "Tid", // Italiano
    "Tid", // Nederlands
    "Tid", // Português
    "Tid", // Français
    "Tid", // Norsk
    "Lähete ID", // suomi
    "Tid", // Polski
    "Tid", // Español
    "Tid", // ÅëëçíéêÜ (Greek)
    "Tid", // Svenska
    "Tid", // Romaneste
    "Tid", // Magyar
    "Tid", // Català
#if VDRVERSNUM >= 10302
    "Tid", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Rid", // English
    "Rid", // Deutsch
    "Rid", // Slovenski
    "Rid", // Italiano
    "Rid", // Nederlands
    "Rid", // Português
    "Rid", // Français
    "Rid", // Norsk
    "Radio ID", // suomi
    "Rid", // Polski
    "Rid", // Español
    "Rid", // ÅëëçíéêÜ (Greek)
    "Rid", // Svenska
    "Rid", // Romaneste
    "Rid", // Magyar
    "Rid", // Català
#if VDRVERSNUM >= 10302
    "Rid", // ÀãááÚØÙ (Russian)
#endif
  },
  { "dB", // English
    "dB", // Deutsch
    "dB", // Slovenski
    "dB", // Italiano
    "dB", // Nederlands
    "dB", // Português
    "dB", // Français
    "dB", // Norsk
    "dB", // suomi
    "dB", // Polski   
    "dB", // Español
    "dB", // ÅëëçíéêÜ (Greek)
    "dB", // Svenska
    "dB", // Romaneste
    "dB", // Magyar
    "dB", // Català
#if VDRVERSNUM >= 10302
    "dB", // ÀãááÚØÙ (Russian)
#endif
  },
  { "not indicated", // English
    "nicht angezeigt", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "ei ilmaistu", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "AC-3", // English
    "AC-3", // Deutsch
    "AC-3", // Slovenski
    "AC-3", // Italiano
    "AC-3", // Nederlands
    "AC-3", // Português
    "AC-3", // Français
    "AC-3", // Norsk
    "AC-3", // suomi
    "AC-3", // Polski
    "AC-3", // Español
    "AC-3", // ÅëëçíéêÜ (Greek)
    "AC-3", // Svenska
    "AC-3", // Romaneste
    "AC-3", // Magyar
    "AC-3", // Català
#if VDRVERSNUM >= 10302
    "AC-3", // ÀãááÚØÙ (Russian)
#endif
  },
  { "AC-3 Stream", // English
    "AC-3 Stream", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "AC-3 ääniraita", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Frame Size", // English
    "Frame Größe", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Kehyksen koko", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Bit Stream Mode", // English
    "Bitstream Modus", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Lähetteen tyyppi", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Audio Coding Mode", // English
    "Audio Coding Modus", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Äänikoodaus", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Center Mix Level", // English
    "Center Mix Pegel", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Keskikanavan taso", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Surround Mix Level", // English
    "Surround Mix Pegel", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Tehostekanavien taso", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Dolby Surround Mode", // English
    "Dolby Surround Modus", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Dolby Surround -moodi", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Low Frequency Effects", // English
    "Tieftöner Effekt", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "LFE-kanava", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Dialogue Normalization", // English
    "Dialog Normalisation", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Dialogin normalisointi", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Complete Main (CM)", // English
    "Complete Main (CM)", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Pääasiallinen (CM)", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Music and Effects (ME)", // English
    "Musik und Effekte (ME)", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Musiikki ja tehosteet (ME)", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Visually Impaired (VI)", // English
    "Sehbehindert (VI)", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Näkörajoitteinen (VI)", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Hearing Impaired (HI)", // English
    "Hörbehindert (HI)", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Kuulorajoitteinen (HI)", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Dialogue (D)", // English
    "Dialog (D)", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Vuoropuhelu (D)", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Commentary (C)", // English
    "Kommentar (C)", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Kommentointi (C)", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Emergency (E)", // English
    "Notfall (E)", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Hätätiedote (E)", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Voice Over (VO)", // English
    "überlagerte Stimme (VO)", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Päälle puhuttu (VO)", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Karaoke", // English
    "Karaoke", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Karaoke", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Ch1", // English "Channel 1"
    "Kanal1", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "kan. 1", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Ch2", // English "Channel 2"
    "Kanal2", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "kan. 2", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "C", // English "Center"
    "C", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "K", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "L", // English "Left"
    "L", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "V", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "R", // English "Right"
    "R", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "O", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "S", // English "Surround"
    "S", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "T", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "SL", // English "Surround Left"
    "SL", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "TV", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "SR", // English "Surround Right"
    "SR", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "TO", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Show CA System", // English
    "CA System anzeigen", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Näytä salausjärjestelmä", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Fixed", // English
    "Fest", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Françeais
    "", // Norsk
    "kiinteä", // suomi
    "", // Polskie
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Free to Air", // English
    "Free to Air", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "salaamaton", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "SECA/Mediaguard", // English
    "SECA/Mediaguard", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "SECA/Mediaguard", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Viaccess", // English
    "Viaccess", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Viaccess", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Irdeto", // English
    "Irdeto", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Irdeto", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "NDS/Videoguard", // English
    "NDS/Videoguard", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "NDS/Videoguard", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "Conax", // English
    "Conax", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Conax", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "CryptoWorks", // English
    "CryptoWorks", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Portuguêes
    "", // Français
    "", // Norsk
    "CryptoWorks", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "PowerVu", // English
    "PowerVu", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "PowerVu", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "BetaCrypt", // English
    "BetaCrypt", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "BetaCrypt", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "NagraVision", // English
    "NagraVision", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "NagraVision", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { "SkyCrypt", // English
    "SkyCrypt", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk   
    "SkyCrypt", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
#if VDRVERSNUM >= 10302
    "", // ÀãááÚØÙ (Russian)
#endif
  },
  { NULL }
};
