<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
</head>
<?php
  
  $idbatch = $_GET['batch'];
  $dbname = isset($_GET['dbname'])? $_GET['dbname'] : null;
  
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
  
  
  echo "<h2>{$_GET['dbname']}</h2>
	<br>
	<script src='js/ajax.js'></script>
	
	<form method='GET' style='margin: 10px;' action='download_geracao.php'>
	  Geração: <input type='text' size='5' name='num_ger' value='1'>
	  <input type='hidden' name='dbname' value='{$dbname}'>
	  <input type='hidden' name='idbatch' value='{$idbatch}'>
	  <input type='submit' value='Baixar cromossomos'>
	</form>
	
	
	<button onclick=\"carregarPaginaAjax('grafico_fitness.php?dbname=$dbname&idbatch=$idbatch&xi=$xi&xf=$xf&yi=$yi&yf=$yf&ref=$ref&desvio=$desvio', 'div_fitness', true);\">
	  Mostrar gr&aacute;fico de fitness
	</button>
	
	<div id='div_fitness'></div>
	
	
	<br><br>
	
	
	<button onclick=\"carregarPaginaAjax('grafico_similaridade.php?dbname=$dbname&idbatch=$idbatch', 'div_sim', true);\">
	  Mostrar gr&aacute;fico de similaridade
	</button>
	
	<div id='div_sim'></div>
	
	<br><br>
	
	<button onclick=\"carregarPaginaAjax('grafico_cromossomos.php?dbname=$dbname&idbatch=$idbatch', 'div_cromo', true);\">
	  Mostrar mapa de cromossomos
	</button>
	
	<div id='div_cromo' style='margin-left: 100px;'></div>
	
	
	<br><br>
	
	<button onclick=\"carregarPaginaAjax('grafico_threshold.php?dbname=$dbname&idbatch=$idbatch', 'div_threshold', true);\">
	  Mostrar n&iacute;veis de threshold
	</button>
	
	<div id='div_threshold'></div>
  ";
  

 
  
 
  
?>

