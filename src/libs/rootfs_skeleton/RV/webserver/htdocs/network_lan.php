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
$get_language=$_COOKIE['language'];

if($get_language==''){
	include 'local/0/local_network_lan.php';
}else{
	include 'local/'.$get_language.'/local_network_lan.php';
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
networkGetJson();
</script>

<div id="lang7"><?php echo $lang7?></div>
<form action="" method="get" id="sys_network">
<div id="row_network">
  <div class="alternating_row">
    <label class="no_field"><?php echo $lang0 ?></label>
    <div id="connect_type"></div>
  </div>
  <div class="alternating_row" id="network_sssid">
    <label class="no_field"><?php echo $lang1 ?></label>
    <div id="f_SSID"></div>
  </div>
  <div class="alternating_row">
    <label class="no_field"><?php echo $lang2 ?></label>
    <div id="f_ip_address"></div>
  </div>
  <div class="alternating_row">
    <label class="no_field"><?php echo $lang3 ?></label>
    <div id="f_netmask"></div>
  </div>
  <div class="alternating_row">
    <label class="no_field"><?php echo $lang4 ?></label>
    <div id="f_gateway"></div>
  </div>
  <div class="alternating_row">
    <label class="no_field"><?php echo $lang5 ?></label>
    <div id="f_dns"></div>
  </div>
  
  <div class="alternating_row">
      <label class="no_field"><?php echo $lang6 ?></label>
    
      <div id="f_workgroup"></div>
  </div>
</div>
</form>
</body>
</html>
