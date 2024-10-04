// ёRadio on Github: https://github.com/e2002/yoradio
// ёRadio - simple myoptions.h generator: https://e2002.github.io/docs/myoptions-generator.html
// Board settings: https://i.imgur.com/HdWUg0w.jpeg
//
#ifndef myoptions_h
#define myoptions_h
#define L10N_LANGUAGE     EN 
#define HOSTNAME          "Yo-Radio"
#define DSP_MODEL         DSP_ILI9488 //DSP_ILI9341
#define DSP_HSPI          true
#define TFT_DC            12
#define TFT_CS             8
#define TFT_RST           -1
#define TFT_MOSI          18  // TFT and TP (FSPI)
#define TFT_MISO          2  // TFT and TP (FSPI)
#define TFT_CLK           17  // TFT and TP (FSPI)
#define TFT_Frequency     20000000
#define BRIGHTNESS_PIN    10

#define BOOTDELAY         5000 // Logo delay in Milli Secoonds

#define VSPI FSPI

#define VS1053_CS           255          /*  XCS pin. Should be set to 255 if the board is not used */

#define I2S_DOUT            9
#define I2S_BCLK            3
#define I2S_LRC             1

#define TS_MODEL            TS_MODEL_XPT2046
#define TS_HSPI             true
#define TS_CS 				15

//#define SD_HSPI             false
#define SDC_CS              48    // Dummy Pin as CS is tied low
#define SD_SPIPINS          13, 11, 14

//#define IR_PIN              4

#endif
