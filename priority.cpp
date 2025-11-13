#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
struct Process { int id, at, bt, pr, ct, tat, wt; };
int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;
    vector<Process> p(n);
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        cout << "\nP" << p[i].id << " Arrival Time: ";
        cin >> p[i].at;
        cout << "Burst Time: ";
        cin >> p[i].bt;
        cout << "Priority: ";
        cin >> p[i].pr;
    }
    sort(p.begin(), p.end(), [](auto &a, auto &b){ return a.at < b.at; });
    int time = 0, done = 0;
    vector<bool> finished(n, false);
    while (done < n) {
        int idx = -1, best = 1e9;
        for (int i = 0; i < n; i++)
            if (!finished[i] && p[i].at <= time && p[i].pr < best)
                best = p[i].pr, idx = i;
        if (idx == -1) { time++; continue; }
        time += p[idx].bt;
        p[idx].ct = time;
        p[idx].tat = time - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
        finished[idx] = true;
        done++;
    }
    float avgTAT = 0, avgWT = 0;
    cout << "\nPID\tAT\tBT\tP\tCT\tTAT\tWT\n";
    for (auto &x : p) {
        cout << x.id << "\t" << x.at << "\t" << x.bt << "\t" << x.pr << "\t"
             << x.ct << "\t" << x.tat << "\t" << x.wt << "\n";
        avgTAT += x.tat; avgWT += x.wt;
    }
    cout << "\nAverage TAT = " << avgTAT / n;
    cout << "\nAverage WT  = " << avgWT / n << "\n";
}
