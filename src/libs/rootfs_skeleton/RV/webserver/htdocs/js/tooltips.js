function initTooltips(){
	
	$(".helpLink, .doTooltip, #Launch_twonkey_btn").mbTooltip({
    opacity : .90, //opacity
    wait:400, //before show
    ancor:"mouse", //"parent"
    cssClass:"default", // default = default
    timePerWord:70, //time to show in milliseconds per word
    hasArrow:false,
    color:"#0e5c8b",
    imgPath:"tooltip_images/",
    shadowColor:"black",
    fade:500
  });
	
	$(".left_nav_item").mbTooltip({
    opacity : .90, //opacity
    wait:1000, //before show
    ancor:"mouse", //"parent"
    cssClass:"default", // default = default
    timePerWord:70, //time to show in milliseconds per word
    hasArrow:false,
    color:"#0e5c8b",
    imgPath:"tooltip_images/",
    shadowColor:"black",
    fade:500
  });
	
	$(".helpLink, .doTooltip").css('cursor', 'pointer');
}
