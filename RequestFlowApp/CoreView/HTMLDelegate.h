#pragma once

#include <QStyledItemDelegate>
#include <QPainter>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QApplication>

namespace view
{
    class HTMLDelegate : public QStyledItemDelegate
    {
        Q_OBJECT
    public:
        HTMLDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}

        void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
        {
            QStyleOptionViewItem opt = option;
            initStyleOption(&opt, index);

            QStyle* style = opt.widget ? opt.widget->style() : QApplication::style();

            QTextDocument doc;
            doc.setHtml(opt.text);
            doc.setDefaultFont(opt.font);

            // Clear the text since we'll draw it ourselves
            opt.text = QString();
            style->drawControl(QStyle::CE_ItemViewItem, &opt, painter);

            // Draw the HTML text
            painter->save();
            painter->translate(opt.rect.left(), opt.rect.top());
            QRect clip(0, 0, opt.rect.width(), opt.rect.height());
            doc.drawContents(painter, clip);
            painter->restore();
        }

        QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override
        {
            QStyleOptionViewItem opt = option;
            initStyleOption(&opt, index);

            QTextDocument doc;
            doc.setHtml(opt.text);
            doc.setDefaultFont(opt.font);
            // Don't set text width to allow horizontal scrolling
            doc.setTextWidth(-1);  // -1 means no wrapping
            return QSize(doc.idealWidth(), doc.size().height());
        }
    };
}
