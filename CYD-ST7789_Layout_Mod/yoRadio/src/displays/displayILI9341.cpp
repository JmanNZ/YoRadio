#include "../core/options.h"
#if DSP_MODEL==DSP_ILI9341

#include "displayILI9341.h"
#include "fonts/bootlogo240.h"
#include "../core/config.h"
#include "../core/network.h"

#if DSP_HSPI
DspCore::DspCore(): Adafruit_ILI9341(&SPI2, TFT_DC, TFT_CS, TFT_RST) {}
#else
DspCore::DspCore(): Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST) {}
#endif

#include "tools/utf8RusGFX.h"

///////////////////////////////////////////////////////////////
#ifndef BATTERY_OFF

  #include "driver/adc.h"			// Подключение необходимого драйвера
  #include "esp_adc_cal.h"			// Подключение необходимой библиотеки

  #ifndef DELTA_BAT
    #define DELTA_BAT 0	// Величина коррекции напряжения батареи
  #endif
  #ifndef R1
    #define R1 50		// Номинал резистора на плюс (+)
  #endif
  #ifndef R2
    #define R2 100	// Номинал резистора на плюс (-)
  #endif

  float ADC_R1 = R1;		// Номинал резистора на плюс (+)
  float ADC_R2 = R2;		// Номинал резистора на минус (-)
  float DELTA = DELTA_BAT;	// Величина коррекции напряжения батареи

  int g = 1;		// Счётчик для мигалок
  bool Charging;		// Признак, что подключено зарядное устройство

  int reads = 100;    		// Количество замеров в одном измерении
  float Volt = 0;		       // Напряжение на батарее
  float Volt1 = 0, Volt2 = 0, Volt3 = 0, Volt4 = 0;	 // Предыдущие замеры напряжения на батарее
  static esp_adc_cal_characteristics_t adc1_chars;

  int ChargeLevel;
// ==================== Массив напряжений на батарее, соответствующий проценту оставшегося заряда: 
  float vs[22] = {2.60, 3.10, 3.20, 3.26, 3.29, 3.33, 3.37, 3.41, 3.46, 3.51, 3.56, 3.61, 3.65, 3.69, 3.72, 3.75, 3.78, 3.82, 3.88, 3.95, 4.03, 4.25};

  #endif
/////////////////////////////////////////////////////////////////////////////////

void DspCore::initDisplay() {
  begin();             /* SPI_DEFAULT_FREQ 40000000 */
  invert();
  cp437(true);
  flip();
  setTextWrap(false);
  
  plItemHeight = playlistConf.widget.textsize*(CHARHEIGHT-1)+playlistConf.widget.textsize*4;
  plTtemsCount = round((float)height()/plItemHeight);
  if(plTtemsCount%2==0) plTtemsCount++;
  plCurrentPos = plTtemsCount/2;
  plYStart = (height() / 2 - plItemHeight / 2) - plItemHeight * (plTtemsCount - 1) / 2 + playlistConf.widget.textsize*2;
}

void DspCore::drawLogo(uint16_t top) { drawRGBBitmap((width() - 240) / 2,(height() - 240) / 4, bootlogo240, 240, 240);
//void DspCore::drawLogo(uint16_t top) { drawRGBBitmap((width() - 320) / 2, 1, bootlogo240, 320, 240);
  delay (5000);  //  5 secondi
  }

void DspCore::printPLitem(uint8_t pos, const char* item, ScrollWidget& current){
  setTextSize(playlistConf.widget.textsize);
  if (pos == plCurrentPos) {
    current.setText(item);
  } else {
    uint8_t plColor = (abs(pos - plCurrentPos)-1)>4?4:abs(pos - plCurrentPos)-1;
    setTextColor(config.theme.playlist[plColor], config.theme.background);
    setCursor(TFT_FRAMEWDT, plYStart + pos * plItemHeight);
    fillRect(0, plYStart + pos * plItemHeight - 1, width(), plItemHeight - 2, config.theme.background);
    print(utf8Rus(item, true));
  }
}

void DspCore::drawPlaylist(uint16_t currentItem) {
  uint8_t lastPos = config.fillPlMenu(currentItem - plCurrentPos, plTtemsCount);
  if(lastPos<plTtemsCount){
    fillRect(0, lastPos*plItemHeight+plYStart, width(), height()/2, config.theme.background);
  }
}

void DspCore::clearDsp(bool black) { fillScreen(black?0:config.theme.background); }

GFXglyph *pgm_read_glyph_ptr(const GFXfont *gfxFont, uint8_t c) {
  return gfxFont->glyph + c;
}

uint8_t DspCore::_charWidth(unsigned char c){
  GFXglyph *glyph = pgm_read_glyph_ptr(&DS_DIGI42pt7b, c - 0x20);
  return pgm_read_byte(&glyph->xAdvance);
}

uint16_t DspCore::textWidth(const char *txt){
  uint16_t w = 0, l=strlen(txt);
  for(uint16_t c=0;c<l;c++) w+=_charWidth(txt[c]);
  return w;
}

void DspCore::_getTimeBounds() {
  _timewidth = textWidth(_timeBuf);
  char buf[4];
  strftime(buf, 4, "%H", &network.timeinfo);
  _dotsLeft=textWidth(buf);
}

void DspCore::_clockSeconds(){
  setTextSize(5);
  setTextColor(config.theme.seconds, config.theme.background);
  // setCursor(width() - 10 - clockRightSpace - CHARWIDTH*3*2, clockTop-clockTimeHeight+1);
  setCursor(width() - 5 - clockRightSpace - CHARWIDTH*3*2, clockTop-CHARHEIGHT*3-14);
  // print(".");
  sprintf(_bufforseconds, "%02d", network.timeinfo.tm_sec);  /* print seconds */
  if(!config.isScreensaver) print(_bufforseconds);
  setTextSize(1);
  setFont(&DS_DIGI42pt7b);
  setTextColor((network.timeinfo.tm_sec % 2 == 0) ? config.theme.clock : (CLOCKFONT_MONO?config.theme.clockbg:config.theme.background), config.theme.background);
  setCursor(_timeleft+_dotsLeft, clockTop);
  print(":");                                     /* print dots */
  setCursor(_timeleft+_dotsLeft+ 94, clockTop);
  print(":");                                     /* print dots 2 */
  
  setFont();                                    

/////////////////////////////////////////////////////////////////////////////
  #ifndef BATTERY_OFF
  if(!config.isScreensaver) {
// ================================ Отрисовка мигалок ===================================
  setTextSize(BatFS);		//    setTextSize(2)

  if (Charging)		// Если идёт зарядка (подключено зарядное устр-во) - бегающие квадратики - цвет Светлосиний (Cyan)
    {
	setTextColor(color565(0, 255, 255), config.theme.background);				// Светлосиний (Cyan)
	if (network.timeinfo.tm_sec % 1 == 0)
	{
		if (g == 1) {setCursor(BatX, BatY); print("\xA0\xA2\x9E\x9F");}     // 2 квад. в конце
		if (g == 2) {setCursor(BatX, BatY); print("\xA0\x9E\x9E\xA3");}     // 2 квад. по краям
		if (g == 3) {setCursor(BatX, BatY); print("\x9D\x9E\xA2\xA3");}     // 2 квад. в начале
		if (g >= 4) {g = 0; setCursor(BatX, BatY); print("\x9D\xA2\xA2\x9F");}     // 2 квад. в середине
                g++;
	}
    }

// ============================= Отрисовка предупреждающей мигалки ==========================
    if (Volt < 2.8 )                 //мигающие квадратики - Красный (Red)
    {
     if (network.timeinfo.tm_sec % 1 == 0)
      {
         if (g == 1) {setTextColor(color565(255, 0, 0), config.theme.background); setCursor(BatX, BatY); print("\xA0\xA2\xA2\xA3");}          // полная - 6 кв.
         if (g >= 2) {g = 0; setTextColor(color565(255, 0, 0), config.theme.background); setCursor(BatX, BatY); print("\x9D\x9E\x9E\x9F");}           // пустая - 0 кв.
         g++;
      }
     }

// ========================== Расчёт и отрисовка напряжений и заряда ==========================
   if (network.timeinfo.tm_sec % 6 == 0)
  {

          //  Читаем АЦП "reads"= раз и складываем результат в милливольтах
  float tempmVolt = 0;
  for(int i = 0; i < reads; i++){

         //  Настройка и инициализация АЦП
//     adc1_config_width(ADC_WIDTH_12Bit); 
//     adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_11db);
     adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_12);
     adc1_ulp_enable();

          //  Расчет характеристик АЦП т.е. коэффициенты усиления и смещения
//     esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 0, &adc1_chars);
     esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_12, 0, &adc1_chars);

     tempmVolt += esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_0), &adc1_chars);
                                        }

  float mVolt = (tempmVolt / reads) / 1000;		       //  Получаем средний результат в Вольтах

          //  Коррекция результата и получение напряжения батареи в Вольтах
  Volt = (mVolt + 0.0028 * mVolt * mVolt + 0.0096 * mVolt - 0.051) / (ADC_R2 / (ADC_R1 + ADC_R2)) + DELTA;
  if (Volt < 1) Volt = 0;

  if ((Volt > Volt1) && (Volt1 > Volt2) && (Volt2 > Volt3) && (Volt3 > Volt4)) {
     Charging = true;						// установка признака, что подключено зарядное устройство
     setTextColor(color565(0, 255, 255), config.theme.background);			// Светло-синий (Cyan)
  									}
  else {Charging = false;}					// установка признака, что зарядное устройство не подключено
  Volt4 = Volt3;
  Volt3 = Volt2;
  Volt2 = Volt1;
  Volt1 = Volt;

// ============ Рассчитываем остаток заряда в процентах ====================================
// Поиск индекса, соответствующего вольтажу. Индекс соответствует проценту оставшегося заряда

  int idx = 0;
  while (true) {
//================= Получение % оставшегося заряда ============================
    if (Volt < vs[idx+1]) {mVolt = Volt - vs[idx]; ChargeLevel = idx * 5 + round(mVolt /((vs[idx+1] - vs[idx]) / 5 )); break;}
    else {idx++;}
                 }
    if (ChargeLevel < 0) ChargeLevel = 0; if (ChargeLevel > 100) ChargeLevel = 100;
// ===================== Отрисовка статической батарейки =================================
  setTextSize(BatFS);		// setTextSize(2)
  if (!Charging)		// Если не идёт зарядка
  {
  setCursor(BatX, BatY);

  if (				Volt >= 3.82) {setTextColor(color565(0, 255, 0), config.theme.background); print("\xA0\xA2\xA2\xA3");}    //больше 85% (6 квад.) - зел.
  if ((Volt < 3.82) && (Volt >= 3.72)) {setTextColor(color565(0, 255, 0), config.theme.background); print("\x9D\xA2\xA2\xA3");}    //от70 до 85% (5 квад.) - зел.
  if ((Volt < 3.72) && (Volt >= 3.61)) {setTextColor(color565(0, 255, 0), config.theme.background); print("\x9D\xA1\xA2\xA3");}    //от 55 до 70% (4 квад.) - зел.
  if ((Volt < 3.61) && (Volt >= 3.46)) {setTextColor(color565(0, 255, 0), config.theme.background); print("\x9D\x9E\xA2\xA3");}    //от 40 до 55% (3 квад.) - зел.
  if ((Volt < 3.46) && (Volt >= 3.33)) {setTextColor(color565(0, 255, 0), config.theme.background); print("\x9D\x9E\xA1\xA3");}    //от 25 до 40% (2 квад.) - зел.
  if ((Volt < 3.33) && (Volt >= 3.20)) {setTextColor(color565(255, 255, 0), config.theme.background); print("\x9D\x9E\x9E\xA3");} //от 10 до 25% (1 квад.) - жёлт.
  if ((Volt < 3.20) && (Volt >= 2.8)) {setTextColor(color565(255, 0, 0), config.theme.background); print("\x9D\x9E\x9E\x9F");}      //от 0 до 10% (0 квад.) - крас.
   }

  if (Volt < 2.8) {setTextColor(color565(255, 0, 0), config.theme.background);	}	// (0%) установка цвет Красный (Red)

// =============== Вывод цифровых значений напряжения  на дисплей ============================
  #ifndef HIDE_VOLT				// ========== Начало вывода напряжения
  setTextSize(VoltFS); 			// setTextSize(2)
  setCursor(VoltX, VoltY); 		// Установка координат для вывода напряжения
  printf("%.3fv", Volt);			// Вывод напряжения (текущим цветом)
  #endif 				// =========== Конец вывода напряжения

// =================== Вывод цифровых значений заряда на дисплей ============================
  setTextSize(ProcFS); 			// setTextSize(2)
  setCursor(ProcX, ProcY); 		// Установка координат для вывода
  printf("%3i%%", ChargeLevel); 	// Вывод процентов заряда батареи (текущим цветом) - формат вправо
//  printf("%i%% ", ChargeLevel); 	// Вывод процентов заряда батареи (текущим цветом) с пробелом в конце - формат влево

//Serial.printf("#BATT#: ADC: %i reads, V-batt: %.3f v, Capacity: %i\n", reads, Volt, ChargeLevel);	// Вывод значений в COM-порт для контроля
  }
  }
  #endif			//   #ifndef BATTERY_OFF
/////////////////////////////////////////////////////////////////////////////////
}

void DspCore::_clockDate(){
  setTextSize(2);
  #ifndef HIDE_DATE
    if(_olddateleft>0)    dsp.fillRect(_olddateleft,  clockTop+10, _olddatewidth, CHARHEIGHT, config.theme.background);
    setTextColor(config.theme.date, config.theme.background);
    setCursor(_dateleft, clockTop+10);
  if(!config.isScreensaver) print(_dateBuf);                                            /* print date */
  #endif
  strlcpy(_oldDateBuf, _dateBuf, sizeof(_dateBuf));
  _olddatewidth = _datewidth;
  _olddateleft = _dateleft;
  setTextSize(2);
  setTextColor(config.theme.dow, config.theme.background);
 // setCursor(width() - 20 - clockRightSpace - CHARWIDTH*3*2, clockTop-CHARHEIGHT*3+10);
 setCursor (0,162);
  print(utf8Rus(dow[network.timeinfo.tm_wday], true));       /* print giorno settimana */
   }

void DspCore::_clockTime(){
  if(_oldtimeleft>0 && !CLOCKFONT_MONO) dsp.fillRect(_oldtimeleft, clockTop-clockTimeHeight, _oldtimewidth, clockTimeHeight+1, config.theme.background);
  _timeleft = width() - clockRightSpace - CHARWIDTH*3*2 - 24 - _timewidth;
  setTextSize(1);
  setFont(&DS_DIGI42pt7b);
  
  if(CLOCKFONT_MONO) {
    setCursor(_timeleft, clockTop);
    setTextColor(config.theme.clockbg, config.theme.background);
    print("88:88");
  }
  setTextColor(config.theme.clock, config.theme.background);
  setCursor(_timeleft, clockTop);
  print(_timeBuf);
  setFont();
  strlcpy(_oldTimeBuf, _timeBuf, sizeof(_timeBuf));
  _oldtimewidth = _timewidth;
  _oldtimeleft = _timeleft;
 // if(!config.isScreensaver) drawFastVLine(width()-clockRightSpace-CHARWIDTH*3*2-24, clockTop-clockTimeHeight, clockTimeHeight+3, config.theme.div);  /*divider vert*/
 // if(!config.isScreensaver) drawFastHLine(width()-clockRightSpace-CHARWIDTH*3*2-24, clockTop-clockTimeHeight+29, 83, config.theme.div);              /*divider hor*/
 
  sprintf(_buffordate, "%2d %s %d", network.timeinfo.tm_mday,mnths[network.timeinfo.tm_mon], network.timeinfo.tm_year+1900);
  
  strlcpy(_dateBuf, utf8Rus(_buffordate, false), sizeof(_dateBuf));
  _datewidth = strlen(_dateBuf) * CHARWIDTH * 2;
  _dateleft = _timeleft + 25+(width() - _timeleft - _datewidth - clockRightSpace) / 2;
   
}

void DspCore::printClock(uint16_t top, uint16_t rightspace, uint16_t timeheight, bool redraw){
  clockTop = top;
  clockRightSpace = rightspace;
  clockTimeHeight = timeheight;
  strftime(_timeBuf, sizeof(_timeBuf), "%H:%M", &network.timeinfo);
  if(strcmp(_oldTimeBuf, _timeBuf)!=0 || redraw){
    _getTimeBounds();
    _clockTime();
    if(!config.isScreensaver) 
      if(strcmp(_oldDateBuf, _dateBuf)!=0 || redraw) _clockDate();
  }
  _clockSeconds();
}

void DspCore::clearClock(){
  dsp.fillRect(_timeleft,  clockTop-clockTimeHeight, _timewidth+CHARWIDTH*3*2+24, clockTimeHeight+10+CHARHEIGHT, config.theme.background);
}

void DspCore::startWrite(void) {
  Adafruit_ILI9341::startWrite();
}

void DspCore::endWrite(void) {
  Adafruit_ILI9341::endWrite();
}

void DspCore::loop(bool force) { }

void DspCore::charSize(uint8_t textsize, uint8_t& width, uint16_t& height){
  width = textsize * CHARWIDTH;
  height = textsize * CHARHEIGHT;
}

void DspCore::setTextSize(uint8_t s){
  Adafruit_GFX::setTextSize(s);
}

void DspCore::flip(){
  setRotation(config.store.flipscreen?1:3);
}

void DspCore::invert(){
  invertDisplay(config.store.invertdisplay);
}

void DspCore::sleep(void) { sendCommand(ILI9341_SLPIN); delay(150); sendCommand(ILI9341_DISPOFF); delay(150);}

void DspCore::wake(void) { sendCommand(ILI9341_DISPON); delay(150); sendCommand(ILI9341_SLPOUT); delay(150);}

void DspCore::writePixel(int16_t x, int16_t y, uint16_t color) {
  if(_clipping){
    if ((x < _cliparea.left) || (x > _cliparea.left+_cliparea.width) || (y < _cliparea.top) || (y > _cliparea.top + _cliparea.height)) return;
  }
  Adafruit_ILI9341::writePixel(x, y, color);
}

void DspCore::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  if(_clipping){
    if ((x < _cliparea.left) || (x >= _cliparea.left+_cliparea.width) || (y < _cliparea.top) || (y > _cliparea.top + _cliparea.height))  return;
  }
  Adafruit_ILI9341::writeFillRect(x, y, w, h, color);
}

void DspCore::setClipping(clipArea ca){
  _cliparea = ca;
  _clipping = true;
}

void DspCore::clearClipping(){
  _clipping = false;
}

void DspCore::setNumFont(){
  setFont(&DS_DIGI42pt7b);
  setTextSize(1);
}
#endif
