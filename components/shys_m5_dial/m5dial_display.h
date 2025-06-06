#pragma once
#include "M5Dial.h"
#include "esphome.h"
#include "m5dial_display_colors.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class M5DialDisplay {
            protected:
                LovyanGFX* gfx = &M5Dial.Display;

                int timeToScreenOff = 30000;
                unsigned long lastEvent = 0;
                uint16_t lastMode = -1;

                std::string fontName = "FreeMono12pt7b";
                float fontFactor = 1;

            public:
                M5DialDisplay() {
                }

                void setTimeToScreenOff(int value){
                    this->timeToScreenOff = value;
                }
                void resetLastEventTimer(){
                    lastEvent = esphome::millis();
                }

                uint16_t getHeight(){
                    return M5Dial.Display.height();
                }
                uint16_t getWidth(){
                    return M5Dial.Display.width();
                }

                LovyanGFX* getGfx() {
                    return gfx;
                }

                void setFontName(std::string name){
                    this->fontName = name;
                }

                void setFontFactor(float factor){
                    this->fontFactor = factor;
                }

                bool isDisplayOn(){
                    return M5Dial.Display.getBrightness() > 0;
                }

                void validateTimeout(){
                    if (esphome::millis() - lastEvent > timeToScreenOff ) {
                        if(M5Dial.Display.getBrightness()>0){
                            M5Dial.Display.setBrightness(0);
                            ESP_LOGI("DISPLAY", "Sleep after %d ms", timeToScreenOff);
                        }
                    } else if ( M5Dial.Display.getBrightness()<=0 ) {
                        M5Dial.Display.setBrightness(100);
                        ESP_LOGI("DISPLAY", "Display on");
                    }
                }

                void showOffline(){
                    uint16_t height = this->getHeight();
                    uint16_t width  = this->getWidth();

                    gfx->setTextColor(OfflineTextColor);
                    gfx->setTextDatum(middle_center);

                    this->setFontByName(this->fontName);

                    gfx->startWrite();                      // Secure SPI bus
                    gfx->fillRect(0, 0, width, height, OfflineBackColor);
                    
                    this->setFontsize(2);
                    gfx->drawString("OFFLINE",
                                    width / 2,
                                    height / 2);

                    gfx->endWrite();                      // Release SPI bus
                }

                void showDisconnected(){
                    uint16_t height = this->getHeight();
                    uint16_t width  = this->getWidth();

                    gfx->setTextColor(DisconnectedTextColor);
                    gfx->setTextDatum(middle_center);

                    this->setFontByName(this->fontName);

                    gfx->startWrite();                      // Secure SPI bus
                    gfx->fillRect(0, 0, width, height, DisconnectedBackColor);
                    
                    this->setFontsize(1);
                    gfx->drawString("DISCONNECTED",
                                    width / 2,
                                    height / 2);

                    gfx->endWrite();                      // Release SPI bus
                }

                void showUnknown(){
                    uint16_t height = this->getHeight();
                    uint16_t width  = this->getWidth();

                    gfx->setTextColor(UnknownTextColor);
                    gfx->setTextDatum(middle_center);

                    this->setFontByName(this->fontName);

                    gfx->startWrite();                      // Secure SPI bus
                    gfx->fillRect(0, 0, width, height, UnknownBackColor);
                    
                    this->setFontsize(2);

                    gfx->drawString("UNKNOWN",
                                    width / 2,
                                    height / 2);

                    gfx->endWrite();                      // Release SPI bus
                }

                float getDegByCoord(uint16_t x, uint16_t y){
                    float mx = M5Dial.Display.width()/2;
                    float my = M5Dial.Display.height()/2;

                    float angle = atan2(y - my, x - mx) * 180.0 / M_PI;
                    angle = 360 - fmod((angle + 360.0 - 90), 360.0);

                    return angle;
                }

                void setFontsize(float size) {
                    getGfx()->setTextSize(size * this->fontFactor);
                }

                int getRowHeight(float fontSize){
                    return (int)this->fontFactor * fontSize;
                }

                void setFontByName(const std::string& name) {
                    if (FONT_MAP.find(name) != FONT_MAP.end()) {
                        this->setFontName(name);
                    } else {
                        this->setFontName("FreeMono12pt7b");
                        ESP_LOGE("DISPLAY", "Font '%s' not found, using default font: 'FreeMono12pt7b'", name.c_str());
                    }
                    getGfx()->setFont(FONT_MAP[this->fontName]);
                }

                void drawBitmap(const uint8_t* bmp, int size, uint8_t x, uint8_t y, uint8_t width, uint8_t height){
                    M5Dial.Display.drawJpg(bmp, size, x, y, width, height, 0, 0);
                }

                void drawBitmapTransparent(const uint16_t* bmp, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint32_t transparentColor){
                    M5Dial.Display.pushImage(x, y, width, height, bmp, transparentColor);
                }

        };
    }
}
