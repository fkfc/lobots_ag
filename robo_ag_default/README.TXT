Cliente do algoritmo evolutivo
Versão 'default': Evolui genes de controle e limiares de distância.


Uso:

./robo_ag <opcoes>

  OPÇÕES:
  
  Obrigatório:
  -pop <tamanho da população> : Número de indivíduos a ser instanciado
  -spawn <nome do arquivo> : Nome do arquivo de spawnpoints, que declara as regiões válidas onde os indivíduos podem ser instanciados.
  
  
  Opcional:
  -host <servidor> : IP do lobots_srv. O valor default é 127.0.0.1
  -port <PORTA> : Especifica uma porta para conectar ao host. O valor default é 7123
  -web <endereco> : Endereço do servidor web, onde são guardados e retirados os cromossomos. Default é http://<host>/lobots . Caso seja informado 0, o servidor web não será utilizado (não será possível realizar Síntese)
  -geracao <segundos> : Duração de cada geração em segundos. O valor default é 60.
  -respawn <segundos> : Tempo para recolocar os robôs no cenário. O valor default é igual ao da duração da geração (reposiciona todos os robôs no início de cada geração). Caso este valor seja zero, os robôs nunca são reposicionados.
  -entrada <arquivo> : Carrega os cromossomos dos indivíduos a partir de um arquivo
  -saida <arquivo> : Salva uma cópia da população atual em um arquivo
  -verbose <nível> : Quantidade de texto exibido na tela. Níveis:
      tudo
      baixo
      nenhum
  
  
Exemplo

./robo_ag -pop 10 -spawn spawnpoints/espacoso.sp -host localhost -port 7123 -geracao 60 -respawn 180 -web http://localhost:8080
  Cria uma população de 10 indivíduos, com o lobots_srv rodando na máquina local, porta 7123.
  A geração dura 60 segundos, e os robôs são reposicionados de acordo com as áreas definidas pelo arquivo 'spawnpoints/espacoso.sp' a cada 3 gerações (180 segundos).
  O servidor web (para armazenar os cromossomos) está em localhost, na porta 8080.