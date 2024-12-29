function loadCodeAndThemes() {
    console.log('loadCodeAndThemes.js is executing');
    alert('loadCodeAndThemes.js loaded!');

    const codeContainers = document.querySelectorAll('.code-container');

    codeContainers.forEach(container => {
        const theme = container.getAttribute('data-theme'); // Get the theme (bash or cpp)
        const fileName = container.getAttribute('data-filename'); // Get the file to load
        const codeElement = container.querySelector('code');

        // Dynamically load the appropriate Prism CSS and JS
        if (theme === 'bash') {
            loadPrismAssets('prism-bash.css', 'prism-bash.js');
        } else if (theme === 'cpp') {
            loadPrismAssets('prism-cpp.css', 'prism-cpp.js');
        }

        // Fetch and display the code
        fetch(fileName)
            .then(response => response.text())
            .then(code => {
                codeElement.textContent = code; // Add code content to the element
                Prism.highlightElement(codeElement); // Highlight using Prism.js
            })
            .catch(error => console.error('Error loading the code:', error));
    });
}

function loadPrismAssets(cssFile, jsFile) {
    // Load CSS dynamically
    if (!document.querySelector(`link[href="assets/css/${cssFile}"]`)) {
        const link = document.createElement('link');
        link.rel = 'stylesheet';
        link.href = `assets/css/${cssFile}`;
        document.head.appendChild(link);
    }

    // Load JS dynamically
    if (!document.querySelector(`script[src="assets/js/${jsFile}"]`)) {
        const script = document.createElement('script');
        script.src = `assets/js/${jsFile}`;
        document.body.appendChild(script);
    }
}

// Call the function on DOMContentLoaded
document.addEventListener('DOMContentLoaded', loadCodeAndThemes);
