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
	include 'local/0/local_media_twonky.php';
}else{
	include 'local/'.$get_language.'/local_media_twonky.php';
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
	$("#load_page").hide();
	initCheckboxes();
	twonkeyGetJson();
	twonkeySubmit();
	launchURL();
	initTooltips();
</script>
<form action="" method="get" id="media_twonkey_from">

<div class="panelText" id="twonkySectionHead"> <?php echo $lang0 ?><a target="_blank" class="reset" href="http://www.twonkymedia.com/"><?php echo $lang1 ?></a> </div>
<div class="alternating_row alternating_row_0">

    <label class="no_field"><?php echo $lang2 ?></label>
    <div class="input checkbox">
      <input name="twonky" type="checkbox" id="twonky"/>
      <label for="twonky"><?php echo $lang3 ?> </label>
      <a href="#" class="helpLink " title="<?php echo $lang4 ?>"><img src="wd_nas/images/common/info.png" alt="" /></a>
    </div>
	</div>
    
  <input type="hidden" name="f_twonky" id="f_twonky" />
   

<div class="alternating_row alternating_row_1">

    <label class="no_field">&nbsp;</label>
    <div id="Launch_twonkey_div" class="float_left_wrap">
    <a href="#" target="_blank" class="button" id="Launch_twonkey_btn" title="<?php echo $lang6 ?>"><?php echo $lang5 ?></a>
    <a href="#" class="helpLink" title="<?php echo $lang6 ?>"><img src="wd_nas/images/common/info.png" width="12" height="12" /></a>
    
    </div>
  <a id="twonkey_btn" class="button madia_btn" href="#"><?php echo $lang7 ?></a></div>

</form>
<div id="load_page"></div>
</body>
</html>
