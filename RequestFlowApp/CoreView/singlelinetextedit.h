#pragma once

#include <QTextEdit>
#include <QKeyEvent>

class SingleLineTextEdit : public QTextEdit
{
public:
    SingleLineTextEdit();

    void keyPressEvent(QKeyEvent *event) override;

    QSize sizeHint() const override;
};
