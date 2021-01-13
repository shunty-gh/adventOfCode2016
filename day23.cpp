#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// https://adventofcode.com/2016/day/23

/* Takes about a minute or so to run when compiled with optimisation
   or about 2+ minutes in the debugger. Could be speeded up by looking
   at the instructions and determining what it does and working out that
   it could all be replaced by a couple of multiplications and some
   additions etc.
   But that would take much longer than waiting 60s to get the answer.
 */

//#define TEST
#ifdef TEST
    const std::string InputFile{"day23-input-test.txt"};
    const int SeedRegA = 0;
#else
    const std::string InputFile{"day23-input.txt"};
    const int SeedRegA = 7;
#endif

enum class Instruction {
    Instruction_Undefined,
    Copy,
    Inc,
    Dec,
    Jnz,
    Tgl,
};

Instruction string_to_instruction(std::string txt) {
    if (txt == "cpy") return Instruction::Copy;
    if (txt == "inc") return Instruction::Inc;
    if (txt == "dec") return Instruction::Dec;
    if (txt == "jnz") return Instruction::Jnz;
    if (txt == "tgl") return Instruction::Tgl;
    return Instruction::Instruction_Undefined;
}

Instruction toggle_instruction(Instruction instr) {
    switch (instr) {
        case Instruction::Copy:
            return Instruction::Jnz;
        case Instruction::Inc:
            return Instruction::Dec;
        case Instruction::Dec:
        case Instruction::Tgl:
            return Instruction::Inc;
        case Instruction::Jnz:
            return Instruction::Copy;
        default:
            return Instruction::Instruction_Undefined;
    }
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

    auto part1 = run_code(commands, SeedRegA);
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = run_code(commands, 12);
    std::cout << "Part 2: " << part2 << std::endl;
}

int run_code(std::vector<Command> commands, int regA) {
    int ip = 0;
    int icount = commands.size();
    int registers[4]{0};
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
            case Instruction::Tgl:
                // Find instruction to toggle
                v1 = cmd.SrcValue;
                if (cmd.SrcRegisterId >= 0) {
                    v1 = registers[cmd.SrcRegisterId];
                }
                v1 += ip;
                if (v1 < icount) {
                    commands[v1].Instruction = toggle_instruction(commands[v1].Instruction);
                }
                ip++;
                break;
            default:
                std::cout << "Error processing instruction " << cmd.instruction << std::endl;
                return -1;
        }
    }
    return registers[0];
}