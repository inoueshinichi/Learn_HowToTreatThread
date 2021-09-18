/**
 * @file list_06.cpp
 * @author your name (you@domain.com)
 * @brief async()で着火し、結果を引き取る (裏でpakcaged_task/future)
 * @version 0.1
 * @date 2021-09-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <future>  // async/future
#include <iostream>
#include <thread>

/*
 * 0+1+...+n を返す
 */
int sigma(int n) {
    int sum = 0;
    for (int i = 0; i <= n; ++i) {
        sum += i;
    }
    return sum;
}

int main() {
    using namespace std;

    // asyncでスレッドを起動し、さらにfutureを手に入れる
    future<int> result = async(sigma, 10);

    // 結果は future<T>::get() で受け取る
    cout << "0+1+...+n = " << result.get() << endl;
}