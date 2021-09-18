/**
 * @file list_04.cpp
 * @author your name (you@domain.com)
 * @brief promise/futureで結果を引き取る
 * @version 0.1
 * @date 2021-09-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <future>  // promise/future
#include <iostream>
#include <thread>
#include <utility>  // move

/*
 * 0+1+...+n を求め result にセットする
 *  promiseを介した（イケてる）方法
 */
void sigma_promise(int n, std::promise<int> result) {
    int sum = 0;
    for (int i = 0; i <= n; ++i) {
        sum += i;
    }
    result.set_value(sum);
}

int main() {
    using namespace std;

    promise<int> prm;
    future<int> result = prm.get_future();
    // ※ promiseはコピーできないため、std::moveで移動する
    thread thr(sigma_promise, 10, move(prm));
    // 後続するget()がスレッドの終了を待ってくれるので親権放棄して構わない。
    thr.detach();

    // 結果は future<T>::get() で受け取る
    cout << "イケてる結果: " << result.get() << endl;
}