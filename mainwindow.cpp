#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->planetsWidget, &PlanetsWidget::planetsChanged,
            ui->solutionWidget, &SolutionWidget::setPlanets);
    connect(ui->rocketWidget, &RocketWidget::rocketChanged,
            ui->solutionWidget, &SolutionWidget::setRocket);
}

MainWindow::~MainWindow()
{
    delete ui;
}
