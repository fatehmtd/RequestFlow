#pragma once

#include "coreview_global.h"
#include <QHash>
#include <QSvgRenderer>
#include <QImage>
#include <QPainter>
#include <QIcon>

namespace view {

class COREVIEW_EXPORT SVGRenderer
{
private:
    SVGRenderer();
public:
    static SVGRenderer *getInstance();

    QPixmap renderToIcon(const QString& path, int size=24);
private:
    QHash<QString, QPixmap> _iconsMap;
    static SVGRenderer *_instance;
};
}


