#include "simplifiedstraightjourneysolutionwidget.h"
#include "ui_simplifiedstraightjourneysolutionwidget.h"

#include "measurementunits.h"

SimplifiedStraightJourneySolutionWidget::SimplifiedStraightJourneySolutionWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SimplifiedStraightJourneySolutionWidget)
{
    ui->setupUi(this);
    textDisplayTemplate = ui->resultsDisplayLabel->text(); // Store template so we can put it back if needed
}

SimplifiedStraightJourneySolutionWidget::~SimplifiedStraightJourneySolutionWidget()
{
    delete ui;
}

void SimplifiedStraightJourneySolutionWidget::setPlanets(const QList<Planet>& newPlanets) {
    planets = &newPlanets;

    updateDisplayedPage();
    updatePlanetSelectOptions();
    updateResultsDisplay();
}

void SimplifiedStraightJourneySolutionWidget::setRocket(const Rocket* newRocket) {
    rocket = newRocket;

    updateDisplayedPage();
    updateResultsDisplay();
}

namespace {
    const auto NO_PLANETS_SELECTED("No origin/destination planets selected.\nPlease select target planets to continue.");
    const auto NO_ORBITAL_DATA("No orbital data loaded for %1.\nPlease load orbital data from the side panel to continue.");
    const auto NO_ROCKET_DATA("No rocket data loaded.\nPlease load rocket data from the side panel to continue.");
    const auto SAME_PLANET("Same planet selected as both origin and destination.\nPlease select two different planets to continue.");
    const auto INVALID_PLANET("Invalid planet selected: %1.\nPlease select a valid planet from the options list to continue.");

    double computeDistance(const double initialVelocity, const double acceleration, const double time) {
        // We have d = vt + at²/2
        return initialVelocity * time + acceleration * time * time / 2;
    }

    QString formatTime(const double timeInSeconds) {
        static const int SECONDS_PER_DAY = TimeUnit::convert(1, TimeUnit::DAYS, TimeUnit::SECONDS);
        static const int SECONDS_PER_HOUR = TimeUnit::convert(1, TimeUnit::HOURS, TimeUnit::SECONDS);
        static const int SECONDS_PER_MINUTE = TimeUnit::convert(1, TimeUnit::MINUTES, TimeUnit::SECONDS);

        double remainingSeconds = timeInSeconds;

        int days = (int)(remainingSeconds / SECONDS_PER_DAY);
        remainingSeconds -= days * SECONDS_PER_DAY;

        int hours = (int)(remainingSeconds / SECONDS_PER_HOUR);
        remainingSeconds -= hours * SECONDS_PER_HOUR;

        int minutes = (int)(remainingSeconds / SECONDS_PER_MINUTE);
        remainingSeconds -= minutes * SECONDS_PER_MINUTE;

        QStringList fragments;
        if (days) fragments.append(QString("%1 d").arg(days));
        if (hours) fragments.append(QString("%1 h").arg(hours));
        if (minutes) fragments.append(QString("%1 m").arg(minutes));
        if (remainingSeconds) fragments.append(QString("%1 s").arg(remainingSeconds));

        return fragments.join(' ');
    }
}

void SimplifiedStraightJourneySolutionWidget::updateDisplayedPage() {
    if (nullptr != rocket && nullptr != planets && !planets->empty()) {
        // Show main processing section
        ui->stackedWidget->setCurrentIndex(1);
    }
    else ui->stackedWidget->setCurrentIndex(0);
}

void SimplifiedStraightJourneySolutionWidget::updatePlanetSelectOptions() {
    // Try to keep current options if possible
    const QString oldFromPlanet = ui->fromPlanetSelect->currentText();
    const QString oldToPlanet = ui->toPlanetSelect->currentText();

    // Clear options and load with new planets
    ui->fromPlanetSelect->clear();
    ui->toPlanetSelect->clear();

    for (int i = 0; i < planets->length(); ++i) {
        const Planet& planet = (*planets)[i];

        ui->fromPlanetSelect->addItem(planet.getName());
        ui->toPlanetSelect->addItem(planet.getName());
    }

    // Try to reinstate old options
    // If not found, Qt will just ignore us since the QComboBox is not editable
    ui->fromPlanetSelect->setCurrentText(oldFromPlanet);
    ui->toPlanetSelect->setCurrentText(oldToPlanet);
}

void SimplifiedStraightJourneySolutionWidget::updateResultsDisplay() {
    // Get selected planet names
    const QString fromPlanetName = ui->fromPlanetSelect->currentText();
    const QString toPlanetName = ui->toPlanetSelect->currentText();

    // First, check whether we can compute at all and warn user if not.
    ui->resultsStackedWidget->setCurrentIndex(0); // Assume error, put back once we're sure all is good

    if (fromPlanetName.isEmpty() || toPlanetName.isEmpty()) {
        ui->resultsIssueLabel->setText(NO_PLANETS_SELECTED);
        return;
    }
    if (fromPlanetName == toPlanetName) {
        ui->resultsIssueLabel->setText(SAME_PLANET);
        return;
    }
    if (nullptr == rocket) {
        ui->resultsIssueLabel->setText(NO_ROCKET_DATA); // Should never be visible, but just in case
        return;
    }

    // Get selected planets
    // Can't use const Planet& references unitialized, annoyingly
    const Planet* fromPlanet = nullptr;
    const Planet* toPlanet = nullptr;
    for (int i = 0; i < planets->length(); ++i) {
        const Planet& planet = (*planets)[i];

        if (planet.getName() == fromPlanetName) fromPlanet = &planet;
        if (planet.getName() == toPlanetName) toPlanet = &planet;
    }

    // Continue checks and warn user of issues
    if (nullptr == fromPlanet || nullptr == toPlanet) {
        // This should be impossible given how we populate the QComboBox
        // But we'll check, just in case
        QStringList invalidPlanets;
        if (nullptr == fromPlanet) invalidPlanets.append(fromPlanetName);
        if (nullptr == toPlanet) invalidPlanets.append(toPlanetName);

        QString invalidPlanetNames = invalidPlanets.join(", ");
        ui->resultsIssueLabel->setText(QString(INVALID_PLANET).arg(invalidPlanetNames));

        return;
    }
    if (!fromPlanet->hasOrbitalData() || !toPlanet->hasOrbitalData()) {
        QStringList invalidPlanets;
        if (!fromPlanet->hasOrbitalData()) invalidPlanets.append(fromPlanetName);
        if (!toPlanet->hasOrbitalData()) invalidPlanets.append(toPlanetName);

        QString invalidPlanetNames = invalidPlanets.join(", ");
        ui->resultsIssueLabel->setText(QString(NO_ORBITAL_DATA).arg(invalidPlanetNames));

        return;
    }

    // If we got here, all is well and we can display the results page
    ui->resultsStackedWidget->setCurrentIndex(1);

    // Compute all the needed values
    // Work with all values in SI, convert to desired units at the end
    double closestDistanceBetweenPlanets = DistanceUnit::convert(
        std::abs(fromPlanet->getOrbitalRadiusInAU().value() - toPlanet->getOrbitalRadiusInAU().value()),
        DistanceUnit::AU,
        DistanceUnit::METRES
    );
    double cruisingVelocity = std::max(
        fromPlanet->getEscapeVelocityInMetresPerSecond(),
        toPlanet->getEscapeVelocityInMetresPerSecond()
    );
    double accelerationTime = rocket->timeToReachTargetSpeed(cruisingVelocity);
    double accelerationDistance = computeDistance(
        0, // Start from initial speed zero
        rocket->getTotalAccelerationInMetresPerSecondSquare(),
        accelerationTime
    );
    double decelerationTime = accelerationTime;
    double decelerationDistance = accelerationDistance;
    double cruisingDistance = closestDistanceBetweenPlanets
                              - fromPlanet->getRadiusInMetres()
                              - accelerationDistance
                              - decelerationDistance
                              - toPlanet->getRadiusInMetres();
    double cruisingTime = cruisingDistance / cruisingVelocity;
    double totalJourneyTime = accelerationTime + cruisingTime + decelerationTime;

    // Then insert into UI
    QString newResultsText = textDisplayTemplate; // Make a copy
    newResultsText
        .replace("{fromPlanet}", fromPlanetName)
        .replace("{toPlanet}", toPlanetName)
        .replace("{closestDistance}", QString::number(
            DistanceUnit::convert(closestDistanceBetweenPlanets, DistanceUnit::METRES, DistanceUnit::AU)
        ))
        .replace("{cruisingVelocityM/S}", QString::number(cruisingVelocity))
        .replace("{cruisingVelocityKm/S}", QString::number(
            SpeedUnit::convert(cruisingVelocity, SpeedUnit::METRES_PER_SECOND, SpeedUnit::METRES_PER_SECOND)
        ))
        .replace("{accelerationTime}", QString::number(accelerationTime))
        .replace("{accelerationDistance}", QString::number(
            DistanceUnit::convert(accelerationDistance, DistanceUnit::METRES, DistanceUnit::KILOMETRES)
        ))
        .replace("{cruisingTime}", QString::number(cruisingTime))
        .replace("{cruisingTimeFormatted}", formatTime(cruisingTime))
        .replace("{decelerationDistance}", QString::number(
            DistanceUnit::convert(decelerationDistance, DistanceUnit::METRES, DistanceUnit::KILOMETRES)
        ))
        .replace("{decelerationTime}", QString::number(decelerationTime))
        .replace("{totalJourneyTime}", QString::number(totalJourneyTime))
        .replace("{totalJourneyTimeFormatted}", formatTime(totalJourneyTime));
    // Note: the above is probably quite inefficient
    // We could make this more efficient by:
    // 1. assembling the string out of parts rather than replacing parts
    // 2. changing the UI to, for instance, be made up of many small labels, so
    //    we can replace each targeted label individually
    ui->resultsDisplayLabel->setText(newResultsText);
}

void SimplifiedStraightJourneySolutionWidget::on_fromPlanetSelect_currentTextChanged(const QString& newFromPlanetName)
{
    // No need to do anything with the name, since updateResultsDisplay() reads it anyway
    updateResultsDisplay();
}


void SimplifiedStraightJourneySolutionWidget::on_toPlanetSelect_currentTextChanged(const QString& newToPlanetName)
{
    // No need to do anything with the name, since updateResultsDisplay() reads it anyway
    updateResultsDisplay();
}

