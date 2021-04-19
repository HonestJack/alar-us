#include "Timer.h"

#include <avr/interrupt.h>

volatile long Timer::counter;
volatile short Timer::aux_counter;
volatile bool Timer::alarme;

void Timer::detecta_alarme(short ocupantes)
{
	if(!is_open() && (ocupantes > 0))
	{
		alarme = true;
	}else{
		alarme = false;
	}
}

ISR (TIMER1_OVF_vect) // Interrup��o do timer respons�vel por contar os segundos
{
	TCNT1  = 3036;
	TIFR1 |= (1 << 0); // Clear Flag
	Timer::aux_counter++;

	if(Timer::aux_counter == 4)
	{
		Timer::counter++;
		PORTB ^= (1 << 6);
		if(Timer::counter >= HORAS_EM_UM_DIA)
		{
			Timer::counter = 0;
		}
		Timer::aux_counter = 0;
	}
	if(Timer::alarme)
	{
		PORTB ^=  (1 << 5);
	}else
	{
		PORTB &= ~(1 << 5);
	}
	
}

Timer::Timer(/* args */)
{
    TCCR1A = 0;        // Timer
	TCCR1B = 3;        // Prescaler 64
	TCCR1C = 0;
	TCNT1  = 3036;     // 2^16 - 250 000u/(0.0625u*presc)
	OCR1A  = 62500;        // 
	TIFR1 |= (1 << 0); // Clear Flag
	TIMSK1 = (1 << 0); // Ativa interrupt do Timer1
	counter = (long)START_TIME;
	aux_counter = 0;
}

Timer::~Timer()
{
}

volatile long Timer::getTime()
{
  return counter;
}

void Timer::setTime(long newTime)
{
	counter = newTime;
}

bool Timer::is_open()
{
	if(counter <  HORA_DE_ABRIR ||
	   counter >= HORA_DE_FECHAR)
	{
		return false;
	}else
	{
		return true;
	}
}