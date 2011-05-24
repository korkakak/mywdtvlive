function toJson(value){
	//alert(value);
	//alert($('#f_remote').val())
	$('#f_remote').val(value)
	$.ajax({type:'POST',url:'cgi-bin/toServerValue.cgi',data:$("#remote_from").fieldsToJson("f_") })
}




