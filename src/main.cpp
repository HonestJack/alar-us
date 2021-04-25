#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include "Display.h"
#include "Timer.h"
#include "Keyboard.h"
#include "User.h"
#include "utils.h"
#include "Admin.h"
#include "Alarme.h"
#include "Sensores.h"
#include "Simulacao_noturna.h"

Display display;
Timer timer; 
Keyboard keyboard;

Simulacao_noturna simulacao_noturna(&timer);
Alarme alarme(&display, &timer);
User usuario_1(34589), usuario_2(12390);
Admin admin(&display, &timer, &keyboard, &alarme, &simulacao_noturna, &usuario_1, &usuario_2);



void rotina_do_relogio()
{
  // These lines are executed only on the first call to this function.
  static volatile long counter = START_TIME;
  static volatile long old_counter = counter;

  counter = timer.getTime();
  if (counter != old_counter)
  {
    old_counter = counter;
    display.print_interfacie_padrao(counter);
  }

//  timer.detecta_alarme(userdatabase.capacity());
  

}

void leitura_do_teclado(short &key_value)
{
  if(keyboard.getIndex() == DIGIT_NUMBER)
  {
    if(key_value == SENHA_ADMIN)
    {
      admin.start();
    }
    else
    {
      if ((key_value == usuario_1.senha && usuario_1.habilitado)||
          (key_value == usuario_2.senha && usuario_2.habilitado))
      {
        alarme.trocar_estado();
      }
      else
      {
        display.limpa_linha(2);
        display.print("SENHA INVALIDA");
      }
      delay_ms(1000);
      display.print_interfacie_padrao(timer.getTime());
      display.limpa_linha(2);
    }
    keyboard.resetIndex();
    key_value = 0; 
  }
}

void rotina_de_ativacao_noturna()
{
  if(timer.getTime() < HORA_DE_FIM_DA_ATIVACAO_AUTOMAICA && // Ativação noturna automática
     !alarme.esta_armado() &&
     alarme.ativacao_noturna_habilitada)
  {
    alarme.trocar_estado();
  }
}

void rotina_de_simulacao_de_presenca()
{
  if((timer.getTime() > HORA_DE_INICIO_DA_SIMULACAO || timer.getTime() < HORA_DE_FIM_DA_SIMULACAO) && 
      simulacao_noturna.simulacao_habilitada) // Simulação de presença
  {
    simulacao_noturna.executa_simulacao();
  }
  else
  {
    PORTC &= ~(1 << 4);      // Desliga luz 1
    PORTC &= ~(1 << 5);      // Desliga luz 2
    PORTD &= ~(1 << 7);      // Desliga luz 3
  }

}

int main()
{
  sei();

  short key_value = 0;

  display.print_interfacie_padrao(START_TIME);

  while(1) 
  {
    rotina_do_relogio();

    key_value += keyboard.reading(display, false);
    leitura_do_teclado(key_value);

    alarme.verificacao(); // Verifica o estado do alarme

    rotina_de_ativacao_noturna();
    rotina_de_simulacao_de_presenca();
  }
  return 0;
}