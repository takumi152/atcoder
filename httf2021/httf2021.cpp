#pragma GCC optimize ("O3")
//#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx2")

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>
#include <random>
#include <cmath>
#include <cassert>

#include <x86intrin.h>

struct xorshift64 {
  unsigned long long int x = 88172645463325252ULL;
  inline unsigned short nextUShort() {
    x = x ^ (x << 7);
    return x = x ^ (x >> 9);
  }
  inline unsigned int nextUShortMod(unsigned long long int mod) {
    x = x ^ (x << 7);
    x = x ^ (x >> 9);
    return ((x & 0x0000ffffffffffff) * mod) >> 48;
  }
  inline unsigned int nextUInt() {
    x = x ^ (x << 7);
    return x = x ^ (x >> 9);
  }
  inline unsigned int nextUIntMod(unsigned long long int mod) {
    x = x ^ (x << 7);
    x = x ^ (x >> 9);
    return ((x & 0x00000000ffffffff) * mod) >> 32;
  }
  inline unsigned long long int nextULL() {
    x = x ^ (x << 7);
    return x = x ^ (x >> 9);
  }
  inline double nextDouble() {
    x = x ^ (x << 7);
    x = x ^ (x >> 9);
    return (double)x * 5.42101086242752217e-20;
  }
};

struct timer {
  double t = 0.0;
  double lastStop = 0.0;
  bool stopped = false;
  timer() {
    restart();
  }
  inline void restart() {
    t = now();
    stopped = false;
  }
  inline void start() {
    if (stopped) {
      t += now() - lastStop;
      stopped = false;
    }
  }
  inline void stop() {
    if (!stopped) {
      lastStop = now();
      stopped = true;
    }
  }
  inline double time() {
    if (stopped) return lastStop - t;
    else return now() - t;
  }
  inline double now() {
    unsigned long long l, h;
    __asm__ ("rdtsc" : "=a"(l), "=d"(h));
    #ifdef LOCAL
    return (double)(l | h << 32) * 2.857142857142857e-10; // 1 / 3.5e9, for local (Ryzen 9 3950X)
    #else
    //return (double)(l | h << 32) * 3.5714285714285715e-10; // 1 / 2.8e9, for AWS EC2 C3 (Xeon E5-2680 v2)
    //return (double)(l | h << 32) * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C4 (Xeon E5-2666 v3)
    return (double)(l | h << 32) * 3.333333333333333e-10; // 1 / 3.0e9, for AWS EC2 C5 (Xeon Platinum 8124M / Xeon Platinum 8275CL)
    #endif
  }
};

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

timer theTimer;
xorshift64 theRandom;
mt19937 theMersenne(1);

//#define DEBUG

constexpr int n = 100;
constexpr int m = 20;

vector<Pii> card_orig(n);
vector<vector<int> > board_orig(m, vector<int>(m, -1));

string solve1() {
  string ans = "";

  stack<int> card_stack;
  auto board = board_orig;
  auto card = card_orig;
  for (int j = 0; j < 10; j++) {
    if (j % 2 == 0) {
      for (int i = 0; i < 20; i++) {
        if (board[i][j] >= 0) {
          card_stack.push(board[i][j]);
          ans.push_back('I');
          card[board[i][j]] = Pii(-1, -1);
          board[i][j] = -1;
        }
        if (i == 19) break;
        ans.push_back('D');
      }
    }
    else {
      for (int i = 19; i >= 0; i--) {
        if (board[i][j] >= 0) {
          card_stack.push(board[i][j]);
          ans.push_back('I');
          card[board[i][j]] = Pii(-1, -1);
          board[i][j] = -1;
        }
        if (i == 0) break;
        ans.push_back('U');
      }
    }
    ans.push_back('R');
  }

  for (int i = 0; i < 10; i++) {
    if (i % 2 == 0) {
      for (int j = 10; j < 20; j++) {
        if (board[i][j] >= 0) {
          card_stack.push(board[i][j]);
          ans.push_back('I');
          card[board[i][j]] = Pii(-1, -1);
          board[i][j] = -1;
        }
        if (j == 19) break;
        ans.push_back('R');
      }
    }
    else {
      for (int j = 19; j >= 10; j--) {
        if (board[i][j] >= 0) {
          card_stack.push(board[i][j]);
          ans.push_back('I');
          card[board[i][j]] = Pii(-1, -1);
          board[i][j] = -1;
        }
        if (j == 10) break;
        ans.push_back('L');
      }
    }
    ans.push_back('D');
  }

  for (int i = 10; i < 20; i++) {
    if (i % 2 == 0) {
      for (int j = 10; j < 20; j++) {
        if (board[i][j] == -1) {
          auto next = card_stack.top();
          card_stack.pop();
          ans.push_back('O');
          card[next] = Pii(i, j);
          board[i][j] = next;
        }
        if (j == 19) break;
        ans.push_back('R');
      }
    }
    else {
      for (int j = 19; j >= 10; j--) {
        if (board[i][j] == -1) {
          auto next = card_stack.top();
          card_stack.pop();
          ans.push_back('O');
          card[next] = Pii(i, j);
          board[i][j] = next;
        }
        if (j == 10) break;
        ans.push_back('L');
      }
    }
    if (i == 19) break;
    ans.push_back('D');
  }

  auto now = Pii(19, 10);
  for (int i = 0; i < n; i++) {
    while (now.first > card[i].first) {
      ans.push_back('U');
      now.first--;
    }
    while (now.first < card[i].first) {
      ans.push_back('D');
      now.first++;
    }
    while (now.second > card[i].second) {
      ans.push_back('L');
      now.second--;
    }
    while (now.second < card[i].second) {
      ans.push_back('R');
      now.second++;
    }
    ans.push_back('I');
  }

  return ans;
}

string solve2() {
  string ans = "";

  stack<int> card_stack;
  auto board = board_orig;
  auto card = card_orig;

  auto now = Pii(0, 0);

  // auto find_max_path = [&](int next) {
  //   auto pos = now;
  //   auto target = card[next];
  //
  //   bool is_down = (pos.first < target.first);
  //   bool is_right = (pos.second < target.second);
  // }

  auto dp_io = [&](string path) {
    auto pos = now;
    int card_num = 0;
    vector<int> card_list;
    for (auto &x: path) {
           if (x == 'U') pos.first--;
      else if (x == 'D') pos.first++;
      else if (x == 'L') pos.second--;
      else if (x == 'R') pos.second++;
      if (board[pos.first][pos.second] >= 0) {
        card_list.push_back(board[pos.first][pos.second]);
        card_num++;
      }
    }
    card_list.pop_back();
    reverse(card_list.begin(), card_list.end());
    card_num--;

    if (card_num > 16) return 0; // avoid TLE / MLE

    pos = now;
    int card_rem = card_num;
    vector<vector<int> > dp(path.size(), vector<int>(1 << card_num, -100000000));
    vector<vector<int> > prev(path.size(), vector<int>(1 << card_num, 0));
    dp[0][0] = 0;

    for (int i = 0; i < (int) path.size()-1; i++) {
           if (path[i] == 'U') pos.first--;
      else if (path[i] == 'D') pos.first++;
      else if (path[i] == 'L') pos.second--;
      else if (path[i] == 'R') pos.second++;

      if (board[pos.first][pos.second] >= 0) {
        for (int j = 0; j < (1 << card_num); j += (1 << card_rem)) {
          // not taken
          if (dp[i][j] > dp[i+1][j]) {
            dp[i+1][j] = dp[i][j];
            prev[i+1][j] = j;
          }
          // taken
          if (dp[i][j] > dp[i+1][j|(1<<(card_rem-1))]) {
            dp[i+1][j|(1<<(card_rem-1))] = dp[i][j];
            prev[i+1][j|(1<<(card_rem-1))] = j;
          }
        }
        card_rem--;
      }
      else {
        for (int j = 0; j < (1 << card_num); j += (1 << card_rem)) {
          // not put
          if (dp[i][j] > dp[i+1][j]) {
            dp[i+1][j] = dp[i][j];
            prev[i+1][j] = j;
          }
          // put
          if (j == 0) continue;
          int delta = 0;
          int next_card_idx = __builtin_ctz(j);
          int card_to_put = card_list[next_card_idx];
          if (card_to_put > 0) {
            if (card[card_to_put-1].first != -1) delta += (abs(card[card_to_put-1].first - card[card_to_put].first) - abs(card[card_to_put-1].first - pos.first)) + (abs(card[card_to_put-1].second - card[card_to_put].second) - abs(card[card_to_put-1].second - pos.second));
          }
          if (card_to_put < n-1) {
            if (card[card_to_put+1].first != -1) delta += (abs(card[card_to_put+1].first - card[card_to_put].first) - abs(card[card_to_put+1].first - pos.first)) + (abs(card[card_to_put+1].second - card[card_to_put].second) - abs(card[card_to_put+1].second - pos.second));
          }
          if (dp[i][j] + delta > dp[i+1][j&~(1<<next_card_idx)]) {
            dp[i+1][j&~(1<<next_card_idx)] = dp[i][j] + delta;
            prev[i+1][j&~(1<<next_card_idx)] = j;
          }
        }
      }
    }

    return dp[path.size()-1][0];
  };

  auto dp_io_ans = [&](string path) {
    auto pos = now;
    int card_num = 0;
    vector<int> card_list;
    for (auto &x: path) {
           if (x == 'U') pos.first--;
      else if (x == 'D') pos.first++;
      else if (x == 'L') pos.second--;
      else if (x == 'R') pos.second++;
      if (board[pos.first][pos.second] >= 0) {
        card_list.push_back(board[pos.first][pos.second]);
        card_num++;
      }
    }
    card_list.pop_back();
    reverse(card_list.begin(), card_list.end());
    card_num--;

    if (card_num > 16) return path; // avoid TLE / MLE

    pos = now;
    int card_rem = card_num;
    vector<vector<int> > dp(path.size(), vector<int>(1 << card_num, -100000000));
    vector<vector<int> > prev(path.size(), vector<int>(1 << card_num, 0));
    dp[0][0] = 0;

    for (int i = 0; i < (int) path.size()-1; i++) {
           if (path[i] == 'U') pos.first--;
      else if (path[i] == 'D') pos.first++;
      else if (path[i] == 'L') pos.second--;
      else if (path[i] == 'R') pos.second++;

      if (board[pos.first][pos.second] >= 0) {
        for (int j = 0; j < (1 << card_num); j += (1 << card_rem)) {
          // not taken
          if (dp[i][j] >= dp[i+1][j]) {
            dp[i+1][j] = dp[i][j];
            prev[i+1][j] = j;
          }
          // taken
          if (dp[i][j] >= dp[i+1][j|(1<<(card_rem-1))]) {
            dp[i+1][j|(1<<(card_rem-1))] = dp[i][j];
            prev[i+1][j|(1<<(card_rem-1))] = j;
          }
        }
        card_rem--;
      }
      else {
        for (int j = 0; j < (1 << card_num); j += (1 << card_rem)) {
          // not put
          if (dp[i][j] >= dp[i+1][j]) {
            dp[i+1][j] = dp[i][j];
            prev[i+1][j] = j;
          }
          // put
          if (j == 0) continue;
          int delta = 0;
          int next_card_idx = __builtin_ctz(j);
          int card_to_put = card_list[next_card_idx];
          if (card_to_put > 0) {
            if (card[card_to_put-1].first != -1) delta += (abs(card[card_to_put-1].first - card[card_to_put].first) - abs(card[card_to_put-1].first - pos.first)) + (abs(card[card_to_put-1].second - card[card_to_put].second) - abs(card[card_to_put-1].second - pos.second));
          }
          if (card_to_put < n-1) {
            if (card[card_to_put+1].first != -1) delta += (abs(card[card_to_put+1].first - card[card_to_put].first) - abs(card[card_to_put+1].first - pos.first)) + (abs(card[card_to_put+1].second - card[card_to_put].second) - abs(card[card_to_put+1].second - pos.second));
          }
          if (dp[i][j] + delta >= dp[i+1][j&~(1<<next_card_idx)]) {
            dp[i+1][j&~(1<<next_card_idx)] = dp[i][j] + delta;
            prev[i+1][j&~(1<<next_card_idx)] = j;
          }
        }
      }
    }

    string io_to_do = "";
    int mask = 0;
    for (int i = (int) path.size()-1; i > 0; i--) {
           if (prev[i][mask] > mask) io_to_do.push_back('O');
      else if (prev[i][mask] < mask) io_to_do.push_back('I');
      else                           io_to_do.push_back('.');
      mask = prev[i][mask];
    }
    reverse(io_to_do.begin(), io_to_do.end());

    string full_path = "";
    for (int i = 0; i < (int) path.size()-1; i++) {
      full_path += path[i];
      if (io_to_do[i] != '.') full_path += io_to_do[i];
    }
    full_path += path[path.size()-1];

    return full_path;
  };

  for (int i = 0; i < n; i++) {
    string path = "";
    for (int j = 0; j < abs(now.first - card[i].first); j++) {
      if (now.first < card[i].first) path.push_back('D');
      if (now.first > card[i].first) path.push_back('U');
    }
    for (int j = 0; j < abs(now.second - card[i].second); j++) {
      if (now.second < card[i].second) path.push_back('R');
      if (now.second > card[i].second) path.push_back('L');
    }
    shuffle(path.begin(), path.end(), theMersenne);
    int best_difference = -1000;
    for (int k = 0; k < 1000; k++) {
      string next_path = path;
      shuffle(next_path.begin(), next_path.end(), theMersenne);

      auto pos = now;
      int difference = 0;
      for (auto &x: next_path) {
             if (x == 'U') pos.first--;
        else if (x == 'D') pos.first++;
        else if (x == 'L') pos.second--;
        else if (x == 'R') pos.second++;
        if (board[pos.first][pos.second] == -1) difference--;
        else difference++;
      }
      if (difference > best_difference) {
        path = next_path;
        best_difference = difference;
      }
    }

    auto full_path = dp_io_ans(path);
    for (auto &x: full_path) {
           if (x == 'U') now.first--;
      else if (x == 'D') now.first++;
      else if (x == 'L') now.second--;
      else if (x == 'R') now.second++;
      else if (x == 'I') {
        card_stack.push(board[now.first][now.second]);
        card[board[now.first][now.second]] = Pii(-1, -1);
        board[now.first][now.second] = -1;
      }
      else if (x == 'O') {
        auto next = card_stack.top();
        card_stack.pop();
        card[next] = Pii(now.first, now.second);
        board[now.first][now.second] = next;
      }
    }

    ans += full_path + "I";
    card[board[now.first][now.second]] = Pii(-1, -1);
    board[now.first][now.second] = -1;
  }

  return ans;
}

string solve3() {
  string ans = "";

  stack<int> card_stack;
  auto board = board_orig;
  auto card = card_orig;
  for (int j = 0; j < 10; j++) {
    if (j % 2 == 0) {
      for (int i = 0; i < 20; i++) {
        if (board[i][j] >= 0) {
          card_stack.push(board[i][j]);
          ans.push_back('I');
          card[board[i][j]] = Pii(-1, -1);
          board[i][j] = -1;
        }
        if (i == 19) break;
        ans.push_back('D');
      }
    }
    else {
      for (int i = 19; i >= 0; i--) {
        if (board[i][j] >= 0) {
          card_stack.push(board[i][j]);
          ans.push_back('I');
          card[board[i][j]] = Pii(-1, -1);
          board[i][j] = -1;
        }
        if (i == 0) break;
        ans.push_back('U');
      }
    }
    ans.push_back('R');
  }

  for (int i = 0; i < 10; i++) {
    if (i % 2 == 0) {
      for (int j = 10; j < 20; j++) {
        if (board[i][j] >= 0) {
          card_stack.push(board[i][j]);
          ans.push_back('I');
          card[board[i][j]] = Pii(-1, -1);
          board[i][j] = -1;
        }
        if (j == 19) break;
        ans.push_back('R');
      }
    }
    else {
      for (int j = 19; j >= 10; j--) {
        if (board[i][j] >= 0) {
          card_stack.push(board[i][j]);
          ans.push_back('I');
          card[board[i][j]] = Pii(-1, -1);
          board[i][j] = -1;
        }
        if (j == 10) break;
        ans.push_back('L');
      }
    }
    ans.push_back('D');
  }

  for (int i = 10; i < 20; i++) {
    if (i % 2 == 0) {
      for (int j = 10; j < 20; j++) {
        if (board[i][j] == -1) {
          auto next = card_stack.top();
          card_stack.pop();
          ans.push_back('O');
          card[next] = Pii(i, j);
          board[i][j] = next;
        }
        if (j == 19) break;
        ans.push_back('R');
      }
    }
    else {
      for (int j = 19; j >= 10; j--) {
        if (board[i][j] == -1) {
          auto next = card_stack.top();
          card_stack.pop();
          ans.push_back('O');
          card[next] = Pii(i, j);
          board[i][j] = next;
        }
        if (j == 10) break;
        ans.push_back('L');
      }
    }
    if (i == 19) break;
    ans.push_back('D');
  }

  auto now = Pii(19, 10);

  auto dp_io_ans = [&](string path) {
    auto pos = now;
    int card_num = 0;
    vector<int> card_list;
    for (auto &x: path) {
           if (x == 'U') pos.first--;
      else if (x == 'D') pos.first++;
      else if (x == 'L') pos.second--;
      else if (x == 'R') pos.second++;
      if (board[pos.first][pos.second] >= 0) {
        card_list.push_back(board[pos.first][pos.second]);
        card_num++;
      }
    }
    card_list.pop_back();
    reverse(card_list.begin(), card_list.end());
    card_num--;

    if (card_num > 16) return path; // avoid TLE / MLE

    pos = now;
    int card_rem = card_num;
    vector<vector<int> > dp(path.size(), vector<int>(1 << card_num, -100000000));
    vector<vector<int> > prev(path.size(), vector<int>(1 << card_num, 0));
    dp[0][0] = 0;

    for (int i = 0; i < (int) path.size()-1; i++) {
           if (path[i] == 'U') pos.first--;
      else if (path[i] == 'D') pos.first++;
      else if (path[i] == 'L') pos.second--;
      else if (path[i] == 'R') pos.second++;

      if (board[pos.first][pos.second] >= 0) {
        for (int j = 0; j < (1 << card_num); j += (1 << card_rem)) {
          // not taken
          if (dp[i][j] >= dp[i+1][j]) {
            dp[i+1][j] = dp[i][j];
            prev[i+1][j] = j;
          }
          // taken
          if (dp[i][j] >= dp[i+1][j|(1<<(card_rem-1))]) {
            dp[i+1][j|(1<<(card_rem-1))] = dp[i][j];
            prev[i+1][j|(1<<(card_rem-1))] = j;
          }
        }
        card_rem--;
      }
      else {
        for (int j = 0; j < (1 << card_num); j += (1 << card_rem)) {
          // not put
          if (dp[i][j] >= dp[i+1][j]) {
            dp[i+1][j] = dp[i][j];
            prev[i+1][j] = j;
          }
          // put
          if (j == 0) continue;
          int delta = 0;
          int next_card_idx = __builtin_ctz(j);
          int card_to_put = card_list[next_card_idx];
          if (card_to_put > 0) {
            if (card[card_to_put-1].first != -1) delta += (abs(card[card_to_put-1].first - card[card_to_put].first) - abs(card[card_to_put-1].first - pos.first)) + (abs(card[card_to_put-1].second - card[card_to_put].second) - abs(card[card_to_put-1].second - pos.second));
          }
          if (card_to_put < n-1) {
            if (card[card_to_put+1].first != -1) delta += (abs(card[card_to_put+1].first - card[card_to_put].first) - abs(card[card_to_put+1].first - pos.first)) + (abs(card[card_to_put+1].second - card[card_to_put].second) - abs(card[card_to_put+1].second - pos.second));
          }
          if (dp[i][j] + delta >= dp[i+1][j&~(1<<next_card_idx)]) {
            dp[i+1][j&~(1<<next_card_idx)] = dp[i][j] + delta;
            prev[i+1][j&~(1<<next_card_idx)] = j;
          }
        }
      }
    }

    string io_to_do = "";
    int mask = 0;
    for (int i = (int) path.size()-1; i > 0; i--) {
           if (prev[i][mask] > mask) io_to_do.push_back('O');
      else if (prev[i][mask] < mask) io_to_do.push_back('I');
      else                           io_to_do.push_back('.');
      mask = prev[i][mask];
    }
    reverse(io_to_do.begin(), io_to_do.end());

    string full_path = "";
    for (int i = 0; i < (int) path.size()-1; i++) {
      full_path += path[i];
      if (io_to_do[i] != '.') full_path += io_to_do[i];
    }
    full_path += path[path.size()-1];

    return full_path;
  };

  for (int i = 0; i < n; i++) {
    string path = "";
    for (int j = 0; j < abs(now.first - card[i].first); j++) {
      if (now.first < card[i].first) path.push_back('D');
      if (now.first > card[i].first) path.push_back('U');
    }
    for (int j = 0; j < abs(now.second - card[i].second); j++) {
      if (now.second < card[i].second) path.push_back('R');
      if (now.second > card[i].second) path.push_back('L');
    }
    shuffle(path.begin(), path.end(), theMersenne);
    int best_difference = 1000;
    for (int k = 0; k < 1000; k++) {
      string next_path = path;
      shuffle(next_path.begin(), next_path.end(), theMersenne);

      auto pos = now;
      int difference = 0;
      for (auto &x: next_path) {
             if (x == 'U') pos.first--;
        else if (x == 'D') pos.first++;
        else if (x == 'L') pos.second--;
        else if (x == 'R') pos.second++;
        if (board[pos.first][pos.second] == -1) difference--;
        else difference++;
      }
      if (abs(difference) < best_difference) {
        path = next_path;
        best_difference = abs(difference);
      }
    }

    auto full_path = dp_io_ans(path);
    for (auto &x: full_path) {
           if (x == 'U') now.first--;
      else if (x == 'D') now.first++;
      else if (x == 'L') now.second--;
      else if (x == 'R') now.second++;
      else if (x == 'I') {
        card_stack.push(board[now.first][now.second]);
        card[board[now.first][now.second]] = Pii(-1, -1);
        board[now.first][now.second] = -1;
      }
      else if (x == 'O') {
        auto next = card_stack.top();
        card_stack.pop();
        card[next] = Pii(now.first, now.second);
        board[now.first][now.second] = next;
      }
    }

    ans += full_path + "I";
    card[board[now.first][now.second]] = Pii(-1, -1);
    board[now.first][now.second] = -1;
  }

  return ans;
}

string solve4() {
  string ans = "";

  stack<int> card_stack;
  auto board = board_orig;
  auto card = card_orig;
  for (int j = 0; j < 10; j++) {
    if (j % 2 == 0) {
      for (int i = 0; i < 20; i++) {
        if (board[i][j] >= 0) {
          card_stack.push(board[i][j]);
          ans.push_back('I');
          card[board[i][j]] = Pii(-1, -1);
          board[i][j] = -1;
        }
        if (i == 19) break;
        ans.push_back('D');
      }
    }
    else {
      for (int i = 19; i >= 0; i--) {
        if (board[i][j] >= 0) {
          card_stack.push(board[i][j]);
          ans.push_back('I');
          card[board[i][j]] = Pii(-1, -1);
          board[i][j] = -1;
        }
        if (i == 0) break;
        ans.push_back('U');
      }
    }
    ans.push_back('R');
  }

  for (int i = 0; i < 10; i++) {
    if (i % 2 == 0) {
      for (int j = 10; j < 20; j++) {
        if (board[i][j] >= 0) {
          card_stack.push(board[i][j]);
          ans.push_back('I');
          card[board[i][j]] = Pii(-1, -1);
          board[i][j] = -1;
        }
        if (j == 19) break;
        ans.push_back('R');
      }
    }
    else {
      for (int j = 19; j >= 10; j--) {
        if (board[i][j] >= 0) {
          card_stack.push(board[i][j]);
          ans.push_back('I');
          card[board[i][j]] = Pii(-1, -1);
          board[i][j] = -1;
        }
        if (j == 10) break;
        ans.push_back('L');
      }
    }
    ans.push_back('D');
  }

  vector<int> card_place_order;
  vector<bool> card_in_stack(n);

  for (int i = 10; i < 20; i++) {
    if (i % 2 == 0) {
      for (int j = 10; j < 20; j++) {
        if (board[i][j] == -1) {
          auto next = card_stack.top();
          card_stack.pop();
          card_place_order.push_back(next);
          card_in_stack[next] = true;
          card[next] = Pii(i, j);
          board[i][j] = next;
        }
      }
    }
    else {
      for (int j = 19; j >= 10; j--) {
        if (board[i][j] == -1) {
          auto next = card_stack.top();
          card_stack.pop();
          card_place_order.push_back(next);
          card_in_stack[next] = true;
          card[next] = Pii(i, j);
          board[i][j] = next;
        }
      }
    }
  }

  auto begin_pos = Pii(10, 10);

  auto calc_score = [&]() {
    int score = 0;

    auto now = begin_pos;

    for (auto &x: card_place_order) {
      score += abs(card[x].first - now.first) + abs(card[x].second - now.second);
      now = card[x];
    }
    for (int i = 0; i < n; i++) {
      score += abs(card[i].first - now.first) + abs(card[i].second - now.second);
      now = card[i];
    }

    return score;
  };


  int score = calc_score();
  int lastScore = score;
  int bestScore = score;

  double baseTemperature = 1e1;
  double temperature = baseTemperature;
  double decayRate = 4e-7;
  double timeLimit = 0.500;
  int iterCount = 0;

  while (theTimer.time() < timeLimit) {
    double roll = theRandom.nextDouble();
    if (roll < 0.5) {
      int p = theRandom.nextUIntMod(n);
      int x = theRandom.nextUIntMod(m);
      int y = theRandom.nextUIntMod(m);
      if (!card_in_stack[p] || board[x][y] != -1) continue;

      auto card_p_before = card[p];
      card[p] = Pii(x, y);

      score = calc_score();

      #ifdef DEBUG
      if (iterCount % 10000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << endl;
      #endif

      if (score <= lastScore) {
        swap(board[card_p_before.first][card_p_before.second], board[card[p].first][card[p].second]);
        lastScore = score;
        if (score < bestScore) {
          bestScore = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(lastScore - score) / temperature)) { // accept
        swap(board[card_p_before.first][card_p_before.second], board[card[p].first][card[p].second]);
        lastScore = score;
      }
      else { // rollback
        card[p] = card_p_before;
        score = lastScore;
      }
    }
    else {
      int p1 = theRandom.nextUIntMod(n);
      int p2 = theRandom.nextUIntMod(n);
      if (p1 == p2 || !card_in_stack[p1] || !card_in_stack[p2]) continue;

      swap(card[p1], card[p2]);

      score = calc_score();

      #ifdef DEBUG
      if (iterCount % 10000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << endl;
      #endif

      if (score <= lastScore) {
        swap(board[card[p1].first][card[p1].second], board[card[p2].first][card[p2].second]);
        lastScore = score;
        if (score < bestScore) {
          bestScore = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(lastScore - score) / temperature)) { // accept
        swap(board[card[p1].first][card[p1].second], board[card[p2].first][card[p2].second]);
        lastScore = score;
      }
      else { // rollback
        swap(card[p1], card[p2]);
        score = lastScore;
      }
    }

    iterCount++;
    temperature *= 1.0 - decayRate;
  }

  cerr << "iterCount   = " << iterCount << endl;
  cerr << "temperature = " << temperature << endl;
  cerr << "score       = " << score << endl;
  cerr << "bestScore   = " << bestScore << endl;

  auto now = begin_pos;
  for (auto &x: card_place_order) {
    for (int j = 0; j < abs(now.first - card[x].first); j++) {
      if (now.first < card[x].first) ans.push_back('D');
      if (now.first > card[x].first) ans.push_back('U');
    }
    for (int j = 0; j < abs(now.second - card[x].second); j++) {
      if (now.second < card[x].second) ans.push_back('R');
      if (now.second > card[x].second) ans.push_back('L');
    }
    ans.push_back('O');
    now = card[x];
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < abs(now.first - card[i].first); j++) {
      if (now.first < card[i].first) ans.push_back('D');
      if (now.first > card[i].first) ans.push_back('U');
    }
    for (int j = 0; j < abs(now.second - card[i].second); j++) {
      if (now.second < card[i].second) ans.push_back('R');
      if (now.second > card[i].second) ans.push_back('L');
    }
    ans.push_back('I');
    now = card[i];
  }

  return ans;
}

string solve5() {
  string ans = "";

  stack<int> card_stack;
  auto board = board_orig;
  auto card = card_orig;

  vector<int> card_collect_order;
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 20; j++) {
      if ((i < 10 || j < 10) && board[i][j] != -1) {
        card_collect_order.push_back(board[i][j]);
      }
    }
  }

  auto calc_score1 = [&]() {
    int score = 0;

    auto now = Pii(0, 0);
    for (auto &x: card_collect_order) {
      score += abs(card[x].first - now.first) + abs(card[x].second - now.second);
      now = card[x];
    }

    return score;
  };

  int score = calc_score1();
  int lastScore = score;
  int bestScore = score;

  double baseTemperature = 5e0;
  double temperature = baseTemperature;
  double decayRate = 5e-7;
  double timeLimit = 0.900;
  int iterCount = 0;

  while (theTimer.time() < timeLimit) {
    int p1 = theRandom.nextUIntMod(card_collect_order.size());
    int p2 = theRandom.nextUIntMod(card_collect_order.size());
    if (p1 == p2) continue;

    swap(card_collect_order[p1], card_collect_order[p2]);

    score = calc_score1();

    #ifdef DEBUG
    if (iterCount % 10000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << endl;
    #endif

    if (score <= lastScore) {
      lastScore = score;
      if (score < bestScore) {
        bestScore = score;
      }
    }
    else if (theRandom.nextDouble() < exp(double(lastScore - score) / temperature)) { // accept
      lastScore = score;
    }
    else { // rollback
      swap(card_collect_order[p1], card_collect_order[p2]);
      score = lastScore;
    }

    iterCount++;
    temperature *= 1.0 - decayRate;
  }

  cerr << "iterCount   = " << iterCount << endl;
  cerr << "temperature = " << temperature << endl;
  cerr << "score       = " << score << endl;
  cerr << "bestScore   = " << bestScore << endl;

  auto now = Pii(0, 0);
  for (auto &x: card_collect_order) {
    for (int j = 0; j < abs(now.first - card[x].first); j++) {
      if (now.first < card[x].first) ans.push_back('D');
      if (now.first > card[x].first) ans.push_back('U');
    }
    for (int j = 0; j < abs(now.second - card[x].second); j++) {
      if (now.second < card[x].second) ans.push_back('R');
      if (now.second > card[x].second) ans.push_back('L');
    }
    card_stack.push(x);
    ans.push_back('I');
    now = card[x];
    board[now.first][now.second] = -1;
    card[x] = Pii(-1, -1);
  }

  vector<int> card_place_order;
  vector<bool> card_in_stack(n);

  for (int i = 10; i < 20; i++) {
    if (card_stack.empty()) break;
    if (i % 2 == 0) {
      for (int j = 10; j < 20; j++) {
        if (board[i][j] == -1) {
          if (card_stack.empty()) break;
          auto next = card_stack.top();
          card_stack.pop();
          card_place_order.push_back(next);
          card_in_stack[next] = true;
          card[next] = Pii(i, j);
          board[i][j] = next;
        }
      }
    }
    else {
      for (int j = 19; j >= 10; j--) {
        if (board[i][j] == -1) {
          if (card_stack.empty()) break;
          auto next = card_stack.top();
          card_stack.pop();
          card_place_order.push_back(next);
          card_in_stack[next] = true;
          card[next] = Pii(i, j);
          board[i][j] = next;
        }
      }
    }
  }

  auto begin_pos = now;

  auto calc_score2 = [&]() {
    int score = 0;

    auto now = begin_pos;

    for (auto &x: card_place_order) {
      score += abs(card[x].first - now.first) + abs(card[x].second - now.second);
      now = card[x];
    }
    for (int i = 0; i < n; i++) {
      score += abs(card[i].first - now.first) + abs(card[i].second - now.second);
      now = card[i];
    }

    return score;
  };


  score = calc_score2();
  lastScore = score;
  bestScore = score;

  baseTemperature = 5e0;
  temperature = baseTemperature;
  decayRate = 4e-7;
  timeLimit = 2.900;
  iterCount = 0;

  while (theTimer.time() < timeLimit) {
    double roll = theRandom.nextDouble();
    if (roll < 0.4) {
      int p = theRandom.nextUIntMod(n);
      int x = theRandom.nextUIntMod(m);
      int y = theRandom.nextUIntMod(m);
      if (!card_in_stack[p] || board[x][y] != -1) continue;

      auto card_p_before = card[p];
      card[p] = Pii(x, y);

      score = calc_score2();

      #ifdef DEBUG
      if (iterCount % 10000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << endl;
      #endif

      if (score <= lastScore) {
        swap(board[card_p_before.first][card_p_before.second], board[card[p].first][card[p].second]);
        lastScore = score;
        if (score < bestScore) {
          bestScore = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(lastScore - score) / temperature)) { // accept
        swap(board[card_p_before.first][card_p_before.second], board[card[p].first][card[p].second]);
        lastScore = score;
      }
      else { // rollback
        card[p] = card_p_before;
        score = lastScore;
      }
    }
    else if (roll < 0.8) {
      int p1 = theRandom.nextUIntMod(n);
      int p2 = theRandom.nextUIntMod(n);
      if (p1 == p2 || !card_in_stack[p1] || !card_in_stack[p2]) continue;

      swap(card[p1], card[p2]);

      score = calc_score2();

      #ifdef DEBUG
      if (iterCount % 10000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << endl;
      #endif

      if (score <= lastScore) {
        swap(board[card[p1].first][card[p1].second], board[card[p2].first][card[p2].second]);
        lastScore = score;
        if (score < bestScore) {
          bestScore = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(lastScore - score) / temperature)) { // accept
        swap(board[card[p1].first][card[p1].second], board[card[p2].first][card[p2].second]);
        lastScore = score;
      }
      else { // rollback
        swap(card[p1], card[p2]);
        score = lastScore;
      }
    }
    else {
      int p1 = theRandom.nextUIntMod(n);
      int p2 = theRandom.nextUIntMod(n);
      int p3 = theRandom.nextUIntMod(n);
      if (p1 == p2 || p1 == p3 || p2 == p3 || !card_in_stack[p1] || !card_in_stack[p2] || !card_in_stack[p3]) continue;

      swap(card[p1], card[p2]);
      swap(card[p2], card[p3]);

      score = calc_score2();

      #ifdef DEBUG
      if (iterCount % 10000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << endl;
      #endif

      if (score <= lastScore) {
        swap(board[card[p1].first][card[p1].second], board[card[p2].first][card[p2].second]);
        swap(board[card[p2].first][card[p2].second], board[card[p3].first][card[p3].second]);
        lastScore = score;
        if (score < bestScore) {
          bestScore = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(lastScore - score) / temperature)) { // accept
        swap(board[card[p1].first][card[p1].second], board[card[p2].first][card[p2].second]);
        swap(board[card[p2].first][card[p2].second], board[card[p3].first][card[p3].second]);
        lastScore = score;
      }
      else { // rollback
        swap(card[p2], card[p3]);
        swap(card[p1], card[p2]);
        score = lastScore;
      }
    }

    iterCount++;
    temperature *= 1.0 - decayRate;
  }

  cerr << "iterCount   = " << iterCount << endl;
  cerr << "temperature = " << temperature << endl;
  cerr << "score       = " << score << endl;
  cerr << "bestScore   = " << bestScore << endl;

  now = begin_pos;
  for (auto &x: card_place_order) {
    for (int j = 0; j < abs(now.first - card[x].first); j++) {
      if (now.first < card[x].first) ans.push_back('D');
      if (now.first > card[x].first) ans.push_back('U');
    }
    for (int j = 0; j < abs(now.second - card[x].second); j++) {
      if (now.second < card[x].second) ans.push_back('R');
      if (now.second > card[x].second) ans.push_back('L');
    }
    ans.push_back('O');
    now = card[x];
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < abs(now.first - card[i].first); j++) {
      if (now.first < card[i].first) ans.push_back('D');
      if (now.first > card[i].first) ans.push_back('U');
    }
    for (int j = 0; j < abs(now.second - card[i].second); j++) {
      if (now.second < card[i].second) ans.push_back('R');
      if (now.second > card[i].second) ans.push_back('L');
    }
    ans.push_back('I');
    now = card[i];
  }

  return ans;
}

string solve6() {
  string ans = "";

  auto board = board_orig;
  auto card = card_orig;

  auto board_after = board_orig;
  auto card_after = card_orig;

  vector<int> card_after_x(n), card_after_y(n);
  for (int i = 0; i < n; i++) {
    card_after_x[i] = card_orig[i].first;
    card_after_y[i] = card_orig[i].second;
  }

  vector<int> card_collect_order(n);
  for (int i = 0; i < n; i++) card_collect_order[i] = i;
  shuffle(card_collect_order.begin(), card_collect_order.end(), theMersenne);

  vector<int> card_subj_x(n*2+1), card_subj_y(n*2+1);
  vector<int> card_ref_x(n*2+1), card_ref_y(n*2+1);
  vector<int> card_same(n*2+1, true);

  for (int i = 0; i < n; i++) {
    card_subj_x[i] = card[card_collect_order[i]].first;
    card_subj_x[2*n-i-1] = card[card_collect_order[i]].first;
    card_subj_y[i] = card[card_collect_order[i]].second;
    card_subj_y[2*n-i-1] = card[card_collect_order[i]].second;
    card_ref_x[i] = card[card_collect_order[i]].first;
    card_ref_x[2*n-i-1] = card[card_collect_order[i]].first;
    card_ref_y[i] = card[card_collect_order[i]].second;
    card_ref_y[2*n-i-1] = card[card_collect_order[i]].second;
  }
  card_same[2*n] = false;

  auto update_ref = [&](int cp_idx, int index) {
    bool card_same_before = card_same[index];
    card_same[index] = (card[card_collect_order[cp_idx]] == card_after[card_collect_order[cp_idx]]);
    if (card_same_before != card_same[index]) {
      if (!card_same[index]) {
        card_ref_x[index] = 0;
        card_ref_y[index] = 0;
        for (int i = index-1; i >= 0; i--) {
          if (!card_same[i]) {
            card_ref_x[index] = card_subj_x[i];
            card_ref_y[index] = card_subj_y[i];
            break;
          }
        }
        for (int i = index+1; i <= 2*n; i++) {
          if (!card_same[i]) {
            card_ref_x[i] = card_subj_x[index];
            card_ref_y[i] = card_subj_y[index];
            break;
          }
        }
      }
      else {
        card_ref_x[index] = card_subj_x[index];
        card_ref_y[index] = card_subj_y[index];
        int p = index-1;
        for (; p >= 0; p--) {
          if (!card_same[p]) break;
        }
        int q = index+1;
        for (; q <= 2*n; q++) {
          if (!card_same[q]) break;
        }
        if (q <= 2*n) {
          if (p >= 0) {
            card_ref_x[q] = card_subj_x[p];
            card_ref_y[q] = card_subj_y[p];
          }
          else {
            card_ref_x[q] = card_subj_x[0];
            card_ref_y[q] = card_subj_y[0];
          }
        }
      }
    }
    else if (!card_same[index]) {
      for (int i = index+1; i <= 2*n; i++) {
        if (!card_same[i]) {
          card_ref_x[i] = card_subj_x[index];
          card_ref_y[i] = card_subj_y[index];
          break;
        }
      }
    }
  };

  for (int i = 0; i < n; i++) {
    update_ref(i, i);
    update_ref(i, 2*n-i-1);
  }

  auto calc_score = [&]() {
    int score = 0;

    for (int i = 0; i < 2*n; i++) {
      score += abs(card_subj_x[i] - card_ref_x[i]);
    }
    for (int i = 0; i < 2*n; i++) {
      score += abs(card_subj_y[i] - card_ref_y[i]);
    }
    score += abs(card_after_x[0] - card_ref_x[2*n]);
    score += abs(card_after_y[0] - card_ref_y[2*n]);
    for (int p = 1; p < n; p++) {
      score += abs(card_after_x[p] - card_after_x[p-1]);
    }
    for (int p = 1; p < n; p++) {
      score += abs(card_after_y[p] - card_after_y[p-1]);
    }

    return score;
  };

  int score = calc_score();
  int lastScore = score;
  int bestScore = score;

  double baseTemperature = 5e0;
  double temperature = baseTemperature;
  double decayRate = 5e-7;
  double timeLimit = 2.990;
  int iterCount = 0;

  while (theTimer.time() < timeLimit) {

    double roll = theRandom.nextDouble();
    if (roll < 0.4) {
      int p1 = theRandom.nextUIntMod(card_collect_order.size());
      int p2 = theRandom.nextUIntMod(card_collect_order.size());
      if (p1 == p2) continue;

      swap(card_collect_order[p1], card_collect_order[p2]);
      swap(card_subj_x[p1], card_subj_x[p2]);
      swap(card_subj_y[p1], card_subj_y[p2]);
      swap(card_subj_x[2*n-p1-1], card_subj_x[2*n-p2-1]);
      swap(card_subj_y[2*n-p1-1], card_subj_y[2*n-p2-1]);
      update_ref(p1, p1);
      update_ref(p2, p2);
      update_ref(p1, 2*n-p1-1);
      update_ref(p2, 2*n-p2-1);

      score = calc_score();

      #ifdef DEBUG
      if (iterCount % 10000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score <= lastScore) {
        lastScore = score;
        if (score < bestScore) {
          bestScore = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(lastScore - score) / temperature)) { // accept
        lastScore = score;
      }
      else { // rollback
        swap(card_collect_order[p1], card_collect_order[p2]);
        swap(card_subj_x[p1], card_subj_x[p2]);
        swap(card_subj_y[p1], card_subj_y[p2]);
        swap(card_subj_x[2*n-p1-1], card_subj_x[2*n-p2-1]);
        swap(card_subj_y[2*n-p1-1], card_subj_y[2*n-p2-1]);
        update_ref(p1, p1);
        update_ref(p2, p2);
        update_ref(p1, 2*n-p1-1);
        update_ref(p2, 2*n-p2-1);
        score = lastScore;
      }
    }
    else if (roll < 0.7) {
      int p = theRandom.nextUIntMod(n);
      int x = theRandom.nextUIntMod(m);
      int y = theRandom.nextUIntMod(m);
      if (board_after[x][y] != -1) continue;

      auto card_p_before = card_after[card_collect_order[p]];
      card_after[card_collect_order[p]] = Pii(x, y);
      card_after_x[card_collect_order[p]] = x;
      card_after_y[card_collect_order[p]] = y;
      card_subj_x[2*n-p-1] = x;
      card_subj_y[2*n-p-1] = y;
      update_ref(p, p);
      update_ref(p, 2*n-p-1);

      score = calc_score();

      #ifdef DEBUG
      if (iterCount % 10000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score <= lastScore) {
        swap(board_after[card_p_before.first][card_p_before.second], board_after[card_after[card_collect_order[p]].first][card_after[card_collect_order[p]].second]);
        lastScore = score;
        if (score < bestScore) {
          bestScore = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(lastScore - score) / temperature)) { // accept
        swap(board_after[card_p_before.first][card_p_before.second], board_after[card_after[card_collect_order[p]].first][card_after[card_collect_order[p]].second]);
        lastScore = score;
      }
      else { // rollback
        card_after[card_collect_order[p]] = card_p_before;
        card_after_x[card_collect_order[p]] = card_p_before.first;
        card_after_y[card_collect_order[p]] = card_p_before.second;
        card_subj_x[2*n-p-1] = card_p_before.first;
        card_subj_y[2*n-p-1] = card_p_before.second;
        update_ref(p, p);
        update_ref(p, 2*n-p-1);
        score = lastScore;
      }
    }
    else {
      int p1 = theRandom.nextUIntMod(n);
      int p2 = theRandom.nextUIntMod(n);
      if (p1 == p2) continue;

      swap(card_after[card_collect_order[p1]], card_after[card_collect_order[p2]]);
      swap(card_after_x[card_collect_order[p1]], card_after_x[card_collect_order[p2]]);
      swap(card_after_y[card_collect_order[p1]], card_after_y[card_collect_order[p2]]);
      swap(card_subj_x[2*n-p1-1], card_subj_x[2*n-p2-1]);
      swap(card_subj_y[2*n-p1-1], card_subj_y[2*n-p2-1]);
      update_ref(p1, p1);
      update_ref(p2, p2);
      update_ref(p1, 2*n-p1-1);
      update_ref(p2, 2*n-p2-1);

      score = calc_score();

      #ifdef DEBUG
      if (iterCount % 10000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score <= lastScore) {
        swap(board_after[card_after[card_collect_order[p1]].first][card_after[card_collect_order[p1]].second], board_after[card_after[card_collect_order[p1]].first][card_after[card_collect_order[p1]].second]);
        lastScore = score;
        if (score < bestScore) {
          bestScore = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(lastScore - score) / temperature)) { // accept
        swap(board_after[card_after[card_collect_order[p1]].first][card_after[card_collect_order[p1]].second], board_after[card_after[card_collect_order[p1]].first][card_after[card_collect_order[p1]].second]);
        lastScore = score;
      }
      else { // rollback
        swap(card_after[card_collect_order[p1]], card_after[card_collect_order[p2]]);
        swap(card_after_x[card_collect_order[p1]], card_after_x[card_collect_order[p2]]);
        swap(card_after_y[card_collect_order[p1]], card_after_y[card_collect_order[p2]]);
        swap(card_subj_x[2*n-p1-1], card_subj_x[2*n-p2-1]);
        swap(card_subj_y[2*n-p1-1], card_subj_y[2*n-p2-1]);
        update_ref(p1, p1);
        update_ref(p2, p2);
        update_ref(p1, 2*n-p1-1);
        update_ref(p2, 2*n-p2-1);
        score = lastScore;
      }
    }

    iterCount++;
    temperature *= 1.0 - decayRate;
  }

  cerr << "iterCount   = " << iterCount << endl;
  cerr << "temperature = " << temperature << endl;
  cerr << "score       = " << score << endl;
  cerr << "bestScore   = " << bestScore << endl;

  vector<bool> preplaced(n);
  int last_placed = card_collect_order[0];

  auto now = Pii(0, 0);
  for (int i = 0; i < n; i++) {
    if (card[card_collect_order[i]] != card_after[card_collect_order[i]]) {
      if (i > 0 && board[card_after[card_collect_order[last_placed]].first][card_after[card_collect_order[last_placed]].second] == -1 && ((now.first <= card_after[card_collect_order[last_placed]].first && card_after[card_collect_order[last_placed]].first <= card[card_collect_order[i]].first) || (now.first >= card_after[card_collect_order[last_placed]].first && card_after[card_collect_order[last_placed]].first >= card[card_collect_order[i]].first)) && ((now.second <= card_after[card_collect_order[last_placed]].second && card_after[card_collect_order[last_placed]].second <= card[card_collect_order[i]].second) || (now.second >= card_after[card_collect_order[last_placed]].second && card_after[card_collect_order[last_placed]].second >= card[card_collect_order[i]].second))) {
        preplaced[card_collect_order[last_placed]] = true;
        for (int j = 0; j < abs(now.first - card_after[card_collect_order[last_placed]].first); j++) {
          if (now.first < card_after[card_collect_order[last_placed]].first) ans.push_back('D');
          if (now.first > card_after[card_collect_order[last_placed]].first) ans.push_back('U');
        }
        for (int j = 0; j < abs(now.second - card_after[card_collect_order[last_placed]].second); j++) {
          if (now.second < card_after[card_collect_order[last_placed]].second) ans.push_back('R');
          if (now.second > card_after[card_collect_order[last_placed]].second) ans.push_back('L');
        }
        ans.push_back('O');
        now = card_after[card_collect_order[last_placed]];
      }
      for (int j = 0; j < abs(now.first - card[card_collect_order[i]].first); j++) {
        if (now.first < card[card_collect_order[i]].first) ans.push_back('D');
        if (now.first > card[card_collect_order[i]].first) ans.push_back('U');
      }
      for (int j = 0; j < abs(now.second - card[card_collect_order[i]].second); j++) {
        if (now.second < card[card_collect_order[i]].second) ans.push_back('R');
        if (now.second > card[card_collect_order[i]].second) ans.push_back('L');
      }
      ans.push_back('I');
      now = card[card_collect_order[i]];
      last_placed = i;
    }
  }
  for (int i = n-1; i >= 0; i--) {
    if (card[card_collect_order[i]] != card_after[card_collect_order[i]] && !preplaced[card_collect_order[i]]) {
      for (int j = 0; j < abs(now.first - card_after[card_collect_order[i]].first); j++) {
        if (now.first < card_after[card_collect_order[i]].first) ans.push_back('D');
        if (now.first > card_after[card_collect_order[i]].first) ans.push_back('U');
      }
      for (int j = 0; j < abs(now.second - card_after[card_collect_order[i]].second); j++) {
        if (now.second < card_after[card_collect_order[i]].second) ans.push_back('R');
        if (now.second > card_after[card_collect_order[i]].second) ans.push_back('L');
      }
      ans.push_back('O');
      now = card_after[card_collect_order[i]];
    }
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < abs(now.first - card_after[i].first); j++) {
      if (now.first < card_after[i].first) ans.push_back('D');
      if (now.first > card_after[i].first) ans.push_back('U');
    }
    for (int j = 0; j < abs(now.second - card_after[i].second); j++) {
      if (now.second < card_after[i].second) ans.push_back('R');
      if (now.second > card_after[i].second) ans.push_back('L');
    }
    ans.push_back('I');
    now = card_after[i];
  }

  return ans;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  // input
  for (int i = 0; i < n; i++) cin >> card_orig[i].first >> card_orig[i].second;

  // preprocessing
  for (int i = 0; i < n; i++) board_orig[card_orig[i].first][card_orig[i].second] = i;

  // optimization
  auto ans = solve6();

  int score = 4000;
  for (auto &x: ans) {
    if (x == 'L' || x == 'R' || x == 'U' || x == 'D') score--;
  }
  if (score < 0) score = 0;
  cerr << "score = " << score << endl;

  // postprocess & output
  cout << ans << endl;

  return 0;
}
