#include "solutionwidget.h"
#include "ui_solutionwidget.h"

SolutionWidget::SolutionWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SolutionWidget)
{
    ui->setupUi(this);
}

SolutionWidget::~SolutionWidget()
{
    delete ui;
}

void SolutionWidget::setPlanets(const QList<Planet>& newPlanets) {
    QStringList planetNames;

    for (int i = 0; i < newPlanets.length(); ++i) {
        planetNames.append(newPlanets[i].getName());
    }

    QString planetNamesString = planetNames.join(", ");
    ui->planetsField->setText(planetNamesString);
}

void SolutionWidget::setRocket(Rocket* rocket) {
    QString rocketEnginesString = QString("%1 x %2 m/s^2")
                                      .arg(rocket->getNumberOfEngines(),
                                           rocket->getAccelerationPerEngineInMetresPerSecondSquare());
    ui->rocketField->setText(rocketEnginesString);
}
