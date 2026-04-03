// ёRadio on Github: https://github.com/e2002/yoradio
// ёRadio - simple myoptions.h generator: https://e2002.github.io/docs/myoptions-generator.html
// Board settings: https://i.imgur.com/HdWUg0w.jpeg
//
#ifndef myoptions_h
#define myoptions_h
#define L10N_LANGUAGE     EN 
#define HOSTNAME          "Yo-Radio"

#define DSP_MODEL         DSP_NV3041A
#define BITRATE_FULL     true   // display bitrate badge
/******************************************/

#define TFT_CS  45                /*  SPI CS pin  */    
#define TFT_RST -1                /*  SPI RST pin.  set to -1 and connect to Esp EN pin */
#define TFT_SCK 47                /*  SPI DC/RS pin  */
#define TFT_D0 21
#define TFT_D1 48
#define TFT_D2 40
#define TFT_D3 39
#define GFX_BL 1 

/******************************************/
//#define DSP_MODEL         DSP_ILI9488 //DSP_ILI9341
//#define DSP_HSPI          true
//#define TFT_DC            12
//#define TFT_CS             8
//#define TFT_RST           -1
//#define TFT_MOSI          18  // TFT and TP (FSPI)
//#define TFT_MISO          2  // TFT and TP (FSPI)
//#define TFT_CLK           17  // TFT and TP (FSPI)
//#define TFT_Frequency     40000000
//#define BRIGHTNESS_PIN    10
/******************************************/

#define BME_SCL           41 // I2C Data
#define BME_SDA           42 //I2C Clk
#define BMP280_ADDRESS    0x76
#define BATTERY_OFF       true
#define SEALEVELPRESSURE_HPA 1013.25

#define BOOTDELAY         5000 // Logo delay in Milli Secoonds
/******************************************/
/*
//      I2S DAC INTERNAL                
#define PLAYER_FORCE_MONO true
#define I2S_DOUT      41 //             DIN connection. Should be set to 255 if the board is not used 
#define I2S_BCLK      42 //            BCLK Bit clock 
#define I2S_LRC        2 //            WSEL Left Right Clock
/******************************************/

//       I2S DAC                 
#define PLAYER_FORCE_MONO false
#define I2S_DOUT      7  //        DIN connection. Should be set to 255 if the board is not used
#define I2S_BCLK      15  //        BCLK Bit clock
#define I2S_LRC       6 //        SEL Left Right
/******************************************/

/******************************************/
#define VS1053_CS           255          /*  XCS pin. Should be set to 255 if the board is not used */
/******************************************/

/*  TOUCHSCREEN  */
#define  TS_MODEL   TS_MODEL_GT911

/*  Capacitive I2C touch screen  */

#define TS_SDA      8    
#define TS_SCL      4       
#define TS_INT      3    
#define TS_RST      38 

/******************************************/

/******************************************/
//#define TS_MODEL            TS_MODEL_XPT2046
//#define TS_HSPI             true
//#define TS_CS 				      15
/******************************************/

//#define SD_SPIPINS    12, 13, 11      /* SCK, MISO, MOSI */
//#define SDC_CS        10              /* Chip Select */

/******************************************/
// #define SD_HSPI             true
//#define SDC_CS              4           // Dummy Pin as CS is tied low
//#define SD_SPIPINS          13, 11, 14  // SCK, MISO, MOSI
/******************************************/
//#define IR_PIN            48

#endif
