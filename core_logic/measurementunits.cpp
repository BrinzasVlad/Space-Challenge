#include <QStringList>
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

TimeUnit::TimeUnit(QString name, QString abbreviation, double ratioToSeconds):
    UnitType<TimeUnit>(name, abbreviation, ratioToSeconds)
{}
const TimeUnit TimeUnit::SECONDS("Seconds", "s", 1.0);
const TimeUnit TimeUnit::MINUTES("Minutes", "m", 60.0);
const TimeUnit TimeUnit::HOURS("Hours", "h", 3600.0);
const TimeUnit TimeUnit::DAYS("Days", "d", 86400.0);
const TimeUnit TimeUnit::YEARS("Years", "y", 31556952.0); // Assuming 365.2425 days per year to account for leap years
std::vector<TimeUnit> TimeUnit::allValues() {
    return std::vector<TimeUnit>({
        TimeUnit::SECONDS,
        TimeUnit::MINUTES,
        TimeUnit::HOURS,
        TimeUnit::DAYS,
        TimeUnit::YEARS
    });
}
QString TimeUnit::formatTime(double totalTime, TimeUnit timeUnit) {
    static const int SECONDS_PER_DAY = TimeUnit::convert(1, TimeUnit::DAYS, TimeUnit::SECONDS);
    static const int SECONDS_PER_HOUR = TimeUnit::convert(1, TimeUnit::HOURS, TimeUnit::SECONDS);
    static const int SECONDS_PER_MINUTE = TimeUnit::convert(1, TimeUnit::MINUTES, TimeUnit::SECONDS);
    static const int SECOND_PER_YEAR = TimeUnit::convert(1, TimeUnit::YEARS, TimeUnit::SECONDS);

    double remainingSeconds = TimeUnit::convert(totalTime, timeUnit, TimeUnit::SECONDS);

    int years = (int)(remainingSeconds / SECOND_PER_YEAR);
    remainingSeconds -= years * SECOND_PER_YEAR;

    int days = (int)(remainingSeconds / SECONDS_PER_DAY);
    remainingSeconds -= days * SECONDS_PER_DAY;

    int hours = (int)(remainingSeconds / SECONDS_PER_HOUR);
    remainingSeconds -= hours * SECONDS_PER_HOUR;

    int minutes = (int)(remainingSeconds / SECONDS_PER_MINUTE);
    remainingSeconds -= minutes * SECONDS_PER_MINUTE;

    QStringList fragments;
    if (years) fragments.append(QString("%1 y").arg(years));
    if (days) fragments.append(QString("%1 d").arg(days));
    if (hours) fragments.append(QString("%1 h").arg(hours));
    if (minutes) fragments.append(QString("%1 m").arg(minutes));
    if (remainingSeconds) fragments.append(QString("%1 s").arg(remainingSeconds));

    return fragments.join(' ');
}

SpeedUnit::SpeedUnit(QString name, QString abbreviation, double ratioToMetresPerSecond):
    UnitType<SpeedUnit>(name, abbreviation, ratioToMetresPerSecond)
{}
const SpeedUnit SpeedUnit::METRES_PER_SECOND("Metres per second", "m/s", 1.0);
const SpeedUnit SpeedUnit::KILOMETRES_PER_HOUR("Kilometres per hour", "km/h", 0.2778);
const SpeedUnit SpeedUnit::MILES_PER_HOUR("Miles per hour", "mph", 0.447);
const SpeedUnit SpeedUnit::KILOMETRES_PER_SECOND("Kilometres per second", "km/s", 1000.0);
std::vector<SpeedUnit> SpeedUnit::allValues() {
    return std::vector<SpeedUnit>({
        SpeedUnit::METRES_PER_SECOND,
        SpeedUnit::KILOMETRES_PER_HOUR,
        SpeedUnit::MILES_PER_HOUR,
        SpeedUnit::KILOMETRES_PER_SECOND
    });
}

AngleUnit::AngleUnit(QString name, QString abbreviation, double ratioToDegrees):
    UnitType<AngleUnit>(name, abbreviation, ratioToDegrees)
{}
const AngleUnit AngleUnit::DEGREES("Degrees", "°", 1.0); // Use degrees as default because application does too
const AngleUnit AngleUnit::RADIANS("Radians", "rad", 57.295779513);
