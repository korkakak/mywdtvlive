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
<title>無標題文件</title>
<script>
$(document).ready(function(){
	
		$("#search_btn").click(function(){
		$.ajax({type:'POST',url:'/cgi-bin/toServerValue.cgi',data:$("#search_form").fieldsToJson("f_") })
	})
})
</script>
</head>

<body>
<form id="search_form" name="search_form">
      <input type="text" id="f_search" style="width:150px"/>
    <a id="search_btn" class="button" href="#"><?php echo $lang3 ?></a>
</form>
</body>
</html>
