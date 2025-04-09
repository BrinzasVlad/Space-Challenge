#ifndef ORBITALMATH_H
#define ORBITALMATH_H

#include "planet.h"
#include "rocket.h"

namespace OrbitalMath {

    /**
     * Computes the escape velocity for the given planet
     * in metres per second.
     */
    double escapeVelocity(const Planet& planet);

    /**
     * Computes the angular orbital velocity for the given
     * planet in degrees per day
     */
    double angularVelocity(const Planet& planet);

    /**
     * Computes the total acceleration for the given
     * rocket (assuming it fires all engines at maximum
     * output) in metres per second square.
     */
    double totalAcceleration(const Rocket& rocket);

    /**
     * A packed result storing the time (in seconds)
     * and distance (in metres) that it would take for
     * a rocket to accelerate (or decelerate) as part of
     * a maneuver.
     */
    struct AccelerationResult {
        /**
         * Time (in seconds) for the rocket to reach the target
         * acceleration.
         */
        double accelerationTime;

        /**
         * Distance (in metres) that the rocket travels until
         * reaching the target acceleration.
         */
        double accelerationDistance;
    };

    /**
     * Computes the time and distance for a rocket to
     * accelerate (or decelerate, under the assumption that
     * a rocket can decelerate exactly as well as it can
     * accelerate) up to (or down to) a target velocity.
     *
     * All calculations are done along the same direction / axis.
     *
     * @param rocket - the rocket for which to compute
     * @param targetVelocity - the velocity the rocket must achieve, in metres per second
     * @param initialVelocity - the initial velocity of the rocket, in metres per second (default 0)
     * @param ambientAcceleration - any ambient acceleration (or deceleration, if negative), e.g. from planet gravity, in metres per second square (default 0)
     */
    AccelerationResult computeAccelerationValues(
        const Rocket& rocket,
        double targetVelocity,
        double initialVelocity = 0,
        double ambientAcceleration = 0
        );

    /**
     * Computes the time, in days, until two planets are aligned
     * (i.e. their angular positions are identical), given their
     * initial angular positions (expressed as degrees of rotation
     * around the sun in their orbits)
     */
    double daysUntilAligned(const Planet& first, double firstAngularPosition, const Planet& second, double secondAngularPosition);

    /**
     * Computes the shortest distance, in AU, between two planets' surfaces.
     * Assumes that it is possible to wait until the planets align, so
     * that the distance between their surfaces is equal to the difference
     * between their orbital radii, minus their planetary radii.
     * If the planets cannot align (e.g. because there is a constant
     * angular position difference between them or because there is
     * a limited timeframe to work with), use `shortestDistanceConstrained` instead.
     */
    double shortestDistance(const Planet& from, const Planet& to);

    /**
     * Computes the shortest distance, in AU, between two planets' surfaces.
     * Takes into account their initial angular positions (expressed
     * as degrees of rotation around the sun in their orbits), as
     * well as a maximum timeframe (expressed in days) that can be
     * waited for in order for the planets to better align.
     */
    double shortestDistanceConstrained(
        const Planet& from,
        double fromAngularPosition,
        const Planet& to,
        double toAngularPosition,
        double maximumTimeframe
        );

    /**
     * Computes the new angular position (expressed in degrees of
     * rotation around the sun) of a planet after an amount of
     * time (given in days) has elapsed.
     */
    double angularPositionAfterDays(const Planet& planet, double daysElapsed, double initialAngularPosition = 0);

    /**
     * Converts a planet's position from a polar coordinate
     * system (given by its orbital radius and angular position
     * in its orbit given in degrees) to a cartesian coordinate
     * system with (0,0) origin at the sun, 0° corresponding to the
     * direction of the positive X axis (like in trigonometry), and
     * distances expressed in AU.
     *
     * E.g. if the Earth (orbital radius 1 AU) were at 150° in its
     * rotation around the sun, this function would return coordinates
     * (-0.866, 0.5).
     */
    std::pair<double, double> polarToCartesianCoordinates(const Planet& planet, double angularPosition);

    /**
     * Computes the distance, in AU, between the centres of two given planets.
     * Takes into account their current angular positions, expressed
     * as degrees of rotation around the sun in their orbits.
     */
    double distance(const Planet& from, double fromAngularPosition, const Planet& to, double toAngularPosition);

    /**
     * A packed result storing data about a transfer maneuver
     * from a start planet to a destination planet.
     * Contains information on:
     * - cruising velocity
     * - time and distance to accelerate from zero to cruising
     * velocity at the start planet
     * - time and distance to decelerate from cruising velocity
     * to zero at the destination planet
     * - time travelling at cruising velocity
     * - total travel time
     *
     * All distances are expressed in metres, all times are expressed
     * in seconds, cruising velocity is expressed in metres per second.
     */
    struct TransferResults {
        /**
         * Velocity (in metres per second) that the rocket travels at
         * between the initial acceleration and the final deceleration.
         */
        double cruisingVelocity;

        /**
         * Time (in seconds) for the rocket to reach cruising velocity
         * at the start of the transfer
         */
        double accelerationTime;

        /**
         * Distance (in metres) that the rocket travels until reaching
         * cruising velocity at the start of the transfer.
         */
        double accelerationDistance;

        /**
         * Total time (in seconds) that the rocket spends moving at
         * cruising velocity.
         */
        double cruisingTime;

        /**
         * Time (in seconds) for the rocket to decelerate from cruising
         * velocity to zero at the end of the transfer.
         */
        double decelerationTime;

        /**
         * Distance (in metres) that the rocket travels while decelerating
         * from cruising velocity to zero at the end of the transfer.
         */
        double decelerationDistance;

        /**
         * Total duration (in seconds) of the transfer, including acceleration,
         * cruising, and deceleration.
         */
        double totalTravelTime;
    };

    /**
     * Computes travel data (see `TransferResults`) for a
     * rocket transfer between two planets as follows:
     * - rocket launches from surface of first planet and
     * accelerates to cruising velocity
     * - cruising velocity is the greater of the escape
     * velocities of the two planets
     * - rocket decelerates when approaching second planet, such
     * as to land on its surface with velocity zero
     * - the `realisticGravity` flag currently does nothing
     * (Ideally, if realistic gravity was enabled then the
     * computation would include the effect of planets'
     * gravities on the acceleration and deceleration efforts.)
     *
     * @param from - the starting/source planet
     * @param to - the destination/target planet
     * @param rocket - the rocket used for the transfer
     * @param totalDistance - the distance the rocket must
     * travel, given in metres
     * @param realisticGravity - whether to use (more) realistic
     * gravity for acceleration calculations
     */
    TransferResults computeTransfer(
        const Planet& from,
        const Planet& to,
        const Rocket& rocket,
        double totalDistance,
        bool realisticGravity = false
        );

    /**
     * Computes travel data (see `computeTransfer` and `TransferResults`) for
     * a rocket transfer between two planets, assuming the distance
     * between their surfaces as the total travel distance.
     *
     * Optionally accepts initial angular positions (expressed as degrees
     * of rotation around the sun) for the planets and will compute distance
     * between them based on these. If `daysElapsed` is given, the angular
     * positions will be adjusted based on the elapsed time first.
     *
     * The `realisticGravity` flag currently does nothing. (Ideally, if
     * realistic gravity was enabled then the computation would include the
     * effects of planets' gravities on the acceleration and deceleration
     * efforts.)
     */
    TransferResults computeSimpleTransfer(
        const Planet& from,
        const Planet& to,
        const Rocket& rocket,
        double fromAngularPosition = 0,
        double toAngularPosition = 0,
        double daysElapsed = 0,
        bool realisticGravity = false
        );

    /**
     * A packed result storing the best time (expressed in years
     * from the starting situation) to launch a rocket from a
     * source planet to a destination one.
     *
     * Contains a theoretical best (first closest approach between
     * the planets, but might be outside the time limit or risk
     * collisions with planets in the way) and a practical best
     * (first launch window that gives the shortest total travel
     * time possible while avoiding collisions and fitting in the
     * timeframe restrictions).
     */
    struct TransferPlanning {
        /**
         * The best time (in years from the starting situation) to
         * execute the transfer, if we ignore any possible collisions
         * as well as any maximum waiting time for the transfer.
         * Equivalent to the soonest moment after the timeframe start
         * when the source and destination planets align.
         */
        double theoreticalBestStartTime;

        /**
         * Whether the theoretical best transfer time produces a valid
         * trip, i.e. avoids collisions with other planets and is
         * situated within the given timeframe.
         */
        bool isTheoreticalBestValid;

        /**
         * A short description detailing why the theoretical best is not
         * valid (e.g. "Collision with X" or "Out of timeframe"). If the
         * theoretical best is valid, unspecified.
         */
        QString reasonTheoreticalBestInvalid;

        /**
         * The best time (in years from the starting situation) to
         * execute the transfer while taking into account all practical
         * limitations (e.g. collisions, timeframe).
         *
         * If the theoretical best is valid, this will be equal to
         * it. Otherwise, it will represent:
         * 1. the first time when the planets are maximally aligned that
         * avoids collisions while still fitting in the timeframe, if any
         * exists
         * 2. if no such time exists, a time when the planets are as
         * closely aligned as possible within the timeframe, preferring
         * earlier times where possible
         */
        double practicalBestStartTime;
    };

    /**
     * Finds a theoretical and practical best time (see `TransferPlanning`) to
     * launch a rocket transfer from a starting planet to a destination
     * planet.
     *
     * The theoretical best will be the time of the first closest approach of
     * the two planets after `timeframeStart`.
     *
     * The practical best is the time between `timeframeStart` and `timeframeEnd` when
     * the transfer can be executed without collisions with other planets (as
     * specified in `solarSystem`) and the travel distance is the shortest it can
     * be within the timeframe. If there are multiple such moments in the timeframe, the
     * earliest one will be returned.
     *
     * If the `systemStatic` flag is enabled, the solar system will be considered
     * static during the rocket's travel. Otherwise, the computation will
     * assume that planets (including the source and the destination) continue
     * their movement during the rocket's travel.
     *
     * All times should be specified in years from the starting situation.
     * The computation assumes that at time 0, all planets are aligned (at
     * angular position 0°). A more generic solution may be implemented in a
     * later version.
     */
    TransferPlanning planTransfer(
        const Planet& from,
        const Planet& to,
        const Rocket& rocket,
        const QList<Planet>& solarSystem,
        double timeframeStart,
        double timeframeEnd,
        bool systemStatic = false
        );
}

#endif // ORBITALMATH_H
