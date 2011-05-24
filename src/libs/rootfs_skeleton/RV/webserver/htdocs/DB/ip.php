<?php
require_once('../security.php');
$isLanReq = isLanRequest();
if($isLanReq === false){
       header("HTTP/1.0 403 Forbidden");
       echo "403 Forbidden";
       return;
}
echo "http://".$_SERVER['HTTP_HOST'].":9000";
//echo "text.php";
?>
