function loadCode() {
    const codeContainers = document.querySelectorAll('.code-container');
    codeContainers.forEach(container => {
        const fileName = container.getAttribute('data-filename');
        
        fetch(fileName)
            .then(response => response.text())
            .then(code => {
                const codeElement = container.querySelector('code');
                codeElement.textContent = code; // Use textContent to prevent HTML interpretation
                Prism.highlightElement(codeElement); // Highlight the code after adding it
            })
            .catch(error => console.error('Error loading the code:', error));
    });
}

function copyCode(event) {
    const button = event.target;
    const code = button.closest('.code-container').querySelector('code').textContent.trim();
    
    navigator.clipboard.writeText(code).then(function() {
        button.textContent = '✓ Copied';
        setTimeout(function() {
            button.textContent = 'Copy';
        }, 3000);
    }, function(err) {
        console.error('Failed to copy: ', err);
    });
}

document.addEventListener('DOMContentLoaded', function() {
    loadCode();
    document.querySelectorAll('.copy-btn').forEach(button => {
        button.addEventListener('click', copyCode);
    });
});

// Example function to handle navigation
document.querySelectorAll('.back-link').forEach(link => {
    link.addEventListener('click', function(event) {
        event.preventDefault();
        window.location.href = link.href; // Navigate to the specified URL
    });
});

