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
	include 'local/0/local_media_itune.php';
}else{
	include 'local/'.$get_language.'/local_media_itune.php';
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
	initCheckboxes();
	ituneGetJson();
	ituneSubmit();
	initTooltips();
</script>
<form action="" method="get" id="media_itune_from">
<div class="panelText itunesText" id="itunePanel">
<?php echo $lang0 ?></div>
<div class="alternating_row alternating_row_0">

    <label class="no_field"><?php echo $lang1 ?></label>
    <div class="input checkbox">
      <input name="twonky" type="checkbox" id="itunes" />
      <label for="twonky"><?php echo $lang2 ?></label>
    </div>
  	<input type="hidden" name="f_itunes" id="f_itunes" />
</div>
<div class="alternating_row alternating_row_1">

    <label class="no_field">&nbsp;</label>
   <div class="float_left_wrap">
   <div id="itunes_rescan_div" class="float_left_wrap"> <a id="itunes_rescan_btn" class="button" href="#"><?php echo $lang3 ?></a><a href="#" class="helpLink" title="<?php echo $lang4 ?>"><img src="wd_nas/images/common/info.png" width="12" height="12" /></a></div>
   
    <a id="itunes_submit_btn" class="button madia_btn" href="#"><?php echo $lang5 ?></a>
    
   </div>   
</div>
</form>
</body>
</html>
