#ifdef __GNUC__
#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx2")
#endif

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <array>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <random>
#include <functional>
#include <cmath>
#include <cassert>

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

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
    #ifdef _MSC_VER
      #ifdef LOCAL
        return __rdtsc() * 2.857142857142857e-10; // 1 / 3.5e9, for local (Ryzen 9 3950X)
      #else
        //return __rdtsc() * 3.5714285714285715e-10; // 1 / 2.8e9, for AWS EC2 C3 (Xeon E5-2680 v2)
        //return __rdtsc() * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C4 (Xeon E5-2666 v3)
        return __rdtsc() * 3.333333333333333e-10; // 1 / 3.0e9, for AWS EC2 C5 (Xeon Platinum 8124M / Xeon Platinum 8275CL)
      #endif
    #else
      unsigned long long l, h;
        __asm__ ("rdtsc" : "=a"(l), "=d"(h));
      #ifdef LOCAL
        return (double)(l | h << 32) * 2.857142857142857e-10; // 1 / 3.5e9, for local (Ryzen 9 3950X)
      #else
        //return (double)(l | h << 32) * 3.5714285714285715e-10; // 1 / 2.8e9, for AWS EC2 C3 (Xeon E5-2680 v2)
        //return (double)(l | h << 32) * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C4 (Xeon E5-2666 v3)
        return (double)(l | h << 32) * 3.333333333333333e-10; // 1 / 3.0e9, for AWS EC2 C5 (Xeon Platinum 8124M / Xeon Platinum 8275CL)
      #endif
    #endif
  }
};

using namespace std;

typedef long long int ll;
typedef unsigned long long int ull;
typedef pair<int, int> Pii;
typedef unsigned char uchar;

const ll mod = 1000000007;

timer theTimer;
xorshift64 theRandom;
mt19937 theMersenne(1);

// hyper parameters

// structs

// enums

// constants
constexpr int board_size = 40;
constexpr int turn_max = 1000;

struct {
  vector<int> x = { 0,-1, 0, 1};
  vector<int> y = {-1, 0, 1, 0};
} delta;

// inputs
vector<Pii> start_pos(turn_max);
vector<Pii> end_pos(turn_max);

// outputs
vector<Pii> best_ans(turn_max, Pii(-1, -1));

// internals

void get_first_input() {
  int _board_size, _turn_max;
  cin >> _board_size >> _turn_max;
  for (int i = 0; i < turn_max; i++) {
    cin >> start_pos[i].first >> start_pos[i].second >> end_pos[i].first >> end_pos[i].second;
  }
}

void init() {

}

bool within_board(int x, int y) {
  return 0 <= x && x < board_size && 0 <= y && y < board_size;
}

int get_distance_from_to(vector<vector<bool>> &wall, Pii begin, Pii end) {
  if (wall[begin.first][begin.second] || wall[end.first][end.second]) return 0;
  static vector<vector<bool>> visited(board_size, vector<bool>(board_size));
  for (int i = 0; i < board_size; i++) {
    for (int j = 0; j < board_size; j++) {
      visited[i][j] = false;
    }
  }
  queue<tuple<int, int, int>> que;
  que.emplace(begin.first, begin.second, 0);
  while (!que.empty()) {
    auto [x, y, dist] = que.front();
    que.pop();
    if (visited[x][y]) continue;
    visited[x][y] = true;
    if (x == end.first && y == end.second) {
      return dist;
    }
    for (int d = 0; d < 4; d++) {
      if (within_board(x + delta.x[d], y + delta.y[d]) && !wall[x + delta.x[d]][y + delta.y[d]] && !visited[x + delta.x[d]][y + delta.y[d]]) {
        que.emplace(x + delta.x[d], y + delta.y[d], dist + 1);
      }
    }
  }
  return 0;
}

vector<vector<bool>> create_base_snake_board() {
  vector<vector<bool>> base_snake_board(board_size, vector<bool>(board_size));
  bool open_up = true;
  for (int r = 1; r < board_size + board_size; r += 3) {
    int i = r;
    for (int j = 0; j < board_size; i--, j++) {
      if (!within_board(i, j)) continue;
      if (open_up && i == 0) continue;
      if (open_up && j == board_size - 1) continue;
      if (!open_up && i == board_size - 1) continue;
      if (!open_up && j == 0) continue;
      base_snake_board[i][j] = true;
    }
    open_up = !open_up;
  }
  return base_snake_board;
}

void solve() {
  ll best_score = 0;
  auto base_snake_board = create_base_snake_board();
  int t = 0;
  for (int i = 0; i < board_size; i++) {
    for (int j = 0; j < board_size; j++) {
      if (base_snake_board[i][j]) {
        best_ans[t] = Pii(i, j);
        t++;
      }
    }
  }
  sort(best_ans.begin(), best_ans.end(), [&](auto &a, auto &b){return abs(a.first + a.second - board_size) < abs(b.first + b.second - board_size);});

  {
    vector<int> row_create_order;
    for (int r = 1; r < board_size + board_size; r += 3) {
      row_create_order.push_back(r);
    }
    sort(row_create_order.begin(), row_create_order.end(), [&](auto &a, auto &b){return abs(a - board_size) < abs(b - board_size);});

    auto generate_ans_without_dp = [&]() {
      vector<Pii> ans(turn_max, Pii(-1, -1));
      int t = 0;
      for (auto &r: row_create_order) {
        int i = r;
        for (int j = 0; j < board_size; i--, j++) {
          if (within_board(i, j) && base_snake_board[i][j]) {
            ans[t] = Pii(i, j);
            t++;
          }
        }
      }
      return ans;
    };

    vector<Pii> ans_with_dp(turn_max);
    {
      vector<int> distance_with_all_wall(turn_max);
      vector<int> distance_with_one_removed(turn_max);
      vector<bool> can_remove_wall(turn_max);
      vector<Pii> wall_to_remove(turn_max);
      auto wall = base_snake_board;
      auto ans_without_dp = generate_ans_without_dp();
      for (int i = 0; i < turn_max; i++) {
        if (ans_without_dp[i] != Pii(-1, -1)) continue;
        distance_with_all_wall[i] = get_distance_from_to(wall, start_pos[i], end_pos[i]);
        bool wall_on_start = base_snake_board[start_pos[i].first][start_pos[i].second];
        bool wall_on_end = base_snake_board[end_pos[i].first][end_pos[i].second];
        if (wall_on_start != wall_on_end) {
          can_remove_wall[i] = true;
          if (wall_on_start) wall_to_remove[i] = start_pos[i];
          else wall_to_remove[i] = end_pos[i];
          wall[wall_to_remove[i].first][wall_to_remove[i].second] = false;
          distance_with_one_removed[i] = get_distance_from_to(wall, start_pos[i], end_pos[i]);
          wall[wall_to_remove[i].first][wall_to_remove[i].second] = true;
        }
      }
      vector<vector<ll>> dp(turn_max + 1, vector<ll>(2));
      vector<vector<bool>> prev_wall_toggled(turn_max + 1, vector<bool>(2));
      dp[t][1] = -9999999;
      for (int i = t; i < turn_max; i++) {
        if (dp[i][0] > dp[i][1]) {
          dp[i+1][0] = dp[i][0] + distance_with_all_wall[i] * distance_with_all_wall[i];
          prev_wall_toggled[i][0] = false;
        }
        else {
          dp[i+1][0] = dp[i][1] + distance_with_all_wall[i] * distance_with_all_wall[i];
          prev_wall_toggled[i][0] = true;
        }
        dp[i+1][1] = dp[i][0] + distance_with_one_removed[i] * distance_with_one_removed[i];
        prev_wall_toggled[i][1] = true;
      }
      ans_with_dp = ans_without_dp;
      int wall_removed = (dp[turn_max][0] < dp[turn_max][1]);
      for (int i = turn_max; i >= t; i--) {
        if (prev_wall_toggled[i][wall_removed]) {
          wall_removed = !wall_removed;
          if (!wall_removed) {
            ans_with_dp[i] = wall_to_remove[i];
            if (i < turn_max - 1) ans_with_dp[i+1] = wall_to_remove[i];
          }
        }
      }
    }


    auto generate_ans = [&]() {
      vector<Pii> ans(turn_max, Pii(-1, -1));
      int t = 0;
      for (auto &r: row_create_order) {
        int i = r;
        for (int j = 0; j < board_size; i--, j++) {
          if (within_board(i, j) && base_snake_board[i][j]) {
            ans[t] = Pii(i, j);
            t++;
          }
        }
      }
      for (int i = t; i < turn_max; i++) ans[i] = ans_with_dp[i];
      return ans;
    };

    auto calc_score = [&]() {
      auto ans = generate_ans();
      static vector<vector<bool>> board(board_size, vector<bool>(board_size));
      for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
          board[i][j] = false;
        }
      }
      ll score = 0;
      for (int i = 0; i < turn_max; i++) {
        if (ans[i] != Pii(-1, -1)) board[ans[i].first][ans[i].second] = !board[ans[i].first][ans[i].second];
        int dist = get_distance_from_to(board, start_pos[i], end_pos[i]);
        score += dist * dist;
      }
      return score;
    };

    best_ans = generate_ans();

    ll score = calc_score();
    ll last_score = score;
    ll best_score = score;

    const double base_temperature = 1e-100;
    const double target_temperature = 1e-100;
    // const double decay_rate = 4e-5;
    double temperature = base_temperature;

    int iter_count = 0;
    double time_start = theTimer.time();
    const double time_limit = 1.900;

    while (theTimer.time() < time_limit) {
      double roll = theRandom.nextDouble();
      if (roll < 0.50) {
        int p1 = theRandom.nextUIntMod(row_create_order.size());
        int p2 = theRandom.nextUIntMod(row_create_order.size());
        if (p1 == p2) continue;

        auto orig = row_create_order[p1];
        row_create_order.erase(row_create_order.begin() + p1);
        row_create_order.insert(row_create_order.begin() + p2, orig);

        score = calc_score();

        #ifdef DEBUG
        if (iter_count % 10 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
            best_ans = generate_ans();
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          score = last_score;
          row_create_order.erase(row_create_order.begin() + p2);
          row_create_order.insert(row_create_order.begin() + p1, orig);
        }
      }
      else if (roll < 1.00) {
        int p1 = theRandom.nextUIntMod(row_create_order.size());
        int p2 = theRandom.nextUIntMod(row_create_order.size());
        if (p1 == p2) continue;

        swap(row_create_order[p1], row_create_order[p2]);

        score = calc_score();

        #ifdef DEBUG
        if (iter_count % 10 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
            best_ans = generate_ans();
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          score = last_score;
          swap(row_create_order[p1], row_create_order[p2]);
        }
      }

      // temperature *= 1.0 - decay_rate;
      temperature = exp(log(base_temperature) - ((log(base_temperature) - log(target_temperature)) * ((theTimer.time() - time_start) * (1.0 / (time_limit - time_start)))));
      iter_count++;
    }

    cerr << "iter_count   = " << iter_count << endl;
    cerr << "score        = " << score << endl;
    cerr << "best_score   = " << best_score << endl;
    cerr << "temperature  = " << temperature << endl;
  }
}

void output_ans() {
  for (auto &x: best_ans) cout << x.first << " " << x.second << endl;
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  get_first_input();

  init();
  solve();

  output_ans();

  return 0;
}
