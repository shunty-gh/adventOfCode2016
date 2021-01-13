#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <iomanip>

// https://adventofcode.com/2016/day/22

const std::string InputFile{"day22-input.txt"};

const std::string pattern{R"(.*-x(\d{1,2})-y(\d{1,2})\s*(\d+)T\s*(\d+)T\s*(\d+)T\s*.*)"};
const std::regex re{pattern};

struct GridLocation {
    int X, Y;
};

bool operator<(const GridLocation& a, const GridLocation& b) { return a.Y == b.Y ? a.X < b.X : a.Y < b.Y; }

struct GridNode {
    int X, Y;
    int Size, Used, Avail;
};

void PrintGrid(std::map<GridLocation, GridNode>&, int, int);

int main() {
    std::fstream inputfile(InputFile, std::ios_base::in | std::ios_base::binary);
    std::string line;
    std::smatch sm;
    std::map<GridLocation, GridNode> nodes;
    int maxX = 0, maxY = 0;
    while (std::getline(inputfile, line)) {
        // Strip CR
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());

        if (line.rfind("/dev/grid", 0) != 0) {
            continue;
        }

        std::regex_match(line, sm, re);
        int x = std::stoi(sm[1]);
        int y = std::stoi(sm[2]);
        if (x > maxX) maxX = x;
        if (y > maxY) maxY = y;
        nodes[GridLocation{x,y}] = GridNode{ x, y, std::stoi(sm[3]), std::stoi(sm[4]), std::stoi(sm[5]) };
    }

    int part1 = 0;
    for (int y = 0; y <= maxY; y++) {
        for (int x = 0; x <= maxX; x++) {
            GridLocation id{x,y};
            auto check = nodes[id];

            if (check.Used == 0) {
                continue;
            }

            for (int y1 = 0; y1 <= maxY; y1++) {
                for (int x1 = 0; x1 <= maxX; x1++) {
                    if (x == x1 && y == y1) {
                        continue;
                    }

                    GridLocation gl{x1,y1};
                    auto gn = nodes[gl];

                    if (check.Used <= gn.Avail) {
                        part1++;
                    }
                }
            }
        }
    }
    std::cout << "Part 1: " << part1 << std::endl;

    /* Part 2

       We could:
         Find the 'empty' node
         Perform A* to get the empty space next to the data node
         Do another sort of A* to get the data node and empty nodes shuffled across
         to the destination

       or

       We could look at it...

       Use the PrintGrid(...) function to display the grid on-screen (wide monitor recommended).
       From observation we see that:
       * Most nodes are in the order of 80-90 in size
       * There is a row of mostly much larger nodes that are almost all virtually full
       * There is one node that has 0 used (for our input this is x=3, y=28)
       * The data node is at x=32, y=0
       We need to 'move' the empty node - by taking all the data in an adjacent node and putting
       it in the current 'empty' node, thus moving the empty node to the adjacent node - up to the
       position to the left of the target data node, ie x=31, y=0.
       However, due to the blocking "wall" of the larger nodes we can't move directly to the data
       node as there is too much data in these nodes to be able to swap with any empty node. We
       have to skirt around the blocking nodes (in this case go to the left end to get round and
       then make our way back up and to the right).
         Start at 3,28
         Move up to 3,21     (7 moves)
         Move along to 0,21  (3)
         Move up to 0,19     (2)
         Move along to 31,19 (31)
         Move up to 31,0     (19)
         = 62 moves
       Once here we need to "shuffle" the data node across to the left until we get it into the
       final target ie we reach 0,0.
       First we swap the empty and data nodes (1 move) - data is now in 0,31 and empty is in 0,32
       Then shuffle along - move the empty node down 1, left 2, up 1 (ie around the current position
       of the data node) and then swap the empty node and data nodes again (5 moves per shuffle)
       Data moves from x=31 to x=0 (31 * 5 moves)

       Total moves = 62 + 1 + (31 * 5) = 218 (NB: for this particular input only)
     */
    PrintGrid(nodes, maxX, maxY);
    std::cout << "Part 2: " << 218 << std::endl;
}

void PrintGrid(std::map<GridLocation, GridNode>& grid, int maxX, int maxY) {
    // This needs a WIDE screen to display without wrapping
    const size_t llen = 9 * (maxX + 1);
    auto dashLine = std::string(llen, '-');
    auto doubleDashLine = std::string(llen, '=');
    std::cout << "Max X: " << maxX << "; Max Y: " << maxY << "\n\n";
    std::cout << "   ";
    for (int x = 0; x <= maxX; x++) {
        std::cout << "|   " << std::setw(3) << x << "  ";
    }
    std::cout << "|\n";
    std::cout << "    " << doubleDashLine << "\n";

    for (int y = 0; y <= maxY; y++) {
        std::cout << std::setw(3) << y;
        for (int x = 0; x <= maxX; x++) {
            auto gl = GridLocation{x,y};
            auto gn = grid[gl];

            if (gn.Used > 100) {
                std::cout << "|#" << gn.Used << "/" << gn.Size << "";
            } else if (gn.Used == 0) { // The empty slot
                std::cout << "| _" << gn.Used << "/" << gn.Size << "_ ";
            } else {
                std::cout << "|  " << gn.Used << "/" << gn.Size << " ";
            }
        }
        std::cout << "|\n";
        std::cout << "    " << dashLine << "\n";
    }
    std::cout << std::endl;
}