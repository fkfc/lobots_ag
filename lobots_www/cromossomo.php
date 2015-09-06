<?php
  
  //Retorna array com cromossomo
  function getCromossomo($dbname, $IDGeracao, $IDIndividuo) {
    include_once "dbhandler.php";
    $banco = new DBHandler($dbname);
    $cromo = array();
    
    $cromo_str = $banco->query("SELECT cromossomo FROM individuo WHERE geracao='$IDGeracao' AND idindividuo = '$IDIndividuo'")->fetch();
    $cromo_str = $cromo_str['cromossomo'];
 
    $cromo = explode(" ", $cromo_str);
    
    $cromo_final = array();
    for ($i = 3; $i < (count($cromo)-2); $i += 4) {
      array_push($cromo_final, $cromo[$i]);
    }
    array_push($cromo_final, $cromo[count($cromo)-3]);
    array_push($cromo_final, $cromo[count($cromo)-2]);
    return $cromo_final;
  } 

?>

