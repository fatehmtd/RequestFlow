#include "singlelinetextedit.h"
#include <QStyleOptionFrame>
#include <QApplication>

SingleLineTextEdit::SingleLineTextEdit()
{
    setTabChangesFocus(true);
    setWordWrapMode(QTextOption::NoWrap);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setFixedHeight(sizeHint().height());
}

void SingleLineTextEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        event->ignore();
    else
        QTextEdit::keyPressEvent(event);
}

QSize SingleLineTextEdit::sizeHint() const
{
    QFontMetrics fm(font());
    int h = qMax(fm.height(), 14) + 4;
    int w = fm.horizontalAdvance(QLatin1Char('x')) * 17 + 4;
    QStyleOptionFrame opt; opt.initFrom(this);
    return (style()->sizeFromContents(QStyle::CT_LineEdit, &opt, QSize(w, h). expandedTo(QApplication::globalStrut()), this));
}
