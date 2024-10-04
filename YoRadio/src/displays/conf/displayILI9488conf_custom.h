/*************************************************************************************
    ILI9488 480X320 displays configuration file.
*************************************************************************************/

#ifndef displayILI9488conf_custom_h
#define displayILI9488conf_custom_h

#define DSP_WIDTH       480
#define DSP_HEIGHT      320
#define TFT_FRAMEWDT    5
#define MAX_WIDTH       DSP_WIDTH-TFT_FRAMEWDT*2

#if BITRATE_FULL
  #define TITLE_FIX 44
#else
  #define TITLE_FIX 0
#endif
#define bootLogoTop     110
#define BOOMBOX_STYLE

/* SROLLS  */                            /* {{ left, top, fontsize, align }, buffsize, uppercase, width, scrolldelay, scrolldelta, scrolltime } */
const ScrollConfig metaConf       PROGMEM = {{ TFT_FRAMEWDT, TFT_FRAMEWDT+5, 4, WA_CENTER }, 140, false, MAX_WIDTH, 5000, 4, 40 };
const ScrollConfig title1Conf     PROGMEM = {{ TFT_FRAMEWDT, 55, 3, WA_CENTER}, 140, false, MAX_WIDTH, 5000, 4, 40 };
const ScrollConfig title2Conf     PROGMEM = {{ TFT_FRAMEWDT, 82, 3, WA_CENTER }, 140, false, MAX_WIDTH, 5000, 4, 40 };
const ScrollConfig playlistConf   PROGMEM = {{ TFT_FRAMEWDT, 146, 2, WA_LEFT }, 140, false, MAX_WIDTH, 1000, 4, 40 };
const ScrollConfig apTitleConf    PROGMEM = {{ TFT_FRAMEWDT, TFT_FRAMEWDT, 4, WA_CENTER }, 140, false, MAX_WIDTH, 0, 4, 40 };
const ScrollConfig apSettConf     PROGMEM = {{ TFT_FRAMEWDT, 320-TFT_FRAMEWDT-16, 2, WA_LEFT }, 140, false, MAX_WIDTH, 0, 4, 40 };
const ScrollConfig weatherConf    PROGMEM = {{ TFT_FRAMEWDT, 111, 3, WA_LEFT }, 300, false, MAX_WIDTH, 0, 4, 50 };

/* BACKGROUNDS  */                       /* {{ left, top, fontsize, align }, width, height, outlined } */
const FillConfig   metaBGConf     PROGMEM = {{ TFT_FRAMEWDT, 0, 0, WA_LEFT }, MAX_WIDTH, 50, false };
const FillConfig   metaBGConfInv  PROGMEM = {{ TFT_FRAMEWDT, 47, 0, WA_LEFT },MAX_WIDTH, 3, false };
const FillConfig   volbarConf     PROGMEM = {{ TFT_FRAMEWDT, DSP_HEIGHT-TFT_FRAMEWDT-12, 0, WA_LEFT }, MAX_WIDTH, 8, true };
const FillConfig  playlBGConf     PROGMEM = {{ 0, 138, 0, WA_LEFT }, DSP_WIDTH, 36, false };
const FillConfig  heapbarConf     PROGMEM = {{ TFT_FRAMEWDT, 314, 0, WA_LEFT }, MAX_WIDTH,6 , true };

/* WIDGETS  */                           /* { left, top, fontsize, align } */
const WidgetConfig bootstrConf    PROGMEM = { 0, 243, 1, WA_CENTER };
const WidgetConfig bitrateConf    PROGMEM = { TFT_FRAMEWDT, 145, 2, WA_RIGHT };
const WidgetConfig voltxtConf     PROGMEM = { 15, DSP_HEIGHT-34, 2, WA_CENTER };
const WidgetConfig  iptxtConf     PROGMEM = {  8, DSP_HEIGHT-34, 2, WA_LEFT  };
const WidgetConfig   rssiConf     PROGMEM = { 435, DSP_HEIGHT-41, 3, WA_LEFT };
const WidgetConfig numConf        PROGMEM = { 0, 200, 70, WA_CENTER };
const WidgetConfig apNameConf     PROGMEM = { TFT_FRAMEWDT, 88, 3, WA_CENTER };
const WidgetConfig apName2Conf    PROGMEM = { TFT_FRAMEWDT, 120, 3, WA_CENTER };
const WidgetConfig apPassConf     PROGMEM = { TFT_FRAMEWDT, 173, 3, WA_CENTER };
const WidgetConfig apPass2Conf    PROGMEM = { TFT_FRAMEWDT, 205, 3, WA_CENTER };
const WidgetConfig  clockConf     PROGMEM = { 16, 211, 70, WA_RIGHT };  /* 52 is a fixed font size. do not change */
const WidgetConfig vuConf         PROGMEM = { TFT_FRAMEWDT, 250, 1, WA_CENTER }; // TFT_FRAMEWDT+20, 188, 1, WA_CENTER }

const WidgetConfig bootWdtConf    PROGMEM = { 0, 216, 1, WA_CENTER };
const ProgressConfig bootPrgConf  PROGMEM = { 90, 14, 4 };

/* BANDS  */                             /* { onebandwidth, onebandheight, bandsHspace, bandsVspace, numofbands, fadespeed } */
const VUBandsConfig bandsConf     PROGMEM = { 230, 30, 5, 2, 10, 8 };
const BitrateConfig fullbitrateConf PROGMEM = {{TFT_FRAMEWDT, 150, 3, WA_RIGHT}, 85 };

/* STRINGS  */
const char         numtxtFmt[]    PROGMEM = "%d";
const char           rssiFmt[]    PROGMEM = "%d dB";
const char          iptxtFmt[]    PROGMEM = "\010 %s";
const char         voltxtFmt[]    PROGMEM = "\023\025%d";
const char        bitrateFmt[]    PROGMEM = "%d kBs";

/* MOVES  */                             /* { left, top, width } */
const MoveConfig    clockMove     PROGMEM = { 0, 176, -1 };
const MoveConfig   weatherMove    PROGMEM = { 8, 111, -1};
const MoveConfig   weatherMoveVU  PROGMEM = { 8, 111, -1};

#endif
