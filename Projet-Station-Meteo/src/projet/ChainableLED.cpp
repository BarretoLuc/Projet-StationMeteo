#include "ChainableLED.h"

// Forward declaration
float hue2rgb(float p, float q, float t);

// --------------------------------------------------------------------------------------

ChainableLED::ChainableLED(byte clk_pin, byte data_pin, byte number_of_leds) :
    _clk_pin(clk_pin), _data_pin(data_pin), _num_leds(number_of_leds)
{
    _led_state = (byte*) calloc(_num_leds*3, sizeof(byte));
}

ChainableLED::~ChainableLED()
{
    free(_led_state);
}

// --------------------------------------------------------------------------------------

void ChainableLED::init()
{
    pinMode(_clk_pin, OUTPUT);
    pinMode(_data_pin, OUTPUT);

    for (byte i=0; i<_num_leds; i++)
        setColorRGB(i, 0, 0, 0);
}

void ChainableLED::clk(void)
{
    digitalWrite(_clk_pin, LOW);
    delayMicroseconds(_CLK_PULSE_DELAY); 
    digitalWrite(_clk_pin, HIGH);
    delayMicroseconds(_CLK_PULSE_DELAY);   
}

void ChainableLED::sendByte(byte b)
{
    // Send one bit at a time, starting with the MSB
    for (byte i=0; i<8; i++)
    {
        // If MSB is 1, write one and clock it, else write 0 and clock
        if ((b & 0x80) != 0)
            digitalWrite(_data_pin, HIGH);
        else
            digitalWrite(_data_pin, LOW);
        clk();

        // Advance to the next bit to send
        b <<= 1;
    }
}
 
void ChainableLED::sendColor(byte red, byte green, byte blue)
{
    // Start by sending a byte with the format "1 1 /B7 /B6 /G7 /G6 /R7 /R6"
    byte prefix = 0b11000000;
    if ((blue & 0x80) == 0)     prefix|= 0b00100000;
    if ((blue & 0x40) == 0)     prefix|= 0b00010000; 
    if ((green & 0x80) == 0)    prefix|= 0b00001000;
    if ((green & 0x40) == 0)    prefix|= 0b00000100;
    if ((red & 0x80) == 0)      prefix|= 0b00000010;
    if ((red & 0x40) == 0)      prefix|= 0b00000001;
    sendByte(prefix);
        
    // Now must send the 3 colors
    sendByte(blue);
    sendByte(green);
    sendByte(red);
}

void ChainableLED::setColorRGB(byte led, byte red, byte green, byte blue)
{
    // Send data frame prefix (32x "0")
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x00);
    
    // Send color data for each one of the leds
    for (byte i=0; i<_num_leds; i++)
    {
        if (i == led)
        {
            _led_state[i*3 + _CL_RED] = red;
            _led_state[i*3 + _CL_GREEN] = green;
            _led_state[i*3 + _CL_BLUE] = blue;
        }
                    
        sendColor(_led_state[i*3 + _CL_RED], 
                  _led_state[i*3 + _CL_GREEN], 
                  _led_state[i*3 + _CL_BLUE]);
    }

    // Terminate data frame (32x "0")
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x00);
}

void ChainableLED::setColorHSL(byte led, float hue, float saturation, float lightness)
{
    float r, g, b;
    
    constrain(hue, 0.0, 1.0);
    constrain(saturation, 0.0, 1.0);
    constrain(lightness, 0.0, 1.0);

    if(saturation == 0.0)
    {
        r = g = b = lightness;
    }
    else
    {
        float q = lightness < 0.5 ? 
            lightness * (1.0 + saturation) : lightness + saturation - lightness * saturation;
        float p = 2.0 * lightness - q;
        r = hue2rgb(p, q, hue + 1.0/3.0);
        g = hue2rgb(p, q, hue);
        b = hue2rgb(p, q, hue - 1.0/3.0);
    }

    setColorRGB(led, (byte)(255.0*r), (byte)(255.0*g), (byte)(255.0*b));
}

// --------------------------------------------------------------------------------------

float hue2rgb(float p, float q, float t)
{
    if (t < 0.0) 
        t += 1.0;
    if(t > 1.0) 
        t -= 1.0;
    if(t < 1.0/6.0) 
        return p + (q - p) * 6.0 * t;
    if(t < 1.0/2.0) 
        return q;
    if(t < 2.0/3.0) 
        return p + (q - p) * (2.0/3.0 - t) * 6.0;

    return p;
}
