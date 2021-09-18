/**
 * @file list_08.cpp
 * @author your name (you@domain.com)
 * @brief exception_ptrで例外を引き渡す
 * @version 0.1
 * @date 2021-09-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <exception>   // current_exception/rethrow_exception
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
 * sigma_referenceをwrapする。
 */
void sigma_reference_wrap(int n, int& result, std::exception_ptr& exp) try {
    sigma_reference(n, result);
} catch (...) {
    // catchされた例外(がwrapされたexception_ptr)をセットする
    exp = std::current_exception();
    // スレッドはこの時点で終了する
}

int main() {
    using namespace std;

    int result;
    for (int n : {5, -3, 10}) try {
        exception_ptr exp;
        thread thr(sigma_reference_wrap, n, ref(result), ref(exp));
        thr.join();
        // スレッド内で例外が発生していたなら再throw
        if (exp) {
            rethrow_exception(exp);
        }
        cout << "1+2+...+" << n << "= " << result << endl;
    } catch (invalid_argument& err) {  // 再throwされた例外をcatch
        cerr << err.what() << endl;
    }
}