#include "HumanPyramids.h"
#include "Testing/HumanPyramidTests.h"
#include "hashmap.h"
#include <iostream>
#include <vector>

using namespace std;


double weightRec(int row, int col, int pyramidHeight, HashMap<Vector<int>,double>& table) {
    /* Input: the position the person of interest (row,column), the size of the pyramid, the table for memoization
     * Output: the weight carried by the person
     * Goal: calculate by recursion the weight supported by everybody in the pyramid.*/

    Vector<int> vec = {row,col};

    /* Base case: the highest person in the pyramid.*/
    if (row == 0 && col == 0){
        table[vec] = 0;
        return 0 ;
    }

    /* avoiding impossible positions*/
    if (row<0){
        error("row non positive");
    }
    if (col<0){
        error("column non positive");
    }

    /* avoid cases outside the pyramid*/
    if (col > pyramidHeight){
        error("cannot return this weight");
    }
    if (row > pyramidHeight){
        error("cannot return this weight");
    }

    /* Memoization use*/
    if (table.containsKey(vec)){
        return table[vec];
    }

    else{

        /* weight for somebody on the right side*/
        if (col > row-1){
            table[vec] = weightRec(row-1,col-1,pyramidHeight-1, table)/2 + 80;
            return weightRec(row-1,col-1,pyramidHeight-1, table)/2 + 80;
        }

        /* weight for somebody on the left side*/
        if (col <= 0){
            table[vec] = weightRec(row-1,col,pyramidHeight-1, table)/2 + 80;
            return weightRec(row-1,col,pyramidHeight-1, table)/2 + 80;
        }

        /* weight calculation*/
        else{
            table[vec] = weightRec(row-1,col,pyramidHeight-1, table)/2 + weightRec(row-1,col-1,pyramidHeight-1, table)/2 + 160;
            return weightRec(row-1,col,pyramidHeight-1, table)/2 + weightRec(row-1,col-1,pyramidHeight-1, table)/2 + 160;
        }
    }
}



double weightOnBackOf(int row, int col, int pyramidHeight) {
    /* * Input: the position the person of interest (row,column), the size of the pyramid
     * Output: the weight carried by the person
     * Goal: add table for memoization*/
    HashMap<Vector<int>,double> table;
    return weightRec(row, col, pyramidHeight,table);
}



/* * * * * * Test Cases * * * * * */



ADD_TEST("Provided Test: Check Person very high in the pyramid") {
    /* Person E is located at row 1, column 0. */
    EXPECT_EQUAL(weightOnBackOf(1, 0, 3), 80);
}


ADD_TEST("Provided Test: Check the highest person in the pyramid") {
    /* Person E is located at row 1, column 0. */
    EXPECT_EQUAL(weightOnBackOf(0, 0, 3), 0);
}

ADD_TEST("Provided Test: Function returns a value, even for someone deep in the pyramid but on the side.") {
    EXPECT(weightOnBackOf(100, 100, 200) < 1000);
}






/* * * * * * Test cases from the starter files below this point. * * * * * */

ADD_TEST("Provided Test: Check Person E from the handout.") {
    /* Person E is located at row 2, column 1. */
    EXPECT_EQUAL(weightOnBackOf(2, 1, 5), 240);
}

ADD_TEST("Provided Test: Function reports errors in invalid cases.") {
    EXPECT_ERROR(weightOnBackOf(-1, 0, 10));
    EXPECT_ERROR(weightOnBackOf(10, 10, 5));
    EXPECT_ERROR(weightOnBackOf(-1, 10, 20));
}

ADD_TEST("Provided Test: Warning! The next test will hang if you haven't implemented memoization.") {
    // Nothing to see here, folks. Move along...
}

ADD_TEST("Provided Test: Function returns a value, even for someone deep in the pyramid.") {
    /* This will take a LONG time to complete if memoization isn't implemented.
     * We're talking "heat death of the universe" amounts of time. :-)
     */
    EXPECT(weightOnBackOf(100, 50, 200) >= 10000);
}


