#ifndef BACKGROUNDPAINTFILTER_H
#define BACKGROUNDPAINTFILTER_H

#include <QPixmap>
#include <QColor>
#include <QWidget>
#include <QPainter>
#include <QEvent>

class BackgroundPaintFilter : public QObject
{
protected:
    QPixmap _pixmap;
    QColor _bgColor;
public:
    BackgroundPaintFilter(QObject* parent) : QObject(parent), _bgColor("#A0A0A0")
    {
        _pixmap = QPixmap(":/ui/requestflow_bg");
    }

    virtual ~BackgroundPaintFilter()
    {
    }

    virtual bool eventFilter(QObject* obj, QEvent* event) override
    {
        if (event->type() != QEvent::Paint) return QObject::eventFilter(obj, event);

        auto widget = dynamic_cast<QWidget*>(obj);
        auto xOffset = 0, yOffset = 0;
        auto width = widget->width();
        auto height = widget->height();

        //const int maxHeight = _pixmap.width(), maxWidth = _pixmap.height();
        //const int minHeight = height / 9, minWidth = maxWidth / 9;
        //const float aspect = (float)maxWidth / 800.0f;

        QPainter painter;
        painter.begin(widget);
        painter.fillRect(0, 0, width, height, _bgColor); // fill the background with a color
        //painter.drawPixmap(width - (_pixmap.width() + xOffset), height - (_pixmap.height() + yOffset), _pixmap.width(), _pixmap.height(), _pixmap);
        painter.drawPixmap(width / 2 - _pixmap.width() / 2, height / 2 - _pixmap.height() / 2, _pixmap.width(), _pixmap.height(), _pixmap);
        painter.end();

        return true;
    }
};
#endif // BACKGROUNDPAINTFILTER_H
