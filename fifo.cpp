#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <limits>
#include <queue> // Required for queue

using namespace std;

// Global frame count
int FRAME_COUNT = 0;

// --- Helper Functions ---

void display_results(int faults, int hits, int total) {
    cout << "\n=== FIFO Algorithm Results ===" << endl;
    cout << "Page Frames Size: " << FRAME_COUNT << endl;
    cout << "Page Faults: " << faults << endl;
    cout << "Page Hits: " << hits << endl;
    cout << fixed << setprecision(2);
    cout << "Hit Ratio: " << (float)hits / total << endl;
}

// --- FIFO Implementation ---

void fifo_replacement(const vector<int>& pages) {
    int page_faults = 0;
    int hits = 0;
    vector<int> frames(FRAME_COUNT, -1); // Frames: -1 means empty slot
    queue<int> arrival_order; // Tracks the order pages entered the frames

    cout << "\n--- FIFO Algorithm Simulation ---" << endl;
    
    for (int page : pages) {
        cout << "Request: " << page << " -> ";
        
        // 1. Check for Hit
        auto it = find(frames.begin(), frames.end(), page);
        if (it != frames.end()) {
            hits++;
            cout << "HIT" << endl;
        } else {
            // 2. Page Fault
            page_faults++;
            cout << "FAULT";
            
            // 3. Check for Free Slot
            auto free_it = find(frames.begin(), frames.end(), -1);
            if (free_it != frames.end()) {
                *free_it = page;
                arrival_order.push(page); // Add new page to FIFO queue
                cout << " (Free Slot)";
            } else {
                // 4. Replacement (Find oldest page using the queue)
                int oldest_page = arrival_order.front();
                arrival_order.pop(); // Remove the oldest page from the queue
                
                // Find and replace the oldest page in the frames vector
                auto oldest_it = find(frames.begin(), frames.end(), oldest_page);
                if (oldest_it != frames.end()) {
                    *oldest_it = page;
                }
                arrival_order.push(page); // New page is now the newest
                cout << " (Replace " << oldest_page << ")";
            }
            cout << endl;
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
    cout << "=== FIFO Page Replacement Simulator ===" << endl;
    
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
    
    // Consume the remaining newline character left by cin >> FRAME_COUNT 
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
    fifo_replacement(pages);

    return 0;
}