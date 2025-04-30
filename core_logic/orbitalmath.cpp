#include "orbitalmath.h"
#include "measurementunits.h"

double OrbitalMath::escapeVelocity(const Planet& planet) {
    static const double gravitationalConstant = 6.67e-11; // m³/kg*s²

    const double planetMass = planet.getMassInKilograms();
    const double planetRadius = planet.getRadiusInMetres();

    return sqrt(2 * gravitationalConstant * planetMass / planetRadius);
}

double OrbitalMath::totalAcceleration(const Rocket& rocket) {
    return rocket.getNumberOfEngines() * rocket.getAccelerationPerEngineInMetresPerSecondSquare();
}

double OrbitalMath::angularVelocity(const Planet& planet) {
    // TODO: how should we handle planets lacking orbital data?
    // Currently we simply trigger an error trying to access the optional.
    return 360.0 / planet.getOrbitalPeriodInDays().value();
}

OrbitalMath::AccelerationResult
OrbitalMath::computeAccelerationValues(const Rocket& rocket, double targetVelocity, double initialVelocity, double ambientAcceleration) {
    const double velocityDelta = targetVelocity - initialVelocity;

    // Rocket can accelerate or decelerate depending on what's needed
    // Note: we assume rocket decelerates as well as it accelerates.
    const double rocketAcceleration = velocityDelta > 0 ? totalAcceleration(rocket) : -totalAcceleration(rocket);

    // Include ambient factor in calculation
    const double overallAcceleration = rocketAcceleration + ambientAcceleration;

    // Δv = at, so t = Δv/a
    const double accelerationTime = velocityDelta / overallAcceleration;

    // Δx = vt + at²/2
    const double accelerationDistance = initialVelocity * accelerationTime
                                        + overallAcceleration * accelerationTime * accelerationTime / 2;

    return { accelerationTime, accelerationDistance };
}

double OrbitalMath::daysUntilAligned(const Planet& first, double firstAngularPosition, const Planet& second, double secondAngularPosition) {
    // If already aligned, shortcut
    if (firstAngularPosition == secondAngularPosition) return 0;

    // Discard extraneous rotations, keep angularΔ between + and - 360°.
    const double angularDelta = std::fmod(secondAngularPosition - firstAngularPosition, 360.0);

    // Angular velocities in °/day
    const double firstAngularVelocity = angularVelocity(first);
    const double secondAngularVelocity = angularVelocity(second);
    const double angularVelocityDelta = secondAngularVelocity - firstAngularVelocity;

    // Need to handle signs
    // If angularΔ has different sign from angularVelocityΔ,
    // we should add/subtract 360° to make them match.
    double targetAngularDelta = angularDelta;
    if (targetAngularDelta < 0 && angularVelocityDelta > 0) targetAngularDelta += 360.0;
    if (targetAngularDelta > 0 && angularVelocityDelta < 0) targetAngularDelta -= 360.0;

    // #days = angularΔ (°) / angularVelocityΔ (°/day)
    // Will always be positive since angularΔ and angularVelocityΔ
    // have the same sign.
    return targetAngularDelta / angularVelocityDelta;
}

double OrbitalMath::shortestDistance(const Planet& from, const Planet& to) {
    // TODO: how should we handle planets lacking orbital data?
    // Currently we simply trigger an error trying to access the optional.

    const double fromOrbitalRadius = from.getOrbitalRadiusInAU().value();
    const double toOrbitalRadius = to.getOrbitalRadiusInAU().value();

    const double distanceBetweenCentres = abs(fromOrbitalRadius - toOrbitalRadius);
    const double distanceBetweenSurfaces = distanceBetweenCentres
                                           - DistanceUnit::convert(from.getRadiusInMetres(), DistanceUnit::METRES, DistanceUnit::AU)
                                           - DistanceUnit::convert(to.getRadiusInMetres(), DistanceUnit::METRES, DistanceUnit::AU);

    return distanceBetweenSurfaces;
}

double OrbitalMath::shortestDistanceConstrained(
    const Planet& from,
    double fromAngularPosition,
    const Planet& to,
    double toAngularPosition,
    double maximumTimeframe
    ) {

    // Compute time until planets align
    const double daysUntilAligned = OrbitalMath::daysUntilAligned(from, fromAngularPosition, to, toAngularPosition);

    // If time is within timeframe, compute with simple formula
    if (daysUntilAligned <= maximumTimeframe) return shortestDistance(from, to);

    // Otherwise, we need to figure the closest-aligned moment
    //
    // If alignment does not happen in the timeframe, then the closest
    // moment to it should be either the first or the last moment in the
    // timeframe.
    //
    // (Justification: imagine the angularΔ between the planets as an angle
    // on a 360° circle, with the angularVelocityΔ being a ray that sweeps that
    // circle. If alignment does not happen in the timeframe, it is because the
    // angularΔ angle was not located within the circular sector angularVelocityΔ
    // swept. Then the point that's closest on the circle to the angularΔ angle
    // will correspond to one of the ends of the swept arc - therefore, to either
    // the start or the end of the timeframe.)
    double angularDeltaStart = std::fmod(toAngularPosition - fromAngularPosition, 360.0);
    if (angularDeltaStart < 0) angularDeltaStart += 360.0;

    const double fromAngularPositionEnd = angularPositionAfterDays(from, maximumTimeframe, fromAngularPosition);
    const double toAngularPositionEnd = angularPositionAfterDays(to, maximumTimeframe, toAngularPosition);
    double angularDeltaEnd = std::fmod(toAngularPositionEnd - fromAngularPositionEnd, 360.0);
    if (angularDeltaEnd < 0) angularDeltaEnd += 360.0;

    if (angularDeltaStart <= angularDeltaEnd) {
        // Starting alignment is closer and should be preferred
        // (Or equal, and in case of ties we favour the earlier moment.)
        return distance(from, fromAngularPosition, to, toAngularPosition);
    }
    else {
        // Final alignment is closer and should be preferred
        return distance(from, fromAngularPositionEnd, to, toAngularPositionEnd);
    }
}

double OrbitalMath::angularPositionAfterDays(const Planet& planet, double daysElapsed, double initialAngularPosition) {
    // TODO: how should we handle planets lacking orbital data?
    // Currently we simply trigger an error trying to access the optional.
    const double orbitalPeriod = planet.getOrbitalPeriodInDays().value();

    // angularΔ = angularVelocity [°/day] * time [days]
    // angularVelocity = 360° / orbitalPeriod [days]
    // So angularΔ = (360 / orbitalPeriod) * time
    const double angularDelta = (360.0 / orbitalPeriod) * daysElapsed;

    // modulo to within 0-360°
    const double finalAngularPosition = std::fmod(initialAngularPosition + angularDelta, 360.0);

    return finalAngularPosition;
}

std::pair<double, double> OrbitalMath::polarToCartesianCoordinates(const Planet& planet, double angularPosition) {
    // TODO: how should we handle planets lacking orbital data?
    // Currently we simply trigger an error trying to access the optional.
    const double orbitalRadius = planet.getOrbitalRadiusInAU().value();
    const double angularPositionInRadians = AngleUnit::convert(angularPosition, AngleUnit::DEGREES, AngleUnit::RADIANS);

    const double x = orbitalRadius * std::cos(angularPositionInRadians);
    const double y = orbitalRadius * std::sin(angularPositionInRadians);

    return {x, y};
}

double OrbitalMath::distance(const Planet& from, double fromAngularPosition, const Planet& to, double toAngularPosition) {
    // TODO: how should we handle planets lacking orbital data?
    // Currently we simply trigger an error trying to access the optional.
    const double angularDelta = std::fmod(toAngularPosition - fromAngularPosition, 360.0);

    // Simple case: planets are aligned
    if (0 == angularDelta) {
        const double fromOrbitalRadius = from.getOrbitalRadiusInAU().value();
        const double toOrbitalRadius = to.getOrbitalRadiusInAU().value();

        return std::abs(toOrbitalRadius - fromOrbitalRadius);
    }

    // Simple case: planets are in opposing directions
    if (180.0 == angularDelta || -180.0 == angularDelta) {
        const double fromOrbitalRadius = from.getOrbitalRadiusInAU().value();
        const double toOrbitalRadius = to.getOrbitalRadiusInAU().value();

        return toOrbitalRadius + fromOrbitalRadius;
    }

    // Otherwise, translate to cartesian and use Pythagorean theorem.
    std::pair<double, double> fromCoords = OrbitalMath::polarToCartesianCoordinates(from, fromAngularPosition);
    std::pair<double, double> toCoords = OrbitalMath::polarToCartesianCoordinates(to, toAngularPosition);

    // distance = √( (x1-x2)² + (y1-y2)² )
    const double deltaX = toCoords.first - fromCoords.first;
    const double deltaY = toCoords.second - fromCoords.second;
    return std::sqrt( deltaX * deltaX  +  deltaY * deltaY );
}

OrbitalMath::TransferResults
OrbitalMath::computeTransfer(const Planet& from, const Planet& to, const Rocket& rocket, double totalDistance, bool realisticGravity) {
    const double fromEscapeVelocity = OrbitalMath::escapeVelocity(from);
    const double toEscapeVelocity = OrbitalMath::escapeVelocity(to);
    const double cruisingVelocity = std::max(fromEscapeVelocity, toEscapeVelocity);

    // TODO: if realistic gravity, include effect here
    const OrbitalMath::AccelerationResult launchAcceleration =
        OrbitalMath::computeAccelerationValues(rocket, cruisingVelocity);

    // TODO: if realistic gravity, include effect here
    const OrbitalMath::AccelerationResult landingDeceleration =
        OrbitalMath::computeAccelerationValues(rocket, 0.0, cruisingVelocity);

    const double cruisingDistance = totalDistance
                                    - launchAcceleration.accelerationDistance
                                    - landingDeceleration.accelerationDistance;
    const double cruisingTime = cruisingDistance / cruisingVelocity;

    const double totalTravelTime = cruisingTime
                                   + launchAcceleration.accelerationTime
                                   + landingDeceleration.accelerationTime;

    return {
        cruisingVelocity,
        launchAcceleration.accelerationTime,
        launchAcceleration.accelerationDistance,
        cruisingTime,
        landingDeceleration.accelerationTime,
        landingDeceleration.accelerationDistance,
        totalTravelTime
    };
}

OrbitalMath::TransferResults
OrbitalMath::computeSimpleTransfer(
    const Planet& from, const Planet& to, const Rocket& rocket,
    double fromAngularPosition, double toAngularPosition, double daysElapsed,
    bool realisticGravity
    ) {

    // Adjust angular positions if needed
    if (daysElapsed) {
        fromAngularPosition = OrbitalMath::angularPositionAfterDays(from, daysElapsed, fromAngularPosition);
        toAngularPosition = OrbitalMath::angularPositionAfterDays(to, daysElapsed, toAngularPosition);
    }

    // Compute distance between planet *surfaces*
    const double distanceBetweenCentres = DistanceUnit::convert(
        OrbitalMath::distance(from, fromAngularPosition, to, toAngularPosition),
        DistanceUnit::AU,
        DistanceUnit::METRES
        );
    const double distanceBetweenSurfaces = distanceBetweenCentres
                                           - from.getRadiusInMetres()
                                           - to.getRadiusInMetres();

    return computeTransfer(from, to, rocket, distanceBetweenSurfaces, realisticGravity);
}

OrbitalMath::TransferPlanning
OrbitalMath::planTransfer(
    const Planet& from, const Planet& to, const Rocket& rocket, const QList<Planet>& solarSystem,
    double timeframeStart, double timeframeEnd, bool systemStatic
    ) {

    // Step 1: compute initial angular positions of planets
    // Note: assuming angular positions of planets at time 0 as 0°
    const double daysToTimeframeStart = TimeUnit::convert(timeframeStart, TimeUnit::YEARS, TimeUnit::DAYS);
    const double fromStartAngularPosition = OrbitalMath::angularPositionAfterDays(from, daysToTimeframeStart, 0.0);
    const double toStartAngularPosition = OrbitalMath::angularPositionAfterDays(to, daysToTimeframeStart, 0.0);

    // Step 2: find closest-approach time with no restrictions; this is the theoretical best
    const double daysUntilAligned = OrbitalMath::daysUntilAligned(from, fromStartAngularPosition, to, toStartAngularPosition);
    const double theoreticalBest = timeframeStart + TimeUnit::convert(daysUntilAligned, TimeUnit::DAYS, TimeUnit::YEARS);

    // Step 3: find closest-approach times within timeframe; these are potential "ideal" candidates
    // TODO
    // likely steps:
    // - compute least common multiple of orbital periods
    // - skip forward from theoretical best one realignment period at a time until out of the timeframe

    // Step 4: take them one-by-one and check them for collisions; if any is safe, that's the practical best
    // TODO
    // likely steps:
    // - filter `solarSystem` for planets with orbitalRadius between `to` and `from`
    // - if none are found, first launch window is automatically good
    // - for each possible launch window:
    // - - for each planet in the filtered list:
    // - - - compute position at the given time
    // - - - compute distance between planet and the to-from line (if to-from line needs to be formulated, can do that only-once earlier)
    // - - - if distance is less than planet's radius, we have a collision; current launch window is invalid, skip
    // - - if no collisions found, launch window is good, break out of loop

    // Step 5: if none work (out of timeframe or collisions), take the first closest approach within the timeframe
    //         and try adjusting the launch earlier (earlier and later should work the same, and we prefer earlier;
    //         if earlier goes out of timeframe, though, try later) as needed until we dodge collisions
    // TODO
    // likely steps:
    // - hmm
    // considerations:
    // - if you're looking for "shortest travel time", you'd need to do some fancy analysis on the collisions to
    //   decide which one can be "fixed" with the smallest desync (= extra distance) between the planets
    // - if you don't care that much about travel time as long as it's not the optimal one, you're probably best
    //   off taking the first alignment time within the timeframe (maybe check the first one *before* the timeframe
    //   too?) an searching for a good solution around that one (e.g. a bit earlier / a bit later)
    // - for maximal mathematical rigor, you should compute the "collision window" of any given collision, then skip
    //   exactly that far earlier/later
}
