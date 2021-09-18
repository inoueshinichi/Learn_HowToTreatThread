/**
 * @file list_07.cpp
 * @author your name (you@domain.com)
 * @brief 例外をつかまえそこねる
 * @version 0.1
 * @date 2021-09-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <functional>  // ref
#include <iostream>
#include <stdexcept>  // invalid_argument
#include <string>     // to_string
#include <thread>

/*
 * 0+1+...+n を求め resultにセットする
 * n < 0 のときは invalid_argument 例外をthrowする
 */
void sigma_reference(int n, int& result) {
    if (n < 0) {
        throw std::invalid_argument(std::to_string(n) + " は勘弁して");
    }
    int sum = 0;
    for (int i = 0; i <= n; ++i) {
        sum += i;
    }
    result = sum;
}

/*
 * ダメな例: スレッド内で起きた例外はcatchできない
 */

int main() {
    using namespace std;

    int result;
    for (int n : {5, -3, 10}) try {
        thread thr(sigma_reference, n, ref(result));
        thr.join();
        cout << "1+2+...+" << n << "= " << result << endl;
    } catch (invalid_argument& err) {
        // main と sigma_reference は 呼び/呼ばれの関係にないため
        // このcatch節には到達できない
        cerr << err.what() << endl;
    }
}