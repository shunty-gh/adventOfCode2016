#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct Command {
    std::string instruction, src, dest;
    int SrcRegisterId;
    int DestRegisterId;
    int SrcValue;
    int DestValue;
};

enum Instruction {
    Instruction_Undefined,
    Copy,
    Inc,
    Dec,
    Jnz,
};

Instruction string_to_instruction(std::string txt) {
    if (txt == "cpy") return Copy;
    if (txt == "inc") return Inc;
    if (txt == "dec") return Dec;
    if (txt == "jnz") return Jnz;
    return Instruction_Undefined;
}

int run_code(std::vector<Command>& commands, int regC);

int main() {
    std::fstream inputfile("day12-input.txt", std::ios_base::in);
    //std::fstream inputfile("day12-input-test.txt", std::ios_base::in);
    std::vector<Command> commands{};
    std::string line;
    while (std::getline(inputfile, line)) {
        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(iss, token, ' ')) {
            tokens.push_back(token);
        }
        // Make sure we have three tokens
        if (tokens.size() == 2) tokens.push_back(" ");
        // Pre-compute the register ids and/or direct values
        int sr = tokens[1] >= "a" && tokens[1] <= "d" ? tokens[1][0] - 'a' : -1;
        int dr = tokens[2] >= "a" && tokens[2] <= "d" ? tokens[2][0] - 'a' : -1;
        int sv = sr < 0 ? std::stoi(tokens[1]) : 0;
        int dv = dr < 0 && tokens[2] != " " ? std::stoi(tokens[2]) : 0;
        commands.push_back(Command{tokens[0], tokens[1], tokens[2], sr, dr, sv, dv});
    }

    int part1 = run_code(commands, 0);
    std::cout << "Part 1: " << part1 << std::endl;

    int part2 = run_code(commands, 1);
    std::cout << "Part 2: " << part2 << std::endl;
}

int run_code(std::vector<Command>& commands, int regC) {
    int ip = 0;
    int icount = commands.size();
    int registers[4]{0};
    registers[2] = regC;
    while (ip < icount) {
        Command cmd = commands[ip];
        Instruction current = string_to_instruction(cmd.instruction);
        int v1;

        switch (current) {
            case Copy:
                v1 = cmd.SrcValue;
                if (cmd.SrcRegisterId >= 0) {
                    v1 = registers[cmd.SrcRegisterId];
                }
                registers[cmd.DestRegisterId] = v1;
                ip++;
                break;
            case Inc:
                registers[cmd.SrcRegisterId] += 1;
                ip++;
                break;
            case Dec:
                registers[cmd.SrcRegisterId] -= 1;
                ip++;
                break;
            case Jnz:
                v1 = cmd.SrcValue;
                if (cmd.SrcRegisterId >= 0) {
                    v1 = registers[cmd.SrcRegisterId];
                }
                ip += v1 != 0 ? cmd.DestValue : 1;
                break;
            default:
                std::cout << "Error processing instruction " << cmd.instruction << std::endl;
                return -1;
        }
    }
    return registers[0];
}