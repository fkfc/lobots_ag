#ifndef gene_H
#define gene_H

#define LASER_PROX 1
#define LASER_MEIO_LONGE 2
#define LASER_LONGE 3
#define LASER_COLISAO 4

#define ACAO_NUM_ACOES 3
#define ACAO_ESQ 1
#define ACAO_DIR 2
#define ACAO_FRENTE 3
#define ACAO_RE 4

typedef struct {
   int laser_frente;
   int laser_esq;
   int laser_dir;
} tstatus_robo;


//Gene de status, que relaciona um status do robô com uma ação
class TGeneStatus {
public:
  TGeneStatus();
  TGeneStatus(tstatus_robo s);
  void GerarAcaoAleatoria();
  void WriteToBuffer(char* buf, int *len);
  int ReadFromBuffer(const char *buf); //retorna nbytes lido
  bool CompareStatus(tstatus_robo s); //retorna true se o status for igual
  TGeneStatus *Copy();
  
  tstatus_robo status;
  int acao;
  
};



//Gene genérico, que armazena um float
class TGeneFloat {
public:
  TGeneFloat(float min, float max); //max e min indicam o range de valores válidos
  void GerarValorAleatorio();
  void WriteToBuffer(char* buf, int *len);
  int ReadFromBuffer(const char *buf); //retorna nbytes lido
  TGeneFloat *Copy();
  void CruzarCom(TGeneFloat *Outro); //Modifica este gene cruzando com Outro
  
  float valor;
  float min, max;
  
};

#endif