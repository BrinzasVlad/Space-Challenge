#include "planet.h"

Planet::Planet(const QString& name, double massInKilograms, double radiusInMetres)
    : name(name)
    , massInKilograms(massInKilograms)
    , radiusInMetres(radiusInMetres) {}

QString Planet::getName() const {
    return name;
}

double Planet::getMassInKilograms() const {
    return massInKilograms;
}

double Planet::getRadiusInMetres() const {
    return radiusInMetres;
}
