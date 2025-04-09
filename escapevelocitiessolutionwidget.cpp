#include "escapevelocitiessolutionwidget.h"
#include "ui_escapevelocitiessolutionwidget.h"

#include "orbitalmath.h"

EscapeVelocitiesSolutionWidget::EscapeVelocitiesSolutionWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EscapeVelocitiesSolutionWidget)
{
    ui->setupUi(this);

    // Set up table model in preparation for data
    escapeVelocitiesTableModel = new QStandardItemModel(this);
    escapeVelocitiesTableModel->setHorizontalHeaderLabels({"Name", "Escape Velocity"});
    ui->escapeVelocitiesTable->setModel(escapeVelocitiesTableModel);
    ui->escapeVelocitiesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

EscapeVelocitiesSolutionWidget::~EscapeVelocitiesSolutionWidget()
{
    delete escapeVelocitiesTableModel;
    delete ui;
}

void EscapeVelocitiesSolutionWidget::setPlanets(const QList<Planet>& newPlanets) {
    // We could store the data if needed, but we don't do anything with it aside display this table

    // Clear table, re-set-up header row
    escapeVelocitiesTableModel->clear();
    escapeVelocitiesTableModel->setHorizontalHeaderLabels({"Name", "Escape Velocity"});
    escapeVelocitiesTableModel->setRowCount(newPlanets.length());

    // Populate table
    for (int i = 0; i < newPlanets.length(); ++i) {
        const Planet& planet = newPlanets[i];

        escapeVelocitiesTableModel->setData(escapeVelocitiesTableModel->index(i, 0),
                                            planet.getName());
        escapeVelocitiesTableModel->setData(escapeVelocitiesTableModel->index(i, 1),
                                            QString("%1 m/s").arg(OrbitalMath::escapeVelocity(planet)));
    }

    // Hide or display table as needed
    if (newPlanets.empty()) ui->stackedWidget->setCurrentIndex(0);
    else ui->stackedWidget->setCurrentIndex(1);
}
