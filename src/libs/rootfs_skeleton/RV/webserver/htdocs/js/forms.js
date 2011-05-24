function initFormButtons(){
	$('.toggle_save_buttons:not(.visible)').hide();
	$('.reset').click(function(event){
							   formID = $(this).attr('rel');
							   $('#'+formID)[0].reset();
							   $('#'+formID).find('select').selectmenu('destroy');
							   initToggledDivs();
							   hideSaveButtons(formID);
							   initSelectMenus($('#'+formID).find('select:not(#hdd_standby_value)'));
							   initCheckboxes();
                               initRadios();
							   initSliders();
							}
						);
	
	$('.submit').click(function(event){
							   formID = $(this).attr('rel');
							   saveFormData(formID);
					   		}
						);
}

function initFormInputs(){
	var $tmpFormValue = '';
	$('input[type=text]').focus(function(event){
							   $tmpFormValue = $(this).val();
					   		}
						);
	
	$('input[type=text]').bind('blur keyup', function(event){
							   curValue = $(this).val();
							   if(curValue != $tmpFormValue){
							   		formID = this.form.id;
								  	showSaveButtons(formID);
							   }
						   }
						);
	
	$('input[type=radio], input[type=checkbox]').click(function(event){
								if(!this.form){
									return;
								}
								formID = this.form.id;
								showSaveButtons(formID);
					   		}
						);
	
	initSelectMenus($('select:not(#hdd_standby_value)'));
	
}

function initSelectMenus(selector){
	selector.each(function(){
						   w = 225;
						   setW = $(this).css('width');
						   if(setW != 'auto'){
							w = setW;
						   }
						   $(this).selectmenu({width:w, maxHeight:150, style:'dropdown', change:function(){
																		  formID = this.form.id;
																		  showSaveButtons(formID);
																		  }});
						   });
}

function initCheckboxes(){
	if ($.browser.msie && $.browser.version.substr(0,1)<8) {
		//return;
	}
	$("input[type='checkbox']:not(.default)").checkBox('reflectUI'); 
	$("input[type='checkbox']:not(.default)").checkBox(); 
}

function initRadios(){
	$("input[type='radio']:not(.default)").checkBox('reflectUI'); 
	$("input[type='radio']:not(.default)").checkBox(); 
}


function initToggledDivs(){
	$("input[type='checkbox']:not(.default)").unbind('click');
	$("input[type='checkbox']:not(.default)").each(function(){
											if(this.checked == true){
												$('div[rel='+this.id+']').css('display', 'block');
											}
											else{
												$('div[rel='+this.id+']').css('display', 'none');
											}
										  
										  	$(this).bind('click', function(){
																	if($(this).is(':checked')){
																		$('div[rel='+this.id+']').show();
																	}
																	else{
																		$('div[rel='+this.id+']').hide();
																	}
																	if(!this.form){
																		return;
																	}
																	formID = this.form.id;
																	showSaveButtons(formID);
																	});
										  }
									);
}

function showSaveButtons(formID){
	$('div[rel='+formID+'].processing_indicator').fadeOut('fast');
	$('div[rel='+formID+'].success_indicator').fadeOut('fast');
	$('div[rel='+formID+'].toggle_save_buttons').fadeIn('fast');
	$('div[rel='+formID+'].toggle_save_buttons').addClass('pending_save');
	$('div[rel='+formID+'].toggle_save_buttons').addClass('visible');
}

function hideSaveButtons(formID){
	$('div[rel='+formID+'].toggle_save_buttons').fadeOut('fast');
	$('div[rel='+formID+'].toggle_save_buttons').removeClass('pending_save');
	$('div[rel='+formID+'].toggle_save_buttons').removeClass('visible');
}

function showProcessing(formID){
	$('div[rel='+formID+'].success_indicator').fadeOut('fast');
	$('div[rel='+formID+'].processing_indicator').fadeIn('fast');
}

function doSuccess(formID){
	$('div[rel='+formID+'].toggle_save_buttons').fadeOut('fast');
	$('div[rel='+formID+'].processing_indicator').fadeOut('fast');
	$('div[rel='+formID+'].success_indicator').fadeIn('fast');
	$('div[rel='+formID+'].success_indicator').fadeOut(8000);
}


function saveFormData(formID){
	hideSaveButtons(formID);
	showProcessing(formID);
	$.get("/tests/ajax_save/"+formID, null, function(data){
													 doSuccess(data.formID);
													 },
													 'json');
}
