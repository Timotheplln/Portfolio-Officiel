document.addEventListener('DOMContentLoaded', function (){
    var fleche = document.getElementById('fleche_m');
    var nav = document.getElementById('nav-phone');
    var estVisible = false;

    fleche.addEventListener('click',function(){
        if(estVisible){
            fleche.style.transform = "scaleY(1)";
            nav.style.display = "none";
            estVisible = false;
        } else {
            fleche.style.transform = "scaleY(-1)";
            nav.style.display = "flex";
            nav.style.flexDirection = "column";
            estVisible = true;
        }
    });
})