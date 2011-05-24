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
	include 'local/0/local_Support.php';
}else{
	include 'local/'.$get_language.'/local_Support.php';
}

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>無標題文件</title>
</head>

<body>
<table width="100%" cellspacing="0" cellpadding="0">
        <tbody><tr valign="top">
        <td>
        <div class="left_interior_panel interior_panel support">
            <div class="panel_section_title no_top"><?php echo $lang0 ?></div>
            <div class="quickSupportDesc panelText">
            <?php echo $lang1 ?>
            </div>
            <hr>
            <div style="text-align: center;" class="panelText">
            <a href="http://support.wdc.com/" target="_blank" class="button" id="begin_support_button"><?php echo $lang2 ?></a>&nbsp;&nbsp;<a href="http://www.wdc.com/en/company/privacy.asp?wdc_lang=en" target="_blank" class="privacyPolicyLink reset"><?php echo $lang3 ?></a>            </div>
            
<!--<div class="vertical_panel_spacer">
                <div class="hide_borders_spacer"></div>
            </div>

            <div class="panel_section_title"><?php echo $lang4 ?></div>
                <a class="button" id="run_report_button" href="/files/system_report"><?php echo $lang5 ?> &amp; <?php echo $lang6 ?></a>-->
            </div>
        
        </td>
        <td align="right">
        <div class="right_interior_panel interior_panel support">
            <div class="panel_section_title no_top"><?php echo $lang7 ?></div>
            <div class="help_text_link_container">
                <a class="reset" href="http://www.wdc.com/en/library/library.asp?t=2
" target="_blank"><?php echo $lang8 ?></a><br><br>
                <a class="reset" href="http://wdc.custhelp.com/cgi-bin/wdc.cfg/php/enduser/ask.php" target="_blank"><?php echo $lang9 ?></a><br><br>
                <a class="reset" href="http://community.wdc.com/
" target="_blank"><?php echo $lang10 ?></a>
            </div>
        </div>
        </td>
        </tr>
        </tbody></table>
</body>
</html>
