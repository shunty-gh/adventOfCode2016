#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <vector>

//#define TEST
#ifdef TEST
    const int RowCountPart1 = 10;
    const int RowCountPart2 = 10;
    const std::string InputFile{"day18-input-test.txt"};
#else
    const int RowCountPart1 = 40;
    const int RowCountPart2 = 400000;
    const std::string InputFile{"day18-input.txt"};
#endif

const bool Trap = true;
const bool Safe = false;
const char TrapChar = '^';

int safe_tile_count(const std::vector<bool>& initialRow, int rowCount);

int main() {
    std::fstream inputfile(InputFile, std::ios_base::in);
    std::string line;
    std::vector<bool> firstRow;
    while (std::getline(inputfile, line)) {
        for (int i = 0; i < line.size(); i++) {
            firstRow.push_back(line[i] == TrapChar);
        }
    }

    auto begin = std::chrono::high_resolution_clock::now();
    std::cout << "Part 1: " << safe_tile_count(firstRow, RowCountPart1) << std::endl;
    std::cout << "Part 2: " << safe_tile_count(firstRow, RowCountPart2) << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << "ms" << std::endl;
}

const std::set<int> TrapScores{1,3,4,6};
bool is_trap_score(int score) { return TrapScores.find(score) != TrapScores.end(); }

int safe_tile_count(const std::vector<bool>& initialRow, int rowCount) {
    /* The score for an item is the 3 bit number derived from
       (prev << 2) + (this << 1) + next
       The next row can easily determine whether it is safe or a trap
       just by looking at the score for the same position in the previous row.
       Scores 1,3,4,6 are traps

       Calculate the scores for the first row at the fn start
       For the rest of the rows we calculate the element score as we go along
       and overwrite the rowscore array as we go. We don't need to keep any other
       previous values or rows because all that the answer requires is the total
       number of safe tiles.
     */

    int rlen = initialRow.size(), score = 0;
    int result = std::count(initialRow.begin(), initialRow.end(), Safe);

    // Build the scores vector for the initial row
    std::vector<int> rowscores;
    // Elements -1 & row-size are 'Safe'
    rowscores.push_back((initialRow[0] << 1) + initialRow[1]); // == (Safe << 2) + (initialRow[0] << 1) + initialRow[1]
    for (int i = 1; i < rlen - 1; i++) {
        score = (initialRow[i-1] << 2) + (initialRow[i] << 1) + initialRow[i+1];
        rowscores.push_back(score);
    }
    // Add the final tile score
    score = (initialRow[rlen - 2] << 2) + (initialRow[rlen - 1] << 1); // == (initialRow[rlen-2] << 2) + (initialRow[rlen-1] << 1) + Safe
    rowscores.push_back(score);

    for (int y = 1; y < rowCount; y++) {
        bool prev1 = Safe, prev2 = Safe, thisTile = Safe;
        for (int x = 0; x < rlen; x++) {
            thisTile = is_trap_score(rowscores[x]);
            result += (thisTile == Safe);

            // Calculate the score for the *previous* tile based on this tile and the previous 2
            if (x > 0) {
                score = (prev2 << 2) + (prev1 << 1) + (thisTile);
                rowscores[x-1] = score;
            }

            // Shuffle along
            prev2 = prev1;
            prev1 = thisTile;
        }
        // Add the score for the final tile in the row
        score = (prev2 << 2) + (prev1 << 1);
        rowscores[rlen - 1] = score;
    }
    return result;
}
