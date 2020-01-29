#include "YouGotHufflepuff.h"
#include "Testing/YouGotHufflepuffTests.h"
#include <algorithm>
using namespace std;


Vector<char> letters = {'O', 'C', 'E', 'A', 'N'};

/* This function converts the integer [1; 5], where 1 means
"strongly disagree" and 5 "strongly agree", to a score [-2; 2].*/
int getScore(int answer) {
    return answer - 3;
}

/* This function takes as input a set of possible personality questions,
a number of questions to ask in the quiz, and a set of people. It administers
a personality quiz and determines which person of the set of people is most
similar to the user.*/
void administerQuiz(const HashSet<Question>& questions,
                    int numQuestions,
                    const HashSet<Person>& people) {

    HashMap<char, int> userScores = HashMap<char, int>();
    Vector<Question> asked = Vector<Question>();

    while (asked.size() < numQuestions) {
        Question q = randomElement(questions);
        if (find(asked.begin(), asked.end(), q) != asked.end()) {
            continue; // do not ask the same question twice
        } else {
            asked.add(q);
            int answer = askPersonalityQuestion(q);
            for (char l: letters) {
                if (q.factors.containsKey(l)) {
                    int oldScore = userScores.get(l);
                    int newScore = oldScore + q.factors.get(l)*getScore(answer);
                    userScores.put(l, newScore);
                }
            }
        }
    }

    displayScores(userScores);

    // handle edge case where all scores are zeros
    bool flag = true;
    for (auto aspect: userScores) {
        if (userScores[aspect] != 0) {
            flag = false;
        }
    }
    if (flag == true) {
        displayMessage("User's scores in all categories are zeros.");
    }

    double normUser = 0.0;
    for (auto aspect: userScores) {
        normUser += userScores[aspect]*userScores[aspect];
    }
    normUser = sqrt(normUser);
    HashMap<char, double> normalizedUserScores = HashMap<char, double>();
    for (auto aspect: userScores) {
        normalizedUserScores.put(aspect, userScores[aspect]/normUser);
    }

    double maxSimilarity = -8000;
    string closest = "none";
    for (auto person: people) {
        double normOther = 0.0;
        for (auto aspect: person.scores) {
            normOther += person.scores[aspect]*person.scores[aspect];
        }
        normOther = sqrt(normOther);
        HashMap<char, double> normalizedOtherScores = HashMap<char, double>();
        for (auto aspect: person.scores) {
            normalizedOtherScores.put(aspect, person.scores[aspect]/normOther);
        }

        double similarity = 0.0;
        for (auto aspect: person.scores) {
            similarity += normalizedUserScores[aspect]*normalizedOtherScores[aspect];
        }

        if (similarity > maxSimilarity) {
            maxSimilarity = similarity;
            closest = person.name;
        }
    }

    displayMessage("You Got Hufflepuff! " + closest + " (Similarity: " + to_string(maxSimilarity) + ")");
}


/* * * * *   Test Cases Below This Point   * * * * */

/* Because administerQuiz is a function that requires interaction with the user,
 * you won't be able to directly test that administerQuiz returns the correct
 * values. Similarly, you can't call the askPersonalityQuestion, displayScores,
 * or displayMessage functions, as those are wired into the demo and don't
 * work in testing.
 *
 * However, you are welcome to add test cases to test any helper functions you
 * write! To do so, use the ADD_TEST macro to define tests, just as you did in
 * Assignment 1 and for Rising Tides.
 *
 * You aren't required to include test cases in this part of the assignment,
 * though we definitely would encourage you to do so! This is a multistep
 * assignment that's going to require each individual function to work
 * correctly, and adding tests for the functions you write is a great way to
 * find and fix bugs early on.
 */

