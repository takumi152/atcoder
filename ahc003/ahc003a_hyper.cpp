#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx2")

#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
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

// constants
constexpr int h = 30;
constexpr int w = 30;
constexpr int turn_limit = 1000;

// states
int turn_count = 0;
double score = 0;
double last_score = 0;

vector<int> result_history(turn_limit);
vector<int> path_length_estimation(turn_limit);

vector<vector<vector<int> > > edge_used_horizontal(h, vector<vector<int> >(w-1));
vector<vector<vector<int> > > edge_used_vertical(h-1, vector<vector<int> >(w));

vector<vector<int> > estimated_base_length_horizontal(h, vector<int>(2, 5000));
vector<vector<int> > estimated_base_length_vertical(w, vector<int>(2, 5000));

vector<int> estimated_base_split_horizontal(h, 15);
vector<int> estimated_base_split_vertical(w, 15);

vector<vector<int> > estimated_extra_length_horizontal(h, vector<int>(w-1));
vector<vector<int> > estimated_extra_length_vertical(h-1, vector<int>(w));

// hyper parameters
int base_penalty_center = 4100;
double base_penalty_factor = 1.8e-9;
double extra_penalty_factor = 5.5e-9;
double base_temperature = 1e-2;
double decay_rate = 1e-2;

array<int, 4> get_input() {
  array<int, 4> next_input;
  for (auto &x: next_input) cin >> x;
  return next_input;
}

int output_and_get_result(string path) {
  cout << path << endl;
  int path_length;
  cin >> path_length;
  return path_length;
}

double calc_score_delta_horizontal(int x, int y, int length_delta) {
  double score_delta = 0;
  for (auto &t: edge_used_horizontal[x][y]) {
    score_delta -= 1.0 - (1.0 - (double) min(result_history[t], path_length_estimation[t]) / (double) max(result_history[t], path_length_estimation[t])) * (1.0 - (double) min(result_history[t], path_length_estimation[t]) / (double) max(result_history[t], path_length_estimation[t]));
    score_delta += 1.0 - (1.0 - (double) min(result_history[t], path_length_estimation[t] + length_delta) / (double) max(result_history[t], path_length_estimation[t] + length_delta)) * (1.0 - (double) min(result_history[t], path_length_estimation[t] + length_delta) / (double) max(result_history[t], path_length_estimation[t] + length_delta));
    path_length_estimation[t] += length_delta;
  }
  return score_delta;
}

double calc_score_delta_vertical(int x, int y, int length_delta) {
  double score_delta = 0;
  for (auto &t: edge_used_vertical[x][y]) {
    score_delta -= 1.0 - (1.0 - (double) min(result_history[t], path_length_estimation[t]) / (double) max(result_history[t], path_length_estimation[t])) * (1.0 - (double) min(result_history[t], path_length_estimation[t]) / (double) max(result_history[t], path_length_estimation[t]));
    score_delta += 1.0 - (1.0 - (double) min(result_history[t], path_length_estimation[t] + length_delta) / (double) max(result_history[t], path_length_estimation[t] + length_delta)) * (1.0 - (double) min(result_history[t], path_length_estimation[t] + length_delta) / (double) max(result_history[t], path_length_estimation[t] + length_delta));
    path_length_estimation[t] += length_delta;
  }
  return score_delta;
}

string solve_random_path(int sx, int sy, int tx, int ty) {
  string path = "";
  int px = sx;
  int py = sy;
  while (!(px == tx && py == ty)) {
         if (px < tx) path.push_back('D');
    else if (px > tx) path.push_back('U');
    else if (py < ty) path.push_back('R');
    else if (py > ty) path.push_back('L');

         if (px < tx) px++;
    else if (px > tx) px--;
    else if (py < ty) py++;
    else if (py > ty) py--;
  }

  shuffle(path.begin(), path.end(), theMersenne);
  return path;
}

string solve_long_path(int sx, int sy, int tx, int ty) {
  vector<vector<bool> > visited(h, vector<bool>(w));
  visited[sx][sy] = true;
  vector<Pii> pos({Pii(sx, sy)});
  {
    int px = sx;
    int py = sy;
    while (!(px == tx && py == ty)) {
           if (px < tx) px++;
      else if (px > tx) px--;
      else if (py < ty) py++;
      else if (py > ty) py--;
      pos.emplace_back(px, py);
      visited[px][py] = true;
    }
  }

  auto dfs = [&](auto self, int px, int py, int nx, int ny, int current_score, int target_score, int &step_count, int step_limit, bool &done, vector<Pii> &new_path) {
    if (step_count > step_limit) return;
    step_count++;

    if (done) return;
    if (px == nx && py == ny && current_score >= target_score) done = true;

    visited[px][py] = true;

    const vector<int> dx = { 0,  0, -1,  1};
    const vector<int> dy = {-1,  1,  0,  0};
    vector<int> dir = {0, 1, 2, 3};
    shuffle(dir.begin(), dir.end(), theMersenne);
    for (int d = 0; d < 4; d++) {
      if (px + dx[dir[d]] < 0 || px + dx[dir[d]] >= h || py + dy[dir[d]] < 0 || py + dy[dir[d]] >= w) continue;
      if (visited[px+dx[dir[d]]][py+dy[dir[d]]]) continue;
      self(self, px + dx[dir[d]], py + dy[dir[d]], nx, ny, current_score + 1, target_score, step_count, step_limit, done, new_path);
    }

    visited[px][py] = false;

    if (done) new_path.emplace_back(px, py);
  };

  double time_limit = 0.010 * (turn_count + 1);
  while (theTimer.time() < time_limit) {
    int p1 = theRandom.nextUIntMod(pos.size());
    int p2 = p1 + theRandom.nextUIntMod(41) - 20;
    if (p2 < 0 || p2 >= (int) pos.size() || abs(p1 - p2) < 5) continue;
    if (p1 > p2) swap(p1, p2);

    for (int i = p1; i <= p2; i++) visited[pos[i].first][pos[i].second] = false;
    int step_count = 0;
    bool done = false;
    vector<Pii> new_path;
    dfs(dfs, pos[p1].first, pos[p1].second, pos[p2].first, pos[p2].second, 0, p2 - p1 + 1, step_count, 1000, done, new_path);
    if (done) {
      reverse(new_path.begin(), new_path.end());

      pos.erase(pos.begin() + p1, pos.begin() + p2 + 1);
      for (int i = 0; i < (int) new_path.size(); i++) {
        pos.insert(pos.begin() + p1 + i, new_path[i]);
        visited[new_path[i].first][new_path[i].second] = true;
      }
    }
    else {
      for (int i = p1; i <= p2; i++) visited[pos[i].first][pos[i].second] = true;
    }
  }

  string path = "";
  {
    const vector<char> delta_char = {'L', 'R', 'U', 'D'};
    const vector<int> dx = { 0,  0, -1,  1};
    const vector<int> dy = {-1,  1,  0,  0};
    for (int i = 0; i < (int) pos.size() - 1; i++) {
      for (int d = 0; d < 4; d++) {
        if (pos[i].first + dx[d] == pos[i+1].first && pos[i].second + dy[d] == pos[i+1].second) {
          path.push_back(delta_char[d]);
          break;
        }
      }
    }
  }

  return path;
}

string solve_simulated_annealing(int sx, int sy, int tx, int ty) {
  // optimize length estimation
  double temperature = base_temperature;
  // double time_limit = 0.000 + 0.00197 * turn_count;
  int iter_limit = min(10000, 1000000 / (turn_count + 1));
  int iter_count = 0;
  // while (theTimer.time() < time_limit) {
  while (iter_count < iter_limit) {
    double roll = theRandom.nextDouble();
    if (roll < 0.20) { // update base length horizontal
      int x = theRandom.nextUIntMod(h);
      int s = theRandom.nextUIntMod(2);
      int v = 1000 + theRandom.nextUIntMod(6001) + theRandom.nextUIntMod(2001);

      if (s == 1) {
        for (int j = 0; j < estimated_base_split_horizontal[x]; j++) score += calc_score_delta_horizontal(x, j, v - estimated_base_length_horizontal[x][s]);
      }
      else {
        for (int j = estimated_base_split_horizontal[x]; j < w-1; j++) score += calc_score_delta_horizontal(x, j, v - estimated_base_length_horizontal[x][s]);
      }
      score -= (double) (v - base_penalty_center) * (double) (v - base_penalty_center) * base_penalty_factor - (double) (estimated_base_length_horizontal[x][s] - base_penalty_center) * (double) (estimated_base_length_horizontal[x][s] - base_penalty_center) * base_penalty_factor;

      if (score >= last_score) {
        estimated_base_length_horizontal[x][s] = v;
        last_score = score;
      }
      else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
        estimated_base_length_horizontal[x][s] = v;
        last_score = score;
      }
      else { // rollback
        if (s == 1) {
          for (int j = 0; j < estimated_base_split_horizontal[x]; j++) calc_score_delta_horizontal(x, j, -(v - estimated_base_length_horizontal[x][s]));
        }
        else {
          for (int j = estimated_base_split_horizontal[x]; j < w-1; j++) calc_score_delta_horizontal(x, j, -(v - estimated_base_length_horizontal[x][s]));
        }
        score = last_score;
      }
    }
    else if (roll < 0.40) { // update base length vertical
      int y = theRandom.nextUIntMod(w);
      int s = theRandom.nextUIntMod(2);
      int v = 1000 + theRandom.nextUIntMod(6001) + theRandom.nextUIntMod(2001);

      if (s == 1) {
        for (int i = 0; i < estimated_base_split_vertical[y]; i++) score += calc_score_delta_vertical(i, y, v - estimated_base_length_vertical[y][s]);
      }
      else {
        for (int i = estimated_base_split_vertical[y]; i < h-1; i++) score += calc_score_delta_vertical(i, y, v - estimated_base_length_vertical[y][s]);
      }
      score -= (double) (v - base_penalty_center) * (double) (v - base_penalty_center) * base_penalty_factor - (double) (estimated_base_length_vertical[y][s] - base_penalty_center) * (double) (estimated_base_length_vertical[y][s] - base_penalty_center) * base_penalty_factor;

      if (score >= last_score) {
        estimated_base_length_vertical[y][s] = v;
        last_score = score;
      }
      else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
        estimated_base_length_vertical[y][s] = v;
        last_score = score;
      }
      else {
        if (s == 1) {
          for (int i = 0; i < estimated_base_split_vertical[y]; i++) calc_score_delta_vertical(i, y, -(v - estimated_base_length_vertical[y][s]));
        }
        else {
          for (int i = estimated_base_split_vertical[y]; i < h-1; i++) calc_score_delta_vertical(i, y, -(v - estimated_base_length_vertical[y][s]));
        }
        score = last_score;
      }
    }
    else if (roll < 0.50) { // update base split horizontal
      int x = theRandom.nextUIntMod(h);
      int v = theRandom.nextUIntMod(w-2) + 1;

      if (v < estimated_base_split_horizontal[x]) {
        for (int j = v; j < estimated_base_split_horizontal[x]; j++) score += calc_score_delta_horizontal(x, j, estimated_base_length_horizontal[x][0] - estimated_base_length_horizontal[x][1]);
      }
      else {
        for (int j = estimated_base_split_horizontal[x]; j < v; j++) score += calc_score_delta_horizontal(x, j, estimated_base_length_horizontal[x][1] - estimated_base_length_horizontal[x][0]);
      }

      if (score >= last_score) {
        estimated_base_split_horizontal[x] = v;
        last_score = score;
      }
      else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
        estimated_base_split_horizontal[x] = v;
        last_score = score;
      }
      else { // rollback
        if (v < estimated_base_split_horizontal[x]) {
          for (int j = v; j < estimated_base_split_horizontal[x]; j++) score += calc_score_delta_horizontal(x, j, -(estimated_base_length_horizontal[x][0] - estimated_base_length_horizontal[x][1]));
        }
        else {
          for (int j = estimated_base_split_horizontal[x]; j < v; j++) score += calc_score_delta_horizontal(x, j, -(estimated_base_length_horizontal[x][1] - estimated_base_length_horizontal[x][0]));
        }
        score = last_score;
      }
    }
    else if (roll < 0.60) { // update base split vertical
      int y = theRandom.nextUIntMod(w);
      int v = theRandom.nextUIntMod(h-2) + 1;

      if (v < estimated_base_split_vertical[y]) {
        for (int i = v; i < estimated_base_split_vertical[y]; i++) score += calc_score_delta_vertical(i, y, estimated_base_length_vertical[y][0] - estimated_base_length_vertical[y][1]);
      }
      else {
        for (int i = estimated_base_split_vertical[y]; i < v; i++) score += calc_score_delta_vertical(i, y, estimated_base_length_vertical[y][1] - estimated_base_length_vertical[y][0]);
      }

      if (score >= last_score) {
        estimated_base_split_vertical[y] = v;
        last_score = score;
      }
      else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
        estimated_base_split_vertical[y] = v;
        last_score = score;
      }
      else { // rollback
        if (v < estimated_base_split_vertical[y]) {
          for (int i = v; i < estimated_base_split_vertical[y]; i++) score += calc_score_delta_vertical(i, y, -(estimated_base_length_vertical[y][0] - estimated_base_length_vertical[y][1]));
        }
        else {
          for (int i = estimated_base_split_vertical[y]; i < v; i++) score += calc_score_delta_vertical(i, y, -(estimated_base_length_vertical[y][1] - estimated_base_length_vertical[y][0]));
        }
        score = last_score;
      }
    }
    else if (roll < 0.80) { // update extra horizontal
      int x = theRandom.nextUIntMod(h);
      int y = theRandom.nextUIntMod(w-1);
      int v = theRandom.nextUIntMod(2001) + theRandom.nextUIntMod(2001) - 2000;

      score += calc_score_delta_horizontal(x, y, v - estimated_extra_length_horizontal[x][y]);
      score -= (double) v * (double) v * extra_penalty_factor - (double) estimated_extra_length_horizontal[x][y] * (double) estimated_extra_length_horizontal[x][y] * extra_penalty_factor;

      if (score >= last_score) {
        estimated_extra_length_horizontal[x][y] = v;
        last_score = score;
      }
      else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
        estimated_extra_length_horizontal[x][y] = v;
        last_score = score;
      }
      else {
        score += calc_score_delta_horizontal(x, y, -(v - estimated_extra_length_horizontal[x][y]));
        score = last_score;
      }
    }
    else if (roll < 1.00) { // update extra vertical
      int x = theRandom.nextUIntMod(h-1);
      int y = theRandom.nextUIntMod(w);
      int v = theRandom.nextUIntMod(2001) + theRandom.nextUIntMod(2001) - 2000;

      score += calc_score_delta_vertical(x, y, v - estimated_extra_length_vertical[x][y]);
      score -= (double) v * (double) v * extra_penalty_factor - (double) estimated_extra_length_vertical[x][y] * (double) estimated_extra_length_vertical[x][y] * extra_penalty_factor;

      if (score >= last_score) {
        estimated_extra_length_vertical[x][y] = v;
        last_score = score;
      }
      else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
        estimated_extra_length_vertical[x][y] = v;
        last_score = score;
      }
      else {
        score += calc_score_delta_vertical(x, y, -(v - estimated_extra_length_vertical[x][y]));
        score = last_score;
      }
    }

    iter_count++;
    temperature *= 1.0 - decay_rate;
  }

  // cerr << "turn_count = " << setw(3) << turn_count << ", iter_count = " << setw(5) << iter_count << ", score = " << setw(12) << (turn_count == 0 ? score : score / turn_count) << ", temperature = " << temperature << endl;

  // find shortest estimated path
  vector<vector<bool> > visited(h, vector<bool>(w));
  vector<vector<char> > direction(h, vector<char>(w));
  priority_queue<tuple<double, int, int, char>, vector<tuple<double, int, int, char> >, greater<tuple<double, int, int, char> > > que;
  que.emplace(0.0, sx, sy, '-');
  while (!que.empty()) {
    auto [dist, px, py, last_direction] = que.top();
    que.pop();
    if (visited[px][py]) continue;
    visited[px][py] = true;
    direction[px][py] = last_direction;
    if (px == tx && py == ty) break;

    const vector<char> delta_char = {'L', 'R', 'U', 'D'};
    const vector<int> dx = { 0,  0, -1,  1};
    const vector<int> dy = {-1,  1,  0,  0};
    for (int d = 0; d < 4; d++) {
      if (px + dx[d] < 0 || px + dx[d] >= h || py + dy[d] < 0 || py + dy[d] >= w) continue;
      if (!visited[px+dx[d]][py+dy[d]]) {
        if (dx[d] == 0) que.emplace(dist + estimated_base_length_horizontal[px][py+(dy[d]-1)/2 < estimated_base_split_horizontal[px]] + estimated_extra_length_horizontal[px][py+(dy[d]-1)/2], px + dx[d], py + dy[d], delta_char[d]);
        else            que.emplace(dist + estimated_base_length_vertical[py][px+(dx[d]-1)/2 < estimated_base_split_vertical[py]]     + estimated_extra_length_vertical[px+(dx[d]-1)/2][py],   px + dx[d], py + dy[d], delta_char[d]);
      }
    }
  }

  string path = "";
  {
    int px = tx;
    int py = ty;
    const vector<char> delta_char = {'L', 'R', 'U', 'D'};
    const vector<int> dx = { 0,  0, -1,  1};
    const vector<int> dy = {-1,  1,  0,  0};
    while (!(px == sx && py == sy)) {
      for (int d = 0; d < 4; d++) {
        if (direction[px][py] == delta_char[d]) {
          path.push_back(delta_char[d]);
          px -= dx[d]; // move backward
          py -= dy[d];
        }
      }
    }
  }
  reverse(path.begin(), path.end());

  return path;
}

void solve() {
  while (turn_count < turn_limit) {
    auto [sx, sy, tx, ty] = get_input();

    string path;
    if (turn_count < 0) path = solve_long_path(sx, sy, tx, ty);
    else path = solve_simulated_annealing(sx, sy, tx, ty);

    // output and update history
    int path_length = output_and_get_result(path);
    result_history[turn_count] = path_length;
    {
      int px = sx;
      int py = sy;
      const vector<char> delta_char = {'L', 'R', 'U', 'D'};
      const vector<int> dx = { 0,  0, -1,  1};
      const vector<int> dy = {-1,  1,  0,  0};
      for (int i = 0; i < (int) path.size(); i++) {
        for (int d = 0; d < 4; d++) {
          if (path[i] == delta_char[d]) {
            if (dx[d] == 0) {
              path_length_estimation[turn_count] += estimated_base_length_horizontal[px][py+(dy[d]-1)/2 < estimated_base_split_horizontal[px]] + estimated_extra_length_horizontal[px][py+(dy[d]-1)/2];
              edge_used_horizontal[px][py+(dy[d]-1)/2].push_back(turn_count);
            }
            else {
              path_length_estimation[turn_count] += estimated_base_length_vertical[py][px+(dx[d]-1)/2 < estimated_base_split_vertical[py]] + estimated_extra_length_vertical[px+(dx[d]-1)/2][py];
              edge_used_vertical[px+(dx[d]-1)/2][py].push_back(turn_count);
            }

            px += dx[d];
            py += dy[d];
            break;
          }
        }
      }
      score += 1.0 - (1.0 - (double) min(result_history[turn_count], path_length_estimation[turn_count]) / (double) max(result_history[turn_count], path_length_estimation[turn_count])) * (1.0 - (double) min(result_history[turn_count], path_length_estimation[turn_count]) / (double) max(result_history[turn_count], path_length_estimation[turn_count]));
      last_score = score;
    }

    turn_count++;
  }
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  if (argc > 1) base_penalty_center = strtol(argv[1], nullptr, 10);
  if (argc > 2) base_penalty_factor = strtod(argv[2], nullptr);
  if (argc > 3) extra_penalty_factor = strtod(argv[3], nullptr);
  if (argc > 4) base_temperature = strtod(argv[4], nullptr);
  if (argc > 5) decay_rate = strtod(argv[5], nullptr);

  theRandom.x = ((unsigned long long int) random_device()() << 32) | (unsigned long long int) random_device()();

  solve();

  // for (int i = 0; i < h; i++) cerr << "h " << i << " " << estimated_base_length_horizontal[i][1] << " " << estimated_base_length_horizontal[i][0] << " " << estimated_base_split_horizontal[i] << endl;
  // for (int j = 0; j < w; j++) cerr << "v " << j << " " << estimated_base_length_vertical[j][1] << " " << estimated_base_length_vertical[j][0] << " " << estimated_base_split_vertical[j] << endl;

  return 0;
}
