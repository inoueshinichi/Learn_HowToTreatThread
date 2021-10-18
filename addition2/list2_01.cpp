
#include <iostream>
#include <thread>
#include <functional>
#include <chrono>
#include <mutex>

// data-race が発生するハダカの足し算
void add(long* address, long value) {
  *address += value;
}


// mutexを用いたお利口さんな足し算
std::mutex mtx;
void mutex_add(long* address, long value) {
  mtx.lock();
  *address += value;
  mtx.unlock();
}

/**
 * @brief クリティカルセクションでの例外処理
 * lock()とunlock()で挟まれた処理の中で例外がthrowされると困ったことになります。
 * 例外で処理が中断するとunlock()に達しないのでロックが解放されず、他のスレッドが動けなくなっちゃいます。
 * → std::lock_guardまたはstd::unique_lock を使う.
 */

void run(std::function<void(long*, long)> fun, long* address) {
    using namespace std;
    using namespace std::chrono;

    auto task = [=](int n, long v) { while (n--) fun(address, v); };
    const int N = 100000; // 10万
    thread threads[4];
    auto start = high_resolution_clock::now();

    // スレッドを4本起こして
    threads[0] = thread(task, N,  1L);
    threads[1] = thread(task, N, -1L);
    threads[2] = thread(task, N,  2L);
    threads[3] = thread(task, N, -2L);

    // 全スレッド終了を待つ
    for (thread &thr : threads) thr.join();
    auto stop = high_resolution_clock::now();

    cout << duration_cast<milliseconds>(stop - start).count() << "[ms]" << flush; 
}

int main(int, char**) {
    using namespace std;

    long count = 0L;

    cout << "data-race:            ";
    count = 0; 
    run(add, &count); 
    cout << ", count = " << count << endl;

    cout << "critical-section:     ";
    count = 0;
    run(mutex_add, &count);
    cout << ", count = " << count << endl;
    
    return 0;
}