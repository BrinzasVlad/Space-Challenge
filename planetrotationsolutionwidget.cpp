#include "planetrotationsolutionwidget.h"
#include "ui_planetrotationsolutionwidget.h"

#include "orbitalmath.h"

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
                const double planetAngle = OrbitalMath::angularPositionAfterDays(planet, daysElapsed);
                planetRotationTableModel->setData(planetRotationTableModel->index(i, 1), QString("%1°").arg(planetAngle));
            }
            else {
                // If planet does not have orbital data, say as such
                planetRotationTableModel->setData(planetRotationTableModel->index(i, 1), "No orbital data");
            }
        }
    }
}
