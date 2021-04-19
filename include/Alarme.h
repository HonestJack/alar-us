#ifndef ALARME_H
#define ALARME_H

#include "Display.h"
#include "Timer.h"
#include "Sensores.h"

#define TEMPO_APOS_LIGAR_DEFAULT  10  // Padrão 90

#define HORA_DE_INICIO_DA_ATIVACAO_AUTOMAICA (long)0*(long)60*(long)60
#define HORA_DE_FIM_DA_ATIVACAO_AUTOMAICA    (long)6*(long)60*(long)60


class Alarme
{
private:
    bool alarme_armado;
public:
    Alarme(Display *d, Timer *t);
    ~Alarme();
    Display *display;
    Timer *timer;

    void armar_alarme();
    void printa_disparo();
    void desarmar_alarme();
    void trocar_estado();
    bool esta_armado();
    void verificacao();
    void tocar_alarme();

    bool ativacao_noturna_habilitada;
    long tempo_que_disparou;
    long tempo_apos_ligar;        // Quantos segundos após a ativação do sistema o usuário tem para sair (sensores começam a funcionar)
    long tempo_em_que_foi_ligado; // Tempo no qual o sistema foi ativado
    short alarme_que_disparou; 
    Conjunto_sensores sensores;
    
};

#endif