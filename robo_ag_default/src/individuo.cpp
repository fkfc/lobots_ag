#include "individuo.h"
#include "trobot.h"

#include <stdio.h>
#include "ag_definicoes.h"

TIndividuo::TIndividuo(char *robotname, void (*robot_function)(void*), char* hostname, int port, int geracao_duracao, int verbose): TRobotClient(robotname, robot_function, hostname, port) {
    primeiro_turno = true;
    dist_anterior = 0;
    fitness = 0;
    fitness_temp = 0;
    Cromossomo = new TCromossomo();
    colidiu = false;
    
    fitness_historico_tam = 0;
    this->geracao_duracao = geracao_duracao;
    this->verbose = verbose;
}



float TIndividuo::LaserFrente() {
  return data.laser[SIM_NUM_LASERS/2];
  int v = 999;
  for (int i = SIM_NUM_LASERS/3; i < SIM_NUM_LASERS*2/3; i++) {
     if (data.laser[i] < v) v = data.laser[i];
  }
  return v;
}

float TIndividuo::LaserDir() {
  return data.laser[SIM_NUM_LASERS/4];
  int v = 999;
  for (int i = SIM_NUM_LASERS/4; i < SIM_NUM_LASERS/3; i++) {
     if (data.laser[i] < v) v = data.laser[i];
  }
  return v;
}

float TIndividuo::LaserEsq() {
  return data.laser[SIM_NUM_LASERS*3/4];
  int v = 999;
  for (int i = SIM_NUM_LASERS*2/3; i < SIM_NUM_LASERS*3/4; i++) {
     if (data.laser[i] < v) v = data.laser[i];
  }
  return v;
}

float TIndividuo::CalcularFitnessFinal() {
  float dist_percorrida = data.odometer - dist_anterior;
  fitness = fitness_temp + dist_percorrida*10;
  dist_anterior = data.odometer;
  
  fitness = fitness - (geracao_duracao - dist_percorrida/AG_DEF_VELOCIDADE); //perde por tempo parado
  
  colidiu = false;
  //return fitness;
  
  //desloca o vetor do fitness_historico, para dar espaço para o fitness novo no índice 0
  for (int i = fitness_historico_tam; i > 0; i--) {
    if (i < 6) fitness_historico[i] = fitness_historico[i - 1];
  }
  if (fitness_historico_tam < 6) fitness_historico_tam++;
  
  //no indice 0 fica o fitness desta geração
  fitness_historico[0] = fitness;
  
  //calcula a média
  float media_fitness = 0;
  for (int i = 0; i < fitness_historico_tam; i++) {
    media_fitness = media_fitness + fitness_historico[i];
  }
  
  fitness = media_fitness/fitness_historico_tam;
  
  return fitness;
}


//fitness atualizado a cada segundo
void TIndividuo::CalcularFitnessTurno() { 
  //if (data.speed < 0) fitness_temp = fitness_temp - 2; //perde ponto por andar de r[é
}

//verifica colisao e ajusta o fitness
void TIndividuo::CalcularFitnessColisao() { 
  tstatus_robo s = GetStatus();
  if (s.laser_frente == LASER_COLISAO || s.laser_esq == LASER_COLISAO || s.laser_dir == LASER_COLISAO) { //se tiver colidido
    if (colidiu == false) { //se ja nao tinha marcado esta colisao
      fitness_temp = fitness_temp - 10;
      colidiu = true;
    }
  } else { //se o laser nao esta em colisao
    colidiu = false; 
  }
}

tstatus_robo TIndividuo::GetStatus() {
  float esq = LaserEsq();
  float dir = LaserDir();
  float frente = LaserFrente();
  
  tstatus_robo s;
  s.laser_esq = (esq < AG_DEF_LASER_COLISAO) ? 
    LASER_COLISAO : (esq < Cromossomo->LaserProx->valor) ? 
      LASER_PROX : (esq < Cromossomo->LaserProx->valor + Cromossomo->LaserMeioLonge->valor) ?
	LASER_MEIO_LONGE: LASER_LONGE;
  
  s.laser_dir = (dir < AG_DEF_LASER_COLISAO) ? 
    LASER_COLISAO : (dir < Cromossomo->LaserProx->valor) ? 
      LASER_PROX : (dir < Cromossomo->LaserProx->valor + Cromossomo->LaserMeioLonge->valor) ?
	LASER_MEIO_LONGE: LASER_LONGE;
  
  s.laser_frente = (frente < AG_DEF_LASER_COLISAO) ? 
    LASER_COLISAO : (frente < Cromossomo->LaserProx->valor) ? 
      LASER_PROX : (frente < Cromossomo->LaserProx->valor + Cromossomo->LaserMeioLonge->valor) ?
	LASER_MEIO_LONGE: LASER_LONGE;
  
  return s;
}
