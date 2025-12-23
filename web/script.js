/**
 * RequestFlow Website - Main Application Entry Point
 * Modular architecture with separate concerns
 */

import { initTheme } from './js/theme.js';
import { initNavigation } from './js/navigation.js';
import { initAnimations } from './js/animations.js';
import { initModal } from './js/modal.js';
import { initAnalytics } from './js/analytics.js';

// Initialize all modules
document.addEventListener('DOMContentLoaded', () => {
    initTheme();
    initNavigation();
    initAnimations();
    initModal();
    initAnalytics();
});