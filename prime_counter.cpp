#include <prime.hpp>

#include <iostream>
#include <chrono>

using namespace std;

// M未満の素数はいくつあるか
int single(int M) {
    auto start = chrono::high_resolution_clock::now();
    int count = count_prime(2, M);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> sec = end - start;
    cout << count << " " << sec.count() << "[sec]" << endl;
    return count;
}

int main(int, char**) {
    const int M = 100000;
    single(M);
    return 0;
}