<?php  

class DBHandler {
  public $dbh;
  
  public function __construct($dbname = null) {
    if ($dbname != null) {
      $this->dbh = new PDO("mysql:host=localhost;dbname=$dbname", 'root', 'root');
    } else {
      $this->dbh = new PDO('mysql:host=localhost;dbname=lobots_ag', 'root', 'root');
    }
  }
  //------------------------------------------------------
  public function conectar($dbname) {
    return true;
  }
  
  //------------------------------------------------------
  public function query($query) {  
    $queryresults = $this->dbh->query($query);
    return $queryresults;
    
  }
  
  
  
}

?>