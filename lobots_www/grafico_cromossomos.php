<?php


$dbname = $_GET['dbname'];
$idbatch = $_GET['idbatch'];
InserirGraficoCromossomos($dbname, $idbatch);

  function InserirGraficoCromossomos($dbname, $IDBatch) {
    $largura_grafico = 600;
    $altura_grafico = 400;
    
    echo "<br>";
     
    include_once "cromossomo.php";
    include_once "dbhandler.php";
    include_once "cromossomo_moda.php";
    $banco = new DBHandler($dbname);
    
    $q = "SELECT idgeracao FROM geracao
	  WHERE batch = $IDBatch ORDER BY idgeracao";
	  
    $geracoes = $banco->query($q)->fetchAll();
    
    $geracoes_por_ponto = 1;
    $num_pontos = ceil(count($geracoes)/$geracoes_por_ponto);
    $largura_ponto = $largura_grafico/$num_pontos;
    
    $largura_minima = 5;
    if ($largura_ponto < $largura_minima) {
      $geracoes_por_ponto = $geracoes_por_ponto*($largura_minima/$largura_ponto);
      $largura_ponto = $largura_minima;
      $num_pontos = ceil(count($geracoes)/$geracoes_por_ponto);
    }
    
    
    $i = 0;
    $lista_geracoes = array();
    foreach ($geracoes as $geracao) {
      $IDGeracao = $geracao['idgeracao'];
      array_push($lista_geracoes, $IDGeracao);
      $i++;
      if (($i > 0) && (($i % $geracoes_por_ponto) == 0) || $i == count($geracoes)) {
	$lista_individuos = array();
	
	//pegar IDGeracao de cada individuo da lista de geracoes
	foreach ($lista_geracoes as $IDGeracao) {
	  $ind = $banco->query("SELECT idindividuo FROM individuo WHERE geracao = '$IDGeracao'")->fetchAll();
	  foreach ($ind as $IDIndividuo) {
	    $IDIndividuo = $IDIndividuo['idindividuo'];
	    
	    $cromo = getCromossomo($dbname, $IDGeracao, $IDIndividuo);
	    array_push($lista_individuos, $cromo);  
	  }
	  
	}
	
	
	$cromo_moda = getCromossomoModa($lista_individuos);
	
	unset($lista_individuos);
	unset($lista_geracoes);
	$lista_geracoes = array();
	
	//imprime o cromossomo
	$width = $largura_ponto;
	$height = $altura_grafico;
	$height_gene = $altura_grafico/count($cromo_moda)*0.7;
	
	
	echo "<div style='float: left; border: 0px solid #000; width: {$width}px; height: {$height}px; position: relative;'>";
	
	
	  $i = 0;
	  $bottom = 0;
	  foreach($cromo_moda as $gene) {
	    if ($i < count($cromo_moda)-2) {
	      switch ($gene) {
		case 1:
		  $cor = "#F00";
		  break;
		case 2:
		  $cor = "#0F0";
		  break;
		case 3:
		  $cor = "#00F";
		  break;
		case 4:
		  $cor = "#FF0";
		  break;
	      }
	      echo "
		<div style='background-color: $cor; border: 0px solid #000; width: 100%; height: {$height_gene}px; position: absolute; bottom: {$bottom}px;'>
		</div>
		";
		$bottom += $height_gene;
	    } else {
	    /*
		if ($i == count($cromo_moda)-2) $cor = "#F00";
		else $cor = "#FF0";
		$altura = ($gene/7)*$altura_grafico*0.15;
		echo "
		<div style='font-size: 0.6em; background-color: $cor; border: 0px solid #000; width: 100%; height: {$altura}px; position: absolute; bottom: {$bottom}px;'>
		
		</div>
		";
		$bottom += $altura;*/
	    }
	    $i++;
	  }
	
	echo "</div>";
	
	
	
	
	unset($cromo_moda);
      }
      
      
      
      
    }
    
    echo "<div style='clear: both;'> </div>";
  }
  
?>