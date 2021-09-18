#pragma once

/**
 * @brief 素数の定義 =「1とそれ自身以外の約数を持たない数」
 * 「2以上n未満のすべての数でnを割り切ることができなければ、nは素数」
 */

// nは素数?
bool is_prime(int n) {
    for (int v = 2; v < n; ++v) {
        if (n % v == 0) {
            return false;
        }
    }
    return true;
}

// lo以上hi未満の範囲にある素数の数を数える
int count_prime(int lo, int hi) {
    int count = 0;
    for (int i = lo; i < hi; ++i) {
        if (is_prime(i))
            count++;
    }
    return count;
}

// 関数形式
void count_prime_function(int lo, int hi, int& result) {
    result = count_prime(lo, hi);
}

// クラス形式 ver1
class count_prime_class_ver1 {
    int& result_;
public:
    count_prime_class_ver1(int& result) : result_(result) {}
    void operator()(int lo, int hi) { result_ = count_prime(lo, hi); }
};

// ↑ ver2
class count_prime_class_ver2 {
public:
    int operator()(int lo, int hi) { return count_prime(lo, hi); }
};


#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

// barrier クラス
// 複数スレッドで待ち合わせをして、最後のスレッドが来たら各スレッドのwaitを解放する.
class barrier {
public:
    barrier(unsigned int count)
        : threshold_(count), count_(count), generation_(0) {
        if (count == 0) {
            throw std::invalid_argument("count cannot be zero.");
        }
    }

    bool wait() {
        std::unique_lock<std::mutex> locker(mutex_);
        bool gen = generation_;
        if (--count_ == 0) {
            generation_++;
            count_ = threshold_;
            condition_.notify_all();
            return true;
        }
        condition_.wait(locker, [&]() { return gen != generation_; });
        return false;
    }

   private:
    std::mutex mutex_;
    std::condition_variable condition_;
    unsigned int threshold_;
    unsigned int count_;
    unsigned int generation_; 
};