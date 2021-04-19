#ifndef DISPLAY_H
#define DISPLAY_H

#include "utils.h"

#define CLEAR_DISPLAY          0x01
#define CURSOR_HOME            0x02
#define ENTRY_SET_MODE         0x06
#define DISPLAY_CONTROL        0x0C
#define CURSOR_DISPLAY_SHIFT   0x17
#define FUNCTION_SET           0x28

#define MAX_STRING_SIZE 16

#define DISPLAY_DELAY 3


class Display
{
public:
  Display(/* args */);
  ~Display();

  void print(char ch);
  void print(char string[MAX_STRING_SIZE]);
  void goto_display(unsigned char linha, unsigned char coluna);
  void print_interfacie_padrao(long int count);
  void clear();
  void limpa_linha(unsigned char linha);
  void print_tempo_restante(long tempo_restante);
  void print_duas_linhas(char string1[MAX_STRING_SIZE], char string2[MAX_STRING_SIZE]);
  void print_horario(long count);
  void print_user(short user);
  void print_menu_deslizante(short user);

private:
  void clear_function();
  void lcd_function(char function);
  void print_relogio(long count);
  void print_usuarios_presentes(short capacity);
};

#endif