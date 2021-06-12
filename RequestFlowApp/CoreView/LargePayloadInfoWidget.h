#ifndef LARGEPAYLOADINFOWIDGET_H
#define LARGEPAYLOADINFOWIDGET_H

#include <QWidget>

namespace Ui {
class LargePayloadInfoWidget;
}

class LargePayloadInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LargePayloadInfoWidget(QWidget *parent = nullptr);
    ~LargePayloadInfoWidget();

signals:
    void viewClicked();

private:
    Ui::LargePayloadInfoWidget *ui;
};

#endif // LARGEPAYLOADINFOWIDGET_H
