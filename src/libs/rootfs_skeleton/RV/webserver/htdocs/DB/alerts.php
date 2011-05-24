<?
require_once('../security.php');
$isLanReq = isLanRequest();
if($isLanReq === false){
       header("HTTP/1.0 403 Forbidden");
       echo "403 Forbidden";
       return;
}

$dbh = new PDO("sqlite:database.db");

if (isset($_GET['id'])) {
	$get_id = $_GET['id'];
}
if (isset($_POST['cap'])) {
	$get_cap = $_POST['cap'];
	$sql = 'UPDATE web_alert SET FULL_CAP_RD="'.$get_cap.'" where rowid="1"';
	$count=$dbh->exec($sql) or die('error: '.print_r($dbh->errorInfo(),true));
	echo $count;
}
if (isset($_POST['frimware'])) {
	$get_frimware = $_POST['frimware'];
	$sql = 'UPDATE web_alert SET UPDATE_FIRMWARE_RD="'.$get_frimware.'" where rowid="1"';
	$count=$dbh->exec($sql) or die('error: '.print_r($dbh->errorInfo(),true));
	echo $count;
}

	$sql = "select * from web_alert";
	
	foreach ($dbh->query($sql) as $row)
	{
		
		if($get_id==1){
			echo '{"FULL_CAP":"'.$row['FULL_CAP_RD'].'","UPDATE_FIRMWARE":"'.$row['UPDATE_FIRMWARE_RD'].'"}';	
		}
		
					
	}


?>
