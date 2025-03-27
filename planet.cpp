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

std::optional<double> Planet::getOrbitalPeriodInDays() const {
    return orbitalPeriodInDays;
}

std::optional<double> Planet::getOrbitalRadiusInAU() const {
    return orbitalRadiusInAU;
}
