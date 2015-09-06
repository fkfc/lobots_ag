#include "gene.h"

#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <time.h>

TGeneStatus::TGeneStatus() {
}


TGeneStatus::TGeneStatus(tstatus_robo s) {
  this->status = s;
}



void TGeneStatus::GerarAcaoAleatoria() {
  acao = rand() % ACAO_NUM_ACOES + 1;
}


void TGeneStatus::WriteToBuffer(char* buf, int *len) {
  sprintf(buf, "%d %d %d %d ", status.laser_frente, status.laser_esq, status.laser_dir, acao);
  *len = strlen(buf);
}

//retorna nbytes lido
int TGeneStatus::ReadFromBuffer(const char *buf) {
  int i = 0;
  int count = 0;
  while (count < 4) {
     if (buf[i] == ' ') count++;
     i++;
  }
  
  char tmp[24];
  memcpy(tmp, buf, i);
  sscanf(tmp, "%d %d %d %d ", &status.laser_frente, &status.laser_esq, &status.laser_dir, &acao);
  
  return i;
}

bool TGeneStatus::CompareStatus(tstatus_robo s) {
  if (status.laser_dir == s.laser_dir && status.laser_esq == s.laser_esq && status.laser_frente == s.laser_frente) return true;
  else return false;
}

TGeneStatus *TGeneStatus::Copy() {
   TGeneStatus *G = new TGeneStatus();
   G->status.laser_dir = status.laser_dir;
   G->status.laser_esq = status.laser_esq;
   G->status.laser_frente = status.laser_frente;
   G->acao = acao;
   
   return G;
}



//----------------------------------------------------------------------------------
//max e min indicam o range de valores válidos
TGeneFloat::TGeneFloat(float min, float max) {
  this->min = min;
  this->max = max;
  GerarValorAleatorio();
}  

void TGeneFloat::GerarValorAleatorio() {
  int range = (max - min)*10 + 1;
  this->valor = (rand() % (range))/10 + min;
}

void TGeneFloat::WriteToBuffer(char* buf, int *len) {
  sprintf(buf, "%f ", valor);
  *len = strlen(buf);
}

int TGeneFloat::ReadFromBuffer(const char *buf) {
  int i = 0;
  while (buf[i] != ' ') i++;
  i++;
  
  float v;
  char tmp[24];
  memcpy(tmp, buf, i);
  sscanf(tmp, "%f ", &v);
  this->valor = v;
  
  return i;
}


TGeneFloat *TGeneFloat::Copy() {
   TGeneFloat *G = new TGeneFloat(this->min, this->max);
   G->valor = this->valor;
   
   return G;
}

void TGeneFloat::CruzarCom(TGeneFloat *Outro){ //Modifica este gene cruzando com Outro
  this->valor = (this->valor + Outro->valor) / 2;
}

