#include <stdio.h>
typedef struct {
    int id, at, bt, ft, tat, wt, done;
} P;
int main() {
    P p[10];
    int n, t = 0, done = 0;
    float twt = 0, ttat = 0;
    printf("Enter no. of processes: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        printf("Enter AT & BT for P%d: ", i + 1);
        scanf("%d%d", &p[i].at, &p[i].bt);
        p[i].id = i + 1;
        p[i].done = 0;
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
        p[idx].done = 1;
        done++;
        twt += p[idx].wt;
        ttat += p[idx].tat;
    }
    printf("\nPID\tAT\tBT\tFT\tTAT\tWT\n");
    for (int i = 0; i < n; i++)
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].id, p[i].at, p[i].bt, p[i].ft, p[i].tat, p[i].wt);
    printf("\nAvg WT: %.2f\nAvg TAT: %.2f\n", twt / n, ttat / n);
}
