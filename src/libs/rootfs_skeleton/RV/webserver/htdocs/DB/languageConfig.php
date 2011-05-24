<?php
require_once('../security.php');
$isLanReq = isLanRequest();
if($isLanReq === false){
       header("HTTP/1.0 403 Forbidden");
       echo "403 Forbidden";
       return;
}

$get_lang=$_COOKIE["language"];
putenv('LC_ALL='.$get_lang);
setlocale(LC_ALL, $get_lang);

$Lang_package="Mutlan";

bindtextdomain($Lang_package, "./locale");
textdomain($Lang_package);
bind_textdomain_codeset ($Lang_package, "utf-8");


?>
