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
	include 'local/0/local_online_live365.php';
}else{
	include 'local/'.$get_language.'/local_online_live365.php';
}

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>無標題文件</title>


</head>

<body>
<div class="Online_panel_text" id="live365">
  <p><?php echo $lang0 ?></p>
  <p><?php echo $lang1 ?></p>
  <p><a href="http://www.live365.com/wdtv" target="_blank" class="reset"><?php echo $lang2 ?></a><br />
  </p>
</div>
</body>
</html>
