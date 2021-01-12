#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

// https://adventofcode.com/2016/day/21

//#define TEST
#ifdef TEST
    const std::string InputFile{"day21-input-test.txt"};
    const std::string Passphrase{"abcde"};
    const std::string Scrambled{"decab"};
#else
    const std::string InputFile{"day21-input.txt"};
    const std::string Passphrase{"abcdefgh"};
    const std::string Scrambled{"fbgdceah"};
#endif

const std::string TwoInts{R"(.*\s(\d+)\s.*\s(\d+).*)"};
const std::string OneInt{R"(.*\s(\d+).*)"};
const std::string TwoChars{R"(.*\s([a-z])\s.*\s([a-z]).*)"};
const std::string OneChar{R"(.*\s([a-z]).*)"};

const std::regex TwoInts_re{TwoInts};
const std::regex OneInt_re{OneInt};
const std::regex TwoChars_re{TwoChars};
const std::regex OneChar_re{OneChar};

enum CommandType {
    SwapPosition,
    SwapLetter,
    ReversePositions,
    RotateLeft,
    RotateRight,
    Rotate,
    MovePosition,
};

struct Command {
    CommandType cmd;
    int n1, n2;
    char c1, c2;
};

void swap_position(std::vector<char>& pwd, int p1, int p2) {
    char tmp = pwd[p1];
    pwd[p1] = pwd[p2];
    pwd[p2] = tmp;
}

void swap_letter(std::vector<char>& pwd, char c1, char c2) {
    int p1 = -1, p2 = -1;
    for (int i = 0; p1 < 0 || p2 < 0; i++) {
        if (pwd[i] == c1) {
            p1 = i;
        } else if (pwd[i] == c2) {
            p2 = i;
        }
    }

    char tmp = pwd[p1];
    pwd[p1] = pwd[p2];
    pwd[p2] = tmp;
}

void reverse_positions(std::vector<char>& pwd, int p1, int p2) {
    std::vector<char> swap;
    for (int i = p1; i <= p2; i++) {
        swap.push_back(pwd[i]);
    }
    for (int i = 0; i < swap.size(); i++) {
        pwd[p2 - i] = swap[i];
    }
}

void rotate(std::vector<char>& pwd, int steps) {
    int sz = pwd.size();
    std::vector<char> result(sz, ' ');
    for (int i = 0; i < sz; i++) {
        int newi = ((((i + steps) % sz) + sz) % sz);
        result[newi] = pwd[i];
    }
    for (int i = 0; i < sz; i++) {
        pwd[i] = result[i];
    }
}

void rotate_left(std::vector<char>& pwd, int steps) {
    rotate(pwd, -steps);
}

void rotate_right(std::vector<char>& pwd, int steps) {
    rotate(pwd, steps);
}

void rotate_based_on(std::vector<char>& pwd, char c1) {
    int p1 = -1;
    for (int i = 0; p1 < 0; i++) {
        if (pwd[i] == c1) {
            p1 = i;
        }
    }
    int steps = p1 >= 4 ? 2 + p1 : 1 + p1;
    rotate_right(pwd, steps);
}

void unrotate_based_on(std::vector<char>& pwd, char c1) {
    // Keep rotating left until it matches the equivalent rotate_based_on result

    // Could (Should?) really do this by working out how many left rotations
    // based on the index of c1 but that doesn't really work well/easily in the
    // general case. This brute force / iterative moethod works fine for smallish
    // lengths of password.

    std::vector<char> tmp1{pwd};
    int rcount = 0;
    while (true) {
        rcount++;
        rotate_left(tmp1, 1);
        std::vector<char> tmp2{tmp1};
        rotate_based_on(tmp2, c1);
        if (tmp2 == pwd) {
            rotate_left(pwd, rcount);
            break;
        }
    }
}

void move_position(std::vector<char>& pwd, int p1, int p2) {
    int sz = pwd.size();
    std::vector<char> result(sz, ' ');
    for (int i = 0; i < sz; i++) {
        int newi = i;

        if ((i < p1 && i < p2) || (i > p1 && i > p2)) {
            result[i] = pwd[i];
        } else if (i == p2) {
            result[i] = pwd[p1];
        } else if (i >= p1 && i < p2) {
            result[i] = pwd[i+1];
        } else if (i <= p1 && i > p2) {
            result[i] = pwd[i-1];
        }
    }
    for (int i = 0; i < sz; i++) {
        pwd[i] = result[i];
    }
}

int main() {
    std::fstream inputfile(InputFile, std::ios_base::in | std::ios_base::binary);
    std::string line;
    std::smatch sm;
    std::vector<Command> commands;
    while (std::getline(inputfile, line)) {
        // Strip CR
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());

        if (line.rfind("swap position", 0) == 0) {
            std::regex_match(line, sm, TwoInts_re);
            commands.push_back(Command{CommandType::SwapPosition, std::stoi(sm[1]), std::stoi(sm[2]), ' ', ' '});
        } else if (line.rfind("swap letter", 0) == 0) {
            std::regex_match(line, sm, TwoChars_re);
            commands.push_back(Command{CommandType::SwapLetter, 0, 0, *sm[1].first, *sm[2].first});
        } else if (line.rfind("reverse positions", 0) == 0) {
            std::regex_match(line, sm, TwoInts_re);
            commands.push_back(Command{CommandType::ReversePositions, std::stoi(sm[1]), std::stoi(sm[2]), ' ', ' '});
        } else if (line.rfind("rotate based on", 0) == 0) {
            std::regex_match(line, sm, OneChar_re);
            commands.push_back(Command{CommandType::Rotate, 0, 0, *sm[1].first, ' '});
        } else if (line.rfind("rotate left", 0) == 0) {
            std::regex_match(line, sm, OneInt_re);
            commands.push_back(Command{CommandType::RotateLeft, std::stoi(sm[1]), 0, ' ', ' '});
        } else if (line.rfind("rotate right", 0) == 0) {
            std::regex_match(line, sm, OneInt_re);
            commands.push_back(Command{CommandType::RotateRight, std::stoi(sm[1]), 0, ' ', ' '});
        } else if (line.rfind("move position", 0) == 0) {
            std::regex_match(line, sm, TwoInts_re);
            commands.push_back(Command{CommandType::MovePosition, std::stoi(sm[1]), std::stoi(sm[2]), ' ', ' '});
        } else {
            std::cout << "Unknown scrambler instruction: " << line << std::endl;
            return -1;
        }
    }

    std::vector<char> passphrase;
    for (auto &&c : Passphrase) {
        passphrase.push_back(c);
    }

    for (int i = 0; i < commands.size(); i++) {
        Command cmd = commands[i];
        switch (cmd.cmd) {
            case CommandType::SwapPosition:
                swap_position(passphrase, cmd.n1, cmd.n2);
                break;
            case CommandType::SwapLetter:
                swap_letter(passphrase, cmd.c1, cmd.c2);
                break;
            case CommandType::ReversePositions:
                reverse_positions(passphrase, cmd.n1, cmd.n2);
                break;
            case CommandType::RotateLeft:
                rotate_left(passphrase, cmd.n1);
                break;
            case CommandType::RotateRight:
                rotate_right(passphrase, cmd.n1);
                break;
            case CommandType::MovePosition:
                move_position(passphrase, cmd.n1, cmd.n2);
                break;
            case CommandType::Rotate:
                rotate_based_on(passphrase, cmd.c1);
                break;
        }
    }

    std::string part1(passphrase.begin(), passphrase.end());
    std::cout << "Part 1: " << part1 << std::endl;

    std::vector<char> scrambled;
    //for (auto &&c : part1) {
    for (auto &&c : Scrambled) {
        scrambled.push_back(c);
    }

    for (int i = commands.size() - 1; i >= 0; i--) {
        Command cmd = commands[i];
        switch (cmd.cmd) {
            case CommandType::SwapPosition: // Same
                swap_position(scrambled, cmd.n1, cmd.n2);
                break;
            case CommandType::SwapLetter: // Same
                swap_letter(scrambled, cmd.c1, cmd.c2);
                break;
            case CommandType::ReversePositions: // Same
                reverse_positions(scrambled, cmd.n1, cmd.n2);
                break;
            case CommandType::RotateLeft: // Opposite rotate
                rotate_right(scrambled, cmd.n1);
                break;
            case CommandType::RotateRight: // Opposite rotate
                rotate_left(scrambled, cmd.n1);
                break;
            case CommandType::MovePosition: // Swap the move position parameters
                move_position(scrambled, cmd.n2, cmd.n1);
                break;
            case CommandType::Rotate: // Un-rotate
                unrotate_based_on(scrambled, cmd.c1);
                break;
        }
    }

    std::string part2(scrambled.begin(), scrambled.end());
    std::cout << "Part 2: " << part2 << std::endl;
}
