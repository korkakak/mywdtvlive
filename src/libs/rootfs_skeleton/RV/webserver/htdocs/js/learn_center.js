var LCItems = [];
var LCCount = 0;
var LCDuration = 10000;
var LCCycleStatus = 'started';
var LCMaxHeight = 0;
var LCTimeout = null;

var haShow=null;



$(function(){

	$('#learn_center_ul').hide();
	$('#learn_center_ul li').each(function(){
										   LCItem = '<div class="learn_center_item">'+$(this).html()+'</div>';
										   $(LCItem).css('cursor', 'pointer');
										   $('#learn_center_ul').parent().append(LCItem);
										   LCItems.push(LCItem);
										   });
	
	setLCContainerHeight();
	createLCItemNavigation();
	startLC();
	
	$('#learn_center_toggle').css('cursor', 'pointer');
	$('#learn_center_toggle').click(function(){
										 
										 $(this).toggleClass('closed');
										 $('#learn_center_content').slideToggle();
										 });
    RegisterGetJson();
	initLearnCenterLinks();
	
});

function setLCContainerHeight(){
	
	$('.learn_center_item').each(function(){
										 LCMaxHeight = Math.max(LCMaxHeight, $(this).height());
										 });
	$('#learn_center_content').css('height', (LCMaxHeight+32)+'px');
	$('#learn_center_navigation').css('top', (LCMaxHeight+25)+'px');
	
}

function startLC(){
	if(LCCycleStatus != 'started'){
		return;
	}
	if(LCCount == LCItems.length){
		LCCount = 0;
	}
	showLCItem(LCCount);
	LCCount++;
	LCTimeout = setTimeout('startLC();', LCDuration);
}

function createLCItemNavigation(){
	LCPrev = '<div class="learn_center_nav_prev"></div>';
	$('#learn_center_navigation').append(LCPrev);
	for(var i=0; i<LCItems.length; i++){
		LCLink = '<div rel="'+i+'" class="learn_center_nav_item"></div>';
		$('#learn_center_navigation').append(LCLink);
	}
	$('.learn_center_nav_item').css('cursor', 'pointer');
	$('.learn_center_nav_item').click(function(){
											   	showLCItemAndStop($(this).attr('rel'));
											   });
	LCNext = '<div class="learn_center_nav_next"></div>';
	$('#learn_center_navigation').append(LCNext);
	
	$('.learn_center_nav_next').click(function(){
											   	showNextLCItem();
											   });
	$('.learn_center_nav_prev').click(function(){
											   	showPrevLCItem();
											   });
	
}

function showLCItemAndStop(index){
	LCCycleStatus = 'stopped';
	showLCItem(index);
}

function showNextLCItem(){
	LCCycleStatus = 'stopped';
	setCurrentLCCount();
	LCCount++;
	if(LCCount >= LCItems.length){
		LCCount = 0;
	}
	setTimeout("showLCItem(LCCount);", 300);
}


function showPrevLCItem(){
	LCCycleStatus = 'stopped';
	foo = setCurrentLCCount();
	LCCount--;
	if(LCCount < 0){
		LCCount = LCItems.length-1;
	}
	setTimeout("showLCItem(LCCount);", 300);
}

function setCurrentLCCount(){
	$('.learn_center_nav_item').each(function(i){
											  if($(this).hasClass('active')){
											  	LCCount = i;
												return;
											  }
											  });
}

function showLCItem(index){
	$('.learn_center_item').hide();
	$('.learn_center_nav_item').removeClass('active');
	$('.learn_center_nav_item:eq('+index+')').addClass('active');
	$('.learn_center_item:eq('+index+')').fadeIn('slow');
	
}

var register_obj={};
var register_json={};

function processRegistration(){
	var search_str = /^[\w\-\.]+@[\w\-\.]+(\.\w+)+$/;
	var email_val = $("#email").val();
	$("f_register").val($("#register_device_form").serializeArray());
	
	register_obj.frist_name=$('input#frist_name').val();
	register_obj.last_name=$('input#last_name').val();
	register_obj.email=$('input#email').val();
	register_obj.divice_sn=$('#PageSn').val();
	
	
	register_obj.keepInfo=$("#keepInfo").val();
	register_json.register=register_obj;
	
	
	var thing=$.toJSON(register_json);
	
	
	
	if($('input#frist_name').val()!='' && $('input#last_name').val()!='' && $('input#email').val()!='' && $('input#confirm_email').val()!=''){
		if(search_str.test(email_val)){
			
			if($('input#email').val()==$('input#confirm_email').val()){
				
			
				$.ajax({type:'POST',
					   	url:'cgi-bin/toServerValue.cgi',
						// url:'DB/dd.php',
						data:thing,
						beforeSend:function(){
							$("#loader").dialog("open");	
						},
						complete:function(data){
							$("#loader").dialog("close");
							 
							if(data=="success"){
									$("#register_device").dialog("close");
									$('#register_confirmation_success').dialog("open");	
								}else if(data=="nosuccess"){
									$('#register_confirmation_Nosuccess').dialog("open");
								}else if(data=="pending"){
									$('#register_confirmation_Pending').dialog("open");
								
								}else{
									$('#register_confirmation_Nosuccess').dialog("open");	
								}
						},
						success:function(data){
							
								if(data=="success"){
									$("#register_device").dialog("close");
									$('#register_confirmation_success').dialog("open");	
								}else if(data=="nosuccess"){
									$('#register_confirmation_Nosuccess').dialog("open");
								}else if(data=="pending"){
									$('#register_confirmation_Pending').dialog("open");
								
								}else{
									$('#register_confirmation_Nosuccess').dialog("open");	
								}
						},
						error:function(xhr, ajaxOptions, thrownError){
							//alert(xhr.status);
                    		showError(xhr.status);
						}
				})
				
						
				
			}else{
				showError('userConfirmEmail');	
			}
			
		}else{
			
			showError('email');
		}
		
		
	}else{
		showError('Required');
	}
	
}

function toJsonCallback(){
	if(data=="success"){
		$("#register_device").dialog("close");
		$('#register_confirmation_success').dialog("open");	
	}else if(data=="nosuccess"){
			$('#register_confirmation_Nosuccess').dialog("open");
	}else{
		$('#register_confirmation_Pending').dialog("open");
	}
		
}

function RegisterGetJson(){
	//$("#register_device_form").validate();
//$.getJSON('json/Register.json',function(data){
											
		$.getJSON('cgi-bin/getServerValue.cgi?id=10',function(data){
		//$.getJSON('json/Register.json',function(data){
		
		$("#divice_sn").text(data.device_sn);
		$("input#PageSn").val(data.device_sn);
		haShow=data.register;
	})
	$("#keepInfo_cb").live('click', function(event){
		
		if( $(this).is(":checked") ){
			
			$("#keepInfo").val(1);
		}else{
			$("#keepInfo").val(0)
		}
		
	})
	
	
	
	
	
	
	
}

function initLearnCenterLinks(){
	$('.learn_center_item').css('cursor', 'pointer');
	
	$("#learn_backup").live('click', // load the backups section
        function(event){
            event.stopPropagation();
            $("#backups_nav").click();
            return false;
        }
    );
    $("#learn_register").live('click', // load the MioNet tab in Settings
        function(event){
			openTab = 2;
           $("#System_nav").click();
            postLoadCallback = function(){
				
				if(haShow!="1"){
					$("#register_device").dialog("open");	
				}
								
				
			};
            event.stopPropagation();
            return false;
        }
    );
    $("#learn_remote").live('click', // load the MioNet tab in Settings
        function(event){
            event.stopPropagation();
           // 
            fromLearningCenter = true;
			$("#network_nav").click();
            // need to load MioNet tab here
            return false;
        }
    );
    $("#learn_photos").live('click', // load the MioNet tab in Settings
         function(event){
            event.stopPropagation();
           // openTab = 3;
            fromLearningCenter = true;
			$("#media_nav").click();
            return false;
        }
    );
    $("#learn_users").live('click', // load the MioNet tab in Settings
        function(event){
            event.stopPropagation();
			openTab = 3;
           $("#media_nav").click();
            return false;
        }
    );
    $("#learn_shares").live('click', // load the MioNet tab in Settings
        function(event){
            event.stopPropagation();
            fromLearningCenter = true;
            postLoadCallback = function(){
                $("a#create_share_btn").click();
            };
            $("#shares_nav").click();
            return false;
        }
    );
}

/* pause cycling when mouse is over the learn center content 
 * This doesn't completely work yet;
 * It automatically advances the cycle on hover, which isn't right
 * */
/*$(function(){#0094EC
    $("#learn_center_content").hover(
        function(){
            LCCycleStatus = 'stopped';
        },
        function(){
            LCCycleStatus = 'started';
            clearTimeout(LCTimeout);
            startLC();
        }
    );
});*/
