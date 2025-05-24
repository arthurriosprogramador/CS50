class NavBar extends HTMLElement {
    connectedCallback() {
        this.innerHTML = `
        <nav class="navbar navbar-expand-lg" id="custom-nav"  data-bs-theme="dark">
            <div class="container-fluid">
                <h1 class="navbar-brand">BPL</h1>
                <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarScroll" aria-controls="navbarScroll" aria-expanded="false" aria-label="Toggle navigation">
                    <span class="navbar-toggler-icon"></span>
                </button>
                <div class="collapse navbar-collapse" id="navbarScroll">
                    <ul class="navbar-nav me-auto my-2 my-lg-0 navbar-nav-scroll" style="--bs-scroll-height: 100px;">
                        <li class="nav-item">
                            <a class="nav-link active" aria-current="page" href="index.html">Home</a>
                        </li>
                        <li class="nav-item">
                            <a class="nav-link" aria-current="page" href="c.html">C</a>
                        </li>
                        <li class="nav-item">
                            <a class="nav-link" aria-current="page" href="python.html">Python</a>
                        </li>
                        <li class="nav-item">
                            <a class="nav-link" aria-current="page" href="kotlin.html">Kotlin</a>
                        </li>
                        <li class="nav-item">
                            <a class="nav-link" aria-current="page" href="java_script.html">Java Script</a>
                        </li>
                        <li class="nav-item">
                            <a class="nav-link" aria-current="page" href="cpp.html">C++</a>
                        </li>
                    </ul>
                </div>
            </div>
        </nav>
        `;
        this.stylizeSelectedPage();
    }

    stylizeSelectedPage() {
        const currentPage = window.location.pathname.split("/").pop();

        this.querySelectorAll(".nav-link").forEach(element => {
            if (element.getAttribute("href") == currentPage) {
                element.classList.add("active");
            } else {
                element.classList.remove("active");
            }
        });
    }
}

customElements.define('custom-navbar', NavBar);
