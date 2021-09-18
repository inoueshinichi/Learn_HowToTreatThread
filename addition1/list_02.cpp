/**
 * @file list_02.cpp
 * @author your name (you@domain.com)
 * @brief 異常終了
 * @version 0.1
 * @date 2021-09-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <chrono>
#include <exception>
#include <iostream>
#include <string>
#include <thread>

void bad_termination() {
    std::cerr << "異常終了!!\n";
}

int main() {
    using namespace std;

    std::set_terminate(bad_termination);
    cout << "main:\n";
    thread thr([](int n) {
        cout << "lambda_exp: " + to_string(n) + " 秒後に終了します...\n";
        this_thread::sleep_for(chrono::seconds(n));
        cout << "lambda_exp: おしまい\n";
    },
               2);
    cout << "main:おしまい\n";
    // 異常終了を避けたいなら thr.join() : 完了待ち
    //            さもなくば thr.detach() : 親権放棄 すべし
}