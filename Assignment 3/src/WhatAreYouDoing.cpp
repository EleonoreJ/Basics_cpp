/* In this program, we find all the ways of capitalizing the words in a sentence using recursion. */
#include "WhatAreYouDoing.h"
#include "Testing/WhatAreYouDoingTests.h"
#include "strlib.h"
#include <ctype.h>

using namespace std;



void EmphaseRec(Vector<string> vec_init, Vector<string> vec_final, HashSet<string>& set){
    /* Input: vec_init(remaining elements) and vec_final(chosen elements), set of sentences completed
     * Output: Nothing
     * Goal: Fulfill the hashset set with all the possibilities of WhatAreYouDoing */

    /* Base case: no more remaining elements */
    if (vec_init.size() == 0){

        /* we transform the vector into a string and add it to the set */
        string phrase;
        for (string item: vec_final){
            phrase = phrase + item;
        }
        set.add(phrase);
    }

    else{
        string s = vec_init[0];

        /* If it a the first elem of the word is a letter */
        if (isalpha(s[0])){
            vec_init = vec_init.subList(1);

            /* New branch with the word in uppercase */
            Vector<string> vec1 = vec_final;
            vec1.add(toUpperCase(s));
            EmphaseRec(vec_init,vec1,set);

            /* New branch with the word in lowercase */
            Vector<string> vec2 = vec_final;
            vec2.add(toLowerCase(s));
            EmphaseRec(vec_init,vec2,set);
        }

        /* if it is not a word, move on*/
        else{
            vec_final.add(s);
            vec_init = vec_init.subList(1);
            EmphaseRec(vec_init,vec_final,set);
        }
    }
}



HashSet<string> allEmphasesOf(const string& sentence) {
    /* Input: the sentence to transform
     * Output: set with all the possibilities of WhatAreYouDoing
     * Goal: calling the recursive function EmphaseRec */

    Vector<string> vec_init = tokenize(sentence);
    Vector<string> vec_final;
    HashSet<string> set;
    EmphaseRec(vec_init, vec_final, set);
    return set;
}



/* * * * * * Test Cases * * * * * */

ADD_TEST("Very long sentence") {
    EXPECT(allEmphasesOf("Hello, our names are Robin and Eleonore").size()>50);
}

ADD_TEST("Very short sentence") {
    EXPECT_EQUAL(allEmphasesOf("A").size(), 2);
}

/* * * * * * Test cases from the starter files below this point. * * * * * */

ADD_TEST("Provided Test: Enumerates all options in a simple case.") {
    HashSet<string> expected = {
        "hello",
        "HELLO",
    };

    EXPECT_EQUAL(allEmphasesOf("Hello"), expected);
}

ADD_TEST("Provided Test: Each option has the right length.") {
    string sentence = "Hello, world!";
    for (string option: allEmphasesOf(sentence)) {
        EXPECT_EQUAL(option.length(), sentence.length());
    }
}

ADD_TEST("Provided Test: Enumerates all options in a more typical case.") {
    HashSet<string> expected = {
        "you are?",
        "you ARE?",
        "YOU are?",
        "YOU ARE?"
    };

    EXPECT_EQUAL(allEmphasesOf("You Are?"), expected);
}

ADD_TEST("Provided Test: Warning! This next test will hang if you produce duplicate capitalizations.") {
    // Nothing to see here, folks. Move along!
}

ADD_TEST("Provided Test: Recursion doesn't branch on non-word symbols.") {
    string punctuation(50, '*'); // 50 copies of *

    /* The only emphasis is itself. */
    HashSet<string> expected = {
        punctuation
    };

    EXPECT_EQUAL(allEmphasesOf(punctuation), expected);
}
