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
        }

        namespace nodes
        {
            const QColor delay = redesign::darkOrange;
            const QColor endpoint = redesign::lightGreen;
            const QColor viewer = redesign::lightBlue;
            const QColor script = redesign::purple;
            const QColor external = redesign::red;
            const QColor payload = redesign::darkYellow;
            const QColor assertion = redesign::darkBlue;
        }
	}
}
