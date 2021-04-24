#ifndef SENSORES_H
#define SENSORES_H

#include <avr/io.h>
#include "Timer.h"

#define NUM_SENSORES 4
#define TEMPO_DE_DETECCAO_DEFAULT 10  // Padrão 30
#define DELAY_DE_DETECCAO 1000 // Tempo para não detectar alarmes falsos do sensor (em ms)

class Sensor
{
private:
    
public:
    Sensor(/* args */);
    ~Sensor();
    long tempo_da_deteccao;
    bool detectou;
};

class Conjunto_sensores
{
private:
    
public:
    Conjunto_sensores();
    ~Conjunto_sensores();

    Timer *timer;
    Sensor sensor[NUM_SENSORES];

    bool algum_disparou();
    void verifica_sensores();

    long tempo_de_deteccao;       // Quantos segundos após a detecção o alarme começa a tocar 
};

#endif