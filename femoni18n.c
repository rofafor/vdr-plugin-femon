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
    "", // Portuguъs
    "", // Franчais
    "", // Norsk
    "DVB signaalimittari (OSD)", // suomi
    "", // Polski
    "", // Espaёol
    "", // Хыычэщъм (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalр
#if VDRVERSNUM >= 10300
    "Монитор качества сигнала"  // Русский (Russian)
#endif
  },
  { "Signal Information", // English
    "Signalinformationen", // Deutsch
    "", // Slovenski
    "Informazioni sul segnale", // Italiano
    "", // Nederlands
    "", // Portuguъs
    "", // Franчais
    "", // Norsk
    "Signaalimittari", // suomi
    "", // Polski
    "", // Espaёol
    "", // Хыычэщъм (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalр
#if VDRVERSNUM >= 10300
    "Сигнал"  // Русский (Russian)
#endif
  },
  { "Position", // English
    "Position", // Deutsch
    "", // Slovenski
    "Posizione", // Italiano
    "", // Nederlands
    "", // Portuguъs
    "", // Franчais
    "", // Norsk
    "Sijainti", // suomi
    "", // Polski
    "", // Espaёol
    "", // Хыычэщъм (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalр
#if VDRVERSNUM >= 10300
    "Размещение окна"  // Русский (Russian)
#endif
  },
  { "Default Display Mode", // English
    "Standard Anzeigemodus", // Deutsch
    "", // Slovenski
    "Modo di visualizzazione standard", // Italiano
    "", // Nederlands
    "", // Portuguъs
    "", // Franчais 
    "", // Norsk
    "Oletus nфyttіmoodi", // suomi
    "", // Polski   
    "", // Espaёol  
    "", // Хыычэщъм (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar  
    "", // Catalр  
#if VDRVERSNUM >= 10300
    "Режим по умолчанию"  // Русский (Russian)
#endif
  },
  { "simple", // English
    "einfach", // Deutsch
    "", // Slovenski
    "semplice", // Italiano
    "", // Nederlands
    "", // Portuguъs
    "", // Franчais 
    "", // Norsk
    "suppea", // suomi
    "", // Polski   
    "", // Espaёol  
    "", // Хыычэщъм (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar  
    "", // Catalр  
#if VDRVERSNUM >= 10300
    "простой"  // Русский (Russian)
#endif
  },
  { "advanced", // English
    "fortgeschritten", // Deutsch
    "", // Slovenski
    "avanzato", // Italiano
    "", // Nederlands
    "", // Portuguъs
    "", // Franчais 
    "", // Norsk
    "laaja", // suomi
    "", // Polski   
    "", // Espaёol  
    "", // Хыычэщъм (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar  
    "", // Catalр  
#if VDRVERSNUM >= 10300
    "расширенный"  // Русский (Russian)
#endif
  },
  { "Hide Mainmenu Entry", // English
    "Hauptmenќeintrag verstecken", // Deutsch
    "", // Slovenski
    "Nascondi voce menљ", // Italiano
    "", // Nederlands
    "", // Portuguъs
    "", // Franчais
    "", // Norsk
    "Piilota valinta pффvalikosta", // suomi
    "", // Polski
    "", // Espaёol
    "", // Хыычэщъм (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalр
#if VDRVERSNUM >= 10300
    "Скрыть команду в главном меню"  // Русский (Russian)
#endif
  },
  { "OSD Update Interval [0.1s]", // English
    "OSD Updateintervall [0.1s]", // Deutsch
    "", // Slovenski
    "Intervallo di aggiornamento [0.1s]", // Italiano
    "", // Nederlands
    "", // Portuguъs
    "", // Franчais
    "", // Norsk
    "Nфytіn pфivitysvфli [0.1s]", // suomi
    "", // Polski
    "", // Espaёol
    "", // Хыычэщъм (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalр
#if VDRVERSNUM >= 10300
    "Частота обновления (0,1 сек)"  // Русский (Russian)
#endif
  },
  { "Bitrate Calculation", // English
    "Bitratenberechnung", // Deutsch
    "", // Slovenski
    "Calcolo Bitrate", // Italiano
    "", // Nederlands
    "", // Portuguъs
    "", // Franчais
    "", // Norsk
    "Siirtonopeuden laskenta", // suomi
    "", // Polski
    "", // Espaёol
    "", // Хыычэщъм (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalр
#if VDRVERSNUM >= 10300
    "Расчёт скорости потока данных"  // Русский (Russian)
#endif
  },
  { "Calculation Interval [0.1s]", // English
    "Berechnungsintervall [0.1s]", // Deutsch
    "", // Slovenski
    "Intervallo di calcolo [0.1s]", // Italiano
    "", // Nederlands
    "", // Portuguъs
    "", // Franчais
    "", // Norsk
    "Laskennan pфivitysvфli [0.1s]", // suomi
    "", // Polski
    "", // Espaёol
    "", // Хыычэщъм (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalр
#if VDRVERSNUM >= 10300
    "Частота пересчёта (0,1 сек)"  // Русский (Russian)
#endif
  },
  { "Red Limit [%]", // English
    "Grenze Rot [%]", // Deutsch
    "", // Slovenski
    "Limite rosso [%]", // Italiano
    "", // Nederlands
    "", // Portuguъs
    "", // Franчais
    "", // Norsk
    "Punaisen taso [%]", // suomi
    "", // Polski
    "", // Espaёol
    "", // Хыычэщъм (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalр
#if VDRVERSNUM >= 10300
    "Красная зона слабого сигнала до (%)"  // Русский (Russian)
#endif
  },
  { "Green Limit [%]", // English
    "Grenze Grќn [%]", // Deutsch
    "", // Slovenski
    "Limite verde [%]", // Italiano
    "", // Nederlands
    "", // Portuguъs
    "", // Franчais
    "", // Norsk
    "Vihreфn taso [%]", // suomi
    "", // Polski
    "", // Espaёol
    "", // Хыычэщъм (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalр
#if VDRVERSNUM >= 10300
    "Зелёная зона сильного сигнала от (%)"  // Русский (Russian)
#endif
  },
  { "Transponder Information", // English
    "Transponderinformation", // Deutsch
    "", // Slovenski
    "Informazioni transponder", // Italiano
    "", // Nederlands
    "", // Portuguъs
    "", // Franчais
    "", // Norsk   
    "Transponderin tiedot", // suomi
    "", // Polski   
    "", // Espaёol
    "", // Хыычэщъм (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalр
#if VDRVERSNUM >= 10300
    "Сведения о канале"  // Русский (Russian)
#endif
  },
  { "Satellite Card", // English
    "Satellitenkarte", // Deutsch
    "", // Slovenski
    "Scheda satellitare", // Italiano
    "", // Nederlands
    "", // Portuguъs
    "", // Franчais
    "", // Norsk
    "Satelliittikortti", // suomi
    "", // Polski
    "", // Espaёol
    "", // Хыычэщъм (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalр
#if VDRVERSNUM >= 10300
    "Плата спутникового приёма"  // Русский (Russian)
#endif
  },
  { "Cable Card", // English
    "Kabelkarte", // Deutsch
    "", // Slovenski
    "Scheda via cavo", // Italiano
    "", // Nederlands
    "", // Portuguъs
    "", // Franчais
    "", // Norsk
    "Kaapelikortti", // suomi
    "", // Polski
    "", // Espaёol
    "", // Хыычэщъм (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalр
#if VDRVERSNUM >= 10300
    "Плата кабельного приёма"  // Русский (Russian)
#endif
  },
  { "Terrestial Card", // English
    "Terrestrische Karte", // Deutsch
    "", // Slovenski
    "Scheda terrestre", // Italiano
    "", // Nederlands
    "", // Portuguъs
    "", // Franчais
    "", // Norsk
    "Terrestiaalikortti", // suomi
    "", // Polski
    "", // Espaёol
    "", // Хыычэщъм (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalр
#if VDRVERSNUM >= 10300
    "Плата эфирного приёма"  // Русский (Russian)
#endif
  },
  { "Use Syslog Output", // English
    "Benutze syslog Ausgabe", // Deutsch
    "", // Slovenski
    "Registra su syslog", // Italiano
    "", // Nederlands
    "", // Portuguъs
    "", // Franчais
    "", // Norsk
    "Kirjaa systeemilokiin", // suomi
    "", // Polski
    "", // Espaёol
    "", // Хыычэщъм (Greek)
    "", // Svenska
    "", // Romaneste
    "", // Magyar
    "", // Catalр
#if VDRVERSNUM >= 10300
    "Использовать службу syslog"  // Русский (Russian)
#endif
  },
  { "LOCK", // English
    "LOCK", // Deutsch
    "LOCK", // Slovenski
    "LOCK", // Italiano
    "LOCK", // Nederlands
    "LOCK", // Portuguъs
    "LOCK", // Franчais
    "LOCK", // Norsk
    "LOCK", // suomi
    "LOCK", // Polski
    "LOCK", // Espaёol
    "LOCK", // Хыычэщъм (Greek)
    "LOCK", // Svenska
    "LOCK", // Romaneste
    "LOCK", // Magyar
    "LOCK", // Catalр
#if VDRVERSNUM >= 10300
    "ЗАХВАТ"  // Русский (Russian)
#endif
  },
  { "SIGNAL", // English
    "SIGNAL", // Deutsch
    "SIGNAL", // Slovenski
    "SIGNAL", // Italiano
    "SIGNAL", // Nederlands
    "SIGNAL", // Portuguъs
    "SIGNAL", // Franчais
    "SIGNAL", // Norsk
    "SIGNAL", // suomi
    "SIGNAL", // Polski
    "SIGNAL", // Espaёol
    "SIGNAL", // Хыычэщъм (Greek)
    "SIGNAL", // Svenska
    "SIGNAL", // Romaneste
    "SIGNAL", // Magyar
    "SIGNAL", // Catalр
#if VDRVERSNUM >= 10300
    "СИГНАЛ"  // Русский (Russian)
#endif
  },
  { "CARRIER", // English
    "CARRIER", // Deutsch
    "CARRIER", // Slovenski
    "CARRIER", // Italiano
    "CARRIER", // Nederlands
    "CARRIER", // Portuguъs
    "CARRIER", // Franчais
    "CARRIER", // Norsk
    "CARRIER", // suomi
    "CARRIER", // Polski
    "CARRIER", // Espaёol
    "CARRIER", // Хыычэщъм (Greek)
    "CARRIER", // Svenska
    "CARRIER", // Romaneste
    "CARRIER", // Magyar
    "CARRIER", // Catalр
#if VDRVERSNUM >= 10300
    "НЕСУЩАЯ"  // Русский (Russian)
#endif
  },
  { "VITERBI", // English
    "VITERBI", // Deutsch
    "VITERBI", // Slovenski
    "VITERBI", // Italiano
    "VITERBI", // Nederlands
    "VITERBI", // Portuguъs
    "VITERBI", // Franчais
    "VITERBI", // Norsk
    "VITERBI", // suomi
    "VITERBI", // Polski
    "VITERBI", // Espaёol
    "VITERBI", // Хыычэщъм
    "VITERBI", // Svenska
    "VITERBI", // Romaneste
    "VITERBI", // Magyar
    "VITERBI", // Catalр
#if VDRVERSNUM >= 10300
    "ВИТЕРБИ"  // Русский (Russian)
#endif
  },
  { "SYNC", // English
    "SYNC", // Deutsch
    "SYNC", // Slovenski
    "SYNC", // Italiano
    "SYNC", // Nederlands
    "SYNC", // Portuguъs
    "SYNC", // Franчais
    "SYNC", // Norsk
    "SYNC", // suomi
    "SYNC", // Polski
    "SYNC", // Espaёol
    "SYNC", // Хыычэщъм (Greek)
    "SYNC", // Svenska
    "SYNC", // Romaneste
    "SYNC", // Magyar
    "SYNC", // Catalр
#if VDRVERSNUM >= 10300
    "СИНХР"  // Русский (Russian)
#endif
  },
  { "Audio", // English
    "Audio", // Deutsch
    "Audio", // Slovenski
    "Audio", // Italiano
    "Audio", // Nederlands
    "Audio", // Portuguъs
    "Audio", // Franчais
    "Audio", // Norsk
    "Audio", // suomi
    "Audio", // Polski
    "Audio", // Espaёol
    "Audio", // Хыычэщъм (Greek)
    "Audio", // Svenska
    "Audio", // Romaneste
    "Audio", // Magyar
    "Audio", // Catalр
#if VDRVERSNUM >= 10300
    "Аудио"  // Русский (Russian)
#endif
  },
  { "Video", // English
    "Video", // Deutsch
    "Video", // Slovenski
    "Video", // Italiano
    "Video", // Nederlands
    "Video", // Portuguъs
    "Video", // Franчais
    "Video", // Norsk
    "Video", // suomi
    "Video", // Polski
    "Video", // Espaёol
    "Video", // Хыычэщъм (Greek)
    "Video", // Svenska
    "Video", // Romaneste
    "Video", // Magyar
    "Video", // Catalр
#if VDRVERSNUM >= 10300
    "Видео"  // Русский (Russian)
#endif
  },
  { "Auto", // English
    "Auto", // Deutsch
    "Auto", // Slovenski
    "Auto", // Italiano
    "Auto", // Nederlands
    "Auto", // Portuguъs
    "Auto", // Franчais
    "Auto", // Norsk
    "Auto", // suomi
    "Auto", // Polski
    "Auto", // Espaёol
    "Auto", // Хыычэщъм (Greek)
    "Auto", // Svenska
    "Auto", // Romaneste
    "Auto", // Magyar
    "Auto", // Catalр
#if VDRVERSNUM >= 10300
    "авто"  // Русский (Russian)
#endif
  },
  { "None", // English
    "None", // Deutsch
    "None", // Slovenski
    "None", // Italiano
    "None", // Nederlands
    "None", // Portuguъs
    "None", // Franчais
    "None", // Norsk
    "None", // suomi
    "None", // Polski
    "None", // Espaёol
    "None", // Хыычэщъм (Greek)
    "None", // Svenska
    "None", // Romaneste
    "None", // Magyar
    "None", // Catalр
#if VDRVERSNUM >= 10300
    "нет"  // Русский (Russian)
#endif
  },
  { "Off", // English
    "Off", // Deutsch
    "Off", // Slovenski
    "Off", // Italiano
    "Off", // Nederlands
    "Off", // Portuguъs
    "Off", // Franчais
    "Off", // Norsk
    "Off", // suomi
    "Off", // Polski
    "Off", // Espaёol
    "Off", // Хыычэщъм (Greek)
    "Off", // Svenska
    "Off", // Romaneste
    "Off", // Magyar
    "Off", // Catalр
#if VDRVERSNUM >= 10300
    "выкл"  // Русский (Russian)
#endif
  },
  { "On", // English
    "On", // Deutsch
    "On", // Slovenski
    "On", // Italiano
    "On", // Nederlands
    "On", // Portuguъs
    "On", // Franчais
    "On", // Norsk
    "On", // suomi
    "On", // Polski
    "On", // Espaёol
    "On", // Хыычэщъм (Greek)
    "On", // Svenska
    "On", // Romaneste
    "On", // Magyar
    "On", // Catalр
#if VDRVERSNUM >= 10300
    "вкл"  // Русский (Russian)
#endif
  },
  { "MHz", // English
    "MHz", // Deutsch
    "MHz", // Slovenski
    "MHz", // Italiano
    "MHz", // Nederlands
    "MHz", // Portuguъs
    "MHz", // Franчais
    "MHz", // Norsk
    "MHz", // suomi
    "MHz", // Polski
    "MHz", // Espaёol
    "MHz", // Хыычэщъм (Greek)
    "MHz", // Svenska
    "MHz", // Romaneste
    "MHz", // Magyar
    "MHz", // Catalр
#if VDRVERSNUM >= 10300
    "МГц"  // Русский (Russian)
#endif
  },
  { "Mbit/s", // English
    "Mbit/s", // Deutsch
    "Mbit/s", // Slovenski
    "Mbit/s", // Italiano
    "Mbit/s", // Nederlands
    "Mbit/s", // Portuguъs
    "Mbit/s", // Franчais
    "Mbit/s", // Norsk
    "Mbit/s", // suomi
    "Mbit/s", // Polski
    "Mbit/s", // Espaёol
    "Mbit/s", // Хыычэщъм (Greek)
    "Mbit/s", // Svenska
    "Mbit/s", // Romaneste
    "Mbit/s", // Magyar
    "Mbit/s", // Catalр
#if VDRVERSNUM >= 10300
    "Мбит/с"  // Русский (Russian)
#endif
  },
  { "kbit/s", // English
    "kbit/s", // Deutsch
    "kbit/s", // Slovenski
    "kbit/s", // Italiano
    "kbit/s", // Nederlands
    "kbit/s", // Portuguъs
    "kbit/s", // Franчais
    "kbit/s", // Norsk
    "kbit/s", // suomi
    "kbit/s", // Polski
    "kbit/s", // Espaёol
    "kbit/s", // Хыычэщъм (Greek)
    "kbit/s", // Svenska
    "kbit/s", // Romaneste
    "kbit/s", // Magyar
    "kbit/s", // Catalр
#if VDRVERSNUM >= 10300
    "кбит/с"  // Русский (Russian)
#endif
  },
  { NULL }
};
