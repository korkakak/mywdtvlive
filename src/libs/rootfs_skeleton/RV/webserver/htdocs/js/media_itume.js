function initCheckboxes(){
	$("input[type='checkbox']").checkBox('reflectUI'); 
	$("input[type='checkbox']").checkBox();
	$("input[type='radio']").checkBox('reflectUI'); 
	$("input[type='radio']").checkBox(); 
}
function ituneGetJson(){
	var RandomSeq=useround(0,10000);
	$.getJSON('cgi-bin/getServerValue.cgi',{"id":"5","randomSeq":RandomSeq},function(data){
		//$.getJSON('json/itunes.json',function(data){
			
		if(data.itunes=="1"){
		
			//$("#itunes").next('span.ui-checkbox').addClass('ui-checkbox-state-checked');
			$('#media_itune_from input[type=checkbox]').checkBox('changeCheckStatus', true); 
		}else{
			$('#itunes_rescan_div').hide();
			}
		$('#f_itunes').val(data.itunes);
	})
}
function ituneSubmit(){
	$('#itunes').click(function(){
		if( $(this).is(":checked") ){
			$('#f_itunes').val(1);
			$('#itunes_rescan_div').show();
			
		}else{
			$('#itunes_rescan_div').hide();
			$('#f_itunes').val(0);
		}
	})
	$('a#itunes_rescan_btn').click(function(){
		$.post('cgi-bin/getServerValue.cgi?id=8',function(data){
			
		})	
	})
	$('a#itunes_submit_btn').click(function(){
			$.ajax({type:'POST',url:'cgi-bin/toServerValue.cgi',data:$("#media_itune_from").fieldsToJson("f_"),
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
function useround(min,max) {
  return Math.round(Math.random()*(max-min)+min);
}
