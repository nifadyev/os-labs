///////////////////////////////////////////////////////////
// CS Direct Monitor v.0.5
// Client-side Javascript Part
// http://www.controlstyle.ru/
///////////////////////////////////////////////////////////
function dmMain()
{
	if (typeof window.addEventListener != "undefined") 
	{
	  window.addEventListener("click", dmLogClick, false);
	}
	else if (typeof document.attachEvent != "undefined") 
	{
		document.attachEvent("onclick", dmLogClick);
	}
}
function dmFindLink(element) 
{
	while(element.parentNode) 
	{
		if (element.href) return element;
		element = element.parentNode;
	}
	return false;
}
function dmPrepareSpan(s)
{
	String.prototype.trim = function() { return this.replace(/^\s+|\s+$/g, ''); };
	var re = /<[^<>]+>/gi;
	var re2 = /[^a-z\.\-]+/g;
	var re3 = /\s/g;
	var re4 = /\s[a-z]+\s/g;
	s = s.replace(re, '');
	s = s.replace(re2, ' ');
	s = ' ' + s.replace(re3, '  ') + ' ';
	s = s.replace(re4, ' ');
	return s.trim();
}
function dmLogClick(e)
{
	var obj = (e.target)?e.target:event.srcElement;
	var spanUrl = 'n/a';
	obj = dmFindLink(obj);
	if (!obj) return true;
	if (obj.href.indexOf('.yandex.ru/count')==-1) return true;
	if (dmXmlDoc)
	{
		if (obj.parentNode.parentNode)
		{
			var spanParent = obj.parentNode.parentNode;
			for (var i=0; i<spanParent.childNodes.length; i++)
			{
				var tmp = spanParent.childNodes[i];
				if (!tmp.className) continue;
				if (tmp.className.toLowerCase()=='url'&&tmp.tagName.toLowerCase()=='span')
				{
					spanUrl = dmPrepareSpan(tmp.innerHTML);
				}
			}
		}
		var data = encodeURIComponent(obj.innerHTML+'; '+spanUrl+'; '+location.href+'; '+document.referrer);
		dmXmlDoc.open("GET", 'http://' + location.host + '/dmonitor/index.php?data=' + data, true);
		dmXmlDoc.send(null);
	}
	return true;
}
function dmCreateXmlDoc()
{
	var dmXmlDoc = null;
	/*@cc_on @*/
	/*@if (@_jscript_version >= 5)
	// JScript gives us Conditional compilation, we can cope with old IE versions.
	// and security blocked creation of the objects.
	 try {
	  dmXmlDoc = new ActiveXObject("Msxml2.XMLHTTP");
	 } catch (e) {
	  try {
	   dmXmlDoc = new ActiveXObject("Microsoft.XMLHTTP");
	  } catch (E) {
	   dmXmlDoc = false;
	  }
	 }
	@end @*/
	if (!dmXmlDoc && typeof XMLHttpRequest!='undefined') {
		try {
			dmXmlDoc = new XMLHttpRequest();
		} catch (e) {
			dmXmlDoc=false;
		}
	}
	if (!dmXmlDoc && window.createRequest) 
	{
		try 
		{
			dmXmlDoc = window.createRequest();
		} catch (e) 
		{
		dmXmlDoc=false;
		}
	}
	return dmXmlDoc;
}
var dmXmlDoc = dmCreateXmlDoc();
dmMain();
///////////////////////////////////////////////////////////
// End Of CS Direct Monitor
///////////////////////////////////////////////////////////