#include "max7219.h"

MAX7219::MAX7219(void)
{
  pinMode(MAX_DIN, OUTPUT);
  pinMode(MAX_CS, OUTPUT);
  pinMode(MAX_CLK, OUTPUT);
}

void MAX7219::MAX7219_ShutdownStart(void)
{
  MAX7219_Write(REG_SHUTDOWN, 0);
}

void MAX7219::MAX7219_DisplayTestStart(void)
{
  MAX7219_Write(REG_DISPLAY_TEST, 1);
}

void MAX7219::Clear(void)
{

  for (int i = 0; i < 8; i++)
  {
    MAX7219_Write(i + 1, 0x00);
  }
}

void MAX7219::MAX7219_DisplayTestStop(void)
{
  MAX7219_Write(REG_DISPLAY_TEST, 0);
}

void MAX7219::MAX7219_SetBrightness(char brightness)
{
  brightness &= 0x0f;
  MAX7219_Write(REG_INTENSITY, brightness);
}

unsigned char MAX7219::MAX7219_LookupCode(char character, unsigned int dp)
{
  int i;
  unsigned int d = 0;
  if (dp)
    d = 1;
  if (character >= 35 && character <= 44)
  {
    character += 13;
    d = 1;
  }
  for (i = 0; MAX7219_Font[i].ascii; i++)
    if (character == MAX7219_Font[i].ascii)
    {
      if (d)
      {
        d = MAX7219_Font[i].segs;
        d |= (1 << 7);
        return (d);
      }
      else
      {
        return MAX7219_Font[i].segs;
      }
    }

  return 0;
}

void MAX7219::DisplayText(char *text, int justify)
{
  int decimal[16];
  char trimStr[16] = "";
  int x, y = 0;
  int s;

  s = strlen(text);
  if (s > 16)
    s = 16;
  for (x = 0; x < s; x++)
  {
    if (text[x] == '.')
    {
      decimal[y - 1] = 1;
    }
    else
    {
      trimStr[y] = text[x];
      decimal[y] = 0;
      y++;
    }
  }
  if (y > 8)
    y = 8;
  for (x = 0; x < y; x++)
  {
    if (justify == 0)
      DisplayChar((int)(y - x + 7 - y), trimStr[x], decimal[x]);
    if (justify == 1)
    {
      DisplayChar((int)(y - x + 7 - y - (8 - y)), trimStr[x], decimal[x]);
    }
  }
}

void MAX7219::MAX7219_Write(volatile byte opcode, volatile byte data)
{
  digitalWrite(MAX_CS, LOW);
  shiftOut(MAX_DIN, MAX_CLK, MSBFIRST, opcode);
  shiftOut(MAX_DIN, MAX_CLK, MSBFIRST, data);
  digitalWrite(MAX_CS, HIGH);
}

void MAX7219::DisplayChar(int digit, char value, bool dp)
{
  MAX7219_Write(digit + 1, MAX7219_LookupCode(value, dp));
}

void MAX7219::MAX7219_ShutdownStop(void)
{
  MAX7219::MAX7219_Write(REG_SHUTDOWN, 1);
}

void MAX7219::Begin()
{
  digitalWrite(MAX_CS, HIGH);
  MAX7219_Write(REG_SCAN_LIMIT, 7);
  MAX7219_Write(REG_DECODE, 0x00);
  MAX7219_ShutdownStop();
  MAX7219_DisplayTestStop();
  Clear();
  MAX7219_SetBrightness(INTENSITY_MAX);
}
