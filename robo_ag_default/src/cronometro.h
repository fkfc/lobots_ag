#ifndef cronometro_H
#define cronometro_H

#include <time.h>
#include <stdlib.h>

class TCronometro {
public:
  TCronometro(double *timer_pointer = NULL); //if timer_pointer != null, uses the external timer pointed instead of RTC
  void Iniciar();
  double Consultar();  //retorna o tempo em segundos
  
private:
  time_t start, end; // cronometro
  double *timer_pointer;
  double timer_start;
};

#endif
