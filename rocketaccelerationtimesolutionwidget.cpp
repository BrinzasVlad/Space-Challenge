#include "rocketaccelerationtimesolutionwidget.h"
#include "ui_rocketaccelerationtimesolutionwidget.h"

RocketAccelerationTimeSolutionWidget::RocketAccelerationTimeSolutionWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RocketAccelerationTimeSolutionWidget)
{
    ui->setupUi(this);

    // Set up table model
    rocketAccelerationTimeTableModel = new QStandardItemModel(this);
    rocketAccelerationTimeTableModel->setHorizontalHeaderLabels(
        {"Planet", "Escape velocity", "Time to escape velocity", "Distance travelled"}
        );
    ui->rocketAccelerationTimeTable->setModel(rocketAccelerationTimeTableModel);
    ui->rocketAccelerationTimeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

RocketAccelerationTimeSolutionWidget::~RocketAccelerationTimeSolutionWidget()
{
    delete rocketAccelerationTimeTableModel;
    delete ui;
}

namespace {
    double computeDistance(const double initialVelocity, const double acceleration, const double time) {
        // We have d = vt + at²/2
        return initialVelocity * time + acceleration * time * time / 2;
    }
}

void RocketAccelerationTimeSolutionWidget::setPlanets(const QList<Planet>& newPlanets) {
    planets = &newPlanets;
    updateRocketAccelerationTimeTable();
}

void RocketAccelerationTimeSolutionWidget::setRocket(const Rocket* newRocket) {
    rocket = newRocket;
    updateRocketAccelerationTimeTable();
}

void RocketAccelerationTimeSolutionWidget::updateRocketAccelerationTimeTable() {
    if (nullptr != rocket && nullptr != planets && !planets->empty()) {
        // Clear table, re-set-up header row
        rocketAccelerationTimeTableModel->clear();
        rocketAccelerationTimeTableModel->setHorizontalHeaderLabels(
            {"Planet", "Escape velocity", "Time to escape velocity", "Distance travelled"}
            );
        rocketAccelerationTimeTableModel->setRowCount(planets->length());

        // Populate table
        for (int i = 0; i < planets->length(); ++i) {
            double rocketTimeToEscapeVelocity = rocket->timeToReachTargetSpeed((*planets)[i].getEscapeVelocityInMetresPerSecond());

            double rocketAcceleration = rocket->getNumberOfEngines() * rocket->getAccelerationPerEngineInMetresPerSecondSquare();
            double rocketDistanceToEscapeVelocity = computeDistance(0, rocketAcceleration, rocketTimeToEscapeVelocity);

            rocketAccelerationTimeTableModel->setData(
                rocketAccelerationTimeTableModel->index(i, 0),
                (*planets)[i].getName()
                );
            rocketAccelerationTimeTableModel->setData(
                rocketAccelerationTimeTableModel->index(i, 1),
                QString("%1 m/s").arg((*planets)[i].getEscapeVelocityInMetresPerSecond())
                );
            rocketAccelerationTimeTableModel->setData(
                rocketAccelerationTimeTableModel->index(i, 2),
                QString("%1 s").arg(rocketTimeToEscapeVelocity)
                );
            rocketAccelerationTimeTableModel->setData(
                rocketAccelerationTimeTableModel->index(i, 3),
                QString("%1 m").arg(rocketDistanceToEscapeVelocity)
                );
        }

        // Show table
        ui->stackedWidget->setCurrentIndex(1);
    }
    else {
        // Hide table
        ui->stackedWidget->setCurrentIndex(0);
    }
}
