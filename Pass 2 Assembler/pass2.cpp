#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>

using namespace std;

// --- Data Structures (Shared with Pass I) ---
struct ICLine { int loc; string type; int opcode; string operand; };

// Define REG (Constant, needed for register codes)
map<string, int> REG = {{"AREG", 1}, {"BREG", 2}, {"CREG", 3}};

// SYMTAB - Will be loaded from file
map<string, int> SYMTAB;

// --- Helper Functions ---

// Loads the Symbol Table (SYMTAB) from symtab.txt
void load_symtab(const string& filename) {
    ifstream file(filename);
    string line;
    
    while (getline(file, line)) {
        stringstream ss(line);
        string symbol_name;
        int address;
        
        // SYMTAB format: SYMBOL \t ADDRESS
        if (ss >> symbol_name >> address) {
            SYMTAB[symbol_name] = address;
        }
    }
}

// Reads Intermediate Code (IC) lines from ic_file.txt
vector<ICLine> read_ic_file(const string& filename) {
    ifstream file(filename);
    vector<ICLine> ic_lines;
    string line;

    if (!file.is_open()) { cerr << "Error: Could not open " << filename << endl; return {}; }

    while (getline(file, line)) {
        stringstream ss(line);
        ICLine ic_line;
        string type_str;
        
        // IC file format: LOC \t TYPE \t OPCODE \t OPERAND
        // Reading LOC, TYPE, OPCODE, OPERAND (tab-separated)
        if (ss >> ic_line.loc >> ic_line.type >> ic_line.opcode >> ic_line.operand) {
             ic_lines.push_back(ic_line);
        } else if (ss.fail()) {
            // Handle case where operand is missing (e.g., END, START directives)
            ss.clear();
            ss.seekg(0, ios::beg);
            if (ss >> ic_line.loc >> ic_line.type >> ic_line.opcode) {
                ic_line.operand = "NONE"; 
                ic_lines.push_back(ic_line);
            }
        }
    }
    return ic_lines;
}

// --- Pass II Implementation ---
void run_pass_two(const vector<ICLine>& intermediate_code, ofstream& output_file) {
    output_file << "--- PASS II OUTPUT (Machine Code) ---\n";
    output_file << "LOC\tMACHINE CODE\n";
    
    // Set up output formatting for hex address and zero padding
    output_file << hex << uppercase << setfill('0'); 

    for (const auto& line : intermediate_code) {
        output_file << setw(4) << line.loc << "\t"; // Write Location Counter

        if (line.type == "IS") {
            // 1. Process Imperative Statement (IS)
            
            // Logic to parse Operand (e.g., "BREG, A" or just "A")
            vector<string> parts; 
            stringstream ss(line.operand); 
            string token;
            while(getline(ss, token, ',')) parts.push_back(token);
            
            string reg_name, symbol_name;
            if (parts.size() == 2) { 
                reg_name = parts[0]; // e.g., BREG
                symbol_name = parts[1]; // e.g., A
            } else { 
                reg_name = "0"; // Default register code
                symbol_name = parts[0]; 
            }
            
            // Lookups: Get Register Code and Symbol Address
            int reg_code = REG.count(reg_name) ? REG.at(reg_name) : 0;
            
            string addr;
            if (SYMTAB.count(symbol_name)) {
                addr = to_string(SYMTAB.at(symbol_name)); // Found in SYMTAB
            } else {
                addr = "??"; // Undefined symbol (Error case, should not happen if Pass I was clean)
            }
            
            // Format: (Opcode) (Reg Code) (Address)
            output_file << "(" << setw(2) << line.opcode << ") (" << reg_code << ") (" << addr << ")\n";
        
        } else if (line.type == "DL") {
            // 2. Process Data Declaration (DL) - No executable code
            output_file << "(00) (0) (" << line.operand << ")\n"; 
        } else if (line.type == "AD") {
            // 3. Process Assembler Directive (AD) - No executable code
            string directive = (line.opcode == 1) ? "START" : (line.opcode == 2) ? "END" : line.operand;
            output_file << " (AD) (" << directive << ")\n";
        }
    }
    output_file << dec; // Resetting to decimal
}

// --- Main Execution ---
int main() {
    // 1. Load data from Pass I output files
    load_symtab("symtab.txt");
    vector<ICLine> ic_result = read_ic_file("ic_file.txt");

    if (ic_result.empty() || SYMTAB.empty()) {
        cerr << "Error: Pass II cannot run. Missing/empty symtab.txt or ic_file.txt. Run Pass I first!" << endl;
        return 1;
    }

    // 2. Execute Pass II and write Machine Code
    ofstream mc_file("machine_code.txt");
    run_pass_two(ic_result, mc_file);
    mc_file.close();
    cout << "Pass II Success: Machine Code written to machine_code.txt" << endl;

    return 0;
}