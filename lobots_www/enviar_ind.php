<?php
//argumentos: geracao, fitness, cromossomo

$geracao = $_GET['geracao'];
$fitness = $_GET['fitness'];
$cromossomo = $_GET['cromossomo'];

include_once "dbhandler.php"; 
$banco = new DBHandler(); $banco->conectar();
$banco->query("INSERT INTO individuo (fitness, cromossomo, geracao) VALUES ('$fitness', '$cromossomo', '$geracao')");

echo "ger = $geracao<br>";
echo "fit = $fitness<br>";
echo "cro = $cromossomo<br>";

?>