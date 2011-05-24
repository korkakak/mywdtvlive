var UserPW;
function initSelectMenus(selector){
	selector.each(function(){
						   w = 225;
						   setW = $(this).css('width');
						   if(setW != 'auto'){
							w = setW;
						   }
						   $(this).selectmenu({width:w, maxHeight:150, style:'dropdown', change:function(){
							
							
								
								$.cookie('user_pw', $("input#PW").val()); 
								
								$.cookie('language', $(this).val()); 
							
								window.location="index.php";
								
														
																		  }});
						   });
}

function initCheckboxes(){
	$("input[type='checkbox']").checkBox('reflectUI'); 
	$("input[type='checkbox']").checkBox(); 
}



function initTosLinks(){
   $("a#login").click(
        function(event){
			
			//$('#content').hide();
			event.stopPropagation();
			
			//fromSubmit();
			
			var pw=$("input#PW").val();
			
			if(pw!=""){
				
				if($('input#checkbox').attr('checked')){
				
				
				
				$.post('DB/login.php',{"password":$("input#PW").val()}, function(data) {
					
				if(data=="yes"){
					
					
					if($.cookie('keepSign')==1){
						$.cookie('user_pw', $("input#PW").val(),{ expires: 365 });	
					}else{
						$.cookie('user_pw', null); 
					}
					
					 
					
					$('.error').hide();
					
					
					$.cookie('AgreeLicense', null);
					
					$('#eulaform').submit();
					
					
				}else{
					$('.error').show();
					$('.error').text($("#correct_password").text());
					
				}
					
				});
		
				
			
				}else{
					$('.error').show();
					$('.error').text($("#accept_agreement").text());
					}
				
				
			}else{
				$('.error').show();
				$('.error').text($("#no_password").text());
			}
			
						
            return false;
        }
    );
    $("#eulaShowLink").click(
        function(event){
            event.preventDefault();
				
            $("#eula_form").parent().fadeOut('fast',
                function(){ 
					
                    $("#eulaView").fadeIn('fast'); 
                    $("#eulaText").jScrollPane({wheelSpeed:100, showArrows:true });
					$(".jScrollPaneContainer").focus();
									
					
                }
            );
            return false;
        }
    );
    $("#eulaPrintLink").click(
        function(){
            $("#eulaText").print();
            return false;
			
        }
    );
	
	

   $("#eulaAgreeLink").live('click', function(){
		 ckd = this.id.match(/^.*Agree.*$/);
	
            $("#eula_form").find('input:checkbox').eq(0).attr('checked',ckd);
			$('#Agree_hd').val(1);
			$.cookie('AgreeLicense', $('#Agree_hd').val());
            $("input[type='checkbox']").checkBox('reflectUI'); 
			
			
			
            $("#eulaView").fadeOut('fast',
                function(){
					
                    $("#eula_form").parent().fadeIn('fast');
					$(".ui-selectmenu").focus();
					
                }
            );													   
	
	})
	
	
	$("#checkbox").live('click', function(){
		if($(this).is(":checked")){
			$('#Agree_hd').val(1);
			
		}else{
			$('#Agree_hd').val(0);
			
		}
		$.cookie('AgreeLicense', $('#Agree_hd').val());
	})
	$("#keepSign_cb").live('click', function(){
		if($(this).is(":checked")){
			$('#keepSign_hd').val(1);
			
		}else{
			$('#keepSign_hd').val(0);
			
		}
		$.cookie('keepSign', $('#keepSign_hd').val(),{ expires: 365 });
	})
	
	
}

$(document).ready(function(){
						   

	//$("a#login").button();

	
	initCheckboxes();
	$('.error').hide();
	if($.cookie('user_pw')!=null){
		$("input#PW").val($.cookie('user_pw'));
	}
	
	$("input#PW").focus();
	
	
	$("input#PW").keydown(function(e){
		if(e.keyCode==13){
				
			$("a#login").focus();
			$("a#login").click();	
		}
				
	})
	
	
    initTosLinks();
	
	
	$('#alertNotification').hide();
	
	if($.cookie('keepSign')==1){
		$("#eula_form").find('input:checkbox').eq(1).attr('checked',true);
		 $("input[type='checkbox']").checkBox('reflectUI');
			
	}else{
		$("#eula_form").find('input:checkbox').eq(1).attr('checked',false);
		$("input[type='checkbox']").checkBox('reflectUI'); 
		
	}
	
	if($.cookie('AgreeLicense')==1){
		$("#eula_form").find('input:checkbox').eq(0).attr('checked',true);
		 $("input[type='checkbox']").checkBox('reflectUI'); 
	}else{
		$("#eula_form").find('input:checkbox').eq(0).attr('checked',false);
		 $("input[type='checkbox']").checkBox('reflectUI'); 		
	}
	
		
		
	$("#LangSelect option[value="+$.cookie('language')+"]").attr('selected', 'selected');	
	
	initSelectMenus($('select'));
	$("a#eula_nbsp").focus(function(){
		$('.ui-checkbox').eq(0).addClass('ui-radio-state-checked-focus');	
		
								   
	})
	$("a#eula_nbsp_2").focus(function(){
		$('.ui-checkbox').eq(1).addClass('ui-radio-state-checked-focus');	
		
								   
	})
	$(".ui-selectmenu").focus(function(){
		$('.ui-checkbox').removeClass('ui-radio-state-checked-focus');									
	})
	$("a#eulaShowLink").focus(function(){
		$('.ui-checkbox').removeClass('ui-radio-state-checked-focus');	
	
	})
	
	$("a#login").focus(function(){
		$('.ui-checkbox').removeClass('ui-radio-state-checked-focus');							
		})
	
	$('a#eula_nbsp').keydown(function(event){
		if(event.keyCode==32){
			
			
			$("#eula_form").find('input:checkbox').eq(0).checkBox('toggle'); 
			
			
			
			if($('#Agree_hd').val()==0){
				$('#Agree_hd').val(1);
		
			}else{
				$('#Agree_hd').val(0);
				$("input[type='checkbox']").checkBox('reflectUI'); 		
			}
			$.cookie('AgreeLicense',$('#Agree_hd').val());
			
		}								 
	})
	
	$('a#eula_nbsp_2').keydown(function(event){
		if(event.keyCode==32){
			
			
			$("#eula_form").find('input:checkbox').eq(1).checkBox('toggle'); 
			
		}								 
	})
	
	  
});
