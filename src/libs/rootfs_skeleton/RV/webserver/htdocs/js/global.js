var navDocked = false;
var postLoadCallback = null;
var fromLearningCenter = false;
var openTab = false;
var slideNav = false;

var old_tab_id=0;

function debug(obj, indent) {
	if (typeof(obj) == typeof('')) return '[String] : ' + obj;
	if (indent == undefined) indent = 0;
	var out = '';
	var tabs  = '';
	for (var i = 0; i <= indent; i++) {
		tabs += "\t";
	}
	for (itm in obj){
		if (Object.prototype.toString.call(obj[itm]) === '[object Object]') {
			out += tabs + itm + " {\n";
			out += tabs + debug(obj[itm], indent+1);
			out += tabs + "}\n";
		} else {
			out += tabs + itm + ": " + obj[itm] + "\n";
		}
	}
	return out;
}

function initLogo(){
	$('#logo').css('position','absolute');
	var cw = $('#header_content').width();
	var nw = $('#left').width();
	var middle = (cw/2);
	var nx = middle - (nw/2);
	$('#logo').css('left',nx+'px');
}

function initNav(){
	$('#left').css('position','absolute');
	var cw = $('#container').width();
	var nw = $('#left').width();
	var middle = (cw/2);
	var nx = middle - (nw/2);
	$('#left').css('left',nx+'px');
	$('#left').fadeIn('slow');
	$('.ajax_nav').click(function(event){
		if(!navDocked){
			slideMainNav();
			slideLogo();
			
		}
		event.preventDefault();
		$('.ajax_nav').removeClass('active');
		$(this).addClass('active');
		url = $(this).attr('href');
		
		old_tab_id=0;
		loadAjaxContent(url);
		
		
		
	});
	
	$('.reset_nav').live('click', function(){
										   $('.ajax_nav').removeClass('active');	
										   });
	if(slideNav){
		slideMainNav();
		slideLogo();
	}
}

function slideMainNav(){
	$('#left').animate({ 'left':'0px' }, 500, function(){
			$('#footer').fadeIn('fast');
			$('#header_content').fadeIn('fast');
			$('#alertNotification').fadeIn('fast');
		 });
	/*alertTimer = setTimeout(function (){
		$('.alert_notify').trigger('click');								  
		},3000);*/
	navDocked = true;
	initFooter();
}

function slideLogo(){
	$('#logo').animate({ 'left':'0px' }, 500);
}

function initAjaxLinks(){
	$('.ajax').unbind('click');
	$('.ajax').live('click', function(event){
						   	event.preventDefault();
						   	url = $(this).attr('href');
						   	loadAjaxContent(url);
						}
					);
			   
}

function initContentContainer(){
	var wh = $(window).height();
	var hh = $('#header').height();
	var nh = wh - hh - 40;
	$('#content_scroll').height(nh);
	$('#content_scroll').css('overflow', 'auto');
	//setPanelHeights(nh);
}

/*function setPanelHeights(nh){
	minHeight = 550;
	maxHeight = 550;
	labelHeight = 36;
	containerH = Math.max(minHeight, Math.min(nh, maxHeight));
	hOffset = 85;
	$('.paneled_content').height(containerH);
	$('.interior_panel').height(Math.min(containerH-hOffset));
	$('#users_list').height(containerH-hOffset-labelHeight-12);
	initScrollbars();
}*/

function initScrollbars(){
	$('.scrollable').css('overflow', 'auto');
	//$('.scrollable').jScrollPane({wheelSpeed:100, showArrows:true });
}

function loadAjaxContentCallback(response, status){
	//response = response.replace(/(^\s*|\s*$)/g, "");
	if(response == 'redirect'){
		window.location = SITE_BASE_URL;
		return;
	}
	setTimeout("showLoadedContent();", 400);	
	initUI();
	
}

function showLoadedContent(){
	initContentContainer();
	$("#content_scroll").show('slide', {callback:null}, null, function(){initScrollbars(); $('.ui-tabs-nav').css('visibility', 'visible');});
	$('#loader').dialog('close');
	//
    if(postLoadCallback !== null){
        setTimeout(postLoadCallback,700);
        postLoadCallback = null;
    }

	$('.ui-tabs-nav').children('.ui-state-default').eq($('.ui-tabs-nav').children('.ui-state-default').length-1).children('a').removeClass('has_separator');
	
	
}

function loadAjaxContent(url){
	if(!navDocked){
		slideMainNav();
		slideLogo();
		removeDialogTitlebars();
	}
	$('#loader').dialog('open');
	$('.ui-tabs-nav').css('visibility', 'hidden');
	$("#content_scroll").hide('slide', null, null, function(){ $("#content").load(url, null, loadAjaxContentCallback); });
	
	
}

function removeDialogTitlebars(){
	$('.dialog_no_titlebar').prev('.ui-dialog-titlebar').css('display', 'none');
}

function formatStorage(){
	$.get('toServerValue.cgi',{"Storage":"format"}, function(data) {
		$("#confirm_delete_user").dialog('close');										
	})
}

/* show/hide functionality for the help text users and shares */
$(function() {
    $(".learningHelpQuestion").live('click',
        function(){
			answer = $(this).next('.learningHelpAnswer');
			answer.parent().siblings().children('.learningHelpAnswer').slideUp('fast');
			answer.slideToggle('fast');
        }
    );
});
$(function(){
	//須與form表單ID名稱相同
	$.validator.addMethod("noSpecialChars", function(value, element) {
      return this.optional(element) || /^[a-z0-9]+$/i.test(value);
  }, "");
	$("#sys_device").validate({
    invalidHandler: function(form, validator) {
        $.each(validator.invalid,function(key,value){
			
			showError('HasSpecialChars');
            return false;
        }); 
    },
    errorPlacement:function(error, element) {
    },
    onkeyup: false,
    onfocusout:false,
	onsubmit: false
  

});
	

});

$(document).ready(function(){				   
	initUI();
	initNav();
	initLogo();
	initAjaxLinks();
	initContentContainer();
	removeDialogTitlebars();
	initTooltips();
	$('.ui-tabs-nav').css('visibility', 'visible');
	
	

	
	
});

$(window).resize(function(){
						  
	initContentContainer();
	
});
