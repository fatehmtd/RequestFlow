/**
 * Modal Module
 * Initialize Fancybox for image gallery functionality
 */
export function initModal() {
    // Fancybox initializes automatically from data-fancybox attributes
    // Configure global options
    if (window.Fancybox) {
        Fancybox.bind('[data-fancybox]', {
            Hash: false,
            Toolbar: {
                display: {
                    left: [],
                    middle: [],
                    right: ['close']
                }
            },
            Thumbs: false
        });
    }
}
