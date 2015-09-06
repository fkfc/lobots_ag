<?php 
  $idbatch = $_GET['idbatch'];
  $num_ger = $_GET['num_ger'];
  $dbname = isset($_GET['dbname'])? $_GET['dbname'] : null;
  
  include_once "dbhandler.php"; 
  $banco = new DBHandler($dbname);

  $num_ger--;
  $q = "SELECT idgeracao FROM geracao WHERE batch='$idbatch' ORDER BY datahora DESC LIMIT {$num_ger},1";
  $idgeracao = $banco->query($q)->fetchAll()[0];

  if ($idgeracao == null) { // não existe nenhuma geração neste batch!
    echo "Erro";
  } else {
    $idgeracao = $idgeracao['idgeracao'];
    
    $q = "SELECT cromossomo FROM individuo WHERE geracao='$idgeracao'";
    $individuos = $banco->query($q)->fetchAll();
    
    header("Content-type: text/plain");
    header("Content-Disposition: attachment; filename=cromossomos_geracao_{$num_ger}.txt");
  
    foreach ($individuos as $individuo) {
      echo "{$individuo['cromossomo']}\n";
    }
  
}
  
?>