#include "planet.h"

Planet::Planet(const QString& name, double massInKilograms, double radiusInMetres)
    : name(name)
    , massInKilograms(massInKilograms)
    , radiusInMetres(radiusInMetres) {}

void Planet::setOrbitalCharacteristics(double orbitalPeriodInDays, double orbitalRadiusInAU) {
    this->orbitalPeriodInDays = orbitalPeriodInDays;
    this->orbitalRadiusInAU = orbitalRadiusInAU;
}

QString Planet::getName() const {
    return name;
}

double Planet::getMassInKilograms() const {
    return massInKilograms;
}

double Planet::getRadiusInMetres() const {
    return radiusInMetres;
}

double Planet::getEscapeVelocityInMetresPerSecond() const {
    static const double gravitationalConstant = 6.67e-11; // m³/kg*s²

    if (!escapeVelocity.has_value()) {
        escapeVelocity = sqrt(2 * gravitationalConstant * massInKilograms / radiusInMetres);
    }

    return escapeVelocity.value();
}

std::optional<double> Planet::getOrbitalPeriodInDays() const {
    return orbitalPeriodInDays;
}

std::optional<double> Planet::getOrbitalRadiusInAU() const {
    return orbitalRadiusInAU;
}

bool Planet::hasOrbitalData() const {
    return orbitalPeriodInDays.has_value() && orbitalRadiusInAU.has_value();
}
