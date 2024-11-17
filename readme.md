Collection of items related to YoRadio orginally from here https://github.com/e2002/yoradio \
Updated to enable use of newer I2S library and newer VS1053 library \
Please note this for the ESP32S3 \
This one is is a 2.4" LCD \

![JmanRadio-640-2](https://github.com/user-attachments/assets/7c842488-bbab-4a61-9cb9-01c8ba87adde)

And this one is a 4" LCD 

![JmanRadio-640-1](https://github.com/user-attachments/assets/28d00591-6ea2-4e18-b0a9-daa9481dd870)

Added Support for the BME280 Sensor\
This has Theme support so you customize the colors new line added to mytheme.h #define COLOR_TEMP\
myoptions.h has 3 new lines use these to setup your sensor defaults are\
#define BME_SCL 41 // Use your pin number\
#define BME_SDA 42 // Use your pin number\ 
#define BMP280_ADDRESS   0x76 // I2C address of sensor sometimes 0x77
Requires Addfruit BME280 library\
 \
 
