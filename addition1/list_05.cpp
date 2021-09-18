/**
 * @file list_05.cpp
 * @author your name (you@domain.com)
 * @brief packaged_task/futureで戻り値を引き取る
 * @version 0.1
 * @date 2021-09-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <iostream>
#include <thread>
#include <future>  // packaged_task/future
#include <utility> // move

/*
 * 0+1+...+n を返す
 */
int sigma(int n) {
  int sum = 0;
  for ( int i = 0; i <= n; ++i ) {
    sum += i;
  }
  return sum;
}

int main() {
  using namespace std;

  // 関数をpackaged_taskでwrapする
  packaged_task<int(int)> pkg(sigma);
  // packaged_taskからfutureを手に入れる
  future<int> result = pkg.get_future();
  // ※ packaged_taskはコピーできないため、std::moveで移動する
  thread thr(move(pkg), 10);
  // 後続するget()がスレッドの終了を待ってくれるので親権放棄して構わない。
  thr.detach(); 

  // 結果は future<T>::get() で受け取る
  cout << "0+1+...+n = " << result.get() << endl;
}