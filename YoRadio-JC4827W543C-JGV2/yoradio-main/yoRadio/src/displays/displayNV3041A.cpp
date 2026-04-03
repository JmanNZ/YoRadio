#include "../core/options.h"
#if DSP_MODEL==DSP_NV3041A

#include "displayNV3041A.h"
#include "fonts/bootlogo2.h"
#include "../core/config.h"
#include "../core/network.h"

#include "Arduino_GFX.h"
Arduino_DataBus *bus = new Arduino_ESP32QSPI(TFT_CS, TFT_SCK, TFT_D0, TFT_D1, TFT_D2, TFT_D3);
DspCore::DspCore(): Arduino_NV3041A(bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */, true /* IPS */) {}

#include "tools/utf8RusGFX.h"
///////////////////////////////////////////////////////////////
#ifndef BATTERY_OFF
  #include "driver/adc.h"			// Ïîäêëþ÷åíèå íåîáõîäèìîãî äðàéâåðà
  #include "esp_adc_cal.h"			// Ïîäêëþ÷åíèå íåîáõîäèìîé áèáëèîòåêè
  #ifndef DELTA_BAT
    #define DELTA_BAT 0	// Âåëè÷èíà êîððåêöèè íàïðÿæåíèÿ áàòàðåè
  #endif
  #ifndef R1
    #define R1 50		// Íîìèíàë ðåçèñòîðà íà ïëþñ (+)
  #endif
  #ifndef R2
    #define R2 100	// Íîìèíàë ðåçèñòîðà íà ïëþñ (-)
  #endif

  float ADC_R1 = R1;		// Íîìèíàë ðåçèñòîðà íà ïëþñ (+)
  float ADC_R2 = R2;		// Íîìèíàë ðåçèñòîðà íà ìèíóñ (-)
  float DELTA = DELTA_BAT;	// Âåëè÷èíà êîððåêöèè íàïðÿæåíèÿ áàòàðåè

  int g = 1;		// Ñ÷¸ò÷èê äëÿ ìèãàëîê
  bool Charging;		// Ïðèçíàê, ÷òî ïîäêëþ÷åíî çàðÿäíîå óñòðîéñòâî

  int reads = 100;    		// Êîëè÷åñòâî çàìåðîâ â îäíîì èçìåðåíèè
  float Volt = 0;		       // Íàïðÿæåíèå íà áàòàðåå
  float Volt1 = 0, Volt2 = 0, Volt3 = 0, Volt4 = 0;	 // Ïðåäûäóùèå çàìåðû íàïðÿæåíèÿ íà áàòàðåå
  static esp_adc_cal_characteristics_t adc1_chars;

  int ChargeLevel;
// ==================== Ìàññèâ íàïðÿæåíèé íà áàòàðåå, ñîîòâåòñòâóþùèé ïðîöåíòó îñòàâøåãîñÿ çàðÿäà: 
  float vs[22] = {2.60, 3.10, 3.20, 3.26, 3.29, 3.33, 3.37, 3.41, 3.46, 3.51, 3.56, 3.61, 3.65, 3.69, 3.72, 3.75, 3.78, 3.82, 3.88, 3.95, 4.03, 4.25};

  #endif
/////////////////////////////////////////////////////////////////////////////////
void DspCore::initDisplay() {
  begin();
  init();
  flip();
  setTextWrap(false);
  setTextSize(1);
  fillScreen(0x0000);
  invert();
  
  plItemHeight = playlistConf.widget.textsize*(CHARHEIGHT-1)+playlistConf.widget.textsize*4;
  plTtemsCount = round((float)height()/plItemHeight);
  if(plTtemsCount%2==0) plTtemsCount++;
  plCurrentPos = plTtemsCount/2;
  plYStart = (height() / 2 - plItemHeight / 2) - plItemHeight * (plTtemsCount - 1) / 2 + playlistConf.widget.textsize*2;
}

//void DspCore::drawLogo(uint16_t top) { drawRGBBitmap((width() - 240) / 2,(height() - 240) / 4, bootlogo2, 240, 240);
void DspCore::drawLogo(uint16_t top) { drawRGBBitmap((width() - 272) / 2, 1, bootlogo240, 272, 272);
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

uint8_t DspCore::_charWidth(unsigned char c){
  GFXglyph *glyph = pgm_read_glyph_ptr(&DS_DIGI56pt7b, c - 0x20);
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
  setTextSize(4);
  setTextColor(config.theme.seconds, config.theme.background);
  setCursor(width() + 25 - clockRightSpace - CHARWIDTH*8, clockTop-clockTimeHeight+1); // //////////////////////////////////////////////
  sprintf(_bufforseconds, "%02d", network.timeinfo.tm_sec);
  if(!config.isScreensaver) print(_bufforseconds);                                      /* print seconds */
  setTextSize(1);
  setFont(&DS_DIGI56pt7b);
  setTextColor((network.timeinfo.tm_sec % 2 == 0) ? config.theme.clock : (CLOCKFONT_MONO?config.theme.clockbg:config.theme.background), config.theme.background);
  setCursor(_timeleft+_dotsLeft, clockTop);
  print(":");                                     /* print dots */
  setFont();

/////////////////////////////////////////////////////////////////////////////
  #ifndef BATTERY_OFF
  if(!config.isScreensaver) {
// ================================ Îòðèñîâêà ìèãàëîê ===================================
  setTextSize(BatFS);		//    setTextSize(2)

  if (Charging)		// Åñëè èä¸ò çàðÿäêà (ïîäêëþ÷åíî çàðÿäíîå óñòð-âî) - áåãàþùèå êâàäðàòèêè - öâåò Ñâåòëîñèíèé (Cyan)
    {
	setTextColor(color565(0, 255, 255), config.theme.background);				// Ñâåòëîñèíèé (Cyan)
	if (network.timeinfo.tm_sec % 1 == 0)
	{
		if (g == 1) {setCursor(BatX, BatY); print("\xA0\xA2\x9E\x9F");}     // 2 êâàä. â êîíöå
		if (g == 2) {setCursor(BatX, BatY); print("\xA0\x9E\x9E\xA3");}     // 2 êâàä. ïî êðàÿì
		if (g == 3) {setCursor(BatX, BatY); print("\x9D\x9E\xA2\xA3");}     // 2 êâàä. â íà÷àëå
		if (g >= 4) {g = 0; setCursor(BatX, BatY); print("\x9D\xA2\xA2\x9F");}     // 2 êâàä. â ñåðåäèíå
                g++;
	}
    }

// ============================= Îòðèñîâêà ïðåäóïðåæäàþùåé ìèãàëêè ==========================
    if (Volt < 2.8 )                 //ìèãàþùèå êâàäðàòèêè - Êðàñíûé (Red)
    {
     if (network.timeinfo.tm_sec % 1 == 0)
      {
         if (g == 1) {setTextColor(color565(255, 0, 0), config.theme.background); setCursor(BatX, BatY); print("\xA0\xA2\xA2\xA3");}          // ïîëíàÿ - 6 êâ.
         if (g >= 2) {g = 0; setTextColor(color565(255, 0, 0), config.theme.background); setCursor(BatX, BatY); print("\x9D\x9E\x9E\x9F");}           // ïóñòàÿ - 0 êâ.
         g++;
      }
     }

// ========================== Ðàñ÷¸ò è îòðèñîâêà íàïðÿæåíèé è çàðÿäà ==========================
   if (network.timeinfo.tm_sec % 6 == 0)
  {

          //  ×èòàåì ÀÖÏ "reads"= ðàç è ñêëàäûâàåì ðåçóëüòàò â ìèëëèâîëüòàõ
  float tempmVolt = 0;
  for(int i = 0; i < reads; i++){

         //  Íàñòðîéêà è èíèöèàëèçàöèÿ ÀÖÏ
//     adc1_config_width(ADC_WIDTH_12Bit); 
//     adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_11db);
     adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_12);
     adc1_ulp_enable();

          //  Ðàñ÷åò õàðàêòåðèñòèê ÀÖÏ ò.å. êîýôôèöèåíòû óñèëåíèÿ è ñìåùåíèÿ
//     esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 0, &adc1_chars);
     esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_12, 0, &adc1_chars);

     tempmVolt += esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_0), &adc1_chars);
                                        }

  float mVolt = (tempmVolt / reads) / 1000;		       //  Ïîëó÷àåì ñðåäíèé ðåçóëüòàò â Âîëüòàõ

          //  Êîððåêöèÿ ðåçóëüòàòà è ïîëó÷åíèå íàïðÿæåíèÿ áàòàðåè â Âîëüòàõ
  Volt = (mVolt + 0.0028 * mVolt * mVolt + 0.0096 * mVolt - 0.051) / (ADC_R2 / (ADC_R1 + ADC_R2)) + DELTA;
  if (Volt < 1) Volt = 0;

  if ((Volt > Volt1) && (Volt1 > Volt2) && (Volt2 > Volt3) && (Volt3 > Volt4)) {
     Charging = true;						// óñòàíîâêà ïðèçíàêà, ÷òî ïîäêëþ÷åíî çàðÿäíîå óñòðîéñòâî
     setTextColor(color565(0, 255, 255), config.theme.background);			// Ñâåòëî-ñèíèé (Cyan)
  									}
  else {Charging = false;}					// óñòàíîâêà ïðèçíàêà, ÷òî çàðÿäíîå óñòðîéñòâî íå ïîäêëþ÷åíî
  Volt4 = Volt3;
  Volt3 = Volt2;
  Volt2 = Volt1;
  Volt1 = Volt;

// ============ Ðàññ÷èòûâàåì îñòàòîê çàðÿäà â ïðîöåíòàõ ====================================
// Ïîèñê èíäåêñà, ñîîòâåòñòâóþùåãî âîëüòàæó. Èíäåêñ ñîîòâåòñòâóåò ïðîöåíòó îñòàâøåãîñÿ çàðÿäà

  int idx = 0;
  while (true) {
//================= Ïîëó÷åíèå % îñòàâøåãîñÿ çàðÿäà ============================
    if (Volt < vs[idx+1]) {mVolt = Volt - vs[idx]; ChargeLevel = idx * 5 + round(mVolt /((vs[idx+1] - vs[idx]) / 5 )); break;}
    else {idx++;}
                 }
    if (ChargeLevel < 0) ChargeLevel = 0; if (ChargeLevel > 100) ChargeLevel = 100;
// ===================== Îòðèñîâêà ñòàòè÷åñêîé áàòàðåéêè =================================
  setTextSize(BatFS);		// setTextSize(2)
  if (!Charging)		// Åñëè íå èä¸ò çàðÿäêà
  {
  setCursor(BatX, BatY);

  if (				Volt >= 3.82) {setTextColor(color565(0, 255, 0), config.theme.background); print("\xA0\xA2\xA2\xA3");}    //áîëüøå 85% (6 êâàä.) - çåë.
  if ((Volt < 3.82) && (Volt >= 3.72)) {setTextColor(color565(0, 255, 0), config.theme.background); print("\x9D\xA2\xA2\xA3");}    //îò70 äî 85% (5 êâàä.) - çåë.
  if ((Volt < 3.72) && (Volt >= 3.61)) {setTextColor(color565(0, 255, 0), config.theme.background); print("\x9D\xA1\xA2\xA3");}    //îò 55 äî 70% (4 êâàä.) - çåë.
  if ((Volt < 3.61) && (Volt >= 3.46)) {setTextColor(color565(0, 255, 0), config.theme.background); print("\x9D\x9E\xA2\xA3");}    //îò 40 äî 55% (3 êâàä.) - çåë.
  if ((Volt < 3.46) && (Volt >= 3.33)) {setTextColor(color565(0, 255, 0), config.theme.background); print("\x9D\x9E\xA1\xA3");}    //îò 25 äî 40% (2 êâàä.) - çåë.
  if ((Volt < 3.33) && (Volt >= 3.20)) {setTextColor(color565(255, 255, 0), config.theme.background); print("\x9D\x9E\x9E\xA3");} //îò 10 äî 25% (1 êâàä.) - æ¸ëò.
  if ((Volt < 3.20) && (Volt >= 2.8)) {setTextColor(color565(255, 0, 0), config.theme.background); print("\x9D\x9E\x9E\x9F");}      //îò 0 äî 10% (0 êâàä.) - êðàñ.
   }

  if (Volt < 2.8) {setTextColor(color565(255, 0, 0), config.theme.background);	}	// (0%) óñòàíîâêà öâåò Êðàñíûé (Red)

// =============== Âûâîä öèôðîâûõ çíà÷åíèé íàïðÿæåíèÿ  íà äèñïëåé ============================
  #ifndef HIDE_VOLT				// ========== Íà÷àëî âûâîäà íàïðÿæåíèÿ
  setTextSize(VoltFS); 			// setTextSize(2)
  setCursor(VoltX, VoltY); 		// Óñòàíîâêà êîîðäèíàò äëÿ âûâîäà íàïðÿæåíèÿ
  printf("%.3fv", Volt);			// Âûâîä íàïðÿæåíèÿ (òåêóùèì öâåòîì)
  #endif 				// =========== Êîíåö âûâîäà íàïðÿæåíèÿ

// =================== Âûâîä öèôðîâûõ çíà÷åíèé çàðÿäà íà äèñïëåé ============================
  setTextSize(ProcFS); 			// setTextSize(2)
  setCursor(ProcX, ProcY); 		// Óñòàíîâêà êîîðäèíàò äëÿ âûâîäà
  printf("%3i%%", ChargeLevel); 	// Âûâîä ïðîöåíòîâ çàðÿäà áàòàðåè (òåêóùèì öâåòîì) - ôîðìàò âïðàâî
//  printf("%i%% ", ChargeLevel); 	// Âûâîä ïðîöåíòîâ çàðÿäà áàòàðåè (òåêóùèì öâåòîì) ñ ïðîáåëîì â êîíöå - ôîðìàò âëåâî

//Serial.printf("#BATT#: ADC: %i reads, V-batt: %.3f v, Capacity: %i\n", reads, Volt, ChargeLevel);	// Âûâîä çíà÷åíèé â COM-ïîðò äëÿ êîíòðîëÿ
  }
  }
  #endif			//   #ifndef BATTERY_OFF
/////////////////////////////////////////////////////////////////////////////////
}

void DspCore::_clockDate(){
  if(_olddateleft>0)
    dsp.fillRect(_olddateleft,  clockTop+14, _olddatewidth, CHARHEIGHT*2, config.theme.background);
  setTextColor(config.theme.date, config.theme.background);
  setCursor(_dateleft-25, clockTop+25); // 20
  setTextSize(2);
  if(!config.isScreensaver) print(_dateBuf);                                            /* print date */
  strlcpy(_oldDateBuf, _dateBuf, sizeof(_dateBuf));
  _olddatewidth = _datewidth;
  _olddateleft = _dateleft;
  setTextSize(4); // 4
  setTextColor(config.theme.dow, config.theme.background);
  setCursor(width() + 25 - clockRightSpace - CHARWIDTH*8, clockTop-clockTimeHeight+40);
  if(!config.isScreensaver) print(utf8Rus(dow[network.timeinfo.tm_wday], false));
}

void DspCore::_clockTime(){
  if(_oldtimeleft>0 && !CLOCKFONT_MONO) dsp.fillRect(_oldtimeleft, clockTop-clockTimeHeight+1, _oldtimewidth, clockTimeHeight, config.theme.background);
  _timeleft = width()-clockRightSpace-CHARWIDTH*4*2-24-_timewidth; //24
  setTextSize(1);
  setFont(&DS_DIGI56pt7b); 
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
  if(!config.isScreensaver) drawFastVLine(width()-clockRightSpace-CHARWIDTH*4*2, clockTop-clockTimeHeight, clockTimeHeight+4, config.theme.div);  /*divider vert 4 */
  if(!config.isScreensaver) drawFastHLine(width()-clockRightSpace-CHARWIDTH*4*2, clockTop-clockTimeHeight+37, 80, config.theme.div);              /*divider hor* 120 */
  sprintf(_buffordate, "%2d %s %d", network.timeinfo.tm_mday,mnths[network.timeinfo.tm_mon], network.timeinfo.tm_year+1900);
  strlcpy(_dateBuf, utf8Rus(_buffordate, true), sizeof(_dateBuf));
  _datewidth = strlen(_dateBuf) * CHARWIDTH*2;
  _dateleft = width() + 50 - clockRightSpace - _datewidth;

}

void DspCore::printClock(uint16_t top, uint16_t rightspace, uint16_t timeheight, bool redraw){
  clockTop = top;
  clockRightSpace = rightspace;
  clockTimeHeight = timeheight;
  strftime(_timeBuf, sizeof(_timeBuf), "%H:%M", &network.timeinfo);
  if(strcmp(_oldTimeBuf, _timeBuf)!=0 || redraw){
    _getTimeBounds();
    _clockTime();
	_clockDate(); // Added
    if(!config.isScreensaver)
    if(strcmp(_oldDateBuf, _dateBuf)!=0 || redraw) _clockDate();
  }
  _clockSeconds();
}

void DspCore::clearClock(){
  dsp.fillRect(_timeleft,  clockTop-clockTimeHeight, _timewidth+CHARWIDTH*3*2+24, clockTimeHeight+12+CHARHEIGHT, config.theme.background);
}

void DspCore::startWrite(void) {
  //ILI9486_SPI::startWrite();
}

void DspCore::endWrite(void) { 
  //ILI9486_SPI::endWrite();
}
  
void DspCore::loop(bool force) { 
}

void DspCore::charSize(uint8_t textsize, uint8_t& width, uint16_t& height){
  width = textsize * CHARWIDTH;
  height = textsize * CHARHEIGHT;
}

void DspCore::setTextSize(uint8_t s){
  Arduino_GFX::setTextSize(s);
}

void DspCore::flip(){
  setRotation(config.store.flipscreen?0:2);


}

void DspCore::invert(){
  
  invertDisplay(config.store.invertdisplay);
  
}



void DspCore::sleep(void) { 
  Serial.println("DspCore::sleep");
   displayOff();
   //sendCommand(ILI9488_SLPIN); delay(150); sendCommand(ILI9488_DISPOFF); delay(150);
}

void DspCore::wake(void) {
  Serial.println("DspCore::wake");
  displayOn();
  //sendCommand(ILI9488_DISPON); delay(150); sendCommand(ILI9488_SLPOUT); delay(150);
}

void DspCore::writePixel(int16_t x, int16_t y, uint16_t color) {
  if(_clipping){
    if ((x < _cliparea.left) || (x > _cliparea.left+_cliparea.width) || (y < _cliparea.top) || (y > _cliparea.top + _cliparea.height)) return;
  }
  Arduino_GFX::drawPixel(x, y, color);
}

void DspCore::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  if(_clipping){
    if ((x < _cliparea.left) || (x >= _cliparea.left+_cliparea.width) || (y < _cliparea.top) || (y > _cliparea.top + _cliparea.height))  return;
  }
  Arduino_GFX::writeFillRect(x, y, w, h, color);
}

void DspCore::setClipping(clipArea ca){
  _cliparea = ca;
  _clipping = true;
}

void DspCore::clearClipping(){
  _clipping = false;
}

void DspCore::setNumFont(){
  setFont(&DS_DIGI56pt7b);
 // setFont(&DS_DIGI56pt7b);
  setTextSize(1);

}
#endif
