#ifndef PLANETSWIDGET_H
#define PLANETSWIDGET_H

#include "planet.h"

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class PlanetsWidget;
}

class PlanetsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlanetsWidget(QWidget *parent = nullptr);
    ~PlanetsWidget();

signals:
    void planetsChanged(const QList<Planet>& newPlanets);

private slots:
    void on_readPlanetsButton_clicked();
    void on_readOrbitalDataButton_clicked();

private:
    Ui::PlanetsWidget *ui;
    QList<Planet> planets;
    QStandardItemModel* planetTableModel;

    void updatePlanetsTable(QList<Planet> newPlanets, bool haveOrbitalData = false);
};

#endif // PLANETSWIDGET_H
