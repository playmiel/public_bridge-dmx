
function load() {
    const btn = document.querySelector('#btn');
    document.getElementById('accept').checked = true;
    btn.onclick = () => {

        if (document.getElementById('accept').checked == false) {
            alert("authentification desactivé");
            var xhrtv = new XMLHttpRequest();
            xhrtv.open("GET", "/checkdesactiver", true);
            xhrtv.send();
            setTimeout(function () { window.open("/", "_self"); }, 1000);
        }
    };
}

function logoutButton() {
    var xhr = new XMLHttpRequest();
    window.open("/logout", "_self");
    xhr.open("GET", "/logout", true);
    xhr.send();

}


