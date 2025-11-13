#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

int main() {
    int framesCount;
    cout << "Enter number of frames: ";
    cin >> framesCount;

    int n;
    cout << "Enter number of page requests: ";
    cin >> n;

    vector<int> pages(n);
    cout << "Enter page reference string:\n";
    for (int i = 0; i < n; i++) cin >> pages[i];

    vector<int> frames(framesCount, -1);
    vector<int> lastUsed(framesCount, -1);

    int pageFaults = 0, hits = 0, time = 0;

    cout << "\n--- LRU Page Replacement Simulation ---\n";
    for (int p : pages) {
        cout << "Request: " << p << " -> ";

        auto it = find(frames.begin(), frames.end(), p);

        if (it != frames.end()) {              // HIT
            hits++;
            lastUsed[it - frames.begin()] = time;
            cout << "HIT\n";
        } else {                               // FAULT
            pageFaults++;
            cout << "FAULT";

            // Check for empty frame
            auto free_it = find(frames.begin(), frames.end(), -1);
            if (free_it != frames.end()) {
                int idx = free_it - frames.begin();
                frames[idx] = p;
                lastUsed[idx] = time;
                cout << " (Free Slot)\n";
            } else {
                // Replace LRU (minimum lastUsed value)
                int lruIndex = min_element(lastUsed.begin(), lastUsed.end()) - lastUsed.begin();
                cout << " (Replace " << frames[lruIndex] << ")\n";
                frames[lruIndex] = p;
                lastUsed[lruIndex] = time;
            }
        }

        // Display frames
        cout << "Frames: | ";
        for (int f : frames)
            cout << (f == -1 ? "-" : to_string(f)) << " | ";
        cout << endl;

        time++;
    }

    cout << "\n=== LRU Results ===\n";
    cout << "Total Requests: " << n << endl;
    cout << "Page Faults: " << pageFaults << endl;
    cout << "Page Hits: " << hits << endl;
    cout << "Hit Ratio: " << fixed << setprecision(2)
         << (float)hits / n << endl;

    return 0;
}
