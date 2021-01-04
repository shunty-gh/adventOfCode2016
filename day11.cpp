#include <chrono>
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>
#include <map>

struct Generator {
    int floor;
    int nameId;
    std::string name;
};

struct Microchip {
    int floor;
    int nameId;
    std::string name;
};

struct State {
    int CurrentFloor;
    int FloorState[4];
};

struct StateToVisit {
    State State;
    int Moves;
};

bool operator<(const State& s1, const State& s2) {
    if (s1.CurrentFloor == s2.CurrentFloor) {
        if (s1.FloorState[0] == s2.FloorState[0]) {
            if (s1.FloorState[1] == s2.FloorState[1]) {
                if (s1.FloorState[2] == s2.FloorState[2]) {
                    return s1.FloorState[3] < s2.FloorState[3];
                }
                return s1.FloorState[2] < s2.FloorState[2];
            }
            return s1.FloorState[1] < s2.FloorState[1];
        }
        return s1.FloorState[0] < s2.FloorState[0];
    }
    return s1.CurrentFloor < s2.CurrentFloor;
}

bool operator==(const State& s1, const State& s2) {
    return s1.CurrentFloor == s2.CurrentFloor
        && s1.FloorState[0] == s2.FloorState[0]
        && s1.FloorState[1] == s2.FloorState[1]
        && s1.FloorState[2] == s2.FloorState[2]
        && s1.FloorState[3] == s2.FloorState[3];
}

bool is_stable(int state) {
    // if there are no generators then it's ok
    if ((state >> 8) == 0) return true;
    // if there are no microchips then it's ok
    if ((state & 255) == 0) return true;

    // Otherwise check that for every microchip there is a matching generator
    for (int i = 0; i < 8; i++) {
        if ((state & (1 << i)) > 0) {
            if ((state & (1 << 8 << i)) == 0) {
                return false;
            }
        }
    }
    return true;
}

int index_of_name(std::vector<std::string>& haystack, std::string needle) {
    // find index of supplied string in the supplied vector
    // if the name doesn't exist then add it
    auto it = std::find(haystack.begin(), haystack.end(), needle);
    int result;
    if (it != haystack.end()) {
         result = std::distance(haystack.begin(), it);
    } else {
        haystack.push_back(needle);
        result = haystack.size() - 1;
    }
    return result;

}

int step_count(const State& initialState, const State& targetState);

int main() {

    std::fstream inputfile("day11-input.txt", std::ios_base::in);
    //std::fstream inputfile("day11-input-test.txt", std::ios_base::in);
    std::string line;
    int i = 0;
    std::vector<std::string> names{};
    std::vector<Generator> generators{};
    std::vector<Microchip> microchips{};
    while (std::getline(inputfile, line)) {
        ++i;
        auto trimmed = line.substr(line.find("contains") + 9);
        if (trimmed.find("nothing relevant.") == 0) {
            //std::cout << "Floor " << i << " is empty" << std::endl;
            continue;
        }
        // Find generators. Scan the line, find "generator" select the previous word to get the name.
        int gstart = 0;
        int gpos = trimmed.find("generator", gstart);
        while (gpos >= 0) {
            gstart = gpos + 1;
            int spos = trimmed.rfind(" ", gpos - 2);
            std::string gen = trimmed.substr(spos + 1, gpos - spos - 2);
            //std::cout << "Found" << gen << " generator on floor " << i << std::endl;
            int nameidx = index_of_name(names, gen);
            Generator g{i, nameidx, gen};
            generators.push_back(g);

            // next generator
            gpos = trimmed.find("generator", gstart);
        }

        // Find microchips. Scan the line etc etc
        int mstart = 0;
        int mpos = trimmed.find("microchip", mstart);
        while (mpos >= 0) {
            mstart = mpos + 1;
            int spos = trimmed.rfind(" ", mpos - 2);
            std::string mc = trimmed.substr(spos + 1, trimmed.find("-", spos) - spos - 1);
            //std::cout << "Found" << mc << " microchip on floor " << i << std::endl;

            int nameidx = index_of_name(names, mc);
            Microchip m{i, nameidx, mc};
            microchips.push_back(m);

            // next chip
            mpos = trimmed.find("microchip", mstart);
        }
    }
    inputfile.close();

    State initialState{0, {0,0,0,0}};
    // convert generators and chips to ints. Lo 8 bits = chips; Hi 8 bits = generators
    int allItemsState = 0;
    for (int i = 0; i < generators.size(); i++) {
        auto gen = generators[i];
        auto mc = microchips[i];
        initialState.FloorState[gen.floor - 1] |= (1 << 8 << gen.nameId);
        initialState.FloorState[mc.floor - 1] |= (1 << mc.nameId);

        allItemsState |= (1 << i);
        allItemsState |= (1 << 8 << i);
    }
    State targetState{3, {0,0,0,allItemsState}};

    // check all appears to be correct
    for (int i = 0; i < 4; i++) {
        if (!is_stable(initialState.FloorState[i])) {
            std::cout << "ERROR: Floor " << i + 1 << " is unstable" << std::endl;
        }
    }

    int part1 = step_count(initialState, targetState);
    std::cout << "Part 1: " << part1 << std::endl;

    // Add extra generators and chips for part 2
    int idx = names.size();
    generators.push_back(Generator{1, idx, "elerium"});
    microchips.push_back(Microchip{1, idx++, "elerium"});
    generators.push_back(Generator{1, idx, "dilithium"});
    microchips.push_back(Microchip{1, idx++, "dilithium"});

    // rebuild initial and target states
    State initialState2{0, {0,0,0,0}};
    int allItemsState2 = 0;
    for (int i = 0; i < generators.size(); i++) {
        auto gen = generators[i];
        auto mc = microchips[i];
        initialState2.FloorState[gen.floor - 1] |= (1 << 8 << gen.nameId);
        initialState2.FloorState[mc.floor - 1] |= (1 << mc.nameId);

        allItemsState2 |= (1 << i);
        allItemsState2 |= (1 << 8 << i);
    }
    State targetState2{3, {0,0,0,allItemsState2}};

    int part2 = step_count(initialState2, targetState2);
    std::cout << "Part 2: " << part2 << std::endl;
}

int step_count(const State& initialState, const State& targetState) {
    std::map<State,int> visited;
    std::queue<StateToVisit> tovisit;
    StateToVisit currentSTV;
    State current;
    tovisit.push(StateToVisit{initialState,0});
    visited[initialState] = 0;
    while (!tovisit.empty()) {
        currentSTV = tovisit.front();
        current = currentSTV.State;
        tovisit.pop();

        auto itc = visited.find(current);
        if (itc != visited.end() && itc->second > currentSTV.Moves) {
            continue;
        }

        if (current == targetState) {
            // Done it
            std::cout << "Complete in " << currentSTV.Moves << " moves" << std::endl;
        }

        // Where can we go from here. Up or down 1 floor...
        for (int floor = -1; floor <= 1; floor++) {
            int nextFloor = current.CurrentFloor + floor;
            if (floor == 0 || nextFloor < 0 || nextFloor > 3) {
                continue;
            }

            int thisFloorState = current.FloorState[current.CurrentFloor];
            int nextFloorState = current.FloorState[nextFloor];

            // ...and take either 1 or 2 items with us
            for (int i = 0; i < 16; i++) {
                if ((thisFloorState & (1 << i)) > 0) { // do we have an item
                    // can we move this item from this floor to the next floor
                    int s1 = thisFloorState & ~(1 << i);
                    int s2 = nextFloorState | (1 << i);
                    if (is_stable(s1) && is_stable(s2)) {
                        // add to the queue if we haven't already tried it
                        State ns{current};
                        ns.CurrentFloor = nextFloor;
                        ns.FloorState[current.CurrentFloor] = s1;
                        ns.FloorState[nextFloor] = s2;

                        if (ns == targetState) {
                            std::cout << "Complete in " << currentSTV.Moves + 1 << " moves" << std::endl;
                            return currentSTV.Moves + 1;
                        }

                        auto it = visited.find(ns);
                        if (it == visited.end() || it->second > currentSTV.Moves + 2) {
                            tovisit.push(StateToVisit{ns, currentSTV.Moves + 1});
                            visited[ns] = currentSTV.Moves + 1;
                        }
                    }

                    for (int j = i + 1; j < 16; j++) {
                        if ((thisFloorState & (1 << j)) > 0) {
                            int s3 = s1 & ~(1 << j);
                            int s4 = s2 | (1 << j);

                            if (is_stable(s3) && is_stable(s4)) {
                                // add to the queue if we haven't already tried it
                                State ns{current};
                                ns.CurrentFloor = nextFloor;
                                ns.FloorState[current.CurrentFloor] = s3;
                                ns.FloorState[nextFloor] = s4;

                                if (ns == targetState) {
                                    std::cout << "Complete in " << currentSTV.Moves + 1 << " moves" << std::endl;
                                    return currentSTV.Moves + 1;
                                }

                                auto it = visited.find(ns);
                                if (it == visited.end() || it->second > currentSTV.Moves + 2) {
                                    tovisit.push(StateToVisit{ns, currentSTV.Moves + 1});
                                    visited[ns] = currentSTV.Moves + 1;
                                }
                            }
                        }
                    }
                }

            }
        }
    }
    return -1;
}
