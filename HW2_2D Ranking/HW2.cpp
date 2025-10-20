#include <iostream>
#include <algorithm>
using namespace std;

int main() {
    fopen("test2.txt", "r");

    float x, y;
    while (cin >> x >> y) {
        cout << x << " "<< y;
    }
    return 0;
}