#include "TopK.h"
#include "Testing/TopKTests.h"
#include "HeapPQueue.h"
using namespace std;


/**
 * Given a stream containing some number of DataPoints, returns the k elements from that
 * data stream that have the highest weight, sorted in descending order of weight.
 * If the number k is higher than the number of elements in the data stream, returns all 
 * the elements of the data stream, sorted in descending order by weight.
 * 
 * @param stream A data stream containing a bunch of DataPoints.
 * @param k The number of elements to read.
 * @return The min{n, k} data points of the stream with the highest weight, sorted in descending
 *         order of weight, where n is the number of items in the stream.
 */
Vector<DataPoint> topK(istream& stream, int k) {
    HeapPQueue pq;

    for (DataPoint pt; stream >> pt; ) {
        pq.enqueue(pt);
    }

    Vector<DataPoint> topKElements;

    if (pq.size() == 0) {
        return topKElements;
    }

    int max;
    int min;
    if (pq.size() > k) {
        max = pq.size();
        min = k;
    }
    else {
        max = k;
        min = pq.size();
    }

    for (int i = 0; i < max-min; i++) {
        if (pq.size() == 1) {
            break;
        }
        pq.dequeue();
    }
    for (int j = 0; j < min; j++) {
        topKElements += pq.dequeue();
    }

    int start = 0;
    int end = topKElements.size() - 1;
    while (start < end)
    {
        DataPoint tmp = topKElements[start];
        topKElements[start] = topKElements[end];
        topKElements[end] = tmp;
        start ++;
        end --;
    }

    return topKElements;
}



/* * * * * * Test Cases Below This Point * * * * * */

/* Helper function that, given a list of data points, produces a stream from them. */
stringstream asStream(const Vector<DataPoint>& dataPoints) {
    stringstream result;
    for (const auto& pt: dataPoints) {
        result << pt;
    }
    return result;
}

/* TODO: Add your own custom tests here! */

ADD_TEST("Stream many elements, ask for top many") {
    Vector<DataPoint> vec;
    for (int i = 0; i < 100; i++) vec.add({ "", i });
    auto stream = asStream(vec);
    int start = 0;
    int end = vec.size() - 1;
    while (start < end) {
        DataPoint tmp = vec[start];
        vec[start] = vec[end];
        vec[end] = tmp;
        start ++;
        end --;
    }
    Vector<DataPoint> expected = vec;
    EXPECT_EQUAL(topK(stream, 100), expected);
}

/* * * * * Provided Tests Below This Point * * * * */

/* Constant used for sizing the tests below this point. */
const int kMany = 100000;

ADD_TEST("Provided Test: Stream 0 elements, ask for top 0") {
    auto stream = asStream({});
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(stream, 0), expected);
}

ADD_TEST("Provided Test: Stream 0 elements, ask for top 1") {
    auto stream = asStream({});
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(stream, 1), expected);
}

ADD_TEST("Provided Test: Stream 0 elements, ask for top many") {
    auto stream = asStream({});
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(stream, kMany), expected);
}

ADD_TEST("Provided Test: Stream 1 element, ask for top 0") {
    auto stream = asStream({ { "A" , 1 } });
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(stream, 0), expected);
}

ADD_TEST("Provided Test: Stream 1 element, ask for top 1") {
    auto stream = asStream({ { "A" , 1 } });
    Vector<DataPoint> expected = { { "A" , 1 } };
    EXPECT_EQUAL(topK(stream, 1), expected);
}

ADD_TEST("Provided Test: Stream 1 element, ask for top many") {
    auto stream = asStream({ { "A" , 1 } });
    Vector<DataPoint> expected = { { "A" , 1 } };
    EXPECT_EQUAL(topK(stream, kMany), expected);
}

ADD_TEST("Provided Test: Works in a simple case.") {
    /* Build a stream with some simple elements in it. */
    auto stream = asStream({
        { "A", 1 }, { "B", 2 }, { "C", 3 }, { "D", 4 }
    });

    /* What we should see. */
    Vector<DataPoint> expected = {
        { "D", 4 }, { "C", 3 }, { "B", 2 }
    };

    EXPECT(topK(stream, 3) == expected);
}

ADD_TEST("Provided Test: Stream contains duplicate elements") {
    Vector<DataPoint> vec = {
        { "" , 1 },
        { "" , 3 },
        { "" , 2 },
        { "" , 1 },
        { "" , 5 },
        { "" , 4 },
        { "" , 2 },
        { "" , 3 },
        { "" , 4 },
        { "" , 5 },
    };
    auto stream = asStream(vec);
    Vector<DataPoint> expected = { vec[4] };
    EXPECT_EQUAL(topK(stream, 1), expected);

    stream = asStream(vec);
    expected = { vec[4], vec[4] };
    EXPECT_EQUAL(topK(stream, 2), expected);

    stream = asStream(vec);
    expected = { vec[4], vec[4], vec[5] };
    EXPECT_EQUAL(topK(stream, 3), expected);

    stream = asStream(vec);
    expected = { vec[4], vec[4], vec[5], vec[5] };
    EXPECT_EQUAL(topK(stream, 4), expected);

    stream = asStream(vec);
    expected = { vec[4], vec[4], vec[5], vec[5], vec[1] };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

ADD_TEST("Provided Test: Stream contains negative elements") {
    Vector<DataPoint> vec = {
        { "" , 1 },
        { "" , 3 },
        { "" , 2 },
        { "" , -1 },
        { "" , -5 },
        { "" , 4 },
        { "" , -2 },
        { "" , 3 },
        { "" , -4 },
        { "" , 5 },
    };
    auto stream = asStream(vec);
    Vector<DataPoint> expected = { vec[9] };
    EXPECT_EQUAL(topK(stream, 1), expected);

    stream = asStream(vec);
    expected = { vec[9], vec[5] };
    EXPECT_EQUAL(topK(stream, 2), expected);

    stream = asStream(vec);
    expected = { vec[9], vec[5], vec[1] };
    EXPECT_EQUAL(topK(stream, 3), expected);

    stream = asStream(vec);
    expected = { vec[9], vec[5], vec[1], vec[1] };
    EXPECT_EQUAL(topK(stream, 4), expected);

    stream = asStream(vec);
    expected = { vec[9], vec[5], vec[1], vec[1], vec[2] };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

ADD_TEST("Provided Test: Stream many elements, ask for top 0") {
    Vector<DataPoint> vec;
    for (int i = 0; i < kMany; i++) vec.add({ "", i });
    auto stream = asStream(vec);
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(stream, 0), expected);
}

ADD_TEST("Provided Test: Stream many elements, ask for top 1") {
    Vector<DataPoint> vec;
    for (int i = 0; i < kMany; i++) vec.add({ "", i });
    auto stream = asStream(vec);
    Vector<DataPoint> expected = { { "" , kMany - 1 } };
    EXPECT_EQUAL(topK(stream, 1), expected);
}

ADD_TEST("Provided Test: Stream many elements, ask for top 5") {
    Vector<DataPoint> vec;
    for (int i = 0; i < kMany; i++) vec.add({ "", i });
    auto stream = asStream(vec);
    Vector<DataPoint> expected = {
        { "" , kMany - 1 },
        { "" , kMany - 2 },
        { "" , kMany - 3 },
        { "" , kMany - 4 },
        { "" , kMany - 5 }
    };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

ADD_TEST("Provided Test: Stress test") {
    Vector<int> sorted;
    Vector<DataPoint> points;
    for (int i = 0; i < 10000; i++) {
        int weight = randomInteger(0, 100000);
        sorted.add(weight);
        points.add({ "" , weight});
    }

    auto stream = asStream(points);
    sort(sorted.begin(), sorted.end(), greater<int>());
    auto result = topK(stream, 10);

    EXPECT_EQUAL(result.size(), 10);
    for (int i = 0; i < 10; i++) {
        EXPECT_EQUAL(result[i].weight, sorted[i]);
    }
}
