#include "HeapPQueue.h"
#include "Testing/HeapTests.h"
#include "vector.h"
using namespace std;


/* Constant controlling the default size of our HeapPQueue. */
const int kInitialSize = 4;

/**
 * Constructor. Creates a new, empty priority queue.
 */
HeapPQueue::HeapPQueue() {
    logicalSize = 0;
    allocatedSize = kInitialSize;
    elems = new DataPoint[allocatedSize];
}

/**
 * Destructor. Cleans up all memory allocated by this priority queue.
 */
HeapPQueue::~HeapPQueue() {
    delete[] elems;
}

/**
 * Adds a new data point into the queue. This operation runs in time O(log n),
 * where n is the number of elements in the queue.
 * @param data The data point to add.
 */
void HeapPQueue::enqueue(const DataPoint& data) {
    if (allocatedSize == logicalSize) {
        grow();
    }

    elems[logicalSize] = data;
    logicalSize ++;

    // Bubble-up
    int currIndex = logicalSize - 1;
    while (currIndex != 0) {
        int pIndex = (currIndex - 1)/2;

        if (elems[currIndex].weight < elems[pIndex].weight) {
            swap(currIndex, pIndex);
        }
        currIndex = pIndex;
    }
}

/**
 * Returns the number of data points in this priority queue.
 * @return The number of elements in the priority queue.
 */
int HeapPQueue::size() const {
    return logicalSize;
}

/**
 * Returns, but does not remove, the element that would next be removed via a call to
 * dequeue.
 * @return The lowest-weight data point in the queue.
 */
DataPoint HeapPQueue::peek() const {
    if (isEmpty()) {
        error("What is the sound of one hand clapping?");
    }

    return elems[0];
}

/**
 * Removes and returns the lowest-weight data point in the priority queue.
 * @return The lowest-weight data point in the queue.
 */
DataPoint HeapPQueue::dequeue() {
    if (isEmpty()) {
        error("What is the sound of one hand clapping?");
    }

    DataPoint first = elems[0];
    elems[0] = elems[logicalSize-1];
    logicalSize --;

    // Bubble-down
    int currIndex = 0;
    while(true) {
        int lChildIndex = 2*currIndex + 1;
        int rChildIndex = 2*(currIndex + 1);
        int minIndex = currIndex;

        if (lChildIndex >= logicalSize) {
            break;
        }

        if (elems[minIndex].weight > elems[lChildIndex].weight) {
            minIndex = lChildIndex;
        }

        if (rChildIndex < logicalSize &&
                elems[minIndex].weight > elems[rChildIndex].weight) {
            minIndex = rChildIndex;
        }

        if (minIndex != currIndex) {
            swap(currIndex, minIndex);
            currIndex = minIndex;
        }

        else {
            break;
        }
    }

    return first;
}

/**
 * Returns whether the priority queue is empty.
 * @return Whether the priority queue is empty.
 */
bool HeapPQueue::isEmpty() const {
    return size() == 0;
}

/* Debug function.
 */
void HeapPQueue::printDebugInfo() {
    /* . */
}

/**
 * Increases allocated memory for the priority queue.
 */
void HeapPQueue::grow() {
    allocatedSize *= 2;

    DataPoint* newElems = new DataPoint[allocatedSize];

    for (int i = 0; i < logicalSize; i++) {
        newElems[i] = elems[i];
    }

    delete[] elems;
    elems = newElems;
}

/**
 * Swaps two elements of the binary heap.
 */
void HeapPQueue::swap(int& i, int& j) {
    DataPoint tmp = elems[i];
    elems[i] = elems[j];
    elems[j] = tmp;
}


/* * * * * * Test Cases Below This Point * * * * * */

ADD_TEST("Custom case test.") {
    HeapPQueue pq;

    pq.enqueue({ "R", 4 });
    pq.enqueue({ "A", 5 });
    pq.enqueue({ "B", 3 });
    pq.enqueue({ "K", 7 });
    pq.enqueue({ "G", 2 });
    pq.enqueue({ "V", 9 });
    pq.enqueue({ "T", 1 });
    pq.enqueue({ "O", 8 });
    pq.enqueue({ "S", 6 });

    EXPECT_EQUAL(pq.size(), 9);
    DataPoint expected = { "T", 1 };
    EXPECT_EQUAL(pq.peek(), expected);
    auto removed = pq.dequeue();
    EXPECT_EQUAL(removed, expected);
    EXPECT_EQUAL(pq.size(), 8);
    DataPoint nextExpected = { "G", 2 };
    EXPECT_EQUAL(pq.peek(), nextExpected);
    auto nextRemoved = pq.dequeue();
    EXPECT_EQUAL(nextRemoved, nextExpected);
    EXPECT_EQUAL(pq.size(), 7);
}

/* * * * * Provided Tests Below This Point * * * * */

ADD_TEST("Provided Test: Newly-created heap is empty.") {
    HeapPQueue pq;

    EXPECT(pq.isEmpty());
    EXPECT(pq.size() == 0);
}

ADD_TEST("Provided Test: Enqueue / dequeue single element") {
    HeapPQueue pq;
    DataPoint point = { "enqueue me!", 4 };
    pq.enqueue(point);
    EXPECT_EQUAL(pq.size(), 1);
    EXPECT_EQUAL(pq.isEmpty(), false);

    EXPECT_EQUAL(pq.dequeue(), point);
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);

    pq.enqueue(point);
    EXPECT_EQUAL(pq.size(), 1);
    EXPECT_EQUAL(pq.isEmpty(), false);

    EXPECT_EQUAL(pq.dequeue(), point);
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

ADD_TEST("Provided Test: Dequeue / peek on empty heap throws error") {
    HeapPQueue pq;

    EXPECT(pq.isEmpty());
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());
}

ADD_TEST("Provided Test: Enqueue elements in sorted order.") {
    HeapPQueue pq;
    for (int i = 0; i < 10; i++) {
        pq.enqueue({ "elem" + to_string(i), i });
    }

    EXPECT_EQUAL(pq.size(), 10);
    for (int i = 0; i < 10; i++) {
        auto removed = pq.dequeue();
        DataPoint expected = {
            "elem" + to_string(i), i
        };
        EXPECT_EQUAL(removed, expected);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

ADD_TEST("Provided Test: Enqueue elements in reverse-sorted order.") {
    HeapPQueue pq;
    for (int i = 10; i >= 0; i--) {
        pq.enqueue({ "elem" + to_string(i), i });
    }

    EXPECT_EQUAL(pq.size(), 11);
    for (int i = 0; i <= 10; i++) {
        auto removed = pq.dequeue();
        DataPoint expected = {
            "elem" + to_string(i), i
        };
        EXPECT_EQUAL(removed, expected);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

ADD_TEST("Provided Test: Insert ascending and descending sequences.") {
    HeapPQueue pq;
    for (int i = 0; i < 20; i++) {
        pq.enqueue({ "a" + to_string(i), 2 * i });
    }
    for (int i = 19; i >= 0; i--) {
        pq.enqueue({ "b" + to_string(i), 2 * i + 1 });
    }

    EXPECT_EQUAL(pq.size(), 40);
    for (int i = 0; i < 40; i++) {
        auto removed = pq.dequeue();
        EXPECT_EQUAL(removed.weight, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

ADD_TEST("Provided Test: Insert random permutation.") {
    Vector<DataPoint> sequence = {
        { "A", 0 },
        { "D", 3 },
        { "F", 5 },
        { "G", 6 },
        { "C", 2 },
        { "H", 7 },
        { "I", 8 },
        { "B", 1 },
        { "E", 4 },
        { "J", 9 },
    };

    HeapPQueue pq;
    for (DataPoint elem: sequence) {
        pq.enqueue(elem);
    }

    EXPECT_EQUAL(pq.size(), sequence.size());

    for (int i = 0; i < 10; i++) {
        auto removed = pq.dequeue();
        DataPoint expected = {
            string(1, 'A' + i), i
        };
        EXPECT_EQUAL(removed, expected);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

ADD_TEST("Provided Test: Insert duplicate elements.") {
    HeapPQueue pq;
    for (int i = 0; i < 20; i++) {
        pq.enqueue({ "a" + to_string(i), i });
    }
    for (int i = 19; i >= 0; i--) {
        pq.enqueue({ "b" + to_string(i), i });
    }

    EXPECT_EQUAL(pq.size(), 40);
    for (int i = 0; i < 20; i++) {
        auto one = pq.dequeue();
        auto two = pq.dequeue();

        EXPECT_EQUAL(one.weight, i);
        EXPECT_EQUAL(two.weight, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

ADD_TEST("Provided Test: Handles data points with empty string name.") {
    HeapPQueue pq;
    for (int i = 0; i < 10; i++) {
        pq.enqueue({ "" , i });
    }
    EXPECT_EQUAL(pq.size(), 10);
}

ADD_TEST("Provided Test: Handles data points with negative weights.") {
    HeapPQueue pq;
    for (int i = -10; i < 10; i++) {
        pq.enqueue({ "" , i });
    }
    EXPECT_EQUAL(pq.size(), 20);
    for (int i = -10; i < 10; i++) {
        EXPECT_EQUAL(pq.dequeue().weight, i);
    }
}

ADD_TEST("Provided Test: Interleave enqueues and dequeues.") {
    HeapPQueue pq;
    int n = 100;
    for (int i = n / 2; i < n; i++) {
        pq.enqueue({"", i});
    }
    EXPECT_EQUAL(pq.size(), 50);
    for (int i = n / 2; i < n; i++) {
        EXPECT_EQUAL(pq.dequeue().weight, i);
    }
    EXPECT_EQUAL(pq.size(), 0);

    for (int i = 0; i < n / 2; i++) {
        pq.enqueue({"", i});
    }
    EXPECT_EQUAL(pq.size(), 50);
    for (int i = 0; i < n / 2; i++) {
        EXPECT_EQUAL(pq.dequeue().weight, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
}

ADD_TEST("Provided Test: Stress test: cycle 250,000 elems (should take at most a few seconds)") {
    HeapPQueue pq;
    int n = 250000;
    for (int i = 0; i < n; i++) {
        pq.enqueue({ "", randomInteger(0, 100000) });
    }
    EXPECT_EQUAL(pq.size(), n);

    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);

    for (int i = 0; i < n; i++) {
        pq.enqueue({ "", randomInteger(0, 100000) });
    }
    EXPECT_EQUAL(pq.size(), n);
}


ADD_TEST("Provided Test: Simple memory leak check.") {
    /* See how many unmatched allocations of DataPoint objects there are
     * before we perform any operations.
     */
    int allocationsBefore = netDataPointAllocations();

    HeapPQueue* pq = new HeapPQueue();
    delete pq;

    /* Net allocations now - net allocations at start = allocations leaked by
     * the HeapPQueue implementation. If this number is not zero, it might
     * indicate a memory leak.
     */
    int netAllocations = netDataPointAllocations() - allocationsBefore;
    EXPECT_EQUAL(netAllocations, 0);
}

ADD_TEST("Provided Test: More aggressive memory leak check.") {
    /* See how many unmatched allocations of DataPoint objects there are
     * before we perform any operations.
     */
    int allocationsBefore = netDataPointAllocations();

    HeapPQueue* pq = new HeapPQueue();

    /* Simulate a workflow on the priority queue. */
    for (int i = 0; i < 10000; i++) {
        pq->enqueue({ "", -i });
    }
    for (int i = 0; i < 5000; i++) {
        auto removed = pq->dequeue();
        removed.weight = -removed.weight;
        pq->enqueue(removed);
    }
    for (int i = 0; i < 9000; i++) {
        (void) pq->dequeue();
    }

    delete pq;

    /* Net allocations now - net allocations at start = allocations leaked by
     * the HeapPQueue implementation. If this number is not zero, it might
     * indicate a memory leak.
     */
    int netAllocations = netDataPointAllocations() - allocationsBefore;
    EXPECT_EQUAL(netAllocations, 0);
}
