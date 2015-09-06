<?php
//argumentos: laser_colisao, laser_prox, tam_pop
//procura um batch com estes argumentos. Se não houver cria um batch novo. Retorna o ID

include_once "datahora.php";
include_once "dbhandler.php"; 
$banco = new DBHandler(); $banco->conectar();

$datahora = datahora_atual();
$tam_pop = $_GET['tam_pop'];

$q = "SELECT idbatch FROM batch WHERE tam_pop='$tam_pop' ORDER BY datahora DESC";

$res = $banco->query($q)->fetchAll()[0];

if ($res == null) {
  $q = "INSERT INTO batch (datahora, tam_pop) VALUES ('$datahora', '$tam_pop')";

  $banco->query($q);
  echo $banco->dbh->lastInsertId();
} else {
  echo $res['idbatch'];
}

?>