#include "Alarme.h"

Alarme::Alarme(Display *d, Timer *t)
{
    display = d;
    timer = t;
    sensores.timer = t;
    ativacao_noturna_habilitada = true;
    tempo_apos_ligar  = TEMPO_APOS_LIGAR_DEFAULT;
    
}

Alarme::~Alarme()
{
}

void Alarme::armar_alarme()
{
    alarme_armado = true;
    tempo_em_que_foi_ligado = timer->getTime();
    display->clear();
    display->print("Alarme Armado");
}

void Alarme::printa_disparo()
{
    display->goto_display(2,1);
    display->print("Alarme tocou");
    delay_ms(2000);
    display->clear();
    switch (alarme_que_disparou)
    {
        case 0:
            display->print("sensor janela 1");
            break;
        case 1:
            display->print("sensor janela 2");
            break;
        case 2:
            display->print("sensor porta 1");
            break;
        case 3:
            display->print("sensor porta 2");
            break;
        
        default:
            break;
    }
    display->goto_display(2,1);
    display->print_horario(tempo_que_disparou);
    delay_ms(2000);
}

void Alarme::desarmar_alarme()
{
    short i;
    alarme_armado = false;
    display->clear();
    display->print("Alarme Desarmado");
    
    if(sensores.algum_disparou())
    {
        for(i = 0; i < NUM_SENSORES;i++)
        {
            if(sensores.sensor[i].detectou)
            {
                sensores.sensor[i].detectou = false;
                tempo_que_disparou = sensores.sensor[i].tempo_da_deteccao;
                alarme_que_disparou = i;
            }
        }
        PORTB &= ~ (1 << 2); // Desliga indicador de disparo
        PORTB &= ~ (1 << 3); // Desliga o alarme sonoro
        printa_disparo();
    }
}

bool Alarme::esta_armado()
{
    return alarme_armado;
}

void Alarme::trocar_estado()
{
    if(esta_armado())
    {
        desarmar_alarme();
    } 
    else
    {
        armar_alarme();
    }
}

void Alarme::verificacao()
{
    if(esta_armado() &&
        timer->getTime() - tempo_em_que_foi_ligado > tempo_apos_ligar)
    {
        sensores.verifica_sensores();
        if(sensores.algum_disparou())
        {
            tocar_alarme();
        }
    }
}

void Alarme::tocar_alarme()
{
    PORTB |= (1 << 3); // Toca o alarme sonoro
}