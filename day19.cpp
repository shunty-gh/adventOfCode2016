#include <iostream>

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

    /* For part 2 we'll build a single linked list and maintain a
       pointer to the element *before* the next item to be deleted.
       Call this the delete pointer (dp). This means that dp->Next is
       the next one to be deleted and therefore dp->Next->Next will
       be the new value that dp will point to after removal.
       Due to the fact that we will always be deleting at a point n/2
       elements away from the current, and the current always advances
       by 1 we see that the delete pointer advances only when n changes
       from odd to even
    */
    Node first{1};
    Node* n1 = &first;
    Node* tail = n1;
    Node* dp;
    for (int i = 2; i <= Input; i++) {
        Node* nn = new Node;
        nn->Id = i;
        if (i == (Input/2)) {
            dp = nn;
        }
        tail->Next = nn;
        tail = nn;
    }
    tail->Next = n1;

    int n = Input;
    Node* nd = n1;
    while (n > 1) {
        // "Remove" element by advancing the ->Next pointer of dp
        dp->Next = dp->Next->Next;
        // Advance the delete pointer when we go from odd to even
        if (n%2 == 1) {
            dp = dp->Next;
        }
        // Reduce total count
        n--;
        // Advance the "current" element
        nd = nd->Next;
    }

    std::cout << "Part 2: " << nd->Id << std::endl;
}
