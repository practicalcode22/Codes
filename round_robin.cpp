#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath> // For min function
#include <iomanip> // For output formatting

using namespace std;

// --- Data Structure ---
struct Process {
    int pid;
    int at;  // Arrival Time
    int bt;  // Burst Time (Original)
    int rt;  // Remaining Time
    int ct;  // Completion Time
    int tat; // Turnaround Time
    int wt;  // Waiting Time
};

// --- Round Robin Scheduling Implementation ---

int main() {
    int n, tq;
    cout << "Enter number of processes: ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid number of processes. Exiting.\n";
        return 0;
    }

    vector<Process> p(n);
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        cout << "\nProcess P" << p[i].pid << " Arrival Time: ";
        cin >> p[i].at;
        cout << "Burst Time: ";
        cin >> p[i].bt;
        p[i].rt = p[i].bt;
    }

    cout << "\nEnter Time Quantum: ";
    cin >> tq;
    
    // Sort processes by Arrival Time (Critical for handling initial arrivals)
    sort(p.begin(), p.end(), [](auto &a, auto &b) {
        return a.at < b.at;
    });
    
    // Ready Queue stores process indices (0 to n-1)
    queue<int> q;
    int current_time = 0;
    int process_index = 0; // Index to track processes yet to arrive
    int completed = 0;

    // Helper Lambda: Adds all processes that have arrived up to a given time
    auto add_arrivals = [&](int time_limit) {
        while (process_index < n && p[process_index].at <= time_limit) {
            q.push(process_index);
            process_index++;
        }
    };
    
    // Initial load into ready queue
    add_arrivals(current_time);
    
    // --- Scheduling Loop ---
    while (completed < n) {

        if (q.empty()) {
            // CPU is idle. Advance time to the arrival time of the next process.
            if (process_index < n) {
                current_time = p[process_index].at;
                add_arrivals(current_time);
            } else {
                // All processes accounted for, loop should end due to completed == n
                break;
            }
            continue;
        }

        int idx = q.front(); 
        q.pop();

        int run_time = min(tq, p[idx].rt);
        
        p[idx].rt -= run_time;
        current_time += run_time; // Advance time by the quantum or remaining time

        // Check for new arrivals immediately after advancing time
        add_arrivals(current_time);

        if (p[idx].rt == 0) {
            // Process completed
            p[idx].ct = current_time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            completed++;
        } else {
            // Process preempted, push back to the queue (after any new arrivals)
            q.push(idx); 
        }
    }
    // 

    // --- Output Results ---
    cout << "\n\n=== Round Robin Scheduling (Preemptive) ===\n";
    cout << "Time Quantum: " << tq << endl;
    cout << "-----------------------------------------------\n";
    cout << setw(3) << "PID" << setw(6) << "AT" << setw(6) << "BT" 
         << setw(6) << "CT" << setw(6) << "TAT" << setw(6) << "WT" << endl;
    cout << "-----------------------------------------------\n";

    float totalTAT = 0, totalWT = 0;
    for (auto &x : p) {
        cout << setw(3) << x.pid << setw(6) << x.at << setw(6) << x.bt
             << setw(6) << x.ct << setw(6) << x.tat << setw(6) << x.wt << endl;
        totalTAT += x.tat;
        totalWT += x.wt;
    }

    cout << "-----------------------------------------------\n";
    cout << fixed << setprecision(2);
    cout << "Average Turnaround Time = " << totalTAT / n << endl;
    cout << "Average Waiting Time    = " << totalWT / n << endl;

    return 0;
}