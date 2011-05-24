function showError(ids){
    $(".generic_error_list li").removeClass('error_visible').hide();
    // check for the error code first
    if (ids == undefined) ids = 'unk';
    if (typeof(ids) === typeof(200)) ids = ids.toString();
    if(!$.isArray(ids) && typeof(ids) === typeof('')){
        // it's a string. or, at least, it had better be
        tmp = ids;
        ids = new Array(tmp);
    }
    for (var i = 0; i < ids.length; i++) {
        id = "#error_"+ids[i];
        $(id).addClass('error_visible').show();
    };
    if(!$(".generic_error_list").children('li.error_visible').length){
        $("#error_unk").show(); // none of the errors have been displayed, show the default
    }
    $("#generic_error_overlay").dialog('open');
}

