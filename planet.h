#ifndef PLANET_H
#define PLANET_H

#include <QString>

class Planet
{
public:
    Planet(const QString& name, double massInKilograms, double radiusInMetres);
    QString getName() const;
    double getMassInKilograms() const;
    double getRadiusInMetres() const;

private:
    QString name;
    double massInKilograms;
    double radiusInMetres;
};

#endif // PLANET_H
