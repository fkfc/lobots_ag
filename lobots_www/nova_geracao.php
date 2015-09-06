<?php
//argumentos: batch

include_once "datahora.php";
include_once "dbhandler.php"; 
$banco = new DBHandler(); $banco->conectar();

$datahora = datahora_atual();
$batch = $_GET['batch'];
$banco->query("INSERT INTO geracao (batch, datahora) VALUES ('$batch', '$datahora')");

echo $banco->dbh->lastInsertId();

?>