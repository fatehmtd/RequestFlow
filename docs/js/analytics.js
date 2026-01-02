/**
 * Analytics Module
 * Handles tracking and analytics events
 */
export function initAnalytics() {
    // Track download button clicks
    document.querySelectorAll('.download-card .btn').forEach(btn => {
        btn.addEventListener('click', (e) => {
            const downloadType = e.target.closest('.download-card').querySelector('h3').textContent;
            console.log('Download clicked:', downloadType);
            
            // Add your analytics tracking here (e.g., Google Analytics, Plausible, etc.)
            // Example: gtag('event', 'download', { 'download_type': downloadType });
        });
    });
}
