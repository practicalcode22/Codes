#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <stdexcept>

using namespace std;

// --- Data Structures ---
struct OpInfo { string type; int opcode; int length; };
struct ICLine { int loc; string type; int opcode; string operand; };

map<string, OpInfo> OPTAB = {
    {"START", {"AD", 1, 0}}, {"END", {"AD", 2, 0}}, 
    {"MOVER", {"IS", 1, 4}}, {"ADD", {"IS", 2, 4}}, {"SUB", {"IS", 3, 4}},
    {"DS",    {"DL", 1, 0}}, {"DC",  {"DL", 2, 0}}
};
map<string, int> REG = {{"AREG", 1}, {"BREG", 2}, {"CREG", 3}};
map<string, int> SYMTAB;

// Helper: Tokenize a line (Condensed)
vector<string> tokenize(const string& line) {
    vector<string> tokens; string token; stringstream ss(line);
    while (ss >> token) tokens.push_back(token);
    if (tokens.empty()) return {};

    // Return [Label, Mnemonic, Operand]
    if (tokens.size() == 3) return tokens; 
    // Return ["NONE", Mnemonic, Operand]
    if (tokens.size() == 2) return {"NONE", tokens[0], tokens[1]};
    return {};
}

// --- Pass I Implementation (Condensed) ---
vector<ICLine> run_pass_one(const string& filename) {
    ifstream infile(filename); 
    vector<ICLine> intermediate_code; 
    int LOC = 0;
    
    if (!infile.is_open()) { cerr << "Error: Could not open " << filename << endl; return {}; }

    string line;
    while (getline(infile, line)) {
        if (line.empty() || line.find_first_not_of(" \t\n\r") == string::npos) continue;

        vector<string> tokens = tokenize(line);
        if (tokens.empty()) continue;

        string label = tokens[0], mnemonic = tokens[1], operand = tokens[2];
        if (OPTAB.find(mnemonic) == OPTAB.end()) continue;

        OpInfo info = OPTAB.at(mnemonic);

        try {
            if (mnemonic == "START") {
                LOC = stoi(operand);
                intermediate_code.push_back({LOC, info.type, info.opcode, operand}); continue;
            }

            if (label != "NONE") SYMTAB[label] = LOC;
            intermediate_code.push_back({LOC, info.type, info.opcode, operand});

            if (info.type == "IS") LOC += info.length;
            else if (info.type == "DL") {
                LOC += (mnemonic == "DS") ? stoi(operand) : 1; // Condensed DL update
            }
        } catch (const exception& e) {
            cerr << "Runtime Error in Pass I: Invalid operand (" << operand << ") at line: " << line << endl;
            return {};
        }

        if (mnemonic == "END") break;
    }

    return intermediate_code;
}

// --- Main Execution (Condensed) ---
int main() {
    vector<ICLine> ic_result = run_pass_one("source_code.asm");

    if (ic_result.empty() && SYMTAB.empty()) {
        cerr << "Pass I failed. Check source_code.asm content or file access." << endl;
        return 1;
    }

    cout << "--- PASS I EXECUTION COMPLETE ---" << endl;

    // Output Generation (Combined streams for brevity)
    ofstream ic_file("ic_file.txt");
    ofstream symtab_file("symtab.txt");
    
    for (const auto& line : ic_result) {
        ic_file << line.loc << "\t" << line.type << "\t" << line.opcode << "\t" << line.operand << "\n";
    }
    ic_file.close();
    cout << "Intermediate Code written to ic_file.txt (" << ic_result.size() << " lines)" << endl;

    for (const auto& pair : SYMTAB) {
        symtab_file << pair.first << "\t" << pair.second << "\n";
    }
    symtab_file.close();
    cout << "Symbol Table written to symtab.txt (" << SYMTAB.size() << " entries)" << endl;

    return 0;
}