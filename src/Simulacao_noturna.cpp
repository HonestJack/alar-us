#include "Simulacao_noturna.h"

Simulacao_noturna::Simulacao_noturna(Timer *t)
{
    timer = t;
    tempo_aux_simulacao = 0;
    simulacao_habilitada = true;
}

Simulacao_noturna::~Simulacao_noturna()
{
}

void Simulacao_noturna::executa_simulacao()
{
  if(tempo_aux_simulacao == 0)
  {
    PORTC |= (1 << 4);                                                     // Liga luz 1  
    tempo_aux_simulacao = timer->getTime();
  }
  if(timer->getTime() - tempo_aux_simulacao ==  60) PORTC |=  (1 << 5);      // Liga luz 2
  if(timer->getTime() - tempo_aux_simulacao ==  90) PORTC &= ~(1 << 4);      // Desliga luz 1
  if(timer->getTime() - tempo_aux_simulacao == 150) PORTD |=  (1 << 7);      // Liga luz 3
  if(timer->getTime() - tempo_aux_simulacao == 180) PORTC &= ~(1 << 5);      // Desliga luz 2
  if(timer->getTime() - tempo_aux_simulacao == 210) PORTD &= ~(1 << 7);      // Desliga luz 3
  if(timer->getTime() - tempo_aux_simulacao == 270) tempo_aux_simulacao = 0; // Reinicia

}