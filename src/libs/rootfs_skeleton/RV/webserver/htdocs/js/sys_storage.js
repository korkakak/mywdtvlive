var storageAry;
function storageGetJson(){
	//alert('a');
	var RandomSeq=useround(0,10000);
	$.getJSON('cgi-bin/getServerValue.cgi',{"id":"2","randomSeq":RandomSeq},function(data){
	//$.getJSON('json/storage.json',function(data){
		
		//alert(data.storage.length);
		$("#lang4").hide();
		
		for(i=0;i<data.Storage.length;i++){
			
			
			if(data.Storage[i].LocalStroge=='local storage'){
			
				$("<option value="+i+">"+$("#lang4").text()+"</option>").appendTo($("#LocalStorageSelect")); 
			}else{
				$("<option value="+i+">"+data.Storage[i].LocalStroge+"</option>").appendTo($("#LocalStorageSelect")); 	
			}
			
			
			storageAry=data.Storage;
			
			
			
		}
		showStorageData(0);
		$("select#LocalStorageSelect").selectmenu({width:245, maxHeight:150, style:'dropdown',change:function(){
			showStorageData($(this).val());																						  
		}});
		
	})
	
}
function showStorageData(value){
	//alert(value);
	$("#lang5").hide();
	$('#Volume').text(storageAry[value].Volume);
	$('#Usage').text(storageAry[value].Used+" / "+storageAry[value].Total);
	if(storageAry[value].Status=="Good"){
		$('#Status').text($("#lang5").text());	
	}else{
		$('#Status').text(storageAry[value].Status);	
	}
	
	
}

