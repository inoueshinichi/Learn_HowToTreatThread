/**
 * @file win_api_thread.cpp
 * @author your name (you@domain.com)
 * @brief Win32 thread
 * @version 0.1
 * @date 2021-07-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <Windows.h>
#include <tuple>
#include <vector>
#include <chrono>

#include <prime.hpp>
#include <div_range.hpp>

// <0>:loi, <1>:hi , <1>:result
using thread_io = std::tuple<int, int, int>;

// スレッド用関数(CreateThread関数に渡すためのラッパー関数)
// スレッドで実行したい関数はcount_prime関数であることに注意.
DWORD WINAPI thread_entry(LPVOID arg)
{
    thread_io &io = *static_cast<thread_io *>(arg);
    get<2>(io) = count_prime(get<0>(io), get<1>(io));
    return 0;
}

/*M未満の素数はいくつある?*/
void multi(int M, int nthr)
{
    std::vector<HANDLE> handle(nthr);
    std::vector<thread_io> io(nthr);
    div_range<> rng(2, M, nthr); // [2, M]をnthr等分する

    for (int i = 0; i < nthr; ++i)
    {
        io[i] = thread_io(rng.lo(i), rng.hi(i), 0);
    }

    auto start = chrono::system_clock::now();

    for (int i = 0; i < nthr; ++i)
    {
        // Win API Thread
        haldle[i] = CreateThread(NULL, 0, &thread_entry, &io[i], 0, NULL);   
    }

    WaitForMultipleObjects(nthr, &handle[0], TRUE, INFINITE);

    auto sec = chrono::system_clock::now() - start;

    for (int i = 0; i < nthr; ++i)
    {
        CloseHandle(handle[i]);
        result += get<2>(io[i]);
    }
    cout << result << ' ' << sec.count() << "[sec] : " << nthr << endl;
}


int main(int, char **)
{
    const int N = 100000;
    for (int i = 1; i < 10; ++i)
        multi(M, i);
}

/**
 * @brief 
 * Window-APIで実装する場合、
 * スレッドの本体であるcount_primeをスレッドに乗っけるためのwrapper
 * （このサンプルではthread_ioとthread_entry）が必要になります。
 */