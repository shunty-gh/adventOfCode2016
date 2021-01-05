#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

//#define TEST

// *** Input values
#ifdef TEST
// expect 11 for part 1
const int MagicNumber = 10;
const int TargetX = 7;
const int TargetY = 4;
#else
const int MagicNumber = 1358;
const int TargetX = 31;
const int TargetY = 39;
#endif
// ***

enum LocationState {
    Unknown,
    Space,
    Wall,
};

int bit_count(int num) {
    int bc = 0;
    int curr = num;
    while (curr > 0) {
        if ((curr & 1) == 1) {
            bc++;
        }
        curr = curr >> 1;
    }
    return bc;
}

bool is_wall(int x, int y) {
    int v1 = MagicNumber + (x * x) + (3 * x) + (2 * x * y) + y + (y * y);
    int bc = bit_count(v1);
    return bc % 2 == 1;
}

bool is_space(int x, int y) { return !is_wall(x,y); }

LocationState get_location_state(int x, int y) {
    if (x < 0 || y < 0)
        return Unknown;
    return is_wall(x, y) ? Wall : Space;
}

struct Location {
    int x, y;
    LocationState state;

    Location(){ x = 0; y = 0; state = get_location_state(0, 0); };
    Location(int x1, int y1){ x = x1; y = y1; state = get_location_state(x1, y1); };
};

bool operator<(const Location& l1, const Location& l2) {
    if (l1.x == l2.x) {
        return l1.y < l2.y;
    }
    return l1.x < l2.x;
}

// bool operator==(const Location& l1, const Location& l2) {
//     return l1.x == l2.x
//         && l1.y == l2.y;
// }

struct LocationToVisit {
    Location location;
    int step_count;
};

std::pair<int, int> step_count(int x, int y, int maxSteps);

int main() {
    auto result = step_count(TargetX, TargetY, 50);
    std::cout << "Part 1: " << result.first << std::endl;
    std::cout << "Part 2: " << result.second << std::endl;

}

std::pair<int, int> step_count(int targetX, int targetY, int maxSteps) {
    int result = -1;
    std::map<Location,int> seen;
    std::queue<LocationToVisit> tovisit;
    LocationToVisit ltv;
    Location current;
    tovisit.push(LocationToVisit{Location{1,1}, 0});
    while (!tovisit.empty()) {
        ltv = tovisit.front();
        current = ltv.location;
        tovisit.pop();

        // Find visitable locations
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                // can only go L/R/up/down ie not diagonally, therefore either dx or dy must be 0
                if (dx == dy || (dx != 0 && dy != 0))
                    continue;
                Location next = Location{current.x + dx, current.y + dy};

                if (next.x < 0 || next.y < 0 || next.state == Wall) { // Can't go this way
                    continue;
                }

                if (next.x == targetX && next.y == targetY) {
                    // Found it
                    if (result < 0 || result > ltv.step_count + 1) {
                        result = ltv.step_count + 1;
                        continue;
                    }
                }
                auto it = seen.find(next);
                if (it == seen.end() || it->second > ltv.step_count + 2) {
                    tovisit.push(LocationToVisit{next, ltv.step_count + 1});
                    seen[next] = ltv.step_count + 1;
                }
            }
        }
    }

    int maxReachable = 0;
    // C++ 17, apparently
    // for (auto& [loc, count] : seen) {
    //     if (count <= 50) part2++;
    // }
    // For C++ 11
    for (const auto& kv : seen) {
        if (kv.second <= maxSteps) maxReachable++;
    }
    return std::make_pair(result, maxReachable);
}
