#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

//#define TEST
#ifdef TEST
const int Part1Length = 20;
const string Input = "10000";
#else
const int Part1Length = 272;
const int Part2Length = 35651584;
const string Input = "10010000000110000";
#endif

string get_checksum(int diskLen, string initialState);

int main() {
    cout << "Part 1: " << get_checksum(Part1Length, Input) << endl;
    cout << "Part 2: " << get_checksum(Part2Length, Input) << endl;
}

string get_checksum(int diskLen, string initialState) {
    vector<bool> disk(diskLen, false);
    for (int i = 0; i < initialState.size(); i++) {
        disk[i] = Input[i] == '1';
    }

    // 'overwrite' the elements
    int idx = Input.size();
    while (idx < diskLen) {
        // Add a 0 and then reverse the previous 'bits'
        disk[idx] = 0;
        for (int i = 0; i < idx && (idx + i) < diskLen; i++) {
            disk[idx + i + 1] = !disk[idx - i - 1];
        }
        idx += idx + 1;
    }

    // determine the checksum. In-place as we don't need the 'disk' bits
    // afterwards so no need to allocate another vector
    int cslen = diskLen;
    while (cslen % 2 == 0) {
        for (int i = 0; i < cslen; i += 2) {
            disk[i/2] = (disk[i] == disk[i+1]);
        }
        cslen /= 2;
    }

    // return checksum as a string
    stringstream s;
    for (int i = 0; i < cslen; i++) {
        s << disk[i];
    }
    return s.str();
}
