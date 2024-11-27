#include "bmesupport.h"

#if BMESUPPORTED
#include <Wire.h>


TwoWire I2CBME = TwoWire(0);
  
BME bme;

bool BME::init(){
	 I2CBME.begin(BME_SDA, BME_SCL, 100000);
	 return begin(BMP280_ADDRESS, &I2CBME);
}

bool BME::status(){
	return status();
}


#endif
