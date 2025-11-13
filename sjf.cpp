#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Process {
    int pid, at, bt, rt, ct, tat, wt;
};

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> p(n);
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        cout << "\nProcess P" << p[i].pid << " Arrival Time: ";
        cin >> p[i].at;
        cout << "Burst Time: ";
        cin >> p[i].bt;
        p[i].rt = p[i].bt;
    }

    // Sort by arrival time
    sort(p.begin(), p.end(), [](auto &a, auto &b){ return a.at < b.at; });

    int completed = 0, time = 0, nprocs = n;

    while (completed < n) {
        int idx = -1, mn = 1e9;

        // Choose process with minimum remaining time so far
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rt > 0 && p[i].rt < mn) {
                mn = p[i].rt;
                idx = i;
            }
        }

        if (idx == -1) { time++; continue; }

        p[idx].rt--;
        time++;

        if (p[idx].rt == 0) {
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt  = p[idx].tat - p[idx].bt;
            completed++;
        }
    }

    cout << "\n\n=== SJF (Preemptive / SRTF) Results ===\n";
    cout << "---------------------------------------------\n";
    cout << "PID  AT  BT  CT  TAT  WT\n";
    cout << "---------------------------------------------\n";

    float total_tat = 0, total_wt = 0;
    for (auto &x : p) {
        cout << x.pid << "   " << x.at << "   " << x.bt 
             << "   " << x.ct << "   " << x.tat << "    " << x.wt << endl;
        total_tat += x.tat;
        total_wt += x.wt;
    }

    cout << "---------------------------------------------\n";
    cout << "Avg TAT = " << total_tat / n << endl;
    cout << "Avg WT  = " << total_wt / n << endl;
}
