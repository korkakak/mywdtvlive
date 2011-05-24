
function deviceGetJson(){
	var RandomSeq=useround(0,10000);
	
	$.getJSON('cgi-bin/getServerValue.cgi',{"id":"9","randomSeq":RandomSeq},function(data){
	//$.getJSON('json/device.json',function(data){
		$('#f_DEVICENAME').val(data.DEVICENAME);
	})
	
}
function deviceToJson(){
	
	$('a#sys_device_btn').click(function(){
		 if ($("#sys_device").valid()) {
			 
				 	$.ajax({type:'POST',
				url:'cgi-bin/toServerValue.cgi',
				data:$("#sys_device").fieldsToJson("f_"),
				beforeSend:function(){
							$("#loader").dialog("open");	
				},
				complete:function(data){
					$("#loader").dialog("close");	
					
								
				},
				success:function(data){
					if(data=="Yes"){
						$("#data_update_show_ok").dialog("open");
					}else{
						$("#data_update_show_fail").dialog("open");
					}
				},
				error:function(xhr, ajaxOptions, thrownError){
							//alert(xhr.status);
                    		showError(xhr.status);
				}
		}) 
			}else{
				showError('HasSpecialChars');	
			}
			 
			
	
	 								 
	
	})
	
}
function useround(min,max) {
  return Math.round(Math.random()*(max-min)+min);
}