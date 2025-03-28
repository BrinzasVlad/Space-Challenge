#ifndef ROCKETWIDGET_H
#define ROCKETWIDGET_H

#include "rocket.h"

#include <QWidget>

namespace Ui {
class RocketWidget;
}

class RocketWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RocketWidget(QWidget *parent = nullptr);
    ~RocketWidget();

signals:
    void rocketChanged(Rocket* newRocket);

private slots:
    void on_readRocketButton_clicked();

private:
    Ui::RocketWidget *ui;
    Rocket* rocket = nullptr;

    void updateRocketDisplay(Rocket* newRocket);
};

#endif // ROCKETWIDGET_H
