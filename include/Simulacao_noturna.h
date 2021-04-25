#ifndef SIMULACAO_NOTURNA_H
#define SIMULACAO_NOTURNA_H

#include <avr/io.h>
#include "Timer.h"

#define HORA_DE_INICIO_DA_SIMULACAO (long)19*(long)60*(long)60
#define HORA_DE_FIM_DA_SIMULACAO    (long)1*(long)60*(long)60

class Simulacao_noturna
{
private:

public:
    Simulacao_noturna(Timer *t);
    ~Simulacao_noturna();

    Timer *timer;

    bool simulacao_habilitada;
    long tempo_aux_simulacao;

    void executa_simulacao();
};

#endif