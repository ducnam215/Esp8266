#ifndef max7219_h
#define max7219_h

#include <Arduino.h> 

// MAX7219
#define REG_DECODE 0x09
#define REG_INTENSITY 0x0a
#define REG_SCAN_LIMIT 0x0b
#define REG_SHUTDOWN 0x0c
#define REG_DISPLAY_TEST 0x0f

#define INTENSITY_MIN 0x00
#define INTENSITY_MAX 0x0f

#define MAX_CLK 14
#define MAX_CS 15
#define MAX_DIN 12

static const struct
{
  char ascii;
  char segs;
} MAX7219_Font[] = {
    {'A', 0b1110111},
    {'B', 0b1111111},
    {'C', 0b1001110},
    {'D', 0b1111110},
    {'E', 0b1001111},
    {'F', 0b1000111},
    {'G', 0b1011110},
    {'H', 0b0110111},
    {'I', 0b0110000},
    {'J', 0b0111100},
    {'L', 0b0001110},
    {'N', 0b1110110},
    {'O', 0b1111110},
    {'P', 0b1100111},
    {'R', 0b0000101},
    {'S', 0b1011011},
    {'T', 0b0001111},
    {'U', 0b0111110},
    {'Y', 0b0100111},
    {'[', 0b1001110},
    {']', 0b1111000},
    {'_', 0b0001000},
    {'a', 0b1110111},
    {'b', 0b0011111},
    {'c', 0b0001101},
    {'d', 0b0111101},
    {'e', 0b1001111},
    {'f', 0b1000111},
    {'g', 0b1011110},
    {'h', 0b0010111},
    {'i', 0b0010000},
    {'j', 0b0111100},
    {'l', 0b0001110},
    {'n', 0b0010101},
    {'o', 0b1111110},
    {'p', 0b1100111},
    {'r', 0b0000101},
    {'s', 0b1011011},
    {'t', 0b0001111},
    {'u', 0b0011100},
    {'y', 0b0100111},
    {'-', 0b0000001},
    {' ', 0b0000000},
    {'0', 0b1111110},
    {'1', 0b0110000},
    {'2', 0b1101101},
    {'3', 0b1111001},
    {'4', 0b0110011},
    {'5', 0b1011011},
    {'6', 0b1011111},
    {'7', 0b1110000},
    {'8', 0b1111111},
    {'9', 0b1111011},
    {'\0', 0b0000000},
};

class MAX7219
{
private:
public:
  MAX7219(void);
  void Begin(void);
  void DisplayChar(char digit, char character, unsigned int dp);
  void MAX7219_DisplayTestStart(void);
  void DisplayChar(int digit, char value, bool dp);
  void clearDisplay();
  void MAX7219_Write(volatile byte opcode, volatile byte data);
  void MAX7219_ShutdownStop(void);
  unsigned char MAX7219_LookupCode(char character, unsigned int dp);
  void MAX7219_ShutdownStart(void);
  void Clear(void);
  void MAX7219_DisplayTestStop(void);
  void MAX7219_SetBrightness(char brightness);
  void DisplayText(char *text, int justify);
};

#endif