#include <iostream>
using namespace std;

int num;
unsigned int seed;

int main() {
  while(cin >> seed >> num && num) {
    for(int i = 0; i < num; i++) {
      cout << ((seed >> 16) & 0x7FFF) << endl;
      seed = seed * 1103515245 + 12345;
    }
  }
}
