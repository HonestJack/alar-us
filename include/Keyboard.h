#include "Display.h"

#ifndef KEYBOARD_H
#define KEYBOARD_H

#define PORTD_COLUMN_MASK 0x70 // 0b0111 0000 - Pega apenas as colunas do teclado

#define COLUNA_0 0x60 // 0b0110 0000
#define COLUNA_1 0x50 // 0b0101 0000
#define COLUNA_2 0x30 // 0b0011 0000

#define COLUNA_0_MASK 0x10 // 0b0001 0000
#define COLUNA_1_MASK 0x20 // 0b0010 0000
#define COLUNA_2_MASK 0x40 // 0b0100 0000

#define PORTD_LINES_MASK  0x0F // 0b00001111 - Pega apenas as linhas do teclado
#define LINHA_3 0x07 // 0b00000111
#define LINHA_2 0x0B // 0b00001011
#define LINHA_1 0x0D // 0b00001101
#define LINHA_0 0x0E // 0b00001110

#define BOUNCE 4


class Keyboard
{
public:
  Keyboard(/* args */);
  ~Keyboard();

  unsigned short reading(Display display, bool admin_usando);
  unsigned char getIndex();
  void resetIndex();

  char readRow();
  void nextRow();

private:
  void deboucing();

  unsigned char m_value_index;

  unsigned char m_rowMaskNow;
  unsigned char m_rowCount;

  unsigned char m_columnsOld[4];
  unsigned char m_columnNow;
};

#endif