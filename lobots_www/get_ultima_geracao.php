<?php
//argumentos: batch
//retorna os elementos da ultima geração do batch

include_once "datahora.php";
include_once "dbhandler.php"; 
$banco = new DBHandler(); $banco->conectar();

$batch = $_GET['batch'];

$q = "SELECT idgeracao FROM geracao WHERE batch='$batch' ORDER BY datahora DESC LIMIT 1";
$idgeracao = $banco->query($q)->fetchAll()[0];

if ($idgeracao == null) { // não existe nenhuma geração neste batch!
  echo "X";
} else {
  $idgeracao = $idgeracao['idgeracao'];
  
  $q = "SELECT cromossomo FROM individuo WHERE geracao='$idgeracao'";
  $individuos = $banco->query($q)->fetchAll();
  
  foreach ($individuos as $individuo) {
    echo "{$individuo['cromossomo']}\n";
  }
  
}

?> 
