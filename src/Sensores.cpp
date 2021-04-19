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

void Conjunto_sensores::verifica_sensores()
{
  short i;
  for (i = 0; i < NUM_SENSORES; i++)
  {
    if(PINC & (1 << i))
    {
        PORTB |= (1 << 2); // Liga o Led de detecção
        if(!sensor[i].detectou)
        {
            sensor[i].tempo_da_deteccao = timer->getTime();
            sensor[i].detectou = true;
        }
    }
  }
}

bool Conjunto_sensores::algum_disparou()
{
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
