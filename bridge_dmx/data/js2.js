
function load() {
    const btn = document.querySelector('#btn');
    document.getElementById('accept').checked = false;
    btn.onclick = () => {
        if (document.getElementById('accept').checked) {
            alert("authentification activé");
            var xhrt = new XMLHttpRequest();
            xhrt.open("GET", "/checkactiver", true);
            xhrt.send();
            setTimeout(function () { window.open("/", "_self"); }, 500);
        }

    };
}

