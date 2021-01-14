#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// https://adventofcode.com/2016/day/25

const std::string InputFile{"day25-input.txt"};

enum class Instruction {
    Instruction_Undefined,
    Copy,
    Inc,
    Dec,
    Jnz,
    Tgl,
    Out,
};

Instruction string_to_instruction(std::string txt) {
    if (txt == "cpy") return Instruction::Copy;
    if (txt == "inc") return Instruction::Inc;
    if (txt == "dec") return Instruction::Dec;
    if (txt == "jnz") return Instruction::Jnz;
    if (txt == "tgl") return Instruction::Tgl;
    if (txt == "out") return Instruction::Out;
    return Instruction::Instruction_Undefined;
}

struct Command {
    std::string instruction, src, dest;
    Instruction Instruction;
    int SrcRegisterId;
    int DestRegisterId;
    int SrcValue;
    int DestValue;
};

int run_code(std::vector<Command>, int);

int main() {
    std::fstream inputfile(InputFile, std::ios_base::in);
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
        commands.push_back(Command{tokens[0], tokens[1], tokens[2], string_to_instruction(tokens[0]), sr, dr, sv, dv});
    }

    int part1 = -1;
    for (int i = 0; i < (1<<30) && part1 < 0; i++) {
        part1 = run_code(commands, i);
    }
    std::cout << "Part 1: " << part1 << std::endl;

}

int run_code(std::vector<Command> commands, int regA) {
    int ip = 0;
    int icount = commands.size();
    int registers[4]{0};
    bool lastout = -1;
    int okcount = 0;
    registers[0] = regA;
    while (ip < icount) {
        Command cmd = commands[ip];
        Instruction current = cmd.Instruction;
        int v1;

        switch (current) {
            case Instruction::Copy:
                v1 = cmd.SrcValue;
                if (cmd.SrcRegisterId >= 0) {
                    v1 = registers[cmd.SrcRegisterId];
                }
                if (cmd.DestRegisterId >= 0) {
                    registers[cmd.DestRegisterId] = v1;
                }
                ip++;
                break;
            case Instruction::Inc:
                if (cmd.SrcRegisterId >= 0) {
                    registers[cmd.SrcRegisterId] += 1;
                }
                ip++;
                break;
            case Instruction::Dec:
                if (cmd.SrcRegisterId >= 0) {
                    registers[cmd.SrcRegisterId] -= 1;
                }
                ip++;
                break;
            case Instruction::Jnz:
                v1 = cmd.SrcValue;
                if (cmd.SrcRegisterId >= 0) {
                    v1 = registers[cmd.SrcRegisterId];
                }
                if (v1 == 0) {
                    ip++;
                } else if (cmd.DestRegisterId >= 0) {
                    ip += registers[cmd.DestRegisterId];
                } else {
                    ip += cmd.DestValue;
                }
                break;
            case Instruction::Out:
                v1 = cmd.SrcValue;
                if (cmd.SrcRegisterId >= 0) {
                    v1 = registers[cmd.SrcRegisterId];
                }
                if (v1 < 0 || v1 > 1) { // Must be a 0 or 1
                    return -1;
                }
                if (lastout == -1) {
                    //std::cout << "Trying " << regA << "\n";
                    lastout = v1;
                } else {
                    // Check it's flipped
                    if (lastout == v1) {
                        //std::cout << "Failed: " << regA << "\n";
                        return -1;
                    }
                }
                lastout = v1;
                okcount++;
                //std::cout << v1;
                // At what point do we consider that it is likely to continue forever?
                // We could take a copy of state, I suppose, and see if/when it repeats.
                // Or we could just wait 'til we've had a LOT of characters that follow
                // the correct pattern.
                if (okcount > 5000) // Let's make it 5k
                    return regA;
                ip++;
                break;
            // case Instruction::Tgl: // Day 25 doesn't use 'tgl'
            //     // Find instruction to toggle
            //     v1 = cmd.SrcValue;
            //     if (cmd.SrcRegisterId >= 0) {
            //         v1 = registers[cmd.SrcRegisterId];
            //     }
            //     v1 += ip;
            //     if (v1 < icount) {
            //         commands[v1].Instruction = toggle_instruction(commands[v1].Instruction);
            //     }
            //     ip++;
            //     break;
            default:
                std::cout << "Error processing instruction " << cmd.instruction << std::endl;
                return -1;
        }
    }
    return registers[0];
}