#ifndef ESCAPEVELOCITIESSOLUTIONWIDGET_H
#define ESCAPEVELOCITIESSOLUTIONWIDGET_H

#include "planet.h"

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class EscapeVelocitiesSolutionWidget;
}

class EscapeVelocitiesSolutionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EscapeVelocitiesSolutionWidget(QWidget *parent = nullptr);
    ~EscapeVelocitiesSolutionWidget();

public slots:
    void setPlanets(const QList<Planet>& newPlanets);

private:
    Ui::EscapeVelocitiesSolutionWidget *ui;
    QStandardItemModel* escapeVelocitiesTableModel;
};

#endif // ESCAPEVELOCITIESSOLUTIONWIDGET_H
