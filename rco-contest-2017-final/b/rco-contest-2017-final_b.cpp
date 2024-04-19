#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
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
#include <unordered_map>
#include <set>
#include <random>
#include <cmath>
#include <cstdlib>
#include <cassert>

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

const int h = 30;
const int w = 30;
const int k = 450;
const int t = 10000;

vector<Pii> start(k);
vector<Pii> finish(k);

vector<vector<int> > board_init(h, vector<int>(w, -1));
int diff_init = 0;

vector<string> movement(k);

ll calc_score(ll d, ll l) {
  return (d + 20) * (l + 1000);
}

ll simulate() {
  vector<Pii> position_prev(k);
  for (int i = 0; i < k; i++) position_prev[i] = start[i];
  vector<Pii> position(k);
  for (int i = 0; i < k; i++) position[i] = start[i];

  vector<vector<int> > board(h, vector<int>(w));
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) board[i][j] = board_init[i][j];
  }

  vector<int> move_count(k);
  for (int i = 0; i < k; i++) move_count[i] = 0;

  vector<vector<int> > block_subject(k);
  for (int i = 0; i < k; i++) block_subject.clear();

  vector<vector<int> > movement_queue(2);
  for (int i = 0; i < 2; i++) movement_queue.clear();
  for (int i = 0; i < k; i++) {
    if (!movement[i].empty()) movement_queue[0].push_back(i);
  }

  int turn_count = 0;
  int diff = diff_init;
  ll best_score = calc_score(diff, turn_count);
  while (turn_count < t) {
    if (movement_queue[turn_count&1].empty()) break; // no moves possible

    // sort(movement_queue[turn_count&1].begin(), movement_queue[turn_count&1].end());

    // attempt to move each cars
    for (auto &move_subject: movement_queue[turn_count&1]) {
      if (movement[move_subject][move_count[move_subject]] == 'L') {
        if (position[move_subject].second-1 < 0) continue;
        if (board[position[move_subject].first][position[move_subject].second-1] == -1) {
          board[position[move_subject].first][position[move_subject].second-1] = move_subject;
          position[move_subject].second--;

          for (auto &unblock_subject: block_subject[move_subject]) movement_queue[(turn_count&1)^1].push_back(unblock_subject);
          block_subject[move_subject].clear();

          move_count[move_subject]++;
          if (move_count[move_subject] < (int) movement[move_subject].size()) movement_queue[(turn_count&1)^1].push_back(move_subject);
        }
        else {
          if (position[board[position[move_subject].first][position[move_subject].second-1]] != Pii(position[move_subject].first, position[move_subject].second-1)) { // already moved
            movement_queue[(turn_count&1)^1].push_back(move_subject);
          }
          else {
            block_subject[board[position[move_subject].first][position[move_subject].second-1]].push_back(move_subject);
          }
        }
      }
      else if (movement[move_subject][move_count[move_subject]] == 'R') {
        if (position[move_subject].second+1 >= w) continue;
        if (board[position[move_subject].first][position[move_subject].second+1] == -1) {
          board[position[move_subject].first][position[move_subject].second+1] = move_subject;
          position[move_subject].second++;

          for (auto &unblock_subject: block_subject[move_subject]) movement_queue[(turn_count&1)^1].push_back(unblock_subject);
          block_subject[move_subject].clear();

          move_count[move_subject]++;
          if (move_count[move_subject] < (int) movement[move_subject].size()) movement_queue[(turn_count&1)^1].push_back(move_subject);
        }
        else {
          if (position[board[position[move_subject].first][position[move_subject].second+1]] != Pii(position[move_subject].first, position[move_subject].second+1)) { // already moved
            movement_queue[(turn_count&1)^1].push_back(move_subject);
          }
          else {
            block_subject[board[position[move_subject].first][position[move_subject].second+1]].push_back(move_subject);
          }
        }
      }
      else if (movement[move_subject][move_count[move_subject]] == 'U') {
        if (position[move_subject].first-1 < 0) continue;
        if (board[position[move_subject].first-1][position[move_subject].second] == -1) {
          board[position[move_subject].first-1][position[move_subject].second] = move_subject;
          position[move_subject].first--;

          for (auto &unblock_subject: block_subject[move_subject]) movement_queue[(turn_count&1)^1].push_back(unblock_subject);
          block_subject[move_subject].clear();

          move_count[move_subject]++;
          if (move_count[move_subject] < (int) movement[move_subject].size()) movement_queue[(turn_count&1)^1].push_back(move_subject);
        }
        else {
          if (position[board[position[move_subject].first-1][position[move_subject].second]] != Pii(position[move_subject].first-1, position[move_subject].second)) { // already moved
            movement_queue[(turn_count&1)^1].push_back(move_subject);
          }
          else {
            block_subject[board[position[move_subject].first-1][position[move_subject].second]].push_back(move_subject);
          }
        }
      }
      else if (movement[move_subject][move_count[move_subject]] == 'D') {
        if (position[move_subject].first+1 >= w) continue;
        if (board[position[move_subject].first+1][position[move_subject].second] == -1) {
          board[position[move_subject].first+1][position[move_subject].second] = move_subject;
          position[move_subject].first++;

          for (auto &unblock_subject: block_subject[move_subject]) movement_queue[(turn_count&1)^1].push_back(unblock_subject);
          block_subject[move_subject].clear();

          move_count[move_subject]++;
          if (move_count[move_subject] < (int) movement[move_subject].size()) movement_queue[(turn_count&1)^1].push_back(move_subject);
        }
        else {
          if (position[board[position[move_subject].first+1][position[move_subject].second]] != Pii(position[move_subject].first+1, position[move_subject].second)) { // already moved
            movement_queue[(turn_count&1)^1].push_back(move_subject);
          }
          else {
            block_subject[board[position[move_subject].first+1][position[move_subject].second]].push_back(move_subject);
          }
        }
      }
    }

    for (auto &move_subject: movement_queue[turn_count&1]) {
      if (position[move_subject] != position_prev[move_subject]) {
        diff += (abs(position[move_subject].first - finish[move_subject].first) + abs(position[move_subject].second - finish[move_subject].second)) - (abs(position_prev[move_subject].first - finish[move_subject].first) + abs(position_prev[move_subject].second - finish[move_subject].second));
        board[position_prev[move_subject].first][position_prev[move_subject].second] = -1;
        position_prev[move_subject] = position[move_subject];
      }
    }
    movement_queue[turn_count&1].clear();

    turn_count++;
    ll score = calc_score(diff, turn_count);
    if (score < best_score) best_score = score;
  }

  return best_score;
}

vector<string> get_answer() {
  vector<Pii> position_prev(k);
  for (int i = 0; i < k; i++) position_prev[i] = start[i];
  vector<Pii> position(k);
  for (int i = 0; i < k; i++) position[i] = start[i];

  vector<vector<int> > board(h, vector<int>(w));
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) board[i][j] = board_init[i][j];
  }

  vector<int> move_count(k);
  for (int i = 0; i < k; i++) move_count[i] = 0;

  vector<vector<int> > block_subject(k);
  for (int i = 0; i < k; i++) block_subject.clear();

  vector<vector<int> > movement_queue(2);
  for (int i = 0; i < 2; i++) movement_queue.clear();
  for (int i = 0; i < k; i++) {
    if (!movement[i].empty()) movement_queue[0].push_back(i);
  }

  int turn_count = 0;
  int best_turn = 0;
  int diff = diff_init;
  ll best_score = calc_score(diff, turn_count);
  vector<string> ans;
  while (turn_count < t) {
    // cerr << turn_count << endl;
    // for (auto &x: board) {
    //   for (auto &y: x) {
    //     if (y == -1) cerr << ".";
    //     else {
    //       if (move_count[y] < (int) movement[y].size()) cerr << movement[y][move_count[y]];
    //       else cerr << "o";
    //     }
    //   }
    //   cerr << endl;
    // }
    // cerr << endl;
    // for (int i = 0; i < h; i++) {
    //   for (int j = 0; j < w; j++) {
    //     if (board[i][j] != -1) assert(position[board[i][j]] == Pii(i, j));
    //   }
    // }

    if (movement_queue[turn_count&1].empty()) break; // no moves possible

    sort(movement_queue[turn_count&1].begin(), movement_queue[turn_count&1].end());

    ans.push_back("");
    for (int i = 0; i < k; i++) ans[turn_count].push_back('-');
    bool moved = false;

    // attempt to move each cars
    for (auto &move_subject: movement_queue[turn_count&1]) {
      if (movement[move_subject][move_count[move_subject]] == 'L') {
        if (position[move_subject].second-1 < 0) continue;
        if (board[position[move_subject].first][position[move_subject].second-1] == -1) {
          board[position[move_subject].first][position[move_subject].second-1] = move_subject;
          position[move_subject].second--;

          for (auto &unblock_subject: block_subject[move_subject]) movement_queue[(turn_count&1)^1].push_back(unblock_subject);
          block_subject[move_subject].clear();

          ans[turn_count][move_subject] = movement[move_subject][move_count[move_subject]];
          moved = true;

          move_count[move_subject]++;
          if (move_count[move_subject] < (int) movement[move_subject].size()) movement_queue[(turn_count&1)^1].push_back(move_subject);
        }
        else {
          if (position[board[position[move_subject].first][position[move_subject].second-1]] != Pii(position[move_subject].first, position[move_subject].second-1)) { // already moved
            movement_queue[(turn_count&1)^1].push_back(move_subject);
          }
          else {
            block_subject[board[position[move_subject].first][position[move_subject].second-1]].push_back(move_subject);
          }
        }
      }
      else if (movement[move_subject][move_count[move_subject]] == 'R') {
        if (position[move_subject].second+1 >= w) continue;
        if (board[position[move_subject].first][position[move_subject].second+1] == -1) {
          board[position[move_subject].first][position[move_subject].second+1] = move_subject;
          position[move_subject].second++;

          for (auto &unblock_subject: block_subject[move_subject]) movement_queue[(turn_count&1)^1].push_back(unblock_subject);
          block_subject[move_subject].clear();

          ans[turn_count][move_subject] = movement[move_subject][move_count[move_subject]];
          moved = true;

          move_count[move_subject]++;
          if (move_count[move_subject] < (int) movement[move_subject].size()) movement_queue[(turn_count&1)^1].push_back(move_subject);
        }
        else {
          if (position[board[position[move_subject].first][position[move_subject].second+1]] != Pii(position[move_subject].first, position[move_subject].second+1)) { // already moved
            movement_queue[(turn_count&1)^1].push_back(move_subject);
          }
          else {
            block_subject[board[position[move_subject].first][position[move_subject].second+1]].push_back(move_subject);
          }
        }
      }
      else if (movement[move_subject][move_count[move_subject]] == 'U') {
        if (position[move_subject].first-1 < 0) continue;
        if (board[position[move_subject].first-1][position[move_subject].second] == -1) {
          board[position[move_subject].first-1][position[move_subject].second] = move_subject;
          position[move_subject].first--;

          for (auto &unblock_subject: block_subject[move_subject]) movement_queue[(turn_count&1)^1].push_back(unblock_subject);
          block_subject[move_subject].clear();

          ans[turn_count][move_subject] = movement[move_subject][move_count[move_subject]];
          moved = true;

          move_count[move_subject]++;
          if (move_count[move_subject] < (int) movement[move_subject].size()) movement_queue[(turn_count&1)^1].push_back(move_subject);
        }
        else {
          if (position[board[position[move_subject].first-1][position[move_subject].second]] != Pii(position[move_subject].first-1, position[move_subject].second)) { // already moved
            movement_queue[(turn_count&1)^1].push_back(move_subject);
          }
          else {
            block_subject[board[position[move_subject].first-1][position[move_subject].second]].push_back(move_subject);
          }
        }
      }
      else if (movement[move_subject][move_count[move_subject]] == 'D') {
        if (position[move_subject].first+1 >= w) continue;
        if (board[position[move_subject].first+1][position[move_subject].second] == -1) {
          board[position[move_subject].first+1][position[move_subject].second] = move_subject;
          position[move_subject].first++;

          for (auto &unblock_subject: block_subject[move_subject]) movement_queue[(turn_count&1)^1].push_back(unblock_subject);
          block_subject[move_subject].clear();

          ans[turn_count][move_subject] = movement[move_subject][move_count[move_subject]];
          moved = true;

          move_count[move_subject]++;
          if (move_count[move_subject] < (int) movement[move_subject].size()) movement_queue[(turn_count&1)^1].push_back(move_subject);
        }
        else {
          if (position[board[position[move_subject].first+1][position[move_subject].second]] != Pii(position[move_subject].first+1, position[move_subject].second)) { // already moved
            movement_queue[(turn_count&1)^1].push_back(move_subject);
          }
          else {
            block_subject[board[position[move_subject].first+1][position[move_subject].second]].push_back(move_subject);
          }
        }
      }
    }

    for (auto &move_subject: movement_queue[turn_count&1]) {
      if (position[move_subject] != position_prev[move_subject]) {
        diff += (abs(position[move_subject].first - finish[move_subject].first) + abs(position[move_subject].second - finish[move_subject].second)) - (abs(position_prev[move_subject].first - finish[move_subject].first) + abs(position_prev[move_subject].second - finish[move_subject].second));
        board[position_prev[move_subject].first][position_prev[move_subject].second] = -1;
        position_prev[move_subject] = position[move_subject];
      }
    }
    movement_queue[turn_count&1].clear();

    turn_count++;
    ll score = calc_score(diff, turn_count);
    if (score < best_score) {
      best_score = score;
      best_turn = turn_count;
    }

    if (!moved) ans.pop_back();
  }

  return ans;
}

vector<Pii> get_final_position() {
  vector<Pii> position_prev(k);
  for (int i = 0; i < k; i++) position_prev[i] = start[i];
  vector<Pii> position(k);
  for (int i = 0; i < k; i++) position[i] = start[i];

  vector<vector<int> > board(h, vector<int>(w));
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) board[i][j] = board_init[i][j];
  }

  vector<int> move_count(k);
  for (int i = 0; i < k; i++) move_count[i] = 0;

  vector<vector<int> > block_subject(k);
  for (int i = 0; i < k; i++) block_subject.clear();

  vector<vector<int> > movement_queue(2);
  for (int i = 0; i < 2; i++) movement_queue.clear();
  for (int i = 0; i < k; i++) {
    if (!movement[i].empty()) movement_queue[0].push_back(i);
  }

  int turn_count = 0;
  int diff = diff_init;
  ll best_score = calc_score(diff, turn_count);
  while (turn_count < t) {
    if (movement_queue[turn_count&1].empty()) break; // no moves possible

    // sort(movement_queue[turn_count&1].begin(), movement_queue[turn_count&1].end());

    // attempt to move each cars
    for (auto &move_subject: movement_queue[turn_count&1]) {
      if (movement[move_subject][move_count[move_subject]] == 'L') {
        if (position[move_subject].second-1 < 0) continue;
        if (board[position[move_subject].first][position[move_subject].second-1] == -1) {
          board[position[move_subject].first][position[move_subject].second-1] = move_subject;
          position[move_subject].second--;

          for (auto &unblock_subject: block_subject[move_subject]) movement_queue[(turn_count&1)^1].push_back(unblock_subject);
          block_subject[move_subject].clear();

          move_count[move_subject]++;
          if (move_count[move_subject] < (int) movement[move_subject].size()) movement_queue[(turn_count&1)^1].push_back(move_subject);
        }
        else {
          if (position[board[position[move_subject].first][position[move_subject].second-1]] != Pii(position[move_subject].first, position[move_subject].second-1)) { // already moved
            movement_queue[(turn_count&1)^1].push_back(move_subject);
          }
          else {
            block_subject[board[position[move_subject].first][position[move_subject].second-1]].push_back(move_subject);
          }
        }
      }
      else if (movement[move_subject][move_count[move_subject]] == 'R') {
        if (position[move_subject].second+1 >= w) continue;
        if (board[position[move_subject].first][position[move_subject].second+1] == -1) {
          board[position[move_subject].first][position[move_subject].second+1] = move_subject;
          position[move_subject].second++;

          for (auto &unblock_subject: block_subject[move_subject]) movement_queue[(turn_count&1)^1].push_back(unblock_subject);
          block_subject[move_subject].clear();

          move_count[move_subject]++;
          if (move_count[move_subject] < (int) movement[move_subject].size()) movement_queue[(turn_count&1)^1].push_back(move_subject);
        }
        else {
          if (position[board[position[move_subject].first][position[move_subject].second+1]] != Pii(position[move_subject].first, position[move_subject].second+1)) { // already moved
            movement_queue[(turn_count&1)^1].push_back(move_subject);
          }
          else {
            block_subject[board[position[move_subject].first][position[move_subject].second+1]].push_back(move_subject);
          }
        }
      }
      else if (movement[move_subject][move_count[move_subject]] == 'U') {
        if (position[move_subject].first-1 < 0) continue;
        if (board[position[move_subject].first-1][position[move_subject].second] == -1) {
          board[position[move_subject].first-1][position[move_subject].second] = move_subject;
          position[move_subject].first--;

          for (auto &unblock_subject: block_subject[move_subject]) movement_queue[(turn_count&1)^1].push_back(unblock_subject);
          block_subject[move_subject].clear();

          move_count[move_subject]++;
          if (move_count[move_subject] < (int) movement[move_subject].size()) movement_queue[(turn_count&1)^1].push_back(move_subject);
        }
        else {
          if (position[board[position[move_subject].first-1][position[move_subject].second]] != Pii(position[move_subject].first-1, position[move_subject].second)) { // already moved
            movement_queue[(turn_count&1)^1].push_back(move_subject);
          }
          else {
            block_subject[board[position[move_subject].first-1][position[move_subject].second]].push_back(move_subject);
          }
        }
      }
      else if (movement[move_subject][move_count[move_subject]] == 'D') {
        if (position[move_subject].first+1 >= w) continue;
        if (board[position[move_subject].first+1][position[move_subject].second] == -1) {
          board[position[move_subject].first+1][position[move_subject].second] = move_subject;
          position[move_subject].first++;

          for (auto &unblock_subject: block_subject[move_subject]) movement_queue[(turn_count&1)^1].push_back(unblock_subject);
          block_subject[move_subject].clear();

          move_count[move_subject]++;
          if (move_count[move_subject] < (int) movement[move_subject].size()) movement_queue[(turn_count&1)^1].push_back(move_subject);
        }
        else {
          if (position[board[position[move_subject].first+1][position[move_subject].second]] != Pii(position[move_subject].first+1, position[move_subject].second)) { // already moved
            movement_queue[(turn_count&1)^1].push_back(move_subject);
          }
          else {
            block_subject[board[position[move_subject].first+1][position[move_subject].second]].push_back(move_subject);
          }
        }
      }
    }

    for (auto &move_subject: movement_queue[turn_count&1]) {
      if (position[move_subject] != position_prev[move_subject]) {
        diff += (abs(position[move_subject].first - finish[move_subject].first) + abs(position[move_subject].second - finish[move_subject].second)) - (abs(position_prev[move_subject].first - finish[move_subject].first) + abs(position_prev[move_subject].second - finish[move_subject].second));
        board[position_prev[move_subject].first][position_prev[move_subject].second] = -1;
        position_prev[move_subject] = position[move_subject];
      }
    }
    movement_queue[turn_count&1].clear();

    turn_count++;
    ll score = calc_score(diff, turn_count);
    if (score < best_score) best_score = score;
  }

  return position;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  // input
  int _h, _w, _k, _t;
  cin >> _h >> _w >> _k >> _t;
  for (int i = 0; i < k; i++) {
    cin >> start[i].first >> start[i].second >> finish[i].first >> finish[i].second;
    start[i].first--;
    start[i].second--;
    finish[i].first--;
    finish[i].second--;
  }

  // preprocessing
  vector<string> ans, ans_next;

  board_init = vector<vector<int> >(h, vector<int>(w, -1));
  for (int i = 0; i < k; i++) {
    board_init[start[i].first][start[i].second] = i;
  }

  diff_init = 0;
  for (int i = 0; i < k; i++) {
    diff_init += abs(start[i].first - finish[i].first) + abs(start[i].second - finish[i].second);
  }

  for (int i = 1; i < h; i += 3) {
    auto filled = board_init;
    for (int j = 29; j >= 0; j--) {
      if (board_init[i][j] != -1) {
        bool found = false;
        int t = board_init[i][j];
        for (int l = 29; l >= 0; l--) {
          if (filled[i-1][l] == -1) {
            if (j < l) {
              for (int m = j; m < l; m++) movement[t].push_back('R');
            }
            else {
              for (int m = j; m > l; m--) movement[t].push_back('L');
            }
            movement[t].push_back('U');
            filled[i-1][l] = t;
            found = true;
          }
          else if (filled[i+1][l] == -1) {
            if (j < l) {
              for (int m = j; m < l; m++) movement[t].push_back('R');
            }
            else {
              for (int m = j; m > l; m--) movement[t].push_back('L');
            }
            movement[t].push_back('D');
            filled[i+1][l] = t;
            found = true;
          }
          if (found) break;
        }
      }
    }
  }

  ans_next = get_answer();
  for (auto &x: ans_next) ans.push_back(x);

  start = get_final_position();

  board_init = vector<vector<int> >(h, vector<int>(w, -1));
  for (int i = 0; i < k; i++) {
    board_init[start[i].first][start[i].second] = i;
  }

  diff_init = 0;
  for (int i = 0; i < k; i++) {
    diff_init += abs(start[i].first - finish[i].first) + abs(start[i].second - finish[i].second);
  }

  for (int i = 0; i < k; i++) {
    movement[i].clear();
    for (int j = 0; j < 30; j++) movement[i].push_back('R');
  }

  ans_next = get_answer();
  for (auto &x: ans_next) ans.push_back(x);

  start = get_final_position();

  // optimization
  auto bfs = [&](int c) {
    vector<vector<int> > dist(h, vector<int>(w, 1000000007));
    queue<pair<Pii, int> > que;
    que.emplace(Pii(start[c].first, start[c].second), 0);
    while (!que.empty()) {
      auto [pos, d] = que.front();
      que.pop();
      if (d >= dist[pos.first][pos.second]) continue;
      dist[pos.first][pos.second] = d;
      if (pos == finish[c]) break;
      const vector<int> dx = {1, -1, 0, 0};
      const vector<int> dy = {0, 0, 1, -1};
      for (int i = 0; i < 4; i++) {
        if (pos.first + dx[i] >= 0 && pos.first + dx[i] < h && pos.second + dy[i] >= 0 && pos.second + dy[i] < w) {
          if (board_init[pos.first + dx[i]][pos.second + dy[i]] == -1 && dist[pos.first + dx[i]][pos.second + dy[i]] == 1000000007) que.emplace(Pii(pos.first + dx[i], pos.second + dy[i]), d+1);
        }
      }
    }

    if (dist[finish[c].first][finish[c].second] == 1000000007) return string();

    string res = "";
    int px = finish[c].first;
    int py = finish[c].second;
    while (Pii(px, py) != start[c]) {
      const vector<int> dx = {1, -1, 0, 0};
      const vector<int> dy = {0, 0, 1, -1};
      const vector<char> m = {'U', 'D', 'L', 'R'};
      for (int i = 0; i < 4; i++) {
        if (px + dx[i] >= 0 && px + dx[i] < h && py + dy[i] >= 0 && py + dy[i] < w) {
          if (dist[px + dx[i]][py + dy[i]] < dist[px][py]) {
            px += dx[i];
            py += dy[i];
            res.push_back(m[i]);
            break;
          }
        }
      }
    }

    reverse(res.begin(), res.end());

    return res;
  };

  for (int j = 0; j < w; j++) {
    board_init = vector<vector<int> >(h, vector<int>(w, -1));
    for (int i = 0; i < k; i++) {
      board_init[start[i].first][start[i].second] = i;
    }

    diff_init = 0;
    for (int i = 0; i < k; i++) {
      diff_init += abs(start[i].first - finish[i].first) + abs(start[i].second - finish[i].second);
    }

    for (int i = 0; i < k; i++) {
      movement[i].clear();
    }

    for (int i = 0; i < k; i++) {
      movement[i].clear();
      if (finish[i].second == j) {
        movement[i] = bfs(i);

        ans_next = get_answer();
        for (auto &x: ans_next) ans.push_back(x);

        start = get_final_position();

        board_init = vector<vector<int> >(h, vector<int>(w, -1));
        for (int l = 0; l < k; l++) {
          board_init[start[l].first][start[l].second] = l;
        }

        diff_init = 0;
        for (int l = 0; l < k; l++) {
          diff_init += abs(start[l].first - finish[l].first) + abs(start[l].second - finish[l].second);
        }

        movement[i].clear();
      }
    }

    for (int i = 0; i < k; i++) {
      movement[i].clear();
      if (start[i] != finish[i]) {
        for (int j = 0; j < 30; j++) movement[i].push_back('R');
      }
    }

    ans_next = get_answer();
    for (auto &x: ans_next) ans.push_back(x);

    start = get_final_position();
  }

  // postprocessing and output
  cout << ans.size() << endl;
  for (auto &x: ans) cout << x << endl;

  return 0;
}
