#include "CoreViewUtils.h"

view::SVGRenderer * view::SVGRenderer::_instance = nullptr;

view::SVGRenderer::SVGRenderer()
{

}

view::SVGRenderer *view::SVGRenderer::getInstance()
{
    if(_instance == nullptr)
    {
        _instance = new SVGRenderer();
    }
    return _instance;
}

QPixmap view::SVGRenderer::renderToIcon(const QString &path, int size)
{
    if(_iconsMap.contains(path)) return _iconsMap[path];

    QImage image(size, size, QImage::Format::Format_RGBA8888);
    image.fill(qRgba(255, 255, 255, 0));
    QSvgRenderer renderer(path);
    QPainter painter(&image);
    renderer.render(&painter);
    _iconsMap.insert(path, QPixmap::fromImage(image));

    return _iconsMap.value(path);
}
