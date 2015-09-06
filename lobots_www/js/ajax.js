//request do ajax
function getRequest() {
  var req = false;
  try{
      // most browsers
      req = new XMLHttpRequest();
  } catch (e){
      // IE
      try{
	  req = new ActiveXObject("Msxml2.XMLHTTP");
      } catch (e) {
	  // try an older version
	  try{
	      req = new ActiveXObject("Microsoft.XMLHTTP");
	  } catch (e){
	      return false;    
	  }
      }
  }
  return req;
}


function carregarPaginaAjax(endereco, idelemento, mostrar_loading) {
      mostrar_loading = typeof mostrar_loading !== 'undefined' ? mostrar_loading : false;
      
      var ajax = getRequest();
      if (idelemento != null) {
	if (mostrar_loading == true) {
	    document.getElementById(idelemento).innerHTML = "<center><img src='images/ajax_loading.gif'></center>";
	}
	ajax.onreadystatechange = function(){
	    if(ajax.readyState == 4){	  
		var resposta = ajax.responseText;
		document.getElementById(idelemento).innerHTML = resposta;
	    }
	}
      }
      ajax.open("GET", endereco, true);
      ajax.send(null);
}