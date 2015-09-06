#!/bin/bash

#--- CONFIGURAÇÕES ESPECÍFICAS DESTA VERSÃO DO ALGORITMO ---
VERSAO_AG="default" # Nome ou numero da versao - utilizado como sufixo na base de dados
NOME_MAPA="espacoso" # Mapa (ambiente) a ser carregado automaticamente no simulador
NOME_SPAWNPOINT="${NOME_MAPA}.sp" # Nome do arquivo de spawnpoint (utilizado pelo cliente)
POPULACAO="10" # Populacao da simulacao
GERACAO_DURACAO="60" # Duração de cada geração
RESPAWN_INTERVALO="${GERACAO_DURACAO}" # Intervalo para reposicionamento dos robôs no mapa (respawn)
ARQUIVO_COPIA_CROMOSSOMOS="cromossomos_${VERSAO_AG}.txt" # Nome do arquivo para cópia local dos cromossomos

#--- CONFIGURAÇÕES GERAIS ---
SERVIDOR_WEB="http://localhost/~felipe/lobots" # Servidor web com os scripts PHP
PORTA_SRV="7123" # Porta de escuta do servidor (lobots_srv)
VERBOSE_CLIENTE="baixo" # Quantidade de informações do cliente exibida na tela 

# Opcoes a serem passadas por linha de comando ao simulador
OPCOES_SERVIDOR="${NOME_MAPA} -port ${PORTA_SRV} -pop ${POPULACAO} -no_clock -clock_fps 10 -no_display" 

# Opcoes a serem passadas por linha de comando ao executavel do cliente
OPCOES_CLIENTE="-pop ${POPULACAO} -host localhost -port ${PORTA_SRV} -geracao ${GERACAO_DURACAO} -respawn ${RESPAWN_INTERVALO} -web ${SERVIDOR_WEB} -spawn spawnpoints/${NOME_SPAWNPOINT} -verbose ${VERBOSE_CLIENTE} -saida ${ARQUIVO_COPIA_CROMOSSOMOS}"

# Base de dados ---
BASE_DE_DADOS_BACKUP="lobots_ag_backup_${VERSAO_AG}" # Nome da base dados onde sera feito o backup dos dados desta versao do AE
PASTA_CLIENTE="$(pwd)" # Pasta onde se encontra o executavel do cliente
#PASTA_SERVIDOR="$(pwd)/../../lobots_srv/" # Pasta onde se encontra o executavel do simulador
PASTA_SERVIDOR="/home/felipe/Dropbox/projetos/lobots/lobots-0.7/lobots_srv"
MYSQL_HOST="localhost" # Endereço do servidor MySQL
MYSQL_LOGIN="root" # Login do servidor MySQL
MYSQL_PASSWORD="root" # Password do MySQL
MYSQL_SCRIPT="mysqlbd.sql" # Script de criaçao da base de dados


limpar() {
  echo "Deletando base"
  mysql -h $MYSQL_HOST -u $MYSQL_LOGIN -p${MYSQL_PASSWORD} -e "drop database \`lobots_ag\`"
  
  echo "Criando nova base"
  mysql -h $MYSQL_HOST -u $MYSQL_LOGIN -p${MYSQL_PASSWORD} < $MYSQL_SCRIPT
}

importar() {
  if [ -z "$1" ]
    then
      echo "Falta argumento: nome da base de dados"
      exit 1
  fi
  
  limpar
  
  echo "Copiando dados"
  
  mysqldump -h $MYSQL_HOST -u $MYSQL_LOGIN -p${MYSQL_PASSWORD} $1 | mysql -h $MYSQL_HOST -u $MYSQL_LOGIN -p${MYSQL_PASSWORD} 'lobots_ag'
}


fazerBackup() {
  mysql -h $MYSQL_HOST -u $MYSQL_LOGIN -p${MYSQL_PASSWORD} -e "drop database ${BASE_DE_DADOS_BACKUP}"
  
  mysql -h $MYSQL_HOST -u $MYSQL_LOGIN -p${MYSQL_PASSWORD} -e "create database ${BASE_DE_DADOS_BACKUP}"
  
  mysqldump -h $MYSQL_HOST -u $MYSQL_LOGIN -p${MYSQL_PASSWORD} 'lobots_ag' | mysql -h $MYSQL_HOST -u $MYSQL_LOGIN -p${MYSQL_PASSWORD} $BASE_DE_DADOS_BACKUP
}



deletarUltimaGeracao() {
  echo "Deletando ultima geração\n";
  
  mysql -h $MYSQL_HOST -u $MYSQL_LOGIN -p${MYSQL_PASSWORD} 'lobots_ag' -e "DELETE FROM individuo WHERE geracao IN (SELECT * FROM (SELECT idgeracao FROM geracao ORDER BY datahora DESC LIMIT 1) as t)"
  
  mysql -h $MYSQL_HOST -u $MYSQL_LOGIN -p${MYSQL_PASSWORD} 'lobots_ag' -e "DELETE FROM geracao ORDER BY datahora DESC LIMIT 1"
}



iniciarSimulacao() {
  while :
  do
    cd $PASTA_SERVIDOR
    START=$(date +%s)
    echo "Executando simulador: lobots_srv ${OPCOES_SERVIDOR}"
    ./lobots_srv $OPCOES_SERVIDOR &
    sleep 3s
    cd $PASTA_CLIENTE
    echo "Executando cliente: ./robo_ag ${OPCOES_CLIENTE}"
    ./robo_ag $OPCOES_CLIENTE &
    
    while [ "$(pidof lobots_srv)" ] && [ "$(pidof robo_ag)" ] 
    do
      sleep 1s
    done
    END=$(date +%s)
    DIFF=$(( $END - $START ))
    if [ $DIFF -lt 10 ] 
    then
      echo "\n\n**Processo morreu muito cedo\n";
      deletarUltimaGeracao
    fi
    killall robo_ag
    killall lobots_srv
  done

}


pararSimulacao() {
  killall robo_ag;
  killall lobots_srv;
}



case "$1" in
  "iniciar")
    iniciarSimulacao ;;
  "parar")
    pararSimulacao ;;
  "backup")
    fazerBackup ;;
  "limpar")
    limpar ;;
   "importar")
    importar $2 ;; 
  *)
    echo ""
    echo "Argumentos: [ iniciar | parar | backup | limpar | importar <base de dados> ]"
    echo ""
    echo "   iniciar  : Inicia a simulação, salvando os resultados na base de dados de"
    echo "              trabalho previamente preparada."
    echo "   parar    : Termina a simulação, interrompe os processos do cliente/servidor," 
    echo "              se necessário."
    echo "   backup   : Salva os dados na simulação atual em uma cópia com o nome "
    echo "              lobots_ag_backup_${VERSAO_AG}"
    echo "   limpar   : Deleta os dados da base atual, e cria uma nova base de dados de"
    echo "              trabalho."
    echo "   importar : Importa os dados de algum backup na base de dados de trabalho."
    echo ""
esac



