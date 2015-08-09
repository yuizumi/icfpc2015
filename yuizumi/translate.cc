#include <iostream>
#include <string>

#include "basic.h"

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int size;
    cin >> size;
    cout << size << endl;
    for (int i = 0; i < size; i++) {
        int id, seed;
        string seq;
        cin >> id >> seed >> seq;
        cout << id << " " << seed << endl << seq << endl;
    }
    return 0;
}
