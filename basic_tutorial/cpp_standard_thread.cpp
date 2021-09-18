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
#include <future>
#include <chrono>
#include <iostream>

#include <prime.hpp>
#include <div_range.hpp>

using namespace std;


// スレッドでタスクを分割する
void multi_case1(int M, int nthr) 
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


// ファンクタ形式ならなんでもタスクにできる
void multi_case2(int M) {
    thread thr[3];
    int count[3];
    div_range<> rng(2, M, 3);

    auto count_prime_lambda = [&](int lo, int hi) { count[2] = count_prime(lo, hi); };

    chrono::system_clock::time_point start = chrono::system_clock::now();

    // 関数ポインタ, クラスインスタンス, lambda式からスレッドを作る
    thr[0] = thread(count_prime_function, rng.lo(0), rng.hi(0), ref(count[0]));
    thr[1] = thread(count_prime_class_ver1(count[1]), rng.lo(1), rng.hi(1));
    thr[2] = thread(count_prime_lambda, rng.lo(2), rng.hi(2));
    for (thread& t : thr) t.join();
    chrono::duration<double> sec = chrono::system_clock::now() - start;

    cout << count[0] + count[1] + count[2] << ' '
         << sec.count() << "[sec]" << endl;
}

// async/future
void multi_case3(int M) {
    future<int> fut[3];
    div_range<> rng(2, M, 3);

    auto count_prime_lambda = [&](int lo, int hi) { return count_prime(lo, hi); };

    chrono::system_clock::time_point start = chrono::system_clock::now();
    // 関数ポインタ, クラスインスタンス, lambda式からfutureを作るcount_prime_class_ver2
    fut[0] = async(launch::async, count_prime, rng.lo(0), rng.hi(0));
    fut[1] = async(launch::async, count_prime_class_ver2(), rng.lo(1), rng.hi(1));
    fut[2] = async(launch::async, count_prime_lambda, rng.lo(2), rng.hi(2));
    int result = fut[0].get() + fut[1].get() + fut[2].get();
    chrono::duration<double> sec = chrono::system_clock::now() - start;

    cout << result << ' ' << sec.count() << "[sec]" << endl;
}

/* async
この関数は、パラメータpolicyで指定された実行ポリシーの値によって振る舞いを変える。

policyを指定しない場合はlaunch::async | launch::deferredとなり、どちらの実行ポリシーが選択されるかは実装定義となる。

各実行ポリシーの振る舞いは以下のようになる：

policy & launch::asyncが0でない場合、新たなスレッドで関数オブジェクトfにargs...を渡して実行する
( INVOKE(DECAY_COPY(std::forward<F>(f)), DECAY_COPY(std::forward<Args>(args))...) )
関数オブジェクトfの戻り値が、この関数の戻り値であるfutureオブジェクトとの共有状態に書き込まれる。
関数オブジェクトfの内部で例外が投げられた場合は、共有状態に投げられた例外が設定される。
policy & launch::deferredが0でない場合、関数オブジェクトfをその場では実行せず、遅延状態にする
(DECAY_COPY(std::forward<F>(f))とDECAY_COPY(std::forward<Args>(args))...をfutureオブジェクトとの共有状態に格納する)。
この関数の戻り値であるfutureオブジェクトのget()もしくはwait()が呼び出されるタイミングで、関数オブジェクトfにargs...を渡して実行する。
有効な実行ポリシーが指定されていない場合(整数値をlaunch型にキャストするような状況)、その動作は未定義(C++14)。
*/

// mutex/atomic
#define VALID_EXCLUSIVE
void multi_case4(int M, int nthr) {
    vector<thread> thr(nthr);
    div_range<> rng(2, M, nthr);

#ifndef VALID_EXCLUSIVE
    int result = 0; // データ競合発生中
    atomic<int> result  = 0; // ビルドイン型やポインタ型に対する++,--,+=,-=などの極めて短い時間でスレッド割り込みを抑制するには,atomicがおすすめ
#else
    mutex mtx;
    int result = 0;
#endif

    chrono::system_clock::time_point start = chrono::system_clock::now();
    for (int t = 0; t < nthr; ++t) {
        thr[t] = thread([&](int lo, int hi) {
            for (int n = lo; n < hi; ++n) {
                // nが素数なら resultに1を加える
                if (is_prime(n)) {
#ifdef VALID_EXCLUSIVE
                    lock_guard<mutex> locker(mtx);
#endif
                    ++result;
                }
            }
        },
                        rng.lo(t), rng.hi(t));
    }

    for (thread& th : thr) {
        th.join();
    }
    chrono::duration<double> sec = chrono::system_clock::now() - start;

    cout << result << ' ' << sec.count() << "[sec] : " << nthr << endl;
}

// condition variable
void multi_case5(int M, int nthr) {
    vector<thread> thr(nthr);
    div_range<> rng(2, M, nthr);
    condition_variable cond;
    int finished = 0;
    atomic<int> result = 0;
    mutex mtx;

    chrono::system_clock::time_point start = chrono::system_clock::now();
    for (int t = 0; t < nthr; ++t) {
        thr[t] = thread([&](int lo, int hi) {
            for (int n = lo; n < hi; ++n) {
                if (is_prime(n)) ++result;
            }
            lock_guard<mutex> guard(mtx);
            ++finished;
            cond.notify_one();
        },
                        rng.lo(t), rng.hi(t));
    }
    unique_lock<mutex> lock(mtx);
    // 全スレッドが++finishedすることでfinished==nthrとなるのを待つ
    cond.wait(lock, [&]() { // wait()は, worker threadからnotify_one()が通知されるたびに実行される.
        cout << "check" << endl;
        return finished == nthr; 
        });
    chrono::duration<double> sec = chrono::system_clock::now() - start;

    cout << result << ' ' << sec.count() << "[sec] : " << nthr << endl;
    for (thread& th : thr) {
        th.join();
    }
}


// barrier
void multi_case6(int M, int nthr) {
    vector<thread> thr(nthr);
    div_range<> rng(2, M, nthr);
    atomic<int> result = 0;
    barrier quit(nthr + 1); // worker thread + main thread

    chrono::system_clock::time_point start = chrono::system_clock::now();

    for (int t = 0; t < nthr; ++t) {
        thr[t] = thread([&](int lo, int hi) {
            for (int n = lo; n < hi; ++n) {
                if (is_prime(n)) ++result;
            }
            quit.wait();
        },
        rng.lo(t), rng.hi(t));
    }
    quit.wait();  // 全スレッドが wait するまで待つ
    chrono::duration<double> sec = chrono::system_clock::now() - start;

    cout << result << ' ' << sec.count() << "[sec] : " << nthr << endl;
    for (thread& th : thr) {
        th.join();
    }
}

int main(int, char **)
{
    const int M = 100000;

    /* --- スレッドでタスクを分割する --- */
    // {
    //     cout << "Case1" << endl;
    //     for (int i = 1; i < 10; ++i)
    //         multi_case1(M, i);
    // }

    /* --- ファンクタ形式ならなんでもタスクにできる --- */
    // {
    //     cout << "Case2" << endl;
    //     multi_case2(M);
    // }

    /* --- async/future --- */
    // {
    //     cout << "Case3" << endl;
    //     multi_case3(M);
    // }

    /* --- mutex/atomic --- */
    // {
    //     cout << "Case4" << endl;
    //     for (int i = 1; i < 20; ++i)
    //         multi_case1(M, i);
    // }

    /* -- condition variable --- */
    // {
    //     cout << "Case5" << endl;
    //     multi_case5(M, 8);
    // }

    /* --- barrier --- */
    {
        cout << "Case6" << endl;
        multi_case6(M, 10);
    }
}