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
	include 'local/0/local_ntp.php';
}else{
	include 'local/'.$get_language.'/local_ntp.php';
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
	system();
	initTooltips();
</script>
<form id="sys_ntp" name="sys_ntp" method="post" action="">

<div class="alternating_row alternating_row_0">


<label class="no_field"><?php echo $lang0 ?></label>
<div id="NtpSelect" style="display:inline-block">
<select name="TimeZoneSelect" id="TimeZoneSelect">
  <option value="-12:00_0"><?php echo _("(UTC-12:00) International Date Line West")?></option>
  <option value="-11:00_0"><?php echo _("(UTC-11:00) Coordinated Universal Time-11")?></option>
  <option value="-11:00_1"><?php echo _("(UTC-11:00) Samoa")?></option>
  <option value="-10:00_0"><?php echo _("(UTC-10:00) Hawaii")?></option>
  <option value="-09:00_0"><?php echo _("(UTC-09:00) Alaska")?></option>
  <option value="-08:00_0"><?php echo _("(UTC-08:00) Baja California")?></option>
  <option value="-08:00_1"><?php echo _("(UTC-08:00) Pacific Time (US &amp Canada)")?></option>
  <option value="-07:00_0"><?php echo _("(UTC-07:00) Arizona")?></option>
  <option value="-07:00_1"><?php echo _("(UTC-07:00) Chihuahua, La Paz, Mazatlan")?></option>
  <option value="-07:00_2"><?php echo _("(UTC-07:00) Mountain Time (US &amp Canada)")?></option>
  <option value="-06:00_0"><?php echo _("(UTC-06:00) Central America")?></option>
  <option value="-06:00_1"><?php echo _("(UTC-06:00) Central Time (US &amp Canada)")?></option>
  <option value="-06:00_2"><?php echo _("(UTC-06:00) Guadalajara, Mexico City, Monterrey")?></option>
  <option value="-06:00_3"><?php echo _("(UTC-06:00) Saskatchewan")?></option>
  <option value="-05:00_0"><?php echo _("(UTC-05:00) Bogota, Lima, Quito")?></option>
  <option value="-05:00_1"><?php echo _("(UTC-05:00) Eastern Time (US &amp Canada)")?></option>
  <option value="-05:00_2"><?php echo _("(UTC-05:00) Indiana (East)")?></option>
  <option value="-04:30_0"><?php echo _("(UTC-04:30) Caracas")?></option>
  <option value="-04:00_0"><?php echo _("(UTC-04:00) Asuncion")?></option>
  <option value="-04:00_1"><?php echo _("(UTC-04:00) Altlantic Time (Canada)")?></option>
  <option value="-04:00_2"><?php echo _("(UTC-04:00) Cuiaba")?></option>
  <option value="-04:00_3"><?php echo _("(UTC-04:00) Georgetown, La Paz, Manaus, San Juan")?></option>
  <option value="-04:00_4"><?php echo _("(UTC-04:00) Santiago")?></option>
  <option value="-03:30_0"><?php echo _("(UTC-03:30) Newfoundland")?></option>
  <option value="-03:00_0"><?php echo _("(UTC-03:00) Brasilia")?></option>
  <option value="-03:00_1"><?php echo _("(UTC-03:00) Buenos Aires")?></option>
  <option value="-03:00_2"><?php echo _("(UTC-03:00) Cayenne, Fortaleza")?></option>
  <option value="-03:00_3"><?php echo _("(UTC-03:00) Greenland")?></option>
  <option value="-03:00_4"><?php echo _("(UTC-03:00) Montevideo")?></option>
  <option value="-02:00_0"><?php echo _("(UTC-02:00) Coordinated Universal Time-02")?></option>
  <option value="-02:00_1"><?php echo _("(UTC-02:00) Mid-Atlantic")?></option>
  <option value="-01:00_0"><?php echo _("(UTC-01:00) Azores")?></option>
  <option value="-01:00_1"><?php echo _("(UTC-01:00) Cape Verde Is.")?></option>
  <option value="+00:00_0"><?php echo _("(UTC) Casablanca")?></option>
  <option value="+00:00_1"><?php echo _("(UTC) Coordinated Universal Time")?></option>
  <option value="+00:00_2"><?php echo _("(UTC) Dublin, Edinburgh, Lisbon, London")?></option>
  <option value="+00:00_3"><?php echo _("(UTC) Monrovia, Reykjavik")?></option>
  <option value="+01:00_0"><?php echo _("(UTC+01:00) Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna")?></option>
  <option value="+01:00_1"><?php echo _("(UTC+01:00) Belgrade, Bratislava, Budapest, Ljubljana, Prague")?></option>
  <option value="+01:00_2"><?php echo _("(UTC+01:00) Brussels, Copenhagen, Madrid, Paris")?></option>
  <option value="+01:00_3"><?php echo _("(UTC+01:00) Sarajevo, Skopje, Warsaw, Zagreb")?></option>
  <option value="+01:00_4"><?php echo _("(UTC+01:00) West Central Africa")?></option>
  <option value="+02:00_0"><?php echo _("(UTC+02:00) Amman")?></option>
  <option value="+02:00_1"><?php echo _("(UTC+02:00) Athens, Bucharest, Istanbul")?></option>
  <option value="+02:00_2"><?php echo _("(UTC+02:00) Beirut")?></option>
  <option value="+02:00_3"><?php echo _("(UTC+02:00) Cairo")?></option>
  <option value="+02:00_4"><?php echo _("(UTC+02:00) Damascus")?></option>
  <option value="+02:00_5"><?php echo _("(UTC+02:00) Harare, Pretoria")?></option>
  <option value="+02:00_6"><?php echo _("(UTC+02:00) Helsinki, Kyiv, Riga, Sofia, Tallinn, Vilnius")?></option>
  <option value="+02:00_7"><?php echo _("(UTC+02:00) Jerusalem")?></option>
  <option value="+02:00_8"><?php echo _("(UTC+02:00) Minsk")?></option>
  <option value="+02:00_9"><?php echo _("(UTC+02:00) Windhoek")?></option>
  <option value="+03:00_0"><?php echo _("(UTC+03:00) Baghdad")?></option>
  <option value="+03:00_1"><?php echo _("(UTC+03:00) Kuwait, Riyadh")?></option>
  <option value="+03:00_2"><?php echo _("(UTC+03:00) Moscow, St. Petersburg, Volgograd")?></option>
  <option value="+03:00_3"><?php echo _("(UTC+03:00) Nairobi")?></option>
  <option value="+03:30_0"><?php echo _("(UTC+03:30) Tehran")?></option>
  <option value="+04:00_0"><?php echo _("(UTC+04:00) Abu Dhabi, Muscat")?></option>
  <option value="+04:00_1"><?php echo _("(UTC+04:00) Baku")?></option>
  <option value="+04:00_2"><?php echo _("(UTC+04:00) Port Louis")?></option>
  <option value="+04:00_3"><?php echo _("(UTC+04:00) Tbilisi")?></option>
  <option value="+04:00_4"><?php echo _("(UTC+04:00) Yerevan")?></option>
  <option value="+04:30_0"><?php echo _("(UTC+04:30) Kabul")?></option>
  <option value="+05:00_0"><?php echo _("(UTC+05:00) Ekaterinburg")?></option>
  <option value="+05:00_1"><?php echo _("(UTC+05:00) Islamabad, Karachi")?></option>
  <option value="+05:00_2"><?php echo _("(UTC+05:00) Tashkent")?></option>
  <option value="+05:30_0"><?php echo _("(UTC+05:30) Chennai, Kolkata, Mumbai, New Delhi")?></option>
  <option value="+05:30_1"><?php echo _("(UTC+05:30) Sri Jayawardenepura")?></option>
  <option value="+05:45_0"><?php echo _("(UTC+05:45) Kathmandu")?></option>
  <option value="+06:00_0"><?php echo _("(UTC+06:00) Astana")?></option>
  <option value="+06:00_1"><?php echo _("(UTC+06:00) Dhaka")?></option>
  <option value="+06:00_2"><?php echo _("(UTC+06:00) Novosibirsk")?></option>
  <option value="+06:30_0"><?php echo _("(UTC+06:30) Yangon (Rangoon)")?></option>
  <option value="+07:00_0"><?php echo _("(UTC+07:00) Bangkok, Hanoi, Jakarta")?></option>
  <option value="+07:00_1"><?php echo _("(UTC+07:00) Krasnoyarsk")?></option>
  <option value="+08:00_0"><?php echo _("(UTC+08:00) Beijing, Chongqing, Hong Kong, Urumqi")?></option>
  <option value="+08:00_1"><?php echo _("(UTC+08:00) Irkutsk")?></option>
  <option value="+08:00_2"><?php echo _("(UTC+08:00) Kuala Lumpur, Singapore")?></option>
  <option value="+08:00_3"><?php echo _("(UTC+08:00) Perth")?></option>
  <option value="+08:00_4"><?php echo _("(UTC+08:00) Taipei")?></option>
  <option value="+08:00_5"><?php echo _("(UTC+08:00) Ulaanbaatar")?></option>
  <option value="+09:00_0"><?php echo _("(UTC+09:00) Osaka, Sapporo, Tokyo")?></option>
  <option value="+09:00_1"><?php echo _("(UTC+09:00) Seoul")?></option>
  <option value="+09:00_2"><?php echo _("(UTC+09:00) Yakutsk")?></option>
  <option value="+09:30_0"><?php echo _("(UTC+09:30) Adelaide")?></option>
  <option value="+09:30_1"><?php echo _("(UTC+09:30) Darwin")?></option>
  <option value="+10:00_0"><?php echo _("(UTC+10:00) Brisbane")?></option>
  <option value="+10:00_1"><?php echo _("(UTC+10:00) Canberra, Melbourne, Sydney")?></option>
  <option value="+10:00_2"><?php echo _("(UTC+10:00) Guam, Port Moresby")?></option>
  <option value="+10:00_3"><?php echo _("(UTC+10:00) Hobart")?></option>
  <option value="+10:00_4"><?php echo _("(UTC+10:00) Vladivostok")?></option>
  <option value="+11:00_0"><?php echo _("(UTC+11:00) Magadan, Solomon Is., New Caledonia")?></option>
  <option value="+12:00_0"><?php echo _("(UTC+12:00) Auckland, Wellington")?></option>
  <option value="+12:00_1"><?php echo _("(UTC+12:00) Coordinated Universal Time+12")?></option>
  <option value="+12:00_2"><?php echo _("(UTC+12:00) Fiji")?></option>
  <option value="+13:00_0"><?php echo _("(UTC+13:00) Nuku'alofa")?></option>
</select>

<label for="TimeZoneSelect"><div id="nowDateTime" style="display:inline"></div><a href="#" class="helpLink" style="width:inherit" title="<?php echo $lang1 ?>"><img src="wd_nas/images/common/info.png" width="12" height="12" /></a></label>


<input name="f_timezone" type="hidden" id="f_timezone"/>
</div>
</div>
<div class="alternating_row alternating_row_1">
<label class="no_field"><?php echo $lang2 ?></label><div class="float_left_wrap"><div class="input radio">
  <input type="radio" name="radio" id="ntp_on" value="1" />
  <label for="ntp_on"><?php echo $lang3 ?></label>
  <input type="radio" name="radio" id="ntp_off" value="0" />
  <label for="ntp_off"><?php echo $lang4 ?></label>
  <input type="hidden" name="f_DST" id="f_DST" />
</div>
</div>
</div>
<div class="alternating_row alternating_row_1">
	<label class="no_field">&nbsp;</label>
      <a id="sys_ntp_btn" class="button" href="#"><?php echo $lang5 ?></a>
</div>
       
</form>
</body>
</html>
