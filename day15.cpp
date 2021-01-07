
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

#define SIEVING

// https://adventofcode.com/2016/day/15

const string pattern{R"(.* has (\d{1,2}) positions; .* at position (\d{1,2})\.)"};

struct Disc {
    int positions;
    int startAt;
    int current;

    Disc(int pos, int start) { positions = pos; startAt = start; current = start; }
};

int run_game(const vector<Disc>& discs);

int main() {
    fstream inputfile("day15-input.txt", ios_base::in | ios_base::binary);
    //fstream inputfile("day15-input-test.txt", std::ios_base::in);
    string line;
    vector<Disc> discs;
    while (getline(inputfile, line)) {
        // Strip CR. Bloody Windows/Linux compatability nonsense
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());

        regex re{pattern};
        smatch sm;
        regex_match(line, sm, re);
        discs.push_back(Disc{stoi(sm[1]), stoi(sm[2])});
    }

    auto begin = std::chrono::high_resolution_clock::now();

    int part1 = run_game(discs);
    cout << "Part 1:" << part1 << endl;

    // Add an extra disc for part 2
    discs.push_back(Disc{11, 0});
    int part2 = run_game(discs);
    cout << "Part 2:" << part2 << endl;

    auto end = std::chrono::high_resolution_clock::now();
#ifdef SIEVING
    std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count() << " micro seconds" << std::endl;
#else
    std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << "ms" << std::endl;
#endif
}

#ifdef SIEVING
int run_game(const vector<Disc>& initialDiscs) {
    // This turns out to be a simple example of the Chinese Remainder Theorem, I think
    // https://en.wikipedia.org/wiki/Chinese_remainder_theorem
    // Therefore we can solve it with sieving
    // https://en.wikipedia.org/wiki/Chinese_remainder_theorem#Search_by_sieving
    // although, in this instance, it doesn't actually take very long using the naive
    // way anyway.
    // However, this method is still *much* faster.

    // This works because all the number of positions on the discs are prime numbers

    // It could, technically, be made faster still if we were to sort the discs by
    // total positions with highest first. But this is a little overkill here (and
    // the gains may even be lost due to the time taken to sort).

    // t0 is the number of seconds that should elapse before capsule release in
    // order that the disc in question be at position 0 when the capsule arrives
    // (if there weren't any other discs obsructing the path).
    //   t0 = ( (Total positions) - (Start position) - (1-based index of the disc in question) ) mod (Total positions)
    // ie the disc will be at the correct position on arrival if the capsule departs at
    //   time = t0
    // or after any subsequent full rotation of the disc:
    //   time = t0 + (n * disc.positions)

    int result = 0, mult = 1;
    for (int discIndex = 0; discIndex < initialDiscs.size(); discIndex++) {
        Disc disc = initialDiscs[discIndex];
        int t0 = (((disc.positions - disc.startAt - (discIndex + 1)) % disc.positions) + disc.positions) % disc.positions; // 'cos -ve % N is still -ve
        while (true) {
            if ((result % disc.positions) == t0) {
                // found a match where all previous discs align and so does the current one
                // because they're all primes we increase the multiplier
                mult *= disc.positions;
                break;
            }
            result += mult;
        }
    }
    return result;
}
#else  // The naive approach
void rotate_discs(vector<Disc>& discs, int howManyTimes) {
    for (int i = 0; i < discs.size(); i++) {
        discs[i].current = (discs[i].current + howManyTimes) % discs[i].positions;
    }
}

int run_game(const vector<Disc>& initialDiscs) {
    int releaseTime = 0;
    int pos = 0;
    while (pos < initialDiscs.size()) {
        // Take/Reset working copy of the discs
        vector<Disc> discs{initialDiscs};
        pos = 0;
        // rotate the discs until it's release time
        rotate_discs(discs, releaseTime);

        while (pos < discs.size()) {
            // rotate all discs
            rotate_discs(discs, 1);

            // can we get through the current disc
            if (discs[pos].current != 0) {
                // try a new start time
                releaseTime++;
                break;
            }
            pos++;
        }
    }

    return releaseTime;
}
#endif
