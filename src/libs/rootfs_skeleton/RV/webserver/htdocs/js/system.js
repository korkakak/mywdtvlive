function initCheckboxes(){
	$("input[type='checkbox']").checkBox('reflectUI'); 
	$("input[type='checkbox']").checkBox();
	$("input[type='radio']").checkBox('reflectUI'); 
	$("input[type='radio']").checkBox(); 
}
function initSelectMenus(selector){
	
	selector.each(function(){
		timezone();
		
						 
	});
}

function timezone(){
	
	var RandomSeq=useround(0,10000);
	
	$.getJSON('cgi-bin/getServerValue.cgi',{"id":"1","randomSeq":RandomSeq},function(data){
		//$.getJSON('json/ntp.json',function(data){	
		
	
   		 $("#TimeZoneSelect option[value="+data.timezone+"]").attr('selected', 'selected');
	
		$("#TimeZoneSelect").selectmenu({width:350, maxHeight:150, style:'dropdown', change:function(){
			$('#f_timezone').val($(this).val());																							 
		}						
		
		});
		
		if(data.DST=="1"){
			$('#sys_ntp input[type=radio]:eq(0)').checkBox('changeCheckStatus', true);	
		}else{
			$('#sys_ntp input[type=radio]:eq(1)').checkBox('changeCheckStatus', true);	
		}
		$('#f_DST').val(data.DST);
		$('#f_timezone').val(data.timezone);
		
		$("#nowDateTime").text(data.NowDateTime);
	})
	

	
}
function timezonetojson(){
	
	$('a#sys_ntp_btn').click(function(){
			
	 	$.ajax({type:'POST',url:'cgi-bin/toServerValue.cgi',data:$("#sys_ntp").fieldsToJson("f_"),
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
	
	})
	
}
function password(){
	
	$('a#sys_pw_btn').click(function(){
									
	var id_check=/[^a-zA-Z0-9]/g;
		
		if($("input#sys_old_pw").val()!='' && $("input#sys_new_pw").val()!='' &&$("input#sys_confirm_pw").val()!=''){
			
			if(!$("input#sys_new_pw").val().match(id_check)&&$("input#sys_new_pw").val().length<=40){
				
								
					if($("input#sys_new_pw").val()==$("input#sys_confirm_pw").val()){
					
					
					$.post('DB/login.php',{"password":$("input#sys_old_pw").val()}, function(data) {
						if(data=="yes"){
							
							//$('#saving_data').dialog('open');
							//$.ajax({type:'POST',url:'DB/modfiy_pw.php',data:$("#sys_pw_form").serialize() })
							$.post('DB/modfiy_pw.php',{"password":$("input#sys_new_pw").val()}, function(data) {
								
								$("#owner_password_update_confirmation").dialog("open");
									
								
								
							})
						}else{
							showError('userConfirmOldPassword');
						}
					
					})
				}else{
					showError('userConfirmPassword');	
				}
				
				
			}else{
				
				showError('userPassword');
			}
			
			
		}else{
			showError('NoValue');
		}
																				 
		
		
		
		
		
		
	})
	
	
}

function system(){
	
	initCheckboxes();
	
	initSelectMenus($('select'));
	timezonetojson();
	
	$("#ntp_on").live('click', function(){
												   
		if( $(this).is(":checked") ){
			
			$('#f_DST').val(1);
			 
		 }										   
	});
	$("#ntp_off").live('click', function(){
												   
		if( $(this).is(":checked") ){
			
			$('#f_DST').val(0);
			 
		 }										   
	});
	setInterval(function (){
						  
		var RandomSeq=useround(0,10000);
		
		
		$.getJSON('cgi-bin/getServerValue.cgi',{"id":"1","randomSeq":RandomSeq},function(data){	
			
			$("#nowDateTime").text(data.NowDateTime);
			
						
		});								   
	},60000);
	
} 
function useround(min,max) {
  return Math.round(Math.random()*(max-min)+min);
}
