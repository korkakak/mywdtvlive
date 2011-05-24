<?php
require_once('../security.php');
$isLanReq = isLanRequest();
if($isLanReq === false){
       header("HTTP/1.0 403 Forbidden");
       echo "403 Forbidden";
       return;
}
$_SESSION["pw"] = null; 
if (isset($_POST['password'])) {
  $get_password = $_POST['password'];
   session_start();
}

?>
<?php
	$dbh = new PDO("sqlite:database.db");
	$sql = "select * from web_password";
	
	foreach ($dbh->query($sql) as $row)
	{
		
		if($row['user_password_pw']==$get_password){
			$_SESSION['pw'] = $row['user_password_pw'];
			echo "yes";
		}else{
			echo "no";
		}
				
	}
?>
