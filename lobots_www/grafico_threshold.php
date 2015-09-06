<?php


  $dbname = $_GET['dbname'];
  $idbatch = $_GET['idbatch'];
  InserirGraficoThresholds($dbname, $idbatch);

  function InserirGraficoThresholds($dbname, $IDBatch) {
    
    $largura_grafico = 600;
    $altura_grafico = 300;
    
    $num_ind = 0;
     
    include_once "cromossomo.php";
    include_once "dbhandler.php";
    $banco = new DBHandler($dbname);
   
    $q = "SELECT idgeracao FROM geracao
	  WHERE batch = $IDBatch ORDER BY idgeracao";
	  
	  
    $geracoes = $banco->query($q)->fetchAll();
    $i = 0;
    $num_ind = 0;
    $pontos = "";
    foreach ($geracoes as $geracao) {
      $i++;
      $pontos .= "$i";
      $IDGeracao = $geracao['idgeracao'];
      
      $ind = $banco->query("SELECT idindividuo FROM individuo WHERE geracao = '$IDGeracao'")->fetchAll();
      
      if ($i == 1) $num_ind = count($ind);
      
      foreach ($ind as $IDIndividuo) {
	$IDIndividuo = $IDIndividuo['idindividuo'];
	$cromo = getCromossomo($dbname, $IDGeracao, $IDIndividuo);
	
	$t1 = $cromo[64];
	$t2 = $cromo[65];
	
	$pontos .= " $t1 $t2";
      
      }
      
      $pontos .= "\n";
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
set ylabel 'Valor do threshold';

set yrange [0:10];

EOT;

    $gnuplotCommands .= "
      plot";
      
      
    for ($i = 0; $i < $num_ind*2; $i += 2) {
      $col1 = $i+2;
      $col2 = $col1 + 1;
      $t1 = "";
      $t2 = "";
      if ($i == 0) {
	$gnuplotCommands .= " ";
	$t1 = "LaserProx";
	$t2 = "LaserMeioLonge";
      }
      else $gnuplotCommands .= ", ";
      /*
      $gnuplotCommands .= "'$arq' using 1:$col1 w l t '$t1' lw 1 lc rgb '#FF0000', '$arq' using 1:$col2 w l t '$t2' lw 1 lc rgb 'green'";
      */
      
      $gnuplotCommands .= "'$arq' using 1:$col1 smooth csplines t '$t1' lc rgb 'red', '$arq' using 1:$col2 smooth csplines t '$t2' lc rgb 'green'";
    }
   


    
    $gnuplotCommands .= ";";

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