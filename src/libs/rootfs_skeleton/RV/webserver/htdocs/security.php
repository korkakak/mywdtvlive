<?php

function getLocalIpAndMaskFromIfConfig(){
	$localIp = array();

	exec('route -n', $route_out);
	if(count($route_out) == 0){
		exec('sudo route -n', $route_out);
	}

	if (count($route_out) == 0) {
		return $localIp;
	}

	foreach($route_out as $key => $entry) {
		if (strncmp( $entry, "0.0.0.0", 7) == 0) {
			list( $Destination, $Gateway, $Genmask, $Flags, $Metric, $Ref, $Use, $Iface) = sscanf( $entry, "%s %s %s %s %s %s %s %s");
			break;
		}
	}

	if (empty($Iface))
		return $localIp;

	exec('ifconfig' , $ifconfigout);
	if(count($ifconfigout) == 0){
		exec('sudo ifconfig ', $ifconfigout);
	}

	foreach($ifconfigout as $key => $entry){
		$eth0 = strstr($entry, $Iface );
		if(!empty($eth0))
			break;
	}

	$entry = explode(' ', trim($ifconfigout[++$key]));

	foreach($entry as $key => $value){
		if(empty($value) || $value === 'inet')
			continue;
		$ip = strstr($value, "addr:" );
		if($ip != false){
			$ipAddress = substr($value, 5); // Skipping "addr:"
			$localIp['ip'] = $ipAddress;
			continue;
		}
		$mask = strstr($value, "Mask:" );
		if($mask != false){
			$mask = substr($value, 5); // Skipping "addr:"
			$localIp['mask'] = $mask ;
			continue;
		}
	}
	return $localIp;
}



/*
 * Returns true if request is from local IP
 */
function isLanRequest(){

	$remoteAddr = $_SERVER['REMOTE_ADDR'];
	if($remoteAddr === '127.0.0.1')
		return true;

	$localIpAndMask = getLocalIpAndMaskFromIfConfig();
	if(isset($localIpAndMask['ip'])){
		$localIp = ip2long(trim($localIpAndMask['ip']));
	    $remoteIp = ip2long($_SERVER['REMOTE_ADDR']);
	    $mask = ip2long(trim($localIpAndMask['mask']));
		if((($localIp ^ $remoteIp) & $mask) == 0){
			return true;
		} else {
			return false;
		}
	}
	return true;
}

?>
