#include "ShiftScheduling.h"
#include "Testing/ShiftSchedulingTests.h"
#include "vector.h"
#include "error.h"
using namespace std;



/**
This function takes as input a HashSet of assigned shifts, then returns
the total value of this schedule.
 *
 * @param assignedShifts Assigned shifts so far.
 * @return val The value corresponding to this schedule.
 */
int computeValue(HashSet<Shift> assignedShifts) {
    int val = 0;
    for (Shift s: assignedShifts) {
        val += valueOf(s);
    }
    return val;
}

/**
This function takes as input a HashSet of assigned shifts, then returns
the total number of hours worked.
 *
 * @param assignedShifts Assigned shifts so far.
 * @return hours The number of hours worked.
 */
int computeHours(HashSet<Shift> assignedShifts) {
    int hours = 0;
    for (Shift s: assignedShifts) {
        hours += lengthOf(s);
    }
    return hours;
}

/**
This function takes as input a HashSet of assigned shifts and a candidate shift, then returns
whether this shift overlaps with one or more already assigned shifts.
 *
 * @param assignedShifts Assigned shifts so far.
 * @param candidateShift A shift that we could assign.
 * @return A boolean, whether there is an overlap (true) or not (false).
 */
bool overlaps(HashSet<Shift> assignedShifts, Shift candidateShift) {
    for (Shift s: assignedShifts) {
        if (overlapsWith(s, candidateShift)) {
            return true;
        }
    }
    return false;
}

/**
This recursive function takes as input a HashSet of possible shifts, a HashSet of assigned shifts,
along with the maximum number of hours the employee is allowed to work for, then returns a new set
of assigned shifts; either identical to previously assigned shifts (if no new shift was added, because
reaching the maximum number of hours, overlapping with an already assigned shift, or simply not optimal.
 *
 * @param shifts Potential shifts that could be assigned.
 * @param assignedShifts Assigned shifts so far.
 * @param maxHours The maximum number of hours that the employee is allowed to work.
 * @return New set of assigned shifts.
 */
HashSet<Shift> computeAssignedShifts(HashSet<Shift> shifts,
                                     HashSet<Shift> assignedShifts,
                                     int maxHours) {
    if (shifts.isEmpty()) {
        return assignedShifts;
    }

    else {
        Shift s = shifts.first();
        if (computeHours(assignedShifts + s) <= maxHours &&
                !overlaps(assignedShifts, s)) {

            HashSet<Shift> with = computeAssignedShifts(shifts - s, assignedShifts + s, maxHours);
            HashSet<Shift> without = computeAssignedShifts(shifts - s, assignedShifts, maxHours);

            if (computeValue(with) > computeValue(without)) {
                return with;
            } else {
                return without;
            }
        }

        return computeAssignedShifts(shifts - s, assignedShifts, maxHours);
    }
}

/**
This function takes as input a HashSet of all the possible shifts, along with the maximum number
of hours the employee is allowed to work for, then returns which shifts the employee should take
in order to generate the maximum amount of value.
 *
 * @param shifts All the potential shifts that could be assigned.
 * @param maxHours The maximum number of hours that the employee is allowed to work.
 * @return A schedule for the worker that maximizes the value brought in. This schedule cannot
 *         have overlapping shifts, nor can it exceed the maximum number of hours.
 */
HashSet<Shift> highestValueScheduleFor(const HashSet<Shift>& shifts, int maxHours) {

    if (maxHours < 0) {
        error("maxHours argument should be nonnegative.");
    }

    HashSet<Shift> assignedShifts = HashSet<Shift>();
    return computeAssignedShifts(shifts, assignedShifts, maxHours);

}



/* * * * * * Test Cases * * * * * */

HashSet<Shift> asSet(const Vector<Shift>& shifts) {
    HashSet<Shift> result;
    for (Shift s: shifts) {
        result += s;
    }
    return result;
}

ADD_TEST("Equally good overlapping shifts.") {
    Vector<Shift> shifts = {
        { Day::MONDAY, 9, 17, 1000 },  // Monday,  9AM - 5PM, value is 1000
        { Day::MONDAY, 8, 18, 1000 },  // Monday, 10AM - 6PM, value is 1000
    };

    EXPECT(highestValueScheduleFor(asSet(shifts), 100).size() == 1);
    EXPECT(highestValueScheduleFor(asSet(shifts), 100).contains(shifts[0]));
}

ADD_TEST("Long shift worth nothing vs. short shift worth a lot.") {
    Vector<Shift> shifts = {
        { Day::MONDAY, 9, 18,    1 },  // Unproductive monday
        { Day::SUNDAY, 8,  9, 1000 },
    };

    EXPECT(highestValueScheduleFor(asSet(shifts), 9).size() == 1);
    EXPECT(highestValueScheduleFor(asSet(shifts), 9).contains(shifts[1]));
}

/* * * * * * Test cases from the starter files below this point. * * * * * */

ADD_TEST("Provided Test: Pick a shift you do have time for.") {
    HashSet<Shift> shifts = {
        { Day::MONDAY, 9, 17, 1000 },  // Monday, 9AM - 5PM, value is 1000
    };

    EXPECT(highestValueScheduleFor(shifts, 24).size() == 1);
    EXPECT(highestValueScheduleFor(shifts, 24) == shifts);
}

ADD_TEST("Provided Test: Don't pick a shift you don't have time for.") {
    HashSet<Shift> shifts = {
        { Day::MONDAY, 9, 17, 1000 },  // Monday, 9AM - 5PM, value is 1000
    };

    EXPECT(highestValueScheduleFor(shifts, 3).isEmpty());
}

ADD_TEST("Provided Test: Don't pick overlapping shifts.") {
    Vector<Shift> shifts = {
        { Day::MONDAY, 9, 17, 1000 },  // Monday,  9AM - 5PM, value is 1000
        { Day::MONDAY, 8, 18, 2000 },  // Monday, 10AM - 6PM, value is 2000
    };

    EXPECT(highestValueScheduleFor(asSet(shifts), 100).size() == 1);
    EXPECT(highestValueScheduleFor(asSet(shifts), 100).contains(shifts[1]));
}

ADD_TEST("Provided Test: Passes the example from the handout.") {
    Vector<Shift> shifts = {
        { Day::MONDAY,     8, 12, 27 },  // Mon  8AM - 12PM, value 27 *
        { Day::MONDAY,    12, 16, 28 },  // Mon 12PM -  4PM, value 28 *
        { Day::MONDAY,    16, 20, 25 },  // Mon  4PM -  8PM, value 25 *
        { Day::MONDAY,     8, 14, 39 },  // Mon  8AM -  2PM, value 39
        { Day::MONDAY,    14, 20, 31 },  // Mon  2PM -  8PM, value 31
        { Day::TUESDAY,    8, 12,  7 },  // Tue  8AM - 12PM, value  7
        { Day::TUESDAY,   12, 16,  7 },  // Tue 12PM -  4PM, value  7
        { Day::TUESDAY,   16, 20, 11 },  // Tue  4PM -  8PM, value 11
        { Day::TUESDAY,    8, 14, 10 },  // Tue  8AM -  2PM, value 10
        { Day::TUESDAY,   14, 20,  8 },  // Tue  2PM -  8PM, value  8
        { Day::WEDNESDAY,  8, 12, 10 },  // Wed  8AM - 12PM, value 10
        { Day::WEDNESDAY, 12, 16, 11 },  // Wed 12PM -  4PM, value 11
        { Day::WEDNESDAY, 16, 20, 13 },  // Wed  4PM -  8PM, value 13
        { Day::WEDNESDAY,  8, 14, 19 },  // Wed  8AM -  2PM, value 19
        { Day::WEDNESDAY, 14, 20, 25 },  // Wed  2PM -  8PM, value 25 *
    };

    /* Get back the solution. */
    HashSet<Shift> computedSolution = highestValueScheduleFor(asSet(shifts), 20);

    /* Form the correct answer. It's the starred entries. */
    HashSet<Shift> actualSolution = {
        shifts[0], shifts[1], shifts[2], shifts[14]
    };

    EXPECT(computedSolution == actualSolution);
}
