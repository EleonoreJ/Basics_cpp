/* Given a list of patients and doctors, this program determines whether it is possible to schedule everyone. */

#include "DoctorsWithoutOrders.h"
#include "Testing/DoctorsWithoutOrdersTests.h"
using namespace std;



bool canAllPatientsBeSeen(const HashMap<string, int>& doctors,
                          const HashMap<string, int>& patients,
                          HashMap<string, HashSet<string>>& schedule) {
    /* Input: a group of doctors, a group of patients, a schedule (list of patients) of each doctor
     * Output: boolean saying whether it's possible to schedule all the patients so that each one is
     * seen by a doctor for the appropriate amount of time.
     * Goal: Determine wheter it's possible to assign all the patients to the doctors. */

    /* Base case */
    if (patients.isEmpty()) {
        return true;
    }

    /* Recursive case */
    else {
        string p = patients.front();
        int hoursNeeded = patients[p];

        for (string d: doctors) {

            if (doctors[d] >= hoursNeeded) {

                HashMap<string, int> updatedDoctors = doctors;
                updatedDoctors[d] = doctors[d] - hoursNeeded;

                HashMap<string, int> updatedPatients = patients;
                updatedPatients.remove(p);

                schedule[d] += p;

                if (canAllPatientsBeSeen(updatedDoctors, updatedPatients, schedule)) {
                    return true;
                }

                schedule[d] -= p;
            }
        }
    }

    return false;

}



/* * * * * * Test Cases Below This Point * * * * * */

ADD_TEST("Impossible example") {
    HashMap<string, HashSet<string>> schedule;

    HashMap<string, int> doctors = {
        { "Doctor Thomas", 10 },
        { "Doctor Taussig", 8 },
        { "Doctor Sacks",   8 },
        { "Doctor Ofri",    8 }
    };
    HashMap<string, int> patients = {
        { "Patient Lacks", 3 },
        { "Patient Gage", 3 },
        { "Patient Molaison", 4 },
        { "Patient Writebol", 3 },
        { "Patient St. Martin", 1 },
        { "Patient Washkansky", 6 },
        { "Patient Sandoval", 8 },
        { "Patient Giese", 6 },
    };

    EXPECT(!canAllPatientsBeSeen(doctors, patients, schedule));
}

/* * * * * Provided Tests Below This Point * * * * */

ADD_TEST("Provided Test: Can't schedule if a patient requires more hours than any doctor has.") {
    HashMap<string, HashSet<string>> schedule;

    HashMap<string, int> doctors = {
        { "Dr. Zhivago", 8 },
        { "Dr. Strange", 8 },
        { "Dr. Horrible", 8 },
    };
    HashMap<string, int> patients = {
        { "You Poor Soul", 9 },  // Needs more time than any one doctor can provide
    };

    EXPECT(!canAllPatientsBeSeen(doctors, patients, schedule));
}

ADD_TEST("Provided Test: Can schedule if doctor has way more time than patient needs.") {
    HashMap<string, HashSet<string>> schedule;

    HashMap<string, int> doctors = {
        { "Dr. Wheelock", 12 },
    };
    HashMap<string, int> patients = {
        { "Lucky Patient", 8 },
    };

    EXPECT(canAllPatientsBeSeen(doctors, patients, schedule));
}

ADD_TEST("Provided Test: Can schedule if there's one doctor and one patient with the same hours.") {
    HashMap<string, HashSet<string>> schedule;

    HashMap<string, int> doctors = {
        { "Dr. Wheelock", 8 },
    };
    HashMap<string, int> patients = {
        { "Lucky Patient", 8 },
    };

    EXPECT(canAllPatientsBeSeen(doctors, patients, schedule));
}

ADD_TEST("Provided Test: Schedule for one doctor and one patient is correct.") {
    HashMap<string, HashSet<string>> schedule;

    HashMap<string, int> doctors = {
        { "Dr. Wheelock", 8 },
    };
    HashMap<string, int> patients = {
        { "Lucky Patient", 8 },
    };

    EXPECT(canAllPatientsBeSeen(doctors, patients, schedule));

    HashMap<string, HashSet<string>> expectedSchedule = {
        { "Dr. Wheelock", { "Lucky Patient" } }
    };
    EXPECT_EQUAL(schedule, expectedSchedule);
}

ADD_TEST("Provided Test: Single doctor can see many patients.") {
    HashMap<string, HashSet<string>> schedule;

    HashMap<string, int> doctors = {
        { "Dr. House", 7 },
    };
    HashMap<string, int> patients = {
        { "Patient A", 4 },
        { "Patient B", 2 },
        { "Patient C", 1 }
    };

    EXPECT(canAllPatientsBeSeen(doctors, patients, schedule));

    HashMap<string, HashSet<string>> expectedSchedule = {
        { "Dr. House", { "Patient A", "Patient B", "Patient C" } }
    };
    EXPECT_EQUAL(schedule, expectedSchedule);
}

ADD_TEST("Provided Test: Multiple doctors can see multiple patients.") {
    HashMap<string, HashSet<string>> schedule;

    HashMap<string, int> doctors = {
        { "Dr. House", 7  },
        { "AutoDoc",   70 }
    };
    HashMap<string, int> patients = {
        { "Patient A", 4  },
        { "Patient B", 2  },
        { "Patient C", 1  },
        { "Patient D", 40 },
        { "Patient E", 20 },
        { "Patient F", 10 }
    };

    EXPECT(canAllPatientsBeSeen(doctors, patients, schedule));

    HashMap<string, HashSet<string>> expectedSchedule = {
        { "Dr. House", { "Patient A", "Patient B", "Patient C" } },
        { "AutoDoc",   { "Patient D", "Patient E", "Patient F" } },
    };
    EXPECT_EQUAL(schedule, expectedSchedule);
}

ADD_TEST("Provided Test: Doesn't necessarily assign neediest patient to most available doctor.") {
    HashMap<string, HashSet<string>> schedule;

    HashMap<string, int> doctors = {
        { "Doctor Workaholic", 10 },
        { "Doctor Average",    8 },
        { "Doctor Lazy",       6 },
    };
    HashMap<string, int> patients = {
        { "Patient EightHour", 8 },
        { "Patient SixHour",   6 },
        { "Patient FiveHour1", 5 },
        { "Patient FiveHour2", 5 }
    };

    /* You can't make this work if you assign Patient EightHour to Doctor Workaholic.
     * The only way for this setup to work is if you give the two five-hour patients
     * to Doctor Workaholic.
     */
    HashMap<string, HashSet<string>> expectedSchedule = {
        { "Doctor Workaholic", { "Patient FiveHour1", "Patient FiveHour2" } },
        { "Doctor Average",    { "Patient EightHour" } },
        { "Doctor Lazy",       { "Patient SixHour" } }
    };

    EXPECT(canAllPatientsBeSeen(doctors, patients, schedule));
    EXPECT_EQUAL(schedule, expectedSchedule);
}

ADD_TEST("Provided Test: Can't schedule patients if there are no doctors.") {
    HashMap<string, int> patients = {
        { "You Poor Soul", 8 },
    };
    HashMap<string, HashSet<string>> schedule;
    EXPECT(!canAllPatientsBeSeen({}, patients, schedule));
}

ADD_TEST("Provided Test: Agreement in total hours doesn't mean a schedule exists (1).") {
    HashMap<string, int> doctors = {
        { "Doctor A", 3 },
        { "Doctor B", 3 },
    };
    HashMap<string, int> patients = {
        { "Patient X", 2 },
        { "Patient Y", 2 },
        { "Patient Z", 2 },
    };

    /* Notice the the total hours free (6) matches the total hours needed (6), but it's set
     * up in a way where things don't align properly.
     */
    HashMap<string, HashSet<string>> schedule;
    EXPECT(!canAllPatientsBeSeen(doctors, patients, schedule));
}

ADD_TEST("Provided Test: Agreement in total hours doesn't mean a schedule exists (2).") {
    HashMap<string, int> doctors = {
        { "Doctor A", 8 },
        { "Doctor B", 8 },
        { "Doctor C", 8 },
    };
    HashMap<string, int> patients = {
        { "Patient U", 5 },
        { "Patient V", 5 },
        { "Patient W", 5 },
        { "Patient X", 4 },
        { "Patient Y", 3 },
        { "Patient Z", 2 },
    };

    /* Notice the the total hours free (24) matches the total number of hours needed
     * (24), but the way those hours are divvied up makes things impossible. Specifically,
     * no doctor can see two of the patients who each need five hours, so they need to be
     * spread around the three doctors evenly, but then there isn't enough time for
     * anyone to see the patient who needs four hours.
     */
    HashMap<string, HashSet<string>> schedule;
    EXPECT(!canAllPatientsBeSeen(doctors, patients, schedule));
}