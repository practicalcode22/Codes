#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <stdexcept>

using namespace std;

// --- Data Structures ---
// MNT Entry: {MacroName: {NumParams, MDTStart, MDTEnd}}
struct MNTE { int num_params; int mdt_start; int mdt_end; };
map<string, MNTE> MNT; 
map<int, string> MDT_Index; // MDT stored by index for easy lookup

// --- Helper Functions (From Pass I) ---

// Helper: Remove leading/trailing whitespace
string trim(const string& str) {
    size_t f = str.find_first_not_of(" \t\n\r");
    if (string::npos == f) return "";
    size_t l = str.find_last_not_of(" \t\n\r");
    return str.substr(f, (l - f + 1));
}

// Helper: Split string by delimiter and trim (used for actual arguments)
vector<string> split_trim(const string& s, char d) {
    vector<string> t; stringstream ss(s); string token;
    while (getline(ss, token, d)) {
        token = trim(token);
        if (!token.empty()) t.push_back(token);
    }
    return t;
}

// Helper: Tokenize line by whitespace
vector<string> tokenize(const string& line) {
    vector<string> t; stringstream ss(line); string token;
    while (ss >> token) t.push_back(token);
    return t;
}

// Helper: Replace substring (used for # substitution)
string replace_all(string str, const string& from, const string& to) {
    size_t pos = 0;
    while((pos = str.find(from, pos)) != string::npos) {
        str.replace(pos, from.length(), to);
        pos += to.length();
    }
    return str;
}

// --- Loading Functions (Reads Pass I Output) ---

// Load MNT from mnt_file.txt
void load_mnt(const string& filename) {
    ifstream file(filename);
    string line;
    // Skip header line
    getline(file, line); 
    
    while (getline(file, line)) {
        stringstream ss(line);
        string name;
        int num_p, start, end;
        
        // Reading tab-separated values from MNT format: NAME \t #P \t START \t END
        if (ss >> name >> num_p >> start >> end) {
            MNT[name] = { num_p, start, end };
        }
    }
}

// Load MDT from mdt_file.txt
void load_mdt(const string& filename) {
    ifstream file(filename);
    string line;
    
    while (getline(file, line)) {
        stringstream ss(line);
        int index;
        string definition_line;
        
        // MDT format: INDEX \t DEFINITION_LINE
        if (ss >> index) {
            // Read the rest of the line as the definition body (handles spaces in body)
            if (getline(ss, definition_line)) {
                // Trim leading whitespace (the leftover '\t' after reading index)
                definition_line = trim(definition_line);
                MDT_Index[index] = definition_line;
            }
        }
    }
}

// Read Intermediate Code (Lines outside definitions)
vector<string> read_ic_file(const string& filename) {
    ifstream file(filename);
    vector<string> intermediate_code;
    string line;
    
    if (!file.is_open()) { cerr << "Error: Could not open " << filename << endl; return {}; }
    
    while (getline(file, line)) {
        line = trim(line);
        if (!line.empty()) {
            intermediate_code.push_back(line);
        }
    }
    return intermediate_code;
}

// --- Pass II Implementation (Core Logic) ---
void run_pass_two(const vector<string>& intermediate_code, ofstream& output_file) {
    output_file << "--- FINAL EXPANDED CODE ---\n";
    
    for (const string& line : intermediate_code) {
        vector<string> parts = tokenize(line);
        if (parts.empty()) continue;
        string mnemonic = parts[0];
        
        // Check if the line is a MACRO CALL
        if (MNT.count(mnemonic)) {
            // It's a MACRO CALL -> Perform Expansion
            MNTE mnt_entry = MNT[mnemonic];
            
            // 1. Prepare ALA (Argument List Array)
            vector<string> ALA;
            if (parts.size() > 1) {
                // Actual arguments are assumed to be in the second token (e.g., "R1, 10")
                ALA = split_trim(parts[1], ',');
            }
            
            // 2. Start Expansion using MDT
            for (int i = mnt_entry.mdt_start; i < mnt_entry.mdt_end; ++i) {
                string mdt_line = MDT_Index[i];
                string expanded_line = mdt_line;
                
                // Substitute positional parameters (#i) with actual arguments (ALA[i])
                for (int j = 0; j < ALA.size(); ++j) {
                    expanded_line = replace_all(expanded_line, "#" + to_string(j), ALA[j]);
                }
                output_file << expanded_line << "\n";
            }
        } else {
            // Non-macro line -> Pass through
            output_file << line << "\n";
        }
    }
}

// --- Main Execution ---
int main() {
    // 1. Load Pass I output files
    load_mnt("mnt_file.txt");
    load_mdt("mdt_file.txt");
    
    vector<string> ic_result = read_ic_file("ic_file.txt");

    if (ic_result.empty() || MNT.empty()) {
        cerr << "Error: Pass II cannot run. Missing/empty MNT, MDT, or IC files. Ensure Pass I ran correctly." << endl;
        return 1;
    }

    // 2. Execute Pass II and write the final output
    ofstream final_output("final_expanded_code.asm");
    run_pass_two(ic_result, final_output);
    final_output.close();
    
    cout << "Pass II Success: Final expanded code written to final_expanded_code.asm" << endl;

    return 0;
}