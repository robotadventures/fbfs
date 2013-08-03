var hash = window.location.hash.substring(1)
xmlhttp = new XMLHttpRequest();

xmlhttp.open("GET", "access_token?" + hash, false);
xmlhttp.send();

close();
