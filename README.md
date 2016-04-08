<!DOCTYPE html>
<html ng-strict-di="" lang="en"><head>
<meta http-equiv="content-type" content="text/html; charset=UTF-8"><style type="text/css">@charset "UTF-8";[ng\:cloak],[ng-cloak],[data-ng-cloak],[x-ng-cloak],.ng-cloak,.x-ng-cloak,.ng-hide:not(.ng-hide-animate){display:none !important;}ng\:form{display:block;}.ng-animate-shim{visibility:hidden;}.ng-anchor{position:absolute;}</style>
  <meta charset="UTF-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">

  <title>Online Markdown Editor - Dillinger, the Last Markdown Editor ever.</title>

  <meta name="description" content="Dillinger is an online cloud based HTML5 filled Markdown Editor. Sync with Dropbox, Github and Google Drive. 100% Open Source!">
  <meta name="keywords" content="Markdown, Dillinger, Editor, ACE, Github, Open Source, Node.js">
  <meta name="author" content="Joe McCann and Martin Broder">

  <link rel="apple-touch-icon" href="http://dillinger.io/apple-touch-icon.png">
  <!--

  Powered by:

  '||''''|
   ||  .   ... ...... ...... ..   ....  ....  ....
   ||''|    '|..'  ||'  ||||' ''.|...||||. ' ||. '
   ||        .|.   ||    |||    ||     . '|... '|..
  .||.....|.|  ||. ||...'.||.    '|...'|'..|'|'..|'
                   ||
                   ||

  using node.js version 4.3.2

  -->

  <!-- Spiders must use meta description -->
  <meta name="robots" content="noodp, noydir">

  <!-- Google Verfication -->
  <meta name="google-site-verification" content="DAyGOgtsg8rJpq9VVktKzDkQ1UhXm1FYl8SD47hPkjA">

  <!-- Viewport and mobile -->
  <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no, maximum-scale=1, minimum-scale=1">
  <meta name="HandheldFriendly" content="true">
  <meta name="MobileOptimized" content="320">
  <meta http-equiv="cleartype" content="on">

  <link href="README_files/css_002.css" rel="stylesheet" type="text/css">
  <link href="README_files/css.css" rel="stylesheet" type="text/css">
  <link href="README_files/app.css" rel="stylesheet">



<style id="ace_editor">.ace_editor {	position: relative;	overflow: hidden;	font: 12px/normal 'Monaco', 'Menlo', 'Ubuntu Mono', 'Consolas', 'source-code-pro', monospace;	direction: ltr;	}	.ace_scroller {	position: absolute;	overflow: hidden;	top: 0;	bottom: 0;	background-color: inherit;	-ms-user-select: none;	-moz-user-select: none;	-webkit-user-select: none;	user-select: none;	cursor: text;	}	.ace_content {	position: absolute;	-moz-box-sizing: border-box;	-webkit-box-sizing: border-box;	box-sizing: border-box;	min-width: 100%;	}	.ace_dragging .ace_scroller:before{	position: absolute;	top: 0;	left: 0;	right: 0;	bottom: 0;	content: '';	background: rgba(250, 250, 250, 0.01);	z-index: 1000;	}	.ace_dragging.ace_dark .ace_scroller:before{	background: rgba(0, 0, 0, 0.01);	}	.ace_selecting, .ace_selecting * {	cursor: text !important;	}	.ace_gutter {	position: absolute;	overflow : hidden;	width: auto;	top: 0;	bottom: 0;	left: 0;	cursor: default;	z-index: 4;	-ms-user-select: none;	-moz-user-select: none;	-webkit-user-select: none;	user-select: none;	}	.ace_gutter-active-line {	position: absolute;	left: 0;	right: 0;	}	.ace_scroller.ace_scroll-left {	box-shadow: 17px 0 16px -16px rgba(0, 0, 0, 0.4) inset;	}	.ace_gutter-cell {	padding-left: 19px;	padding-right: 6px;	background-repeat: no-repeat;	}	.ace_gutter-cell.ace_error {	background-image: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABOFBMVEX/////////QRswFAb/Ui4wFAYwFAYwFAaWGAfDRymzOSH/PxswFAb/SiUwFAYwFAbUPRvjQiDllog5HhHdRybsTi3/Tyv9Tir+Syj/UC3////XurebMBIwFAb/RSHbPx/gUzfdwL3kzMivKBAwFAbbvbnhPx66NhowFAYwFAaZJg8wFAaxKBDZurf/RB6mMxb/SCMwFAYwFAbxQB3+RB4wFAb/Qhy4Oh+4QifbNRcwFAYwFAYwFAb/QRzdNhgwFAYwFAbav7v/Uy7oaE68MBK5LxLewr/r2NXewLswFAaxJw4wFAbkPRy2PyYwFAaxKhLm1tMwFAazPiQwFAaUGAb/QBrfOx3bvrv/VC/maE4wFAbRPBq6MRO8Qynew8Dp2tjfwb0wFAbx6eju5+by6uns4uH9/f36+vr/GkHjAAAAYnRSTlMAGt+64rnWu/bo8eAA4InH3+DwoN7j4eLi4xP99Nfg4+b+/u9B/eDs1MD1mO7+4PHg2MXa347g7vDizMLN4eG+Pv7i5evs/v79yu7S3/DV7/498Yv24eH+4ufQ3Ozu/v7+y13sRqwAAADLSURBVHjaZc/XDsFgGIBhtDrshlitmk2IrbHFqL2pvXf/+78DPokj7+Fz9qpU/9UXJIlhmPaTaQ6QPaz0mm+5gwkgovcV6GZzd5JtCQwgsxoHOvJO15kleRLAnMgHFIESUEPmawB9ngmelTtipwwfASilxOLyiV5UVUyVAfbG0cCPHig+GBkzAENHS0AstVF6bacZIOzgLmxsHbt2OecNgJC83JERmePUYq8ARGkJx6XtFsdddBQgZE2nPR6CICZhawjA4Fb/chv+399kfR+MMMDGOQAAAABJRU5ErkJggg==");	background-repeat: no-repeat;	background-position: 2px center;	}	.ace_gutter-cell.ace_warning {	background-image: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAAmVBMVEX///8AAAD///8AAAAAAABPSzb/5sAAAAB/blH/73z/ulkAAAAAAAD85pkAAAAAAAACAgP/vGz/rkDerGbGrV7/pkQICAf////e0IsAAAD/oED/qTvhrnUAAAD/yHD/njcAAADuv2r/nz//oTj/p064oGf/zHAAAAA9Nir/tFIAAAD/tlTiuWf/tkIAAACynXEAAAAAAAAtIRW7zBpBAAAAM3RSTlMAABR1m7RXO8Ln31Z36zT+neXe5OzooRDfn+TZ4p3h2hTf4t3k3ucyrN1K5+Xaks52Sfs9CXgrAAAAjklEQVR42o3PbQ+CIBQFYEwboPhSYgoYunIqqLn6/z8uYdH8Vmdnu9vz4WwXgN/xTPRD2+sgOcZjsge/whXZgUaYYvT8QnuJaUrjrHUQreGczuEafQCO/SJTufTbroWsPgsllVhq3wJEk2jUSzX3CUEDJC84707djRc5MTAQxoLgupWRwW6UB5fS++NV8AbOZgnsC7BpEAAAAABJRU5ErkJggg==");	background-position: 2px center;	}	.ace_gutter-cell.ace_info {	background-image: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAAAAAA6mKC9AAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAAAJ0Uk5TAAB2k804AAAAPklEQVQY02NgIB68QuO3tiLznjAwpKTgNyDbMegwisCHZUETUZV0ZqOquBpXj2rtnpSJT1AEnnRmL2OgGgAAIKkRQap2htgAAAAASUVORK5CYII=");	background-position: 2px center;	}	.ace_dark .ace_gutter-cell.ace_info {	background-image: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQBAMAAADt3eJSAAAAJFBMVEUAAAChoaGAgIAqKiq+vr6tra1ZWVmUlJSbm5s8PDxubm56enrdgzg3AAAAAXRSTlMAQObYZgAAAClJREFUeNpjYMAPdsMYHegyJZFQBlsUlMFVCWUYKkAZMxZAGdxlDMQBAG+TBP4B6RyJAAAAAElFTkSuQmCC");	}	.ace_scrollbar {	position: absolute;	right: 0;	bottom: 0;	z-index: 6;	}	.ace_scrollbar-inner {	position: absolute;	cursor: text;	left: 0;	top: 0;	}	.ace_scrollbar-v{	overflow-x: hidden;	overflow-y: scroll;	top: 0;	}	.ace_scrollbar-h {	overflow-x: scroll;	overflow-y: hidden;	left: 0;	}	.ace_print-margin {	position: absolute;	height: 100%;	}	.ace_text-input {	position: absolute;	z-index: 0;	width: 0.5em;	height: 1em;	opacity: 0;	background: transparent;	-moz-appearance: none;	appearance: none;	border: none;	resize: none;	outline: none;	overflow: hidden;	font: inherit;	padding: 0 1px;	margin: 0 -1px;	text-indent: -1em;	-ms-user-select: text;	-moz-user-select: text;	-webkit-user-select: text;	user-select: text;	}	.ace_text-input.ace_composition {	background: inherit;	color: inherit;	z-index: 1000;	opacity: 1;	text-indent: 0;	}	.ace_layer {	z-index: 1;	position: absolute;	overflow: hidden;	white-space: pre;	height: 100%;	width: 100%;	-moz-box-sizing: border-box;	-webkit-box-sizing: border-box;	box-sizing: border-box;	pointer-events: none;	}	.ace_gutter-layer {	position: relative;	width: auto;	text-align: right;	pointer-events: auto;	}	.ace_text-layer {	font: inherit !important;	}	.ace_cjk {	display: inline-block;	text-align: center;	}	.ace_cursor-layer {	z-index: 4;	}	.ace_cursor {	z-index: 4;	position: absolute;	-moz-box-sizing: border-box;	-webkit-box-sizing: border-box;	box-sizing: border-box;	border-left: 2px solid	}	.ace_slim-cursors .ace_cursor {	border-left-width: 1px;	}	.ace_overwrite-cursors .ace_cursor {	border-left-width: 0;	border-bottom: 1px solid;	}	.ace_hidden-cursors .ace_cursor {	opacity: 0.2;	}	.ace_smooth-blinking .ace_cursor {	-webkit-transition: opacity 0.18s;	transition: opacity 0.18s;	}	.ace_editor.ace_multiselect .ace_cursor {	border-left-width: 1px;	}	.ace_marker-layer .ace_step, .ace_marker-layer .ace_stack {	position: absolute;	z-index: 3;	}	.ace_marker-layer .ace_selection {	position: absolute;	z-index: 5;	}	.ace_marker-layer .ace_bracket {	position: absolute;	z-index: 6;	}	.ace_marker-layer .ace_active-line {	position: absolute;	z-index: 2;	}	.ace_marker-layer .ace_selected-word {	position: absolute;	z-index: 4;	-moz-box-sizing: border-box;	-webkit-box-sizing: border-box;	box-sizing: border-box;	}	.ace_line .ace_fold {	-moz-box-sizing: border-box;	-webkit-box-sizing: border-box;	box-sizing: border-box;	display: inline-block;	height: 11px;	margin-top: -2px;	vertical-align: middle;	background-image:	url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABEAAAAJCAYAAADU6McMAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAAJpJREFUeNpi/P//PwOlgAXGYGRklAVSokD8GmjwY1wasKljQpYACtpCFeADcHVQfQyMQAwzwAZI3wJKvCLkfKBaMSClBlR7BOQikCFGQEErIH0VqkabiGCAqwUadAzZJRxQr/0gwiXIal8zQQPnNVTgJ1TdawL0T5gBIP1MUJNhBv2HKoQHHjqNrA4WO4zY0glyNKLT2KIfIMAAQsdgGiXvgnYAAAAASUVORK5CYII="),	url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAA3CAYAAADNNiA5AAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAACJJREFUeNpi+P//fxgTAwPDBxDxD078RSX+YeEyDFMCIMAAI3INmXiwf2YAAAAASUVORK5CYII=");	background-repeat: no-repeat, repeat-x;	background-position: center center, top left;	color: transparent;	border: 1px solid black;	border-radius: 2px;	cursor: pointer;	pointer-events: auto;	}	.ace_dark .ace_fold {	}	.ace_fold:hover{	background-image:	url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABEAAAAJCAYAAADU6McMAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAAJpJREFUeNpi/P//PwOlgAXGYGRklAVSokD8GmjwY1wasKljQpYACtpCFeADcHVQfQyMQAwzwAZI3wJKvCLkfKBaMSClBlR7BOQikCFGQEErIH0VqkabiGCAqwUadAzZJRxQr/0gwiXIal8zQQPnNVTgJ1TdawL0T5gBIP1MUJNhBv2HKoQHHjqNrA4WO4zY0glyNKLT2KIfIMAAQsdgGiXvgnYAAAAASUVORK5CYII="),	url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAA3CAYAAADNNiA5AAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAACBJREFUeNpi+P//fz4TAwPDZxDxD5X4i5fLMEwJgAADAEPVDbjNw87ZAAAAAElFTkSuQmCC");	}	.ace_tooltip {	background-color: #FFF;	background-image: -webkit-linear-gradient(top, transparent, rgba(0, 0, 0, 0.1));	background-image: linear-gradient(to bottom, transparent, rgba(0, 0, 0, 0.1));	border: 1px solid gray;	border-radius: 1px;	box-shadow: 0 1px 2px rgba(0, 0, 0, 0.3);	color: black;	max-width: 100%;	padding: 3px 4px;	position: fixed;	z-index: 999999;	-moz-box-sizing: border-box;	-webkit-box-sizing: border-box;	box-sizing: border-box;	cursor: default;	white-space: pre;	word-wrap: break-word;	line-height: normal;	font-style: normal;	font-weight: normal;	letter-spacing: normal;	pointer-events: none;	}	.ace_folding-enabled > .ace_gutter-cell {	padding-right: 13px;	}	.ace_fold-widget {	-moz-box-sizing: border-box;	-webkit-box-sizing: border-box;	box-sizing: border-box;	margin: 0 -12px 0 1px;	display: none;	width: 11px;	vertical-align: top;	background-image: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAUAAAAFCAYAAACNbyblAAAANElEQVR42mWKsQ0AMAzC8ixLlrzQjzmBiEjp0A6WwBCSPgKAXoLkqSot7nN3yMwR7pZ32NzpKkVoDBUxKAAAAABJRU5ErkJggg==");	background-repeat: no-repeat;	background-position: center;	border-radius: 3px;	border: 1px solid transparent;	cursor: pointer;	}	.ace_folding-enabled .ace_fold-widget {	display: inline-block;   	}	.ace_fold-widget.ace_end {	background-image: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAUAAAAFCAYAAACNbyblAAAANElEQVR42m3HwQkAMAhD0YzsRchFKI7sAikeWkrxwScEB0nh5e7KTPWimZki4tYfVbX+MNl4pyZXejUO1QAAAABJRU5ErkJggg==");	}	.ace_fold-widget.ace_closed {	background-image: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAMAAAAGCAYAAAAG5SQMAAAAOUlEQVR42jXKwQkAMAgDwKwqKD4EwQ26sSOkVWjgIIHAzPiCgaqiqnJHZnKICBERHN194O5b9vbLuAVRL+l0YWnZAAAAAElFTkSuQmCCXA==");	}	.ace_fold-widget:hover {	border: 1px solid rgba(0, 0, 0, 0.3);	background-color: rgba(255, 255, 255, 0.2);	box-shadow: 0 1px 1px rgba(255, 255, 255, 0.7);	}	.ace_fold-widget:active {	border: 1px solid rgba(0, 0, 0, 0.4);	background-color: rgba(0, 0, 0, 0.05);	box-shadow: 0 1px 1px rgba(255, 255, 255, 0.8);	}	.ace_dark .ace_fold-widget {	background-image: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAUAAAAFCAYAAACNbyblAAAAHklEQVQIW2P4//8/AzoGEQ7oGCaLLAhWiSwB146BAQCSTPYocqT0AAAAAElFTkSuQmCC");	}	.ace_dark .ace_fold-widget.ace_end {	background-image: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAUAAAAFCAYAAACNbyblAAAAH0lEQVQIW2P4//8/AxQ7wNjIAjDMgC4AxjCVKBirIAAF0kz2rlhxpAAAAABJRU5ErkJggg==");	}	.ace_dark .ace_fold-widget.ace_closed {	background-image: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAMAAAAFCAYAAACAcVaiAAAAHElEQVQIW2P4//+/AxAzgDADlOOAznHAKgPWAwARji8UIDTfQQAAAABJRU5ErkJggg==");	}	.ace_dark .ace_fold-widget:hover {	box-shadow: 0 1px 1px rgba(255, 255, 255, 0.2);	background-color: rgba(255, 255, 255, 0.1);	}	.ace_dark .ace_fold-widget:active {	box-shadow: 0 1px 1px rgba(255, 255, 255, 0.2);	}	.ace_fold-widget.ace_invalid {	background-color: #FFB4B4;	border-color: #DE5555;	}	.ace_fade-fold-widgets .ace_fold-widget {	-webkit-transition: opacity 0.4s ease 0.05s;	transition: opacity 0.4s ease 0.05s;	opacity: 0;	}	.ace_fade-fold-widgets:hover .ace_fold-widget {	-webkit-transition: opacity 0.05s ease 0.05s;	transition: opacity 0.05s ease 0.05s;	opacity:1;	}	.ace_underline {	text-decoration: underline;	}	.ace_bold {	font-weight: bold;	}	.ace_nobold .ace_bold {	font-weight: normal;	}	.ace_italic {	font-style: italic;	}	.ace_error-marker {	background-color: rgba(255, 0, 0,0.2);	position: absolute;	z-index: 9;	}	.ace_highlight-marker {	background-color: rgba(255, 255, 0,0.2);	position: absolute;	z-index: 8;	}	</style><style id="ace-tm">.ace-tm .ace_gutter {	background: #f0f0f0;	color: #333;	}	.ace-tm .ace_print-margin {	width: 1px;	background: #e8e8e8;	}	.ace-tm .ace_fold {	background-color: #6B72E6;	}	.ace-tm {	background-color: #FFFFFF;	color: black;	}	.ace-tm .ace_cursor {	color: black;	}	.ace-tm .ace_invisible {	color: rgb(191, 191, 191);	}	.ace-tm .ace_storage,	.ace-tm .ace_keyword {	color: blue;	}	.ace-tm .ace_constant {	color: rgb(197, 6, 11);	}	.ace-tm .ace_constant.ace_buildin {	color: rgb(88, 72, 246);	}	.ace-tm .ace_constant.ace_language {	color: rgb(88, 92, 246);	}	.ace-tm .ace_constant.ace_library {	color: rgb(6, 150, 14);	}	.ace-tm .ace_invalid {	background-color: rgba(255, 0, 0, 0.1);	color: red;	}	.ace-tm .ace_support.ace_function {	color: rgb(60, 76, 114);	}	.ace-tm .ace_support.ace_constant {	color: rgb(6, 150, 14);	}	.ace-tm .ace_support.ace_type,	.ace-tm .ace_support.ace_class {	color: rgb(109, 121, 222);	}	.ace-tm .ace_keyword.ace_operator {	color: rgb(104, 118, 135);	}	.ace-tm .ace_string {	color: rgb(3, 106, 7);	}	.ace-tm .ace_comment {	color: rgb(76, 136, 107);	}	.ace-tm .ace_comment.ace_doc {	color: rgb(0, 102, 255);	}	.ace-tm .ace_comment.ace_doc.ace_tag {	color: rgb(128, 159, 191);	}	.ace-tm .ace_constant.ace_numeric {	color: rgb(0, 0, 205);	}	.ace-tm .ace_variable {	color: rgb(49, 132, 149);	}	.ace-tm .ace_xml-pe {	color: rgb(104, 104, 91);	}	.ace-tm .ace_entity.ace_name.ace_function {	color: #0000A2;	}	.ace-tm .ace_heading {	color: rgb(12, 7, 255);	}	.ace-tm .ace_list {	color:rgb(185, 6, 144);	}	.ace-tm .ace_meta.ace_tag {	color:rgb(0, 22, 142);	}	.ace-tm .ace_string.ace_regex {	color: rgb(255, 0, 0)	}	.ace-tm .ace_marker-layer .ace_selection {	background: rgb(181, 213, 255);	}	.ace-tm.ace_multiselect .ace_selection.ace_start {	box-shadow: 0 0 3px 0px white;	border-radius: 2px;	}	.ace-tm .ace_marker-layer .ace_step {	background: rgb(252, 255, 0);	}	.ace-tm .ace_marker-layer .ace_stack {	background: rgb(164, 229, 101);	}	.ace-tm .ace_marker-layer .ace_bracket {	margin: -1px 0 0 -1px;	border: 1px solid rgb(192, 192, 192);	}	.ace-tm .ace_marker-layer .ace_active-line {	background: rgba(0, 0, 0, 0.07);	}	.ace-tm .ace_gutter-active-line {	background-color : #dcdcdc;	}	.ace-tm .ace_marker-layer .ace_selected-word {	background: rgb(250, 250, 255);	border: 1px solid rgb(200, 200, 250);	}	.ace-tm .ace_indent-guide {	background: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAACCAYAAACZgbYnAAAAE0lEQVQImWP4////f4bLly//BwAmVgd1/w11/gAAAABJRU5ErkJggg==") right repeat-y;	}	</style><style>	    .error_widget_wrapper {	        background: inherit;	        color: inherit;	        border:none	    }	    .error_widget {	        border-top: solid 2px;	        border-bottom: solid 2px;	        margin: 5px 0;	        padding: 10px 40px;	        white-space: pre-wrap;	    }	    .error_widget.ace_error, .error_widget_arrow.ace_error{	        border-color: #ff5a5a	    }	    .error_widget.ace_warning, .error_widget_arrow.ace_warning{	        border-color: #F1D817	    }	    .error_widget.ace_info, .error_widget_arrow.ace_info{	        border-color: #5a5a5a	    }	    .error_widget.ace_ok, .error_widget_arrow.ace_ok{	        border-color: #5aaa5a	    }	    .error_widget_arrow {	        position: absolute;	        border: solid 5px;	        border-top-color: transparent!important;	        border-right-color: transparent!important;	        border-left-color: transparent!important;	        top: -5px;	    }	</style><style id="ace-dillinger">.ace-dillinger .ace_gutter {	background: #F5F7FA;	color: #A0AABF;	overflow : hidden;	border-right: 1px solid #D3DAEA;	}	.ace-dillinger .ace_print-margin {	width: 1px;	background: #e8e8e8;	}	.ace-dillinger .ace_strong,	.ace-dillinger .ace_constant,	.ace-dillinger .ace_heading {	font-weight: 600;	}	.ace-dillinger .ace_markup.ace_heading {	font-weight: 400;	}	.ace-dillinger .ace_emphasis,	.ace-dillinger .ace_list {	font-style: italic;	}	.ace-dillinger .ace_markup.ace_list {	font-style: normal;	}	.ace-dillinger {	background-color: #FFFFFF;	color: black;	}	.ace-dillinger .ace_cursor {	color: black;	}	.ace-dillinger .ace_invisible {	color: rgb(191, 191, 191);	}	.ace-dillinger .ace_constant.ace_buildin {	color: rgb(88, 72, 246);	}	.ace-dillinger .ace_constant.ace_language {	color: rgb(88, 92, 246);	}	.ace-dillinger .ace_constant.ace_library {	color: rgb(6, 150, 14);	}	.ace-dillinger .ace_invalid {	background-color: rgb(153, 0, 0);	color: white;	}	.ace-dillinger .ace_fold {	}	.ace-dillinger .ace_support.ace_function {	color: rgb(60, 76, 114);	}	.ace-dillinger .ace_support.ace_constant {	color: rgb(6, 150, 14);	}	.ace-dillinger .ace_support.ace_type,	.ace-dillinger .ace_support.ace_class	.ace-dillinger .ace_support.ace_other {	color: rgb(109, 121, 222);	}	.ace-dillinger .ace_variable.ace_parameter {	font-style:italic;	color:#FD971F;	}	.ace-dillinger .ace_keyword.ace_operator {	color: rgb(104, 118, 135);	}	.ace-dillinger .ace_comment {	color: #236e24;	}	.ace-dillinger .ace_comment.ace_doc {	color: #236e24;	}	.ace-dillinger .ace_comment.ace_doc.ace_tag {	color: #236e24;	}	.ace-dillinger .ace_constant.ace_numeric {	color: rgb(0, 0, 205);	}	.ace-dillinger .ace_variable {	color: rgb(49, 132, 149);	}	.ace-dillinger .ace_xml-pe {	color: rgb(104, 104, 91);	}	.ace-dillinger .ace_entity.ace_name.ace_function {	color: #0000A2;	}	.ace-dillinger .ace_heading {	}	.ace-dillinger .ace_marker-layer .ace_selection {	background: rgb(181, 213, 255);	}	.ace-dillinger .ace_marker-layer .ace_step {	background: rgb(252, 255, 0);	}	.ace-dillinger .ace_marker-layer .ace_stack {	background: rgb(164, 229, 101);	}	.ace-dillinger .ace_marker-layer .ace_bracket {	margin: -1px 0 0 -1px;	border: 1px solid rgb(192, 192, 192);	}	.ace-dillinger .ace_marker-layer .ace_active-line {	background: #F7FAFC;	}	.ace-dillinger .ace_gutter-active-line {	background-color : #E0E5EC;	}	.ace-dillinger .ace_marker-layer .ace_selected-word {	background: rgb(250, 250, 255);	border: 1px solid rgb(200, 200, 250);	}	.ace-dillinger .ace_storage,	.ace-dillinger .ace_keyword,	.ace-dillinger .ace_meta.ace_tag {	color: rgb(147, 15, 128);	}	.ace-dillinger .ace_string.ace_regex {	color: rgb(255, 0, 0)	}	.ace-dillinger .ace_entity.ace_other.ace_attribute-name {	color: #994409;	}	.ace-dillinger .ace_indent-guide {	background: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAACCAYAAACZgbYnAAAAE0lEQVQImWP4////f4bLly//BwAmVgd1/w11/gAAAABJRU5ErkJggg==") right repeat-y;	}	</style><script src="README_files/ext-searchbox.html"></script></head>
<body class="ng-scope" ng-controller="Base">

  <form class="ng-pristine ng-valid" method="POST" id="downloader">
    <input name="name" type="hidden">
    <input name="unmd" type="hidden">
    <input name="formatting" type="hidden">
  </form>

  


  <ul class="settings ng-scope" role="menu" ng-controller="User as user">
  <li>
    <a ng-click="user.toggleAutoSave($event)">
      <span class="has-checkbox">Auto Save</span>
      <span value="user.profile.enableAutoSave" class="toggle-auto-save switch ng-isolate-scope checked" ng-class="{checked: toggleValue}">
  <input checked="checked" class="ng-pristine ng-untouched ng-valid" ng-model="toggleValue" type="checkbox">
  <small></small>
</span>
    </a>
  </li>
  <li>
    <a ng-click="user.toggleWordsCount($event)">
      <span class="has-checkbox">Word Count</span>
      <span value="user.profile.enableWordsCount" class="toggle-word-count switch ng-isolate-scope checked" ng-class="{checked: toggleValue}">
  <input checked="checked" class="ng-pristine ng-untouched ng-valid" ng-model="toggleValue" type="checkbox">
  <small></small>
</span>
    </a>
  </li>
  <li>
    <a ng-click="user.toggleScrollSync($event)">
      <span class="has-checkbox">Scroll Sync</span>
      <span value="user.profile.enableScrollSync" class="toggle-scroll-sync switch ng-isolate-scope" ng-class="{checked: toggleValue}">
  <input class="ng-pristine ng-untouched ng-valid" ng-model="toggleValue" type="checkbox">
  <small></small>
</span>
    </a>
  </li>
  <!--
  <li>
    <a ng-click="user.toggleNightMode()">
      <span>Night Mode</span>
      <switch class="toggle-night-mode" value="user.profile.enableNightMode"></switch>
    </a>
  </li>
  -->
  <li>
    <a target="_blank" href="https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet">
      <span>Cheat Sheet</span>
    </a>
  </li>
  <li>
    <a class="toggle-wtf" ng-click="user.showAbout($event)">
      <span>WTF is Dillinger?</span>
    </a>
  </li>
  <li>
    <a class="toggle-reset" ng-click="user.resetProfile($event)">
      <span>Reset Profile</span>
    </a>
  </li>
</ul>


  <div class="wrapper">
    <div class="sidebar-wrapper">
  <div class="sidebar ng-scope" ng-controller="Documents as document">
    <h2 class="sidebar-branding">
      <svg viewBox="0 0 85 11" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g id="Page-1" stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g id="Desktop---Save-to" sketch:type="MSArtboardGroup" transform="translate(-92.000000, -58.000000)" fill="#FFFFFF">
            <g id="Navigation" sketch:type="MSLayerGroup" transform="translate(0.000000, 38.000000)">
                <g id="Menu-Item:-Branding" transform="translate(91.000000, 17.000000)" sketch:type="MSShapeGroup">
                    <path d="M1.17,13 L4.563,13 C7.423,13 9.399,11.011 9.399,8.45 L9.399,8.424 C9.399,5.863 7.423,3.9 4.563,3.9 L1.17,3.9 L1.17,13 Z M2.769,11.544 L2.769,5.356 L4.563,5.356 C6.474,5.356 7.722,6.669 7.722,8.45 L7.722,8.476 C7.722,10.257 6.474,11.544 4.563,11.544 L2.769,11.544 Z M13.047,13 L14.646,13 L14.646,3.9 L13.047,3.9 L13.047,13 Z M18.697,13 L25.08,13 L25.08,11.544 L20.296,11.544 L20.296,3.9 L18.697,3.9 L18.697,13 Z M28.364,13 L34.747,13 L34.747,11.544 L29.963,11.544 L29.963,3.9 L28.364,3.9 L28.364,13 Z M38.122,13 L39.721,13 L39.721,3.9 L38.122,3.9 L38.122,13 Z M43.772,13 L45.345,13 L45.345,6.526 L50.363,13 L51.702,13 L51.702,3.9 L50.129,3.9 L50.129,10.192 L45.254,3.9 L43.772,3.9 L43.772,13 Z M59.978,13.156 C61.59,13.156 62.877,12.506 63.774,11.739 L63.774,7.917 L59.9,7.917 L59.9,9.308 L62.227,9.308 L62.227,11.011 C61.668,11.427 60.888,11.7 60.03,11.7 C58.171,11.7 56.936,10.322 56.936,8.45 L56.936,8.424 C56.936,6.682 58.21,5.213 59.887,5.213 C61.044,5.213 61.733,5.59 62.435,6.188 L63.449,4.979 C62.513,4.186 61.538,3.744 59.952,3.744 C57.209,3.744 55.259,5.902 55.259,8.45 L55.259,8.476 C55.259,11.128 57.131,13.156 59.978,13.156 Z M67.474,13 L74.286,13 L74.286,11.57 L69.073,11.57 L69.073,9.126 L73.636,9.126 L73.636,7.696 L69.073,7.696 L69.073,5.33 L74.221,5.33 L74.221,3.9 L67.474,3.9 L67.474,13 Z M77.804,13 L79.403,13 L79.403,9.828 L81.405,9.828 L83.641,13 L85.526,13 L83.069,9.555 C84.343,9.191 85.24,8.294 85.24,6.799 L85.24,6.773 C85.24,5.98 84.967,5.304 84.486,4.81 C83.901,4.238 83.004,3.9 81.86,3.9 L77.804,3.9 L77.804,13 Z M79.403,8.411 L79.403,5.356 L81.73,5.356 C82.913,5.356 83.615,5.889 83.615,6.864 L83.615,6.89 C83.615,7.813 82.887,8.411 81.743,8.411 L79.403,8.411 Z" id="DILLINGER-2"></path>
                </g>
            </g>
        </g>
    </g>
</svg>

    </h2>

    <nav class="nav nav-sidebar">
      <ul class="menu menu-sidebar">

        <li class="menu-item menu-item--link-unlink in-sidebar">
          <a class="menu-link" ng-click="document.status.linkUnlink = !document.status.linkUnlink">
          <span>Services</span> <span class="caret"></span></a>

          <ul style="height: 0px;" class="sidebar-list collapse" collapse="document.status.linkUnlink">
            
<li>
  
    <a href="http://dillinger.io/redirect/dropbox" class="import-dropbox unlinked">Link with
  
    <span>Dropbox</span>
    <span class="icon icon-link"><svg viewBox="0 0 16 16" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g sketch:type="MSArtboardGroup" transform="translate(-238.000000, -212.000000)" fill="#FFFFFF">
            <g sketch:type="MSLayerGroup" transform="translate(0.000000, 65.000000)">
                <g transform="translate(0.000000, 75.000000)" sketch:type="MSShapeGroup">
                    <g transform="translate(32.000000, 72.000000)">
                        <g transform="translate(206.000000, 0.000000)">
                            <path d="M15.0962672,6.56194892 L10.8290766,10.8291395 C9.6502947,12.0079214 7.74066798,12.0079214 6.56188605,10.8291395 L5.13948919,9.40674263 L6.56188605,7.98434578 L7.98428291,9.40674263 C8.37721022,9.79869548 9.01472692,9.79966994 9.40667976,9.40674263 L13.6738703,5.13955206 C14.0658232,4.7471277 14.0658232,4.10910806 13.6738703,3.71665226 L12.2514735,2.2942554 C11.8595206,1.90230255 11.2210295,1.90230255 10.8290766,2.2942554 L9.30549312,3.8178389 C8.59919843,3.40231041 7.79665226,3.22401572 7.00196464,3.27754813 L9.40667976,0.871858546 C10.5854617,-0.306420432 12.4960629,-0.306420432 13.6738703,0.871858546 L15.0962672,2.2942554 C16.2750491,3.47256582 16.2750491,5.38366994 15.0962672,6.56194892 L15.0962672,6.56194892 Z M6.66209823,12.1513242 L5.13948919,13.6739332 C4.74656189,14.0668605 4.10904519,14.0658861 3.71709234,13.6739332 L2.29469548,12.2515363 C1.90176817,11.8595835 1.90176817,11.2220668 2.29469548,10.8291395 L6.56188605,6.56194892 C6.9538389,6.16999607 7.59233006,6.16999607 7.98428291,6.56194892 L9.40667976,7.98434578 L10.8290766,6.56194892 L9.40667976,5.13955206 C8.22789784,3.96127308 6.31827112,3.96127308 5.13948919,5.13955206 L0.872298625,9.40674263 C-0.306483301,10.5855246 -0.306483301,12.4961257 0.872298625,13.6739332 L2.29469548,15.0963301 C3.47250295,16.275112 5.38310413,16.275112 6.56188605,15.0963301 L8.96660118,12.6916149 C8.17191356,12.7446444 7.37131631,12.5658782 6.66209823,12.1513242 L6.66209823,12.1513242 Z" id="Shape"></path>
                        </g>
                    </g>
                </g>
            </g>
        </g>
    </g>
</svg>
</span>
  </a>
</li>



<li class="ng-scope" ng-controller="Github as github">
  
    <a class="import-github unlinked" ng-click="github.chooseScope()">Link with
  
    <span>Github</span>
    <span class="icon icon-link"><svg viewBox="0 0 16 16" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g sketch:type="MSArtboardGroup" transform="translate(-238.000000, -212.000000)" fill="#FFFFFF">
            <g sketch:type="MSLayerGroup" transform="translate(0.000000, 65.000000)">
                <g transform="translate(0.000000, 75.000000)" sketch:type="MSShapeGroup">
                    <g transform="translate(32.000000, 72.000000)">
                        <g transform="translate(206.000000, 0.000000)">
                            <path d="M15.0962672,6.56194892 L10.8290766,10.8291395 C9.6502947,12.0079214 7.74066798,12.0079214 6.56188605,10.8291395 L5.13948919,9.40674263 L6.56188605,7.98434578 L7.98428291,9.40674263 C8.37721022,9.79869548 9.01472692,9.79966994 9.40667976,9.40674263 L13.6738703,5.13955206 C14.0658232,4.7471277 14.0658232,4.10910806 13.6738703,3.71665226 L12.2514735,2.2942554 C11.8595206,1.90230255 11.2210295,1.90230255 10.8290766,2.2942554 L9.30549312,3.8178389 C8.59919843,3.40231041 7.79665226,3.22401572 7.00196464,3.27754813 L9.40667976,0.871858546 C10.5854617,-0.306420432 12.4960629,-0.306420432 13.6738703,0.871858546 L15.0962672,2.2942554 C16.2750491,3.47256582 16.2750491,5.38366994 15.0962672,6.56194892 L15.0962672,6.56194892 Z M6.66209823,12.1513242 L5.13948919,13.6739332 C4.74656189,14.0668605 4.10904519,14.0658861 3.71709234,13.6739332 L2.29469548,12.2515363 C1.90176817,11.8595835 1.90176817,11.2220668 2.29469548,10.8291395 L6.56188605,6.56194892 C6.9538389,6.16999607 7.59233006,6.16999607 7.98428291,6.56194892 L9.40667976,7.98434578 L10.8290766,6.56194892 L9.40667976,5.13955206 C8.22789784,3.96127308 6.31827112,3.96127308 5.13948919,5.13955206 L0.872298625,9.40674263 C-0.306483301,10.5855246 -0.306483301,12.4961257 0.872298625,13.6739332 L2.29469548,15.0963301 C3.47250295,16.275112 5.38310413,16.275112 6.56188605,15.0963301 L8.96660118,12.6916149 C8.17191356,12.7446444 7.37131631,12.5658782 6.66209823,12.1513242 L6.66209823,12.1513242 Z" id="Shape"></path>
                        </g>
                    </g>
                </g>
            </g>
        </g>
    </g>
</svg>
</span>
  </a>
</li>



<li>
  
    <a href="http://dillinger.io/redirect/googledrive" class="import-google-drive unlinked">Link with
  
    <span>Google Drive</span>
    <span class="icon icon-link"><svg viewBox="0 0 16 16" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g sketch:type="MSArtboardGroup" transform="translate(-238.000000, -212.000000)" fill="#FFFFFF">
            <g sketch:type="MSLayerGroup" transform="translate(0.000000, 65.000000)">
                <g transform="translate(0.000000, 75.000000)" sketch:type="MSShapeGroup">
                    <g transform="translate(32.000000, 72.000000)">
                        <g transform="translate(206.000000, 0.000000)">
                            <path d="M15.0962672,6.56194892 L10.8290766,10.8291395 C9.6502947,12.0079214 7.74066798,12.0079214 6.56188605,10.8291395 L5.13948919,9.40674263 L6.56188605,7.98434578 L7.98428291,9.40674263 C8.37721022,9.79869548 9.01472692,9.79966994 9.40667976,9.40674263 L13.6738703,5.13955206 C14.0658232,4.7471277 14.0658232,4.10910806 13.6738703,3.71665226 L12.2514735,2.2942554 C11.8595206,1.90230255 11.2210295,1.90230255 10.8290766,2.2942554 L9.30549312,3.8178389 C8.59919843,3.40231041 7.79665226,3.22401572 7.00196464,3.27754813 L9.40667976,0.871858546 C10.5854617,-0.306420432 12.4960629,-0.306420432 13.6738703,0.871858546 L15.0962672,2.2942554 C16.2750491,3.47256582 16.2750491,5.38366994 15.0962672,6.56194892 L15.0962672,6.56194892 Z M6.66209823,12.1513242 L5.13948919,13.6739332 C4.74656189,14.0668605 4.10904519,14.0658861 3.71709234,13.6739332 L2.29469548,12.2515363 C1.90176817,11.8595835 1.90176817,11.2220668 2.29469548,10.8291395 L6.56188605,6.56194892 C6.9538389,6.16999607 7.59233006,6.16999607 7.98428291,6.56194892 L9.40667976,7.98434578 L10.8290766,6.56194892 L9.40667976,5.13955206 C8.22789784,3.96127308 6.31827112,3.96127308 5.13948919,5.13955206 L0.872298625,9.40674263 C-0.306483301,10.5855246 -0.306483301,12.4961257 0.872298625,13.6739332 L2.29469548,15.0963301 C3.47250295,16.275112 5.38310413,16.275112 6.56188605,15.0963301 L8.96660118,12.6916149 C8.17191356,12.7446444 7.37131631,12.5658782 6.66209823,12.1513242 L6.66209823,12.1513242 Z" id="Shape"></path>
                        </g>
                    </g>
                </g>
            </g>
        </g>
    </g>
</svg>
</span>
  </a>
</li>



<li>
  
    <a href="http://dillinger.io/redirect/onedrive" class="import-one-drive unlinked">Link with
  
    <span>One Drive</span>
    <span class="icon icon-link"><svg viewBox="0 0 16 16" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g sketch:type="MSArtboardGroup" transform="translate(-238.000000, -212.000000)" fill="#FFFFFF">
            <g sketch:type="MSLayerGroup" transform="translate(0.000000, 65.000000)">
                <g transform="translate(0.000000, 75.000000)" sketch:type="MSShapeGroup">
                    <g transform="translate(32.000000, 72.000000)">
                        <g transform="translate(206.000000, 0.000000)">
                            <path d="M15.0962672,6.56194892 L10.8290766,10.8291395 C9.6502947,12.0079214 7.74066798,12.0079214 6.56188605,10.8291395 L5.13948919,9.40674263 L6.56188605,7.98434578 L7.98428291,9.40674263 C8.37721022,9.79869548 9.01472692,9.79966994 9.40667976,9.40674263 L13.6738703,5.13955206 C14.0658232,4.7471277 14.0658232,4.10910806 13.6738703,3.71665226 L12.2514735,2.2942554 C11.8595206,1.90230255 11.2210295,1.90230255 10.8290766,2.2942554 L9.30549312,3.8178389 C8.59919843,3.40231041 7.79665226,3.22401572 7.00196464,3.27754813 L9.40667976,0.871858546 C10.5854617,-0.306420432 12.4960629,-0.306420432 13.6738703,0.871858546 L15.0962672,2.2942554 C16.2750491,3.47256582 16.2750491,5.38366994 15.0962672,6.56194892 L15.0962672,6.56194892 Z M6.66209823,12.1513242 L5.13948919,13.6739332 C4.74656189,14.0668605 4.10904519,14.0658861 3.71709234,13.6739332 L2.29469548,12.2515363 C1.90176817,11.8595835 1.90176817,11.2220668 2.29469548,10.8291395 L6.56188605,6.56194892 C6.9538389,6.16999607 7.59233006,6.16999607 7.98428291,6.56194892 L9.40667976,7.98434578 L10.8290766,6.56194892 L9.40667976,5.13955206 C8.22789784,3.96127308 6.31827112,3.96127308 5.13948919,5.13955206 L0.872298625,9.40674263 C-0.306483301,10.5855246 -0.306483301,12.4961257 0.872298625,13.6739332 L2.29469548,15.0963301 C3.47250295,16.275112 5.38310413,16.275112 6.56188605,15.0963301 L8.96660118,12.6916149 C8.17191356,12.7446444 7.37131631,12.5658782 6.66209823,12.1513242 L6.66209823,12.1513242 Z" id="Shape"></path>
                        </g>
                    </g>
                </g>
            </g>
        </g>
    </g>
</svg>
</span>
  </a>
</li>


          </ul>
        </li>

        <li class="menu-item menu-item--save-to in-sidebar">
          <a class="menu-link" ng-click="document.status.save = !document.status.save">
          <span>Save to</span> <span class="caret"></span></a>

          <ul style="height: 0px;" class="sidebar-list collapse" collapse="document.status.save">
              
  <li class="ng-scope" ng-controller="Dropbox as dropbox">
    
      <a href="http://dillinger.io/redirect/dropbox" class="import-dropbox unlinked">
    
      <span>Dropbox</span>
      <span class="icon icon-link"><svg viewBox="0 0 16 16" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g sketch:type="MSArtboardGroup" transform="translate(-238.000000, -212.000000)" fill="#FFFFFF">
            <g sketch:type="MSLayerGroup" transform="translate(0.000000, 65.000000)">
                <g transform="translate(0.000000, 75.000000)" sketch:type="MSShapeGroup">
                    <g transform="translate(32.000000, 72.000000)">
                        <g transform="translate(206.000000, 0.000000)">
                            <path d="M15.0962672,6.56194892 L10.8290766,10.8291395 C9.6502947,12.0079214 7.74066798,12.0079214 6.56188605,10.8291395 L5.13948919,9.40674263 L6.56188605,7.98434578 L7.98428291,9.40674263 C8.37721022,9.79869548 9.01472692,9.79966994 9.40667976,9.40674263 L13.6738703,5.13955206 C14.0658232,4.7471277 14.0658232,4.10910806 13.6738703,3.71665226 L12.2514735,2.2942554 C11.8595206,1.90230255 11.2210295,1.90230255 10.8290766,2.2942554 L9.30549312,3.8178389 C8.59919843,3.40231041 7.79665226,3.22401572 7.00196464,3.27754813 L9.40667976,0.871858546 C10.5854617,-0.306420432 12.4960629,-0.306420432 13.6738703,0.871858546 L15.0962672,2.2942554 C16.2750491,3.47256582 16.2750491,5.38366994 15.0962672,6.56194892 L15.0962672,6.56194892 Z M6.66209823,12.1513242 L5.13948919,13.6739332 C4.74656189,14.0668605 4.10904519,14.0658861 3.71709234,13.6739332 L2.29469548,12.2515363 C1.90176817,11.8595835 1.90176817,11.2220668 2.29469548,10.8291395 L6.56188605,6.56194892 C6.9538389,6.16999607 7.59233006,6.16999607 7.98428291,6.56194892 L9.40667976,7.98434578 L10.8290766,6.56194892 L9.40667976,5.13955206 C8.22789784,3.96127308 6.31827112,3.96127308 5.13948919,5.13955206 L0.872298625,9.40674263 C-0.306483301,10.5855246 -0.306483301,12.4961257 0.872298625,13.6739332 L2.29469548,15.0963301 C3.47250295,16.275112 5.38310413,16.275112 6.56188605,15.0963301 L8.96660118,12.6916149 C8.17191356,12.7446444 7.37131631,12.5658782 6.66209823,12.1513242 L6.66209823,12.1513242 Z" id="Shape"></path>
                        </g>
                    </g>
                </g>
            </g>
        </g>
    </g>
</svg>
</span>
    </a>
  </li>
  

  
  <li class="ng-scope" ng-controller="Github as github">
    
      <a class="import-github unlinked" ng-click="github.chooseScope()">
    
      <span>Github</span>
      <span class="icon icon-link"><svg viewBox="0 0 16 16" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g sketch:type="MSArtboardGroup" transform="translate(-238.000000, -212.000000)" fill="#FFFFFF">
            <g sketch:type="MSLayerGroup" transform="translate(0.000000, 65.000000)">
                <g transform="translate(0.000000, 75.000000)" sketch:type="MSShapeGroup">
                    <g transform="translate(32.000000, 72.000000)">
                        <g transform="translate(206.000000, 0.000000)">
                            <path d="M15.0962672,6.56194892 L10.8290766,10.8291395 C9.6502947,12.0079214 7.74066798,12.0079214 6.56188605,10.8291395 L5.13948919,9.40674263 L6.56188605,7.98434578 L7.98428291,9.40674263 C8.37721022,9.79869548 9.01472692,9.79966994 9.40667976,9.40674263 L13.6738703,5.13955206 C14.0658232,4.7471277 14.0658232,4.10910806 13.6738703,3.71665226 L12.2514735,2.2942554 C11.8595206,1.90230255 11.2210295,1.90230255 10.8290766,2.2942554 L9.30549312,3.8178389 C8.59919843,3.40231041 7.79665226,3.22401572 7.00196464,3.27754813 L9.40667976,0.871858546 C10.5854617,-0.306420432 12.4960629,-0.306420432 13.6738703,0.871858546 L15.0962672,2.2942554 C16.2750491,3.47256582 16.2750491,5.38366994 15.0962672,6.56194892 L15.0962672,6.56194892 Z M6.66209823,12.1513242 L5.13948919,13.6739332 C4.74656189,14.0668605 4.10904519,14.0658861 3.71709234,13.6739332 L2.29469548,12.2515363 C1.90176817,11.8595835 1.90176817,11.2220668 2.29469548,10.8291395 L6.56188605,6.56194892 C6.9538389,6.16999607 7.59233006,6.16999607 7.98428291,6.56194892 L9.40667976,7.98434578 L10.8290766,6.56194892 L9.40667976,5.13955206 C8.22789784,3.96127308 6.31827112,3.96127308 5.13948919,5.13955206 L0.872298625,9.40674263 C-0.306483301,10.5855246 -0.306483301,12.4961257 0.872298625,13.6739332 L2.29469548,15.0963301 C3.47250295,16.275112 5.38310413,16.275112 6.56188605,15.0963301 L8.96660118,12.6916149 C8.17191356,12.7446444 7.37131631,12.5658782 6.66209823,12.1513242 L6.66209823,12.1513242 Z" id="Shape"></path>
                        </g>
                    </g>
                </g>
            </g>
        </g>
    </g>
</svg>
</span>
    </a>
  </li>
  

  
  <li class="ng-scope" ng-controller="Googledrive as googledrive">
    
      <a href="http://dillinger.io/redirect/googledrive" class="import-google-drive unlinked">
    
      <span>Google Drive</span>
      <span class="icon icon-link"><svg viewBox="0 0 16 16" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g sketch:type="MSArtboardGroup" transform="translate(-238.000000, -212.000000)" fill="#FFFFFF">
            <g sketch:type="MSLayerGroup" transform="translate(0.000000, 65.000000)">
                <g transform="translate(0.000000, 75.000000)" sketch:type="MSShapeGroup">
                    <g transform="translate(32.000000, 72.000000)">
                        <g transform="translate(206.000000, 0.000000)">
                            <path d="M15.0962672,6.56194892 L10.8290766,10.8291395 C9.6502947,12.0079214 7.74066798,12.0079214 6.56188605,10.8291395 L5.13948919,9.40674263 L6.56188605,7.98434578 L7.98428291,9.40674263 C8.37721022,9.79869548 9.01472692,9.79966994 9.40667976,9.40674263 L13.6738703,5.13955206 C14.0658232,4.7471277 14.0658232,4.10910806 13.6738703,3.71665226 L12.2514735,2.2942554 C11.8595206,1.90230255 11.2210295,1.90230255 10.8290766,2.2942554 L9.30549312,3.8178389 C8.59919843,3.40231041 7.79665226,3.22401572 7.00196464,3.27754813 L9.40667976,0.871858546 C10.5854617,-0.306420432 12.4960629,-0.306420432 13.6738703,0.871858546 L15.0962672,2.2942554 C16.2750491,3.47256582 16.2750491,5.38366994 15.0962672,6.56194892 L15.0962672,6.56194892 Z M6.66209823,12.1513242 L5.13948919,13.6739332 C4.74656189,14.0668605 4.10904519,14.0658861 3.71709234,13.6739332 L2.29469548,12.2515363 C1.90176817,11.8595835 1.90176817,11.2220668 2.29469548,10.8291395 L6.56188605,6.56194892 C6.9538389,6.16999607 7.59233006,6.16999607 7.98428291,6.56194892 L9.40667976,7.98434578 L10.8290766,6.56194892 L9.40667976,5.13955206 C8.22789784,3.96127308 6.31827112,3.96127308 5.13948919,5.13955206 L0.872298625,9.40674263 C-0.306483301,10.5855246 -0.306483301,12.4961257 0.872298625,13.6739332 L2.29469548,15.0963301 C3.47250295,16.275112 5.38310413,16.275112 6.56188605,15.0963301 L8.96660118,12.6916149 C8.17191356,12.7446444 7.37131631,12.5658782 6.66209823,12.1513242 L6.66209823,12.1513242 Z" id="Shape"></path>
                        </g>
                    </g>
                </g>
            </g>
        </g>
    </g>
</svg>
</span>
    </a>
  </li>
  

  
  <li class="ng-scope" ng-controller="Onedrive as onedrive">
    
      <a href="http://dillinger.io/redirect/onedrive" class="import-one-drive unlinked">
    
      <span>One Drive</span>
      <span class="icon icon-link"><svg viewBox="0 0 16 16" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g sketch:type="MSArtboardGroup" transform="translate(-238.000000, -212.000000)" fill="#FFFFFF">
            <g sketch:type="MSLayerGroup" transform="translate(0.000000, 65.000000)">
                <g transform="translate(0.000000, 75.000000)" sketch:type="MSShapeGroup">
                    <g transform="translate(32.000000, 72.000000)">
                        <g transform="translate(206.000000, 0.000000)">
                            <path d="M15.0962672,6.56194892 L10.8290766,10.8291395 C9.6502947,12.0079214 7.74066798,12.0079214 6.56188605,10.8291395 L5.13948919,9.40674263 L6.56188605,7.98434578 L7.98428291,9.40674263 C8.37721022,9.79869548 9.01472692,9.79966994 9.40667976,9.40674263 L13.6738703,5.13955206 C14.0658232,4.7471277 14.0658232,4.10910806 13.6738703,3.71665226 L12.2514735,2.2942554 C11.8595206,1.90230255 11.2210295,1.90230255 10.8290766,2.2942554 L9.30549312,3.8178389 C8.59919843,3.40231041 7.79665226,3.22401572 7.00196464,3.27754813 L9.40667976,0.871858546 C10.5854617,-0.306420432 12.4960629,-0.306420432 13.6738703,0.871858546 L15.0962672,2.2942554 C16.2750491,3.47256582 16.2750491,5.38366994 15.0962672,6.56194892 L15.0962672,6.56194892 Z M6.66209823,12.1513242 L5.13948919,13.6739332 C4.74656189,14.0668605 4.10904519,14.0658861 3.71709234,13.6739332 L2.29469548,12.2515363 C1.90176817,11.8595835 1.90176817,11.2220668 2.29469548,10.8291395 L6.56188605,6.56194892 C6.9538389,6.16999607 7.59233006,6.16999607 7.98428291,6.56194892 L9.40667976,7.98434578 L10.8290766,6.56194892 L9.40667976,5.13955206 C8.22789784,3.96127308 6.31827112,3.96127308 5.13948919,5.13955206 L0.872298625,9.40674263 C-0.306483301,10.5855246 -0.306483301,12.4961257 0.872298625,13.6739332 L2.29469548,15.0963301 C3.47250295,16.275112 5.38310413,16.275112 6.56188605,15.0963301 L8.96660118,12.6916149 C8.17191356,12.7446444 7.37131631,12.5658782 6.66209823,12.1513242 L6.66209823,12.1513242 Z" id="Shape"></path>
                        </g>
                    </g>
                </g>
            </g>
        </g>
    </g>
</svg>
</span>
    </a>
  </li>
  

          </ul>
        </li>
        <li class="menu-item menu-item--import-from in-sidebar">
          <a class="menu-link" ng-click="document.status.import = !document.status.import">
          <span>Import from</span> <span class="caret"></span></a>

          <ul style="height: 0px;" class="sidebar-list collapse" collapse="document.status.import">
              
  <li class="ng-scope" ng-controller="Dropbox as dropbox">
    
      <a href="http://dillinger.io/redirect/dropbox" class="import-dropbox unlinked">
    
      <span>Dropbox</span>
      <span class="icon icon-link"><svg viewBox="0 0 16 16" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g sketch:type="MSArtboardGroup" transform="translate(-238.000000, -212.000000)" fill="#FFFFFF">
            <g sketch:type="MSLayerGroup" transform="translate(0.000000, 65.000000)">
                <g transform="translate(0.000000, 75.000000)" sketch:type="MSShapeGroup">
                    <g transform="translate(32.000000, 72.000000)">
                        <g transform="translate(206.000000, 0.000000)">
                            <path d="M15.0962672,6.56194892 L10.8290766,10.8291395 C9.6502947,12.0079214 7.74066798,12.0079214 6.56188605,10.8291395 L5.13948919,9.40674263 L6.56188605,7.98434578 L7.98428291,9.40674263 C8.37721022,9.79869548 9.01472692,9.79966994 9.40667976,9.40674263 L13.6738703,5.13955206 C14.0658232,4.7471277 14.0658232,4.10910806 13.6738703,3.71665226 L12.2514735,2.2942554 C11.8595206,1.90230255 11.2210295,1.90230255 10.8290766,2.2942554 L9.30549312,3.8178389 C8.59919843,3.40231041 7.79665226,3.22401572 7.00196464,3.27754813 L9.40667976,0.871858546 C10.5854617,-0.306420432 12.4960629,-0.306420432 13.6738703,0.871858546 L15.0962672,2.2942554 C16.2750491,3.47256582 16.2750491,5.38366994 15.0962672,6.56194892 L15.0962672,6.56194892 Z M6.66209823,12.1513242 L5.13948919,13.6739332 C4.74656189,14.0668605 4.10904519,14.0658861 3.71709234,13.6739332 L2.29469548,12.2515363 C1.90176817,11.8595835 1.90176817,11.2220668 2.29469548,10.8291395 L6.56188605,6.56194892 C6.9538389,6.16999607 7.59233006,6.16999607 7.98428291,6.56194892 L9.40667976,7.98434578 L10.8290766,6.56194892 L9.40667976,5.13955206 C8.22789784,3.96127308 6.31827112,3.96127308 5.13948919,5.13955206 L0.872298625,9.40674263 C-0.306483301,10.5855246 -0.306483301,12.4961257 0.872298625,13.6739332 L2.29469548,15.0963301 C3.47250295,16.275112 5.38310413,16.275112 6.56188605,15.0963301 L8.96660118,12.6916149 C8.17191356,12.7446444 7.37131631,12.5658782 6.66209823,12.1513242 L6.66209823,12.1513242 Z" id="Shape"></path>
                        </g>
                    </g>
                </g>
            </g>
        </g>
    </g>
</svg>
</span>
    </a>
  </li>
  

  
  <li class="ng-scope" ng-controller="Github as github">
    
      <a class="import-github unlinked" ng-click="github.chooseScope()">
    
      <span>Github</span>
      <span class="icon icon-link"><svg viewBox="0 0 16 16" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g sketch:type="MSArtboardGroup" transform="translate(-238.000000, -212.000000)" fill="#FFFFFF">
            <g sketch:type="MSLayerGroup" transform="translate(0.000000, 65.000000)">
                <g transform="translate(0.000000, 75.000000)" sketch:type="MSShapeGroup">
                    <g transform="translate(32.000000, 72.000000)">
                        <g transform="translate(206.000000, 0.000000)">
                            <path d="M15.0962672,6.56194892 L10.8290766,10.8291395 C9.6502947,12.0079214 7.74066798,12.0079214 6.56188605,10.8291395 L5.13948919,9.40674263 L6.56188605,7.98434578 L7.98428291,9.40674263 C8.37721022,9.79869548 9.01472692,9.79966994 9.40667976,9.40674263 L13.6738703,5.13955206 C14.0658232,4.7471277 14.0658232,4.10910806 13.6738703,3.71665226 L12.2514735,2.2942554 C11.8595206,1.90230255 11.2210295,1.90230255 10.8290766,2.2942554 L9.30549312,3.8178389 C8.59919843,3.40231041 7.79665226,3.22401572 7.00196464,3.27754813 L9.40667976,0.871858546 C10.5854617,-0.306420432 12.4960629,-0.306420432 13.6738703,0.871858546 L15.0962672,2.2942554 C16.2750491,3.47256582 16.2750491,5.38366994 15.0962672,6.56194892 L15.0962672,6.56194892 Z M6.66209823,12.1513242 L5.13948919,13.6739332 C4.74656189,14.0668605 4.10904519,14.0658861 3.71709234,13.6739332 L2.29469548,12.2515363 C1.90176817,11.8595835 1.90176817,11.2220668 2.29469548,10.8291395 L6.56188605,6.56194892 C6.9538389,6.16999607 7.59233006,6.16999607 7.98428291,6.56194892 L9.40667976,7.98434578 L10.8290766,6.56194892 L9.40667976,5.13955206 C8.22789784,3.96127308 6.31827112,3.96127308 5.13948919,5.13955206 L0.872298625,9.40674263 C-0.306483301,10.5855246 -0.306483301,12.4961257 0.872298625,13.6739332 L2.29469548,15.0963301 C3.47250295,16.275112 5.38310413,16.275112 6.56188605,15.0963301 L8.96660118,12.6916149 C8.17191356,12.7446444 7.37131631,12.5658782 6.66209823,12.1513242 L6.66209823,12.1513242 Z" id="Shape"></path>
                        </g>
                    </g>
                </g>
            </g>
        </g>
    </g>
</svg>
</span>
    </a>
  </li>
  

  
  <li class="ng-scope" ng-controller="Googledrive as googledrive">
    
      <a href="http://dillinger.io/redirect/googledrive" class="import-google-drive unlinked">
    
      <span>Google Drive</span>
      <span class="icon icon-link"><svg viewBox="0 0 16 16" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g sketch:type="MSArtboardGroup" transform="translate(-238.000000, -212.000000)" fill="#FFFFFF">
            <g sketch:type="MSLayerGroup" transform="translate(0.000000, 65.000000)">
                <g transform="translate(0.000000, 75.000000)" sketch:type="MSShapeGroup">
                    <g transform="translate(32.000000, 72.000000)">
                        <g transform="translate(206.000000, 0.000000)">
                            <path d="M15.0962672,6.56194892 L10.8290766,10.8291395 C9.6502947,12.0079214 7.74066798,12.0079214 6.56188605,10.8291395 L5.13948919,9.40674263 L6.56188605,7.98434578 L7.98428291,9.40674263 C8.37721022,9.79869548 9.01472692,9.79966994 9.40667976,9.40674263 L13.6738703,5.13955206 C14.0658232,4.7471277 14.0658232,4.10910806 13.6738703,3.71665226 L12.2514735,2.2942554 C11.8595206,1.90230255 11.2210295,1.90230255 10.8290766,2.2942554 L9.30549312,3.8178389 C8.59919843,3.40231041 7.79665226,3.22401572 7.00196464,3.27754813 L9.40667976,0.871858546 C10.5854617,-0.306420432 12.4960629,-0.306420432 13.6738703,0.871858546 L15.0962672,2.2942554 C16.2750491,3.47256582 16.2750491,5.38366994 15.0962672,6.56194892 L15.0962672,6.56194892 Z M6.66209823,12.1513242 L5.13948919,13.6739332 C4.74656189,14.0668605 4.10904519,14.0658861 3.71709234,13.6739332 L2.29469548,12.2515363 C1.90176817,11.8595835 1.90176817,11.2220668 2.29469548,10.8291395 L6.56188605,6.56194892 C6.9538389,6.16999607 7.59233006,6.16999607 7.98428291,6.56194892 L9.40667976,7.98434578 L10.8290766,6.56194892 L9.40667976,5.13955206 C8.22789784,3.96127308 6.31827112,3.96127308 5.13948919,5.13955206 L0.872298625,9.40674263 C-0.306483301,10.5855246 -0.306483301,12.4961257 0.872298625,13.6739332 L2.29469548,15.0963301 C3.47250295,16.275112 5.38310413,16.275112 6.56188605,15.0963301 L8.96660118,12.6916149 C8.17191356,12.7446444 7.37131631,12.5658782 6.66209823,12.1513242 L6.66209823,12.1513242 Z" id="Shape"></path>
                        </g>
                    </g>
                </g>
            </g>
        </g>
    </g>
</svg>
</span>
    </a>
  </li>
  

  
  <li class="ng-scope" ng-controller="Onedrive as onedrive">
    
      <a href="http://dillinger.io/redirect/onedrive" class="import-one-drive unlinked">
    
      <span>One Drive</span>
      <span class="icon icon-link"><svg viewBox="0 0 16 16" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g sketch:type="MSArtboardGroup" transform="translate(-238.000000, -212.000000)" fill="#FFFFFF">
            <g sketch:type="MSLayerGroup" transform="translate(0.000000, 65.000000)">
                <g transform="translate(0.000000, 75.000000)" sketch:type="MSShapeGroup">
                    <g transform="translate(32.000000, 72.000000)">
                        <g transform="translate(206.000000, 0.000000)">
                            <path d="M15.0962672,6.56194892 L10.8290766,10.8291395 C9.6502947,12.0079214 7.74066798,12.0079214 6.56188605,10.8291395 L5.13948919,9.40674263 L6.56188605,7.98434578 L7.98428291,9.40674263 C8.37721022,9.79869548 9.01472692,9.79966994 9.40667976,9.40674263 L13.6738703,5.13955206 C14.0658232,4.7471277 14.0658232,4.10910806 13.6738703,3.71665226 L12.2514735,2.2942554 C11.8595206,1.90230255 11.2210295,1.90230255 10.8290766,2.2942554 L9.30549312,3.8178389 C8.59919843,3.40231041 7.79665226,3.22401572 7.00196464,3.27754813 L9.40667976,0.871858546 C10.5854617,-0.306420432 12.4960629,-0.306420432 13.6738703,0.871858546 L15.0962672,2.2942554 C16.2750491,3.47256582 16.2750491,5.38366994 15.0962672,6.56194892 L15.0962672,6.56194892 Z M6.66209823,12.1513242 L5.13948919,13.6739332 C4.74656189,14.0668605 4.10904519,14.0658861 3.71709234,13.6739332 L2.29469548,12.2515363 C1.90176817,11.8595835 1.90176817,11.2220668 2.29469548,10.8291395 L6.56188605,6.56194892 C6.9538389,6.16999607 7.59233006,6.16999607 7.98428291,6.56194892 L9.40667976,7.98434578 L10.8290766,6.56194892 L9.40667976,5.13955206 C8.22789784,3.96127308 6.31827112,3.96127308 5.13948919,5.13955206 L0.872298625,9.40674263 C-0.306483301,10.5855246 -0.306483301,12.4961257 0.872298625,13.6739332 L2.29469548,15.0963301 C3.47250295,16.275112 5.38310413,16.275112 6.56188605,15.0963301 L8.96660118,12.6916149 C8.17191356,12.7446444 7.37131631,12.5658782 6.66209823,12.1513242 L6.66209823,12.1513242 Z" id="Shape"></path>
                        </g>
                    </g>
                </g>
            </g>
        </g>
    </g>
</svg>
</span>
    </a>
  </li>
  

          </ul>
        </li>
        <li class="menu-item menu-item--documents in-sidebar">
          <a class="menu-link" ng-click="document.status.document = !document.status.document">
          <span>Documents</span> <span class="caret"></span></a>

          <ul style="height: auto;" class="documents sidebar-list collapse in" collapse="document.status.document" role="menu">
            <!-- ngRepeat: document in documents track by document.id --><li class="ng-scope active" ng-repeat="document in documents track by document.id" ng-class="{
    'active': currentDocument.id === document.id,
    'octocat': document.isGithubFile
    }">
  <a class="ng-binding" ng-click="selectDocument(document)">Untitled Document.md</a>
</li><!-- end ngRepeat: document in documents track by document.id -->

          </ul>
        </li>
      </ul>
    </nav>
    <a class="btn btn--new" ng-click="createDocument()">New Document</a>
    <a class="btn btn--save" ng-click="saveDocument(true)">Save Session</a>
    <!-- ngIf: documents.length > 1 -->
  </div>
</div>

    <div class="notification-container">
  <div class="notification"></div>
</div>

    <div class="page">
      <div class="navbar">

  <a class="toggle">
  <span></span>
</a>

  <h1 class="navbar-brand">
    <a class="brand" href="http://dillinger.io/"><svg viewBox="0 0 85 11" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g id="Page-1" stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g id="Desktop---Save-to" sketch:type="MSArtboardGroup" transform="translate(-92.000000, -58.000000)" fill="#FFFFFF">
            <g id="Navigation" sketch:type="MSLayerGroup" transform="translate(0.000000, 38.000000)">
                <g id="Menu-Item:-Branding" transform="translate(91.000000, 17.000000)" sketch:type="MSShapeGroup">
                    <path d="M1.17,13 L4.563,13 C7.423,13 9.399,11.011 9.399,8.45 L9.399,8.424 C9.399,5.863 7.423,3.9 4.563,3.9 L1.17,3.9 L1.17,13 Z M2.769,11.544 L2.769,5.356 L4.563,5.356 C6.474,5.356 7.722,6.669 7.722,8.45 L7.722,8.476 C7.722,10.257 6.474,11.544 4.563,11.544 L2.769,11.544 Z M13.047,13 L14.646,13 L14.646,3.9 L13.047,3.9 L13.047,13 Z M18.697,13 L25.08,13 L25.08,11.544 L20.296,11.544 L20.296,3.9 L18.697,3.9 L18.697,13 Z M28.364,13 L34.747,13 L34.747,11.544 L29.963,11.544 L29.963,3.9 L28.364,3.9 L28.364,13 Z M38.122,13 L39.721,13 L39.721,3.9 L38.122,3.9 L38.122,13 Z M43.772,13 L45.345,13 L45.345,6.526 L50.363,13 L51.702,13 L51.702,3.9 L50.129,3.9 L50.129,10.192 L45.254,3.9 L43.772,3.9 L43.772,13 Z M59.978,13.156 C61.59,13.156 62.877,12.506 63.774,11.739 L63.774,7.917 L59.9,7.917 L59.9,9.308 L62.227,9.308 L62.227,11.011 C61.668,11.427 60.888,11.7 60.03,11.7 C58.171,11.7 56.936,10.322 56.936,8.45 L56.936,8.424 C56.936,6.682 58.21,5.213 59.887,5.213 C61.044,5.213 61.733,5.59 62.435,6.188 L63.449,4.979 C62.513,4.186 61.538,3.744 59.952,3.744 C57.209,3.744 55.259,5.902 55.259,8.45 L55.259,8.476 C55.259,11.128 57.131,13.156 59.978,13.156 Z M67.474,13 L74.286,13 L74.286,11.57 L69.073,11.57 L69.073,9.126 L73.636,9.126 L73.636,7.696 L69.073,7.696 L69.073,5.33 L74.221,5.33 L74.221,3.9 L67.474,3.9 L67.474,13 Z M77.804,13 L79.403,13 L79.403,9.828 L81.405,9.828 L83.641,13 L85.526,13 L83.069,9.555 C84.343,9.191 85.24,8.294 85.24,6.799 L85.24,6.773 C85.24,5.98 84.967,5.304 84.486,4.81 C83.901,4.238 83.004,3.9 81.86,3.9 L77.804,3.9 L77.804,13 Z M79.403,8.411 L79.403,5.356 L81.73,5.356 C82.913,5.356 83.615,5.889 83.615,6.864 L83.615,6.89 C83.615,7.813 82.887,8.411 81.743,8.411 L79.403,8.411 Z" id="DILLINGER-2"></path>
                </g>
            </g>
        </g>
    </g>
</svg>
</a>
  </h1>

  <nav class="nav nav-right">
    <ul class="menu menu-utilities">
      <li class="menu-item menu-item--export-as has-dropdown" dropdown="">
        <a aria-expanded="false" aria-haspopup="true" class="dropdown-toggle" dropdown-toggle="">Export as <span class="caret"></span></a>
        <ul class="dropdown dropdown-menu ng-scope" role="menu" ng-controller="DocumentsExport as export">
  <li>
    <a ng-click="export.asHTML()" class="export-html">HTML</a>
  </li>
  <li>
    <a ng-click="export.asStyledHTML()" class="export-styled_html">Styled HTML</a>
  </li>
  <li>
    <a ng-click="export.asMarkdown()" class="export-md">Markdown</a>
  </li>
  <li>
    <a ng-click="export.asPDF()" class="export-pdf">PDF</a>
  </li>
</ul>

      </li>
      <li class="menu-item menu-item--save-to has-dropdown" dropdown="">
        <a aria-expanded="false" aria-haspopup="true" class="dropdown-toggle" dropdown-toggle="">Save to <span class="caret"></span></a>

        <ul class="dropdown dropdown-menu" role="menu">
            
  <li class="ng-scope" ng-controller="Dropbox as dropbox">
    
      <a href="http://dillinger.io/redirect/dropbox" class="import-dropbox unlinked">
    
      <span>Dropbox</span>
      <span class="icon icon-link"><svg viewBox="0 0 16 16" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g sketch:type="MSArtboardGroup" transform="translate(-238.000000, -212.000000)" fill="#FFFFFF">
            <g sketch:type="MSLayerGroup" transform="translate(0.000000, 65.000000)">
                <g transform="translate(0.000000, 75.000000)" sketch:type="MSShapeGroup">
                    <g transform="translate(32.000000, 72.000000)">
                        <g transform="translate(206.000000, 0.000000)">
                            <path d="M15.0962672,6.56194892 L10.8290766,10.8291395 C9.6502947,12.0079214 7.74066798,12.0079214 6.56188605,10.8291395 L5.13948919,9.40674263 L6.56188605,7.98434578 L7.98428291,9.40674263 C8.37721022,9.79869548 9.01472692,9.79966994 9.40667976,9.40674263 L13.6738703,5.13955206 C14.0658232,4.7471277 14.0658232,4.10910806 13.6738703,3.71665226 L12.2514735,2.2942554 C11.8595206,1.90230255 11.2210295,1.90230255 10.8290766,2.2942554 L9.30549312,3.8178389 C8.59919843,3.40231041 7.79665226,3.22401572 7.00196464,3.27754813 L9.40667976,0.871858546 C10.5854617,-0.306420432 12.4960629,-0.306420432 13.6738703,0.871858546 L15.0962672,2.2942554 C16.2750491,3.47256582 16.2750491,5.38366994 15.0962672,6.56194892 L15.0962672,6.56194892 Z M6.66209823,12.1513242 L5.13948919,13.6739332 C4.74656189,14.0668605 4.10904519,14.0658861 3.71709234,13.6739332 L2.29469548,12.2515363 C1.90176817,11.8595835 1.90176817,11.2220668 2.29469548,10.8291395 L6.56188605,6.56194892 C6.9538389,6.16999607 7.59233006,6.16999607 7.98428291,6.56194892 L9.40667976,7.98434578 L10.8290766,6.56194892 L9.40667976,5.13955206 C8.22789784,3.96127308 6.31827112,3.96127308 5.13948919,5.13955206 L0.872298625,9.40674263 C-0.306483301,10.5855246 -0.306483301,12.4961257 0.872298625,13.6739332 L2.29469548,15.0963301 C3.47250295,16.275112 5.38310413,16.275112 6.56188605,15.0963301 L8.96660118,12.6916149 C8.17191356,12.7446444 7.37131631,12.5658782 6.66209823,12.1513242 L6.66209823,12.1513242 Z" id="Shape"></path>
                        </g>
                    </g>
                </g>
            </g>
        </g>
    </g>
</svg>
</span>
    </a>
  </li>
  

  
  <li class="ng-scope" ng-controller="Github as github">
    
      <a class="import-github unlinked" ng-click="github.chooseScope()">
    
      <span>Github</span>
      <span class="icon icon-link"><svg viewBox="0 0 16 16" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g sketch:type="MSArtboardGroup" transform="translate(-238.000000, -212.000000)" fill="#FFFFFF">
            <g sketch:type="MSLayerGroup" transform="translate(0.000000, 65.000000)">
                <g transform="translate(0.000000, 75.000000)" sketch:type="MSShapeGroup">
                    <g transform="translate(32.000000, 72.000000)">
                        <g transform="translate(206.000000, 0.000000)">
                            <path d="M15.0962672,6.56194892 L10.8290766,10.8291395 C9.6502947,12.0079214 7.74066798,12.0079214 6.56188605,10.8291395 L5.13948919,9.40674263 L6.56188605,7.98434578 L7.98428291,9.40674263 C8.37721022,9.79869548 9.01472692,9.79966994 9.40667976,9.40674263 L13.6738703,5.13955206 C14.0658232,4.7471277 14.0658232,4.10910806 13.6738703,3.71665226 L12.2514735,2.2942554 C11.8595206,1.90230255 11.2210295,1.90230255 10.8290766,2.2942554 L9.30549312,3.8178389 C8.59919843,3.40231041 7.79665226,3.22401572 7.00196464,3.27754813 L9.40667976,0.871858546 C10.5854617,-0.306420432 12.4960629,-0.306420432 13.6738703,0.871858546 L15.0962672,2.2942554 C16.2750491,3.47256582 16.2750491,5.38366994 15.0962672,6.56194892 L15.0962672,6.56194892 Z M6.66209823,12.1513242 L5.13948919,13.6739332 C4.74656189,14.0668605 4.10904519,14.0658861 3.71709234,13.6739332 L2.29469548,12.2515363 C1.90176817,11.8595835 1.90176817,11.2220668 2.29469548,10.8291395 L6.56188605,6.56194892 C6.9538389,6.16999607 7.59233006,6.16999607 7.98428291,6.56194892 L9.40667976,7.98434578 L10.8290766,6.56194892 L9.40667976,5.13955206 C8.22789784,3.96127308 6.31827112,3.96127308 5.13948919,5.13955206 L0.872298625,9.40674263 C-0.306483301,10.5855246 -0.306483301,12.4961257 0.872298625,13.6739332 L2.29469548,15.0963301 C3.47250295,16.275112 5.38310413,16.275112 6.56188605,15.0963301 L8.96660118,12.6916149 C8.17191356,12.7446444 7.37131631,12.5658782 6.66209823,12.1513242 L6.66209823,12.1513242 Z" id="Shape"></path>
                        </g>
                    </g>
                </g>
            </g>
        </g>
    </g>
</svg>
</span>
    </a>
  </li>
  

  
  <li class="ng-scope" ng-controller="Googledrive as googledrive">
    
      <a href="http://dillinger.io/redirect/googledrive" class="import-google-drive unlinked">
    
      <span>Google Drive</span>
      <span class="icon icon-link"><svg viewBox="0 0 16 16" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g sketch:type="MSArtboardGroup" transform="translate(-238.000000, -212.000000)" fill="#FFFFFF">
            <g sketch:type="MSLayerGroup" transform="translate(0.000000, 65.000000)">
                <g transform="translate(0.000000, 75.000000)" sketch:type="MSShapeGroup">
                    <g transform="translate(32.000000, 72.000000)">
                        <g transform="translate(206.000000, 0.000000)">
                            <path d="M15.0962672,6.56194892 L10.8290766,10.8291395 C9.6502947,12.0079214 7.74066798,12.0079214 6.56188605,10.8291395 L5.13948919,9.40674263 L6.56188605,7.98434578 L7.98428291,9.40674263 C8.37721022,9.79869548 9.01472692,9.79966994 9.40667976,9.40674263 L13.6738703,5.13955206 C14.0658232,4.7471277 14.0658232,4.10910806 13.6738703,3.71665226 L12.2514735,2.2942554 C11.8595206,1.90230255 11.2210295,1.90230255 10.8290766,2.2942554 L9.30549312,3.8178389 C8.59919843,3.40231041 7.79665226,3.22401572 7.00196464,3.27754813 L9.40667976,0.871858546 C10.5854617,-0.306420432 12.4960629,-0.306420432 13.6738703,0.871858546 L15.0962672,2.2942554 C16.2750491,3.47256582 16.2750491,5.38366994 15.0962672,6.56194892 L15.0962672,6.56194892 Z M6.66209823,12.1513242 L5.13948919,13.6739332 C4.74656189,14.0668605 4.10904519,14.0658861 3.71709234,13.6739332 L2.29469548,12.2515363 C1.90176817,11.8595835 1.90176817,11.2220668 2.29469548,10.8291395 L6.56188605,6.56194892 C6.9538389,6.16999607 7.59233006,6.16999607 7.98428291,6.56194892 L9.40667976,7.98434578 L10.8290766,6.56194892 L9.40667976,5.13955206 C8.22789784,3.96127308 6.31827112,3.96127308 5.13948919,5.13955206 L0.872298625,9.40674263 C-0.306483301,10.5855246 -0.306483301,12.4961257 0.872298625,13.6739332 L2.29469548,15.0963301 C3.47250295,16.275112 5.38310413,16.275112 6.56188605,15.0963301 L8.96660118,12.6916149 C8.17191356,12.7446444 7.37131631,12.5658782 6.66209823,12.1513242 L6.66209823,12.1513242 Z" id="Shape"></path>
                        </g>
                    </g>
                </g>
            </g>
        </g>
    </g>
</svg>
</span>
    </a>
  </li>
  

  
  <li class="ng-scope" ng-controller="Onedrive as onedrive">
    
      <a href="http://dillinger.io/redirect/onedrive" class="import-one-drive unlinked">
    
      <span>One Drive</span>
      <span class="icon icon-link"><svg viewBox="0 0 16 16" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g sketch:type="MSArtboardGroup" transform="translate(-238.000000, -212.000000)" fill="#FFFFFF">
            <g sketch:type="MSLayerGroup" transform="translate(0.000000, 65.000000)">
                <g transform="translate(0.000000, 75.000000)" sketch:type="MSShapeGroup">
                    <g transform="translate(32.000000, 72.000000)">
                        <g transform="translate(206.000000, 0.000000)">
                            <path d="M15.0962672,6.56194892 L10.8290766,10.8291395 C9.6502947,12.0079214 7.74066798,12.0079214 6.56188605,10.8291395 L5.13948919,9.40674263 L6.56188605,7.98434578 L7.98428291,9.40674263 C8.37721022,9.79869548 9.01472692,9.79966994 9.40667976,9.40674263 L13.6738703,5.13955206 C14.0658232,4.7471277 14.0658232,4.10910806 13.6738703,3.71665226 L12.2514735,2.2942554 C11.8595206,1.90230255 11.2210295,1.90230255 10.8290766,2.2942554 L9.30549312,3.8178389 C8.59919843,3.40231041 7.79665226,3.22401572 7.00196464,3.27754813 L9.40667976,0.871858546 C10.5854617,-0.306420432 12.4960629,-0.306420432 13.6738703,0.871858546 L15.0962672,2.2942554 C16.2750491,3.47256582 16.2750491,5.38366994 15.0962672,6.56194892 L15.0962672,6.56194892 Z M6.66209823,12.1513242 L5.13948919,13.6739332 C4.74656189,14.0668605 4.10904519,14.0658861 3.71709234,13.6739332 L2.29469548,12.2515363 C1.90176817,11.8595835 1.90176817,11.2220668 2.29469548,10.8291395 L6.56188605,6.56194892 C6.9538389,6.16999607 7.59233006,6.16999607 7.98428291,6.56194892 L9.40667976,7.98434578 L10.8290766,6.56194892 L9.40667976,5.13955206 C8.22789784,3.96127308 6.31827112,3.96127308 5.13948919,5.13955206 L0.872298625,9.40674263 C-0.306483301,10.5855246 -0.306483301,12.4961257 0.872298625,13.6739332 L2.29469548,15.0963301 C3.47250295,16.275112 5.38310413,16.275112 6.56188605,15.0963301 L8.96660118,12.6916149 C8.17191356,12.7446444 7.37131631,12.5658782 6.66209823,12.1513242 L6.66209823,12.1513242 Z" id="Shape"></path>
                        </g>
                    </g>
                </g>
            </g>
        </g>
    </g>
</svg>
</span>
    </a>
  </li>
  

        </ul>
      </li>
      <li class="menu-item menu-item--import-from has-dropdown" dropdown="">
        <a aria-expanded="false" aria-haspopup="true" class="dropdown-toggle" dropdown-toggle="">Import from <span class="caret"></span></a>

        <ul class="dropdown dropdown-menu" role="menu">
            
  <li class="ng-scope" ng-controller="Dropbox as dropbox">
    
      <a href="http://dillinger.io/redirect/dropbox" class="import-dropbox unlinked">
    
      <span>Dropbox</span>
      <span class="icon icon-link"><svg viewBox="0 0 16 16" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g sketch:type="MSArtboardGroup" transform="translate(-238.000000, -212.000000)" fill="#FFFFFF">
            <g sketch:type="MSLayerGroup" transform="translate(0.000000, 65.000000)">
                <g transform="translate(0.000000, 75.000000)" sketch:type="MSShapeGroup">
                    <g transform="translate(32.000000, 72.000000)">
                        <g transform="translate(206.000000, 0.000000)">
                            <path d="M15.0962672,6.56194892 L10.8290766,10.8291395 C9.6502947,12.0079214 7.74066798,12.0079214 6.56188605,10.8291395 L5.13948919,9.40674263 L6.56188605,7.98434578 L7.98428291,9.40674263 C8.37721022,9.79869548 9.01472692,9.79966994 9.40667976,9.40674263 L13.6738703,5.13955206 C14.0658232,4.7471277 14.0658232,4.10910806 13.6738703,3.71665226 L12.2514735,2.2942554 C11.8595206,1.90230255 11.2210295,1.90230255 10.8290766,2.2942554 L9.30549312,3.8178389 C8.59919843,3.40231041 7.79665226,3.22401572 7.00196464,3.27754813 L9.40667976,0.871858546 C10.5854617,-0.306420432 12.4960629,-0.306420432 13.6738703,0.871858546 L15.0962672,2.2942554 C16.2750491,3.47256582 16.2750491,5.38366994 15.0962672,6.56194892 L15.0962672,6.56194892 Z M6.66209823,12.1513242 L5.13948919,13.6739332 C4.74656189,14.0668605 4.10904519,14.0658861 3.71709234,13.6739332 L2.29469548,12.2515363 C1.90176817,11.8595835 1.90176817,11.2220668 2.29469548,10.8291395 L6.56188605,6.56194892 C6.9538389,6.16999607 7.59233006,6.16999607 7.98428291,6.56194892 L9.40667976,7.98434578 L10.8290766,6.56194892 L9.40667976,5.13955206 C8.22789784,3.96127308 6.31827112,3.96127308 5.13948919,5.13955206 L0.872298625,9.40674263 C-0.306483301,10.5855246 -0.306483301,12.4961257 0.872298625,13.6739332 L2.29469548,15.0963301 C3.47250295,16.275112 5.38310413,16.275112 6.56188605,15.0963301 L8.96660118,12.6916149 C8.17191356,12.7446444 7.37131631,12.5658782 6.66209823,12.1513242 L6.66209823,12.1513242 Z" id="Shape"></path>
                        </g>
                    </g>
                </g>
            </g>
        </g>
    </g>
</svg>
</span>
    </a>
  </li>
  

  
  <li class="ng-scope" ng-controller="Github as github">
    
      <a class="import-github unlinked" ng-click="github.chooseScope()">
    
      <span>Github</span>
      <span class="icon icon-link"><svg viewBox="0 0 16 16" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g sketch:type="MSArtboardGroup" transform="translate(-238.000000, -212.000000)" fill="#FFFFFF">
            <g sketch:type="MSLayerGroup" transform="translate(0.000000, 65.000000)">
                <g transform="translate(0.000000, 75.000000)" sketch:type="MSShapeGroup">
                    <g transform="translate(32.000000, 72.000000)">
                        <g transform="translate(206.000000, 0.000000)">
                            <path d="M15.0962672,6.56194892 L10.8290766,10.8291395 C9.6502947,12.0079214 7.74066798,12.0079214 6.56188605,10.8291395 L5.13948919,9.40674263 L6.56188605,7.98434578 L7.98428291,9.40674263 C8.37721022,9.79869548 9.01472692,9.79966994 9.40667976,9.40674263 L13.6738703,5.13955206 C14.0658232,4.7471277 14.0658232,4.10910806 13.6738703,3.71665226 L12.2514735,2.2942554 C11.8595206,1.90230255 11.2210295,1.90230255 10.8290766,2.2942554 L9.30549312,3.8178389 C8.59919843,3.40231041 7.79665226,3.22401572 7.00196464,3.27754813 L9.40667976,0.871858546 C10.5854617,-0.306420432 12.4960629,-0.306420432 13.6738703,0.871858546 L15.0962672,2.2942554 C16.2750491,3.47256582 16.2750491,5.38366994 15.0962672,6.56194892 L15.0962672,6.56194892 Z M6.66209823,12.1513242 L5.13948919,13.6739332 C4.74656189,14.0668605 4.10904519,14.0658861 3.71709234,13.6739332 L2.29469548,12.2515363 C1.90176817,11.8595835 1.90176817,11.2220668 2.29469548,10.8291395 L6.56188605,6.56194892 C6.9538389,6.16999607 7.59233006,6.16999607 7.98428291,6.56194892 L9.40667976,7.98434578 L10.8290766,6.56194892 L9.40667976,5.13955206 C8.22789784,3.96127308 6.31827112,3.96127308 5.13948919,5.13955206 L0.872298625,9.40674263 C-0.306483301,10.5855246 -0.306483301,12.4961257 0.872298625,13.6739332 L2.29469548,15.0963301 C3.47250295,16.275112 5.38310413,16.275112 6.56188605,15.0963301 L8.96660118,12.6916149 C8.17191356,12.7446444 7.37131631,12.5658782 6.66209823,12.1513242 L6.66209823,12.1513242 Z" id="Shape"></path>
                        </g>
                    </g>
                </g>
            </g>
        </g>
    </g>
</svg>
</span>
    </a>
  </li>
  

  
  <li class="ng-scope" ng-controller="Googledrive as googledrive">
    
      <a href="http://dillinger.io/redirect/googledrive" class="import-google-drive unlinked">
    
      <span>Google Drive</span>
      <span class="icon icon-link"><svg viewBox="0 0 16 16" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g sketch:type="MSArtboardGroup" transform="translate(-238.000000, -212.000000)" fill="#FFFFFF">
            <g sketch:type="MSLayerGroup" transform="translate(0.000000, 65.000000)">
                <g transform="translate(0.000000, 75.000000)" sketch:type="MSShapeGroup">
                    <g transform="translate(32.000000, 72.000000)">
                        <g transform="translate(206.000000, 0.000000)">
                            <path d="M15.0962672,6.56194892 L10.8290766,10.8291395 C9.6502947,12.0079214 7.74066798,12.0079214 6.56188605,10.8291395 L5.13948919,9.40674263 L6.56188605,7.98434578 L7.98428291,9.40674263 C8.37721022,9.79869548 9.01472692,9.79966994 9.40667976,9.40674263 L13.6738703,5.13955206 C14.0658232,4.7471277 14.0658232,4.10910806 13.6738703,3.71665226 L12.2514735,2.2942554 C11.8595206,1.90230255 11.2210295,1.90230255 10.8290766,2.2942554 L9.30549312,3.8178389 C8.59919843,3.40231041 7.79665226,3.22401572 7.00196464,3.27754813 L9.40667976,0.871858546 C10.5854617,-0.306420432 12.4960629,-0.306420432 13.6738703,0.871858546 L15.0962672,2.2942554 C16.2750491,3.47256582 16.2750491,5.38366994 15.0962672,6.56194892 L15.0962672,6.56194892 Z M6.66209823,12.1513242 L5.13948919,13.6739332 C4.74656189,14.0668605 4.10904519,14.0658861 3.71709234,13.6739332 L2.29469548,12.2515363 C1.90176817,11.8595835 1.90176817,11.2220668 2.29469548,10.8291395 L6.56188605,6.56194892 C6.9538389,6.16999607 7.59233006,6.16999607 7.98428291,6.56194892 L9.40667976,7.98434578 L10.8290766,6.56194892 L9.40667976,5.13955206 C8.22789784,3.96127308 6.31827112,3.96127308 5.13948919,5.13955206 L0.872298625,9.40674263 C-0.306483301,10.5855246 -0.306483301,12.4961257 0.872298625,13.6739332 L2.29469548,15.0963301 C3.47250295,16.275112 5.38310413,16.275112 6.56188605,15.0963301 L8.96660118,12.6916149 C8.17191356,12.7446444 7.37131631,12.5658782 6.66209823,12.1513242 L6.66209823,12.1513242 Z" id="Shape"></path>
                        </g>
                    </g>
                </g>
            </g>
        </g>
    </g>
</svg>
</span>
    </a>
  </li>
  

  
  <li class="ng-scope" ng-controller="Onedrive as onedrive">
    
      <a href="http://dillinger.io/redirect/onedrive" class="import-one-drive unlinked">
    
      <span>One Drive</span>
      <span class="icon icon-link"><svg viewBox="0 0 16 16" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g sketch:type="MSArtboardGroup" transform="translate(-238.000000, -212.000000)" fill="#FFFFFF">
            <g sketch:type="MSLayerGroup" transform="translate(0.000000, 65.000000)">
                <g transform="translate(0.000000, 75.000000)" sketch:type="MSShapeGroup">
                    <g transform="translate(32.000000, 72.000000)">
                        <g transform="translate(206.000000, 0.000000)">
                            <path d="M15.0962672,6.56194892 L10.8290766,10.8291395 C9.6502947,12.0079214 7.74066798,12.0079214 6.56188605,10.8291395 L5.13948919,9.40674263 L6.56188605,7.98434578 L7.98428291,9.40674263 C8.37721022,9.79869548 9.01472692,9.79966994 9.40667976,9.40674263 L13.6738703,5.13955206 C14.0658232,4.7471277 14.0658232,4.10910806 13.6738703,3.71665226 L12.2514735,2.2942554 C11.8595206,1.90230255 11.2210295,1.90230255 10.8290766,2.2942554 L9.30549312,3.8178389 C8.59919843,3.40231041 7.79665226,3.22401572 7.00196464,3.27754813 L9.40667976,0.871858546 C10.5854617,-0.306420432 12.4960629,-0.306420432 13.6738703,0.871858546 L15.0962672,2.2942554 C16.2750491,3.47256582 16.2750491,5.38366994 15.0962672,6.56194892 L15.0962672,6.56194892 Z M6.66209823,12.1513242 L5.13948919,13.6739332 C4.74656189,14.0668605 4.10904519,14.0658861 3.71709234,13.6739332 L2.29469548,12.2515363 C1.90176817,11.8595835 1.90176817,11.2220668 2.29469548,10.8291395 L6.56188605,6.56194892 C6.9538389,6.16999607 7.59233006,6.16999607 7.98428291,6.56194892 L9.40667976,7.98434578 L10.8290766,6.56194892 L9.40667976,5.13955206 C8.22789784,3.96127308 6.31827112,3.96127308 5.13948919,5.13955206 L0.872298625,9.40674263 C-0.306483301,10.5855246 -0.306483301,12.4961257 0.872298625,13.6739332 L2.29469548,15.0963301 C3.47250295,16.275112 5.38310413,16.275112 6.56188605,15.0963301 L8.96660118,12.6916149 C8.17191356,12.7446444 7.37131631,12.5658782 6.66209823,12.1513242 L6.66209823,12.1513242 Z" id="Shape"></path>
                        </g>
                    </g>
                </g>
            </g>
        </g>
    </g>
</svg>
</span>
    </a>
  </li>
  

        </ul>
      </li>
      <li class="menu-item menu-item-icon menu-item--preview">
        <a class="menu-link menu-link-preview" preview-toggle="">
          <i class="icon icon-preview"><svg viewBox="0 0 19 12" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g id="Page-1" stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g id="Mobile---Home" sketch:type="MSArtboardGroup" transform="translate(-234.000000, -85.000000)" fill="#D3DBEB">
            <g id="Navigation" sketch:type="MSLayerGroup" transform="translate(0.000000, 65.000000)">
                <g id="Menu-Item:-Preview" transform="translate(218.000000, 0.000000)" sketch:type="MSShapeGroup">
                    <g id="Icon:-Preview" transform="translate(17.000000, 20.000000)">
                        <path d="M8.90526316,0.0311320755 C3.00069474,0.0311320755 -0.0947368421,4.79641509 -0.0947368421,5.55188679 C-0.0947368421,6.30756604 3.00069474,11.0726415 8.90526316,11.0726415 C14.8094526,11.0726415 17.9052632,6.30756604 17.9052632,5.55188679 C17.9052632,4.79641509 14.8094526,0.0311320755 8.90526316,0.0311320755 L8.90526316,0.0311320755 Z M8.90526316,9.79871698 C6.69618947,9.79871698 4.90509474,7.89737736 4.90509474,5.55188679 C4.90509474,3.20639623 6.69618947,1.3050566 8.90526316,1.3050566 C11.1143368,1.3050566 12.9052421,3.20639623 12.9052421,5.55188679 C12.9052421,7.89737736 11.1143368,9.79871698 8.90526316,9.79871698 L8.90526316,9.79871698 Z M10.9051579,5.55188679 C10.9051579,6.7245283 10.0095158,7.67550943 8.90526316,7.67550943 C7.80063158,7.67550943 6.90536842,6.7245283 6.90536842,5.55188679 C6.90536842,4.37924528 7.80063158,3.4284717 8.90526316,3.4284717 C9.50210526,3.4284717 8.53901053,5.11126415 8.90526316,5.55188679 C9.21656842,5.92609434 10.9051579,5.01330189 10.9051579,5.55188679 L10.9051579,5.55188679 Z" id="Shape"></path>
                    </g>
                </g>
            </g>
        </g>
    </g>
</svg>
</i>
          <span class="sr-only">Preview</span>
        </a>
      </li>
      <li class="menu-item menu-item-icon menu-item--settings">
        <a class="menu-link menu-link-settings" settings-toggle="">
          <i class="icon icon-settings"><svg viewBox="0 0 18 18" version="1.1" xmlns="http://www.w3.org/2000/svg" xlink="http://www.w3.org/1999/xlink" xmlns:sketch="http://www.bohemiancoding.com/sketch/ns">
    <g id="Page-1" stroke="none" stroke-width="1" fill="none" fill-rule="evenodd" sketch:type="MSPage">
        <g id="Mobile---Home" sketch:type="MSArtboardGroup" transform="translate(-286.000000, -81.000000)" fill="#D3DBEB">
            <g id="Navigation" sketch:type="MSLayerGroup" transform="translate(0.000000, 65.000000)">
                <g id="Menu-Item:-Settings" transform="translate(269.000000, 0.000000)" sketch:type="MSShapeGroup">
                    <g id="Icon:-Settings" transform="translate(17.000000, 16.000000)">
                        <path d="M17.341654,7.68935491 L15.5585491,7.39189286 C15.4289531,6.86205804 15.2399933,6.35558705 14.9938996,5.88320759 L16.1353929,4.46787723 C16.3781719,4.16876786 16.3671429,3.73839509 16.1111853,3.44971205 L15.4020134,2.65156473 C15.1449509,2.36318304 14.7186763,2.30219196 14.3934911,2.507625 L12.8685937,3.46620536 C12.1978929,2.99760268 11.4381562,2.65156473 10.6185937,2.45077232 L10.3202679,0.658104911 C10.2576696,0.278517857 9.92918973,0 9.54359598,0 L8.47488616,0 C8.09007589,0 7.76049107,0.278517857 7.69925893,0.658104911 L7.39932589,2.45133482 C6.72173437,2.61723214 6.0864308,2.88502232 5.50665402,3.23770982 L4.05644866,2.20275 C3.74389955,1.97945759 3.31461161,2.01437277 3.04185937,2.28654241 L2.28684375,3.04210045 C2.01467411,3.31485268 1.97975893,3.74414062 2.20363393,4.05668973 L3.24100446,5.50966741 C2.8916317,6.08506473 2.62659375,6.71675223 2.46071652,7.38829687 L0.65784375,7.68935491 C0.278839286,7.75195312 -0.000220982143,8.08047321 -0.000220982143,8.46606696 L-0.000220982143,9.53393304 C-0.000220982143,9.91952679 0.278839286,10.2480469 0.65784375,10.3106451 L2.46071652,10.6117031 C2.59694196,11.1642991 2.79470089,11.6921853 3.05754911,12.1816205 L1.92125893,13.5889554 C1.67930357,13.8877835 1.68946875,14.3184375 1.94542634,14.6068594 L2.6540558,15.4050067 C2.91113839,15.6939308 3.33765402,15.753817 3.66285937,15.5489063 L5.20973437,14.5771875 C5.86426339,15.0248772 6.60473437,15.3539196 7.39932589,15.5489063 L7.69925893,17.341875 C7.76049107,17.7215022 8.09007589,18 8.47488616,18 L9.54359598,18 C9.92918973,18 10.2576696,17.7215022 10.3202679,17.3418951 L10.619096,15.5489263 C11.2882299,15.3852589 11.9154777,15.1221295 12.4900915,14.7754888 L14.0007254,15.8543839 C14.3127321,16.0785 14.7423214,16.0433437 15.0147924,15.7703103 L15.7700893,15.0150134 C16.0419576,14.7430848 16.0787812,14.3140379 15.8529978,14.0009464 L14.7774978,12.4930647 C15.1278951,11.9157388 15.3943192,11.2829263 15.5591518,10.6083683 L17.3422366,10.310625 C17.722346,10.2480268 18.0002812,9.9195067 18.0002812,9.53391295 L18.0002812,8.46604688 C17.9997388,8.08047321 17.7218036,7.75195312 17.341654,7.68935491 L17.341654,7.68935491 Z M9.0283058,12.375 C7.1647433,12.375 5.6533058,10.8632812 5.6533058,9 C5.6533058,7.1364375 7.1647433,5.625 9.0283058,5.625 C10.8915469,5.625 12.4032656,7.1364375 12.4032656,9 C12.4032656,10.8632812 10.8915469,12.375 9.0283058,12.375 L9.0283058,12.375 Z" id="settings_1_"></path>
                    </g>
                </g>
            </g>
        </g>
    </g>
</svg>
</i>
          <span class="sr-only">Settings</span>
        </a>
      </li>
    </ul>
  </nav>
</div>

      <div class="overlay"></div>

      <div class="header">
  <h2 class="title">Document Name</h2>
  <document-title><input value="Untitled Document.md" class="title-document ng-pristine ng-untouched ng-valid" name="documentTitle" ng-change="updateDocument()" ng-model-options="{ debounce: 500 }" ng-model="currentDocument.title" type="text">
</document-title>

  <!-- ngIf: profile.enableWordsCount --><p ng-if="profile.enableWordsCount" class="words ng-scope">Words: <span class="counter ng-binding" ng-bind="words">645</span></p><!-- end ngIf: profile.enableWordsCount -->
</div>

      <div class="g mnone">
        <div class="editor-header editor-header--first">
  <h3 class="title">Markdown</h3>
  <a class="enter-zen-mode" ng-click="zenmode.toggle()">Toggle Zen Mode</a>
</div>
<div class="editor-header">
  <h3 class="title">Preview</h3>
</div>

        <div class="g-b g-b--t1of2 split split-editor">
  <div draggable="false" style="height: 4312px;" class=" ace_editor ace-dillinger ace_focus" id="editor"><textarea style="opacity: 0; height: 28px; width: 7px; left: 395px; top: 420px;" spellcheck="false" class="ace_text-input" wrap="off"></textarea><div class="ace_gutter"><div style="margin-top: 0px; height: 4368px; width: 54px;" class="ace_layer ace_gutter-layer ace_folding-enabled"><div style="height: 28px;" class="ace_gutter-cell ">1<span style="height: 28px;" class="ace_fold-widget ace_start ace_open"></span></div><div style="height: 28px;" class="ace_gutter-cell ">2</div><div style="height: 28px;" class="ace_gutter-cell ">3</div><div style="height: 28px;" class="ace_gutter-cell ">4</div><div style="height: 28px;" class="ace_gutter-cell ">5</div><div style="height: 28px;" class="ace_gutter-cell ">6</div><div style="height: 28px;" class="ace_gutter-cell ">7</div><div style="height: 28px;" class="ace_gutter-cell ">8</div><div style="height: 28px;" class="ace_gutter-cell ">9</div><div style="height: 28px;" class="ace_gutter-cell ">10</div><div style="height: 28px;" class="ace_gutter-cell ">11</div><div style="height: 28px;" class="ace_gutter-cell ">12</div><div style="height: 28px;" class="ace_gutter-cell ">13<span style="height: 28px;" class="ace_fold-widget ace_start ace_open"></span></div><div style="height: 28px;" class="ace_gutter-cell ">14</div><div style="height: 28px;" class="ace_gutter-cell ">15<span style="height: 28px;" class="ace_fold-widget ace_end ace_open"></span></div><div style="height: 28px;" class="ace_gutter-cell ">16</div><div style="height: 28px;" class="ace_gutter-cell ">17</div><div style="height: 28px;" class="ace_gutter-cell ">18</div><div style="height: 28px;" class="ace_gutter-cell ">19</div><div style="height: 28px;" class="ace_gutter-cell ">20<span style="height: 28px;" class="ace_fold-widget ace_end ace_open"></span></div><div style="height: 28px;" class="ace_gutter-cell ">21</div><div style="height: 28px;" class="ace_gutter-cell ">22</div><div style="height: 28px;" class="ace_gutter-cell ">23</div><div style="height: 28px;" class="ace_gutter-cell ">24</div><div style="height: 28px;" class="ace_gutter-cell ">25</div><div style="height: 28px;" class="ace_gutter-cell ">26</div><div style="height: 28px;" class="ace_gutter-cell ">27</div><div style="height: 28px;" class="ace_gutter-cell ">28</div><div style="height: 28px;" class="ace_gutter-cell ">29</div><div style="height: 28px;" class="ace_gutter-cell ">30</div><div style="height: 28px;" class="ace_gutter-cell ">31<span style="height: 28px;" class="ace_fold-widget ace_start ace_open"></span></div><div style="height: 28px;" class="ace_gutter-cell ">32</div><div style="height: 28px;" class="ace_gutter-cell ">33</div><div style="height: 28px;" class="ace_gutter-cell ">34</div><div style="height: 28px;" class="ace_gutter-cell ">35</div><div style="height: 28px;" class="ace_gutter-cell ">36</div><div style="height: 28px;" class="ace_gutter-cell ">37</div><div style="height: 28px;" class="ace_gutter-cell ">38</div><div style="height: 28px;" class="ace_gutter-cell ">39<span style="height: 28px;" class="ace_fold-widget ace_start ace_open"></span></div><div style="height: 56px;" class="ace_gutter-cell ">40</div><div style="height: 28px;" class="ace_gutter-cell ">41</div><div style="height: 28px;" class="ace_gutter-cell ">42</div><div style="height: 28px;" class="ace_gutter-cell ">43<span style="height: 28px;" class="ace_fold-widget ace_start ace_open"></span></div><div style="height: 28px;" class="ace_gutter-cell ">44</div><div style="height: 28px;" class="ace_gutter-cell ">45<span style="height: 28px;" class="ace_fold-widget ace_end ace_open"></span></div><div style="height: 28px;" class="ace_gutter-cell ">46</div><div style="height: 28px;" class="ace_gutter-cell ">47<span style="height: 28px;" class="ace_fold-widget ace_start ace_open"></span></div><div style="height: 28px;" class="ace_gutter-cell ">48<span style="height: 28px;" class="ace_fold-widget ace_start ace_open"></span></div><div style="height: 28px;" class="ace_gutter-cell ">49</div><div style="height: 28px;" class="ace_gutter-cell ">50<span style="height: 28px;" class="ace_fold-widget ace_end ace_open"></span></div><div style="height: 28px;" class="ace_gutter-cell ">51</div><div style="height: 56px;" class="ace_gutter-cell ">52</div><div style="height: 28px;" class="ace_gutter-cell ">53</div><div style="height: 28px;" class="ace_gutter-cell ">54</div><div style="height: 28px;" class="ace_gutter-cell ">55</div><div style="height: 28px;" class="ace_gutter-cell ">56</div><div style="height: 28px;" class="ace_gutter-cell ">57</div><div style="height: 28px;" class="ace_gutter-cell ">58</div><div style="height: 28px;" class="ace_gutter-cell ">59</div><div style="height: 28px;" class="ace_gutter-cell ">60</div><div style="height: 28px;" class="ace_gutter-cell ">61</div><div style="height: 28px;" class="ace_gutter-cell ">62</div><div style="height: 28px;" class="ace_gutter-cell ">63<span style="height: 28px;" class="ace_fold-widget ace_start ace_open"></span></div><div style="height: 28px;" class="ace_gutter-cell ">64</div><div style="height: 28px;" class="ace_gutter-cell ">65<span style="height: 28px;" class="ace_fold-widget ace_end ace_open"></span></div><div style="height: 28px;" class="ace_gutter-cell ">66</div><div style="height: 28px;" class="ace_gutter-cell ">67</div><div style="height: 28px;" class="ace_gutter-cell ">68<span style="height: 28px;" class="ace_fold-widget ace_start ace_open"></span></div><div style="height: 28px;" class="ace_gutter-cell ">69</div><div style="height: 28px;" class="ace_gutter-cell ">70<span style="height: 28px;" class="ace_fold-widget ace_start ace_open"></span></div><div style="height: 28px;" class="ace_gutter-cell ">71</div><div style="height: 28px;" class="ace_gutter-cell ">72</div><div style="height: 28px;" class="ace_gutter-cell ">73</div><div style="height: 28px;" class="ace_gutter-cell ">74</div><div style="height: 28px;" class="ace_gutter-cell ">75</div><div style="height: 28px;" class="ace_gutter-cell ">76</div><div style="height: 28px;" class="ace_gutter-cell ">77</div><div style="height: 28px;" class="ace_gutter-cell ">78</div><div style="height: 28px;" class="ace_gutter-cell ">79</div><div style="height: 28px;" class="ace_gutter-cell ">80</div><div style="height: 28px;" class="ace_gutter-cell ">81</div><div style="height: 28px;" class="ace_gutter-cell ">82</div><div style="height: 28px;" class="ace_gutter-cell ">83</div><div style="height: 28px;" class="ace_gutter-cell ">84</div><div style="height: 28px;" class="ace_gutter-cell ">85</div><div style="height: 28px;" class="ace_gutter-cell ">86</div><div style="height: 28px;" class="ace_gutter-cell ">87<span style="height: 28px;" class="ace_fold-widget ace_end ace_open"></span></div><div style="height: 28px;" class="ace_gutter-cell ">88</div><div style="height: 28px;" class="ace_gutter-cell ">89<span style="height: 28px;" class="ace_fold-widget ace_end ace_open"></span></div><div style="height: 28px;" class="ace_gutter-cell ">90</div><div style="height: 28px;" class="ace_gutter-cell ">91</div><div style="height: 28px;" class="ace_gutter-cell ">92</div><div style="height: 28px;" class="ace_gutter-cell ">93</div><div style="height: 28px;" class="ace_gutter-cell ">94</div><div style="height: 28px;" class="ace_gutter-cell ">95</div><div style="height: 28px;" class="ace_gutter-cell ">96</div><div style="height: 28px;" class="ace_gutter-cell ">97</div><div style="height: 28px;" class="ace_gutter-cell ">98</div><div style="height: 28px;" class="ace_gutter-cell ">99</div><div style="height: 28px;" class="ace_gutter-cell ">100</div><div style="height: 28px;" class="ace_gutter-cell ">101</div><div style="height: 28px;" class="ace_gutter-cell ">102</div><div style="height: 28px;" class="ace_gutter-cell ">103</div><div style="height: 28px;" class="ace_gutter-cell ">104</div><div style="height: 28px;" class="ace_gutter-cell ">105</div><div style="height: 28px;" class="ace_gutter-cell ">106</div><div style="height: 28px;" class="ace_gutter-cell ">107</div><div style="height: 28px;" class="ace_gutter-cell ">108</div><div style="height: 28px;" class="ace_gutter-cell ">109</div><div style="height: 28px;" class="ace_gutter-cell ">110</div><div style="height: 28px;" class="ace_gutter-cell ">111</div><div style="height: 28px;" class="ace_gutter-cell ">112</div><div style="height: 28px;" class="ace_gutter-cell ">113</div><div style="height: 28px;" class="ace_gutter-cell ">114</div><div style="height: 28px;" class="ace_gutter-cell ">115</div><div style="height: 56px;" class="ace_gutter-cell ">116<span style="height: 28px;" class="ace_fold-widget ace_start ace_open"></span></div><div style="height: 28px;" class="ace_gutter-cell ">117</div><div style="height: 56px;" class="ace_gutter-cell ">118</div><div style="height: 56px;" class="ace_gutter-cell ">119</div><div style="height: 56px;" class="ace_gutter-cell ">120</div><div style="height: 56px;" class="ace_gutter-cell ">121</div><div style="height: 56px;" class="ace_gutter-cell ">122</div><div style="height: 56px;" class="ace_gutter-cell ">123</div><div style="height: 56px;" class="ace_gutter-cell ">124</div><div style="height: 56px;" class="ace_gutter-cell ">125</div><div style="height: 56px;" class="ace_gutter-cell ">126</div><div style="height: 56px;" class="ace_gutter-cell ">127</div><div style="height: 56px;" class="ace_gutter-cell ">128</div><div style="height: 56px;" class="ace_gutter-cell ">129</div><div style="height: 56px;" class="ace_gutter-cell ">130</div><div style="height: 56px;" class="ace_gutter-cell ">131</div><div style="height: 56px;" class="ace_gutter-cell ">132<span style="height: 28px;" class="ace_fold-widget ace_start ace_open"></span></div><div style="height: 28px;" class="ace_gutter-cell ">133</div><div style="height: 28px;" class="ace_gutter-cell ">134</div><div style="height: 28px;" class="ace_gutter-cell ">135</div><div style="height: 28px;" class="ace_gutter-cell ">136</div></div><div style="top: 420px; height: 28px;" class="ace_gutter-active-line"></div></div><div style="left: 55px; right: 0px; bottom: 0px;" class="ace_scroller"><div style="margin-top: 0px; width: 859px; height: 4368px; margin-left: 0px;" class="ace_content"><div class="ace_layer ace_print-margin-layer"><div style="left: 564px; visibility: hidden;" class="ace_print-margin"></div></div><div class="ace_layer ace_marker-layer"><div class="ace_active-line" style="height:28px;top:420px;left:0;right:0;"></div></div><div style="padding: 0px 4px;" class="ace_layer ace_text-layer"><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_heading ace_1">#</span><span class="ace_heading">&nbsp;ProQDock</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_text ace_xml">Requires&nbsp;PERL&nbsp;(v.5.8&nbsp;or&nbsp;higher),&nbsp;and&nbsp;a&nbsp;fortran90&nbsp;compiler&nbsp;(prefered:&nbsp;ifort)</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_text ace_xml">and&nbsp;five&nbsp;additional&nbsp;packages&nbsp;to&nbsp;be&nbsp;pre-installed</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_list">1.&nbsp;</span><span class="ace_list">sc&nbsp;from&nbsp;ccp4&nbsp;(http://www.ccp4.ac.uk/)&nbsp;[executable_name:&nbsp;sc]</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_list">2.&nbsp;</span><span class="ace_list">delphi&nbsp;v.6.2&nbsp;(http://compbio.clemson.edu/delphi/)&nbsp;[executable_name:&nbsp;delphi95]</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_list">3.&nbsp;</span><span class="ace_list">EDTSurf&nbsp;(http://zhanglab.ccmb.med.umich.edu/EDTSurf/)&nbsp;[executable_name:&nbsp;EDTSurf]</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_list">4.&nbsp;</span><span class="ace_list">Rosetta&nbsp;(https://www.rosettacommons.org/software/)</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_list">5.&nbsp;</span><span class="ace_list">ProQ2&nbsp;(https://github.com/bjornwallner/ProQ_scripts/)</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_list">6.&nbsp;</span><span class="ace_list">needle&nbsp;from&nbsp;EMBOS&nbsp;(http://emboss.sourceforge.net/download/)</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_heading ace_3">###</span><span class="ace_heading">&nbsp;Installation</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_support ace_function">```sh</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_support ace_function">$&nbsp;git&nbsp;clone&nbsp;https://github.com/nemo8130/ProQDock</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_support ace_function">$&nbsp;cd&nbsp;ProQDock</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_support ace_function">$&nbsp;chmod&nbsp;+x&nbsp;compileF</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_support ace_function">$&nbsp;./compileF&nbsp;&lt;fortran90-compiler&gt;&nbsp;&nbsp;(Default:&nbsp;ifort)</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_support ace_function">```</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_text ace_xml">open&nbsp;</span><span class="ace_support ace_function">`ProQDock.bash`</span><span class="ace_text ace_xml">&nbsp;in&nbsp;any&nbsp;text&nbsp;editor&nbsp;and&nbsp;change&nbsp;the&nbsp;fullpaths&nbsp;for&nbsp;the&nbsp;following&nbsp;fields&nbsp;according&nbsp;to&nbsp;your&nbsp;system:&nbsp;</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_list">-&nbsp;</span><span class="ace_list">sc_path=/software/apps/ccp4/ccp4-6.5.0/ccp4-6.5/bin</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_list">-&nbsp;</span><span class="ace_list">delphi</span><span class="ace_string ace_emphasis">_path=/home/x_</span><span class="ace_list">sabas/bin</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_list">-&nbsp;</span><span class="ace_list">rosetta</span><span class="ace_string ace_emphasis">_path=/proj/wallner/users/x_</span><span class="ace_list">bjowa/github/Rosetta/main/source/bin</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_list">-&nbsp;</span><span class="ace_list">rosetta</span><span class="ace_string ace_emphasis">_db=/proj/wallner/users/x_</span><span class="ace_list">bjowa/github/Rosetta/main/database</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_list">-&nbsp;</span><span class="ace_list">svm</span><span class="ace_string ace_emphasis">_path=/home/x_</span><span class="ace_list">sabas/proj/svm_light</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_list">-&nbsp;</span><span class="ace_list">ESpath=/home/x_sabas/proj/EDTSurf</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_list">-&nbsp;</span><span class="ace_list">proqpath=/home/x</span><span class="ace_string ace_emphasis">_sabas/proj/BACKUP/ProQ_</span><span class="ace_list">scripts/bin</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_heading ace_2">##</span><span class="ace_heading">&nbsp;The&nbsp;program&nbsp;has&nbsp;two&nbsp;mandetory&nbsp;inputs&nbsp;:</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_support ace_function">&nbsp;&nbsp;&nbsp;&nbsp;1.&nbsp;The&nbsp;coordinate&nbsp;(PDB)&nbsp;file&nbsp;for&nbsp;the&nbsp;model</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_support ace_function">&nbsp;&nbsp;&nbsp;&nbsp;2.&nbsp;The&nbsp;full-length&nbsp;target&nbsp;sequence</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_list">-&nbsp;</span><span class="ace_list">The&nbsp;model&nbsp;must&nbsp;be&nbsp;a&nbsp;perfect&nbsp;or&nbsp;imperfect&nbsp;subset&nbsp;of&nbsp;the&nbsp;target&nbsp;sequence&nbsp;</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_list">-&nbsp;</span><span class="ace_list">i.e.,&nbsp;Any&nbsp;and&nbsp;all&nbsp;sub-sequence&nbsp;present&nbsp;in&nbsp;the&nbsp;model&nbsp;must&nbsp;be&nbsp;present&nbsp;in&nbsp;the&nbsp;tareget</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_heading ace_5">#####</span><span class="ace_heading">&nbsp;Preparatory&nbsp;Step:&nbsp;</span></div></div><div class="ace_line_group" style="height:56px"><div class="ace_line" style="height:28px"><span class="ace_text ace_xml">The&nbsp;fulllength&nbsp;(FASTA)&nbsp;target&nbsp;sequence&nbsp;(one&nbsp;letter&nbsp;amino&nbsp;acid&nbsp;code&nbsp;followed&nbsp;by&nbsp;a&nbsp;header&nbsp;starting&nbsp;with&nbsp;&gt;)&nbsp;should&nbsp;be&nbsp;</span></div><div class="ace_line" style="height:28px"><span class="ace_text ace_xml">prepared&nbsp;and&nbsp;provided&nbsp;alongside&nbsp;the&nbsp;coordinate&nbsp;(PDB)&nbsp;file&nbsp;for&nbsp;the&nbsp;model.</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_text ace_xml">You&nbsp;can&nbsp;generate&nbsp;the&nbsp;fasta&nbsp;sequence&nbsp;by&nbsp;using:</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_support ace_function">```sh</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_support ace_function">$&nbsp;./ProQDock/EXEC/pdb2fasta.pl&nbsp;&lt;input.pdb&gt;&nbsp;&nbsp;&gt;&nbsp;&nbsp;input.pdb.fasta</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_support ace_function">```</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_heading ace_5">#####</span><span class="ace_heading">&nbsp;Run&nbsp;Step:&nbsp;</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_support ace_function">```sh</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_support ace_function">$&nbsp;./ProQDock.bash&nbsp;&lt;model.PDB&gt;&nbsp;&lt;fulllength.fasta&gt;</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_support ace_function">```</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_text ace_xml">where,</span></div></div><div class="ace_line_group" style="height:56px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_list">-&nbsp;</span><span class="ace_list">model.pdb:&nbsp;The&nbsp;input&nbsp;pdb&nbsp;(coordinate&nbsp;file&nbsp;in&nbsp;Brrokheaven&nbsp;format;&nbsp;http://www.ccp4.ac.uk/html/procheck_man/manappb.html)&nbsp;</span></div><div class="ace_line" style="height:28px"><span class="ace_list">file</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_list">-&nbsp;</span><span class="ace_list">fulllength.fasta:&nbsp;The&nbsp;fulllength&nbsp;fasta&nbsp;sequence&nbsp;of&nbsp;the&nbsp;target</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_markup ace_list">-&nbsp;</span><span class="ace_list">optional&nbsp;arguments:&nbsp;-d&nbsp;(0/1)&nbsp;:&nbsp;delphi_mode:&nbsp;</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_list">&gt;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0:&nbsp;Single</span><span class="ace_string ace_emphasis">_internal_</span><span class="ace_list">dielectric&nbsp;(epsilon=2)&nbsp;</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_list">&gt;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1:&nbsp;Multidielctric&nbsp;mode&nbsp;(internal&nbsp;dielectric&nbsp;modeled&nbsp;as&nbsp;a&nbsp;Gaussian&nbsp;Function)</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_list">&nbsp;&nbsp;&nbsp;</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_list">&gt;&nbsp;If&nbsp;you&nbsp;intend&nbsp;to&nbsp;Run&nbsp;ProQDock&nbsp;for&nbsp;a&nbsp;list&nbsp;of&nbsp;PDB&nbsp;files&nbsp;(models)&nbsp;corresponding&nbsp;to&nbsp;the&nbsp;same&nbsp;target&nbsp;sequence,</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_list">then,&nbsp;SKIP&nbsp;repeating&nbsp;the&nbsp;profile&nbsp;generation&nbsp;step&nbsp;more&nbsp;than&nbsp;once,&nbsp;by&nbsp;providing&nbsp;a&nbsp;[consistent&nbsp;basename]&nbsp;for&nbsp;the&nbsp;fasta&nbsp;file.</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_text ace_xml">Launch&nbsp;the&nbsp;following&nbsp;bash&nbsp;script&nbsp;which&nbsp;runs&nbsp;on&nbsp;a&nbsp;set&nbsp;of&nbsp;(example)&nbsp;models&nbsp;kept&nbsp;at&nbsp;./EXAMPLE</span><span class="ace_string ace_emphasis">_CASE_</span><span class="ace_text ace_xml">STUDY/</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_support ace_function">```sh</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_support ace_function">$&nbsp;./runex.bash</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_support ace_function">```</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">&gt;&nbsp;EXAMPLE&nbsp;OUTPUT:&nbsp;</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">```sh&nbsp;</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">$&nbsp;cat&nbsp;EXAMPLE</span><span class="ace_string ace_emphasis">_CASE_</span><span class="ace_string ace_blockquote">STUDY/D1OYV-models.status.results&nbsp;</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">```</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">&gt;&nbsp;</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;&nbsp;&nbsp;&nbsp;Model&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;fnat&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;LRMS&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;iRMS&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;CAPRI_class&nbsp;&nbsp;&nbsp;&nbsp;DockQ&nbsp;&nbsp;&nbsp;&nbsp;ProQDock</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;D1OYV-a102a&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.000&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;35.348&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;15.846&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Incorrect&nbsp;&nbsp;&nbsp;&nbsp;0.02118&nbsp;&nbsp;&nbsp;&nbsp;0.05908</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;D1OYV-a104c&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.000&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;44.703&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;16.626&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Incorrect&nbsp;&nbsp;&nbsp;&nbsp;0.01432&nbsp;&nbsp;&nbsp;&nbsp;0.21024</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;D1OYV-a106c&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.000&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;44.600&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;18.136&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Incorrect&nbsp;&nbsp;&nbsp;&nbsp;0.01395&nbsp;&nbsp;&nbsp;&nbsp;0.03890</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;D1OYV-a115c&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.000&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;31.946&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;11.941&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Incorrect&nbsp;&nbsp;&nbsp;&nbsp;0.02722&nbsp;&nbsp;&nbsp;&nbsp;0.02730</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;&nbsp;D1OYV-a61c&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.652&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;6.820&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.334&nbsp;&nbsp;&nbsp;&nbsp;Acceptable&nbsp;&nbsp;&nbsp;&nbsp;0.51761&nbsp;&nbsp;&nbsp;&nbsp;0.51060</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;&nbsp;D1OYV-a61d&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.630&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;6.574&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.450&nbsp;&nbsp;&nbsp;&nbsp;Acceptable&nbsp;&nbsp;&nbsp;&nbsp;0.50960&nbsp;&nbsp;&nbsp;&nbsp;0.46046</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;&nbsp;D1OYV-a81d&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.587&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;6.788&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.481&nbsp;&nbsp;&nbsp;&nbsp;Acceptable&nbsp;&nbsp;&nbsp;&nbsp;0.48841&nbsp;&nbsp;&nbsp;&nbsp;0.42648</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;&nbsp;D1OYV-a41b&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.717&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;3.619&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1.428&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Medium&nbsp;&nbsp;&nbsp;&nbsp;0.69617&nbsp;&nbsp;&nbsp;&nbsp;0.48751</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;&nbsp;D1OYV-a62c&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.707&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;3.991&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1.628&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Medium&nbsp;&nbsp;&nbsp;&nbsp;0.66168&nbsp;&nbsp;&nbsp;&nbsp;0.55795</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;&nbsp;D1OYV-a82d&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.707&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.347&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1.593&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Medium&nbsp;&nbsp;&nbsp;&nbsp;0.65639&nbsp;&nbsp;&nbsp;&nbsp;0.48664</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">&gt;&nbsp;</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">&gt;&nbsp;Example&nbsp;Output&nbsp;for&nbsp;a&nbsp;single&nbsp;PDB&nbsp;file:&nbsp;</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_support ace_function">```sh</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_support ace_function">$&nbsp;./ProQDock&nbsp;./TESTPDBS/1u07.pdb&nbsp;./EXAMPLE/1u07.pdb.fasta</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_support ace_function">```</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">&gt;&nbsp;</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;&nbsp;&nbsp;&nbsp;===========================================================================================</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;&nbsp;&nbsp;&nbsp;Your&nbsp;Input:&nbsp;1u07.pdb</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;&nbsp;&nbsp;&nbsp;The&nbsp;file&nbsp;has&nbsp;a&nbsp;Proper&nbsp;extension&nbsp;(.pdb&nbsp;/&nbsp;.PDB)</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;&nbsp;&nbsp;&nbsp;Normal&nbsp;Delphi&nbsp;(single&nbsp;internal&nbsp;dielectric,&nbsp;epsilon&nbsp;=&nbsp;2.0)&nbsp;will&nbsp;be&nbsp;executed</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;&nbsp;&nbsp;&nbsp;===========================================================================================</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_empty_line">&nbsp;&nbsp;&nbsp;&nbsp;</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_text ace_xml">&nbsp;&nbsp;&nbsp;&nbsp;Total&nbsp;number&nbsp;of&nbsp;residues&nbsp;in&nbsp;the&nbsp;Complex:&nbsp;178</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_text ace_xml">&nbsp;&nbsp;&nbsp;&nbsp;EC=0.11</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_text ace_xml">&nbsp;&nbsp;&nbsp;&nbsp;Sc=0.767</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_text ace_xml">&nbsp;&nbsp;&nbsp;&nbsp;rGb=0.07170</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_text ace_xml">&nbsp;&nbsp;&nbsp;&nbsp;Ld=&nbsp;0.14815</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_text ace_xml">&nbsp;&nbsp;&nbsp;&nbsp;nBSA=0.11534</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_text ace_xml">&nbsp;&nbsp;&nbsp;&nbsp;Fintres=0.28652</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_text ace_xml">&nbsp;&nbsp;&nbsp;&nbsp;CPscore=&nbsp;0.48131</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_text ace_xml">&nbsp;&nbsp;&nbsp;&nbsp;CPM=&nbsp;0.89454</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_text ace_xml">&nbsp;&nbsp;&nbsp;&nbsp;rTs=0.08767</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_text ace_xml">&nbsp;&nbsp;&nbsp;&nbsp;Isc=0.00002</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_text ace_xml">&nbsp;&nbsp;&nbsp;&nbsp;Erep=0.27086</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_text ace_xml">&nbsp;&nbsp;&nbsp;&nbsp;Etmr=0.21257</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_text ace_xml">&nbsp;&nbsp;&nbsp;&nbsp;ProQ=&nbsp;0.845</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_text ace_xml">&nbsp;&nbsp;&nbsp;&nbsp;========================&nbsp;The&nbsp;ProQDock&nbsp;Score&nbsp;====================</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_text ace_xml">&nbsp;&nbsp;&nbsp;&nbsp;0.85573</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_text ace_xml">&nbsp;&nbsp;&nbsp;&nbsp;================================================================</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">&gt;&nbsp;</span></div></div><div class="ace_line_group" style="height:56px"><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">-------------------------------------------------------------------------------------------------------------------------</span></div><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">----------------------------------------------</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">&gt;</span></div></div><div class="ace_line_group" style="height:56px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;&nbsp;Feature&nbsp;&nbsp;&nbsp;Range&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Favorable&nbsp;Directions&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Description&nbsp;&nbsp;&nbsp;Feature</span><span class="ace_string ace_emphasis">_Type&nbsp;&nbsp;&nbsp;&nbsp;Stability*&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Native_</span><span class="ace_string ace_blockquote">values&nbsp;</span></div><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">(DB3)&nbsp;</span></div></div><div class="ace_line_group" style="height:56px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;&nbsp;EC:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[-1,1]&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Higher&nbsp;the&nbsp;Better&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Electrostatic&nbsp;balance&nbsp;at&nbsp;the&nbsp;interface&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span></div><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">&nbsp;Interface&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Variable&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.20&nbsp;(+/-0.38)</span></div></div><div class="ace_line_group" style="height:56px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;Sc:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[-1,1]&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Higher&nbsp;the&nbsp;Better&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Geometric&nbsp;/&nbsp;Steric&nbsp;fit&nbsp;at&nbsp;the&nbsp;interface&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span></div><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">Interface&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Stable&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.68&nbsp;(+/-0.17)</span></div></div><div class="ace_line_group" style="height:56px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;rGb:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[-1,1]&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Higher&nbsp;the&nbsp;Better&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Distribution&nbsp;of&nbsp;hydrophobicity&nbsp;with&nbsp;respect&nbsp;to&nbsp;burial&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span></div><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">All_Atom&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Stable&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.06&nbsp;(+/-0.02)</span></div></div><div class="ace_line_group" style="height:56px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;Ld:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[0,&nbsp;1]&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Ambiguous&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Link&nbsp;(packing)&nbsp;densely&nbsp;at&nbsp;the&nbsp;interface&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span></div><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">Interface&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Stable&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.15&nbsp;(+/-0.17)</span></div></div><div class="ace_line_group" style="height:56px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;nBSA:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[0,&nbsp;1]&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Ambiguous&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Size&nbsp;of&nbsp;the&nbsp;interface&nbsp;relative&nbsp;to&nbsp;the&nbsp;whole&nbsp;complex&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span></div><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">Interface&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Variable&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.07&nbsp;(+/-0.05)</span></div></div><div class="ace_line_group" style="height:56px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;Fintres:&nbsp;&nbsp;[0,&nbsp;1]&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Ambiguous&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Size&nbsp;of&nbsp;the&nbsp;interface&nbsp;relative&nbsp;to&nbsp;the&nbsp;whole&nbsp;complex&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span></div><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">Interface&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Variable&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.19&nbsp;(+/-0.13)</span></div></div><div class="ace_line_group" style="height:56px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;CPscore:&nbsp;&nbsp;[0,&nbsp;1]&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Higher&nbsp;the&nbsp;Better&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Preference&nbsp;of&nbsp;different&nbsp;inter-residue&nbsp;contacts&nbsp;at&nbsp;the&nbsp;interface&nbsp;&nbsp;&nbsp;&nbsp;</span></div><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">Interface&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Variable&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.35&nbsp;(+/-0.17)</span></div></div><div class="ace_line_group" style="height:56px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;CPM:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[0,&nbsp;1]&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Higher&nbsp;the&nbsp;Better&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Likelyhood&nbsp;of&nbsp;(Sc,EC)&nbsp;as&nbsp;a&nbsp;function&nbsp;of&nbsp;nBSA&nbsp;compared&nbsp;to&nbsp;native&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span></div><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">Interface&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Variable&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.80&nbsp;(+/-0.18)</span></div></div><div class="ace_line_group" style="height:56px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;rTs:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[0,&nbsp;1]&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Lower&nbsp;the&nbsp;Better&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Energetic&nbsp;Stability&nbsp;of&nbsp;the&nbsp;whole&nbsp;molecule&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span></div><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">All_Atom&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Stable&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.22&nbsp;(+/-0.21)</span></div></div><div class="ace_line_group" style="height:56px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;Isc:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[0,&nbsp;1]&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Lower&nbsp;the&nbsp;Better&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Binding&nbsp;energy&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span></div><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">Interface&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Variable&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.20&nbsp;(+/-0.33)</span></div></div><div class="ace_line_group" style="height:56px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;Erep:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[0,&nbsp;1]&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Lower&nbsp;the&nbsp;Better&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Unfavourable&nbsp;Energy&nbsp;due&nbsp;to&nbsp;atomic&nbsp;Clashes&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span></div><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">All_Atom&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Stable&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.41&nbsp;(+/-0.15)</span></div></div><div class="ace_line_group" style="height:56px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;Etmr:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[0,&nbsp;1]&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Lower&nbsp;the&nbsp;Better&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Energetic&nbsp;Stability&nbsp;without&nbsp;condiderating&nbsp;clashes&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span></div><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">All_Atom&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Stable&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.28&nbsp;(+/-0.16)</span></div></div><div class="ace_line_group" style="height:56px"><div class="ace_line" style="height:28px"><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_indent-guide">&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="ace_string ace_blockquote">&nbsp;ProQ:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[0,&nbsp;1]&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Higher&nbsp;the&nbsp;Better&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Overall&nbsp;Structural&nbsp;Quality&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span></div><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">All_Atom&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Variable&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.80&nbsp;(+/-0.12)</span></div></div><div class="ace_line_group" style="height:56px"><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">-------------------------------------------------------------------------------------------------------------------------</span></div><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">----------------------------------------------</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"><span class="ace_string ace_blockquote">*&nbsp;In&nbsp;Correct&nbsp;Models&nbsp;</span></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"></div></div><div class="ace_line_group" style="height:28px"><div class="ace_line" style="height:28px"></div></div></div><div class="ace_layer ace_marker-layer"></div><div class="ace_layer ace_cursor-layer"><div style="left: 340px; top: 420px; width: 7px; height: 28px;" class="ace_cursor"></div></div></div></div><div style="display: none; width: 18px; bottom: 0px;" class="ace_scrollbar ace_scrollbar-v"><div style="width: 18px; height: 4312px;" class="ace_scrollbar-inner"></div></div><div style="display: none; height: 18px; left: 55px; right: 0px;" class="ace_scrollbar ace_scrollbar-h"><div style="height: 18px; width: 859px;" class="ace_scrollbar-inner"></div></div><div style="height: auto; width: auto; top: -100px; left: -100px; visibility: hidden; position: fixed; white-space: pre; font: inherit; overflow: hidden;"><div style="height: auto; width: auto; top: -100px; left: -100px; visibility: hidden; position: fixed; white-space: pre; font: inherit; overflow: visible;"></div><div style="height: auto; width: auto; top: -100px; left: -100px; visibility: hidden; position: fixed; white-space: pre; font-family: inherit; font-style: inherit; font-size: inherit; line-height: inherit; font-size-adjust: inherit; font-stretch: inherit; font-feature-settings: inherit; font-language-override: inherit; font-kerning: inherit; font-synthesis: inherit; font-variant: inherit; overflow: visible;">XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX</div></div></div>
</div>

        <div class="g-b g-b--t1of2 split split-preview">
  <div id="preview" preview="" debounce="150"><h1><a id="ProQDock_0"></a>ProQDock</h1>
<p>Requires PERL (v.5.8 or higher), and a fortran90 compiler (prefered: ifort)
and five additional packages to be pre-installed</p>
<ol>
<li>sc from ccp4 (<a href="http://www.ccp4.ac.uk/">http://www.ccp4.ac.uk/</a>) [executable_name: sc]</li>
<li>delphi v.6.2 (<a href="http://compbio.clemson.edu/delphi/">http://compbio.clemson.edu/delphi/</a>) [executable_name: delphi95]</li>
<li>EDTSurf (<a href="http://zhanglab.ccmb.med.umich.edu/EDTSurf/">http://zhanglab.ccmb.med.umich.edu/EDTSurf/</a>) [executable_name: EDTSurf]</li>
<li>Rosetta (<a href="https://www.rosettacommons.org/software/">https://www.rosettacommons.org/software/</a>)</li>
<li>ProQ2 (<a href="https://github.com/bjornwallner/ProQ_scripts/">https://github.com/bjornwallner/ProQ_scripts/</a>)</li>
<li>needle from EMBOS (<a href="http://emboss.sourceforge.net/download/">http://emboss.sourceforge.net/download/</a>)</li>
</ol>
<h3><a id="Installation_12"></a>Installation</h3>
<pre><code class="language-sh">$ git <span class="hljs-built_in">clone</span> https://github.com/nemo8130/ProQDock
$ <span class="hljs-built_in">cd</span> ProQDock
$ chmod +x compileF
$ ./compileF &lt;fortran90-compiler&gt;  (Default: ifort)
</code></pre>
<p>open <code>ProQDock.bash</code> in any text editor and change the fullpaths for the following fields according to your system:</p>
<ul>
<li>sc_path=/software/apps/ccp4/ccp4-6.5.0/ccp4-6.5/bin</li>
<li>delphi_path=/home/x_sabas/bin</li>
<li>rosetta_path=/proj/wallner/users/x_bjowa/github/Rosetta/main/source/bin</li>
<li>rosetta_db=/proj/wallner/users/x_bjowa/github/Rosetta/main/database</li>
<li>svm_path=/home/x_sabas/proj/svm_light</li>
<li>ESpath=/home/x_sabas/proj/EDTSurf</li>
<li>proqpath=/home/x_sabas/proj/BACKUP/ProQ_scripts/bin</li>
</ul>
<h2><a id="The_program_has_two_mandetory_inputs__30"></a>The program has two mandetory inputs :</h2>
<pre><code>    1. The coordinate (PDB) file for the model
    2. The full-length target sequence
</code></pre>
<ul>
<li>The model must be a perfect or imperfect subset of the target sequence</li>
<li>i.e., Any and all sub-sequence present in the model must be present in the tareget</li>
</ul>
<h5><a id="Preparatory_Step_38"></a>Preparatory Step:</h5>
<p>The fulllength (FASTA) target sequence (one letter amino acid code 
followed by a header starting with &gt;) should be prepared and provided
 alongside the coordinate (PDB) file for the model.</p>
<p>You can generate the fasta sequence by using:</p>
<pre><code class="language-sh">$ ./ProQDock/EXEC/pdb2fasta.pl &lt;input.pdb&gt;  &gt;  input.pdb.fasta
</code></pre>
<h5><a id="Run_Step_46"></a>Run Step:</h5>
<pre><code class="language-sh">$ ./ProQDock.bash &lt;model.PDB&gt; &lt;fulllength.fasta&gt;
</code></pre>
<p>where,</p>
<ul>
<li>
<p>model.pdb: The input pdb (coordinate file in Brrokheaven format; <a href="http://www.ccp4.ac.uk/html/procheck_man/manappb.html">http://www.ccp4.ac.uk/html/procheck_man/manappb.html</a>) file</p>
</li>
<li>
<p>fulllength.fasta: The fulllength fasta sequence of the target</p>
</li>
<li>
<p>optional arguments: -d (0/1) : delphi_mode:</p>
</li>
</ul>
<blockquote>
<pre><code>       0: Single_internal_dielectric (epsilon=2) 
       1: Multidielctric mode (internal dielectric modeled as a Gaussian Function)
</code></pre>
</blockquote>
<blockquote>
<p>If you intend to Run ProQDock for a list of PDB files (models) corresponding to the same target sequence,
then, SKIP repeating the profile generation step more than once, by providing a [consistent basename] for the fasta file.</p>
</blockquote>
<p>Launch the following bash script which runs on a set of (example) models kept at ./EXAMPLE_CASE_STUDY/</p>
<pre><code class="language-sh">$ ./runex.bash
</code></pre>
<blockquote>
<p>EXAMPLE OUTPUT:</p>
</blockquote>
<pre><code class="language-sh">$ cat EXAMPLE_CASE_STUDY/D1OYV-models.status.results 
</code></pre>
<blockquote></blockquote>
<pre><code>        Model         fnat       LRMS       iRMS     CAPRI_class    DockQ    ProQDock
     D1OYV-a102a      0.000     35.348     15.846     Incorrect    0.02118    0.05908
     D1OYV-a104c      0.000     44.703     16.626     Incorrect    0.01432    0.21024
     D1OYV-a106c      0.000     44.600     18.136     Incorrect    0.01395    0.03890
     D1OYV-a115c      0.000     31.946     11.941     Incorrect    0.02722    0.02730
      D1OYV-a61c      0.652      6.820      2.334    Acceptable    0.51761    0.51060
      D1OYV-a61d      0.630      6.574      2.450    Acceptable    0.50960    0.46046
      D1OYV-a81d      0.587      6.788      2.481    Acceptable    0.48841    0.42648
      D1OYV-a41b      0.717      3.619      1.428        Medium    0.69617    0.48751
      D1OYV-a62c      0.707      3.991      1.628        Medium    0.66168    0.55795
      D1OYV-a82d      0.707      4.347      1.593        Medium    0.65639    0.48664
</code></pre>
<blockquote></blockquote>
<blockquote>
<p>Example Output for a single PDB file:</p>
</blockquote>
<pre><code class="language-sh">$ ./ProQDock ./TESTPDBS/<span class="hljs-number">1</span>u07.pdb ./EXAMPLE/<span class="hljs-number">1</span>u07.pdb.fasta
</code></pre>
<blockquote></blockquote>
<pre><code>    ===========================================================================================
    Your Input: 1u07.pdb
    The file has a Proper extension (.pdb / .PDB)
    Normal Delphi (single internal dielectric, epsilon = 2.0) will be executed
    ===========================================================================================
    
    Total number of residues in the Complex: 178
    EC=0.11
    Sc=0.767
    rGb=0.07170
    Ld= 0.14815
    nBSA=0.11534
    Fintres=0.28652
    CPscore= 0.48131
    CPM= 0.89454
    rTs=0.08767
    Isc=0.00002
    Erep=0.27086
    Etmr=0.21257
    ProQ= 0.845
    ======================== The ProQDock Score ====================
    0.85573
    ================================================================
</code></pre>
<blockquote></blockquote>
<hr>
<blockquote></blockquote>
<pre><code>      Feature   Range      Favorable Directions       Description   Feature_Type    Stability*     Native_values (DB3) 
      EC:       [-1,1]     Higher the Better       Electrostatic balance at the interface                             Interface      Variable      0.20 (+/-0.38)
     Sc:       [-1,1]     Higher the Better       Geometric / Steric fit at the interface                            Interface       Stable      0.68 (+/-0.17)
     rGb:      [-1,1]     Higher the Better       Distribution of hydrophobicity with respect to burial              All_Atom        Stable      0.06 (+/-0.02)
     Ld:       [0, 1]         Ambiguous           Link (packing) densely at the interface                            Interface       Stable      0.15 (+/-0.17)
     nBSA:     [0, 1]         Ambiguous           Size of the interface relative to the whole complex                Interface      Variable      0.07 (+/-0.05)
     Fintres:  [0, 1]         Ambiguous           Size of the interface relative to the whole complex                Interface      Variable      0.19 (+/-0.13)
     CPscore:  [0, 1]     Higher the Better       Preference of different inter-residue contacts at the interface    Interface      Variable         0.35 (+/-0.17)
     CPM:      [0, 1]     Higher the Better       Likelyhood of (Sc,EC) as a function of nBSA compared to native     Interface      Variable         0.80 (+/-0.18)
     rTs:      [0, 1]     Lower the Better        Energetic Stability of the whole molecule                          All_Atom        Stable      0.22 (+/-0.21)
     Isc:      [0, 1]     Lower the Better        Binding energy                                                     Interface      Variable         0.20 (+/-0.33)
     Erep:     [0, 1]     Lower the Better        Unfavourable Energy due to atomic Clashes                          All_Atom        Stable      0.41 (+/-0.15)
     Etmr:     [0, 1]     Lower the Better        Energetic Stability without condiderating clashes                  All_Atom        Stable      0.28 (+/-0.16)
     ProQ:     [0, 1]     Higher the Better       Overall Structural Quality                                         All_Atom       Variable         0.80 (+/-0.12)
</code></pre>
<hr>
<ul>
<li>In Correct Models</li>
</ul>
</div>
</div>

      </div>
    </div>
  </div>

  
  <script src="README_files/ga.js" async="" type="text/javascript"></script><script src="README_files/bundle.js" type="text/javascript" async=""></script>
  <script type="text/javascript">
    var _gaq = _gaq || [];
    _gaq.push(['_setAccount', 'UA-3312370-15']);
    _gaq.push(['_trackPageview']);

    (function() {
      var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;
      ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
      var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);
    })();
  </script>




</body></html>