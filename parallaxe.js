document.addEventListener("DOMContentLoaded", function() {
    
    const para = document.getElementById("para");

    const hauteurDuHeader = "139px";


    window.addEventListener("scroll", function() {
        const scrollPos = window.scrollY;
        para.style.backgroundPositionY = ` ${scrollPos * 0.5}px`
    })
});