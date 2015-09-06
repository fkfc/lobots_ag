<?php
//error_reporting(-1);
//ini_set('display_errors', 'On');

$dbname = $_GET['dbname'];
$idbatch = $_GET['idbatch'];

$xi = null;
$xf = null;
$yi = null;
$yf = null;
$ref = null;
  
if (isset($_GET['xi'])) $xi = $_GET['xi'];
if (isset($_GET['xf'])) $xf = $_GET['xf'];
if (isset($_GET['yi'])) $yi = $_GET['yi'];
if (isset($_GET['yf'])) $yf = $_GET['yf'];
if (isset($_GET['ref'])) $ref = $_GET['ref'];
if (isset($_GET['desvio'])) $desvio = $_GET['desvio'];


InserirGraficoFitness($dbname, $idbatch, $xi, $xf, $yi, $yf, $ref, $desvio);


function InserirGraficoFitness($dbname, $idbatch, $xi, $xf, $yi, $yf, $ref, $desvio) {

  include_once "dbhandler.php";
  $banco = new DBHandler($dbname);
  
  $q = "SELECT idgeracao, 
	       MAX(fitness) AS melhor 
	FROM geracao
	JOIN individuo ON individuo.geracao = geracao.idgeracao
	WHERE batch = $idbatch
	GROUP BY idgeracao";

  $geracoes = $banco->query($q)->fetchAll();
 
  
  $media_movel_c = 5;
  $media_movel = array();
  
  $medias = "";
  $count_ger = 0;
  $count_media_global = 0;
  $l = 0; //contador de linhas ou gerações
  $media_global = 0;
  $vetor_fitness_individuos = array();
  foreach ($geracoes as $geracao) {
    $count_ger++;
    $idgeracao = $geracao['idgeracao'];
    $melhor_ind = $geracao['melhor']; //fitness do melhor individuo
    $query = "SELECT fitness FROM individuo WHERE geracao = $idgeracao";
    
    $individuos = $banco->query($query)->fetchAll();
    $tam_pop = count($individuos);
    $media = 0;
    $vetor_fitness_individuos_geracao = array();
    foreach ($individuos as $individuo) {
      $media = $media + $individuo['fitness'];
      array_push($vetor_fitness_individuos_geracao, $individuo['fitness']);
    }
    $media = $media / $tam_pop;
    
    if (($xi == null && $xf == null) || ($xi == null && $xf != null && $xf >= $count_ger) || ($xi != null && $xf == null && $xi <= $count_ger) || ($xi != null && $xf != null && $xi <= $count_ger && $xf >= $count_ger)) {
      $media_global += $media;
      $count_media_global++;
      $vetor_fitness_individuos = array_merge($vetor_fitness_individuos, $vetor_fitness_individuos_geracao);
      //echo count($vetor_fitness_individuos);
      //echo "<br>";
    }
        
    unset($vetor_fitness_individuos_geracao);
    
    
    
    //calcula media movel
    $ii = ($l > $media_movel_c)? $media_movel_c : $l;
    for ($i = $ii; $i > 0; $i--) {
      $media_movel[$i] = $media_movel[$i - 1];
    }
    $media_movel[0] = $media;
    $mm = array_sum($media_movel)/count($media_movel);
    
    //escreve a linha
    $medias .= "$l $media $mm $melhor_ind\n";
    
    $l++;
  }
  $media_global = $media_global/$count_media_global;
  
  echo "Media (total) fitness: $media_global<br>";
  
  
  
  //calcula o desvio padrão
  $desvio_padrao = 0;
  foreach ($vetor_fitness_individuos as $fitness_individuo) {
    $desvio_padrao += (($fitness_individuo-$media_global) * ($fitness_individuo-$media_global));
  }
  $desvio_padrao = $desvio_padrao/(count($vetor_fitness_individuos)-1);
  $desvio_padrao = sqrt($desvio_padrao);
  echo "Desvio padr&atilde;o: $desvio_padrao<br>";
  
  
  file_put_contents("pontos.bin", $medias);
  
  
  //grafico da media de fitness
  InserirGraficoBatch("pontos.bin", $xi, $xf, $yi, $yf, $ref, $desvio);
  
  
  unset($vetor_fitness_individuos);
  
  //deleta o arquivo temporario
  unlink("pontos.bin");
  
  
}



  
  function InserirGraficoBatch($arq, $xi, $xf, $yi, $yf, $ref, $desvio) {
  
    $gnuplotCommands = <<<EOT
set term png size 860,520;
set output;
set autoscale;

set multiplot;
set lmargin 10;
set rmargin 1;
set key outside right tmargin;
set grid;

set xlabel 'Geração';
set ylabel 'Fitness';


EOT;

if ($xi != null) {
  $gnuplotCommands .= "set xrange [$xi:$xf]; ";
}

if ($yi != null) {
  $gnuplotCommands .= "set yrange [$yi:$yf]; ";
}

if ($ref != null) {
$referencia = ", $ref w l lw 2 lc rgb 'red' t 'Refer&ecirc;ncia'";
}

if ($desvio != null) {
//plot '+' using 1:(3):(4) with filledcurves closed
$inferior = $ref - $desvio;
$superior = $ref + $desvio;


$plotdesvio = ", '+' using 1:($inferior):($superior) with filledcurves lc rgb '#FFbbbb' t 'Desvio padr&atilde;o da refer&ecirc;ncia'";
}

/*
$gnuplotCommands .= "
  plot '$arq' using 1:2 w l t 'Fitness m&eacute;dio' lw 2, '$arq' using 1:3 w l t 'M&eacute;dia m&oacute;vel do fitness m&eacute;dio' lw 5 lc rgb 'blue', '$arq' using 1:4 w l t 'Melhor indivíduo da geração' lw 2 lc rgb 'green';
";
*/

$gnuplotCommands .= "
  plot '$arq' using 1:3:4 w filledcurves t 'Melhor indivíduo da geração' lw 2 lc rgb '#88EE88', '$arq' using 1:3 w l t 'M&eacute;dia m&oacute;vel do fitness m&eacute;dio' lw 5 lc rgb 'blue'  $plotdesvio $referencia;
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


  }
?>
