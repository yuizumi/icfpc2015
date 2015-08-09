#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "basic.h"

using namespace std;

vector<Command> Parse(const string& seq) {
    vector<Command> commands;
    for (char ch : seq) {
        commands.push_back(static_cast<Command>(ch - '0'));
    }
    return commands;
}

string Build(const vector<Command>& commands) {
    const map<Command, char> command2char = {
        {kNone, '\t'},
        {kMoveE, 'e'},
        {kMoveW, '!'},
        {kMoveSE, 'n'},
        {kMoveSW, 'i'},
        {kRotateRight, 'v'},  // Clockwise.
        {kRotateLeft, 'x'},   // Counter-clockwise.
    };

    string seq(commands.size(), '\0');
    for (size_t i = 0; i < commands.size(); i++) {
        seq[i] = command2char.find(commands[i])->second;
    }
    return seq;
}

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
        cout << id << " " << seed << endl << Build(Parse(seq)) << endl;
    }
    return 0;
}
