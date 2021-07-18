/**
 * @file cpp_standard_thread.cpp
 * @author your name (you@domain.com)
 * @brief C++11以降で実行できるスレッドライブラリ
 * @version 0.1
 * @date 2021-07-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

#include <prime.hpp>
#include <div_range.hpp>

using namespace std;

void multi(int M, int nthr) 
{
    vector<thread> thr(nthr);
    vector<int> count(nthr);
    div_range<> rng(2, M, nthr);

    auto start = chrono::system_clock::now();

    for (int i = 0; i < nthr; ++i)
    {
        thr[i] = thread([&, i](int lo, int hi)
                        { count[i] = count_prime(lo, hi); },
                        rng.lo(i), rng.hi(i));
    }

    int result = 0;
    for (int i = 0; i < nthr; ++i)
    {
        thr[i].join();
        result += count[i];
    }

    chrono::duration<double> sec = chrono::system_clock::now() - start;
    cout << result << ' ' << sec.count() << "[sec] : " << nthr << endl;
}

int main(int, char **)
{
    const int M = 100000;
    for (int i = 1; i < 10; ++i)
        multi(M, i);
}