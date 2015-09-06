<?php


$dbname = $_GET['dbname'];
$idbatch = $_GET['idbatch'];
InserirGraficoSimilaridadeGeracao($dbname, $idbatch);


function InserirGraficoSimilaridadeGeracao($dbname, $IDBatch) {
    
    include_once "similaridade_geracao.php";
    include_once "dbhandler.php";
    $banco = new DBHandler($dbname);
    
    $q = "SELECT idgeracao FROM geracao
	  WHERE batch = $IDBatch ORDER BY idgeracao";
    $geracoes = $banco->query($q)->fetchAll();
    
    $pontos = "";
    
    $i = 0;
    foreach ($geracoes as $geracao) {
      $IDGeracao = $geracao['idgeracao'];
      $Sim = SimilaridadeGeracao($dbname, $IDBatch, $IDGeracao);
      $Sim = $Sim*100;
      $pontos = $pontos . "$i $Sim\n";
      $i++;
    }
    
    $arq = "pontos.bin";
    file_put_contents($arq, $pontos);
    
    
    $gnuplotCommands = <<<EOT
set term png size 860,500;
set output;
set autoscale;

set multiplot;
set lmargin 10;
set rmargin 1;
set key outside right tmargin;
set grid;

set xlabel 'Geração';
set ylabel 'Porcentagem de similaridade';


EOT;


  $gnuplotCommands .= "
    plot '$arq' using 1:2 w l t 'Similaridade Gen&oacute;tipo' lw 2 lc rgb 'green';
  ";

  //prepara plotagem   -   
  $output    = array();
  $errorCode = 0;

  //executa o comando do gnuplot
  exec('echo "' . $gnuplotCommands . '" | gnuplot | base64', $output, $errorCode);


  if ($errorCode !== 0) {
      echo 'Something went wrong. Return code: ' . $errorCode;
      exit;
  }


  $imgData = implode("\n", $output);

  //insere a imagem inline
  echo '<img src="data:image/png;base64,' . $imgData . '">';


  //deleta o arquivo temporario
  unlink("pontos.bin");
    
    
  }
  
?>