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

  
	
	if($_POST['PW']!=''){
		$_SESSION['reg']=true;
	}
	
	if($_SESSION['reg']==''){
		header("Location: index.php" );
	}
  	//
	
	
  	
	
	$get_language=$_SESSION['lang_id'];


	if($get_language==''){
		
		$get_language=0;
	}
	include 'local/'.$get_language.'/main.php';
	
	
	
	function timeShow(){
		$today = date("F j, Y, g:i a"); 
		$seq=strtotime($today);
		return $seq;
	}
	
	
	
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>WD TV® Live Hub</title>
<link rel="stylesheet" type="text/css" href="wd_nas/jquery_ui/themes/default/ui.all.css"/>
<link rel="stylesheet" type="text/css" href="wd_nas/global.css"/>
<script type="text/javascript" src="js/jquery/jquery-1.3.2.js"></script>
<script type="text/javascript" src="js/jquery/jquery-ui-1.7.2.custom.min.js"></script>
<script type="text/javascript" src="js/jquery/ui.selectmenu.js"></script>
<script type="text/javascript" src="js/jquery/jquery.bind.js"></script>
<script type="text/javascript" src="js/jquery/ui.checkbox.js"></script>
<script type="text/javascript" src="js/jquery/jquery.mousewheel.js"></script>
<script type="text/javascript" src="js/jquery/jquery.em.js"></script>
<script type="text/javascript" src="js/jquery/jScrollPane.js"></script>
<script type="text/javascript" src="js/jquery/jquery.print.js"></script>
<script type="text/javascript" src="js/jquery/jquery.timers.js"></script>
<script type="text/javascript" src="js/jquery/jquery.dropshadow.js"></script>
<script type="text/javascript" src="js/global.js"></script>
<script type="text/javascript" src="js/ui_init.js"></script>
<script type="text/javascript" src="js/forms.js"></script>
<script type="text/javascript" src="js/learn_center.js"></script>
<script type="text/javascript" src="js/mbTooltip.min.js"></script>
<script type="text/javascript" src="js/tooltips.js"></script>
<script type="text/javascript" src="js/footer.js"></script>
<script type="text/javascript" src="js/alerts.js"></script>
<script type="text/javascript" src="js/errors.js"></script>
<script type="text/javascript" src="js/json2.js"></script>
<script type="text/javascript" src="js/jquery.filedsToJson.js"></script>
<script type="text/javascript" src="js/system.js"></script>
<script type="text/javascript" src="js/sys_storage.js"></script>
<script type="text/javascript" src="js/device.js"></script>
<script type="text/javascript" src="js/remote.js"></script>
<script type="text/javascript" src="js/media.js"></script>
<script type="text/javascript" src="js/media_itume.js"></script>
<script type="text/javascript" src="js/network.js"></script>
<script type="text/javascript" src="js/jquery.json-2.2.js"></script>
<script type="text/javascript" src="js/jquery.validate.js"></script>
<script type="text/javascript" src="js/jquery.form.js"></script>
<script type="text/javascript" src="js/jquery/jquery.cookie.js"></script>
<link rel="shortcut icon" href="favicon.ico" />
</head>
<body>
<div id="lang68"><?php echo $lang68?></div>
<div id="loader" class="dialog_no_titlebar overlay">
  <div style="height:17px;"></div>
  <img src="wd_nas/loaders/big_circle.gif" alt="" /><?php echo $lang0 ?> </div>
<script type="text/javascript">
$("#loader").dialog({
			autoOpen: false,
			resizable: false,
			position: 'center',
			width:250,
			minHeight:100,
			modal: true,
			stack: true
		});	
</script>

<div id="owner_password_update_confirmation" title="<?php echo $lang1?>" class="overlay">
	<?php echo $lang2 ?></div>
<script type="text/javascript">
	$("#owner_password_update_confirmation").dialog({
			autoOpen: false,
			resizable: false,
			position: 'center',
			width:420,
			minHeight:120,
			modal: true,
			stack: false,
			buttons: {
				'<?php echo $lang3?>': function() {
					$(this).dialog('close');	
				}
			}
		});
</script>

<div id="data_update_show_ok" title="<?php echo $lang63?>" class="overlay">
	<?php echo $lang65 ?></div>
<script type="text/javascript">
	$("#data_update_show_ok").dialog({
			autoOpen: false,
			resizable: false,
			position: 'center',
			width:420,
			minHeight:120,
			modal: true,
			stack: false,
			buttons: {
				'<?php echo $lang3?>': function() {
					$(this).dialog('close');	
				}
			}
		});
</script>

<div id="data_update_show_fail" title="<?php echo $lang64?>" class="overlay">
	<?php echo $lang66 ?></div>
<script type="text/javascript">
	$("#data_update_show_fail").dialog({
			autoOpen: false,
			resizable: false,
			position: 'center',
			width:420,
			minHeight:120,
			dialogClass:'warning_dialog',
			modal: true,
			stack: false,
			buttons: {
				'<?php echo $lang3?>': function() {
					$(this).dialog('close');	
				}
			}
		});
</script>

<div id="register_confirmation_success" title="<?php echo $lang4 ?>" class="overlay">
	<?php echo $lang5 ?></div>
<script type="text/javascript">
	$("#register_confirmation_success").dialog({
			autoOpen: false,
			resizable: false,
			position: 'center',
			width:420,
			minHeight:120,
			modal: true,
			stack: false,
			buttons: {
				'<?php echo $lang3?>': function() {
					$(this).dialog('close');	
				}
			}
		});
</script>

<div id="register_confirmation_Nosuccess" title="<?php echo $lang6 ?>" class="overlay">
	<?php echo $lang7 ?></div>
<script type="text/javascript">
	$("#register_confirmation_Nosuccess").dialog({
			autoOpen: false,
			resizable: false,
			position: 'center',
			width:420,
			minHeight:120,
			dialogClass:'warning_dialog',
			modal: true,
			stack: false,
			buttons: {
				'<?php echo $lang3?>': function() {
					$(this).dialog('close');	
				}
			}
		});
</script>

<div id="register_confirmation_Pending" title="<?php echo $lang8?>" class="overlay">
	<?php echo $lang9 ?></div>
<script type="text/javascript">
	$("#register_confirmation_Pending").dialog({
			autoOpen: false,
			resizable: false,
			position: 'center',
			width:420,
			minHeight:120,
			dialogClass:'warning_dialog',
			modal: true,
			stack: false,
			buttons: {
				'<?php echo $lang3?>': function() {
					$(this).dialog('close');	
				}
			}
		});
</script>

<div id="generic_error_overlay" title="<?php echo $lang10 ?>" class="overlay">
	<ul class="generic_error_list">
		<!-- define additional errors here -->
                <li id="error_NoValue"><?php echo $lang11 ?></li>
                <li id="error_userPassword"><?php echo $lang12?></li>
                <li id="error_userConfirmOldPassword"><?php echo $lang13 ?></li>
				<li id="error_userConfirmPassword"><?php echo $lang14 ?></li>
				<li id="error_Required"><?php echo $lang15 ?></li>
                <li id="error_email"><?php echo $lang16 ?></li>
                <li id="error_userConfirmEmail"><?php echo $lang17 ?></li>
              	<li id="error_400"><?php echo $lang18?></li>
				<li id="error_401"><?php echo $lang19?></li>
				<li id="error_404"><?php echo $lang20?></li>
				<li id="error_405"><?php echo $lang21?></li>
                <li id="error_0"><?php echo $lang22 ?></li>
				<li id="error_HasSpecialChars"><?php echo $lang70.'<br>'.'"\/:*?\";|`~!@#$^&()=+[]{}<>%_,.()- "'.$lang71 ?></li>

	</ul>
</div>
<script type="text/javascript">
	$("#generic_error_overlay").dialog({
			autoOpen: false,
			resizable: false,
			position: 'center',
			width:420,
			minHeight:0,
			dialogClass:'warning_dialog',
			modal: true,
			stack: false,
			buttons: {
				'<?php echo $lang3 ?>': function() {
					$(this).dialog('close');	
				}
			}
		});
</script>
<div id="register_device" title="<?php echo $lang23 ?>" class="overlay">
    <div class="panelText" style="padding: 15px 5px 15px 5px;">
    <?php echo $lang24 ?>
    </div>
    
    
    <form class="cmxform" id="register_device_form"> 
    <div class="panel_form_row alternating_row_0">
    	<table cellpadding="0" cellspacing="0">
        <tr valign="top">

        <td><div class="label"><?php echo $lang25?><span class="required">*</span></div></td>
        <td>
        <input name="frist_name" type="text" tabindex="1" id="frist_name" class="required" value="" />        </td>
        </tr>
        </table>
    </div>
    <div class="panel_form_row alternating_row_1">

    	<table cellpadding="0" cellspacing="0">
        <tr valign="top">
        <td><div class="label"><?php echo $lang26 ?><span class="required">*</span></div></td>
        <td>
        <input name="last_name" type="text" tabindex="2" id="last_name" class="required" value="" />        </td>
        </tr>
        </table>

    </div>
	<div class="panel_form_row alternating_row_0">
    	<table cellpadding="0" cellspacing="0">
        <tr valign="top">
        <td><div class="label"><?php echo $lang27 ?><span class="required">*</span></div></td>
        <td>
        <input name="email" type="text" tabindex="3" id="email" class="required email" value=""/>        </td>

        </tr>
        </table>
    </div>
    <div class="panel_form_row alternating_row_1">
    	<table cellpadding="0" cellspacing="0">
        <tr valign="top">
        <td><div class="label"><?php echo $lang28 ?><span class="required">*</span></div></td>
        <td>

        <input name="confirm_email" type="text" tabindex="4" id="confirm_email" class="required email" value="" />        </td>
        </tr>
        </table>
    </div>
    <div class="panel_form_row alternating_row_0">
    	<table cellpadding="0" cellspacing="0">
        <tr valign="middle">
        <td><div class="label"><?php echo $lang29 ?></div></td>

        <td>
        <input type="hidden" name="PageSn" value="" id="PageSn" /><div id="divice_sn"></div> </td>
        </tr>
        </table>
    </div>
    <div class="panel_form_row alternating_row_1">
    	<table cellpadding="0" cellspacing="0">
        <tr valign="top">

        <td style="padding-right:10px;"><input name="keepInfo_cb" type="checkbox" value="" id="keepInfo_cb" /><input name="keepInfo" type="hidden" value="0" id="keepInfo" /></td>
        <td><label for="userCreatePrivateShare"><?php echo $lang30 ?></label></td>
        </tr>
        </table>
    </div>
    <div class="panel_form_row alternating_row_1">
     	<div class="required_text required" style="margin-top:10px;">
	 <?php echo $lang31?>       </div>

    </div>
    <input type="submit" style="position:absoute; height:1px; visibility:hidden; overflow:hidden;" />
   <input type="hidden" name="f_register" value="" id="f_register" />
    </form>    
    
    
</div>
<script type="text/javascript">
	$("#register_device").dialog({
			autoOpen: false,
			resizable: false,
			position: 'center',
			width:400,
			minHeight:350,
			modal: true,
			stack: true,
			buttons: {
				'<?php echo $lang32 ?>': function() {
					$(this).dialog('close');
				},
				'<?php echo $lang33 ?>': function() {
					processRegistration();
				}
			}
		});
</script>

<div id="container">
  <div id="right" style="top:85px">
    <div id="content_scroll">
      <div id="content"> </div>
    </div>
  </div>
  <div id="left" style="top:85px">
    <ul id="left_nav">
      <li><a class="ajax_nav left_nav_item" id="System_nav" href="<?php echo "DB/connect2sqlite.php?entry_id=1&lang_id=".$get_language."&time=".timeShow()?>" title="<?php echo $lang41 ?>"><?php echo $lang42 ?></a></li>
      <li><a class="ajax_nav left_nav_item" id="network_nav" href="<?php echo "DB/connect2sqlite.php?entry_id=2&lang_id=".$get_language."&time=".timeShow()?>" title="<?php echo $lang43 ?>"><?php echo $lang44 ?></a></li>
         <li><a class="ajax_nav left_nav_item" id="media_nav" href="<?php echo "DB/connect2sqlite.php?entry_id=3&lang_id=".$get_language."&time=".timeShow()?>" title="<?php echo $lang45 ?>"><?php echo $lang46 ?></a></li>
      <li><a class="ajax_nav left_nav_item" id="online_nav" href="<?php  echo "DB/connect2sqlite.php?entry_id=4&lang_id=".$get_language."&time=".timeShow()?>" title="<?php echo $lang47 ?> "><?php echo $lang48 ?></a></li>
      <li><a class="ajax_nav left_nav_item" id="support_nav" href="<?php echo "DB/connect2sqlite.php?entry_id=5&lang_id=".$get_language."&time=".timeShow()?>"  title="<?php echo $lang49 ?>"><?php echo $lang50 ?></a></li>
    </ul>
    <div id="learn_center">
      <div id="learn_center_toggle"><?php echo $lang51 ?> </div>
      <div id="learn_center_content">
        <ul id="learn_center_ul">
          <li>
            <div id="learn_register">
              <div class="learn_center_item_title"><?php echo $lang52 ?></div>
             <?php echo $lang53 ?>  </div>
          </li>
          <li>
            <div id="learn_remote">
              <div class="learn_center_item_title"><?php echo $lang54 ?></div>
            <?php echo $lang55 ?>  </div>
          </li>
          <li>
            <div id="learn_users">
              <div class="learn_center_item_title"><?php echo $lang56 ?></div>
              <?php echo $lang57 ?> </div>
          </li>
        </ul>
        <div id="learn_center_navigation"></div>
      </div>
    </div>
  </div>
</div>
<div id="header">
  <div id="header_content" style="text-align:right;"> <a href="http://www.wdc.com" target="_blank"><img src="wd_nas/images/logo.png" id="logo" alt="" /></a>
    <table cellpadding="0" cellspacing="0" align="right">
      <tr>
        <td><div id="alertNotification">
            <div id="alertNotificationContainer">
              <div id="alertNotificationHead"> <a href="#" class="alert_notify"><img src="wd_nas/images/alert/alert.png" alt="" /></a> </div>
              <div class="alertsDisplayToggle">
                <div id="alerts">
                  <div class="alertContainer"> <a href="#" class="alertClose"><img src="wd_nas/images/alert/alert_close.png" alt="" /></a>
                    <div class="alertTitle"> <?php echo $lang34 ?> </div>
                    <div class="alertMessage"><?php echo $lang35 ?>  </div>
                    <div class="alertOptionsContainer">
                      <table cellpadding="0" cellspacing="0">
                        <tr>
                          <td>
                            <input type="checkbox" id="cap_cb"/><input name="cap_hd" id="cap_hd" type="hidden" value="" />                          </td>
                          <td><span class="alertCheckboxLabel"><?php echo $lang36 ?></span> </td>
                        </tr>
                      </table>
                    </div>
                  </div>
                  <div class="alertContainer"> <a href="#" class="alertClose"><img src="wd_nas/images/alert/alert_close.png" alt="" /></a>
                    <div class="alertTitle"><?php echo $lang37 ?></div>
                    <div class="alertMessage"><?php echo $lang38 ?><div id="vision" style="display:inline"></div> <?php echo "- ".$lang39 ?> </div>
                    <!-- <ul class="alertLinkList">
                     <li><a href="#">Release Notes</a></li>
                      <li><a href="#" class="auto_update_trigger">Begin Update</a></li>
                    </ul>-->
                    <div class="alertOptionsContainer">
                      <table cellpadding="0" cellspacing="0">
                        <tr>
                          <td>
                           <input type="checkbox" id="fiemwire_cb"/><input name="fiemwire_hd" id="fiemwire_hd" type="hidden" value="" />                          </td>
                          <td><span class="alertCheckboxLabel"><?php echo $lang36?></span> </td>
                        </tr>
                      </table>
                    </div>
                  </div>
                </div>
                <div id="alertNotificationFoot"></div>
              </div>
            </div>
          </div></td>
        <td><div id="header_links">
            <div class="not_secure_depends secure_device_link_container"> <a href="index.php?clearn=reg" class="secure_device_link"><?php echo $lang40 ?></a> </div>
            <!-- this is only for testing... DELETE BEFORE PRODUCTION -->
            <br />
            <br />
            <br />
            <!--<a href="/users/output" class="ajax">Show Session Data</a>-->
            <!--  -->
          </div></td>
      </tr>
    </table>
  </div>
</div>
<div id="footer">
  <div id="footer_content">
    <div id="device_graphic"></div>
    <div id="device_display_name" class="open">WDTVLiveHub</div>
    <div id="device_display_status"><?php echo $lang58 ?></div>
    <div id="footer_chart_container">
      <div id="usage_chart_wrap">
        <div id="usage_chart">
          <div class="usage_indicator music" ></div>
          <div class="usage_indicator photos"></div>
          <div class="usage_indicator video"></div>
          <div class="usage_indicator other"></div>
          <div class="usage_indicator all status_good" id="all_usage_indicator"></div>
        </div>
        <div id="indicator_label_wrap">
          <div class="usage_indicator_label music"><?php echo $lang59 ?></div>
          <div class="usage_indicator_label photos"><?php echo $lang60 ?></div>
          <div class="usage_indicator_label video"><?php echo $lang61 ?></div>
         <div class="usage_indicator_label other"><?php echo $lang67 ?></div>
        </div>
      </div>
      <div class="usage_indicator_label" id="all_usage_label"><?php echo $lang62 ?></div>
    </div>
  </div>
</div>
</body>
</html>
