/**
 * Animations Module
 * Handles scroll-based animations and intersection observers
 */
export function initAnimations() {
    // Scroll animation observer
    const observerOptions = {
        threshold: 0.1,
        rootMargin: '0px 0px -100px 0px'
    };

    const observer = new IntersectionObserver((entries) => {
        entries.forEach(entry => {
            if (entry.isIntersecting) {
                entry.target.style.opacity = '1';
                entry.target.style.transform = 'translateY(0)';
            }
        });
    }, observerOptions);

    // Observe elements for animation
    document.querySelectorAll('.feature-card, .screenshot-item, .video-item, .download-card').forEach(el => {
        el.style.opacity = '0';
        el.style.transform = 'translateY(20px)';
        el.style.transition = 'opacity 0.6s ease, transform 0.6s ease';
        observer.observe(el);
    });

    // Hero fade-in effect
    window.addEventListener('load', () => {
        const hero = document.querySelector('.hero');
        hero.style.opacity = '0';
        hero.style.transition = 'opacity 1s ease';
        
        setTimeout(() => {
            hero.style.opacity = '1';
        }, 100);
    });

    // Placeholder hover effects
    document.querySelectorAll('.placeholder-image, .placeholder-video').forEach(placeholder => {
        placeholder.addEventListener('mouseenter', function() {
            this.style.transform = 'scale(1.02)';
            this.style.transition = 'transform 0.3s ease';
        });
        
        placeholder.addEventListener('mouseleave', function() {
            this.style.transform = 'scale(1)';
        });
    });
}
