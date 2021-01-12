#include <iostream>
#include <memory>

// https://adventofcode.com/2016/day/19

const int Input = 3012210;

struct Node {
    int Id;
    std::unique_ptr<Node> Next;

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

    auto first = std::make_unique<Node>(Node{1});
    auto head = first.get();
    auto tail = first.get();
    Node* dp;
    for (int i = 2; i <= Input; i++) {
        tail->Next = std::make_unique<Node>(Node{i});
        if (i == (Input/2)) {
            dp = tail->Next.get();
        }
        tail = tail->Next.get();
    }
    tail->Next = std::move(first);

    int n = Input;
    while (n > 1) {
        dp->Next = std::move(dp->Next->Next);

        if (n%2 == 1) {
            dp = dp->Next.get();
        }
        n--;
    }
    std::cout << "Part 2: " << dp->Id << std::endl;
    dp->Next = nullptr; // Prevents a 16 byte leak. Totally not necessary in this case but just trying out [Valgrind](https://www.valgrind.org/)
}
