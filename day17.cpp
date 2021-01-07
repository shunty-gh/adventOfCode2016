#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include "md5.h"

/* https://adventofcode.com/2016/day/17 */
/* NB: This file needs to be compiled along with md5.cpp */

//#define TEST
#ifdef TEST
//const std::string Passcode = "ihgpwlah"; // expect DDRRRD
const std::string Passcode = "kglvqrro"; // DDUDRLRRUDRD
//const std::string Passcode = "ulqzkmiv"; // DRURDRUDDLLDLUURRDULRLDUUDDDRR
#else
const std::string Passcode = "vwbaicqe";
#endif

std::pair<std::string, int> find_path(std::string passcode);

int main() {
    auto paths = find_path(Passcode);
    std::cout << "Part 1 (shortest path)      : " << paths.first << std::endl;
    std::cout << "Part 2 (longest path length): " << paths.second << std::endl;
}

struct Coord {
    int X, Y;
};

struct RoomToVisit {
    int X, Y;
    std::string Path;
};

const int MaxX = 3, MaxY = 3;
const std::set<char> DoorOpenChars{'b', 'c', 'd', 'e', 'f'};
const std::string DirectionChars{"UDLR"};
const std::vector<Coord> DirectionDeltas{Coord{0,-1}, Coord{0,1}, Coord{-1,0}, Coord{1,0}};
const bool is_open(char door) { return DoorOpenChars.find(door) != DoorOpenChars.end(); }
const bool is_target(RoomToVisit rtv) { return rtv.X == MaxX && rtv.Y == MaxY; }

std::pair<std::string, int> find_path(std::string passcode) {
    std::string shortestPath = "";
    int shortestPathLen = -1, longestPathLen = -1, nextPathLen;
    std::queue<RoomToVisit> tovisit;
    tovisit.push(RoomToVisit{0,0,""});
    RoomToVisit currentRTV;

    while (!tovisit.empty()) {
        currentRTV = tovisit.front();
        tovisit.pop();

        std::string hash = md5(passcode + currentRTV.Path);

        for (int i = 0; i < 4; i++) {
            Coord dd = DirectionDeltas[i];
            int nextx = currentRTV.X + dd.X, nexty = currentRTV.Y + dd.Y;
            if (nextx < 0 || nextx > MaxX || nexty < 0 || nexty > MaxY || !is_open(hash[i])) continue;
            RoomToVisit next{nextx, nexty, currentRTV.Path + DirectionChars[i]};
            nextPathLen = next.Path.size();
            // For part 1 only we could do this as a short circuit
            //if (shortestPathLen > 0 && shortestPathLen <= pathlen) continue;
            if (is_target(next)) {
                if (shortestPathLen < 0 || shortestPathLen > nextPathLen) {
                    shortestPath = next.Path;
                    shortestPathLen = nextPathLen;
                }
                if (longestPathLen < 0 || longestPathLen < nextPathLen) {
                    longestPathLen = nextPathLen;
                }
                continue;
            }
            tovisit.push(next);
        }
    }
    return std::make_pair(shortestPath, longestPathLen);
}
