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
    "Affiche les informations du signal DVB", // Français
    "", // Norsk
    "Signaalimittari (OSD)", // suomi
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
    "", // Èesky (Czech)
  },
  { "Signal Information", // English
    "Signalinformationen", // Deutsch
    "", // Slovenski
    "Informazioni sul segnale", // Italiano
    "", // Nederlands
    "", // Português
    "Signal DVB", // Français
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
    "", // Èesky (Czech)
  },
  { "Position", // English
    "Position", // Deutsch
    "", // Slovenski
    "Posizione", // Italiano
    "", // Nederlands
    "", // Português
    "Position", // Français
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
    "", // Èesky (Czech)
  },
  { "Height", // English
    "Höhe", // Deutsch
    "", // Slovenski
    "Altezza", // Italiano
    "", // Nederlands
    "", // Português
    "Hauteur", // Français
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
    "", // Èesky (Czech)
  },
  { "Horizontal offset", // English
    "Horizontaler Offset", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Déplacement horizontal", // Français
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
    "Horisontaalne nihe", // Eesti
    "", // Dansk
    "", // Èesky (Czech)
  },
  { "Default display mode", // English
    "Standard Anzeigemodus", // Deutsch
    "", // Slovenski
    "Modo di visualizzazione standard", // Italiano
    "", // Nederlands
    "", // Português
    "Affichage par défaut", // Français
    "", // Norsk
    "Oletusnäyttötila", // suomi
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
    "", // Èesky (Czech)
  },
  { "basic", // English
    "Standard", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "basique", // Français
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
    "", // Èesky (Czech)
  },
  { "transponder", // English
    "Transponder", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "transpondeur", // Français
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
    "transponder", // Eesti
    "", // Dansk
    "", // Èesky (Czech)
  },
  { "stream", // English
    "Stream", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "flux", // Français
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
    "voog", // Eesti
    "", // Dansk
    "", // Èesky (Czech)
  },
  { "Skin", // English
    "Oberfläche", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Skin", // Français
    "", // Norsk
    "Ulkoasu", // suomi
    "", // Polski
    "", // Español  
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català 
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "Kest", // Eesti
    "", // Dansk 
    "", // Èesky (Czech)
  },
  { "Theme", // English
    "Thema", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Thème", // Français
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
    "", // Èesky (Czech)
  },
  { "Classic", // English
    "Klassischer", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Classique", // Français
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
    "", // Èesky (Czech)
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
    "Elchi", // Èesky (Czech)
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
    "DeepBlue", // Èesky (Czech)
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
    "Moronimo", // Èesky (Czech)
  },
  { "Enigma", // English
    "Enigma", // Deutsch
    "Enigma", // Slovenski
    "Enigma", // Italiano
    "Enigma", // Nederlands
    "Enigma", // Português
    "Enigma", // Français
    "Enigma", // Norsk
    "Enigma", // suomi
    "Enigma", // Polski
    "Enigma", // Español
    "Enigma", // ÅëëçíéêÜ (Greek)
    "Enigma", // Svenska
    "Enigma", // Romaneste
    "Enigma", // Magyar
    "Enigma", // Català
    "Enigma", // ÀãááÚØÙ (Russian)
    "Enigma", // Hrvatski (Croatian)
    "Enigma", // Eesti
    "Enigma", // Dansk
    "Enigma", // Èesky (Czech)
  },
  { "EgalsTry", // English
    "EgalsTry", // Deutsch
    "EgalsTry", // Slovenski
    "EgalsTry", // Italiano
    "EgalsTry", // Nederlands
    "EgalsTry", // Português
    "EgalsTry", // Français
    "EgalsTry", // Norsk
    "EgalsTry", // suomi
    "EgalsTry", // Polski
    "EgalsTry", // Español
    "EgalsTry", // ÅëëçíéêÜ (Greek)
    "EgalsTry", // Svenska
    "EgalsTry", // Romaneste
    "EgalsTry", // Magyar
    "EgalsTry", // Català
    "EgalsTry", // ÀãááÚØÙ (Russian)
    "EgalsTry", // Hrvatski (Croatian)
    "EgalsTry", // Eesti
    "EgalsTry", // Dansk
    "EgalsTry", // Èesky (Czech)
  },
  { "Duotone", // English
    "Duotone", // Deutsch
    "Duotone", // Slovenski
    "Duotone", // Italiano
    "Duotone", // Nederlands
    "Duotone", // Português
    "Duotone", // Français
    "Duotone", // Norsk
    "Duotone", // suomi
    "Duotone", // Polski
    "Duotone", // Español
    "Duotone", // ÅëëçíéêÜ (Greek)
    "Duotone", // Svenska
    "Duotone", // Romaneste
    "Duotone", // Magyar
    "Duotone", // Català
    "Duotone", // ÀãááÚØÙ (Russian)
    "Duotone", // Hrvatski (Croatian)
    "Duotone", // Eesti
    "Duotone", // Dansk
    "Duotone", // Èesky (Czech)
  },
  { "SilverGreen", // English
    "SilverGreen", // Deutsch
    "SilverGreen", // Slovenski
    "SilverGreen", // Italiano
    "SilverGreen", // Nederlands
    "SilverGreen", // Português
    "SilverGreen", // Français
    "SilverGreen", // Norsk
    "SilverGreen", // suomi
    "SilverGreen", // Polski
    "SilverGreen", // Español
    "SilverGreen", // ÅëëçíéêÜ (Greek)
    "SilverGreen", // Svenska
    "SilverGreen", // Romaneste
    "SilverGreen", // Magyar
    "SilverGreen", // Català
    "SilverGreen", // ÀãááÚØÙ (Russian)
    "SilverGreen", // Hrvatski (Croatian)
    "SilverGreen", // Eesti
    "SilverGreen", // Dansk
    "SilverGreen", // Èesky (Czech)
  },
  { "Hide main menu entry", // English
    "Hauptmenüeintrag verstecken", // Deutsch
    "", // Slovenski
    "Nascondi voce menù", // Italiano
    "", // Nederlands
    "", // Português
    "Masquer dans le menu principal", // Français
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
    "", // Èesky (Czech)
  },
  { "OSD update interval [0.1s]", // English
    "OSD Updateintervall [0.1s]", // Deutsch
    "", // Slovenski
    "Intervallo di aggiornamento [0.1s]", // Italiano
    "", // Nederlands
    "", // Português
    "Intervalle de mise à jour (0,1s)", // Français
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
    "", // Èesky (Czech)
  },
  { "Analyze stream", // English
    "Stream analysieren", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Analyser le flux", // Français
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
    "Voo analüüs", // Eesti
    "", // Dansk
    "", // Èesky (Czech)
  },
  { "Calculation interval [0.1s]", // English
    "Berechnungsintervall [0.1s]", // Deutsch
    "", // Slovenski
    "Intervallo di calcolo [0.1s]", // Italiano
    "", // Nederlands
    "", // Português
    "Intervalle de calcul (0,1s)", // Français
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
    "", // Èesky (Czech)
  },
  { "Red limit [%]", // English
    "Grenze Rot [%]", // Deutsch
    "", // Slovenski
    "Limite rosso [%]", // Italiano
    "", // Nederlands
    "", // Português
    "Limite du rouge (%)", // Français
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
    "", // Èesky (Czech)
  },
  { "Green limit [%]", // English
    "Grenze Grün [%]", // Deutsch
    "", // Slovenski
    "Limite verde [%]", // Italiano
    "", // Nederlands
    "", // Português
    "Limite du vert (%)", // Français
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
    "", // Èesky (Czech)
  },
  { "Transponder Information", // English
    "Transponderinformation", // Deutsch
    "", // Slovenski
    "Informazioni transponder", // Italiano
    "", // Nederlands
    "", // Português
    "Information transpondeur", // Français
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
    "", // Èesky (Czech)
  },
  { "Satellite Card", // English
    "Satellitenkarte", // Deutsch
    "", // Slovenski
    "Scheda satellitare", // Italiano
    "", // Nederlands
    "", // Português
    "Carte Satellite", // Français
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
    "", // Èesky (Czech)
  },
  { "Cable Card", // English
    "Kabelkarte", // Deutsch
    "", // Slovenski
    "Scheda via cavo", // Italiano
    "", // Nederlands
    "", // Português
    "Carte Câble", // Français
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
    "", // Èesky (Czech)
  },
  { "Terrestrial Card", // English
    "Terrestrische Karte", // Deutsch
    "", // Slovenski
    "Scheda terrestre", // Italiano
    "", // Nederlands
    "", // Português
    "Carte TNT", // Français
    "", // Norsk
    "Terrestriaalikortti", // suomi
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
    "", // Èesky (Czech)
  },
  { "Use syslog output", // English
    "Benutze syslog Ausgabe", // Deutsch
    "", // Slovenski
    "Registra su syslog", // Italiano
    "", // Nederlands
    "", // Português
    "Enregistrer les infos dans syslog", // Français
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
    "", // Èesky (Czech)
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
    "Audio", // Èesky (Czech)
  },
  { "Video", // English
    "Video", // Deutsch
    "Video", // Slovenski
    "Video", // Italiano
    "Video", // Nederlands
    "Video", // Português
    "Vidéo", // Français
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
    "Video", // Èesky (Czech)
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
    "Auto", // Èesky (Czech)
  },
  { "None", // English
    "None", // Deutsch
    "None", // Slovenski
    "None", // Italiano
    "None", // Nederlands
    "None", // Português
    "Aucun", // Français
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
    "None", // Èesky (Czech)
  },
  { "Off", // English
    "Off", // Deutsch
    "Off", // Slovenski
    "Off", // Italiano
    "Off", // Nederlands
    "Off", // Português
    "Non", // Français
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
    "Off", // Èesky (Czech)
  },
  { "On", // English
    "On", // Deutsch
    "On", // Slovenski
    "On", // Italiano
    "On", // Nederlands
    "On", // Português
    "Oui", // Français
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
    "On", // Èesky (Czech)
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
    "MHz", // Èesky (Czech)
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
    "Mbit/s", // Èesky (Czech)
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
    "kbit/s", // Èesky (Czech)
  },
  { "Stream Information", // English
    "Streaminformation", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Information flux", // Français
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
    "Voo info", // Eesti
    "", // Dansk
    "", // Èesky (Czech)
  },
  { "MPEG Layer", // English
    "MPEG Layer", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "MPEG Layer", // Français
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
    "", // Èesky (Czech)
  },
  { "Video Stream", // English
    "Video Stream", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Flux vidéo", // Français
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
    "Videovoog", // Eesti
    "", // Dansk
    "", // Èesky (Czech)
  },
  { "Bitrate", // English
    "Bitrate", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Bitrate", // Français
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
    "", // Èesky (Czech)
  },
  { "Frame Rate", // English
    "Bildrate", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Rafraîchissement", // Français
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
    "", // Èesky (Czech)
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
    "Hz", // Èesky (Czech)
  },
  { "Aspect Ratio", // English
    "Seitenverhältnis", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Proportions d'image", // Français
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
    "", // Èesky (Czech)
  },
  { "reserved", // English
    "belegt", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "réservé", // Français
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
    "", // Èesky (Czech)
  },
  { "free", // English
    "frei", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "libre", // Français
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
    "", // Èesky (Czech)
  },
  { "Video Format", // English
    "Bildformat", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Standard vidéo", // Français
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
    "", // Èesky (Czech)
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
    "PAL", // Èesky (Czech)
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
    "NTSC", // Èesky (Czech)
  },
  { "unknown", // English
    "unbekannt", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "inconnu", // Français
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
    "", // Èesky (Czech)
  },
  { "Resolution", // English
    "Auflösung", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Résolution", // Français
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
    "", // Èesky (Czech)
  },
  { "Audio Stream", // English
    "Audio Stream", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Flux audio", // Français
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
    "Audiovoog", // Eesti
    "", // Dansk
    "", // Èesky (Czech)
  },
  { "Sampling Frequency", // English
    "Abtastrate", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Fréquence d'échantillonage", // Français
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
    "", // Èesky (Czech)
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
    "kHz", // Èesky (Czech)
  },
  { "Apid", // English
    "Apid", // Deutsch
    "Apid", // Slovenski
    "Apid", // Italiano
    "Apid", // Nederlands
    "Apid", // Português
    "Apid", // Français
    "Apid", // Norsk
    "Ääni-PID", // suomi
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
    "Apid", // Èesky (Czech)
  },
  { "Dpid", // English
    "Dpid", // Deutsch
    "Dpid", // Slovenski
    "Dpid", // Italiano
    "Dpid", // Nederlands
    "Dpid", // Português
    "Dpid", // Français
    "Dpid", // Norsk
    "Dolby-PID", // suomi
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
    "Dpid", // Èesky (Czech)
  },
  { "Nid", // English
    "Nid", // Deutsch
    "Nid", // Slovenski
    "Nid", // Italiano
    "Nid", // Nederlands
    "Nid", // Português
    "Nid", // Français
    "Nid", // Norsk
    "Verkko-ID", // suomi
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
    "Nid", // Èesky (Czech)
  },
  { "Tid", // English
    "Tid", // Deutsch
    "Tid", // Slovenski
    "Tid", // Italiano
    "Tid", // Nederlands
    "Tid", // Português
    "Tid", // Français
    "Tid", // Norsk
    "Lähete-ID", // suomi
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
    "Tid", // Èesky (Czech)
  },
  { "Rid", // English
    "Rid", // Deutsch
    "Rid", // Slovenski
    "Rid", // Italiano
    "Rid", // Nederlands
    "Rid", // Português
    "Rid", // Français
    "Rid", // Norsk
    "Radio-ID", // suomi
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
    "Rid", // Èesky (Czech)
  },
  { "Coderate", // English
    "Coderate", // Deutsch
    "Coderate", // Slovenski
    "Coderate", // Italiano
    "Coderate", // Nederlands
    "Coderate", // Português
    "Coderate", // Français
    "Coderate", // Norsk
    "Suojaustaso", // suomi
    "Coderate", // Polski
    "Coderate", // Español
    "Coderate", // ÅëëçíéêÜ (Greek)
    "Coderate", // Svenska
    "Coderate", // Romaneste
    "Coderate", // Magyar
    "Coderate", // Català
    "Coderate", // ÀãááÚØÙ (Russian)
    "Coderate", // Hrvatski (Croatian)
    "Coderate", // Eesti
    "Coderate", // Dansk
    "Coderate", // Èesky (Czech)
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
    "dB", // Èesky (Czech)
  },
  { "not indicated", // English
    "nicht angegeben", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "non indiqué", // Français
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
    "", // Èesky (Czech)
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
    "AC-3", // Èesky (Czech)
  },
  { "AC-3 Stream", // English
    "AC-3 Stream", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Flux AC-3", // Français
    "", // Norsk
    "AC-3-ääniraita", // suomi
    "", // Polski
    "", // Español
    "", // ÅëëçíéêÜ (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Català
    "", // ÀãááÚØÙ (Russian)
    "", // Hrvatski (Croatian)
    "AC-3 voog", // Eesti
    "", // Dansk
    "", // Èesky (Czech)
  },
  { "Frame Size", // English
    "Frame Größe", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Taille de paquet", // Français
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
    "", // Èesky (Czech)
  },
  { "Bit Stream Mode", // English
    "Bitstream Modus", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Mode bitstream", // Français
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
    "Bitivoo tüüp", // Eesti
    "", // Dansk
    "", // Èesky (Czech)
  },
  { "Audio Coding Mode", // English
    "Audio Coding Modus", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Mode de codage audio", // Français
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
    "", // Èesky (Czech)
  },
  { "Center Mix Level", // English
    "Center Mix Pegel", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Niveau sonore milieu", // Français
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
    "", // Èesky (Czech)
  },
  { "Surround Mix Level", // English
    "Surround Mix Pegel", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Niveau sonore surround", // Français
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
    "", // Èesky (Czech)
  },
  { "Dolby Surround Mode", // English
    "Dolby Surround Modus", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Mode Dolby Surround", // Français
    "", // Norsk
    "Dolby Surround -tehoste", // suomi
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
    "", // Èesky (Czech)
  },
  { "Low Frequency Effects", // English
    "Tieftöner Effekte", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Effets de basses", // Français
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
    "", // Èesky (Czech)
  },
  { "Dialogue Normalization", // English
    "Dialog Normalisierung", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Normalisation des dialogues", // Français
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
    "", // Èesky (Czech)
  },
  { "Complete Main (CM)", // English
    "Complete Main (CM)", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Principal (CM)", // Français
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
    "Täiskomplekt (CM)", // Eesti
    "", // Dansk
    "", // Èesky (Czech)
  },
  { "Music and Effects (ME)", // English
    "Musik und Effekte (ME)", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Musique et effets (ME)", // Français
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
    "", // Èesky (Czech)
  },
  { "Visually Impaired (VI)", // English
    "Sehbehindert (VI)", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Malvoyants (VI)", // Français
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
    "", // Èesky (Czech)
  },
  { "Hearing Impaired (HI)", // English
    "Hörbehindert (HI)", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Malentendants (HI)", // Français
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
    "", // Èesky (Czech)
  },
  { "Dialogue (D)", // English
    "Dialog (D)", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Dialogue (D)", // Français
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
    "", // Èesky (Czech)
  },
  { "Commentary (C)", // English
    "Kommentar (C)", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Commentaires (C)", // Français
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
    "", // Èesky (Czech)
  },
  { "Emergency (E)", // English
    "Notfall (E)", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Urgence (E)", // Français
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
    "", // Èesky (Czech)
  },
  { "Voice Over (VO)", // English
    "überlagerte Stimme (VO)", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Voix off (VO)", // Français
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
    "", // Èesky (Czech)
  },
  { "Karaoke", // English
    "Karaoke", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Karaoke", // Français
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
    "", // Èesky (Czech)
  },
  { "Ch1", // English "Channel 1"
    "Kan1", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Can. 1", // Français
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
    "", // Èesky (Czech)
  },
  { "Ch2", // English "Channel 2"
    "Kan2", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Can. 2", // Français
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
    "", // Èesky (Czech)
  },
  { "C", // English "Center"
    "C", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Centre", // Français
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
    "C", // Eesti
    "", // Dansk
    "", // Èesky (Czech)
  },
  { "L", // English "Left"
    "L", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Gauche", // Français
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
    "", // Èesky (Czech)
  },
  { "R", // English "Right"
    "R", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Droite", // Français
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
    "", // Èesky (Czech)
  },
  { "S", // English "Surround"
    "S", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Surround", // Français
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
    "", // Èesky (Czech)
  },
  { "SL", // English "Surround Left"
    "SL", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Surround gauche", // Français
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
    "", // Èesky (Czech)
  },
  { "SR", // English "Surround Right"
    "SR", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Surround droit", // Français
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
    "", // Èesky (Czech)
  },
  { "Show CA system", // English
    "CA System anzeigen", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Système CA", // Français
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
    "", // Èesky (Czech)
  },
  { "Fixed", // English
    "Fest", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Fixe", // Français
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
    "", // Èesky (Czech)
  },
  { "Analog", // English
    "", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Analogique", // Français
    "", // Norsk
    "analoginen", // suomi
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
    "", // Èesky (Czech)
  },
  { "Free to Air", // English
    "Free to Air", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Free To Air", // Français
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
    "", // Èesky (Czech)
  },
  { "SECA/Mediaguard", // English
    "SECA/Mediaguard", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "SECA/Mediaguard", // Français
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
    "", // Èesky (Czech)
  },
  { "Viaccess", // English
    "Viaccess", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Viaccess", // Français
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
    "", // Èesky (Czech)
  },
  { "Irdeto", // English
    "Irdeto", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Irdeto", // Français
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
    "", // Èesky (Czech)
  },
  { "NDS/Videoguard", // English
    "NDS/Videoguard", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "NDS/Videoguard", // Français
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
    "", // Èesky (Czech)
  },
  { "Conax", // English
    "Conax", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "Conax", // Français
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
    "", // Èesky (Czech)
  },
  { "CryptoWorks", // English
    "CryptoWorks", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "CryptoWorks", // Français
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
    "", // Èesky (Czech)
  },
  { "PowerVu", // English
    "PowerVu", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "PowerVu", // Français
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
    "", // Èesky (Czech)
  },
  { "BetaCrypt", // English
    "BetaCrypt", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "BetaCrypt", // Français
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
    "", // Èesky (Czech)
  },
  { "NagraVision", // English
    "NagraVision", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "NagraVision", // Français
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
    "", // Èesky (Czech)
  },
  { "SkyCrypt", // English
    "SkyCrypt", // Deutsch
    "", // Slovenski
    "", // Italiano
    "", // Nederlands
    "", // Português
    "SkyCrypt", // Français
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
    "", // Èesky (Czech)
  },
  { NULL }
};
