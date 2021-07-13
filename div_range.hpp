#pragma once

// [lo, hi)をn等分する
template <typename T = int>
class div_range {
    T lo_;
    T hi_;
    T strides_;
    int n_;
public:
    div_range(T lo, T hi, int n)
        : lo_(lo), hi_(hi), n_(n) { strides_ = (hi - lo) / n; }
    T lo(int n) { return lo + strides_ * n; }
    T hi(int n) { return (++n < n_) ? lo_ + strides_ * n : hi_; }
};

