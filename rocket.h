#ifndef ROCKET_H
#define ROCKET_H

class Rocket
{
public:
    Rocket(short numberOfEngines, double accelerationPerEngineInMetresPerSecondSquare);
    short getNumberOfEngines() const;
    double getAccelerationPerEngineInMetresPerSecondSquare() const;
private:
    short numberOfEngines;
    double accelerationPerEngineInMetresPerSecondSquare;
};

#endif // ROCKET_H
