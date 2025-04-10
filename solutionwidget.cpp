#include "solutionwidget.h"
#include "ui_solutionwidget.h"

SolutionWidget::SolutionWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SolutionWidget)
{
    ui->setupUi(this);

    connect(this, &SolutionWidget::planetsChanged,
            ui->stageOneTab, &EscapeVelocitiesSolutionWidget::setPlanets);

    connect(this, &SolutionWidget::planetsChanged,
            ui->stageTwoTab, &RocketAccelerationTimeSolutionWidget::setPlanets);
    connect(this, &SolutionWidget::rocketChanged,
            ui->stageTwoTab, &RocketAccelerationTimeSolutionWidget::setRocket);

    connect(this, &SolutionWidget::planetsChanged,
            ui->stageThreeTab, &SimplifiedStraightJourneySolutionWidget::setPlanets);
    connect(this, &SolutionWidget::rocketChanged,
            ui->stageThreeTab, &SimplifiedStraightJourneySolutionWidget::setRocket);

    connect(this, &SolutionWidget::planetsChanged,
            ui->stageFourTab, &PlanetRotationSolutionWidget::setPlanets);
}

SolutionWidget::~SolutionWidget()
{
    delete ui;
}

void SolutionWidget::setPlanets(const QList<Planet>& newPlanets) {
    // Forward signal
    emit planetsChanged(newPlanets);
}

void SolutionWidget::setRocket(const Rocket* newRocket) {
    // Forward signal
    emit rocketChanged(newRocket);
}
