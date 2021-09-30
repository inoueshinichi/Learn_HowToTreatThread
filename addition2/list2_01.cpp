
#include <iostream>
#include <thread>
#include <functional>
#include <chrono>
#include <mutex>

// data-race が発生するハダカの足し算
void add(long* address, long value) {
  *address += value;
}

void run(std::function<void(long*, long)> fun, long* address) {
    using namespace std;
    using namespace std::chrono;

    auto task = [=](int n, long v) { while (n--) fun(address, v); };
    const int N = 100000;
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
    
    return 0;
}