<?php


//converte ano-mes-dia hora:min:seg para dia/mes/ano hora:min:seg
//para exibir na tela
function datahora2str($datahora) {

  $timestamp = DateTime::createFromFormat('Y-m-d H:i:s', $datahora);
  $str = $timestamp->format('d/m/Y H:i:s');
  
  return $str; 
}


//converte ano-mes-dia hora:min:seg para dia/mes/ano (somente data)
function datahora2strdata($datahora) {
  $timestamp = DateTime::createFromFormat('Y-m-d H:i:s', $datahora);
  $str = $timestamp->format('d/m/Y');  
  return $str; 
}


//converte ano-mes-dia para dia/mes/ano
function data2str($data) { 
  $timestamp = DateTime::createFromFormat('Y-m-d', $data);
  if ($timestamp != null) $str = $timestamp->format('d/m/Y');
  else $str = "";
  return $str; 
}

//converte dia/mes/ano hora:min:seg para ano-mes-dia hora:min:seg
//para inserir no sql
function str2datahora($str) {
  $timestamp = DateTime::createFromFormat('d/m/Y H:i:s', $str);
  $datahora = $timestamp->format('Y-m-d H:i:s');
  
  return $datahora;
}

//converte dia/mes/ano para ano-mes-dia (sql)
function str2data($str) {
  $timestamp = DateTime::createFromFormat('d/m/Y', $str);
  if ($timestamp != null) $data = $timestamp->format('Y-m-d');
  else $data = "";
  
  return $data;
  
}

//insere um input de datahora, usar dentro de um form
function InserirDataHora($name, $datahoradefault) {

echo "
      <script language=\"javascript\" type=\"text/javascript\" src=\"cal/datetimepicker.js\"></script>

	<input type=\"text\" id=\"{$name}\" name='{$name}' maxlength=\"30\" size=\"17\" value = '{$datahoradefault}'>
	<a href=\"javascript:NewCal('{$name}','ddmmyyyy',true,24)\"><img src=\"cal/images/cal.gif\" width=\"16\" height=\"16\" border=\"0\" alt=\"Escolha uma data\"></a><br>
";
}


//pega a data/hora atual
function datahora_atual() {
  return date('Y-m-d H:i:s', time());
}

//soma $dias dias na $data, retorna nova data
function dataSomarDias($data, $dias) {
  $timestamp = DateTime::createFromFormat('Y-m-d', $data);
  if ($dias < 0) {
    $dias = sprintf("%02d", -$dias);
    $intervalo = new DateInterval("P{$dias}D");
    $timestamp->sub($intervalo);
  } else {
    $dias = sprintf("%02d", $dias);
    $intervalo = new DateInterval("P{$dias}D");
    $timestamp->add($intervalo);
  }
  
  return $timestamp->format('Y-m-d');
}


//soma um mes na datahora (datahora)
function datahoraSomarMes($datahora) {
  $timestamp = DateTime::createFromFormat('Y-m-d H:i:s', $datahora);
  
  $intervalo = new DateInterval('P1M');
  
  //soma o intervalo
  $timestamp->add($intervalo);
  
  return $timestamp->format('Y-m-d H:i:s');
}

//retorna a diferenca em dias
function calcularDiferenca($datahora1, $datahora2) {
  $data1 = explode(" ", $datahora1)[0];
  $data2 = explode(" ", $datahora2)[0];
  $timestamp1 = DateTime::createFromFormat('Y-m-d', $data1);
  $timestamp2 = DateTime::createFromFormat('Y-m-d', $data2);
  
  $interval = $timestamp1->diff($timestamp2);
  return sprintf("%d", $interval->format('%R%a'));
}


//retorna o nome do mes, dado o numero
function getNomeMes($mes) {
  $ret = "Mes";
  switch ($mes) {
    case 1: $ret = "JANEIRO"; break;
    case 2: $ret = "FEVEREIRO"; break;
    case 3: $ret = "MARÇO"; break;
    case 4: $ret = "ABRIL"; break;
    case 5: $ret = "MAIO"; break;
    case 6: $ret = "JUNHO"; break;
    case 7: $ret = "JULHO"; break;
    case 8: $ret = "AGOSTO"; break;
    case 9: $ret = "SETEMBRO"; break;
    case 10: $ret = "OUTUBRO"; break;
    case 11: $ret = "NOVEMBRO"; break;
    case 12: $ret = "DEZEMBRO"; break;
  }
  return $ret;
}

?>
