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
	include 'local/0/local_password.php';
}else{
	include 'local/'.$get_language.'/local_password.php';
}

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>無標題文件&nbsp;</title>
</head>
<body> 
<script type="text/javascript">
	password();
</script>
<form id="sys_pw_form" name="form1" method="post" action="">
<div class="alternating_row alternating_row_0">
	<label class="no_field"><?php echo $lang0 ?></label>
   
      <input type="password" name="sys_old_pw" id="sys_old_pw" />
   
</div>
<div class="alternating_row alternating_row_1">
	<label class="no_field"><?php echo $lang1 ?></label>
     <input type="password" name="sys_new_pw" id="sys_new_pw" />
</div>
<div class="alternating_row alternating_row_0">
	<label class="no_field"><?php echo $lang2 ?></label>
    <input type="password" name="sys_confirm_pw" id="sys_confirm_pw" />
</div>
<div class="alternating_row alternating_row_1">
	<label class="no_field">&nbsp;</label>
      <a id="sys_pw_btn" class="button" href="#"><?php echo $lang3 ?></a>
</div>
 </form>
</body>
</html>
