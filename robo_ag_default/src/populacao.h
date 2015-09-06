#ifndef populacao_H
#define populacao_H

#include "individuo.h"

#define AG_MAX_POP 200

class TPopulacao {
public:
  TPopulacao(int pop, void (*robot_function)(void*), char *servidor, int porta, const char* servidor_web, int geracao_duracao, int verbose); //cria populacao aleatoria
  void ProximaGeracao(float chance_mut); //cruza e cria prox geracao
  void Sintese(int num_ger); //realiza sintese, substituindo o pior individuo da pop
  void Predacao();
  void CarregarServidor();
  void CarregarArquivo(const char *arquivo_entrada);
  void SalvarServidor();
  void SalvarArquivo(const char *arquivo_saida);
  void ImprimirNaTela();
  
  int tam_pop;
  TIndividuo *Individuo[AG_MAX_POP]; //populacao
  int geracao_duracao;
  
private:   
  void CalcularFitness(); //calcula o fitness de toda a populacao
  void Ordenar(); //ordena a populacao em ordem decrescente de fitness
  int batch; //identificador do batch no servidor
  char servidor_web[4096];
  bool utilizar_servidor;
  int verbose;
  
};

#endif
