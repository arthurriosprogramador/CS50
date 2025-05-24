document.addEventListener("DOMContentLoaded", () => {
    const cards = document.querySelectorAll(".card");

    cards.forEach(element => {
        element.addEventListener("mouseenter", () => {
            element.style.transform = "scale(1.1)";
            element.style.transition = "transform 0.3s ease";
            element.style.zIndex = "10";
        });

        element.addEventListener("mouseleave", () => {
            element.style.transform = "scale(1)";
            element.style.transition = "transform 0.3s ease";
            element.style.zIndex = "1";
        });
    });
});

