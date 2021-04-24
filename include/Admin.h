#ifndef ADMIN_H
#define ADMIN_H

#include "Timer.h"
#include "Display.h"
#include "Keyboard.h"
#include "Alarme.h"
#include "Simulacao_noturna.h"
#include "User.h"

#define SENHA_ADMIN 12345

class Admin
{
public:
  Admin(Display *d, Timer *t, Keyboard *k, Alarme *a, Simulacao_noturna *s, User *u1, User *u2);
  ~Admin();

  void start();

private:
  char le_opcao(char num_options);
  void mostra_opcao(char pagina);

  bool retorna_on_off();

  void des_habilita_user();
  void des_habilita_ativacao_noturna();
  void des_habilita_simulacao();
  void muda_tempos();

  void muda_horario();
  void muda_hora();
  void muda_minuto();
  void muda_segundo();
  unsigned short le_tempo(char digitos);

  Display *display;
	Timer *timer;
	Keyboard *keyboard;
  Alarme *alarme;
  Simulacao_noturna *simulacao_noturna;
  User *usuario_1;
  User *usuario_2;
};


#endif