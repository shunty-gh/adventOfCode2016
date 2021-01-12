#include <iostream>
#include <vector>

const int Input = 3012210;

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


    /* *** Part 2 ***
       Instead of a linked list all we really need is an array (vector)
       of ints. The vector index [] indicates the element id and the
       value at [index] is the 'pointer' to the next element in the
       circle (ie the index of the next entry still in the ring).
       To start with all entries will have the value of index+1.
       When an element is deleted/removed we simply change the value
       of the preceeding entry to 'point' to the item that the deleted
       one was pointing to.
       The 'delete pointer' (dp) is the index of the entry *prior*
       to the one that is going to be removed from the ring.
       This means that vector[dp] is the index of the entry to be removed
       and the value held at vector[vector[dp]] is the new value that
       vector[dp] will 'point to'.
       Due to the fact that we will always be deleting at a point n/2
       elements away from the current, and the current always advances
       by 1 we see that the delete pointer advances only when n changes
       from odd to even. (Due to the AoC rule: "If two Elves are across
       the circle, the one on the left (from the perspective of the
       stealer) is stolen from.")
       And because, at the end, there'll only be one element left, we
       don't even need to keep track of the 'current' item because it will
       be the same as the delete pointer.
    */

    std::vector<int> elves;
    for (int i = 0; i <= Input; i++) { // Add, but ignore, element[0]. Makes accessing by index easier.
        elves.push_back(i+1);
    }
    elves[Input] = 1; // Complete the circle

    int curr = 1;
    int n = Input;
    int dp = n/2;
    while (n > 1) {
        elves[dp] = elves[elves[dp]];
        // Advance the dp only when n is about to change from odd to even
        if (n%2 == 1) {
            dp = elves[dp];
        }
        n--;
    }
    std::cout << "Part 2: " << dp << std::endl;
}
