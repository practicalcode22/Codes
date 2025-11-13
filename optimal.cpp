#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <limits> 

using namespace std;

// Global frame count
int FRAME_COUNT = 0;

// --- Helper Functions ---

// Finds the page currently in frames that won't be used for the longest time in the future.
int find_optimal_page(const vector<int>& frames, const vector<int>& pages, int current_index) {
    int page_to_replace = -1;
    // Max distance is initialized low. We are looking for the highest possible next_use_index.
    int max_distance = -1; 

    // Iterate through every page currently loaded in the frame array
    for (int page : frames) {
        // Assume page is never used again (maximum possible distance: the end of the string)
        int next_use_index = pages.size(); 
        
        // Search the future reference string (from current_index + 1 onwards)
        for (int i = current_index + 1; i < pages.size(); ++i) {
            if (pages[i] == page) {
                next_use_index = i; // Found the next time this page is needed
                break;
            }
        }
        
        // Update selection if this page's next use is farther away (higher index).
        // If next_use_index == max_distance (a tie), the page encountered FIRST in the frame iteration wins (FIFO tie-breaker).
        if (next_use_index > max_distance) {
            max_distance = next_use_index;
            page_to_replace = page;
        }
    }
    return page_to_replace;
}

void display_results(int faults, int hits, int total) {
    cout << "\n=== Optimal Algorithm Results ===" << endl;
    cout << "Page Frames Size: " << FRAME_COUNT << endl;
    cout << "Page Faults: " << faults << endl;
    cout << "Page Hits: " << hits << endl;
    cout << fixed << setprecision(2);
    cout << "Hit Ratio: " << (float)hits / total << endl;
}

// --- Optimal Replacement Implementation ---

void optimal_replacement(const vector<int>& pages) {
    int page_faults = 0;
    int hits = 0;
    vector<int> frames(FRAME_COUNT, -1); 

    cout << "\n--- Optimal Algorithm Simulation ---" << endl;
    
    for (int i = 0; i < pages.size(); ++i) {
        int page = pages[i];
        
        // 1. Check for Hit
        auto it = find(frames.begin(), frames.end(), page);
        if (it != frames.end()) {
            hits++;
            cout << "Request: " << page << " -> HIT" << endl;
        } else {
            // 2. Page Fault
            page_faults++;
            cout << "Request: " << page << " -> FAULT";
            
            // 3. Find Free Slot
            auto free_it = find(frames.begin(), frames.end(), -1);
            if (free_it != frames.end()) {
                *free_it = page;
                cout << " (Free Slot)" << endl;
            } else {
                // 4. Replacement (Find the page to replace by looking ahead)
                int page_to_replace = find_optimal_page(frames, pages, i);
                
                // Replace the selected page
                auto replace_it = find(frames.begin(), frames.end(), page_to_replace);
                if (replace_it != frames.end()) {
                    *replace_it = page;
                }
                cout << " (Replace " << page_to_replace << ")" << endl;
            }
        }
        // Display frame state after each request
        cout << "Frames: ";
        for(int f : frames) cout << f << " ";
        cout << endl;
    }
    display_results(page_faults, hits, pages.size());
}

// --- Main Function and Input Handling ---

int main() {
    cout << "=== Optimal Page Replacement Simulator ===" << endl;
    
    // Input for Frames
    cout << "Enter the number of Page Frames (M): ";
    if (!(cin >> FRAME_COUNT) || FRAME_COUNT <= 0) {
        cout << "Invalid frames entered. Exiting." << endl;
        return 1;
    }

    // Input for Reference String
    cout << "Enter the page reference string (e.g., 7 0 1 2 0 3 0 4 2 3):" << endl;
    
    vector<int> pages;
    int page_ref;
    string line;
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << ">> ";
    if (!getline(cin, line)) {
        cout << "Error reading input line. Exiting." << endl;
        return 1;
    }
    
    stringstream ss(line);
    while (ss >> page_ref) {
        pages.push_back(page_ref);
    }

    if (pages.empty()) {
        cout << "No page references entered. Exiting." << endl;
        return 1;
    }

    // Execute
    optimal_replacement(pages);

    return 0;
}