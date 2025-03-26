#include "rocketwidget.h"
#include "ui_rocketwidget.h"
#include "dialogutils.h"
#include "measurementunits.h"

#include <QFileDialog>
#include <QMessageBox>
#include <qregularexpression.h>

RocketWidget::RocketWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RocketWidget)
{
    ui->setupUi(this);
}

RocketWidget::~RocketWidget()
{
    delete ui;
    if (nullptr != rocket) delete rocket;
}

void RocketWidget::on_readRocketButton_clicked()
{
    // Open a file-picker dialog to fetch rocket data from a file
    // TODO: start from a more reasonable location maybe?
    QString rocketFileName = QFileDialog::getOpenFileName(this, tr("Select a rocket data file"));

    QFile rocketFile(rocketFileName);
    if (rocketFile.open(QIODeviceBase::ReadOnly)) {
        QList<QString> errors;
        short rocketNumberOfEngines = 0; // Initialize with placeholders
        double rocketAccelerationPerEngineInMetresPerSecondSquare = 0.0;

        // Read whole file into one string, since we expect it to be small
        QTextStream in(&rocketFile);
        QString rocketFileText = in.readAll();

        // Number of engines
        static QRegularExpression enginesRegex("Number of rocket engines: ([1-9][0-9]*)");
        QRegularExpressionMatch enginesMatch = enginesRegex.match(rocketFileText);
        if (!enginesMatch.hasMatch()) {
            errors.append("No number of engines specified for rocket; use format 'Number of rocket engines: <positive integer>'");
        }
        else {
            bool okConversion;
            rocketNumberOfEngines = enginesMatch.captured(1).toShort(&okConversion);
            if (!okConversion) errors.append(QString("Invalid number of engines '%1'"));
            // Above will 99.9% only happen if we exceed SHORT_MAX
            // Which probably isn't a reasonable number of engines on a rocket anyway.
        }

        // Acceleration per engine
        static QRegularExpression accelerationPerEngineRegex("Acceleration per engine: ([0-9]+(?:.[0-9]+)?) (k?m/s(?:\\^2|²))");
        // Regex explanation: accept an optionally-with-decimals number, accept seconds square as 's^2' or 's²'
        QRegularExpressionMatch accelerationPerEngineMatch = accelerationPerEngineRegex.match(rocketFileText);
        if (!accelerationPerEngineMatch.hasMatch()) {
            errors.append("No acceleration per engine specified; use format 'Acceleration per engine: <value> <m/s^2|km/s^2>'");
        }
        else {
            bool okConversion;
            double rocketAccelerationValue = accelerationPerEngineMatch.captured(1).toDouble(&okConversion);
            if (!okConversion) {
                errors.append(QString("Invalid acceleration per engine value '%1'").arg(accelerationPerEngineMatch.captured(1)));
            }

            AccelerationUnit accelerationPerEngineUnit = AccelerationUnit::parse(accelerationPerEngineMatch.captured(2));
            if (AccelerationUnit::INVALID == accelerationPerEngineUnit) {
                errors.append(QString("Invalid acceleration unit '%1'; expected 'm/s^2' or 'km/s^2'"));
            }
            rocketAccelerationPerEngineInMetresPerSecondSquare = AccelerationUnit::convert(
                    rocketAccelerationValue,
                    accelerationPerEngineUnit,
                    AccelerationUnit::METERS_PER_SECOND_SQUARE
                );
        }

        if (!errors.empty()) DialogUtils::showError(errors.join('\n'));
        else {
            // Replace rocket
            if (nullptr != rocket) delete rocket;
            rocket = new Rocket(rocketNumberOfEngines, rocketAccelerationPerEngineInMetresPerSecondSquare);

            // Update display
            updateRocketDisplay(rocket);

            // Emit signal
            emit rocketChanged(rocket);
        }

    } else {
        if (rocketFileName.isEmpty()) DialogUtils::showError("No file selected");
        else DialogUtils::showError(QString("Could not open file %1").arg(rocketFileName));
    }
}

void RocketWidget::updateRocketDisplay(Rocket* newRocket) {
    if (nullptr == newRocket) {
        ui->stackedWidget->setCurrentIndex(0);
    }
    else {
        ui->rocketNumberOfEnginesField->setText(QString::number(newRocket->getNumberOfEngines()));
        ui->rocketAccelerationPerEngineField->setText(
            QString("%1 m/s²").arg(newRocket->getAccelerationPerEngineInMetresPerSecondSquare())
        );

        ui->stackedWidget->setCurrentIndex(1);
    }
}
