#include "Sensores.h"

Sensor::Sensor(/* args */)
{
    detectou = false;
}

Sensor::~Sensor()
{
}

Conjunto_sensores::Conjunto_sensores()
{
    DDRC   &= ~(1 << 0);   // PD.0 como entrada - Sensor da janela 1
    DDRC   &= ~(1 << 1);   // PD.1 como entrada - Sensor da janela 2
    DDRC   &= ~(1 << 2);   // PD.2 como entrada - Sensor da porta 1
    DDRC   &= ~(1 << 3);   // PD.3 como entrada - Sensor da porta 2

    DDRC |= (1 << 4);      // Luzes do simulador de presença
    DDRC |= (1 << 5);
    DDRD |= (1 << 7);

    tempo_de_deteccao = TEMPO_DE_DETECCAO_DEFAULT;
}

Conjunto_sensores::~Conjunto_sensores()
{
}

void Conjunto_sensores::verifica_sensores() // Lê os sensores
{
  short i;
  for (i = 0; i < NUM_SENSORES; i++)
  {
    if(PINC & (1 << i))
    {
        delay_ms(DELAY_DE_DETECCAO);
        if (PINC & (1 << i)) // Lê o sensor novamente após 300ms
        {
            timer->controla_pisca_led(true); // Liga pisca led
            if(!sensor[i].detectou)
            {
                sensor[i].tempo_da_deteccao = timer->getTime();
                sensor[i].detectou = true;
            }
        }
    }
  }
}

bool Conjunto_sensores::algum_disparou() // Verifica se algum sensor foi acionado e se passou o tempo necessário
{                                        // antes de acionar o alarme
    short i;
    for(i = 0; i < NUM_SENSORES; i++)
    {
        if(sensor[i].detectou &&
           timer->getTime() - sensor[i].tempo_da_deteccao > tempo_de_deteccao)
        {
            return true;
        }
    }
    return false;
}

bool Conjunto_sensores::algum_achou() // Faz a mesma coisa que algum_disparou mas sem verificar se passou o tempo
{
    short i;
    for(i = 0; i < NUM_SENSORES; i++)
    {
        if(sensor[i].detectou)
        {
            return true;
        }
    }
    return false;
}