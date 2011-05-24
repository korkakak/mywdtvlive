function setAlertContainerDisplay(){
    if(!$(".alertMessage:visible").length){
        $(".alertsDisplayToggle").fadeOut('slow',
            function(){        
                $("#alertNotificationHead").removeClass('alertContainerActive');
            }
        );
        $("#alertNotification").css('visibility','hidden');
    }else{
        $(".alertContainer:first").addClass('alertContainerFirst');
        $(".alertContainer:last").addClass('alertContainerLast'); 
    }
	
	
}




function alertGetJson(){
	
	

	
	//$.getJSON('json/alerts.json',function(data){
	$.getJSON('cgi-bin/getServerValue.cgi?id=7',function(data){
														   
		var alert_cap;
		var alert_fw;
		
		if(data.FULL_CAP=="0" && data.UPDATE_FIRMWARE=="0"){
			
			  $("#alertNotificationContainer").hide();	
		}else{
			if(data.FULL_CAP=="0"){
				$(".alertContainer:first").hide();
				alert_cap=0;
				
			}else{
				alert_cap=1;	
			}	
			if(data.UPDATE_FIRMWARE=="0"){
				$(".alertContainer:last").hide();
				
				alert_fw=0;
				
			}else{
				alert_fw=1;
				
			}	
			
		}
		$.cookie('online', data.ONLINE);
		$("#cap_hd").val(alert_cap);
		$("#fiemwire_hd").val(alert_fw);
		
		$('#vision').text(data.VISION);
		
		$.getJSON('DB/alerts.php',{"id":1},function(data){
		
		
		
		if(data.FULL_CAP=='n'){
			if(alert_cap==1){
				$("#alertNotificationHead").addClass('alertContainerActive');
                $(".alertsDisplayToggle").fadeIn('slow');	
			}
		}
		if(data.UPDATE_FIRMWARE=='n'){
			if(alert_fw==1){
				$("#alertNotificationHead").addClass('alertContainerActive');
                $(".alertsDisplayToggle").fadeIn('slow');	
			}	
		}
		if(data.FULL_CAP=="1"){
			$("input#cap_cb").attr('checked','checked');
			$(".alertContainer:first").hide();
			
		}else{
			if(alert_cap==1){
				$("#alertNotificationHead").addClass('alertContainerActive');
                $(".alertsDisplayToggle").fadeIn('slow');	
			}	
		}
		
		if(data.UPDATE_FIRMWARE=="1"){
			$(".alertContainer:last").hide();
			$("input#fiemwire_cb").attr('checked','checked');	
				
		}else{
			if(alert_fw==1){
				$("#alertNotificationHead").addClass('alertContainerActive');
                $(".alertsDisplayToggle").fadeIn('slow');	
			}	
		}
		
		
	});
			
		
		
	});
	
		
	
	
	
}

function dataRemind(){
	
	
	
	
	$("#cap_cb").live('click',
	   function(){
		  if($(this).is(":checked") ){
			
			$.post('DB/alerts.php',{"cap":1},function(){
													  
			})
		  }else{
			$.post('DB/alerts.php',{"cap":0},function(){
													  
			}) 
			}
	   }
	);
	$("#fiemwire_cb").live('click',
	   function(){
		  if($(this).is(":checked") ){
			
			$.post('DB/alerts.php',{"frimware":1},function(){
													  
			})
		  }else{
			$.post('DB/alerts.php',{"frimware":0},function(){
													  
			}) 
			}
	   }
	);
}

$(function(){
	alertGetJson();
	dataRemind();
   // $(".alertContainer:first").addClass('alertContainerFirst');
  //  $(".alertContainer:last").addClass('alertContainerLast'); 
    $(".alert_notify").click(
        function(event){
            if($(".alertsDisplayToggle").is(":visible")){
                $("#alertNotificationHead").removeClass('alertContainerActive');
                $(".alertsDisplayToggle").hide();
            }else{
			//clearTimeout(alertTimer);
                $("#alertNotificationHead").addClass('alertContainerActive');
                $(".alertsDisplayToggle").fadeIn('slow');
				
				if($("#cap_hd").val()==1){
					$(".alertContainer:first").show();	
				}
				if($("#fiemwire_hd").val()==1){
					$(".alertContainer:last").show();	
				}
				
            }
        }
    );
    $(".alertClose").live('click',
        function(event){
            c = $(this).closest('.alertContainer')
            $(c).fadeOut('fast',
                function(){ 
                    $(c).remove(); 
                    setAlertContainerDisplay();
                }
            );
        }
    );	
});
