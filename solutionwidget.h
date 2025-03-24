#ifndef SOLUTIONWIDGET_H
#define SOLUTIONWIDGET_H

#include "planet.h"
#include "rocket.h"

#include <QWidget>

namespace Ui {
class SolutionWidget;
}

class SolutionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SolutionWidget(QWidget *parent = nullptr);
    ~SolutionWidget();

public slots:
    void setPlanets(const QList<Planet>& newPlanets);
    void setRocket(Rocket* newRocket);

private:
    Ui::SolutionWidget *ui;
};

#endif // SOLUTIONWIDGET_H
