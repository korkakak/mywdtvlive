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
	
}

if($_SESSION['reg']!=''){
	unset($_SESSION['reg']);
	setcookie("user_pw",""); 
	setcookie("keepSign",""); 
}

$get_language=$_COOKIE['language'];

$_SESSION['lang_id']=$get_language;

if($get_language==''){
	include 'local/0/home.php';
}else{
	
	include 'local/'.$get_language.'/home.php';
}


?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" type="text/css" href="wd_nas/jquery_ui/themes/default/ui.all.css"/>
<link rel="stylesheet" type="text/css" href="wd_nas/global.css"/>
<link rel="stylesheet" type="text/css" href="wd_nas/onboarding.css"/>
<link rel="stylesheet" type="text/css" href="wd_nas/jScrollPane.css"/>
<script type="text/javascript" src="js/jquery/jquery-1.3.2.js"></script>
<script type="text/javascript" src="js/jquery/jquery-ui-1.7.2.custom.min.js"></script>
<script type="text/javascript" src="js/jquery/ui.selectmenu.js"></script>
<script type="text/javascript" src="js/jquery/jquery.bind.js"></script>
<script type="text/javascript" src="js/jquery/ui.checkbox.js"></script>
<script type="text/javascript" src="js/jquery/jquery.mousewheel.js"></script>
<script type="text/javascript" src="js/jquery/jquery.em.js"></script>
<script type="text/javascript" src="js/jquery/jScrollPane.js"></script>
<script type="text/javascript" src="js/jquery/jquery.print.js"></script>
<script type="text/javascript" src="js/jquery/jquery.cookie.js"></script>
<script type="text/javascript" src="js/onboarding.js"></script>
<script type="text/javascript" src="js/ui_init.js"></script>
<link rel="shortcut icon" href="favicon.ico" />
<title>WD TV® Live Hub</title>
</head>
<body>
<div id="alert_List">
  <div id="correct_password"><?php echo $lang0 ?></div>
  <div id="accept_agreement"><?php echo $lang1 ?></div>
  <div id="no_password"><?php echo $lang2 ?></div>
</div>
<script type="text/javascript">
	$("#alert_List").hide();	
</script>
<div id="container">
  <div id="content">
    <div id="onBoarding_logo"><img src="wd_nas/images/logo.png" width="170" height="41" /></div>
    <div id="eulaView">
      <div id="eulaHead">
        <div class="eulaTitle"><?php echo $lang3 ?><br />
          <?php echo $lang4 ?> </div>
        <span class="eulaSubtext"><?php echo $lang5 ?> </span> </div>
      <div id="eulaTextWrap">
        <div id="eulaText">
          <p><?php echo $lang6 ?></p>
          <p><?php echo $lang7 ?><a href="HTTP://www.wdc.com/company/legal">www.wdc.com/company/legal/</a> <?php echo $lang7_1 ?></p>
          <p><?php echo $lang8 ?><br/>
            <?php echo $lang9 ?></p>
          <p><?php echo $lang10 ?> <br/>
            <?php echo $lang11 ?> </p>
          <p><?php echo $lang12 ?></p>
          <p><?php echo $lang13 ?> <br/>
            <?php echo $lang14?></p>
          <p><?php echo $lang15 ?> <br/>
            <?php echo $lang16 ?> </p>
          <p><?php echo $lang17 ?> <br/>
            <?php echo $lang18 ?></p>
          <p><?php echo $lang19 ?> <br/>
            <?php echo $lang20 ?> </p>
          <p><?php echo $lang21 ?> <br/>
            <?php echo $lang22?></p>
          <p><?php echo $lang23 ?></p>
          <p><?php echo $lang24?> <br/>
            <?php echo $lang25 ?></p>
          <p><?php echo $lang26 ?> </p>
          <p><?php echo $lang27?> <br/>
            <?php echo $lang28?> </p>
          <p><?php echo $lang29?> <br/>
            <?php echo $lang30?> </p>
          <p><?php echo  $lang31 ?><br/>
            <?php echo $lang32 ?> </p>
          <p>
            <?php $lang33?>
            <?php echo $lang34?> <br/>
          </p>
          <p><?php echo $lang35?><br/>
            <?php echo $lang36 ?> </p>
          <p>
            <?php $lang37?>
            <br/>
            <?php $lang38 ?>
          </p>
          <p>
            <?php $lang39 ?>
            <br/>
            <?php echo $lang40 ?> </p>
          <p><?php echo $lang41 ?> <br/>
            <?php echo $lang42?></p>
          <p><?php echo $lang43 ?> <br/>
            <?php echo $lang44 ?> <a href="HTTP://WWW.MPEGLA.COM">HTTP://WWW.MPEGLA.COM</a><?php echo $lang44_1 ?></p>
          <p><?php echo $lang45 ?> <br/>
            <?php echo $lang46 ?> <a href="HTTP://WWW.MPEGLA.COM">HTTP://WWW.MPEGLA.COM</a><?php echo $lang46_1 ?></p>
          <p><?php echo $lang47 ?><br/>
            <?php echo $lang48 ?> </p>
          <p><?php echo $lang49 ?> <br/>
            <?php echo $lang50 ?><a href="HTTP://WWW.MPEGLA.COM">HTTP://WWW.MPEGLA.COM</a><?php echo $lang50_1 ?></p>
          <p><?php echo $lang51 ?> <br/>
            <?php echo $lang52 ?> </p>
        </div>
      </div>
      <div id="eulaActions"> <a class="eulaBigLink" id="eulaPrintLink" href="#"><?php echo $lang53 ?></a> <a href="#" class="eulaBigLink" id="eulaAgreeLink"><?php echo $lang54 ?></a>
        <!--<a href="#" id="eulaCancelLink">Cancel</a>-->
      </div>
    </div>
    <form id="eulaform" method="POST" action="Main.php">
      <div id="eula_form">
        <h2><?php echo $lang55 ?></h2>
        <div class="input text">
          <p><?php echo $lang56 ?><br/>
            <input type="password" name="PW" id="PW" tabindex="1"/>
          </p>
        </div>
        <div class="input select"><?php echo $lang57 ?>
          <select name="LangSelect" id="LangSelect" tabindex="2">
            <option value="0" selected="selected"><?php echo $lang59?></option>
            <option value="1"><?php echo $lang60?></option>
            <option value="2"><?php echo $lang61?></option>
            <option value="3"><?php echo $lang62 ?></option>
            <option value="4"><?php echo $lang63 ?></option>
            <option value="5"><?php echo $lang64 ?></option>
            <option value="6"><?php echo $lang65?></option>
            <option value="7"><?php echo $lang66 ?></option>
            <option value="8"><?php echo $lang67?></option>
            <option value="9"><?php echo $lang68 ?></option>
            <option value="10"><?php echo $lang69?></option>
            <option value="11"><?php echo $lang70?></option>
            <option value="12"><?php echo $lang71?></option>
            <option value="13"><?php echo $lang74?></option>
            <option value="14"><?php echo $lang75?></option>
            <option value="15"><?php echo $lang76?></option>
          </select>
          <br />
        </div>
         
        <div class="input checkbox" id="onCheckboxDiv">
          <input type="hidden" name="Agree_hd" id="Agree_hd"/>
          <input name="checkbox" type="checkbox" id="checkbox"/>
          <label class="no_field_onStart"> <?php echo $lang72?> <a id="eula_nbsp" href="#" tabindex="3">&nbsp;</a> <a id="eulaShowLink" href="#" tabindex="4" style="top:inherit"><?php echo $lang4 ?></a> </label>
        </div>
        <div class="input checkbox" id="onCheckboxDiv2">
          <input type="hidden" name="keepSign_hd" id="keepSign_hd"/>
          <input name="keepSign_cb" type="checkbox" id="keepSign_cb"/>
          <label class="no_field_onStart"> <?php echo $lang73?><a id="eula_nbsp_2" href="#" tabindex="5">&nbsp;</a> </label>
        </div>
      </div>
      <div id="eulaSubmitContainer">
        <div class="error"><?php echo $lang1 ?></div>
        <div align="right"><a id="login" class="button" href="#" tabindex="6"><?php echo $lang55?></a></div>
      </div>
    </form>
  </div>
</div>
</body>
</html>
<!-- 0.0872s -->
