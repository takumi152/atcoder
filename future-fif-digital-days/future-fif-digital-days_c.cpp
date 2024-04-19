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
#include <array>
#include <random>
#include <cmath>
#include <cassert>

struct xorshift64 {
  unsigned long long int x = 88172645463325252ULL;
  inline unsigned short nextUShort() {
    x = x ^ (x << 7);
    return x = x ^ (x >> 9);
  }
  inline unsigned short nextUShortMod(unsigned long long int mod) {
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

constexpr int n = 50;

// inputs
int k, b;
vector<Pii> target_position;

vector<Pii> poly_size;
vector<int> poly_cost;
vector<vector<Pii> > poly_info;

// outputs
vector<vector<int> > ans;

// internals
vector<vector<bool> > board_target(n, vector<bool>(n));
vector<vector<bool> > board_placement(n, vector<bool>(n));
vector<vector<int> > board_place_info;
int total_cost = 0;

bool is_placeable(int id, int x, int y) {
  if (x < 0 || x + poly_size[id].first > n || y < 0 || y + poly_size[id].second > n) return false;
  for (auto &[dx, dy]: poly_info[id]) {
    if (board_placement[x + dx][y + dy]) return false;
  }
  return true;
}

bool place_poly(int id, int x, int y) {
  if (!is_placeable(id, x, y)) return false;

  for (auto &[dx, dy]: poly_info[id]) board_placement[x + dx][y + dy] = true;
  board_place_info.push_back(vector<int>({id, x, y}));
  total_cost += poly_cost[id];
  return true;
}

bool remove_poly(int idx) {
  if (idx >= (int) board_place_info.size()) return false;

  for (auto &[dx, dy]: poly_info[board_place_info[idx][0]]) board_placement[board_place_info[idx][1] + dx][board_place_info[idx][2] + dy] = false;
  total_cost -= poly_cost[board_place_info[idx][0]];
  swap(board_place_info[idx], board_place_info[board_place_info.size()-1]);
  board_place_info.pop_back();
  return true;
}

int calc_score() {
  int score = total_cost;
  for (int i = 0; i < k; i++) {
    vector<vector<bool> > visited(n, vector<bool>(n));
    queue<array<char, 3> > que;
    que.push(array<char, 3>({target_position[i].first, target_position[i].second, 0}));
    while (!que.empty()) {
      auto now = que.front();
      que.pop();
      if (visited[now[0]][now[1]]) continue;
      visited[now[0]][now[1]] = true;
      if (board_placement[now[0]][now[1]]) {
        score += now[2] * poly_cost[0];
        break;
      }

      const vector<int> dx = {-1, 1, 0, 0};
      const vector<int> dy = {0, 0, -1, 1};
      for (int d = 0; d < 4; d++) {
        if (now[0] + dx[d] < 0 || now[0] + dx[d] >= n || now[1] + dy[d] < 0 || now[1] + dy[d] >= n) continue;
        if (!visited[now[0] + dx[d]][now[1] + dy[d]]) que.push(array<char, 3>({now[0] + dx[d], now[1] + dy[d], now[2] + 1}));
      }
    }
  }

  return score;
}

void solve() {
  for (int i = 0; i < k; i++) board_target[target_position[0].first][target_position[0].second] = true;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      for (int l = b-1; l >= 1; l--)
      place_poly(l, i, j);
    }
  }

  {
    int score = calc_score();
    int last_score = score;
    int best_score = score;

    double base_temperature = 1e1;
    double temperature = base_temperature;
    double decay_rate = 3e-3;
    double time_limit = 1.000;
    int iter_count = 0;
    while (theTimer.time() < time_limit) {
      double roll = theRandom.nextDouble();
      if (roll < 0.50) {
        int id = theRandom.nextUIntMod(b-1) + 1;
        int x = theRandom.nextUIntMod(n);
        int y = theRandom.nextUIntMod(n);

        if (!place_poly(id, x, y)) continue;

        score = calc_score();

        #ifdef DEBUG
        if (iter_count % 100 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score <= last_score) {
          last_score = score;
          if (score < best_score) {
            best_score = score;
          }
        }
        else if (theRandom.nextDouble() < exp(double(last_score - score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          remove_poly(board_place_info.size()-1);
          score = last_score;
        }
      }
      else if (roll < 1.00) {
        if (board_place_info.empty()) continue;
        int idx = theRandom.nextUIntMod(board_place_info.size());

        auto removed_poly_info = board_place_info[idx];
        remove_poly(idx);

        score = calc_score();

        #ifdef DEBUG
        if (iter_count % 100 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score <= last_score) {
          last_score = score;
          if (score < best_score) {
            best_score = score;
          }
        }
        else if (theRandom.nextDouble() < exp(double(last_score - score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          place_poly(removed_poly_info[0], removed_poly_info[1], removed_poly_info[2]);
          score = last_score;
        }
      }

      iter_count++;
      temperature *= 1.0 - decay_rate;
    }
  }

  {
    for (int i = 0; i < k; i++) {
      vector<vector<bool> > visited(n, vector<bool>(n));
      visited[target_position[i].first][target_position[i].second] = true;
      vector<vector<Pii> > prev(n, vector<Pii>(n));
      queue<array<char, 3> > que;
      que.push(array<char, 3>({target_position[i].first, target_position[i].second, 0}));
      while (!que.empty()) {
        auto now = que.front();
        que.pop();
        if (board_placement[now[0]][now[1]]) {
          int px = now[0];
          int py = now[1];
          while (px != target_position[i].first || py != target_position[i].second) {
            int npx = prev[px][py].first;
            int npy = prev[px][py].second;
            px = npx;
            py = npy;
            place_poly(0, px, py);
          }
          break;
        }

        const vector<int> dx = {-1, 1, 0, 0};
        const vector<int> dy = {0, 0, -1, 1};
        for (int d = 0; d < 4; d++) {
          if (now[0] + dx[d] < 0 || now[0] + dx[d] >= n || now[1] + dy[d] < 0 || now[1] + dy[d] >= n) continue;
          if (!visited[now[0] + dx[d]][now[1] + dy[d]]) {
            visited[now[0] + dx[d]][now[1] + dy[d]] = true;
            prev[now[0] + dx[d]][now[1] + dy[d]] = Pii(now[0], now[1]);
            que.push(array<char, 3>({now[0] + dx[d], now[1] + dy[d], now[2] + 1}));
          }
        }
      }
    }
  }

  {
    vector<vector<int> > group(n, vector<int>(n, -1));
    Pii first_group;
    int group_count = 0;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (board_placement[i][j] && group[i][j] == -1) {
          if (group_count == 0) first_group = Pii(i, j);
          queue<Pii> que;
          que.emplace(i, j);
          while (!que.empty()) {
            auto [nx, ny] = que.front();
            que.pop();

            const vector<int> dx = {-1, 1, 0, 0};
            const vector<int> dy = {0, 0, -1, 1};
            for (int d = 0; d < 4; d++) {
              if (nx + dx[d] < 0 || nx + dx[d] >= n || ny + dy[d] < 0 || ny + dy[d] >= n) continue;
              if (board_placement[nx + dx[d]][ny + dy[d]] && group[nx + dx[d]][ny + dy[d]] == -1) {
                group[nx + dx[d]][ny + dy[d]] = group_count;
                que.emplace(nx + dx[d], ny + dy[d]);
              }
            }
          }
          group_count++;
        }
      }
    }

    for (int g = 0; g < group_count-1; g++) {
      vector<Pii> in_group;
      {
        vector<vector<bool> > visited(n, vector<bool>(n));
        queue<Pii> que;
        que.push(first_group);
        while (!que.empty()) {
          auto [nx, ny] = que.front();
          que.pop();
          in_group.emplace_back(nx, ny);

          const vector<int> dx = {-1, 1, 0, 0};
          const vector<int> dy = {0, 0, -1, 1};
          for (int d = 0; d < 4; d++) {
            if (nx + dx[d] < 0 || nx + dx[d] >= n || ny + dy[d] < 0 || ny + dy[d] >= n) continue;
            if (board_placement[nx + dx[d]][ny + dy[d]] && !visited[nx + dx[d]][ny + dy[d]]) {
              visited[nx + dx[d]][ny + dy[d]] = true;
              que.emplace(nx + dx[d], ny + dy[d]);
            }
          }
        }
      }
      {
        vector<vector<bool> > visited(n, vector<bool>(n));
        vector<vector<Pii> > prev(n, vector<Pii>(n, Pii(-1, -1)));
        queue<array<char, 3> > que;
        for (auto &[fx, fy]: in_group) {
          que.push(array<char, 3>({fx, fy, 0}));
          visited[fx][fy] = true;
        }
        while (!que.empty()) {
          auto now = que.front();
          que.pop();
          if (board_placement[now[0]][now[1]] && now[2] > 0) {
            int px = now[0];
            int py = now[1];
            while (true) {
              int npx = prev[px][py].first;
              int npy = prev[px][py].second;
              if (npx == -1 || npy == -1) break;
              px = npx;
              py = npy;
              place_poly(0, px, py);
            }
            break;
          }

          const vector<int> dx = {-1, 1, 0, 0};
          const vector<int> dy = {0, 0, -1, 1};
          for (int d = 0; d < 4; d++) {
            if (now[0] + dx[d] < 0 || now[0] + dx[d] >= n || now[1] + dy[d] < 0 || now[1] + dy[d] >= n) continue;
            if (!visited[now[0] + dx[d]][now[1] + dy[d]]) {
              visited[now[0] + dx[d]][now[1] + dy[d]] = true;
              prev[now[0] + dx[d]][now[1] + dy[d]] = Pii(now[0], now[1]);
              que.push(array<char, 3>({now[0] + dx[d], now[1] + dy[d], now[2] + 1}));
            }
          }
        }
      }
    }
  }

  ans = board_place_info;
  for (auto &x: ans) x[0]++;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  // input
  int _n;
  cin >> _n >> k >> b;
  target_position = vector<Pii>(k);
  for (auto &x: target_position) cin >> x.first >> x.second;
  poly_size = vector<Pii>(b);
  poly_cost = vector<int>(b);
  poly_info = vector<vector<Pii> >(b);
  for (int i = 0; i < b; i++) {
    cin >> poly_size[i].first >> poly_size[i].second >> poly_cost[i];
    string poly;
    for (int u = 0; u < poly_size[i].first; u++) {
      cin >> poly;
      for (int v = 0; v < poly_size[i].second; v++) {
        if (poly[v] == '#') poly_info[i].emplace_back(u, v);
      }
    }
  }

  solve();

  cout << ans.size() << endl;
  for (auto &x: ans) {
    for (auto &y: x) cout << y << " ";
    cout << endl;
  }

  return 0;
}
