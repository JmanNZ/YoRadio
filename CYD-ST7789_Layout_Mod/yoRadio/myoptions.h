// My youtube channel: https://www.youtube.com/@LeventeDaradici/videos
// My pastebin page: https://pastebin.com/u/LeventeDaradici
// ёRadio on Github: https://github.com/e2002/yoradio
// ёRadio - simple myoptions.h generator: https://e2002.github.io/docs/myoptions-generator.html
// Board settings: https://i.imgur.com/HdWUg0w.jpeg
// About how we load the file system on the board: https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide/
// The board I used: https://macsbug.wordpress.com/2022/08/17/esp32-2432s028/
// I bought the board from here:https://www.aliexpress.com/item/1005004502250619.html
// The answer that helped me solve the problem with the touchscreen: https://github.com/e2002/yoradio/issues/34
//
#ifndef myoptions_h
#define myoptions_h


#define L10N_LANGUAGE     EN 
#define HOSTNAME          "Yo-Radio"

#define DSP_MODEL           DSP_ST7789
#define DSP_HSPI            true
#define LED_INVERT          true
#define TFT_CS              15
#define TFT_DC              2
#define TFT_RST             -1
//#define LED_BUILTIN         17
#define BRIGHTNESS_PIN      21
//#define I2S_INTERNAL    false
//#define PLAYER_FORCE_MONO false



#define BME_SCL           41 // I2C Data
#define BME_SDA           42 //I2C Clk
#define BMP280_ADDRESS    0x76

#define SEALEVELPRESSURE_HPA 1013.25

#define BOOTDELAY         5000 // Logo delay in Milli Secoonds
#define BATTERY_OFF       true  

#define BOOTDELAY         5000 // Logo delay in Milli Secoonds
/*  VS1053 VSPI PINS. VS1053 SCK must be connected to pin 18
                      VS1053 MISO must be connected to pin 19
                      VS1053 MOSI must be connected to pin 23  */

#define VS1053_CS         05               //  XCS pin. Should be set to 255 if the board is not used 
#define VS1053_DCS        22               //  XDCS pin.  
#define VS1053_DREQ       04               //  DREQ pin.  
#define VS1053_RST        17               //  XRESET pin. Set to -1 if connected to Esp EN pin 
//#define VS_HSPI           false            //  Use HSPI for VS  

#define I2S_DOUT          255      //  DIN connection. Should be set to 255 if the board is not used 

#define TS_MODEL            TS_MODEL_XPT2046
#define TS_SPIPINS          25, 39, 32    // SCK, MISO, MOSI 
#define TS_CS               33
//#define IR_PIN              27
//#define LIGHT_SENSOR        34
//#define AUTOBACKLIGHT_MAX   1024

#endif
