<?php
require_once('../security.php');
$isLanReq = isLanRequest();
if($isLanReq === false){
       header("HTTP/1.0 403 Forbidden");
       echo "403 Forbidden";
       return;
}

if (isset($_POST['password'])) {
  $get_password = $_POST['password'];
  
  echo $get_password;
  
  
}
$id=1;

$sql = 'UPDATE web_password SET user_password_pw="'.$get_password.'" where user_id="1"';

echo $sql;

$dbh = new PDO("sqlite:database.db");

$count=$dbh->exec($sql) or die('error: '.print_r($dbh->errorInfo(),true));

echo $count;
?>
