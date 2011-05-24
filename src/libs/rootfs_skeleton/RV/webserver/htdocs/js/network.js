function networkGetJson(){
	$('#lang7').hide();
	$.getJSON('cgi-bin/getServerValue.cgi?id=3',function(data){
//	$.getJSON('json/network_info.json',function(data){													 
		if(data.connect_type==0){
			$('#network_sssid').hide();
			$('#connect_type').text($('#lang7').text());
			InitRow();
		}else{
			$('#network_sssid').show();
			$('#connect_type').text('Wireless');
			InitRow();
		}
		$('#f_ip_address').text(data.ip_address);
		$('#f_netmask').text(data.netmask);
		$('#f_gateway').text(data.gateway);
		$('#f_dns').text(data.dns);
		$('#f_SSID').text(data.SSID);
		$('#f_workgroup').text(data.workgroup);
	})	
}
/*function networkSubmit(){
	$('#sys_network_btn').click(function(){
		$.ajax({type:'POST',url:'toServerValue.cgi',data:$("#sys_network").fieldsToJson("f_") })
	})	
	$('#sys_workgroup_btn').click(function(){
		$.ajax({type:'POST',url:'toServerValue.cgi',data:$("#sys_workgroup").fieldsToJson("f_") })
	})
}*/
function InitRow(){
	
	var count = 0;
	$('#row_network').find('.alternating_row').each(function(){
		row = (count%2);
		
		if($(this).css("display")=="block"){
			$(this).addClass('alternating_row_'+row);
			count++;	
		}
		
		
	})
	
		
}
