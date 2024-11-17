#ifndef bmesupport_h
#define bmesupport_h
#include "options.h"
#define BMESUPPORTED (BME_SDA!=255 && BME_SCL!=255)

#if BMESUPPORTED
#include <Adafruit_BME280.h>

class BME: public Adafruit_BME280 {

  public:  
  bool init();
  bool status();
  };
	
extern BME bme;
#endif

#endif
