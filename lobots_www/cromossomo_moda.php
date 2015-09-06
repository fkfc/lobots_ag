<?php
  //ini_set('display_errors', 1);
  //error_reporting(-1);
  
  
  //array_individuos = array de cromossomos
  function getCromossomoModa($array_individuos) {
    $toleranciaFloatIgual = 0.4;
    
    $arrayCromo = $array_individuos;
    
    $num_genes = count($arrayCromo[0]);
    $num_individuos = count($arrayCromo);
    
    $CromossomoModa = array();
    $FrequenciaModa = array();
    
    $tm1 = 0;
    $tm2 = 0;
    
    for ($i_gene = 0; $i_gene < $num_genes; $i_gene++) { //para cada gene
      $ocorrencias_gene = array();
      foreach($arrayCromo as $Cromossomo) { //para cada individuo
	if ($i_gene == $num_genes-2) $tm1 += $Cromossomo[$i_gene];
	if ($i_gene == $num_genes-1) $tm2 += $Cromossomo[$i_gene];
	
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
      
      unset($ocorrencias_gene);
    }
    
    
    $Cromossomo[$num_genes-2] = $tm1/($num_genes*$num_individuos);
    $Cromossomo[$num_genes-1] = $tm2/($num_genes*$num_individuos);
    
    return $CromossomoModa;

  }
  
  
?>
