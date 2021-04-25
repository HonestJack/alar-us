#include "Display.h"

#ifndef TECLADO_H
#define TECLADO_H

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


class Teclado
{
public:
  Teclado();
  ~Teclado();

  unsigned short lendo(Display display, bool admin_usando);
  unsigned char retorna_indice();
  void reseta_indice();

  char le_coluna();
  void proxima_coluna();

private:
  void deboucing();

  unsigned char indice;

  unsigned char mascara_coluna_atual;
  unsigned char contador_de_coluna;

  unsigned char coluna_anterior[4];
  unsigned char coluna_atual;
};

#endif