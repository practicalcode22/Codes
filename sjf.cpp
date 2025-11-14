#include <iostream>
using namespace std;

struct P {
    int id, at, bt, ft, tat, wt;
    bool done;
};

int main() {
    P p[10];
    int n, t = 0, done = 0;
    float twt = 0, ttat = 0;
    cout << "Enter no. of processes: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        cout << "Enter AT & BT for P" << i + 1 << ": ";
        cin >> p[i].at >> p[i].bt;
        p[i].id = i + 1;
        p[i].done = false;
    }

    while (done < n) {
        int idx = -1, min = 9999;
        for (int i = 0; i < n; i++)
            if (!p[i].done && p[i].at <= t && p[i].bt < min)
                min = p[i].bt, idx = i;
        if (idx == -1) { t++; continue; }

        p[idx].ft = t + p[idx].bt;
        p[idx].tat = p[idx].ft - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
        t = p[idx].ft;
        p[idx].done = true;
        done++;
        twt += p[idx].wt;
        ttat += p[idx].tat;
    }

    cout << "\nPID\tAT\tBT\tFT\tTAT\tWT\n";
    for (int i = 0; i < n; i++)
        cout << p[i].id << "\t" << p[i].at << "\t" << p[i].bt << "\t"
             << p[i].ft << "\t" << p[i].tat << "\t" << p[i].wt << "\n";

    cout << "\nAvg WT: " << twt / n << "\nAvg TAT: " << ttat / n << "\n";
}
