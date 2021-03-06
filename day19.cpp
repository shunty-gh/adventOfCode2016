#include <iostream>

// https://adventofcode.com/2016/day/20

/* Linked list using 'new' and 'delete'
   For a more RAII approach see ./day19-raii.cpp
   For a far simpler method just using vector<int>
   see ./day19-no-pointers.cpp
 */

const int Input = 3012210;
//const int Input = 5;
//const int Input = 10;

struct Node {
    int Id;
    Node *Next;

    Node() { Id = 0; Next = nullptr; }
    Node(int id) { Id = id; Next = nullptr; }
};

int main() {
    /* From observation of the patterns generated when writing
       out by hand the process for low input values it can be seen
       that part 1 can be determined by finding the highest power of
       2 less than input. In this case 2^21.
       Subtract that from Input, multiply by 2 and add 1
    */
    int part1 = ((Input - (1<<21)) * 2) + 1;
    std::cout << "Part 1: " << part1 << std::endl;

    /* Later...
       It turns out that this is the [Josephus Problem](https://en.wikipedia.org/wiki/Josephus_problem)
       and the solution given on the wikipedia page is:
         2(n - 2^Log2(n)) + 1

       Who knew.
    */

    /* For part 2 we'll build a single linked list and maintain a
       pointer to the element *before* the next item to be deleted.
       Call this the delete pointer (dp). This means that dp->Next is
       the next item to be deleted and therefore dp->Next->Next will
       be the new value that dp will point to after removal.
       Due to the fact that we will always be deleting at a point n/2
       elements away from the current, and the current always advances
       by 1 we see that the delete pointer advances only when n changes
       from odd to even. (Due to the AoC rule: "If two Elves are across
       the circle, the one on the left (from the perspective of the
       stealer) is stolen from.")
    */
    Node* n1 = new Node(1);
    Node* tail = n1;
    Node* dp;
    for (int i = 2; i <= Input; i++) {
        Node* nn = new Node(i);
        if (i == (Input/2)) {
            dp = nn;
        }
        tail->Next = nn;
        tail = nn;
    }
    tail->Next = n1;

    // Because we have the 'delete pointer' we don't actually need to keep
    // track of any notion of a 'current' element
    int n = Input;
    while (n > 1) {
        // Remove element by advancing the ->Next pointer of dp
        // Should really free the no-longer used memory - technically we can get
        // away with it because the program terminates straight after this but
        // I suppose we should be 'correct'.
        Node* tmp = dp->Next;
        // Point the dp to the new node, ie skip the one we're currently pointing at
        dp->Next = dp->Next->Next;
        // Clean up the removed node
        tmp->Next = NULL;
        delete tmp;

        // Advance the delete pointer when we go from odd to even
        if (n%2 == 1) {
            dp = dp->Next;
        }
        // Reduce total count
        n--;
    }

    std::cout << "Part 2: " << dp->Id << std::endl;
    delete dp; // For the sake of completeness. Otherwise, technically, we'd leak 16 whole bytes
}
