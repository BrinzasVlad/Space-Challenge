#include "measurementunits.h"

DistanceUnit::DistanceUnit(QString name, QString abbreviation, double ratioToMetres):
    UnitType<DistanceUnit>(name, abbreviation, ratioToMetres)
{}
const DistanceUnit DistanceUnit::METRES("Metres", "m", 1.0);
const DistanceUnit DistanceUnit::KILOMETRES("Kilometres", "km", 1000.0);
const DistanceUnit DistanceUnit::FEET("Feet", "ft", 0.3048);
const DistanceUnit DistanceUnit::MILES("Miles", "mi", 1609.344);
const DistanceUnit DistanceUnit::AU("Astronomical units", "AU", 149597870700);
std::vector<DistanceUnit> DistanceUnit::allValues() {
    return std::vector<DistanceUnit>({
        DistanceUnit::METRES,
        DistanceUnit::KILOMETRES,
        DistanceUnit::FEET,
        DistanceUnit::MILES,
        DistanceUnit::AU
    });
}

MassUnit::MassUnit(QString name, QString abbreviation, double ratioToKilograms):
    UnitType<MassUnit>(name, abbreviation, ratioToKilograms)
{}
const MassUnit MassUnit::KILOGRAMS("Kilograms", "kg", 1.0);
const MassUnit MassUnit::TONNES("Tonnes", "t", 1000.0);
const MassUnit MassUnit::EARTHS("Earths", "Earths", NAN); // Special case, handle based on mass given to Earth in file
// Arguably maybe 'Earths' should not even be a unit, but instead if some
// gibberish unit is given we assume it refers to some other planet in the file and
// compute based on that?
// Probably not worth the extra logic, though, unless this is likely to be common.
std::vector<MassUnit> MassUnit::allValues() {
    return std::vector<MassUnit>({
        MassUnit::KILOGRAMS,
        MassUnit::TONNES,
        MassUnit::EARTHS
    });
}

AccelerationUnit::AccelerationUnit(QString name, QString abbreviation, double ratioToMetresPerSecondSquare):
    UnitType<AccelerationUnit>(name, abbreviation, ratioToMetresPerSecondSquare)
{}
const AccelerationUnit AccelerationUnit::METERS_PER_SECOND_SQUARE("Meters per second square", "m/s²", 1.0);
const AccelerationUnit AccelerationUnit::KILOMETERS_PER_SECOND_SQUARE("Kilometers per second square", "km/s²", 1000.0);
std::vector<AccelerationUnit> AccelerationUnit::allValues() {
    return std::vector<AccelerationUnit>({
        AccelerationUnit::METERS_PER_SECOND_SQUARE,
        AccelerationUnit::KILOMETERS_PER_SECOND_SQUARE
    });
}
