<?php
//argumentos: laser_colisao, laser_prox, laser_longe, tam_pop

include_once "datahora.php";
include_once "dbhandler.php"; 
$banco = new DBHandler(); $banco->conectar();

$datahora = datahora_atual();
$laser_colisao = $_GET['laser_colisao'];
$laser_prox = $_GET['laser_prox'];
$laser_longe = $_GET['laser_longe'];
$tam_pop = $_GET['tam_pop'];


$q = "INSERT INTO batch (datahora, laser_colisao, laser_prox, laser_longe, tam_pop) VALUES ('$datahora', '$laser_colisao', '$laser_prox', '$laser_longe', '$tam_pop')";

$banco->query($q);

echo $banco->dbh->lastInsertId();

?>