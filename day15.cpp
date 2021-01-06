#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

const string pattern{R"(.* has (\d{1,2}) positions; .* at position (\d{1,2})\.)"};

struct Disc {
    int positions;
    int startAt;
    int current;

    Disc(int pos, int start) { positions = pos; startAt = start; current = start; }
};

int run_game(const vector<Disc>& discs);

int main() {
    fstream inputfile("day15-input.txt", std::ios_base::in);
    //fstream inputfile("day15-input-test.txt", std::ios_base::in);
    string line;
    regex re{pattern};
    vector<Disc> discs;
    while (getline(inputfile, line)) {
        smatch sm;
        regex_match(line, sm, re);
        discs.push_back(Disc{stoi(sm[1]), stoi(sm[2])});
    }

    int part1 = run_game(discs);
    cout << "Part 1:" << part1 << endl;

    // Add an extra disc for part 2
    discs.push_back(Disc{11, 0});
    int part2 = run_game(discs);
    cout << "Part 2:" << part2 << endl;
}

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
