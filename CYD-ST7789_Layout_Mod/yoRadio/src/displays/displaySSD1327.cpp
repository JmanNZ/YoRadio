#include "../core/options.h"
#if DSP_MODEL==DSP_SSD1327

#include "displaySSD1327.h"
#include <Wire.h>
#include "fonts/bootlogo40.h"
#include "../core/player.h"
#include "../core/config.h"
#include "../core/network.h"

#ifndef SCREEN_ADDRESS
  #define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; or scan it https://create.arduino.cc/projecthub/abdularbi17/how-to-scan-i2c-address-in-arduino-eaadda
#endif

#ifndef I2CFREQ_HZ
  #define I2CFREQ_HZ   6000000UL
#endif

TwoWire tw = TwoWire(0);

DspCore::DspCore(): Adafruit_SSD1327(128, 128, &tw, I2C_RST/*, I2CFREQ_HZ*/) {}

#include "tools/utf8RusGFX.h"

////////////////////////////////////////////////////////////////////////////
#ifndef BATTERY_OFF

  #include "driver/adc.h"			// ����������� ������������ ��������
  #include "esp_adc_cal.h"			// ����������� ����������� ����������

  #ifndef DELTA_BAT
    #define DELTA_BAT 0	// �������� ��������� ���������� �������
  #endif
  #ifndef R1
    #define R1 50		// ������� ��������� �� ���� (+)
  #endif
  #ifndef R2
    #define R2 100	// ������� ��������� �� ����� (-)
  #endif

  float ADC_R1 = R1;		// ������� ��������� �� ���� (+)
  float ADC_R2 = R2;		// ������� ��������� �� ����� (-)
  float DELTA = DELTA_BAT;	// �������� ��������� ���������� �������

  int g = 1, d = 1, e = 1;		// ������� ��� �������
  bool Charging;		// �������, ��� ���������� �������� ����������

  int reads = 100;    		// ���������� ������� � ����� ���������
  float Volt = 0;		       // ���������� �� �������
  float Volt1 = 0, Volt2 = 0, Volt3 = 0, Volt4 = 0;	 // ���������� ������ ���������� �� �������
  static esp_adc_cal_characteristics_t adc1_chars;

  int ChargeLevel;
// ==================== ������ ���������� �� �������, ��������������� �������� ����������� ������: 
  float vs[22] = {2.60, 3.10, 3.20, 3.26, 3.29, 3.33, 3.37, 3.41, 3.46, 3.51, 3.56, 3.61, 3.65, 3.69, 3.72, 3.75, 3.78, 3.82, 3.88, 3.95, 4.03, 4.25};

  #endif	//#ifndef BATTERY_OFF
/////////////////////////////////////////////////////////////////////////////////

#define CLR_ITEM1    0xA
#define CLR_ITEM2    0x8
#define CLR_ITEM3    0x5
void DspCore::initDisplay() {
  tw.begin(I2C_SDA, I2C_SCL);
  if (!begin(SCREEN_ADDRESS)) {
    Serial.println(F("SSD1327 allocation failed"));
    for (;;);
  }
  config.theme.background = TFT_BG;
  config.theme.meta       = TFT_BG;
  config.theme.metabg     = TFT_LOGO;
  config.theme.metafill   = TFT_LOGO;
  config.theme.title1     = TFT_LOGO;
  config.theme.title2     = SILVER;
  config.theme.clock      = TFT_LOGO;
  config.theme.clockbg    = DARK_GRAY;
  config.theme.rssi       = TFT_FG;
  config.theme.weather    = ORANGE;
  config.theme.ip         = SILVER;
  config.theme.vol        = SILVER;
  config.theme.bitrate    = TFT_LOGO;
  config.theme.digit      = TFT_LOGO;
  config.theme.buffer     = TFT_FG;
  config.theme.volbarout  = TFT_FG;
  config.theme.volbarin   = SILVER;
  config.theme.playlist[0] = CLR_ITEM1;
  config.theme.playlist[1] = CLR_ITEM2;
  config.theme.playlist[2] = CLR_ITEM3;
  config.theme.playlist[3] = CLR_ITEM3;
  config.theme.playlist[4] = CLR_ITEM3;
  cp437(true);
  flip();
  invert();
  setTextWrap(false);
  
  plItemHeight = playlistConf.widget.textsize*(CHARHEIGHT-1)+playlistConf.widget.textsize*4;
  plTtemsCount = round((float)height()/plItemHeight);
  if(plTtemsCount%2==0) plTtemsCount++;
  plCurrentPos = plTtemsCount/2;
  plYStart = (height() / 2 - plItemHeight / 2) - plItemHeight * (plTtemsCount - 1) / 2 + playlistConf.widget.textsize*2;
}

void DspCore::drawLogo(uint16_t top) {
  drawRGBBitmap((DSP_WIDTH - 62) / 2, top, bootlogo40, 62, 40);
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

void DspCore::clearDsp(bool black) {
  fillScreen(black?0:config.theme.background);
}

GFXglyph *pgm_read_glyph_ptr(const GFXfont *gfxFont, uint8_t c) {
  return gfxFont->glyph + c;
}

uint8_t DspCore::_charWidth(unsigned char c){
  GFXglyph *glyph = pgm_read_glyph_ptr(&DS_DIGI28pt7b, c - 0x20);
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
  setTextSize(1);
  setFont(&DS_DIGI28pt7b);
  setTextColor((network.timeinfo.tm_sec % 2 == 0) ? config.theme.clock : (CLOCKFONT_MONO?config.theme.clockbg:config.theme.background), config.theme.background);
  setCursor(_timeleft+_dotsLeft, clockTop);
  print(":");                                     /* print dots */
  setFont();

/////////////////////////////////////////////////////////////////////////////
  #ifndef BATTERY_OFF
  if(!config.isScreensaver) {
// ===================== ��������� ������� ========================================
    setTextSize(1);

  if (Charging)		// ���� ��� ������� (���������� �������� ����-��) - �������� ����������
    {
//	setTextColor(config.theme.clock, config.theme.background);				// �������
	if (network.timeinfo.tm_sec % 1 == 0)
	{
           setCursor(BatX, BatY);
	   if (g == 1) {print("\xA0\xA2\x9E\x9F");} 			// 2 ����. � �����
	   if (g == 2) {print("\xA0\x9E\x9E\xA3");} 			// 2 ����. �� �����
	   if (g == 3) {print("\x9D\x9E\xA2\xA3");} 			// 2 ����. � ������
	   if (g >= 4) {g = 0; print("\x9D\xA2\xA2\x9F");} 		// 2 ����. � ��������
           g++;
	}
    }

// ============= ��������� ��������������� ������� ==================================
    if (Volt < 2.8 )                 //�������� ����������
    {
     if (network.timeinfo.tm_sec % 1 == 0)
      {
//	 setTextColor(config.theme.clock, config.theme.background);				// �������
         setCursor(BatX, BatY);
         if (d == 1) {print("\xA0\xA2\xA2\xA3");} 			// ������ - 6 ��.
         if (d >= 2) {d = 0; print("\x9D\x9E\x9E\x9F");} 		// ������ - 0 ��.
         d++;
      }
     }

// ================= ����� �������� �������� ������ � ���������� =======================
   if (network.timeinfo.tm_min % 1 == 0)
      {
//         setTextColor(config.theme.clock, config.theme.background);
         setCursor(ProcX, ProcY); 			// ��������� ��������� ��� ������ ��������� ������
         if (e == 1) {printf("%4i%%", ChargeLevel);}  // ����� ��������� ������ ������� � ��������
         if (e >= 3) {
            #ifndef HIDE_VOLT  			// ���� ���������� ���������
//              if (_mode()!=PLAYER) {		// � ���� �� ����� "Play"
//              if (plStatus_e()!=PLAYING) {		// � ���� �� ����� "Play"
//              if (Display.displayMode!=PLAYER) {		// � ���� �� ����� "Play"
                 setCursor(ProcX, ProcY);		// ��������� ��������� ��� ������ ����������
                 printf("%.2fv", Volt);     /* } */			// ����� ����������
            #endif 				// ����� ������ ����������
                         }
         if (e >= 4) {e = 0;}
         e++;
       }
// ========================== ������ ���������� � ������ ==========================
   if (network.timeinfo.tm_sec % 6 == 0)
  {

          //  ������ ��� "reads"= ��� � ���������� ��������� � ������������
  float tempmVolt = 0;
  for(int i = 0; i < reads; i++){

         //  ��������� � ������������� ���
//     adc1_config_width(ADC_WIDTH_12Bit); 
//     adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_11db);
     adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_12);
     adc1_ulp_enable();

          //  ������ ������������� ��� �.�. ������������ �������� � ��������
//     esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 0, &adc1_chars);
     esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_12, 0, &adc1_chars);

     tempmVolt += esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_0), &adc1_chars);
                                        }

  float mVolt = (tempmVolt / reads) / 1000; 			//  �������� ������� ��������� � �������

          //  ��������� ���������� � ��������� ���������� ������� � �������
  Volt = (mVolt + 0.0028 * mVolt * mVolt + 0.0096 * mVolt - 0.051) / (ADC_R2 / (ADC_R1 + ADC_R2)) + DELTA;
  if (Volt < 1) Volt = 0;

  if ((Volt > Volt1) && (Volt1 > Volt2) && (Volt2 > Volt3) && (Volt3 > Volt4)) {
     Charging = true;						// ��������� ��������, ��� ���������� �������� ����������
     setTextColor(config.theme.clock, config.theme.background);			// �������
  									}
  else {Charging = false;}					// ��������� ��������, ��� �������� ���������� �� ����������
  Volt4 = Volt3;
  Volt3 = Volt2;
  Volt2 = Volt1;
  Volt1 = Volt;

// ============ ������������ ������� ������ � ��������� ====================================
// ����� �������, ���������������� ��������. ������ ������������� �������� ����������� ������

  int idx = 0;
  while (true) {
//================== ��������� % ����������� ������ ====================
    if (Volt < vs[idx+1]) {mVolt = Volt - vs[idx]; ChargeLevel = idx * 5 + round(mVolt /((vs[idx+1] - vs[idx]) / 5 )); break;}
    else {idx++;}
                 }
    if (ChargeLevel < 0) ChargeLevel = 0; if (ChargeLevel > 100) ChargeLevel = 100;
// ======================== ��������� ����������� ��������� ========================
  if (!Charging)		// ���� �� ��� �������
  {
//  setTextColor(config.theme.clock, config.theme.background); 
  setCursor(BatX, BatY);

  if (                         Volt >= 3.82) {print("\xA0\xA2\xA2\xA3");} 	//������ 85 % (6 ����.)
  if ((Volt < 3.82) && (Volt >= 3.72)) {print("\x9D\xA2\xA2\xA3");} 	//��70 �� 85% (5 ����.)
  if ((Volt < 3.72) && (Volt >= 3.61)) {print("\x9D\xA1\xA2\xA3");} 	//�� 55 �� 70% (4 ����.)
  if ((Volt < 3.61) && (Volt >= 3.46)) {print("\x9D\x9E\xA2\xA3");} 	//�� 40 �� 55% (3 ����.)
  if ((Volt < 3.46) && (Volt >= 3.33)) {print("\x9D\x9E\xA1\xA3");} 	//�� 25 �� 40% (2 ����.)
  if ((Volt < 3.33) && (Volt >= 3.20)) {print("\x9D\x9E\x9E\xA3");} 	//�� 10 �� 25% (1 ����.)
  if ((Volt < 3.20) && (Volt >= 2.8)) {print("\x9D\x9E\x9E\x9F");} 	//�� 0 �� 10% (0 ����.)
  }

//  if (Volt < 2.8) {setTextColor(config.theme.background, config.theme.clock);}	// (0%) ��������� ���������� �����

//Serial.printf("#BATT#: ADC: %i reads, V-batt: %.3f v, Capacity: %i\n", reads, Volt, ChargeLevel);	// ����� �������� � COM-���� ��� ��������

//  drawFastVLine(_timeleft-2, clockTop, 8, config.theme.clock); 			/*divider vert*/
//  drawFastVLine(_timeleft-2, clockTop, clockTimeHeight, config.theme.clock); 		/*divider vert*/
  }
  }
  #endif	//#ifndef BATTERY_OFF
/////////////////////////////////////////////////////////////////////////////////
}

void DspCore::_clockDate(){ }

void DspCore::_clockTime(){
  if(_oldtimeleft>0 && !CLOCKFONT_MONO) dsp.fillRect(_oldtimeleft,  clockTop-clockTimeHeight+1, _oldtimewidth, clockTimeHeight, config.theme.background);
  _timeleft = (width()/2 - _timewidth/2)+clockRightSpace;
  setTextSize(1);
  setFont(&DS_DIGI28pt7b);
  
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
}

void DspCore::printClock(uint16_t top, uint16_t rightspace, uint16_t timeheight, bool redraw){
  clockTop = top;
  clockRightSpace = rightspace;
  clockTimeHeight = timeheight;
  strftime(_timeBuf, sizeof(_timeBuf), "%H:%M", &network.timeinfo);
  if(strcmp(_oldTimeBuf, _timeBuf)!=0 || redraw){
    _getTimeBounds();
    _clockTime();
  }
  //_clockSeconds();
}

void DspCore::clearClock(){
  dsp.fillRect(_timeleft,  clockTop-clockTimeHeight, _timewidth, clockTimeHeight+1, config.theme.background);
}

void DspCore::startWrite(void) { }

void DspCore::endWrite(void) { }

void DspCore::loop(bool force) {
  display();
  delay(5);
}

void DspCore::charSize(uint8_t textsize, uint8_t& width, uint16_t& height){
  width = textsize * CHARWIDTH;
  height = textsize * CHARHEIGHT;
}

void DspCore::setTextSize(uint8_t s){
  Adafruit_GFX::setTextSize(s);
}

void DspCore::flip(){
  if(ROTATE_90){
    setRotation(config.store.flipscreen?3:1);
  }else{
    setRotation(config.store.flipscreen?2:0);
  }
}

void DspCore::invert(){
  invertDisplay(config.store.invertdisplay);
}

void DspCore::sleep(void) { oled_command(SSD1327_DISPLAYOFF); }
void DspCore::wake(void) { oled_command(SSD1327_DISPLAYON); }

void DspCore::writePixel(int16_t x, int16_t y, uint16_t color) {
  if(_clipping){
    if ((x < _cliparea.left) || (x > _cliparea.left+_cliparea.width) || (y < _cliparea.top) || (y > _cliparea.top + _cliparea.height)) return;
  }
  Adafruit_SSD1327::writePixel(x, y, color);
}

void DspCore::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  if(_clipping){
    if ((x < _cliparea.left) || (x >= _cliparea.left+_cliparea.width) || (y < _cliparea.top) || (y > _cliparea.top + _cliparea.height))  return;
  }
  Adafruit_SSD1327::writeFillRect(x, y, w, h, color);
}

void DspCore::setClipping(clipArea ca){
  _cliparea = ca;
  _clipping = true;
}

void DspCore::clearClipping(){
  _clipping = false;
}

void DspCore::setNumFont(){
  setFont(&DS_DIGI28pt7b);
  setTextSize(1);
}

#endif
