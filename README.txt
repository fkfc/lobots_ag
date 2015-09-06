INSTALAÇÃO -------------------------------------------------------

Instalar pacotes necessários para compilação e execução do software servidor+cliente
  sudo apt-get -y install mysql-server apache2 php5 libapache2-mod-php5 php5-mysql gnuplot build-essential freeglut3-dev wget

Copiar scripts PHP/Javascript (conteúdo da pasta lobots_www) para o diretório do apache

Verificar se todos os usuários tem permissão de escrita no diretório dos scripts PHP - Necessário para plotar gráficos, pois o script cria arquivos temporários
  chmod 777 /var/www/html/lobots

Editar o arquivo dbhandler.php em /var/www/html/..... 
  Corrigir a senha do root do mySQL
  
Verificar o script do simulador: lobots-0.8/lobots_cli/robo_ag_default/simulacao.sh
  Conferir os seguintes campos:
    - senha do root do mySQL
    - caminho da pasta do cliente (por exemplo /home/felipe/lobots-0.8/lobots_cli
    - host da interface web (exemplos, de acordo com o local onde foram copiados os arquivos PHP: "http://localhost", "http://localhost/lobots", "http://localhost/~felipe/lobots")
  
  
Criar tabela lobots_ag (BD de trabalho)
  sh simulacao.sh limpar 
  
Compilar cliente e servidor (make)





UTILIZAÇÃO -------------------------------------------------------

Para iniciar o simulador: entrar na pasta do cliente, e digitar
  sh simulacao.sh iniciar
  
Para parar a simulação: CTRL+c, e digitar: (para matar os processos)
  sh simulacao.sh parar
  
Para iniciar um novo experimento (fazer backup da base de dados atual, e iniciar uma nova "família"):
  0. Fazer backup da base de dados referente a "família" anterior: 
      Entrar na pasta do cliente antigo (por exemplo 'lobots-0.8/lobots_cli/robo_ag_default'), e executar o script:
      sh simulacao.sh backup
  1. Fazer uma cópia da pasta lobots_ag_default, e salvar com um novo sufixo, por exemplo "robo_ag_novoteste"
  2. Entrar na nova pasta robo_ag_novoteste/
  3. Editar o arquivo simulacao.sh, e alterar o nome da "família":
       VERSAO_AG="default"    ->     VERSAO_AG="novoteste"
  4. (opcional) Alterar parâmetros desejados, como população, mapa, duração da geração
  5. (opcional) Alterar o código C++ localizado em lobots_ag_novoteste/src
  6. (opcional) Limpar a base de dados de trabalho atual, executando o script na pasta do cliente:
     sh simulacao.sh limpar
     Caso a base não seja limpa, o novo AG continuará evoluindo os genes do experimento anterior.
  7. Iniciar o experimento novo: Entrar na pasta do cliente (Ex: lobots-0.8/lobots_cli/robo_ag_novoteste), e executar o script:
     sh simulacao.sh iniciar
     
     