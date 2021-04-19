
#include "Display.h"
#include <avr/io.h>
#include <avr/interrupt.h>

Display::Display(/* args */)
{
  DDRB   = 0xFF;

  PORTB &= ~(1 << 0);

  PORTB |=  (1 << 1);
  PORTB &= 0x0F;
  PORTB |= (0x20 & 0xF0);
  PORTB &= ~(1 << 1);
  delay_us(40);
  
  lcd_function(FUNCTION_SET);
  lcd_function(ENTRY_SET_MODE);
  lcd_function(DISPLAY_CONTROL);
  lcd_function(CURSOR_DISPLAY_SHIFT);
  lcd_function(CLEAR_DISPLAY);
}

Display::~Display()
{
}

void Display::print(char ch)
{
  PORTB |=  (1 << 0);
  lcd_function(ch);
  PORTB &= ~(1 << 0);
}

void Display::print(char string[])
{
  unsigned char n = 0;
  PORTB |=  (1 << 0);
  while(string[n] != '\0'){
    lcd_function(string[n]);
    n++;
  }
  PORTB &= ~(1 << 0);
}

void Display::clear_function()
{
  lcd_function(CLEAR_DISPLAY);
}

void Display::lcd_function(char function)
{

	PORTB |=  (1 << 1); // E = 1
	PORTB &= 0x0F;
	PORTB |= (function & 0xF0);
	PORTB &= ~(1 << 1); // E = 0

	PORTB |=  (1 << 1); // E = 1
	PORTB &= 0x0F;
	PORTB |= ((function & 0x0F) << 4);
	PORTB &= ~(1 << 1); // E = 0

	if(function != CLEAR_DISPLAY){
		delay_us(40);
		} else{
		delay_us(1640);
	}
}	

void Display::goto_display(unsigned char linha, unsigned char coluna)
{
  if(linha == 2)
  {
    coluna |= 0x40;
  }
  lcd_function(0x80 | (coluna - 1));
}

void Display::clear()
{
  limpa_linha(2);
  limpa_linha(1);
}

void Display::limpa_linha(unsigned char linha)
{
  goto_display(linha, 1);
   print("                ");
  goto_display(linha, 1);
}

void Display::print_horario(long count)
{
  long aux_1, aux_2;

  aux_1 = count/3600;
  aux_2 = aux_1/10;

  print(aux_2 + ASCII_SHIFT);
  print(aux_1 - 10*aux_2 + ASCII_SHIFT);
  print(':');

  count = count - 3600*aux_1;
  aux_1 = count/60;
  aux_2 = aux_1/10;

  print(aux_2 + ASCII_SHIFT);
  print(aux_1 - 10*aux_2 + ASCII_SHIFT);
  print(':');

  aux_1 = count - 60*aux_1;
  aux_2 = aux_1/10;

  print(aux_2 + ASCII_SHIFT);
  print(aux_1 - 10*aux_2 + ASCII_SHIFT);
}

void Display::print_tempo_restante(long tempo_restante)
{
  limpa_linha(2);
  goto_display(2,1);
  if (tempo_restante > 0)
  {
    print("Restam: ");
    print_horario(tempo_restante);
  }else{
    print("Seu Tempo Acabou");
  }  
}

void Display::print_relogio(long count)
{
  goto_display(1,1);
  print_horario(count);
}

void Display::print_usuarios_presentes(short capacity)
{
  goto_display(1, 14);
  print("U:");
  print((capacity) + ASCII_SHIFT);
}

void Display::print_interfacie_padrao(long count)
{
  limpa_linha(1);
  print_relogio(count);
}

void Display::print_duas_linhas(char string1[MAX_STRING_SIZE], char string2[MAX_STRING_SIZE])
{
  limpa_linha(1);
  print(string1);
  limpa_linha(2);
  print(string2);
}

void Display::print_user(short user)
{
  int i;
  short aux;
  for(i=0;i<DIGIT_NUMBER;i++)
  {
    aux = user/pot(10, DIGIT_NUMBER - i - 1);
    print(aux + ASCII_SHIFT);
    user = user - aux*pot(10, DIGIT_NUMBER - i - 1);
  }
}

void Display::print_menu_deslizante(short user)
{
  limpa_linha(2);
  print("<-2  ");
  print_user(user);
  print("   1->");
}