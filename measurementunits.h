#ifndef MEASUREMENTUNITS_H
#define MEASUREMENTUNITS_H

#include <QString>

// TODO: maybe this header should be split?
// E.g. one header for the template, then many small header+cpps for the units?

template <class DerivedUnit>
class UnitType {
protected:
    QString _name;
    QString _abbreviation;
    double _ratioToSIUnit;

    UnitType(QString name, QString abbreviation, double ratioToSIUnit):
        _name(name),
        _abbreviation(abbreviation),
        _ratioToSIUnit(ratioToSIUnit)
    {}
    virtual ~UnitType() {}; // Virtual destructor to avoid polymorphic deletion

public:
    QString name() const { return _name; }
    QString abbreviation() const { return _abbreviation; }

    static double convert(double value, const DerivedUnit& from, const DerivedUnit& to) {
        return value * from._ratioToSIUnit / to._ratioToSIUnit;
    }

    bool operator==(const DerivedUnit& other) const {
        return static_cast<const DerivedUnit*>(this)->_name == other._name;
        // Could check abbreviation and ratio as well, but name should be unique
    }
    bool operator!=(const DerivedUnit& other) const {
        return static_cast<const DerivedUnit*>(this)->_name != other._name;
        // Could check abbreviation and ratio as well, but name should be unique
    }

    // Call `allValues()` static function of derived class to get all derived-class units
    // Possibly return `INVALID` static value of derived class for no-matching-unit
    static DerivedUnit parse(const QString& string, const std::vector<DerivedUnit>& acceptedUnits = DerivedUnit::allValues()) {
        // Try abbreviations
        for (DerivedUnit unit : acceptedUnits) {
            if (0 == string.compare(unit._abbreviation, Qt::CaseInsensitive)) return unit;
        }

        // Try names
        for (DerivedUnit unit : acceptedUnits) {
            if (0 == string.compare(unit._name, Qt::CaseInsensitive)) return unit;
        }

        // Extra: if above failed, try replacing ^2 and ^3 with superscript ² and ³ and checking abbreviations again
        QString stringWithReplacements(string);
        stringWithReplacements.replace("^2", "²").replace("^3", "³");
        for (DerivedUnit unit : acceptedUnits) {
            if (0 == stringWithReplacements.compare(unit._abbreviation, Qt::CaseInsensitive)) return unit;
        }

        // If nothing matched, return invalid
        return INVALID;
    }

    static const DerivedUnit INVALID;
};
template <class DerivedUnit>
const DerivedUnit UnitType<DerivedUnit>::INVALID("Invalid unit", "invalid", NAN);


class DistanceUnit : public UnitType<DistanceUnit> {
private:
    DistanceUnit(QString name, QString abbreviation, double ratioToMetres);
    friend class UnitType<DistanceUnit>; // Needed to allow UnitType to define INVALID

public:
    static const DistanceUnit METRES;
    static const DistanceUnit KILOMETRES;
    static const DistanceUnit FEET;
    static const DistanceUnit MILES;
    static const DistanceUnit AU;
    static std::vector<DistanceUnit> allValues();
};


class MassUnit : public UnitType<MassUnit> {
private:
    MassUnit(QString name, QString abbreviation, double ratioToKilograms);
    friend class UnitType<MassUnit>; // Needed to allow UnitType to define INVALID

public:
    static const MassUnit KILOGRAMS;
    static const MassUnit TONNES;
    static const MassUnit EARTHS;
    static std::vector<MassUnit> allValues();
};


class AccelerationUnit : public UnitType<AccelerationUnit> {
private:
    AccelerationUnit(QString name, QString abbreviation, double ratioToMetresPerSecondSquare);
    friend class UnitType<AccelerationUnit>; // Needed to allow UnitType to define INVALID

public:
    static const AccelerationUnit METERS_PER_SECOND_SQUARE;
    static const AccelerationUnit KILOMETERS_PER_SECOND_SQUARE;
    static std::vector<AccelerationUnit> allValues();
};


class TimeUnit : public UnitType<TimeUnit> {
private:
    TimeUnit(QString name, QString abbreviation, double ratioToSeconds);
    friend class UnitType<TimeUnit>; // Needed to allow UnitType to define INVALID

public:
    static const TimeUnit SECONDS;
    static const TimeUnit MINUTES;
    static const TimeUnit HOURS;
    static const TimeUnit DAYS;
    static const TimeUnit YEARS;
    static std::vector<TimeUnit> allValues();
};


class SpeedUnit : public UnitType<SpeedUnit> {
private:
    SpeedUnit(QString name, QString abbreviation, double ratioToMetresPerSecond);
    friend class UnitType<SpeedUnit>; // Needed to allow UnitType to define INVALID

public:
    static const SpeedUnit METRES_PER_SECOND;
    static const SpeedUnit KILOMETRES_PER_HOUR;
    static const SpeedUnit MILES_PER_HOUR;
    static const SpeedUnit KILOMETRES_PER_SECOND;
    static std::vector<SpeedUnit> allValues();
};


class AngleUnit : public UnitType<AngleUnit> {
private:
    AngleUnit(QString name, QString abbreviation, double ratioToDegrees);
    friend class UnitType<AngleUnit>; // Needed to allow UnitType to define INVALID

public:
    static const AngleUnit DEGREES;
    static const AngleUnit RADIANS;
    static std::vector<AngleUnit> allValues();
};

#endif // MEASUREMENTUNITS_H
