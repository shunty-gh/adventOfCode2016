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
#include <functional>


/* ***
 AoC 2016 - Day 11: Radioisotope Thermoelectric Generators
 The state of each floor is held as an int - technically we could use 16 bit int.
 Each generator and each microchip takes one bit. The lower 8 bits represent microchips
 and the higher 8 bits represent generators. For a microchip at bit k the matching
 generator will be bit k << 8. eg if we set the polonium microchip to be the
 value 1 (ie bit 0) then a polonium generator on the same floor will be indicated
 by bit 9 being set.
 To "move" an item from one floor to another we *unset* the appropriate state bit
 on the existing floor and *set* the state bit on the new floor.
   x & ~(1 << i) => unset the bit at position i
   x | (1 << i)  => set the bit at position i

 There are two options here - use a priority queue or just a normal queue.
 Comment/uncommment the #define line below as appropriate.
 It seems, in this case, that the normal queue is significantly quicker. This could
 well be to do with my sub-par implementation or it could be to do with the fact that
 there are only 4 floors and a small number of items so the priorities are mostly
 the same.

* ***  */

//#define PRIORITY_QUEUE

struct Component {
    int floor;
    std::string name;
};

struct State {
    int CurrentFloor;
    int FloorState[4];
};

struct StateToVisit {
    State ThisState;
    int Moves;
#ifdef PRIORITY_QUEUE
    int fScore;
#endif
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

State get_initial_state(std::vector<Component>& generators, std::vector<Component>& microchips) {
    // convert generators and chips to ints. Lo 8 bits = chips; Hi 8 bits = generators
    State result{0, {0,0,0,0}};
    for (int i = 0; i < generators.size(); i++) {
        result.FloorState[generators[i].floor - 1] |= (1 << 8 << i);
        result.FloorState[microchips[i].floor - 1] |= (1 << i);
    }
    return result;
}

int step_count(const State& initialState, const State& targetState);

int main() {

    std::fstream inputfile("day11-input.txt", std::ios_base::in);
    //std::fstream inputfile("day11-input-test.txt", std::ios_base::in);
    std::string line;
    int floor = 0;
    std::vector<Component> generators{};
    std::vector<Component> microchips{};
    while (std::getline(inputfile, line)) {
        ++floor; // the input contains floors in proper order

        // Find generators. Scan the line, find "generator" select the previous word to get the name.
        int gpos = line.find("generator", 0);
        while (gpos >= 0) {
            int spos = line.rfind(" ", gpos - 2);
            std::string gen = line.substr(spos + 1, gpos - spos - 2);
            Component g{floor, gen};
            generators.push_back(g);

            // next generator
            gpos = line.find("generator", gpos + 1);
        }

        // Find microchips. Scan the line etc etc
        int mpos = line.find("microchip", 0);
        while (mpos >= 0) {
            int spos = line.rfind(" ", mpos - 2);
            std::string mc = line.substr(spos + 1, line.find("-", spos) - spos - 1);
            Component m{floor, mc};
            microchips.push_back(m);

            // next chip
            mpos = line.find("microchip", mpos + 1);
        }
    }
    inputfile.close();
    // sort generators and microchips by name so that the indexes match
    std::sort(generators.begin(), generators.end(), [](Component& a, Component& b) { return a.name < b.name; });
    std::sort(microchips.begin(), microchips.end(), [](Component& a, Component& b) { return a.name < b.name; });

    auto begin = std::chrono::high_resolution_clock::now();

    //
    // *** Part 1
    //
    State initialState = get_initial_state(generators, microchips);
    int allbits = (1 << generators.size()) - 1;
    State targetState{3, {0,0,0, allbits | (allbits << 8)}};

    int part1 = step_count(initialState, targetState);
    std::cout << "Part 1: " << part1 << std::endl;

    //
    // *** Part 2 ***
    //
    // Add extra generators and chips for part 2
    generators.push_back(Component{1, "elerium"});
    microchips.push_back(Component{1, "elerium"});
    generators.push_back(Component{1, "dilithium"});
    microchips.push_back(Component{1, "dilithium"});

    // rebuild initial and target states
    initialState = get_initial_state(generators, microchips);
    allbits = (1 << generators.size()) - 1;
    targetState = State{3, {0,0,0, allbits | (allbits << 8)}};

    int part2 = step_count(initialState, targetState);
    std::cout << "Part 2: " << part2 << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << "ms" << std::endl;
}

#ifdef PRIORITY_QUEUE
int calculate_heuristic(const State& current) {
    // Estimate minimum cost of moving all items up to fourth floor
    int result = 0;
    for (int i = 0; i < 3; i++) {
        // Get bit count for this floor
        int bc = 0;
        int fs = current.FloorState[i];
        while (fs > 0) {
            if ((fs & 1) == 1) {
                bc++;
            }
            fs = fs >> 1;
        }
        result += (bc * (3 - i));
    }
    return result / 2; // We can move up to 2 items at once
}

// Comparison function for the StateToVisit priority queue
bool compare_state_to_visit(StateToVisit s1, StateToVisit s2) {
    // We want *lower* values to be at the top
    // ie the opposite of the normal priority_queue order
    //return s1.Moves > s2.Moves;
    return s1.fScore > s2.fScore;
}

//typedef std::priority_queue<StateToVisit, std::vector<StateToVisit>, decltype(&compare_state_to_visit)> stv_queue;
typedef std::priority_queue<StateToVisit, std::vector<StateToVisit>,  std::function<bool(StateToVisit, StateToVisit)>> stv_queue;
#endif

int step_count(const State& initialState, const State& targetState) {
    std::map<State,int> seen;
#ifdef PRIORITY_QUEUE
    stv_queue tovisit(compare_state_to_visit);
#else
    std::queue<StateToVisit> tovisit;
#endif
    StateToVisit currentSTV;
    State current;
    tovisit.push(StateToVisit{initialState,0});
    seen[initialState] = 0;
    while (!tovisit.empty()) {
#ifdef PRIORITY_QUEUE
        currentSTV = tovisit.top();
#else
        currentSTV = tovisit.front();
#endif
        current = currentSTV.ThisState;
        tovisit.pop();

        auto itc = seen.find(current);
        if (itc != seen.end() && itc->second > currentSTV.Moves) {
            continue;
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
                            //std::cout << "Complete in " << currentSTV.Moves + 1 << " moves" << std::endl;
                            return currentSTV.Moves + 1;
                        }

                        auto it = seen.find(ns);
                        if (it == seen.end() || it->second > currentSTV.Moves + 2) {
#ifdef PRIORITY_QUEUE
                            tovisit.push(StateToVisit{ns, currentSTV.Moves + 1, currentSTV.Moves + 1 + calculate_heuristic(ns)});
#else
                            tovisit.push(StateToVisit{ns, currentSTV.Moves + 1});
#endif
                            seen[ns] = currentSTV.Moves + 1;
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
                                    //std::cout << "Complete in " << currentSTV.Moves + 1 << " moves" << std::endl;
                                    return currentSTV.Moves + 1;
                                }

                                auto it = seen.find(ns);
                                if (it == seen.end() || it->second > currentSTV.Moves + 2) {
#ifdef PRIORITY_QUEUE
                                    tovisit.push(StateToVisit{ns, currentSTV.Moves + 1, currentSTV.Moves + 1 + calculate_heuristic(ns)});
#else
                                    tovisit.push(StateToVisit{ns, currentSTV.Moves + 1});
#endif
                                    seen[ns] = currentSTV.Moves + 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return -1; // failed to complete
}
