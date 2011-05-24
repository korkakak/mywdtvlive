<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>WD TV® Live Hub</title>
<script type="text/javascript" src="../../js/jquery/jquery-1.3.2.js"></script>
<script type="text/javascript" src="../../js/json2.js"></script>
<script type="text/javascript" src="../../js/jquery.filedsToJson.js"></script>
<link href="../../wd_nas/global.css" rel="stylesheet" type="text/css" />
<style type="text/css">
body{
	
	background-color:#ffffff;
	background-image:url();

}
</style>
</head>

<body>
<script type="text/javascript">
	$(document).ready(function(){
		var windowW=$(window).width();
		var windowH=$(window).height();
		
		if(windowW<=240 || windowH<=320){
			$("#myFrame").load("remote_small.html");
		}else{
			$("#myFrame").load("remote.html");
		}
		
	})
	
</script>
<br />
<div id="search_div" align="center"></div><br />
<div id="myFrame"></div>
</body>
</html>
