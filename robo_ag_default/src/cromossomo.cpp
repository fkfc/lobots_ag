#include "cromossomo.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ag_definicoes.h"

//constroi cromossomo aleatorio
TCromossomo::TCromossomo() {
    int i = 0;
    for (int laser_esq = 1; laser_esq <= 4; laser_esq++) {
	for (int laser_dir = 1; laser_dir <= 4; laser_dir++) {
	    for (int laser_frente = 1; laser_frente <= 4; laser_frente++) {
	      tstatus_robo s;
	      s.laser_esq = laser_esq;
	      s.laser_dir = laser_dir;
	      s.laser_frente = laser_frente;
	      
	      TGeneStatus *G = new TGeneStatus(s);
	      G->GerarAcaoAleatoria();
	      
	      GeneStatus[i] = G;
	      i++;
	    }
	}  
    }
    num_genes_status = i;

    
    //laser de proximidade
    LaserProx = new TGeneFloat(AG_DEF_LASER_PROX_MIN, AG_DEF_LASER_PROX_MAX);
    LaserMeioLonge = new TGeneFloat(AG_DEF_LASER_MEIO_LONGE_MIN, AG_DEF_LASER_MEIO_LONGE_MAX);
        
    
}


void TCromossomo::GerarAleatorio() {
  for (int i = 0; i < num_genes_status; i++) {
     GeneStatus[i]->GerarAcaoAleatoria();
  }
  LaserProx->GerarValorAleatorio();
  LaserMeioLonge->GerarValorAleatorio();
  
  LaserMeioLonge->valor = 99;  
}


//pega um gene que corresponde ao status s
TGeneStatus* TCromossomo::GetGeneStatus(tstatus_robo s) {
  for (int i = 0; i < num_genes_status; i++) {
      if (GeneStatus[i]->CompareStatus(s) == true) return GeneStatus[i];
  }
}


char* TCromossomo::WriteToBuffer(int *len) {
  char *buf = (char*) malloc(sizeof(char)*7000);
  int nbytes;
  
  //armazena os genes de status
  int p = 0;
  for (int i = 0; i < num_genes_status; i++) {
     GeneStatus[i]->WriteToBuffer(&buf[p], &nbytes);
     p = p + nbytes;
  }
  
  //armazena o gene de laser_prox
  LaserProx->WriteToBuffer(&buf[p], &nbytes);
  p = p + nbytes;
  
  //armazena o gene de laser_prox
  LaserMeioLonge->WriteToBuffer(&buf[p], &nbytes);
  p = p + nbytes;
  
  buf[p] = 0;
  
  *len = p;
  return buf;
}


int TCromossomo::GetAcao(tstatus_robo s){ //pega a ação do gene que corresponde ao status s
  TGeneStatus *G = GetGeneStatus(s);
  return G->acao;
}
 
void TCromossomo::ReadFromBuffer(const char *buf) { //carrega um cromossomo
  int p = 0;
  
  //lê os genes de status
  for (int i = 0; i < num_genes_status; i++) {
    p = p + GeneStatus[i]->ReadFromBuffer(&buf[p]);
  }
  
  
  //demais genes
  p = p + LaserProx->ReadFromBuffer(&buf[p]);
  p = p + LaserMeioLonge->ReadFromBuffer(&buf[p]);
  
  
}

//altera estes genes cruzando com outro cromossomo
void TCromossomo::CruzarCom(TCromossomo *C, float prob_mut) {
  
  
  //cruza os genes de status
  for (int i = 0; i < num_genes_status; i++) {
    if (rand() % 100 < (prob_mut*100.0) ) { // mutação
	GeneStatus[i]->GerarAcaoAleatoria();  
    } else {
	if (rand() % 2 == 1) {
	  TGeneStatus *Velho = GeneStatus[i];
	  GeneStatus[i] = C->GeneStatus[i]->Copy(); 
	  delete(Velho);
	}
    }
  }
  
  
  
  //Cruza os demais genes
  if (rand() % 100 < (prob_mut*100.0)) LaserProx->GerarValorAleatorio();
  else LaserProx->CruzarCom(C->LaserProx);
  
  if (rand() % 100 < (prob_mut*100.0)) LaserMeioLonge->GerarValorAleatorio();
  else LaserMeioLonge->CruzarCom(C->LaserMeioLonge);
  
  
}


TCromossomo* TCromossomo::Clone() {
  TCromossomo *N = new TCromossomo();
  for (int i = 0; i < num_genes_status; i++) {
    delete(N->GeneStatus[i]);
    N->GeneStatus[i] = this->GeneStatus[i]->Copy();
  }
  
  delete(N->LaserProx); N->LaserProx = this->LaserProx->Copy();
  delete(N->LaserMeioLonge); N->LaserMeioLonge = this->LaserMeioLonge->Copy();
  
  return N;
}


//imprime os genes na tela
void TCromossomo::ImprimirNaTela() {
  printf("C = ");
  for (int i = 0; i < num_genes_status; i++) {
      printf("%d ", GeneStatus[i]->acao);
  }
  
  printf("%.2f ", LaserProx->valor);
  printf("%.2f ", LaserMeioLonge->valor);
}