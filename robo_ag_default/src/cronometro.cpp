#include "cronometro.h"


TCronometro::TCronometro(double *timer_pointer) {
  this->timer_pointer = timer_pointer;
}

void TCronometro::Iniciar() {
  if (this->timer_pointer != NULL) {
    timer_start = *timer_pointer;
  } else {
    time (&start);
  }
}

double TCronometro::Consultar() {   //retorna o tempo em segundos
  if (this->timer_pointer != NULL) {
    double dif = *timer_pointer - timer_start;
    return dif;
  } else {
    time (&end);
    double dif = difftime (end,start);
    return dif;
  }
} 
