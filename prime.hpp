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

