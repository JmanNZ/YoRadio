Collection of items related to YoRadio orginally from here https://github.com/e2002/yoradio \
Updated to enable use of newer I2S library Version 3.0.13v Updated on: Nov 22.2024 and update VS1053 library \
I2S Library can be found here https://github.com/schreibfaul1/ESP32-audioI2S/tree/master \
These audio libarys can play chunked streams \
Note this I2S libary requires a dual core ESP32S3 or simlair \
Includes ESP32S3 updates from the orginal author \
This one is is a 2.4" LCD 

![JmanRadio-640-2](https://github.com/user-attachments/assets/7c842488-bbab-4a61-9cb9-01c8ba87adde)

And this one is a 4" LCD 

![JmanRadio-640-1](https://github.com/user-attachments/assets/28d00591-6ea2-4e18-b0a9-daa9481dd870)

Added Support for the BME280 Sensor (only for the ILI9341 and ILI9488 you can add to your display type file)\
This has Theme support so you can customize the colors new line added to mytheme.h #define COLOR_TEMP\
myoptions.h has 3 new lines use these to setup your sensor defaults are \
#define BME_SCL 41 //Use your pin number\
#define BME_SDA 42 //Use your pin number\
#define BMP280_ADDRESS   0x76 //I2C address of sensor sometimes 0x77\
Addfruit BME280 library is now included\
Custom display layouts are in the Display folder copy the files to your ..\yoRadio\src\displays\conf folder\
if you want to use the custom layout\
4" LCD

![4-TempDisplay](https://github.com/user-attachments/assets/58424bba-b993-43ee-975d-514fc2b5400d)

 
 
