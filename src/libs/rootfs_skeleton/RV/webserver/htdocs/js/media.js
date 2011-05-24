function initCheckboxes(){
	$("input[type='checkbox']").checkBox('reflectUI'); 
	$("input[type='checkbox']").checkBox();
	$("input[type='radio']").checkBox('reflectUI'); 
	$("input[type='radio']").checkBox(); 
}
function twonkeyGetJson(){
	var RandomSeq=useround(0,10000);
	
	$.getJSON('cgi-bin/getServerValue.cgi',{"id":"4","randomSeq":RandomSeq},function(data){
	//$.getJSON('json/twonkey.json',function(data){
	
		if(data.twonky=="1"){
		
			//$("#twonky").next('span.ui-checkbox').addClass('ui-checkbox-state-checked');
			$('#media_twonkey_from input[type=checkbox]').checkBox('changeCheckStatus', true); 
			/*$.get('DB/ip.php',function(data){
					$("#load_page").show();  
					$("#load_page").append(data) 
					
			})*/
			
			$('#Launch_twonkey_div').show();

		}else{
			$('#Launch_twonkey_div').hide();
			//$("#load_page").hide();
		}
		$('#f_twonky').val(data.twonky);
	})	
}
function twonkeySubmit(){
	$('#twonky').click(function(){
		if( $(this).is(":checked") ){
			$('#f_twonky').val(1);
			/*$.get('DB/ip.php',function(data){
					$("#load_page").show();  
					$("#load_page").load(data); 
					
			})*/
			$('#Launch_twonkey_div').show();
			
		}else{
			$('#f_twonky').val(0);
			//$("#load_page").hide();
			$('#Launch_twonkey_div').hide();
		}
	})
	$('a#twonkey_btn').click(function(){
		$.ajax({type:'POST',url:'cgi-bin/toServerValue.cgi',data:$("#media_twonkey_from").fieldsToJson("f_"),
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
				}})	
	})
}
function launchURL(){
	$.get('DB/ip.php',function(data){
			
			
		$('a#Launch_twonkey_btn').attr('href',data)
		
					
	})	
}
function useround(min,max) {
  return Math.round(Math.random()*(max-min)+min);
}