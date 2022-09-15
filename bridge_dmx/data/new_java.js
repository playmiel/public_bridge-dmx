$(function () {
  $(window).on("scroll", function () {
    if ($(window).scrollTop() > 10) {
      $(".navbar").addClass("active");
    } else {
      $(".navbar").removeClass("active");
    }
  });
});

function setText(ida, newvalue) {
  var s = document.getElementById(ida);
  s.innerHTML = newvalue;
}

function map1(x, in_min, in_max, out_min, out_max) {
  return ((x - in_min) * (out_max - out_min)) / (in_max - in_min) + out_min;
}
var username = "admin";
var password = "esp32";
var xhr = new XMLHttpRequest();
xhr.withCredentials = true;
var id;
var canal_valeur = 0;
var valeur_canal = 0;
var offset_max_valeur = 255;
var offset_min_valeur = 0;
var gamma_valeur = 0;
var time;
var delays = 10000;
var valeur_groupe = 0;
var dimmer_valeur = 100;
var name_groupe;

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.open("POST", "/info", true);
  //xhttp.open("POST", "/info", true,username,esp32);
  xhttp.send();
  xhttp.onload = function () {
    if (this.status == 200) {
      var jsonResponse = JSON.parse(this.responseText);
      var xlevel = map1(jsonResponse.level, 0, 255, 0, 100);
      var xoffset_max = map1(jsonResponse.offset_max, 1, 255, 0, 100);
      var xoffset_min = map1(jsonResponse.offset_min, 0, 254, 0, 100);
      dimmer_valeur = jsonResponse.dimmer * 100;
      valeur_groupe = map1(jsonResponse.val_moy, 0, 255, 0, 100);
      valeur_groupe = valeur_groupe.toFixed(0);
      document.getElementById("value_groupeNumber").value =
        Math.trunc(valeur_groupe);
      document.getElementById("valeurNumber").value = Math.trunc(xlevel);
      document.getElementById("gammaNumber").value = jsonResponse.gamma;
      document.getElementById("offset_maxNumber").value =
        Math.trunc(xoffset_max);
      document.getElementById("offset_minNumber").value =
        Math.trunc(xoffset_min);
      console.log(jsonResponse);
      var s = "";
      var s2 = "";
      var xhttps = new XMLHttpRequest();
      xhttps.open("GET", "/requete?CMD=groupe&nom_groupe1", true);
      xhttps.send();
      xhttps.onreadystatechange = function () {
        if (xhttps.readyState == 4 && xhttps.status == 200) {
          for (let i = 1; i <= 32; i++) {
            s2 = "nom_groupe";
            s2 += i;
            s = "groupe";
            s += i;
            var jsonResponse2 = JSON.parse(xhttps.responseText);
            name_groupe = jsonResponse2.groupe[s2];
            document.getElementById(s).innerHTML = name_groupe;
          }
        } else {
          console.log("not ready yet");
        }

        var xps = new XMLHttpRequest();
        var s3 = "";
        xps.open("GET", "/array_groupe", true);
        xps.send();
        xps.onreadystatechange = function () {
          if (xps.readyState == 4 && xps.status == 200) {
            let jsonResponse3 = JSON.parse(xps.responseText);
            console.log(jsonResponse3.value);
            var value_canal_groupe = 0;
            for (let i = 0; i <= 32; i++) {
              if (jsonResponse3 === 0) {
                value_canal_groupe = 1;
                s3 = "canal_groupeNumber";
                s3 += i + 1;
                document.getElementById(s3).value = 0;
              } else {
                value_canal_groupe = jsonResponse3.value[i];
                s3 = "canal_groupeNumber";
                s3 += i + 1;
                document.getElementById(s3).value = value_canal_groupe;
              }
            }
          } else {
            console.log("pas prêt");
          }
        };
      };
      var https = new XMLHttpRequest();
      https.open("GET", "/requete?CMD=groupe&nom_canal1", true);
      https.send();
      let num;
      https.onreadystatechange = function () {
        if (https.readyState == 4 && https.status == 200) {
          for (let i = 1; i <= 32; i++) {
            num = "nom_groupe";
            num += i;
            var jsonResponse2 = JSON.parse(https.responseText);
            name_groupe = jsonResponse2.groupe[i];
            document.getElementById(i).innerHTML = name_groupe;
          }
        } else {
          console.log("not ready yet");
        }
      };
    } else {
      console.log(this.status);
    }
  };
}

function resetInterval() {
  clearInterval(time);

  time = setInterval(function () {
    console.log("restarted interval");
    getData();
    /*     valeur();
    offset_max();
    offset_min(); */
    document.getElementById("valeurNumber").value = valeur_canal;
  }, delays);
}
function load() {
  $("#bootstrap-alert").hide();
  setInterval(function () {
    getData();
    /*     valeur();
    offset_max();
    offset_min(); */
    document.getElementById("valeurNumber").value = valeur_canal;
  }, delays);

  $.ajax({
    type: "GET",
    url: "/requete",
    dataType: "json",
    async: false,
    username: username,
    password: password,
    success: function () {
      alert("connecter");
    },
    error: function () {
      alert("pas connecter");
    },
  });
  getData();
}

/* $(".dropdown-menu li a").click(function(){
  $(this).parents(".dropdown").find('.btn').html($(this).text() + ' <span class="caret"></span>');
  $(this).parents(".dropdown").find('.btn').val($(this).data('value'));

}); */

function showGenre(item) {
  id = item.id;
  let title = item.title;
  document.getElementById("dropdownMenu1").innerHTML = item.innerHTML;
  document.getElementById("text_groupe").value = item.innerHTML;
  document.getElementById("text_groupe").disabled = false;
  let xhtp = new XMLHttpRequest();
  xhtp.open("GET", "/requete?CMD=groupe&" + title, true);
  xhtp.send();
  xhtp.onreadystatechange = function () {
    if (xhtp.readyState == 4 && xhtp.status == 200) {
      let jsonResponse4 = JSON.parse(xhtp.responseText);
      console.log(jsonResponse4);
    }
  };
}

function clicksave() {
  var val = document.getElementById("text_groupe").value;
  document.getElementById(id).innerHTML = val;
  document.getElementById("dropdownMenu1").innerHTML = val;
  document.getElementById("clicksave").disabled = true;
}

function saveclick() {
  clearTimeout(timer);
  resetInterval();
  timer = setTimeout(() => {
    document.getElementById("clicksave").disabled = false;
    delays = 10000;
  }, 2500);
}
function clickclose() {
  document.getElementById("clicksave").disabled = true;
}

function old_value_canal(item3) {
  var id_canal = item3.id;
  var val = document.getElementById(id_canal).value;
  document.getElementById(id_canal).innerHTML = val;
  var xht = new XMLHttpRequest();
  xht.open("GET", "/requete?CMD=groupe&supp_in_groupe=" + val, true);
  xht.send();
}

function canal_groupe(item2) {
  clearTimeout(timer);
  resetInterval();
  timer = setTimeout(() => {
    console.log("ok");
    var id_canal = item2.id;
    var val = document.getElementById(id_canal).value;
    var expression = id_canal;
    var xhts = new XMLHttpRequest();
    switch (expression) {
      case "canal_groupeNumber1":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber2":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber3":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber4":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber5":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber6":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber7":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber8":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber9":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber10":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber11":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber12":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber13":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber14":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber15":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber16":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber17":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber18":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber19":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber20":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber21":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber22":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber23":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber24":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber25":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber26":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber27":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber28":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber29":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber30":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber31":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      case "canal_groupeNumber32":
        xhts.open("GET", "/requete?CMD=groupe&add_in_groupe=" + val, true);
        xhts.send();
        break;
      default:
        console.log("no");
        break;
    }
    document.getElementById(id_canal).blur();
  }, 1500);
}

function namegroupe() {
  delays = 10000;
  var val = document.getElementById("text_groupe").value;
  const expr = id;
  var jsonResponse;
  var xht = new XMLHttpRequest();
  /*   xhr.open("GET", "/requete?CMD=groupe&nom_+"+id+"=" + val, true); */
  switch (expr) {
    case "groupe1":
      xht.open("GET", "/requete?CMD=groupe&groupe1&nom_groupe1=" + val, true);
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe1;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe2":
      xht.open("GET", "/requete?CMD=groupe&groupe=2&nom_groupe2=" + val, true);
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe2;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe3":
      xht.open("GET", "/requete?CMD=groupe&groupe=3&nom_groupe3=" + val, true);
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe3;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe4":
      xht.open("GET", "/requete?CMD=groupe&groupe=4&nom_groupe4=" + val, true);
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe4;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe5":
      xht.open("GET", "/requete?CMD=groupe&groupe=5&nom_groupe5=" + val, true);
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe5;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe6":
      xht.open("GET", "/requete?CMD=groupe&groupe=6&nom_groupe6=" + val, true);
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe6;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe7":
      xht.open("GET", "/requete?CMD=groupe&groupe=7&nom_groupe7=" + val, true);
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe7;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe8":
      xht.open("GET", "/requete?CMD=groupe&groupe=8&nom_groupe8=" + val, true);
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe8;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe9":
      xht.open("GET", "/requete?CMD=groupe&groupe=9&nom_groupe9=" + val, true);
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe9;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe10":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=10&nom_groupe10=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe10;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe11":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=11&nom_groupe11=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe11;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe12":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=12&nom_groupe12=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe12;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe13":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=13&nom_groupe13=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe13;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe14":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=14&nom_groupe14=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe14;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe15":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=15&nom_groupe15=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe15;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe16":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=16&nom_groupe16=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe16;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe17":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=17&nom_groupe17=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe17;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe18":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=18&nom_groupe18=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe18;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe19":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=19&nom_groupe19=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe19;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe20":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=20&nom_groupe20=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe20;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe21":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=21&nom_groupe21=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe21;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe22":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=22&nom_groupe22=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe22;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe23":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=23&nom_groupe23=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe23;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe24":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=24&nom_groupe24=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe24;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe25":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=25&nom_groupe25=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe25;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe26":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=26&nom_groupe26=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe26;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe27":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=27&nom_groupe27=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe27;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe28":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=28&nom_groupe28=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe28;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe29":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=29&nom_groupe29=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe29;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe30":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=30&nom_groupe30=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe30;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe31":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=31&nom_groupe31=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe31;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    case "groupe32":
      xht.open(
        "GET",
        "/requete?CMD=groupe&groupe=32&nom_groupe32=" + val,
        true
      );
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe32;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      break;
    default:
      xht.open("GET", "/requete?CMD=groupe&groupe=1&nom_groupe1=" + val, true);
      xht.send();
      xht.onreadystatechange = function () {
        if (xht.readyState == 4 && xht.status == 200) {
          jsonResponse = JSON.parse(xht.responseText);
          name_groupe = jsonResponse.groupe.nom_groupe1;
          document.getElementById(id).innerHTML = name_groupe;
          document.getElementById("dropdownMenu1").innerHTML = name_groupe;
        }
      };
      console.log(`Sorry, we are out of ${expr}.`);
      break;
  }
  /* document.getElementById(id).innerHTML = val;
  document.getElementById("dropdownMenu1").innerHTML = val;
 */
  //valeur();
}

function saveButton_groupe() {
  xhr.open("GET", "/requete?save", true);
  var save_valeur = document.getElementById("valeurNumber").value;
  //var save_valeurv2 = map1(save_valeur, 0, 100, 0, 255);

  xhr.send();
  if (!xhr.readyState == 4 && xhr.status == 200) {
    alert("rien a etait envoyer");
  } else {
    setText("This-is-the-real-id", "");
    showAlerts();
    //alert("la valeur du canal sauvegarder est "+save_valeur);
  }
}

function showAlerts() {
  $("#bootstrap-alert").fadeIn(1000);
  setTimeout(function () {
    $("#bootstrap-alert").fadeOut(1000);
  }, 3000);
}
function valBetween(v, min, max) {
  return Math.min(max, Math.max(min, v));
}

function dimmer() {
  clearTimeout(timer);
  resetInterval();
  timer = setTimeout(() => {
    dimmer_valeur = document.getElementById("dimmerNumber").value;

    xhr.open("GET", "/requete?CMD=groupe&dimmer=" + dimmer_valeur, true);
    xhr.send();
    getData();
  }, 2500);

  //valeur();
}

function canal() {
  clearTimeout(timer);
  resetInterval();
  timer = setTimeout(() => {
    canal_valeur = document.getElementById("canalNumber").value;

    xhr.open("GET", "/requete?CMD=canaux&canal=" + canal_valeur, true);
    xhr.send();
    getData();
  }, 2500);

  //valeur();
}

function value_groupe() {
  clearTimeout(timer);
  resetInterval();
  timer = setTimeout(() => {
    valeur_groupe = document.getElementById("value_groupeNumber").value;
    /*     valeur_canal = valBetween(
      valeur_canal,
      offset_min_valeur,
      offset_max_valeur
    ); */
    document.getElementById("value_groupeNumber").value = valeur_groupe;

    xhr.open(
      "GET",
      "/requete?CMD=groupe&canal_value_groupe=" + valeur_groupe,
      true
    );
    xhr.send();
  }, 1500);
}

function valeur() {
  clearTimeout(timer);
  resetInterval();
  timer = setTimeout(() => {
    valeur_canal = document.getElementById("valeurNumber").value;
    valeur_canal = valBetween(
      valeur_canal,
      offset_min_valeur,
      offset_max_valeur
    );
    document.getElementById("valeurNumber").value = valeur_canal;

    xhr.open("GET", "/requete?CMD=canaux&value_canal=" + valeur_canal, true);
    xhr.send();
  }, 2500);
}

function offset_max() {
  clearTimeout(timer);
  resetInterval();
  timer = setTimeout(() => {
    offset_max_valeur = document.getElementById("offset_maxNumber").value;
    if (offset_max_valeur < offset_min_valeur) {
      offset_min_valeur = offset_max_valeur - 1;
      document.getElementById("offset_minNumber").value = offset_min_valeur;
    }
    if (offset_max_valeur < offset_min_valeur) {
      offset_max_valeur = offset_min_valeur + 1;
      document.getElementById("offset_maxNumber").value = offset_max_valeur;
    }
    xhr.open(
      "GET",
      "/requete?CMD=canaux&offset_max=" + offset_max_valeur,
      true
    );
    xhr.send();

    valeur();
  }, 2500);
}

function offset_min() {
  clearTimeout(timer);
  resetInterval();
  timer = setTimeout(() => {
    offset_min_valeur = document.getElementById("offset_minNumber").value;

    if (offset_max_valeur < offset_min_valeur) {
      offset_min_valeur = offset_max_valeur - 1;
      document.getElementById("offset_minNumber").value = offset_min_valeur;
    }
    if (offset_max_valeur < offset_min_valeur) {
      offset_max_valeur = offset_min_valeur + 1;
      document.getElementById("offset_maxNumber").value = offset_max_valeur;
    }
    xhr.open(
      "GET",
      "/requete?CMD=canaux&offset_min=" + offset_min_valeur,
      true
    );
    xhr.send();

    valeur();
  }, 2500);
}
function gamma() {
  clearTimeout(timer);
  resetInterval();
  timer = setTimeout(() => {
    gamma_valeur = document.getElementById("gammaNumber").value;
    xhr.open("GET", "/requete?CMD=canaux&gamma=" + gamma_valeur, true);
    xhr.send();
  }, 2500);
}

function saveButton() {
  xhr.open("GET", "/requete?save", true);
  var save_valeur = document.getElementById("valeurNumber").value;
  //var save_valeurv2 = map1(save_valeur, 0, 100, 0, 255);

  xhr.send();
  if (!xhr.readyState == 4 && xhr.status == 200) {
    alert("rien a etait envoyer");
  } else {
    setText("This-is-the-real-id", "");
    showAlerts();
    //alert("la valeur du canal sauvegarder est "+save_valeur);
  }
}
