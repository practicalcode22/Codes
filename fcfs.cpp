#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Process {
    int pid, at, bt, ct, tat, wt;
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
    }

    sort(p.begin(), p.end(), [](auto &a, auto &b){ return a.at < b.at; });

    int time = 0;
    for (auto &x : p) {
        if (time < x.at) time = x.at;        // CPU idle time handling
        time += x.bt;
        x.ct = time;
        x.tat = x.ct - x.at;
        x.wt = x.tat - x.bt;
    }

    cout << "\n\n=== FCFS Scheduling Results ===\n";
    cout << "---------------------------------------------\n";
    cout << "PID  AT  BT  CT  TAT  WT\n";
    cout << "---------------------------------------------\n";

    float ttat = 0, twt = 0;
    for (auto &x : p) {
        cout << x.pid << "   " << x.at << "   " << x.bt
             << "   " << x.ct << "   " << x.tat
             << "    " << x.wt << endl;
        ttat += x.tat;
        twt += x.wt;
    }

    cout << "---------------------------------------------\n";
    cout << "Avg TAT = " << ttat / n << endl;
    cout << "Avg WT  = " << twt / n << endl;
}
