/***************************************************************************
 *   Copyright (C) 2009 by Felipe Kermentz Ferraz Costa                    *
 *   felipekfcosta@gmail.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 
#include "trobot.h"
#include "individuo.h"
#include "spawnpoint.h"
#include "cronometro.h"
#include "gene.h"
#include "populacao.h"
#include "ag_definicoes.h"
#include "argumentos.h"
#include "string.h"


TPopulacao *Populacao;

TSpawnPoint *Spawn;


void robotfunc(void* arg) {
  TIndividuo *Robo = (TIndividuo*) arg;
  
  
  if (Robo->primeiro_turno == true) { //teleporta o robo para o local de spawn
    Robo->primeiro_turno = false;  
    int sx, sy, sz;
    Spawn->GetSpawnPoint(&sx, &sy, &sz);
    Robo->data.x = sx; Robo->data.y = sy; Robo->data.z = sz;
    if (Robo->verbose >= VERBOSE_TUDO) printf("Spawn: %d, %d, %d\n", sx, sy, sz);
    return;
  }
  
  
  
  
  
  if ( fabs(Robo->data.turn_angle) < 1) { //só executa ação se tiver acabado a anterior
    Robo->CalcularFitnessColisao(); //verifica colisao
    int acao = Robo->Cromossomo->GetAcao( Robo->GetStatus() );
    switch (acao) {
      case ACAO_ESQ:
	  Robo->data.speed = AG_DEF_VELOCIDADE/3;
	  Robo->data.turn_angle = 15;
	  break;
      case ACAO_DIR:
	  Robo->data.speed = AG_DEF_VELOCIDADE/3;
	  Robo->data.turn_angle = -15;
	  break;
      case ACAO_FRENTE:
	  Robo->data.speed = AG_DEF_VELOCIDADE;
	  break;	
      case ACAO_RE:
	  Robo->data.speed = - AG_DEF_VELOCIDADE;
	  break;
    }
    
  
  }
  
}
	     
int main(int argc, char *argv[])
{
	int count_sintese = 0 - AG_DEF_SINTESE_INICIO_CONTAGEM;
	int count_predacao = 0 - AG_DEF_PREDACAO_INICIO_CONTAGEM;
	char host[1024];
	int port;
	char web[1024];
	int geracao_duracao;
	int respawn_time;
	char arquivo_entrada[1024];
	char arquivo_saida[1024];
	int verbose = VERBOSE_BAIXO;
	
	
	srand (time(NULL));
	
	TArgumentos* Argumentos = new TArgumentos(argc, argv); 
	
	if ((Argumentos->isSet("pop") && Argumentos->isSet("spawn")) == false) {
	  printf ("Uso: %s -pop <numero de individuos> -spawn <arquivo de spawnpoint> <opcoes>\n", argv[0]);
	  return 0;
	}
	
	//Command line obrigatorios
	int pop = atoi(Argumentos->getVal("pop"));
	const char *arquivo = Argumentos->getVal("spawn");
	
	
	//Command line - Outras opcoes
	if (Argumentos->isSet("host")) strcpy(host, Argumentos->getVal("host"));
	    else strcpy(host, "127.0.0.1");
	if (Argumentos->isSet("port")) port = atoi(Argumentos->getVal("port"));
	    else port = 7123;  
	if (Argumentos->isSet("web")) strcpy(web, Argumentos->getVal("web"));
	    else sprintf(web, "http://%s/lobots", host);
	if (Argumentos->isSet("geracao")) geracao_duracao = atoi(Argumentos->getVal("geracao"));
	    else geracao_duracao = AG_DEF_GERACAO_TEMPO_DEFAULT;  
	if (Argumentos->isSet("respawn")) respawn_time = atoi(Argumentos->getVal("respawn"));
	    else respawn_time = geracao_duracao;
	if (Argumentos->isSet("entrada")) strcpy(arquivo_entrada, Argumentos->getVal("entrada"));
	    else arquivo_entrada[0] = 0;
	if (Argumentos->isSet("saida")) strcpy(arquivo_saida, Argumentos->getVal("saida"));
	    else arquivo_saida[0] = 0;        
	if (Argumentos->isSet("verbose")) {
	  if (strcmp(Argumentos->getVal("verbose"), "tudo") == 0) verbose = VERBOSE_TUDO;
	  if (strcmp(Argumentos->getVal("verbose"), "baixo") == 0) verbose = VERBOSE_BAIXO;
	  if (strcmp(Argumentos->getVal("verbose"), "nenhum") == 0) verbose = VERBOSE_NENHUM;	  
	} 
	
	
	//Exibe as opções na tela
	if (verbose > VERBOSE_NENHUM) {
	  printf("População: %d\n", pop);
	  printf("Arquivo de spawnpoints: [%s]\n", arquivo);
	  printf("Host: [%s]\n", host);
	  printf("Porta: %d\n", port);
	  printf("Servidor web: [%s]\n", web);
	  printf("Duração da geração: %d segundos\n", geracao_duracao);
	  printf("Respawn: a cada %d segundos\n", respawn_time);
	  printf("Arquivo de entrada: [%s]\n", arquivo_entrada);
	  printf("Arquivo de saída: [%s]\n", arquivo_saida);
	  printf("Nível verbose: %d\n", verbose);
	  printf("\n");
	}
	      
	
	//declara as areas validas onde os robos podem nascer
	Spawn = new TSpawnPoint();
	Spawn->AbrirArquivo(arquivo);
         
	
	//Cria os robôs
	Populacao = new TPopulacao(pop, robotfunc, host, port, web, geracao_duracao, verbose);
	
	//Carrega os cromossomos 
	if (strlen(arquivo_entrada) == 0) {
	  Populacao->CarregarServidor();
	} else {
	  Populacao->CarregarArquivo(arquivo_entrada);
	}
	     
	if (verbose >= VERBOSE_TUDO) Populacao->ImprimirNaTela();
	
	//Se estiver habilitado, salva uma cópia da população em arquivo
	if (strlen(arquivo_saida) > 0) {
	  Populacao->SalvarArquivo(arquivo_saida);
	}
	     
	
	// espera a simulação começar
	bool espera;
	do { 
	   espera = false;
	   for (int i = 0; i < pop; i++) espera = espera || Populacao->Individuo[i]->status == ST_STOPPED;
	} while (espera);
	
	
	//Simulação começou -----------------------
	
	//cronometro de geracao
	TCronometro *CronGeracao = new TCronometro(&(Populacao->Individuo[0]->data.elapsed_time)); 
	//cronometro de 'turno', para calcular fitness
	TCronometro *CronTurno = new TCronometro(&(Populacao->Individuo[0]->data.elapsed_time)); 
	//cronometro para respawn (evita que os robôs fiquem enroscados)	
	TCronometro *CronRespawn = new TCronometro(&(Populacao->Individuo[0]->data.elapsed_time)); 
	
	CronGeracao->Iniciar();
	CronTurno->Iniciar();
	CronRespawn->Iniciar();

	do {  //loop enquanto a simulação durar
	    
	   if (CronGeracao->Consultar() >= geracao_duracao) { //Verifica se já é hora de criar a prox geração
	     //Cria a nova população
	     Populacao->ProximaGeracao(AG_DEF_CHANCE_MUT);
	     
	    
	     
	     //verifica se é hora de fazer síntese
	     count_sintese++;
	     if (count_sintese > 0 && count_sintese % AG_DEF_SINTESE_A_CADA == 0) {
	       count_sintese = 0;
	       Populacao->Sintese(AG_DEF_SINTESE_NUM_GER);
	     }
	     
	     //verifica se é hora de fazer predacao
	     count_predacao++;
	     if (count_predacao > 0 && count_predacao % AG_DEF_PREDACAO_A_CADA == 0) {
	       count_predacao = 0;
	       Populacao->Predacao();
	     }
	     
	     //Imprime na tela
	     if (verbose >= VERBOSE_BAIXO) printf("Nova geracao\n");
	     if (verbose >= VERBOSE_TUDO) {
		Populacao->ImprimirNaTela();
	     }
	     
	     //Se estiver habilitado, salva uma cópia da população em arquivo
	     if (strlen(arquivo_saida) > 0) {
	       Populacao->SalvarArquivo(arquivo_saida);
	     }
	     
	     //Verifica se é hora de dar respawn em todos os robôs
	     if (respawn_time != 0 && CronRespawn->Consultar() >= respawn_time) { 
	       if (verbose >= VERBOSE_BAIXO) printf ("Respawn\n");
	       for (int i = 0; i < Populacao->tam_pop; i++) Populacao->Individuo[i]->primeiro_turno = true;
	       CronRespawn->Iniciar();
	     }
	     Populacao->SalvarServidor();
	     CronGeracao->Iniciar(); //reinicia o cronometro
	     
	   }
	   
	   if (CronTurno->Consultar() >= 1) { //passou um turno, atualizar fitness
	     for (int i = 0; i < pop; i++) {
	       Populacao->Individuo[i]->CalcularFitnessTurno();
	     }
	     CronTurno->Iniciar();
	   }
	   
	   
	   
	   
	   
	   
	   
	   
	   espera = false; //verifica se a simulação acabou
	   for (int i = 0; i < pop; i++) espera = espera || Populacao->Individuo[i]->status == ST_RUNNING;
	} while (espera);
	
	//simulacao acabou -------------
	if (verbose > VERBOSE_NENHUM) printf("Fim\n");
	return 0;
}

 
