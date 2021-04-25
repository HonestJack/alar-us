#include "Teclado.h"


#include "utils.h"

#include <avr/io.h>
#include <avr/interrupt.h>

Teclado::Teclado()
{
	DDRD   |=  (1 << 0);   // PD.0 como saida - Linha 0 do teclado
  DDRD   |=  (1 << 1);   // PD.1 como saida - Linha 1 do teclado
  DDRD   |=  (1 << 2);   // PD.2 como saida - Linha 2 do teclado
  DDRD   |=  (1 << 3);   // PD.3 como saida - Linha 3 do teclado

  DDRD   &= ~(1 << 4);   // PD.4 como entrada - Coluna 0 do teclado
  DDRD   &= ~(1 << 5);   // PD.5 como entrada - Coluna 1 do teclado
  DDRD   &= ~(1 << 6);   // PD.6 como entrada - Coluna 2 do teclado

  PORTD |=  (1 << 4); // Ativa pull up
  PORTD |=  (1 << 5); // Ativa pull up
  PORTD |=  (1 << 6); // Ativa pull up

  indice = 0;
  mascara_coluna_atual = LINHA_0;
  contador_de_coluna = 0;
  PORTD &= ~(PORTD_LINES_MASK); 
  PORTD |= mascara_coluna_atual;

  coluna_atual = PIND & PORTD_COLUMN_MASK;

  for(int i=0; i<4; i++)
    coluna_anterior[i] = PORTD_COLUMN_MASK;
}

Teclado::~Teclado()
{
}

char Teclado::le_coluna()
{
  coluna_atual = PIND & PORTD_COLUMN_MASK;
  
  // checks if there is any change in the columns.
  if(coluna_anterior[contador_de_coluna] != coluna_atual)
  {    
    deboucing();
    coluna_atual = PIND & PORTD_COLUMN_MASK; // se comentar nao muda nada, sei la pq raios.
    coluna_anterior[contador_de_coluna] = coluna_atual;
    switch (coluna_atual)
    {
      case COLUNA_0: 
        switch (mascara_coluna_atual)
        {
          case LINHA_0:
            return('1');
            break;
          case LINHA_1:
            return('4');
            break;
          case LINHA_2:
            return('7');
            break;
          case LINHA_3:
            return('*');
          default:
            break;
        }
        break;

      case COLUNA_1: 
        switch (mascara_coluna_atual)
        {
          case LINHA_0:
            return('2');
            break;
          case LINHA_1:
            return('5');
            break;
          case LINHA_2:
            return('8');
            break;
          case LINHA_3:
            return('0');
            break;
          default:
            break;
        }
        break;
      case COLUNA_2:
        switch (mascara_coluna_atual)
        {
          case LINHA_0:
            return('3');
            break;
          case LINHA_1:
            return('6');
            break;
          case LINHA_2:
            return('9');
            break;
          case LINHA_3:
            return('#');
          break;
          default:
            break;
        }
        break;
      default:
        break;
    }
  }   
  return 0;
}

void Teclado::proxima_coluna()
{
  switch (mascara_coluna_atual)
  {
  case LINHA_0:
    mascara_coluna_atual = LINHA_1;
    contador_de_coluna=1;
    break;
  case LINHA_1:
    mascara_coluna_atual = LINHA_2;
    contador_de_coluna=2;
    break;
  case LINHA_2:
    mascara_coluna_atual = LINHA_3;
    contador_de_coluna=3; 
    break;
  case LINHA_3:
    mascara_coluna_atual = LINHA_0;
    contador_de_coluna=0;
    break;
  default:
    mascara_coluna_atual = LINHA_0;
    contador_de_coluna=0;
    break;
  }

  PORTD &= ~(PORTD_LINES_MASK); 
  PORTD |= mascara_coluna_atual;
}

// Função que faz o deboucing
void Teclado::deboucing()
{
	unsigned char count = 0,  
                b_antigo = coluna_atual,
                b_novo;
  
	do
  {

    delay_us(10000);

		b_novo = PORTD & PORTD_COLUMN_MASK;

		if(b_novo == b_antigo)
    {
			count++;
		}
    else
    {
			count = 0;
		}

		b_antigo = b_novo;

	}while(count < BOUNCE);
}

unsigned short Teclado::lendo(Display display, bool admin_usando)
{
  unsigned char key;
  short value = 0;
  key = le_coluna();
  proxima_coluna();

  if(key)
  {
    display.goto_display(2, indice+1);
    if(indice == 0)
    {
      display.limpa_linha(2);
      display.goto_display(2, indice+1);
    }
    if(admin_usando)
    {
      display.print(key);
    }
    else
    {
      display.print('*');
    }
    
    value = (key - ASCII_SHIFT)*pot(10,(DIGIT_NUMBER - 1 - indice));
    indice++;
  }

  return value;
}

unsigned char Teclado::retorna_indice()
{
  return indice;
}

void Teclado::reseta_indice()
{
  indice=0;
}