#ifndef TIndividuo_H
#define TIndividuo_H

#include "trobot.h"

#include "cromossomo.h"



class TIndividuo: public TRobotClient {
  public:
    TIndividuo(char *robotname, void (*robot_function)(void*), char* hostname, int port, int geracao_duracao, int verbose);
    float LaserFrente();
    float LaserEsq();
    float LaserDir();
    float CalcularFitnessFinal(); //calculo final do fitness, considerando dist percorrida
    void CalcularFitnessTurno(); //fitness atualizado a cada segundo
    void CalcularFitnessColisao(); //verifica colisao e ajusta o fitness
    tstatus_robo GetStatus();
    
    TCromossomo *Cromossomo;
    bool primeiro_turno;
    float dist_anterior;
    float fitness;
    float fitness_temp;
    
    //Guarda o historico do fitness para calcular média
    float fitness_historico[6];
    int fitness_historico_tam;
    
    int verbose;
  private:
    bool colidiu;
    int geracao_duracao;
    
    
    
};

#endif
