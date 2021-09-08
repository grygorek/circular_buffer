/// Copyright 2021 Piotr Grygorczuk <grygorek@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.

#ifndef CIRC_BUFFER_LIB_H_
#define CIRC_BUFFER_LIB_H_

#include <array>

namespace utils {
  /// @brief Circular buffer
  /// 
  /// Circular buffer of elements of type T in a static array of fixed size N.
  /// 
  /// 
  /// @tparam T - type of items
  /// @tparam N - size of the container
  template<class T, int N>
  class CircBuffer {
    std::array<T, N> _d{};
    int _h{};
    int _t{};
    bool _full{};
  public:
    constexpr bool empty() const { return _h == _t && !_full; }
    constexpr bool full() const { return _full; }
    constexpr int size() const {
      if (_full)
        return N;
      if (_h < _t)
        return N - _t + _h;
      return _h - _t;
    }
    constexpr void push(const T& e) {
      if (_full)
        return;
      _d[_h++] = e;
      if (_h == N)
        _h = 0;
      if (_h == _t)
        _full = true;
    }
    constexpr void push(T&& e) {
      if (_full)
        return;
      _d[_h++] = std::move(e);
      if (_h == N)
        _h = 0;
      if (_h == _t)
        _full = true;
    }
    constexpr void pop() {
      if (empty())
        return;

      _d[_t++] = {};
      _full = false;
      if (_t == N)
        _t = 0;
      if (_t == _h)
        _t = _h = 0;
    }
    constexpr T& back() {
      if (_h == 0)
        return _d[N - 1];
      return _d[_h - 1];
    }
    constexpr const T& back() const {
      if (_h == 0)
        return _d[N - 1];
      return _d[_h - 1];
    }
    constexpr T& front() {
      return _d[_t];
    }
    constexpr const T& front() const {
      return _d[_t];
    }
    constexpr T& operator[](int idx) {
      idx %= N;
      if (_t + idx >= N)
        return _d[_t + idx - N];
      return _d[_t + idx];
    }
    constexpr const T& operator[](int idx) const {
      idx %= N;
      if (_t + idx >= N)
        return _d[_t + idx - N];
      return _d[_t + idx];
    }

    template<class C, int K>
    class iterator {
      CircBuffer<C, K>* _cb;
      int _idx;
    public:
      constexpr iterator(CircBuffer<C, K>& c, int idx) : _cb(&c), _idx(idx) {}
      constexpr iterator& operator++() { _idx++; return *this; }
      constexpr iterator operator++(int) { iterator ret = *this; ++(*this); return ret; }
      constexpr T& operator*() { return (*_cb)[_idx]; }
      constexpr bool operator==(iterator& other) { return _cb == other._cb && _idx == other._idx; }
      constexpr bool operator!=(iterator& other) { return !(*this == other); }
    };

    constexpr iterator<T, N> begin() {
      return { *this, _t };
    }
    constexpr const iterator<T, N> begin() const {
      return { *this, _t };
    }
    constexpr iterator<T, N> end() {
      return { *this, _h };
    }
    constexpr const iterator<T, N> end() const {
      return { *this, _h };
    }
  };


} // namespace utils
#endif

