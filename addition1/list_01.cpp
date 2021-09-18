/**
 * @file list_01.cpp
 * @author your name (you@domain.com)
 * @brief スレッドの生成
 * @version 0.1
 * @date 2021-09-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

/*
 * フツーの関数
 */
void global_fun(int n) {
    using namespace std;
    cout << "global_fun: " + to_string(n) + " 秒後に終了します...\n";
    this_thread::sleep_for(chrono::seconds(n));
    cout << "global_fun: おしまい\n";
}

/*
 * ラムダ式
 */
auto lambda_exp = [](int n) {
    using namespace std;
    cout << "lambda_exp: " + to_string(n) + " 秒後に終了します...\n";
    this_thread::sleep_for(chrono::seconds(n));
    cout << "lambda_exp: おしまい\n";
};

/*
 * メンバ関数
 */
#include <functional>

class Foo {
private:
    int bias_;

public:
    explicit Foo(int b) : bias_(b) {}

    void member_fun(int n) {
        using namespace std;
        cout << "member_fun: " + to_string(bias_ + n) + " 秒後に終了します...\n";
        this_thread::sleep_for(chrono::seconds(bias_ + n));
        cout << "member_fun: おしまい\n";
    }

    // 関数オブジェクトを返す wrapper
    std::function<void(int)> member_fun() {
        // this をキャプチャした lambda で wrap する
        return [this](int n) { member_fun(n); };
    }
};

// おためし
int main() {
    using namespace std;
    cout << "いろんなタスクからスレッドを作るよ!\n";
    thread thr0(global_fun, 2);
    thread thr1(lambda_exp, 3);
    Foo foo(3);
    thread thr2(foo.member_fun(), 1);
    cout << "スレッドの終了を待ってます\n";
    thr0.join();
    thr1.join();
    thr2.join();
    cout << "ぜんぶおしまい\n";
}