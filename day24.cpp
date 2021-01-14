#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>

// https://adventofcode.com/2016/day/24

//#define TEST
#ifdef TEST
    const std::string InputFile{"day24-input-test.txt"};
#else
    const std::string InputFile{"day24-input.txt"};
#endif

struct GridLocation {
    int X, Y;
};

bool operator<(const GridLocation& a, const GridLocation& b) { return a.Y == b.Y ? a.X < b.X : a.Y < b.Y; }

struct GridNode {
    int X, Y, Content;
};

std::pair<int, int> shortest_paths(std::map<GridLocation, GridNode>&, int, int, std::vector<GridNode>);

int main() {
    std::fstream inputfile(InputFile, std::ios_base::in | std::ios_base::binary);
    std::string line;
    std::map<GridLocation, GridNode> nodes;
    int maxX = 0, maxY = 0, startX, startY;
    int y = 0;
    std::vector<GridNode>nodestovisit;
    while (std::getline(inputfile, line)) {
        // Strip CR
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());

        for (int x = 0; x < line.length(); x++) {
            char c = line[x];
            if (c >= '0' && c <= '9') {
                int n = int(c - '0');
                nodestovisit.push_back(GridNode{x, y, n});
                nodes[GridLocation{x,y}] = GridNode{x, y, n };
            } else {
                nodes[GridLocation{x,y}] = GridNode{x, y, c == '#' ? -99 : -1 };
            }

            if (c == '0') {
                startX = x;
                startY = y;
            }
        }
        y++;
    }
    maxX = line.length() - 1;
    maxY = y - 1;

    auto result = shortest_paths(nodes, maxX, maxY, nodestovisit);
    std::cout << "Part 1: " << result.first << std::endl;
    std::cout << "Part 2: " << result.second << std::endl;
}

struct NodeToVisit {
    int x, y, pathLen;
};

const std::vector<GridLocation>& DirectionDeltas{GridLocation{0,-1}, GridLocation{0,1}, GridLocation{-1,0}, GridLocation{1,0}};

int shortest_path_segment(std::map<GridLocation, GridNode>& grid, int maxX, int maxY, GridNode src, GridNode dest) {
    int result = -1;
    std::map<GridLocation, int> seen{};
    std::queue<NodeToVisit> tovisit{};
    tovisit.push(NodeToVisit{src.X, src.Y, 0});
    seen[GridLocation{src.X, src.Y}] = 0;
    NodeToVisit current;
    while (!tovisit.empty()) {
        current = tovisit.front();
        tovisit.pop();

        for (auto& dd : DirectionDeltas) {
            int newx = current.x + dd.X, newy = current.y + dd.Y;
            if (newx <= 0 || newy <= 0 || newx >= maxX || newy >= maxY) {
                continue;
            }
            if (newx == dest.X && newy == dest.Y) {
                // Found a path
                if ((result < 0) || ((current.pathLen + 1) < result)) {
                    result = current.pathLen + 1;
                }
                continue;
            }

            GridLocation next{newx, newy};
            auto gn = grid[next];
            auto it = seen.find(next);
            if (gn.Content >= -1) { // it's not a wall
                if (it == seen.end() || it->second > current.pathLen + 2) {
                    tovisit.push(NodeToVisit{newx, newy, current.pathLen + 1});
                    seen[next] = current.pathLen + 1;
                }
            }
        }
    }
    return result;
}

struct SegmentLengthToVisit {
    int poi;
    int pathLen;
    std::vector<int> seen;
};

int shortest_combination(const std::vector<int>& segLengths, int poiCount, bool part2) {
    // Start from 0 and find the smallest combination of segments
    int result = -1;

    std::queue<SegmentLengthToVisit> tovisit;
    tovisit.push({0, 0, std::vector<int>{0}});
    SegmentLengthToVisit current;
    while (!tovisit.empty()) {
        current = tovisit.front();
        tovisit.pop();

        for (int i = 1; i < poiCount; i++) {
            // Don't try and visit ourself
            if (current.poi == i) {
                continue;
            }

            // Have we already included this path
            if (std::find(current.seen.begin(), current.seen.end(), i) != current.seen.end()) {
                continue;
            }

            // Distance from our current POI to this new one. As we stored both back and forward
            // lengths we don't have to worry which way round we index.
            int plen = segLengths[(current.poi * 10) + i];
            // Is this going to be the last item we need to visit (other than back to 0 for part 2)
            if (current.seen.size() == poiCount - 1) {
                if (part2) {
                    plen += segLengths[i * 10]; // Length from new poi back to 0
                }
                // Is it the smallest route we've seen so far
                if (result < 0 || (current.pathLen + plen) < result) {
                    result = current.pathLen + plen;
                    // We could make a record of the path to get here - but we don't need it
                    // for either part of the puzzle
                }
                continue;
            }
            // Otherwise we need include it
            std::vector<int> p{current.seen};
            p.push_back(i);
            tovisit.push({i, current.pathLen + plen, p});
        }
    }
    return result;
}

std::pair<int,int> shortest_paths(std::map<GridLocation, GridNode>& grid, int maxX, int maxY, std::vector<GridNode> mustVisit) {

    /* Rather than try to find the solution in one big hair-raising search
       we first determine the shortest paths between every pair of nodes
       0:1, 0:2, ..., 5:6, ..., 8:9 etc. For ease of lookup later on we also
       store the reverse ie 1:0... etc in our list but we don't need to recalculate
       it as it will be the same as the forward length.
       Store the segments in (i*10)+j indexes of a vector as this makes lookup
       much quicker ie the path length from POI=3 to POI=7 is in index [37].

       Once we have all segment lengths we run another shortest path routine
       on the segment lengths to find the shortest combination starting from 0.
       (and finishing back at 0 for part 2)
     */

    std::sort(mustVisit.begin(), mustVisit.end(), [](const GridNode& a, const GridNode& b){ return a.Content < b.Content; });

    // Get a list of shortest paths between each pair of points of interest
    int sz = mustVisit.size();
    std::vector<int> seglen(sz * 10); // Never going to be more than 10 POI
    for (int i = 0; i < sz; i++) {
        auto from = mustVisit[i];
        for (int j = i; j < sz; j++) {
            auto to = mustVisit[j];
            if (from.Content == to.Content) { // Don't visit ourself
                continue;
            }
            int sp = shortest_path_segment(grid, maxX, maxY, from, to);
            seglen[(i * 10) + j] = sp; // Forward...
            seglen[(j * 10) + i] = sp; // ...and backward for ease of lookup
        }
    }

    // Find shortest combination of paths
    int part1 = shortest_combination(seglen, sz, false);
    int part2 = shortest_combination(seglen, sz, true);
    return std::make_pair(part1, part2);
}
