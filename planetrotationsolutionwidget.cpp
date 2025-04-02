#include "planetrotationsolutionwidget.h"
#include "ui_planetrotationsolutionwidget.h"

PlanetRotationSolutionWidget::PlanetRotationSolutionWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlanetRotationSolutionWidget)
{
    ui->setupUi(this);

    // Set up table model in preparation for data
    planetRotationTableModel = new QStandardItemModel(this);
    planetRotationTableModel->setHorizontalHeaderLabels({"Name", "Angular Position"});
    ui->planetRotationTable->setModel(planetRotationTableModel);
    ui->planetRotationTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

PlanetRotationSolutionWidget::~PlanetRotationSolutionWidget()
{
    delete planetRotationTableModel;
    delete ui;
}

void PlanetRotationSolutionWidget::setPlanets(const QList<Planet>& newPlanets) {
    // Store reference to planets
    planets = &newPlanets;

    if (!planets->empty()) {
        // Update and display table
        updatePlanetRotationTable();
        ui->stackedWidget->setCurrentIndex(1);
    }
    else ui->stackedWidget->setCurrentIndex(0); // Display "no planets" page
}

void PlanetRotationSolutionWidget::on_daysElapsedSpinBox_valueChanged(int newDaysElapsed)
{
    updatePlanetRotationTable();
}

void PlanetRotationSolutionWidget::updatePlanetRotationTable() {
    if (nullptr != planets && !planets->empty()) {
        // Clear table of old data and re-set-up
        planetRotationTableModel->clear(); // Also clears header row
        planetRotationTableModel->setHorizontalHeaderLabels({"Name", "Angular Position"});
        planetRotationTableModel->setRowCount(planets->length());

        // Fetch days elapsed since time 0 (when all planets were at angle 0°)
        const int daysElapsed = ui->daysElapsedSpinBox->value();

        // Insert planets into table
        for (int i = 0; i < planets->length(); ++i) {
            const Planet& planet = planets->at(i);

            // Insert planet name
            planetRotationTableModel->setData(planetRotationTableModel->index(i, 0), planet.getName());

            if (planet.getOrbitalPeriodInDays().has_value()) {
                // If planet has orbital data, compute angle and insert
                const double orbitalPeriod = planet.getOrbitalPeriodInDays().value();

                const int fullRotations = (int)(daysElapsed / orbitalPeriod);
                const double partialRotations = (daysElapsed - fullRotations * orbitalPeriod) / orbitalPeriod;

                const double planetAngle = 360 * partialRotations;
                planetRotationTableModel->setData(planetRotationTableModel->index(i, 1), QString("%1°").arg(planetAngle));
            }
            else {
                // If planet does not have orbital data, say as such
                planetRotationTableModel->setData(planetRotationTableModel->index(i, 1), "No orbital data");
            }
        }
    }
    // Check that we have valid planet data to work with
    // Fetch value of days elapsed for each planet
    // Compute current angular position as 360° * (daysElapsed % orbitalPeriod) / orbitalPeriod for each planet and insert into table
}
