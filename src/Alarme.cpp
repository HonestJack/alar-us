#include "Alarme.h"

Alarme::Alarme(Display *d, Timer *t)
{
    display = d;
    timer = t;
    sensores.timer = t;
    ativacao_noturna_habilitada = true;
    ocorreu_ativacao_noturna = false;
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
            display->print("Sensor janela 1");
            break;
        case 1:
            display->print("Sensor janela 2");
            break;
        case 2:
            display->print("Sensor porta 1");
            break;
        case 3:
            display->print("Sensor porta 2");
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
    bool disparou = false;
    alarme_armado = false;
    display->clear();
    display->print("Alarme Desarmado");
    timer->controla_pisca_led(false); // Desliga indicador de disparo
    
    for(i = 0; i < NUM_SENSORES;i++)
    {
        if(sensores.sensor[i].detectou)
        {
            sensores.sensor[i].detectou = false;
            if (timer->getTime() - sensores.sensor[i].tempo_da_deteccao > sensores.tempo_de_deteccao)
            {
                tempo_que_disparou = sensores.sensor[i].tempo_da_deteccao;
                alarme_que_disparou = i;
                disparou = true;
            }
        }
    }
    if (disparou)
    {
        PORTB &= ~(1 << 3); // Desliga o alarme sonoro
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
        if(!sensores.algum_achou()) // Só verifica os sensores caso algum não tenha detectado ainda
        {
            sensores.verifica_sensores();
        }
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