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
	include 'local/0/local_storage_status.php';
}else{
	include 'local/'.$get_language.'/local_storage_status.php';
}

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>無標題文件</title>


</head>

<body>
<script type="text/javascript">
	storageGetJson();
</script>
<form action="" method="get">
<div id="lang4"><?php echo $lang4?></div>
<div id="lang5"><?php echo $lang5?></div>
<div class="alternating_row alternating_row_0">
<label class="no_field"><?php echo $lang0 ?></label>
<select name="LocalStorageSelect" id="LocalStorageSelect">
</select>
</div>

<div class="alternating_row alternating_row_1">
  <label class="no_field"><?php echo $lang1 ?></label> 
  <div id="Volume"></div>
</div>
<div class="alternating_row alternating_row_0">
  <label class="no_field"><?php echo $lang2 ?></label>
 <div id="Usage"></div></div>
<div class="alternating_row alternating_row_1">
  <label class="no_field"><?php echo $lang3 ?></label>
  <div id="Status"></div></div>

</form>
</body>
</html>
