#include <iostream>
using namespace std;

int main() {
    int n, f;
    cout << "Enter number of pages: ";
    cin >> n;

    int pages[n];
    cout << "Enter pages: ";
    for(int i=0;i<n;i++) cin >> pages[i];

    cout << "Enter number of frames: ";
    cin >> f;

    int frames[f];
    int faults = 0, index = 0;

    for(int i=0;i<f;i++) frames[i] = -1;

    cout << "\nFIFO Page Replacement\n";
    for(int i=0;i<n;i++) {
        bool found = false;

        for(int j=0;j<f;j++)
            if(frames[j] == pages[i])
                found = true;

        if(!found) {
            frames[index] = pages[i];
            index = (index + 1) % f;
            faults++;
        }

        cout << "Page " << pages[i] << " -> ";
        for(int j=0;j<f;j++) cout << frames[j] << " ";
        cout << endl;
    }

    cout << "\nTotal Faults = " << faults;
    return 0;
}
//7
//1 3 0 3 5 6 3
//3
