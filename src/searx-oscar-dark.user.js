// ==UserScript==
// @name          Searx - Oscar Dark
// @namespace     http://userstyles.org
// @description	  A dark iteration of the 'Oscar' theme for searx.me.
// @author        trashpile
// @homepage      https://userstyles.org/styles/140971
// @include       http://searx.me/*
// @include       https://searx.me/*
// @include       http://*.searx.me/*
// @include       https://*.searx.me/*
// @run-at        document-start
// @version       0.20170405164344
// ==/UserScript==
(function() {var css = [
	"@namespace url(http://www.w3.org/1999/xhtml);",
	"* {",
	"    color: #fff;",
	"  }",
	"  ",
	" .input-group-btn, .panel-heading, input, .nav .active a, .nav-justified a:hover, select, .btn, td, th, th:hover, .close:hover, .close:focus, .onoffswitch-inner::before, .onoffswitch-inner::after, .onoffswitch-switch, .nav-justified li, .tt-dropdown-menu {",
	"   color: #fff !important;",
	"   background: #111 !important;",
	"  }",
	"  ",
	"  body, .searx-navbar {",
	"    background: #222 !important;",
	"  }",
	"  ",
	"  #q, #time-range, #language, #categories label, #categories label:last-child, .alert, .panel-default, #search_url, button, #search_form .input-group-btn .btn, .btn-default, .modal-wrapper, .thumbnail {",
	"    color: #fff !important;",
	"    background: #111 !important;",
	"    border: 1px #222 solid;",
	"  }",
	"",
	"  #search_form .input-group-btn .btn:hover, .result_header a .highlight, .tt-dropdown-menu .tt-suggestion.tt-cursor {",
	"    color: #fff;",
	"    background: #666;",
	"    border-left: 0;",
	"  }",
	" ",
	"  .highlight, .result_header a .highlight {",
	"    background: none;",
	"    border: 0;",
	"  }",
	"  ",
	"  .nav-justified li {",
	"    border: 1px #222 solid !important;",
	"  }",
	"  ",
	"  .nav-justified a {",
	"    border: 0 !important;",
	"  }",
	"  ",
	"  a {",
	"    color: #fff !important;",
	"  }",
	"  ",
	"  .input-group #q {",
	"    border-right: 0 !important;",
	"  }",
	"  ",
	"  .panel-heading {",
	"    border: 0 !important;",
	"  }",
	"  ",
	"  .btn-primary {",
	"    border-color: #fff;",
	"  }",
	"  ",
	"  #categories input[type=\"checkbox\"]:checked + label, .nav-justified .active a{",
	"    border-bottom: 5px #fff solid !important;",
	"  }",
	"  ",
	"  #categories label {",
	"    border-right: 0;",
	"    padding-bottom: 0;",
	"  }",
	" ",
	"  .custom-select {",
	"    color: #ccc !important;",
	"  }",
	"  ",
	"  .close {",
	"    color: #fff;",
	"    opacity: 1;",
	"    text-shadow: none;",
	"  }",
	"  ",
	"  .well, .result:hover {",
	"    background: none;",
	"    border: none;",
	"  }",
	"  ",
	"  .well, .result-images {",
	"    margin: 0;",
	"  }",
	"  ",
	"  .result-content {",
	"    color: #999;",
	"  }",
	"  ",
	"  .result {",
	"    padding: 10px;",
	"  }",
	"",
	"  .btn.btn-default.btn-xs {",
	"    background: none !important;",
	"    font-weight: bold;",
	"  }",
	"  ",
	"  .img-thumbnail {",
	"    width: 100% !important;",
	"    object-fit: cover;",
	"    padding: 0;",
	"    margin: 0;",
	"    border: 0;",
	"  }"
].join("\n");
if (typeof GM_addStyle != "undefined") {
	GM_addStyle(css);
} else if (typeof PRO_addStyle != "undefined") {
	PRO_addStyle(css);
} else if (typeof addStyle != "undefined") {
	addStyle(css);
} else {
	var node = document.createElement("style");
	node.type = "text/css";
	node.appendChild(document.createTextNode(css));
	var heads = document.getElementsByTagName("head");
	if (heads.length > 0) {
		heads[0].appendChild(node);
	} else {
		// no head yet, stick it whereever
		document.documentElement.appendChild(node);
	}
}
})();
