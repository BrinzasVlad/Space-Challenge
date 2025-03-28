#ifndef SIMPLIFIEDSTRAIGHTJOURNEYSOLUTIONWIDGET_H
#define SIMPLIFIEDSTRAIGHTJOURNEYSOLUTIONWIDGET_H

#include <QWidget>

#include "planet.h"
#include "rocket.h"

namespace Ui {
class SimplifiedStraightJourneySolutionWidget;
}

class SimplifiedStraightJourneySolutionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SimplifiedStraightJourneySolutionWidget(QWidget *parent = nullptr);
    ~SimplifiedStraightJourneySolutionWidget();

public slots:
    void setPlanets(const QList<Planet>& newPlanets);
    void setRocket(const Rocket* newRocket);

private slots:
    void on_fromPlanetSelect_currentTextChanged(const QString& newFromPlanetName);
    void on_toPlanetSelect_currentTextChanged(const QString& newToPlanetName);

private:
    Ui::SimplifiedStraightJourneySolutionWidget *ui;
    const QList<Planet>* planets;
    const Rocket* rocket;
    QString textDisplayTemplate;

    void updateDisplayedPage();
    void updatePlanetSelectOptions();
    void updateResultsDisplay();
};

#endif // SIMPLIFIEDSTRAIGHTJOURNEYSOLUTIONWIDGET_H
