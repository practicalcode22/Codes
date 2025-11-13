#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Process {
    int pid, at, bt, pr, ct, tat, wt;
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
        cout << "Priority: ";
        cin >> p[i].pr;
    }

    sort(p.begin(), p.end(), [](auto &a, auto &b){ return a.at < b.at; });

    int time = 0, completed = 0;
    vector<bool> done(n, false);

    while (completed < n) {
        int idx = -1, best = 1e9;

        // Pick highest priority among arrived processes
        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].at <= time && p[i].pr < best) {
                best = p[i].pr;
                idx = i;
            }
        }

        if (idx == -1) { time++; continue; }

        time += p[idx].bt;
        p[idx].ct  = time;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt  = p[idx].tat - p[idx].bt;
        done[idx] = true;
        completed++;
    }

    cout << "\n\n=== Priority Scheduling (Non-Preemptive) ===\n";
    cout << "---------------------------------------------\n";
    cout << "PID  AT  BT  P  CT  TAT  WT\n";
    cout << "---------------------------------------------\n";

    float ttat = 0, twt = 0;
    for (auto &x : p) {
        cout << x.pid << "   " << x.at << "   " << x.bt
             << "   " << x.pr << "   " << x.ct
             << "   " << x.tat << "    " << x.wt << endl;

        ttat += x.tat;
        twt += x.wt;
    }

    cout << "---------------------------------------------\n";
    cout << "Avg TAT = " << ttat / n << endl;
    cout << "Avg WT  = " << twt / n << endl;
}
