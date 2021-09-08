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

#include "gtest/gtest.h"

#include "circ_buffer.h"

template<class C>
void test_empty(C& c) {
  EXPECT_EQ(c.size(), 0);
  EXPECT_EQ(c.full(), false);
  EXPECT_EQ(c.empty(), true);
}

template<class C>
void one_element(C& c) {
  test_empty(c);

  // one element
  c.push(1);
  EXPECT_EQ(c.size(), 1);
  EXPECT_EQ(c.full(), false);
  EXPECT_EQ(c.empty(), false);
  EXPECT_EQ(c[0], 1);
  EXPECT_EQ(c.front(), 1);
  EXPECT_EQ(c.back(), 1);

  // remove one -> empty
  c.pop();
  test_empty(c);
}

TEST(CircBuffer, one_element) {
  utils::CircBuffer<int, 3> c;
  one_element(c);
}

TEST(CircBuffer, two_elements) {
  utils::CircBuffer<int, 3> c;

  // add two
  c.push(1);
  c.push(2);
  EXPECT_EQ(c.size(), 2);
  EXPECT_EQ(c.full(), false);
  EXPECT_EQ(c.empty(), false);
  EXPECT_EQ(c.front(), 1);
  EXPECT_EQ(c.back(), 2);
  EXPECT_EQ(c[0], 1);
  EXPECT_EQ(c[1], 2);

  // remove one -> keep one
  c.pop();
  EXPECT_EQ(c[0], 2);
  EXPECT_EQ(c.size(), 1);
  EXPECT_EQ(c.full(), false);
  EXPECT_EQ(c.empty(), false);
  EXPECT_EQ(c.front(), 2);
  EXPECT_EQ(c.back(), 2);

  // remove second -> empty
  c.pop();
  test_empty(c);
}

TEST(CircBuffer, full_and_rollover) {
  utils::CircBuffer<int, 3> c;
  // add three -> full
  c.push(1);
  c.push(2);
  c.push(3);
  c.push(4); // will be rejected
  EXPECT_EQ(c.size(), 3);
  EXPECT_EQ(c.full(), true);
  EXPECT_EQ(c.empty(), false);
  EXPECT_EQ(c.front(), 1);
  EXPECT_EQ(c.back(), 3);
  EXPECT_EQ(c[0], 1);
  EXPECT_EQ(c[1], 2);
  EXPECT_EQ(c[2], 3);

  // remove one -> there are two at the end of the container
  c.pop();
  EXPECT_EQ(c.size(), 2);
  EXPECT_EQ(c.full(), false);
  EXPECT_EQ(c.empty(), false);
  EXPECT_EQ(c.front(), 2);
  EXPECT_EQ(c.back(), 3);
  EXPECT_EQ(c[0], 2);
  EXPECT_EQ(c[1], 3);

  // add one -> full but rolled over
  c.push(4);
  EXPECT_EQ(c.size(), 3);
  EXPECT_EQ(c.full(), true);
  EXPECT_EQ(c.empty(), false);
  EXPECT_EQ(c.front(), 2);
  EXPECT_EQ(c.back(), 4);
  EXPECT_EQ(c[0], 2);
  EXPECT_EQ(c[1], 3);
  EXPECT_EQ(c[2], 4);

  // remove one -> there are two, one at the end, one at front
  c.pop();
  EXPECT_EQ(c.size(), 2);
  EXPECT_EQ(c.full(), false);
  EXPECT_EQ(c.empty(), false);
  EXPECT_EQ(c.front(), 3);
  EXPECT_EQ(c.back(), 4);
  EXPECT_EQ(c[0], 3);
  EXPECT_EQ(c[1], 4);

  c.pop();
  c.pop();
  test_empty(c);
}

TEST(CircBuffer, Iterate) {
  utils::CircBuffer<int, 3> c;

  c.push(1);
  c.push(2);
  c.push(3);

  int i = 1;
  for (const auto& e : c)
    EXPECT_EQ(e, i++);
}
