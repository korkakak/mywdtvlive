var usage=[];
function initFooter(){
	hideFooter();
	bindFooterMouseover();
	bindFooterMouseout();
}

function bindFooterMouseover(){
	$('#footer').bind('mouseenter',
					function(){
					   	showFooter();
					}
				);
}

function bindFooterMouseout(){
	$('#footer').bind('mouseleave',
					function(){
					   	hideFooter();
					}
				);
}

function showFooter(){
	
	
	
	pos = $('#footer').css('bottom');
	if(pos != '-47px'){
		return;
	}
	
	$('#all_usage_label').animate({ 'top':'21px'}, 200, function(){
																 $('#usage_chart_wrap').animate({ 'right':'1px'}, 200, function(){
																			$('#indicator_label_wrap').slideDown(200);
																			$('#footer').animate({'bottom':'0px'}, 200, function(){
																																 $('#device_display_status').fadeIn(200);
																																});
																			$('#device_graphic').animate({'top':'5px'}, 200);
																			});
																 });
	
	$('#device_display_name').animate({'left':'65px'}, 200, function(){
																	$('#device_display_name').animate({'top':'15px'}, 200);
																	});
	
	$('#device_display_name').addClass('open');
	$('#all_usage_indicator').fadeOut();
	
	for(var i=0; i<usage.length; i++){
		
		$('#usage_chart .'+usage[i].name).show();
	}
	
	
}

function hideFooter(){
	pos = $('#footer').css('bottom');
	if(pos != '0px'){
		return;
	}
	$('#footer').animate({'bottom':'-47px'}, 200, function(){
														   	$('#indicator_label_wrap').slideUp(200);
															$('#usage_chart_wrap').animate({ 'right':'133px'}, 200, function(){
																															 $('#all_usage_label').animate({ 'top':'1px'}, 200);
																															 });
															});
	$('#device_display_name').animate({'top':'5px'}, 200, function(){
																   $('#device_display_name').animate({'left':'87px'}, 200);
																   $('#device_display_status').fadeOut(200);
																   });
	$('#device_display_name').removeClass('open');
	//$('#device_graphic').animate({'top':'-10px'}, 200);
	$('#all_usage_indicator').fadeIn();
	
	//$('#usage_chart .music').css('width', 0+'px');
			//$('#usage_chart .music').css('left', 0+'px');
	for(var i=0; i<usage.length; i++){
		
		$('#usage_chart .'+usage[i].name).hide();
	}
}



function initFooterChart(){
	
	
	$("#lang68").hide();
$.getJSON('cgi-bin/getServerValue.cgi?id=6',function(data){
	//	$.getJSON('json/id_6_storage.json',function(data){
		
		var k=0;
		
		$('#footer').css('display', 'block');
		var chartW = $('#usage_chart').width();
		$('#footer').css('display', 'none');
		
		$.each(data, function(i,item)
		{
			//alert(i);
			if(k<4){
				
				usage.push({"name":i,"percentage":data[i]});
				
				
				
				
				k++;
				
				
			}
			
			if(i=="ONLINE"){
				$.cookie('online', item);
			}
			
			
		});
		//chartAry=usage;
		
		for(var i=0; i<usage.length; i++){
			blockW = (usage[i].percentage*chartW)/100;
			blockW = Math.floor(blockW);
			usage[i].width = blockW;
			$('#usage_chart .'+usage[i].name).width(blockW);
		}
		var left = 0;
		var totalWidth = 0;
		for(var j=0; j<usage.length; j++){
			$('#usage_chart .'+usage[j].name).css('left', left+'px');
			left += usage[j].width+1;
			totalWidth += left;
		}
		
		var AllblockW = (data.Used_percent*chartW)/100;
		
		$('#usage_chart .all').css('width', AllblockW+'px');
		
		//alert(data.Used);
		$('#all_usage_label').append('<span>'+data.Used+" / "+data.Total+'</span>');
		
		if(data.Status=="Good"){
			$('#device_display_status').append('<span class="status_good">'+$("#lang68").text()+'</span>');
			$('#device_graphic').addClass('good');
		}else{
			$('#device_display_status').append('<span class="status_bad">'+data.Status+'</span>');
			$('#device_graphic').addClass('bad');
		}
		
		$('#footer').fadeIn();
		showFooter();
		
	})
	
	
	
	
}

$(document).ready(function(){
	
	initFooterChart();
	//$('#footer').fadeIn();
	//showFooter();
});