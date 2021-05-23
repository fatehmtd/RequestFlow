#pragma once

#include <QPixmap>
#include <QColor>
#include <QWidget>
#include <QPainter>
#include <QEvent>
#include <QDebug>

class BackgroundPaintFilter : public QObject
{
protected:
    QPixmap _pixmap;
    QColor _bgColor;
public:
    BackgroundPaintFilter(QObject* parent) : QObject(parent), _bgColor("#FFFFFF")
    {
        _pixmap = QPixmap(":/ui/requestflow_bg");
    }

    virtual ~BackgroundPaintFilter()
    {
    }

    virtual bool eventFilter(QObject* obj, QEvent* event) override
    {
        if (event->type() != QEvent::Paint)
        {
            //qDebug() << __FUNCTION__ << obj << event;
            return QObject::eventFilter(obj, event);
        }

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

        //paintGrid(&painter, widget->rect(), 40, 10, QColor(200, 200, 200), QColor(160, 160, 160));
        paintCrossBackground(&painter, widget->rect(), 80);
        //painter.drawPixmap(width - (_pixmap.width() + xOffset), height - (_pixmap.height() + yOffset), _pixmap.width(), _pixmap.height(), _pixmap);
        painter.drawPixmap(width / 2 - _pixmap.width() / 2, height / 2 - _pixmap.height() / 2, _pixmap.width(), _pixmap.height(), _pixmap);
        painter.end();

        return true;
    }

    void paintCrossBackground(QPainter* painter, QRect rect, int _cellSize)
    {
        const int left = floor(rect.left());
        const int right = ceil(rect.right());
        const int top = floor(rect.top());
        const int bottom = ceil(rect.bottom());

        // points
    const int firstLeft = left - (left % _cellSize);
        const int firstTop = top - (top % _cellSize);
        int count = (1 + (right - firstLeft) / _cellSize) * (1 + (bottom - firstTop) / _cellSize);
        //QVector<QPointF> points(count);
        QVector<QLine> lines(count * 2);

        float halfLen = 5;

        int index = 0;
        for (int i = firstLeft; i < right; i += _cellSize)
        {
            for (int j = firstTop; j < bottom; j += _cellSize)
            {
                lines[index++] = QLine(i - halfLen, j, i + halfLen, j);
                lines[index++] = QLine(i, j - halfLen, i, j + halfLen);
            }
        }

        QPen pen(QBrush(QColor("#E4E3E4")), 2.0f);
        //pen.setStyle(Qt::PenStyle::DashLine);
        painter->setPen(pen);
        painter->drawLines(lines.data(), lines.size());
    }


    void paintGrid(QPainter* painter, QRect rect, int _cellSize, int _blockSize, QColor _lightGrid, QColor _darkGrid)
    {
        const int left = floor(rect.left());
        const int right = ceil(rect.right());
        const int top = floor(rect.top());
        const int bottom = ceil(rect.bottom());

        // cells
        {
            QVector<QLine> _lines;

            const int firstLeft = left - (left % _cellSize);
            const int firstTop = top - (top % _cellSize);

            for (int i = firstLeft; i < right; i += _cellSize)
            {
                _lines.push_back(QLine(i, top, i, bottom));
            }

            for (int i = firstTop; i < bottom; i += _cellSize)
            {
                _lines.push_back(QLine(left, i, right, i));
            }

            QPen pen(QBrush(_lightGrid), 1.0f);
            pen.setStyle(Qt::PenStyle::DashLine);
            painter->setPen(pen);
            painter->drawLines(_lines);
        }

        // blocks
        {
            QVector<QLine> _lines;

            const int blockSizePixels = _cellSize * _blockSize;

            const int firstLeft = left - (left % blockSizePixels);
            const int firstTop = top - (top % blockSizePixels);

            for (int i = firstLeft; i < right; i += blockSizePixels)
            {
                _lines.push_back(QLine(i, top, i, bottom));
            }

            for (int i = firstTop; i < bottom; i += blockSizePixels)
            {
                _lines.push_back(QLine(left, i, right, i));
            }

            QPen pen(QBrush(_darkGrid), 1.0f);
            painter->setPen(pen);
            painter->drawLines(_lines);
        }

    }
};
