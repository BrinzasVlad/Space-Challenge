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

double Rocket::timeToReachTargetSpeed(const double targetSpeed, const double initialSpeed) const {
    const double rocketAcceleration = numberOfEngines * accelerationPerEngineInMetresPerSecondSquare;

    // Δv = at, so t = Δv/a
    return (targetSpeed - initialSpeed) / rocketAcceleration;

    // If acceleration is equal to deceleration, we could use std::abs(targetSpeed - initialSpeed)
    // to handle deceleration as well.
}
