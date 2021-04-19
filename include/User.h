#ifndef USER_H
#define USER_H

#include "Timer.h"
#include "Display.h"
#include "Keyboard.h"

class User
{
public:
    short senha;
    bool habilitado;

    User(short senha_inicial);
    ~User();

private:
    
};

#endif