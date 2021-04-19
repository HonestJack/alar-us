#include "Admin.h"
#include <stdlib.h>

#define TEMPO_B (long)60*60*60
#define TEMPO_P (long)10
#define TEMPO_M 1
#define TEMPO_X 0

Admin::Admin(Display *d, Timer *t, Keyboard *k, Alarme *a, Simulacao_noturna *s, User *u1, User *u2)
{
  display = d;
  timer = t;
  keyboard = k;
  alarme = a;
  simulacao_noturna = s;
  usuario_1 = u1;
  usuario_2 = u2;
}

Admin::~Admin()
{
}


void Admin::start()
{
  char escolha;
  char pagina = 0;
//entrar_senha_admin(*display, *keyboard);
  do
  {
    showOptions(pagina);
    escolha = readOptionDigit(4);
    switch (escolha + 3*pagina)
    {
      case 1:
        des_habilita_user();
        break;
      case 2:
        muda_horario();
        break;
      case 3:
      case 6:
      case 9:
        pagina++;
        if(pagina > 2)
        {
          pagina = 0;
        }
        break;
      case 4:
        des_habilita_ativacao_noturna();
        break;
      case 5:
        des_habilita_simulacao();
        break;
      case 7:
        muda_tempos();
        break;
      case 8:
        break;
    }
  }while(escolha + 3*pagina != 8);

  display->clear();
}

char Admin::readOptionDigit(char num_options)
{
  char key = 0;
  char option=0;
  do
  {
    while (key==0)
    {
      key = keyboard->readRow();
      keyboard->nextRow();
    }
    option = key - '0';
    key = 0;
  } while (option < 1 || option > num_options);

  return option;
}

void Admin::showOptions(char pagina)
{
  display->clear();
  switch (pagina)
  {
  case 0:
    display->print_duas_linhas("1-Usuarios","2-Horario 3-Mais");
    break;
  case 1:
    display->print_duas_linhas("1-Ativacao Noite","2-Simul   3-Mais");
    break;
  case 2:
    display->print_duas_linhas("1-Tempos","2-Sair    3-Mais");
    break;
  
  default:
    break;
  }
  
}

void Admin::des_habilita_user()
{
  bool usuario_1_selecionado;
  display->clear();
  display->print("Usuário");
  display->goto_display(2,1);
  display->print("1-User1 2-User2");
  usuario_1_selecionado = retorna_on_off();

  display->clear();
  display->goto_display(2,1);
  display->print("1-On 2-Off");
  display->goto_display(1,1);
  display->print("Usuario ");
  if(usuario_1_selecionado)
  {
    display->print('1');
    usuario_1->habilitado = retorna_on_off();
  }
  else
  {
    display->print('2');
    usuario_2->habilitado = retorna_on_off();
  } 
  
  display->clear();
  display->print("Escolha");
  display->goto_display(2,1);
  display->print("confirmada");
  delay_ms(1000);

}

void Admin::des_habilita_ativacao_noturna()
{
  display->clear();
  display->print("Ativacao Noturna");
  display->goto_display(2,1);
  display->print("1-On 2-Off");
  alarme->ativacao_noturna_habilitada = retorna_on_off();
  display->clear();
  display->print("Escolha");
  display->goto_display(2,1);
  display->print("confirmada");
  delay_ms(1000);
}

void Admin::des_habilita_simulacao()
{
  display->clear();
  display->print("Simulacao de Pre");
  display->goto_display(2,1);
  display->print("1-On 2-Off");
  simulacao_noturna->simulacao_habilitada = retorna_on_off();
  display->clear();
  display->print("Escolha");
  display->goto_display(2,1);
  display->print("confirmada");
  delay_ms(1000);
}

bool Admin::retorna_on_off()
{
  char escolha;
  bool escolha_bool;
  do
  {
    escolha = readOptionDigit(2);
    switch (escolha)
    {
    case 1:
      escolha_bool = true;
      break;
    case 2:
      escolha_bool = false;
    default:
      break;
    }
  } while (escolha < 1 && escolha > 2);
  return escolha_bool;
}

void Admin::muda_tempos()
{
  bool opcao_1_selecionada;
  display->clear();
  display->print("Novos tempos");
  display->goto_display(2,1);
  display->print("1-Ligar 2-Detect");
  opcao_1_selecionada = retorna_on_off();

  display->clear();
  if(opcao_1_selecionada)
  {
    display->print("Tempo para ligar");
    display->goto_display(2,1);
    do
    {
      alarme->tempo_apos_ligar = le_tempo(3);
    }while(alarme->tempo_apos_ligar > 255);
  }
  else
  {
    display->print("Tempo para detect");
    display->goto_display(2,1);
    do
    {
      alarme->sensores.tempo_de_deteccao = le_tempo(3);
    }while(alarme->sensores.tempo_de_deteccao > 255);
  }
}

void Admin::muda_horario()
{
  char option;

  do
  {
    display->clear();
    display->print_duas_linhas("1-Hora 2-Min","3-Seg  4-Sair");

    option = readOptionDigit(4);
    switch (option)
    {
      case 1:
        muda_hora();
        break;
      case 2:
        muda_minuto();
        break;
      case 3:
        muda_segundo();
        break;
      case 4:
        break;
    }
  }while(option != 4);

}

void Admin::muda_hora()
{  
  display->clear();
  display->print("Nova Hora:");
  display->goto_display(2,1);

  short newHour=0;
  do
  {  
    newHour = le_tempo(2);
  } while (newHour > 24);

  long time = timer->getTime();
  long hours = time/3600;
  long min_sec = time - hours*3600;
  long min = min_sec/60;
  long sec = min_sec-min*60;

  timer->setTime(newHour*(long)3600 + min*(long)60 + sec);
}

void Admin::muda_minuto()
{  
  display->clear();
  display->print("Novo Minuto:");
  display->goto_display(2,1);

  short newMinute=0;
  do
  {  
    newMinute = le_tempo(2);
  } while (newMinute > 59);

  long time = timer->getTime();
  long hours = time/(long)3600;
  long min_sec = time - hours*(long)3600;
  long min = min_sec/(long)60;
  long sec = min_sec-min*(long)60;
  
  timer->setTime(hours*(long)3600 + newMinute*(long)60 + sec);

}

void Admin::muda_segundo()
{  
  display->clear();
  display->print("Novo Segundo:");
  display->goto_display(2,1);

  short newSeconds=0;
  do
  {  
    newSeconds = le_tempo(2);
  } while (newSeconds > 59);

  long time = timer->getTime();
  long hours = time/3600;
  long min_sec = time - hours*3600;
  long min = min_sec/60;
    
  timer->setTime(hours*(long)3600 + min*(long)60 + newSeconds);
}

unsigned short Admin::le_tempo(char digitos)
{
  unsigned short key_value = 0;
  keyboard->resetIndex();
  do
  {
    key_value += keyboard->reading(*display);
  } while(keyboard->getIndex() < digitos);

  return key_value/pot(10,5-digitos);  
}

/*
void Admin::verifica_usuario_presentes(short login)
{
  if (userdatabase->capacity()>0)
  {
    display->print_menu_deslizante(login);
  }else
  {
    display->limpa_linha(2);
    display->print("Nenhum Presente");
  }

}

void Admin::menu_report()
{ 
  char option;
  short i = 0, j;

  short presentes[MAXIMO_USUARIOS_PRESENTES];

  for (j = 0; j < QUANTIDADE_DE_USUARIOS; j++)
  {
    if(userdatabase->usuarios[j].esta_dentro)
    {
      presentes[i] = userdatabase->usuarios[j].login;
      i++;
    }
  }
  i = 0;

  display->limpa_linha(2);
  display->limpa_linha(1);
  display->print("0");
  display->print(userdatabase->capacity() + ASCII_SHIFT);
  display->print(" Pres:  3-Exit");
  
  do
  {
    verifica_usuario_presentes(presentes[i]);
    option = readOptionDigit(3);
    switch (option)
    {
      case 1:
        i++;
        if (i >= userdatabase->capacity())
        {
          i = 0;
        }     
        break;
      case 2:
        i--;
        if (i < 0)
        {
          i = userdatabase->capacity() - 1;
        }  
        break;
      case 3:
        break;
    }
  }while(option != 3);
}


void Admin::menu_troca_conta()
{
  char option;
  short i = 0;

  display->limpa_linha(2);
  display->limpa_linha(1);
  display->print("3-Exit  4-Select");

  do
  {
    display->print_menu_deslizante(userdatabase->usuarios[i].login);
    option = readOptionDigit(4);
    switch (option)
    {
      case 1:
        i++;
        if (i >= QUANTIDADE_DE_USUARIOS)
        {
          i = 0;
        }     
        break;
      case 2:
        i--;
        if (i < 0)
        {
          i = QUANTIDADE_DE_USUARIOS - 1;
        }  
        break;
      case 3:
        break;
      case 4:
        seleciona_troca(i);
        option = 3;
        break;
    }
  }while(option != 3);
  

}

void Admin::seleciona_troca(short i)
{
  char option;
  display->limpa_linha(1);
  display->print_user(userdatabase->usuarios[i].login);
  display->print(": ");
  display->print(userdatabase->usuarios[i].plano);
  display->limpa_linha(2);
  display->print("1-B 2-P 3-M 4-X");
  do
  {
    option = readOptionDigit(9);
    switch (option)
    {
      case 1:
                userdatabase->usuarios[i].tempo_restante = TEMPO_B;
                userdatabase->usuarios[i].plano = 'B';
                break;
      case 2:
                userdatabase->usuarios[i].tempo_restante = TEMPO_P;
                userdatabase->usuarios[i].plano = 'P';
                break;
      case 3:
                userdatabase->usuarios[i].tempo_restante = TEMPO_M;
                userdatabase->usuarios[i].plano = 'M';
                break;
      case 4:
                userdatabase->usuarios[i].tempo_restante = TEMPO_X;
                userdatabase->usuarios[i].plano = 'X';
                break;
      default:
                display->limpa_linha(1);
                display->print("Wrong Plan!");
                break;
    }
  } while (option < 1 || option > 4);
  eeprom->escreve(i,userdatabase->usuarios[i].plano);
  display->limpa_linha(1);
  display->print("Troca confirmada");
  display->limpa_linha(2);
  display->print(eeprom->at(i));
  delay_ms(2000);
}

*/