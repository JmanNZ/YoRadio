/*************************************************************************************
    ST7796 480X320 displays configuration file.
    Copy this file to yoRadio/src/displays/conf/displayST7789conf_custom.h
    and modify it
    More info on https://github.com/e2002/yoradio/wiki/Widgets#widgets-description
*************************************************************************************/

#ifndef displayILI9488conf_h
#define displayILI9488conf_h

#define DSP_WIDTH       480
#define DSP_HEIGHT      320
#define TFT_FRAMEWDT    10
#define MAX_WIDTH       DSP_WIDTH-TFT_FRAMEWDT*2

#if BITRATE_FULL
  #define TITLE_FIX 44
#else
  #define TITLE_FIX 0
#endif
#define bootLogoTop     110

/* SROLLS  */                            /* {{ left, top, fontsize, align }, buffsize, uppercase, width, scrolldelay, scrolldelta, scrolltime } */
const ScrollConfig metaConf       PROGMEM = {{ TFT_FRAMEWDT, TFT_FRAMEWDT, 4, WA_LEFT }, 140, false, MAX_WIDTH, 5000, 4, 40 };
const ScrollConfig title1Conf     PROGMEM = {{ TFT_FRAMEWDT, 62, 2, WA_LEFT }, 140, false, MAX_WIDTH-(TITLE_FIX==0?6*2*7-6:TITLE_FIX), 5000, 4, 40 };
const ScrollConfig title2Conf     PROGMEM = {{ TFT_FRAMEWDT, 86, 2, WA_LEFT }, 140, false, MAX_WIDTH-TITLE_FIX, 5000, 4, 40 };
const ScrollConfig playlistConf   PROGMEM = {{ TFT_FRAMEWDT, 146, 3, WA_LEFT }, 140, false, MAX_WIDTH, 1000, 4, 40 };
const ScrollConfig apTitleConf    PROGMEM = {{ TFT_FRAMEWDT, TFT_FRAMEWDT, 4, WA_CENTER }, 140, false, MAX_WIDTH, 0, 4, 40 };
const ScrollConfig apSettConf     PROGMEM = {{ TFT_FRAMEWDT, 320-TFT_FRAMEWDT-16, 2, WA_LEFT }, 140, false, MAX_WIDTH, 0, 4, 40 };
const ScrollConfig weatherConf    PROGMEM = {{ TFT_FRAMEWDT, 116, 2, WA_LEFT }, 140, false, MAX_WIDTH, 0, 4, 40 };

/* BACKGROUNDS  */                       /* {{ left, top, fontsize, align }, width, height, outlined } */
const FillConfig   metaBGConf     PROGMEM = {{ 0, 0, 0, WA_LEFT }, DSP_WIDTH, 50, true };
const FillConfig   metaBGConfInv  PROGMEM = {{ 0, 50, 0, WA_LEFT }, DSP_WIDTH, 2, true };
const FillConfig   volbarConf     PROGMEM = {{ TFT_FRAMEWDT, DSP_HEIGHT-TFT_FRAMEWDT-8, 0, WA_LEFT }, MAX_WIDTH, 8, true };
const FillConfig  playlBGConf     PROGMEM = {{ 0, 138, 0, WA_LEFT }, DSP_WIDTH, 36, true };
const FillConfig  heapbarConf     PROGMEM = {{ 0, DSP_HEIGHT-2, 0, WA_LEFT }, DSP_WIDTH, 2, true };

/* WIDGETS  */                           /* { left, top, fontsize, align } */
const WidgetConfig bootstrConf    PROGMEM = { 0, 300, 2, WA_CENTER }; // 243
const WidgetConfig bitrateConf    PROGMEM = { 6, 62, 2, WA_RIGHT };
const WidgetConfig voltxtConf     PROGMEM = { 0, DSP_HEIGHT-38, 2, WA_CENTER };
const WidgetConfig  iptxtConf     PROGMEM = { TFT_FRAMEWDT, DSP_HEIGHT-38, 2, WA_LEFT };
const WidgetConfig   rssiConf     PROGMEM = { TFT_FRAMEWDT, DSP_HEIGHT-38-6, 3, WA_RIGHT };
const WidgetConfig  tempConf      PROGMEM = { 100, DSP_HEIGHT-80, 2, WA_LEFT }; // Addded by Jman
const WidgetConfig numConf        PROGMEM = { 0, 200, 70, WA_CENTER };
const WidgetConfig apNameConf     PROGMEM = { TFT_FRAMEWDT, 88, 3, WA_CENTER };
const WidgetConfig apName2Conf    PROGMEM = { TFT_FRAMEWDT, 120, 3, WA_CENTER };
const WidgetConfig apPassConf     PROGMEM = { TFT_FRAMEWDT, 173, 3, WA_CENTER };
const WidgetConfig apPass2Conf    PROGMEM = { TFT_FRAMEWDT, 205, 3, WA_CENTER };
const WidgetConfig  clockConf     PROGMEM = { 16, 224, 70, WA_RIGHT };  /* 52 is a fixed font size. do not change */
const WidgetConfig vuConf         PROGMEM = { TFT_FRAMEWDT, 136, 1, WA_LEFT };

const WidgetConfig bootWdtConf    PROGMEM = { 0, 216, 1, WA_CENTER };
const ProgressConfig bootPrgConf  PROGMEM = { 90, 14, 4 };
const BitrateConfig fullbitrateConf PROGMEM = {{DSP_WIDTH-TFT_FRAMEWDT-38, 59, 2, WA_LEFT}, 42 };

/* BANDS  */                             /* { onebandwidth, onebandheight, bandsHspace, bandsVspace, numofbands, fadespeed } */
const VUBandsConfig bandsConf     PROGMEM = { 32, 130, 4, 2, 10, 10 };

/* STRINGS  */
const char         numtxtFmt[]    PROGMEM = "%d";
const char           rssiFmt[]    PROGMEM = "WiFi %d";
const char           tempFmt[]    PROGMEM = "%srH"; // Added by jman 
const char          iptxtFmt[]    PROGMEM = "%s";
const char         voltxtFmt[]    PROGMEM = "\023\025%d";
const char        bitrateFmt[]    PROGMEM = "%d kBs";

/* MOVES  */                             /* { left, top, width } */
const MoveConfig    clockMove     PROGMEM = { 0, 176, -1 };
const MoveConfig   weatherMove    PROGMEM = { 8, 120, MAX_WIDTH };
const MoveConfig   weatherMoveVU  PROGMEM = { 8, 120, MAX_WIDTH }; //{ 89, 120, 381 };

#endif
