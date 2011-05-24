

function initCustomAccordions(){
	$('.accordion').attr({ 'role':'tablist', 'class':'accordion ui-accordion ui-widget ui-helper-reset' });
	
	$('.accordion_toggle').unbind('click');
	$('.accordion_toggle').addClass('ui-accordion-header ui-helper-reset ui-state-default ui-corner-all');
	$('.accordion_toggle').each(function(){
									$(this).attr({'tabindex':'0', 'aria-expanded':'false', 'role':'tab'});
									$(this).append('<span class="ui-icon ui-icon-triangle-1-e"></span>');
									$(this).next().addClass('accordion_content ui-accordion-content ui-helper-reset ui-widget-content ui-corner-bottom');
									$(this).next().attr({'role':'tabpanel', 'style':'display:none'});
									$(this).children('a').attr( {'tabindex':'-1' });
									}
								);
	$('.accordion_toggle:not(.always_open)').click(function() {
		$(this).next().slideToggle('fast');
		$(this).toggleClass('ui-state-active');
		$(this).toggleClass('ui-corner-all');
		$(this).toggleClass('ui-corner-top');
		$(this).children('span').toggleClass('ui-icon-triangle-1-s');
		return false;
	}).next().hide();
	
	$('.accordion_toggle').each(function(){
										 if($(this).hasClass('default_open')){
										 	$(this).trigger('click');
										 }
									}
								);
	
	openActivePanels();
}

function initAccordionContentRows(){
	$('.accordion_toggle').each(function(){
										 var count = 0;
										 $(this).next().find('.alternating_row').each(function(){
																					   row = (count%2);
																					   $(this).addClass('alternating_row_'+row);
																					   count++;
																					   });
										 });
}


function openActivePanels(){
	$('.ui-state-active').next().slideDown();
	$('.ui-state-active').removeClass('ui-corner-all');
	$('.ui-state-active').addClass('ui-corner-top');
	$('.ui-state-active').css('display', 'block');
	$('.ui-state-active').children('span').addClass('ui-icon-triangle-1-s');
}

function initUIComponents(){
	// Accordion
	//$(".accordion").accordion({ header: ".accordion_toggle", collapsible: true, active:false});
	 

	// Tabs
	
	
	if(!openTab){
/*	$(".tabs").tabs({
			fx: { height: 'toggle', opacity: 'toggle', duration: 'fast'},
			cache:true,
			select:function(event, ui) { $('#loader').dialog('open'); },
			show:function(event, ui) { initToggledDivs(); $('#loader').dialog('close');},
			load: function(event, ui) { switchTabCallback(event, ui); },
			ajaxOptions: {
				error: function(xhr, status, index, anchor) {
					$(anchor.hash).html("Couldn't load this tab.");
				},
				success: function(response){
					response = response.replace(/(^\s*|\s*$)/g, "");
					if(response == 'login'){
						window.location = '/login';
						return;
					}
				}
			}
		});
	}*/
	
	$(".tabs").tabs({
			selected: 0,
			fx: { height: 'toggle', opacity: 'toggle', duration: 'fast'},
			collapsible: true
		});
	}
	
	else{
		$(".tabs").tabs({
			selected: openTab,
			fx: { height: 'toggle', opacity: 'toggle', duration: 'fast'},
			collapsible: true
		});
		
	}
	$('.tabs').bind('tabsselect', function(event, ui) {
										   
		//alert(old_tab_id);
		if(old_tab_id==ui.index){
			event.preventDefault();	
		}
		
		
		old_tab_id=ui.index;
		
		
		
		
		
		
										   
		if(ui.tab.id=='remote_btn'){
			
			
			//$(".tabs").tabs({selected: 0,collapsible: true});
			//window.location="wdtvlivehub/remote/";
			//window.parent.location.href="wdtvlivehub/remote/";
			
			window.open("wdtvlivehub/remote/","_blank");
			//window.open("wdtvlivehub/remote/", "_parent");
		//	alert('a');
		}else{
			
			
			//return old_tab_id;
			//alert($(ui.tab))
			//$(ui.tab).click();
			
			
		}
		
		return old_tab_id;
						
	});
};

function initSliders(){
	if($('#hdd_standby_value').attr('id') == undefined){
		return;
	}
	else{	
		$('.ui-slider').remove();
		$('#hdd_standby_value').selectToUISlider({sliderOptions:{change:function(){showSaveButtons('system_energy_saver');}}}).hide();
		$('.ui-slider-handle').live('click', function(event){event.preventDefault(); alert('clicked');});
	}
}

function initUI(){
	initCustomAccordions();
	initUIComponents();
	initFormButtons();
	initFormInputs();
	initCheckboxes();
	initRadios();
	initAccordionContentRows();
	initToggledDivs();
	initSliders();
	//initScrollbars();
	initTooltips();
}

$(document).ready(function(){
	//initUI();
});
