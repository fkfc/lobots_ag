<?php
//argumentos: batch, qtd (numero de geracoes para fazer a sintese)
//retorna a síntese (cromossomo moda) das QTD últimas gerações

include_once "datahora.php";
include_once "dbhandler.php"; 
include_once "cromossomo.php";
include_once "cromossomo_moda.php";

$banco = new DBHandler(); $banco->conectar();

$batch = $_GET['batch'];
$qtd = $_GET['qtd'];
$dbname = "lobots_ag";

$q = "SELECT idgeracao FROM geracao WHERE batch='$batch' ORDER BY datahora DESC LIMIT $qtd";

$geracoes = $banco->query($q)->fetchAll();

if (count($geracoes) == 0) { // não existe nenhuma geração neste batch!
  echo "X";
  die();
} else {
  $array_cromos = array();
  foreach ($geracoes as $geracao) {
    $IDGeracao = $geracao['idgeracao'];
    $q = "SELECT idindividuo FROM individuo WHERE geracao = '$IDGeracao'";
    $individuos = $banco->query($q)->fetchAll();
    
    foreach ($individuos as $IDIndividuo) {
      $IDIndividuo = $IDIndividuo['idindividuo'];
      $individuo = getCromossomo($dbname, $IDGeracao, $IDIndividuo);
      array_push($array_cromos, $individuo);
    }
  }
  
  $sintese = getCromossomoModa($array_cromos);
  
  $i = 0;
  for ($a = 1; $a <= 4; $a++) {
    for ($b = 1; $b <= 4; $b++) {
      for ($c = 1; $c <= 4; $c++) {
	  if ($i > 0) echo " ";
	  echo "$c $a $b {$sintese[$i]}";
	  $i++;
      }
    }
  }
  
  $j = $i + 1;
  echo " {$sintese[$i]} {$sintese[$j]} \n";
  
  
  
}

?> 
