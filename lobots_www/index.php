<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>Lobots - Populações</title>
  <link rel="stylesheet" type="text/css" href="css/estilo.css">
</head>
<body>
<?php
  
  include_once "dbhandler.php";
  $banco = new DBHandler();
  $lista = $banco->query("show databases like 'lobots%'")->fetchAll();
  
  foreach ($lista as $dbname) {
    $dbname = $dbname[0];
    imprimirTabela($dbname);
  }
  

  
  function imprimirTabela($dbname) {
    
    include_once "dbhandler.php";
    $banco = new DBHandler($dbname);
    $batches = $banco->query("SELECT idbatch, batch.datahora, tam_pop, COUNT('batch') as num_ger FROM batch JOIN geracao ON geracao.batch = idbatch GROUP BY batch ORDER BY num_ger DESC")->fetchAll(); 
    echo "<h2>$dbname</h2>";
    echo "<table>";
    echo "
      <tr class='titulo'>
	<td>ID</td>
	<td>Criado em</td>
	<td>Popula&ccedil;&atilde;o</td>

	<td>Num ger</td>
      </tr>
    ";
    foreach ($batches as $batch) {
      echo "
	<tr>
	  <td><a href='ver_batch.php?dbname=$dbname&batch={$batch['idbatch']}'>{$batch['idbatch']}</a></td>
	  <td>{$batch['datahora']}</td>
	  <td>{$batch['tam_pop']}</td>
	  <td>{$batch['num_ger']}</td>
	</tr>
      ";
    }
  
  echo "</table>";
  }
?>
</body>