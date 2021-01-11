#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

// https://adventofcode.com/2016/day/20

const std::string pattern{R"((\d+)-(\d+))"};
const unsigned long MaxAddress = 4294967295; // given in the puzzle but could also use ULONG_MAX, UINT32_MAX etc from <limits>

struct IPRange {
    unsigned long Lo, Hi;
};

int main() {
    std::fstream inputfile("day20-input.txt", std::ios_base::in | std::ios_base::binary);
    //std::fstream inputfile("day20-input-test.txt", std::ios_base::in);
    std::string line;
    std::vector<IPRange> ranges;
    while (std::getline(inputfile, line)) {
        // Strip CR
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());

        std::regex re{pattern};
        std::smatch sm;
        std::regex_match(line, sm, re);
        ranges.push_back(IPRange{std::stoul(sm[1]), stoul(sm[2])});
    }

    std::sort(ranges.begin(), ranges.end(), [](const IPRange& a, const IPRange& b) { return a.Lo == b.Lo ? a.Hi < b.Hi : a.Lo < b.Lo; });
    unsigned long lowest = ranges[0].Hi + 1;
    int loindex = 0;
    bool lofound = false;
    unsigned long allowedcount = 0;
    for (int i = 1; i < ranges.size(); i++) {
        IPRange r = ranges[i];
        if (r.Lo > lowest) {
            // We're there, part 1
            std::cout << "Part 1: " << lowest << std::endl;
            lofound = true;
            loindex = i;
            break;
        }

        if (r.Hi >= lowest) {
            lowest = r.Hi + 1;
        }
    }

    // Part 2
    // Need to allow for overlapped ranges
    unsigned long result = ranges[loindex].Lo - lowest;
    IPRange curr = ranges[loindex];
    for (int i = loindex + 1; i < ranges.size(); i++) {
        // Because it's sorted on r.Lo then the next range will
        // always have a higher (or equal) .Lo value
        IPRange r = ranges[i];
        if (r.Lo <= curr.Hi) {
            if (r.Hi > curr.Hi) { // Expand the exclusion zone
                curr.Hi = r.Hi;
            }
            // else... (r.Hi <= curr.Hi) ie Totally contained, no need to do anything
            continue;
        }
        // otherwise r.Lo must be greater than our current exclusion zone
        // and so we must have some allowed addresses
        result += (r.Lo - curr.Hi - 1);
        // start a new exclusion zone
        curr.Lo = r.Lo;
        curr.Hi = r.Hi;
    }
    // Add on any addresses beyond our highest exclusion
    result += MaxAddress - curr.Hi;
    std::cout << "Part 2: " << result << std::endl;
}
