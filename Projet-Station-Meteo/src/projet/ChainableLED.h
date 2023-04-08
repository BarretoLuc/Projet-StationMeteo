#ifndef __ChainableLED_h__
#define __ChainableLED_h__

#if defined (SPARK)
  #include "application.h"
#else
  #include "Arduino.h"
#endif

#define _CL_RED             0
#define _CL_GREEN           1
#define _CL_BLUE            2
#define _CLK_PULSE_DELAY    20

class ChainableLED
{
public:
    ChainableLED(byte clk_pin, byte data_pin, byte number_of_leds);
    ~ChainableLED();
    
    void init();
    void setColorRGB(byte led, byte red, byte green, byte blue);
    void setColorHSL(byte led, float hue, float saturation, float lightness);

private:
    byte _clk_pin;
    byte _data_pin;
    byte _num_leds; 

    byte* _led_state;
    
    void clk(void);
    void sendByte(byte b);
    void sendColor(byte red, byte green, byte blue);
};

#endif
