<?php 
require_once('security.php');
$isLanReq = isLanRequest();
if($isLanReq === false){
	header("HTTP/1.0 403 Forbidden");
	echo "403 Forbidden";
	return;
}

if (!isset($_SESSION)) {
  session_start();
  
  if($_SESSION['reg']==''){
		header("Location: index.php" );
	}
	$get_language=$_SESSION['lang_id'];
}

if($get_language==''){
	include 'local/0/local_online_Pandora.php';
}else{
	include 'local/'.$get_language.'/local_online_Pandora.php';
}

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>無標題文件</title>

</head>

<body><div class="Online_panel_text" id="Pandora">
  <p><?php echo $lang0 ?></p>
  <p><a href="http://www.pandora.com/wd" target="_blank" class="reset"><?php echo $lang1 ?></a> </p>
  <p><em><?php echo $lang2 ?></em></p>
</div>
</body>
</html>
