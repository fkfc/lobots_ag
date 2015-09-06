#ifndef cromossomo_H
#define cromossomo_H

#include "gene.h"

#define NUM_GENES 65

class TCromossomo {
public:
  TCromossomo();
  void GerarAleatorio(); //constroi cromossomo aleatorio
  TGeneStatus *GetGeneStatus(tstatus_robo s);//pega um gene que corresponde ao status s
  int GetAcao(tstatus_robo s);//pega a ação do gene que corresponde ao status s
  char* WriteToBuffer(int *len);
  void ReadFromBuffer(const char *buf); //carrega um cromossomo
  void CruzarCom(TCromossomo *C, float prob_mut); //altera estes genes cruzando com outro cromossomo
  void ImprimirNaTela(); //imprime os genes na tela
  TCromossomo *Clone();
  
  //vetor de genes de status --------------
  TGeneStatus *GeneStatus[NUM_GENES]; 
  int num_genes_status; 
  
  //Outros genes --------------------------
  TGeneFloat *LaserProx; //Valor de distancia - abaixo: LASER_PROX, acima: LASER_MEIO_LONGE
  TGeneFloat *LaserMeioLonge; //Valor de distancia (adicionado a LaserProx) - abaixo: LASER_MEIO_LONGE, acima: LASER_LONGE
  
  
private:
  
};

#endif