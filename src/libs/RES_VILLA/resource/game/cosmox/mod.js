
var MK_ALT = "18";
var MK_ENTER = "13";
var MK_LEFT = "37";
var MK_UP = "38";
var MK_RIGHT = "39";
var MK_DOWN = "40";
var MK_0 = "48";
var MK_1 = "49";
var MK_2 = "50";
var MK_3 = "51";
var MK_4 = "52";
var MK_5 = "53";
var MK_6 = "54";
var MK_7 = "55";
var MK_8 = "56";
var MK_9 = "57";
var MK_ASTERISK = "42";
var MK_POUND = "35";
var MK_PREVIOUS_PAGE = "112";
var MK_HOME_PAGE = "104";
var MK_PAGE_PLUS = "pagePlus";
var MK_PAGE_MINUS = "pageMinus";
var MK_BLUE = "blue";
var MK_RED = "red";
var MK_YELLOW = "yellow";
var MK_GREEN = "green";
var MK_PURPLE = "purple";
var MK_ORANGE = "orange";
var MK_BACKSPACE = "backspace";
var MK_MUTE = "mute";
var MK_HELP = "help";
var MK_DELETE = "delete";
var MK_BOOKMARK = "bookmark";
var MK_STOP = "stop";
var MK_PAUSE = "pause";
var MK_PLAY = "play";
var MK_REWIND = "rewind";
var MK_FAST_FORWARD = "fastForward";
var MK_VOLUME_UP = "volumeUp";
var MK_VOLUME_DOWN = "volumeDown";
var MK_INFO = "info";
//
var isAlt = false;
var AmcCtrl;
//
function onKeyPress201 (evt)
{
	var __keyCode = evt.which;
	switch (__keyCode)
	{
		case 1073741838 :
		kp (MK_UP);
		break;
		case 1073741839 :
		kp (MK_DOWN);
		break;
		case 1073741840 :
		kp (MK_LEFT);
		break;
		case 1073741841 :
		kp (MK_RIGHT);
		break;
		case 1073741828 :
		kp (MK_ENTER);
		break;
		case 48 :
		case 49 :
		case 50 :
		case 51 :
		case 52 :
		case 53 :
		case 54 :
		case 55 :
		case 56 :
		case 57 :
		//MK_0
		kp (String (__keyCode));
		break;
		case 1342177296 :
		kp (MK_PREVIOUS_PAGE);
		break;
		case 8 :
		kp (MK_HOME_PAGE);
		break;
		case 1073741837 :
		kp (MK_PAGE_PLUS);
		break;
		case 1073741836 :
		kp (MK_PAGE_MINUS);
		break;
		case 99 :
		kp (MK_RED);
		break;
		case 100 :
		kp (MK_GREEN);
		break;
		case 101 :
		kp (MK_PURPLE);
		break;
		case 103 :
		kp (MK_YELLOW);
		break;
		case 106 :
		kp (MK_BLUE);
		break;
		case 110 :
		kp (MK_ORANGE);
		break;
		case 104 :
		kp (MK_HELP);
		break;
		case 115 :
		kp (MK_MUTE);
		break;
		case 1073741827 :
		kp (MK_BACKSPACE);
		break;
		case 1073741833 :
		kp (MK_DELETE);
		break;
		case 111 :
		kp (MK_BOOKMARK);
		break;
		case 98 :
		kp (MK_REWIND);
		break;
		case 102 :
		kp (MK_FAST_FORWARD);
		break;
		case 116 :
		kp (MK_STOP);
		break;
		case 112 :
		kp (MK_PAUSE);
		break;
		case 114 :
		kp (MK_PLAY);
		break;
		case 1073741850 :
		kp (MK_VOLUME_UP);
		break;
		case 1073741849 :
		kp (MK_VOLUME_DOWN);
		break;
		case 121 :
		kp (MK_INFO);
		break;
		default :
		kp (String (__keyCode));
		break;
	}
	return false;
}
function onKeyPress202 (evt)
{
	var __keyCode = evt.keyCode ? evt.keyCode : evt.charCode;
	/*
	if(evt.keyCode){
	kp("keyCode");
	}else if(evt.charCode){
	kp("charCode");
	}else if(evt.which){
	kp("which");
	}
	*/
	if ( ! isAlt)
	{
		switch (__keyCode)
		{
			case 37 :
			kp (MK_LEFT);
			break;
			case 38 :
			kp (MK_UP);
			break;
			case 39 :
			kp (MK_RIGHT);
			break;
			case 40 :
			kp (MK_DOWN);
			break;
			case 13 :
			kp (MK_ENTER);
			break;
			case 48 :
			case 49 :
			case 50 :
			case 51 :
			case 52 :
			case 53 :
			case 54 :
			case 55 :
			case 56 :
			case 57 :
			kp (String (__keyCode));
			break;
			case 112 :
			kp (MK_PREVIOUS_PAGE);
			break;
			case 104 :
			kp (MK_HOME_PAGE);
			break;
			case 18 :
			isAlt = true;
			break;
			case 33 :
			kp (MK_PAGE_PLUS);
			break;
			case 34 :
			kp (MK_PAGE_MINUS);
			break;
			case 122 :
			kp (MK_HELP);
			break;
			case 109 :
			kp (MK_MUTE);
			break;
			case 8 :
			kp (MK_BACKSPACE);
			break;
			case 99 :
			kp (MK_DELETE);
			break;
			case 97 :
			kp (MK_BOOKMARK);
			break;
			case 98 :
			kp (MK_REWIND);
			break;
			case 102 :
			kp (MK_FAST_FORWARD);
			break;
			case 113 :
			kp (MK_STOP);
			break;
			case 111 :
			kp (MK_PAUSE);
			break;
			case 118 :
			kp (MK_PLAY);
			break;
			case 43 :
			kp (MK_VOLUME_UP);
			break;
			case 45 :
			kp (MK_VOLUME_DOWN);
			break;
			case 114 :
			kp (MK_INFO);
			break;
			default :
			kp (String (__keyCode));
			//return true;
		}
		return false;
	} else
	{
		isAlt = false;
		switch (__keyCode)
		{
			case 114 :
			kp (MK_RED);
			break;
			case 103 :
			kp (MK_GREEN);
			break;
			case 121 :
			kp (MK_YELLOW);
			break;
			case 98 :
			kp (MK_BLUE);
			break;
			case 112 :
			kp (MK_PURPLE);
			break;
			case 111 :
			kp (MK_ORANGE);
			break;
			default :
			kp ("alt:" + String (__keyCode));
			//return true;
		}
		return false;
	}
}
function onKeyRelease201 (evt)
{
  alert("release:"+evt.which);
}
function onKeyRelease202 (evt)
{
  alert("release:"+evt.which);
}
function onKeyPressHook (evt)
{
}
function onKeyReleaseHook (evt)
{
}
function initKey ()
{
	if (navigator.userAgent.indexOf ("WinCE") > - 1)
	{
		onKeyPressHook = onKeyPress201;
		onKeyReleaseHook = onKeyRelease201;
		AmcCtrl=new AmcCtrlPlugin();
		//AmcCtrl.InitMedia(0x000000);
		AmcCtrl.GamePad_Open();
	} else
	{
		onKeyPressHook = onKeyPress202;
		onKeyReleaseHook = onKeyRelease202;
	}
}
function deInitKey ()
{
	if (navigator.userAgent.indexOf ("WinCE") > - 1)
	{
		AmcCtrl.GamePad_Close();
		//AmcCtrl.UnInitMedia();	
		AmcCtrl=null;
	} 
}
