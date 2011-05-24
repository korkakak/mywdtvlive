/* 
==== jQuery Plugin - Fields To Json Ver 0.5 ====
** by Jeffrey Lee, http://blog.darkthread.net
Revision:
Ver 0.5 Beta 2009-08-23 
 - Convert all input, select and textarea to a JSON string
 - Restore all input, select and textarea from a JSON string
*/
 
(function($) {
    $.fn.fieldsToJson = function(prefix) {
        if (!JSON || !JSON.stringify) {
           // alert("JSON is required!");
            return "";
        }
        var obj = {};
        this.find("input,select,textarea")
        .each(function() {
            if (!this.id) return; //Skip no id attribute
            var fn = this.id;
            //Filtered by prefix
            if (prefix && fn.indexOf(prefix) == -1) return;
            //Remove prefix
            if (prefix) fn = fn.substr(prefix.length, fn.length - prefix.length);
            var val = this.value + ""; //Avoid IE8 JSON bug
            if (this.type == "checkbox" || this.type == "radio")
                val = this.checked;
            else if (this.type == "select-one")
                val = this.selectedIndex;
            else if (this.type == "select-multiple") {
                var selected = [];
                $(this).children().each(function(i) {
                    if (this.selected) selected.push(i);
                });
                val = selected.join(",");
            }
            obj[fn] = val;
        });
       // alert(JSON.stringify(obj));
        return JSON.stringify(obj);
    }
    $.fn.jsonToFields = function(jsonString, prefix) {
        if (!JSON || !JSON.parse) {
           // alert("JSON is required!");
            return "";
        }
        var obj = JSON.parse(jsonString);
        this.find("input,select,textarea")
        .each(function() {
            if (!this.id) return; //Skip no id attribute
            var fn = this.id;
            //Filtered by prefix
            if (prefix && fn.indexOf(prefix) == -1) return;
            //Remove prefix
            if (prefix) fn = fn.substr(prefix.length, fn.length - prefix.length);
            if (obj[fn] == undefined) return;
            var val = obj[fn];
            if (this.type == "checkbox" || this.type == "radio")
                this.checked = val;
            else if (this.type == "select-one")
                this.selectedIndex = val;
            else if (this.type == "select-multiple") {
                var selected = val.split(",");
                var options = $(this).children();
                options.filter(":selected").removeAttr("selected");
                for (var i = 0; i < selected.length; i++) {
                    var opt = options[selected[i]];
                    if (opt) opt.selected = true;
                }
            }
            else this.value = val;
        });
    }
})(jQuery);// JavaScript Document