#include "populacao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <fstream>
#include "string_exec.h"
#include "str_replace.h"
#include "explode.h"
#include "ag_definicoes.h"

TPopulacao::TPopulacao(int pop, void (*robot_function)(void*), char *servidor, int porta, const char* servidor_web, int geracao_duracao, int verbose ) { //cria populacao aleatoria
  tam_pop = pop;
  for (int i = 0; i < tam_pop; i++) {
	  char nome[50];
	  sprintf(nome, "robo_ag_%02d", i);
	  Individuo[i] = new TIndividuo(nome, robot_function, servidor, porta, geracao_duracao, verbose);
  }
  strcpy(this->servidor_web, servidor_web);
  this->geracao_duracao = geracao_duracao;
  
  if (servidor_web[0] == '0') utilizar_servidor = false;
  else utilizar_servidor = true;
  
  this->verbose = verbose;
}


//ordena a populacao em ordem decrescente de fitness
void TPopulacao::Ordenar() {
   TIndividuo *Aux;   
   for (int i = 0; i < tam_pop - 1; i++) {
     for (int j = i + 1; j < tam_pop; j++) {
	if (Individuo[j]->fitness > Individuo[i]->fitness) { //se o da frente for maior, swap
	  Aux = Individuo[i];
	  Individuo[i] = Individuo[j];
	  Individuo[j] = Aux;
	}
     }
   }
}

//calcula o fitness de toda a populacao
void TPopulacao::CalcularFitness() {
  for (int i = 0; i < tam_pop; i++) {
    Individuo[i]->CalcularFitnessFinal();
  }
}

//cruza e cria prox geracao
void TPopulacao::ProximaGeracao(float chance_mut) {
  CalcularFitness();
  Ordenar();
  
  //calcula a média do fitness desta geração
  int fitness_media_t = 0;
  for (int i = 0; i < tam_pop; i++) {
    fitness_media_t = fitness_media_t + Individuo[i]->fitness_historico[0]; //pega o fitness desta geração, (não média)
  }
  fitness_media_t = fitness_media_t/tam_pop;
  
  //cruza todos com o melhor
  for (int i = 2; i < tam_pop; i++) {
    
    //Se este individuo teve uma performance melhor que a média, não cruzar, e sim preservar para a próx geração
    if (Individuo[i]->fitness_historico[0] < fitness_media_t) {
      Individuo[i]->Cromossomo->CruzarCom(Individuo[0]->Cromossomo, chance_mut);
    }
  }

  
  //zera o fitness de todos (fitness temporário usado para o calculo no decorrer da geração)
  for (int i = 0; i < tam_pop; i++) {
    Individuo[i]->fitness_temp = 0;
  }
}


void TPopulacao::CarregarServidor() {
  if (utilizar_servidor == true) {
    char comando[1024];
    
    //pega o batch
    sprintf(comando, "wget \"%s/get_batch.php?tam_pop=%d\" -q -O -", servidor_web, tam_pop);
    std::string str_batch = exec(comando); //executa o comando
    batch = atoi(str_batch.c_str()); //pega o (int) batch
    
    //pega a geracao
    sprintf(comando, "wget \"%s/get_ultima_geracao.php?batch=%d\" -q -O -", servidor_web, batch);
    std::string str_pop = exec(comando); //executa o comando
    
    if (str_pop.c_str()[0] == 'X') {
      if (verbose >= VERBOSE_BAIXO) printf("Criando robos aleatorios\n");
      return; //primeira geração, nada a carregar
    }
    
    if (verbose >= VERBOSE_BAIXO) printf("\nCarregando robos do servidor\n");
    
    //populacao está em str_pop, onde cada cromossomo é uma linha
    std::vector<std::string> cromossomos = explode("\n", str_pop); //explode cromossomos em vetor
    
    //coloca cada cromossomo no individuo
    for (int i = 0; i < tam_pop; i++) {
      Individuo[i]->Cromossomo->ReadFromBuffer(cromossomos[i].c_str());
    }
  } 
}



void TPopulacao::CarregarArquivo(const char *arquivo_entrada) {

    if (verbose >= VERBOSE_BAIXO) printf("Carregando arquivo de cromossomos\n");
    
    
    std::string str_pop;
    std::ifstream ifs(arquivo_entrada);
   
    str_pop.assign( (std::istreambuf_iterator<char>(ifs) ),
                (std::istreambuf_iterator<char>()    ) );
    
    
    //populacao está em str_pop, onde cada cromossomo é uma linha
    std::vector<std::string> cromossomos = explode("\n", str_pop); //explode cromossomos em vetor
    
    //coloca cada cromossomo no individuo
    for (int i = 0; i < tam_pop; i++) {
      Individuo[i]->Cromossomo->ReadFromBuffer(cromossomos[i].c_str());
    }
  
  

}

void TPopulacao::SalvarServidor() {
  if (utilizar_servidor == true) {
    char comando[10240];
    //cria a geracao
    sprintf(comando, "wget \"%s/nova_geracao.php?batch=%d\" -q -O -", servidor_web, batch);
    std::string str_geracao = exec(comando); //executa o comando
    int geracao = atoi(str_geracao.c_str()); //pega o (int) identificador da geracao
    
    
    //envia cada individuo da geracao
    int l;
    std::string cromo; 
    for (int i = 0; i < tam_pop; i++) {
      cromo = Individuo[i]->Cromossomo->WriteToBuffer(&l);
      cromo = str_replace(" ", "%20", cromo);
      sprintf(comando, "wget \"%s/enviar_ind.php?geracao=%d&fitness=%f&cromossomo=%s\" -q -O -", servidor_web, geracao, Individuo[i]->fitness, cromo.c_str());
      exec(comando); //envia o individuo para o servidor
    }
  }
}


void TPopulacao::SalvarArquivo(const char *arquivo_saida) {
    if (verbose >= VERBOSE_BAIXO) printf("Salvando cromossomos da população em arquivo\n");
  
    FILE *arq = fopen(arquivo_saida, "w");
    
    int l;
    std::string cromo; 
    for (int i = 0; i < tam_pop; i++) {
      cromo.append(Individuo[i]->Cromossomo->WriteToBuffer(&l));
      cromo.append("\n");
    }
    
    fwrite (cromo.c_str(), sizeof(char), cromo.size(), arq);
    
    fclose(arq);
  
}


void TPopulacao::ImprimirNaTela() {
    for (int i = 0; i < tam_pop; i++) {
      Individuo[i]->Cromossomo->ImprimirNaTela();
      printf("  %.2f\n", Individuo[i]->fitness);
    }
}


//realiza sintese, substituindo o pior individuo
//Sintese: pega o cromossomo "moda" das num_ger gerações anteriores
void TPopulacao::Sintese(int num_ger) {
  if (utilizar_servidor == true) {
    if (verbose >= VERBOSE_BAIXO) printf("Sintese\n");
    
    char comando[1024];
    
    //pega o cromossomo
    sprintf(comando, "wget \"%s/get_sintese.php?batch=%d&qtd=%d\" -q -O -", servidor_web, batch, num_ger);
    std::string cromo = exec(comando); //executa o comando
    
    
    if (cromo.c_str()[0] == 'X') {
      if (verbose >= VERBOSE_BAIXO) printf("Nao foi possivel pegar sintese\n");
      return; //primeira geração, nada a carregar
    }
    
    if (verbose >= VERBOSE_BAIXO) printf("Carregando cromossomo sintese\n");
    
    //coloca o cromossomo no pior individuo
    Individuo[tam_pop -1]->Cromossomo->ReadFromBuffer(cromo.c_str());
    if (verbose >= VERBOSE_TUDO) Individuo[tam_pop -1]->Cromossomo->ImprimirNaTela();
    
    printf("\n");
  }
  
}


void TPopulacao::Predacao() {
  if (verbose >= VERBOSE_BAIXO) printf("Predacao\n");
  Individuo[tam_pop -1]->Cromossomo->GerarAleatorio();
}