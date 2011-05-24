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
	include 'local/0/local_online_Netflix.php';
}else{
	include 'local/'.$get_language.'/local_online_Netflix.php';
}

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>無標題文件</title>
</head>

<body>
<div class="Online_panel_text" id="Netflix">
<p><?php echo $lang0 ?><a href="http://www.netflix.com/wdtv" target="_blank" class="reset">http://www.netflix.com/wdtv</a><?php echo $lang0_1 ?></p>
  <p><a id="itunes_rescan_btn" class="button" href="http://www.netflix.com/" target="_blank"><?php echo $lang1 ?></a></p>
</div>
</body>
</html>
