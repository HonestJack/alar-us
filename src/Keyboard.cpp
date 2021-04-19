#include "Keyboard.h"


#include "utils.h"

#include <avr/io.h>
#include <avr/interrupt.h>

Keyboard::Keyboard(/* args */)
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

  // Init index
  m_value_index = 0;

  // Init row control variables
  m_rowMaskNow = LINHA_0;
  m_rowCount = 0;
  PORTD &= ~(PORTD_LINES_MASK); 
  PORTD |= m_rowMaskNow;

  // Init column related variables
  m_columnNow = PIND & PORTD_COLUMN_MASK;

  for(int i=0; i<4; i++)
    m_columnsOld[i] = PORTD_COLUMN_MASK;
}

Keyboard::~Keyboard()
{
}

char Keyboard::readRow()
{
  m_columnNow = PIND & PORTD_COLUMN_MASK;
  
  // checks if there is any change in the columns.
  if(m_columnsOld[m_rowCount] != m_columnNow)
  {    
    deboucing();
    m_columnNow = PIND & PORTD_COLUMN_MASK; // se comentar nao muda nada, sei la pq raios.
    m_columnsOld[m_rowCount] = m_columnNow;
    switch (m_columnNow)
    {
      case COLUNA_0: 
        switch (m_rowMaskNow)
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
        switch (m_rowMaskNow)
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
        switch (m_rowMaskNow)
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

void Keyboard::nextRow()
{
  switch (m_rowMaskNow)
  {
  case LINHA_0:
    m_rowMaskNow = LINHA_1;
    m_rowCount=1;
    break;
  case LINHA_1:
    m_rowMaskNow = LINHA_2;
    m_rowCount=2;
    break;
  case LINHA_2:
    m_rowMaskNow = LINHA_3;
    m_rowCount=3; 
    break;
  case LINHA_3:
    m_rowMaskNow = LINHA_0;
    m_rowCount=0;
    break;
  default:
    m_rowMaskNow = LINHA_0;
    m_rowCount=0;
    break;
  }

  PORTD &= ~(PORTD_LINES_MASK); 
  PORTD |= m_rowMaskNow;
}

// Função que faz o deboucing
void Keyboard::deboucing()
{
	unsigned char count = 0,  
                b_antigo = m_columnNow,
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

unsigned short Keyboard::reading(Display display)
{
  unsigned char key;
  short value = 0;
  key = readRow();
  nextRow();

  if(key)
  {
    display.goto_display(2, m_value_index+1);
    if(m_value_index == 0)
    {
      display.limpa_linha(2);
      display.goto_display(2, m_value_index+1);
    }
    display.print(key);
    value = (key - ASCII_SHIFT)*pot(10,(DIGIT_NUMBER - 1 - m_value_index));
    m_value_index++;
  }

  return value;
}

unsigned char Keyboard::getIndex()
{
  return m_value_index;
}

void Keyboard::resetIndex()
{
  m_value_index=0;
}