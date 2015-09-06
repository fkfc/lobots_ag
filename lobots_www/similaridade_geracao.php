<?php
  function SimilaridadeGeracao($dbname, $IDBatch, $IDGeracao) {
    $toleranciaFloatIgual = 0.4;
    include_once "cromossomo.php";
    include_once "dbhandler.php";
    $banco = new DBHandler($dbname);
    
    $arrayCromo = array();
    
    $num_genes = 0;
   
    //Pega os cromossomos e coloca em arrays
    $lista_individuos = $banco->query("SELECT idindividuo FROM individuo WHERE geracao = '$IDGeracao'")->fetchAll();
    foreach ($lista_individuos as $individuo) {
      $IDIndividuo = $individuo['idindividuo'];
      $Cromossomo = getCromossomo($dbname, $IDGeracao, $IDIndividuo);
      $num_genes = count($Cromossomo);
      array_push($arrayCromo, $Cromossomo);
      
    }
    
    $num_individuos = count($lista_individuos);
    
    //Calcula o "cromossomo moda"
    $CromossomoModa = array();
    $FrequenciaModa = array();
    
    for ($i_gene = 0; $i_gene < $num_genes; $i_gene++) { //para cada gene
      $ocorrencias_gene = array();
      foreach($arrayCromo as $Cromossomo) { //para cada individuo
	$gene = $Cromossomo[$i_gene];
	if (strpos($gene, '.') !== FALSE) { //se o gene contem '.', é float
	  $indices = array_keys($ocorrencias_gene); //encontra uma ocorrencia parecida
	  $encontrou = false;
	  foreach($indices as $indice) {
	    if (abs((floatval($indice) - floatval($gene))) < $toleranciaFloatIgual) {
	      $encontrou = true;
	      $ocorrencias_gene[$indice]++;
	    }
	  }
	  if ($encontrou == false) $ocorrencias_gene[$gene] = 0;
	} else { //inteiro
	  if (isset($ocorrencias_gene[$gene])) $ocorrencias_gene[$gene]++;
	  else $ocorrencias_gene[$gene] = 0;
	}
      }
      $ocorrencia_mais_frequente = 0;
      $gene_mais_frequente = "";
      $genes = array_keys($ocorrencias_gene);
      foreach($genes as $gene) {
	if ($ocorrencias_gene[$gene] >= $ocorrencia_mais_frequente) {
	  $ocorrencia_mais_frequente = $ocorrencias_gene[$gene];
	  $gene_mais_frequente = $gene;
	}
      }
      $CromossomoModa[$i_gene] = $gene;
      $FrequenciaModa[$i_gene] = $ocorrencia_mais_frequente/($num_individuos-1);
    }
    
    $Sim = 0;
    foreach($FrequenciaModa as $freq_gene) {
      $Sim = $Sim + $freq_gene;
    }
    
    $Sim = $Sim / $num_genes;
    
    return $Sim;
    
  }
  
  

?>
