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
    "DVB Signaalmonitor (OSD)", // Eesti
    "", // Dansk
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
    "Signaaliinfo", // Eesti
    "", // Dansk
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
    "Positsioon", // Eesti
    "", // Dansk
  },
  { "Height", // English
    "Höhe", // Deutsch
    "", // Slovenski
    "Altezza", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Korkeus", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
    "²ëáŞâĞ", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Kõrgus", // Eesti
    "", // Dansk
  },
  { "Horizontal offset", // English
    "", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "Vaakakeskitys", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "", // Eesti
    "", // Dansk
  },
  { "Default display mode", // English
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
    "Vaikimisi displei moodus", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "standard", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "", // Eesti
    "transponder", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "striim", // Eesti
    "", // Dansk
  },
  { "Theme", // English
    "Thema", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français 
    "", // Norsk
    "Teema", // suomi
    "", // Polski   
    "", // Español  
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar  
    "", // Català  
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Teema", // Eesti
    "", // Dansk
  },
  { "Classic", // English
    "Klassischer", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français 
    "", // Norsk
    "Klassinen", // suomi
    "", // Polski   
    "", // Español  
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar  
    "", // Català  
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Klassikaline", // Eesti
    "", // Dansk
  },
  { "Elchi", // English
    "Elchi", // Deutsch
    "Elchi", // Slovenski
    "Elchi", // Italiano
    "Elchi", // Nederlands
    "Elchi", // Português
    "Elchi", // Français 
    "Elchi", // Norsk
    "Elchi", // suomi
    "Elchi", // Polski   
    "Elchi", // Español  
    "Elchi", // ÅëëçíéêÜ (Greek)
    "Elchi", // Svenska
    "Elchi", // Romaneste
    "Elchi", // Magyar  
    "Elchi", // Català  
    "Elchi", // ÀãááÚØÙ (Russian)
    "Elchi", // Hrvatski (Croatian)
    "Elchi", // Eesti
    "Elchi", // Dansk
  },
  { "DeepBlue", // English
    "DeepBlue", // Deutsch
    "DeepBlue", // Slovenski
    "DeepBlue", // Italiano
    "DeepBlue", // Nederlands
    "DeepBlue", // Português
    "DeepBlue", // Français 
    "DeepBlue", // Norsk
    "DeepBlue", // suomi
    "DeepBlue", // Polski   
    "DeepBlue", // Español  
    "DeepBlue", // ÅëëçíéêÜ (Greek)
    "DeepBlue", // Svenska
    "DeepBlue", // Romaneste
    "DeepBlue", // Magyar  
    "DeepBlue", // Català  
    "DeepBlue", // ÀãááÚØÙ (Russian)
    "DeepBlue", // Hrvatski (Croatian)
    "DeepBlue", // Eesti
    "DeepBlue", // Dansk
  },
  { "Moronimo", // English
    "Moronimo", // Deutsch
    "Moronimo", // Slovenski
    "Moronimo", // Italiano
    "Moronimo", // Nederlands
    "Moronimo", // Português
    "Moronimo", // Français 
    "Moronimo", // Norsk
    "Moronimo", // suomi
    "Moronimo", // Polski   
    "Moronimo", // Español  
    "Moronimo", // ÅëëçíéêÜ (Greek)
    "Moronimo", // Svenska
    "Moronimo", // Romaneste
    "Moronimo", // Magyar  
    "Moronimo", // Català  
    "Moronimo", // ÀãááÚØÙ (Russian)
    "Moronimo", // Hrvatski (Croatian)
    "Moronimo", // Eesti
    "Moronimo", // Dansk
  },
  { "Hide main menu entry", // English
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
    "Peida valik peamenüüs", // Eesti
    "", // Dansk
  },
  { "OSD update interval [0.1s]", // English
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
    "Uuendusintervall [0,1s]", // Eesti
    "", // Dansk
  },
  { "Analyze stream", // English
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
    "ÀĞáçñâ áÚŞàŞáâØ ßŞâŞÚĞ ÔĞİİëå", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Striimi analüüs", // Eesti
    "", // Dansk
  },
  { "Calculation interval [0.1s]", // English
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
    "Kalkulatsiooni intervall [0,1s]", // Eesti
    "", // Dansk
  },
  { "Red limit [%]", // English
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
    "Punase limiit [%]", // Eesti
    "", // Dansk
  },
  { "Green limit [%]", // English
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
    "Rohelise limiit [%]", // Eesti
    "", // Dansk
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
    "Transponderi info", // Eesti
    "", // Dansk
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
    "Satelliidikaart", // Eesti
    "", // Dansk
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
    "Kaablikaart", // Eesti
    "", // Dansk
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
    "Terrestiaalkaart", // Eesti
    "", // Dansk
  },
  { "Use syslog output", // English
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
    "Kirjuta süsteemilogisse", // Eesti
    "", // Dansk
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
    "°ãÔØŞ", // ÀãááÚØÙ (Russian)
    "Audio", // Hrvatski (Croatian)
    "Audio", // Eesti
    "Audio", // Dansk
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
    "²ØÔÕŞ", // ÀãááÚØÙ (Russian)
    "Video", // Hrvatski (Croatian)
    "Video", // Eesti
    "Video", // Dansk
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
    "ĞÒâŞ", // ÀãááÚØÙ (Russian)
    "Auto", // Hrvatski (Croatian)
    "Auto", // Eesti
    "Auto", // Dansk
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
    "İÕâ", // ÀãááÚØÙ (Russian)
    "None", // Hrvatski (Croatian)
    "Ei", // Eesti
    "None", // Dansk
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
    "ÒëÚÛ", // ÀãááÚØÙ (Russian)
    "Off", // Hrvatski (Croatian)
    "Väljas", // Eesti
    "Off", // Dansk
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
    "ÒÚÛ", // ÀãááÚØÙ (Russian)
    "On", // Hrvatski (Croatian)
    "Sees", // Eesti
    "On", // Dansk
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
    "MHz", // Eesti
    "MHz", // Dansk
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
    "Mbit/s", // Eesti
    "Mbit/s", // Dansk
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
    "kbit/s", // Eesti
    "kbit/s", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Striimi info", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "MPEG tase", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Videostriim", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Bitikiirus", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Kaadrisagedus", // Eesti
    "", // Dansk
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
    "³æ", // ÀãááÚØÙ (Russian)
    "Hz", // Hrvatski (Croatian)
    "Hz", // Eesti
    "Hz", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Külgsuhe", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "reserveeritud", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "vaba", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Videoformaat", // Eesti
    "", // Dansk
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
    "PAL", // ÀãááÚØÙ (Russian)
    "PAL", // Hrvatski (Croatian)
    "PAL", // Eesti
    "PAL", // Dansk
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
    "NTSC", // ÀãááÚØÙ (Russian)
    "NTSC", // Hrvatski (Croatian)
    "NTSC", // Eesti
    "NTSC", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "tundmatu", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Resolutsioon", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Audiostriim", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Sämplimissagedus", // Eesti
    "", // Dansk
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
    "Ú³æ", // ÀãááÚØÙ (Russian)
    "kHz", // Hrvatski (Croatian)
    "kHz", // Eesti
    "kHz", // Dansk
  },
  { "Apid", // English
    "Apid", // Deutsch
    "Apid", // Slovenski
    "Apid", // Italiano
    "Apid", // Nederlands
    "Apid", // Português
    "Apid", // Français
    "Apid", // Norsk
    "Ääni PID", // suomi
    "Apid", // Polski
    "Apid", // Español
    "Apid", // ÅëëçíéêÜ (Greek)
    "Apid", // Svenska
    "Apid", // Romaneste
    "Apid", // Magyar
    "Apid", // Català
    "Apid", // ÀãááÚØÙ (Russian)
    "Apid", // Hrvatski (Croatian)
    "Apid", // Eesti
    "Apid", // Dansk
  },
  { "Dpid", // English
    "Dpid", // Deutsch
    "Dpid", // Slovenski
    "Dpid", // Italiano
    "Dpid", // Nederlands
    "Dpid", // Português
    "Dpid", // Français
    "Dpid", // Norsk
    "Dolby PID", // suomi
    "Dpid", // Polski
    "Dpid", // Español
    "Dpid", // ÅëëçíéêÜ (Greek)
    "Dpid", // Svenska
    "Dpid", // Romaneste
    "Dpid", // Magyar
    "Dpid", // Català
    "Dpid", // ÀãááÚØÙ (Russian)
    "Dpid", // Hrvatski (Croatian)
    "Dpid", // Eesti
    "Dpid", // Dansk
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
    "Nid", // ÀãááÚØÙ (Russian)
    "Nid", // Hrvatski (Croatian)
    "Nid", // Eesti
    "Nid", // Dansk
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
    "Tid", // ÀãááÚØÙ (Russian)
    "Tid", // Hrvatski (Croatian)
    "Tid", // Eesti
    "Tid", // Dansk
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
    "Rid", // ÀãááÚØÙ (Russian)
    "Rid", // Hrvatski (Croatian)
    "Rid", // Eesti
    "Rid", // Dansk
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
    "dB", // ÀãááÚØÙ (Russian)
    "dB", // Hrvatski (Croatian)
    "dB", // Eesti
    "dB", // Dansk
  },
  { "not indicated", // English
    "", // Deutsch
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "märkimata", // Eesti
    "", // Dansk
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
    "AC-3", // ÀãááÚØÙ (Russian)
    "AC-3", // Hrvatski (Croatian)
    "AC-3", // Eesti
    "AC-3", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "AC-3 striim", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Kaadri suurus", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Bitistriimi tüüp", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Audiokodeering", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Keskmise kanali tase", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Surround kanali tase", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Dolby Surround'i tüüp", // Eesti
    "", // Dansk
  },
  { "Low Frequency Effects", // English
    "Tieftöner Effekte", // Deutsch
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "LFE kanal", // Eesti
    "", // Dansk
  },
  { "Dialogue Normalization", // English
    "Dialog Normalisierung", // Deutsch
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Dialoogi normalisatsioon", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "CM", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Muusika ja efektid (ME)", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Vaegnägemine (VE)", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Vaegkuulmine (HI)", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Dialoog (D)", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Kommentaar (C)", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Hädateade (E)", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Pealerääkimine (VO)", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Karaoke", // Eesti
    "", // Dansk
  },
  { "Ch1", // English "Channel 1"
    "Kan1", // Deutsch
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Kan. 1", // Eesti
    "", // Dansk
  },
  { "Ch2", // English "Channel 2"
    "Kan2", // Deutsch
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Kan. 2", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "K", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "L", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "R", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "S", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "SL", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "SR", // Eesti
    "", // Dansk
  },
  { "Show CA system", // English
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Näita CA süsteemi", // Eesti
    "", // Dansk
  },
  { "Fixed", // English
    "Fest", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "", // Français
    "", // Norsk
    "kiinteä", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Fikseeritud", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "FTA", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "SECA/Mediaguard", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Viaccess", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Irdeto", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "NDS/Videoguard", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Conax", // Eesti
    "", // Dansk
  },
  { "CryptoWorks", // English
    "CryptoWorks", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "CryptoWorks", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "PowerVu", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "BetaCrypt", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "NagraVision", // Eesti
    "", // Dansk
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
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "SkyCrypt", // Eesti
    "", // Dansk
  },
  { NULL }
};
