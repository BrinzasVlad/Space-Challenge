#ifndef PLANET_H
#define PLANET_H

#include <QString>

class Planet
{
public:
    Planet(const QString& name, double massInKilograms, double radiusInMetres);
    void setOrbitalCharacteristics(double orbitalPeriodInDays, double orbitalRadiusInAU);
    QString getName() const;
    double getMassInKilograms() const;
    double getRadiusInMetres() const;
    std::optional<double> getOrbitalPeriodInDays() const;
    std::optional<double> getOrbitalRadiusInAU() const;
    bool hasOrbitalData() const;

private:
    QString name;
    double massInKilograms;
    double radiusInMetres;
    std::optional<double> orbitalPeriodInDays;
    std::optional<double> orbitalRadiusInAU;
};

#endif // PLANET_H
