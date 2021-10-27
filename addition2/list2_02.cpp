/**
 * @file list2_02.cpp
 * @author your name (you@domain.com)
 * @brief ミューテックスによるデッドロック
 * @version 0.1
 * @date 2021-10-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <iostream>
#include <thread>
#include <functional>
#include <chrono>
#include <mutex>
using namespace std;

mutex mtxA;
mutex mtxB;

int countA;
int countB;

// データの競合を避けるべくロックで排他したはいいが、今度はロックが競合するというヤレヤレな事態.

void incA() {
    mtxA.lock();   // lock-A
    countA += 1;

    // ---------------- deadlock -----------------------
    // 追加start
    mtxB.lock();   // lock-B
    countB += 1;
    mtxB.unlock(); // unlock-B
    // 追加end

    mtxA.unlock(); // unlock-A
}

void incB() {
    mtxB.lock();   // lock-B
    countB += 1;

    // ---------------- deadlock -----------------------
    // 追加start
    mtxA.lock();   // lock-A
    countA -= 1;
    mtxA.unlock(); // unlock-A
    // 追加end

    mtxB.unlock(); // unlock-B
}

// 複数のmutexをlock()するときはlock順を揃えておく必要がある.
// incA()、incB()ともにmtxA、mtxBの順でlock()すればdeadlockは起こらない.
// mutexのプライオリティ（lock順）は設計時にちゃんと決めておくべきこと.

