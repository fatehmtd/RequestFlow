#pragma once
#include <QColor>

namespace view
{
	namespace colors
	{
		const QColor purple("#6A4C93");
		const QColor blue("#1982C4");
		const QColor green("#8AC926");
		const QColor yellow("#FFCA3A");
		const QColor red("#FF595E");
		//const QColor whiteGrey("#999999");
		const QColor whiteGrey("#F3F2F0");
		const QColor blueGrey("#919AB3");
		const QColor darkGrey("#333333");
		const QColor lightestGrey("#BDBDBD");
		const QColor lightGrey("#8E8E8E");

		const QColor orange("#FF953D");
		const QColor outlineGrey("#5F5F5F");

		const QColor metalicSunburst("#A59132");
		const QColor chocolateWeb("#DA7422");
		const QColor vividBurgundy("#9E0031");
		const QColor byzantium("#770058");
		const QColor charcoal("#40434E");
		const QColor queenBlue("#577590");
		const QColor gunMetal("#1F2933");

		namespace light
		{
			const QColor purple("#583353");
			const QColor cyan("#3C5258");
			const QColor green("#2D5536");
			const QColor yellow("#FFCA3A");
		}

		namespace dark
		{
			const QColor purple("#352232");
			const QColor cyan("#28363A");
			const QColor green("#1F3323");
		}

        namespace redesign
        {
            const QColor darkOrange("#FF924C");
            const QColor darkGreen("#57B079");
            const QColor lightGreen("#8AC926");
            const QColor lightBlue("#17A0ED");
            const QColor purple("#9874F8");
            const QColor red("#B80006");
            const QColor darkYellow("#FFCA3A");
            const QColor darkBlue("#1D5972");

            // Core workflow colors
            const QColor deepBlue("#1E3A8A");      // Primary actions, LLM nodes
            const QColor brightBlue("#3B82F6");    // Endpoints, HTTP requests
            const QColor emerald("#10B981");       // Success states, valid responses
            const QColor amber("#F59E0B");         // Warning, processing states
            const QColor rose("#EF4444");          // Errors, failed requests
            const QColor violet("#8B5CF6");        // AI/ML specific nodes
            const QColor slate("#64748B");         // Utility nodes, delays
            const QColor indigo("#6366F1");        // Logic nodes, conditionals

            // Optimized for dark backgrounds
            const QColor oceanBlue("#0EA5E9");     // Primary endpoints
            const QColor seafoam("#06D6A0");       // Success/completion
            const QColor coral("#FF6B6B");         // Errors/failures
            const QColor gold("#FFD23F");          // Warnings/processing
            const QColor lavender("#A78BFA");      // AI/LLM nodes
            const QColor mint("#4ECDC4");          // Data nodes
            const QColor peach("#FF8E53");         // Logic/conditional
            const QColor steel("#6B7280");         // Utility/system
        }

        namespace nodes
        {
            /*const QColor delay = redesign::darkOrange;
            const QColor endpoint = redesign::lightGreen;
            const QColor viewer = redesign::lightBlue;
            const QColor script = redesign::purple;
            const QColor external = redesign::red;
            const QColor payload = redesign::darkYellow;
            const QColor assertion = redesign::darkBlue;*/
            const QColor delay = redesign::amber;          // Processing/waiting states
            const QColor endpoint = redesign::brightBlue;  // HTTP requests/API calls
            const QColor viewer = redesign::emerald;       // Success/data display
            const QColor script = redesign::violet;        // AI/ML and scripting
            const QColor external = redesign::rose;        // External/error states
            const QColor payload = redesign::gold;         // Data/content nodes
            const QColor assertion = redesign::deepBlue;   // Validation/testing
        }
	}
}
