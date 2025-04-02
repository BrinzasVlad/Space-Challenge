#ifndef PLANETROTATIONSOLUTIONWIDGET_H
#define PLANETROTATIONSOLUTIONWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

#include "planet.h"

namespace Ui {
class PlanetRotationSolutionWidget;
}

class PlanetRotationSolutionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlanetRotationSolutionWidget(QWidget *parent = nullptr);
    ~PlanetRotationSolutionWidget();

public slots:
    void setPlanets(const QList<Planet>& newPlanets);

private slots:
    void on_daysElapsedSpinBox_valueChanged(int newDaysElapsed);

private:
    Ui::PlanetRotationSolutionWidget *ui;
    QStandardItemModel* planetRotationTableModel;
    const QList<Planet>* planets = nullptr;

    void updatePlanetRotationTable();
};

#endif // PLANETROTATIONSOLUTIONWIDGET_H
