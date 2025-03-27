#ifndef ROCKETACCELERATIONTIMESOLUTIONWIDGET_H
#define ROCKETACCELERATIONTIMESOLUTIONWIDGET_H

#include "planet.h"
#include "rocket.h"

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class RocketAccelerationTimeSolutionWidget;
}

class RocketAccelerationTimeSolutionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RocketAccelerationTimeSolutionWidget(QWidget *parent = nullptr);
    ~RocketAccelerationTimeSolutionWidget();

public slots:
    void setPlanets(const QList<Planet>& newPlanets);
    void setRocket(const Rocket* newRocket);

private:
    Ui::RocketAccelerationTimeSolutionWidget *ui;
    const QList<Planet>* planets = nullptr;
    const Rocket* rocket = nullptr;

    QStandardItemModel* rocketAccelerationTimeTableModel;
    void updateRocketAccelerationTimeTable();
};

#endif // ROCKETACCELERATIONTIMESOLUTIONWIDGET_H
