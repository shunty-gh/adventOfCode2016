/* This file needs to be compiled along with md5.cpp
 */
#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "./md5.h"

//#define TEST

#ifdef TEST
const std::string Input{"abc"}; // expect Part1 == 22728
#else
const std::string Input{"ahsbgdzn"};
#endif
const int TargetKeyCount = 64;
const int Part2ExtraHashCount = 2016;

struct HashInfo {
    int hashIndex;
    int matchIndex;
    std::string hash;
    char tripleChar;
    std::vector<char> fives;

    HashInfo() { hashIndex = 0; matchIndex = 0; hash = ""; tripleChar = ' '; fives = std::vector<char>{}; }
    HashInfo(int index, std::string hash1 ) { hashIndex = index; matchIndex = 0; hash = hash1; tripleChar = ' '; fives = std::vector<char>{}; }
};

bool operator<(const HashInfo& h1, const HashInfo& h2) {
    return h1.hashIndex < h2.hashIndex;
}

std::string generate_md5(std::string salt, int index, int extraHashCount) {
    std::string s = salt + std::to_string(index);

    std::string result = md5(s);
    for (int i = 0; i < extraHashCount; i++) {
        result = md5(result);
    }
    return result;
}

std::vector<HashInfo> key_search(std::string salt, int keyCount, int extraHashCount);
HashInfo process_hash(std::string hash, int index);

int main() {
    auto begin = std::chrono::high_resolution_clock::now();

    auto keys = key_search(Input, TargetKeyCount, 0);
    std::cout << "Part 1: " << keys[TargetKeyCount - 1].hashIndex << std::endl;

    auto keys2 = key_search(Input, TargetKeyCount, Part2ExtraHashCount);
    std::cout << "Part 2: " << keys2[TargetKeyCount - 1].hashIndex << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << "ms" << std::endl;
}

std::vector<HashInfo> key_search(std::string salt, int keyCount, int extraHashCount) {
    std::vector<HashInfo> keys{};
    std::vector<HashInfo> search_list{};
    int index = 0;
    while (keys.size() < keyCount) {
        std::string hash = generate_md5(salt, index, extraHashCount);
        HashInfo hi = process_hash(hash, index);

        // Do we have a triple? If so add it to the search list and, also, see if it
        // has any 5s that match any of the hashes/triples we're already looking for
        if (hi.tripleChar != ' ') {
            if (hi.fives.size() > 0) {
                // reverse search so we can easily ignore out-of-range indexes without
                // having to bother removing them from the list. This does, however, mean
                // that our final keys list will probably be out of index order so will
                // need to be sorted before we return.
                for (int i = search_list.size() - 1; i >= 0; i--) {
                    HashInfo shi = search_list[i];
                    if (index > (shi.hashIndex + 1000)) {
                        break; // don't check any more they're out of range
                    }
                    if (shi.matchIndex > 0) {
                        continue; // already found a match
                    }
                    char tc = shi.tripleChar;
                    for (int j = 0; j < hi.fives.size(); j++) {
                        if (tc == hi.fives[j]) {
                            // found a match
                            shi.matchIndex = hi.hashIndex;
                            keys.push_back(shi);
                            break;
                        }
                    }
                }
            }
            search_list.push_back(hi);
        }
        index++;
    }

    // sort them by hash index
    std::sort(keys.begin(), keys.end());
    return keys;
}

HashInfo process_hash(std::string hash, int index) {
    HashInfo result{index, hash};

    // scan the string, look for triples. if found, then look for
    // any groups of five
    int hs = hash.size();
    char c1, c2, c3, c4, c5;
    for (int i = 0; i < hs - 2; i++) {
        c1 = hash[i];
        c2 = hash[i+1];
        c3 = hash[i+2];
        if (c1 == c2 && c2 == c3) {
            result.tripleChar = c1;

            // Look for any groups of five
            for (int j = i; j < hs - 4; j++) {
                c1 = hash[j];
                c2 = hash[j+1];
                c3 = hash[j+2];
                c4 = hash[j+3];
                c5 = hash[j+4];
                if (c1 == c2 && c1 == c3 && c1 == c4 && c1 == c5) {
                    // found a set of five
                    result.fives.push_back(c1);
                }
            }
            break; // only looking for the first triple
        }
    }
    return result;
}
