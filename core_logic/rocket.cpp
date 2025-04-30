#include "rocket.h"

Rocket::Rocket(short numberOfEngines, double accelerationPerEngineInMetresPerSecondSquare)
    : numberOfEngines(numberOfEngines)
    , accelerationPerEngineInMetresPerSecondSquare(accelerationPerEngineInMetresPerSecondSquare) {}

short Rocket::getNumberOfEngines() const {
    return numberOfEngines;
}

double Rocket::getAccelerationPerEngineInMetresPerSecondSquare() const {
    return accelerationPerEngineInMetresPerSecondSquare;
}
