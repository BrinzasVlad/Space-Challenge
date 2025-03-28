#ifndef ROCKET_H
#define ROCKET_H

class Rocket
{
public:
    Rocket(short numberOfEngines, double accelerationPerEngineInMetresPerSecondSquare);
    short getNumberOfEngines() const;
    double getAccelerationPerEngineInMetresPerSecondSquare() const;
    double getTotalAccelerationInMetresPerSecondSquare() const;

    double timeToReachTargetSpeed(const double targetSpeed, const double initialSpeed = 0) const;
private:
    short numberOfEngines;
    double accelerationPerEngineInMetresPerSecondSquare;
};

#endif // ROCKET_H
