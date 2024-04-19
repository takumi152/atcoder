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

// hyper parameters

double hyper_roll_prob_1 = 0.25052566737125076;
double hyper_roll_prob_2 = 0.4990612837803949;
double hyper_roll_prob_3 = 0.8852751396260895;
double hyper_anneal_time_limit_1 = 0.950;
double hyper_anneal_base_temperature_1 = 1e-3;
double hyper_anneal_target_temperature_1 = 1e-4;
double hyper_anneal_base_temperature_2 = 5e-4;
double hyper_anneal_target_temperature_2 = 1e-5;

// structs

// constants
constexpr int turn_limit = 200;
constexpr int board_size = 20;

struct {
  const vector<int>  x = { -1,   1,   0,   0};
  const vector<int>  y = {  0,   0,  -1,   1};
  const vector<char> d = {'U', 'D', 'L', 'R'};
} delta;

// inputs
int sx, sy, tx, ty;
double p;

vector<vector<bool> > wall_h(board_size, vector<bool>(board_size - 1));
vector<vector<bool> > wall_v(board_size - 1, vector<bool>(board_size));

// outputs
string ans_global;

// internals

bool within_board(int x, int y) {
  return x >= 0 && x < board_size && y >= 0 && y < board_size;
}

bool is_movable(int x, int y, int d) {
  assert(0 <= d && d < 4);
  if (!within_board(x + delta.x[d], y + delta.y[d])) return false;

  if (delta.x[d] != 0) {
    if (wall_v[x + min(0, delta.x[d])][y]) return false;
    else return true;
  }
  else {
    if (wall_h[x][y + min(0, delta.y[d])]) return false;
    else return true;
  }
}

void init() {

}

double simulate(string ans) {
  static vector<vector<double> > dp(board_size, vector<double>(board_size));
  for (int i = 0; i < board_size; i++) {
    for (int j = 0; j < board_size; j++) {
      dp[i][j] = 0.0;
    }
  }
  dp[sx][sy] = 1.0;
  double score = 0.0;
  int turn_count = 0;
  for (auto &x: ans) {
    if (turn_count >= turn_limit) assert(false);
    for (int d = 0; d < 4; d++) {
      if (delta.d[d] == x) {
        static vector<vector<double> > next_dp(board_size, vector<double>(board_size));
        for (int i = 0; i < board_size; i++) {
          for (int j = 0; j < board_size; j++) {
            next_dp[i][j] = 0.0;
          }
        }
        for (int i = 0; i < board_size; i++) {
          for (int j = 0; j < board_size; j++) {
            if (is_movable(i, j, d)) {
              next_dp[i + delta.x[d]][j + delta.y[d]] += dp[i][j] * (1.0 - p);
              next_dp[i][j] += dp[i][j] * p;
            }
            else {
              next_dp[i][j] += dp[i][j];
            }
          }
        }
        for (int i = 0; i < board_size; i++) {
          for (int j = 0; j < board_size; j++) {
            dp[i][j] = next_dp[i][j];
          }
        }
        break;
      }
    }
    score += dp[tx][ty] * ((double) (400 - turn_count) / 400.0);
    dp[tx][ty] = 0.0;
    turn_count++;
  }

  return score;
}

string solve_ricochet_robot() {
  vector<vector<tuple<int, int> > > prev(board_size, vector<tuple<int, int> >(board_size, make_tuple(-1, -1)));
  queue<tuple<int, int> > que;
  que.emplace(sx, sy);
  while (!que.empty()) {
    auto [nx, ny] = que.front();
    que.pop();
    if (nx == tx && ny == ty) break;
    for (int d = 0; d < 4; d++) {
      int px = nx;
      int py = ny;
      while (is_movable(px, py, d)) {
        px += delta.x[d];
        py += delta.y[d];
        if (px == tx && py == ty) break;
      }
      if (!(px == nx && py == ny) && prev[px][py] == tuple<int, int>(-1, -1)) {
        prev[px][py] = make_tuple(nx, ny);
        que.emplace(px, py);
      }
    }
  }

  if (prev[tx][ty] == tuple<int, int>(-1, -1)) return ""; // impossible

  vector<tuple<int, int> > path_info; // dir_id, length
  {
    int px = tx;
    int py = ty;
    while (!(px == sx && py == sy)) {
      assert(px == get<0>(prev[px][py]) || py == get<1>(prev[px][py]));
           if (get<0>(prev[px][py]) > px) path_info.emplace_back('U', abs(get<0>(prev[px][py]) - px));
      else if (get<0>(prev[px][py]) < px) path_info.emplace_back('D', abs(get<0>(prev[px][py]) - px));
      else if (get<1>(prev[px][py]) > py) path_info.emplace_back('L', abs(get<1>(prev[px][py]) - py));
      else if (get<1>(prev[px][py]) < py) path_info.emplace_back('R', abs(get<1>(prev[px][py]) - py));

      int nx = get<0>(prev[px][py]);
      int ny = get<1>(prev[px][py]);
      px = nx;
      py = ny;
    }
  }
  reverse(path_info.begin(), path_info.end());

  auto calc_score = [&](vector<int> &move_duration) {
    assert(path_info.size() == move_duration.size());

    double prob = 1.0;
    double score = 0.0;
    int turn_count = 0;
    for (int i = 0; i < (int) path_info.size(); i++) {
      vector<double> dp(get<1>(path_info[i]) + 1);
      dp[0] = prob;
      for (int j = 0; j < move_duration[i]; j++) {
        if (turn_count >= turn_limit) break;
        for (int k = get<1>(path_info[i]); k > 0; k--) {
          dp[k] = dp[k] + dp[k-1] * (1.0 - p);
          dp[k-1] = dp[k-1] * p;
        }
        if (i == (int) path_info.size() - 1) {
          score += dp[get<1>(path_info[i])] * ((double) (400 - turn_count) / 400.0);
          dp[get<1>(path_info[i])] = 0.0;
        }
        turn_count++;
      }
      prob = dp[get<1>(path_info[i])];
    }

    return score;
  };

  vector<int> move_duration;
  for (int i = 0; i < (int) path_info.size(); i++) {
    move_duration.push_back(get<1>(path_info[i]));
  }

  {
    double score = calc_score(move_duration);
    double last_score = score;
    double best_score = score;

    const double base_temperature = 1e-2;
    const double target_temperature = 1e-5;
    // const double decay_rate = 4e-5;
    double temperature = base_temperature;

    double time_start = theTimer.time();
    const double time_limit = 0.050;
    int iter_count = 0;

    while (theTimer.time() < time_limit) {
      double roll = theRandom.nextDouble();
      if (roll < 1.00) {
        int p = theRandom.nextUIntMod(path_info.size());
        int l = theRandom.nextUIntMod(get<1>(path_info[p]) * 4) + get<1>(path_info[p]);
        int prev_dur = move_duration[p];
        move_duration[p] = l;

        score = calc_score(move_duration);

        #ifdef DEBUG
        if (iter_count % 10000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          move_duration[p] = prev_dur;
          score = last_score;
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

  string ans = "";
  for (int i = 0; i < (int) path_info.size(); i++) {
    for (int j = 0; j < move_duration[i]; j++) {
      if (ans.size() >= turn_limit) break;
      ans.push_back(get<0>(path_info[i]));
    }
  }
  return ans;
}

string solve_random_path(string base_ans, double time_limit, double base_temperature, double target_temperature) {
  vector<char> path;
  for (auto &x: base_ans) path.push_back(x);

  // random fill
  while ((int) path.size() < turn_limit) {
    double roll = theRandom.nextDouble();
         if (roll < 0.1) path.push_back('U');
    else if (roll < 0.5) path.push_back('D');
    else if (roll < 0.6) path.push_back('L');
    else                 path.push_back('R');
  }

  auto calc_score = [&]() {
    static vector<vector<double> > dp(board_size, vector<double>(board_size));
    for (int i = 0; i < board_size; i++) {
      for (int j = 0; j < board_size; j++) {
        dp[i][j] = 0.0;
      }
    }
    dp[sx][sy] = 1.0;
    double score = 0.0;
    int turn_count = 0;
    for (auto &x: path) {
      if (turn_count >= turn_limit) assert(false);
      for (int d = 0; d < 4; d++) {
        if (delta.d[d] == x) {
          static vector<vector<double> > next_dp(board_size, vector<double>(board_size));
          for (int i = 0; i < board_size; i++) {
            for (int j = 0; j < board_size; j++) {
              next_dp[i][j] = 0.0;
            }
          }
          for (int i = 0; i < board_size; i++) {
            for (int j = 0; j < board_size; j++) {
              if (is_movable(i, j, d)) {
                next_dp[i + delta.x[d]][j + delta.y[d]] += dp[i][j] * (1.0 - p);
                next_dp[i][j] += dp[i][j] * p;
              }
              else {
                next_dp[i][j] += dp[i][j];
              }
            }
          }
          for (int i = 0; i < board_size; i++) {
            for (int j = 0; j < board_size; j++) {
              dp[i][j] = next_dp[i][j];
            }
          }
          break;
        }
      }
      score += dp[tx][ty] * ((double) (400 - turn_count) / 400.0);
      dp[tx][ty] = 0.0;
      turn_count++;
    }

    return score;
  };

  {
    double score = calc_score();
    double last_score = score;
    double best_score = score;

    // const double decay_rate = 4e-5;
    double temperature = base_temperature;

    double time_start = theTimer.time();
    // const double time_limit = 1.950;
    int iter_count = 0;

    while (theTimer.time() < time_limit) {
      double roll = theRandom.nextDouble();
      if (roll < hyper_roll_prob_1) {
        int p = theRandom.nextUIntMod(turn_limit);
        int d = theRandom.nextUIntMod(4);
        if (path[p] == d) continue;

        char prev_d = path[p];
        path[p] = delta.d[d];

        score = calc_score();

        #ifdef DEBUG
        if (iter_count % 100 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          path[p] = prev_d;
          score = last_score;
        }
      }
      else if (roll < hyper_roll_prob_2) {
        int p1 = theRandom.nextUIntMod(turn_limit);
        int p2 = theRandom.nextUIntMod(turn_limit);
        if (path[p1] == path[p2]) continue;

        swap(path[p1], path[p2]);

        score = calc_score();

        #ifdef DEBUG
        if (iter_count % 100 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          swap(path[p1], path[p2]);
          score = last_score;
        }
      }
      else if (roll < hyper_roll_prob_3) {
        int p1 = theRandom.nextUIntMod(turn_limit);
        int p2 = theRandom.nextUIntMod(turn_limit) + 1;
        if (abs(p1 - p2) <= 1) continue;

        if (p1 < p2) {
          path.insert(path.begin() + p2, path[p1]);
          path.erase(path.begin() + p1);
        }
        else {
          path.insert(path.begin() + p2, path[p1]);
          path.erase(path.begin() + p1 + 1);
        }

        score = calc_score();

        #ifdef DEBUG
        if (iter_count % 100 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          if (p1 < p2) {
            path.insert(path.begin() + p1, path[p2 - 1]);
            path.erase(path.begin() + p2);
          }
          else {
            path.insert(path.begin() + p1 + 1, path[p2]);
            path.erase(path.begin() + p2);
          }
          score = last_score;
        }
      }
      else if (roll < 1.00) {
        int p1 = theRandom.nextUIntMod(turn_limit - 1);
        if (path[p1] == path[p1 + 1]) continue;

        char prev_path = path[p1 + 1];
        path[p1 + 1] = path[p1];

        score = calc_score();

        #ifdef DEBUG
        if (iter_count % 100 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          path[p1 + 1] = prev_path;
          score = last_score;
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

  string ans;
  for (auto &x: path) ans.push_back(x);

  return ans;
}

string solve_highest_prob_shortest_path(string base_ans) {
  vector<char> path;
  for (auto &x: base_ans) path.push_back(x);

  auto calc_score = [&]() {
    static vector<vector<double> > dp(board_size, vector<double>(board_size));
    for (int i = 0; i < board_size; i++) {
      for (int j = 0; j < board_size; j++) {
        dp[i][j] = 0.0;
      }
    }
    dp[sx][sy] = 1.0;
    double score = 0.0;
    int turn_count = 0;
    for (auto &x: path) {
      if (turn_count >= turn_limit) assert(false);
      for (int d = 0; d < 4; d++) {
        if (delta.d[d] == x) {
          static vector<vector<double> > next_dp(board_size, vector<double>(board_size));
          for (int i = 0; i < board_size; i++) {
            for (int j = 0; j < board_size; j++) {
              next_dp[i][j] = 0.0;
            }
          }
          for (int i = 0; i < board_size; i++) {
            for (int j = 0; j < board_size; j++) {
              if (is_movable(i, j, d)) {
                next_dp[i + delta.x[d]][j + delta.y[d]] += dp[i][j] * (1.0 - p);
                next_dp[i][j] += dp[i][j] * p;
              }
              else {
                next_dp[i][j] += dp[i][j];
              }
            }
          }
          for (int i = 0; i < board_size; i++) {
            for (int j = 0; j < board_size; j++) {
              dp[i][j] = next_dp[i][j];
            }
          }
          break;
        }
      }
      score += dp[tx][ty] * ((double) (400 - turn_count) / 400.0);
      dp[tx][ty] = 0.0;
      turn_count++;
    }

    return score;
  };

  {
    vector<vector<char> > shortest_dir(board_size, vector<char>(board_size, '.'));
    {
      vector<vector<bool> > visited(board_size, vector<bool>(board_size));
      queue<tuple<int, int> > que;
      que.emplace(tx, ty);
      while (!que.empty()) {
        auto [nx, ny] = que.front();
        que.pop();
        if (visited[nx][ny]) continue;
        visited[nx][ny] = true;
        for (int d = 0; d < 4; d++) {
          if (!is_movable(nx, ny, d)) continue;
          if (shortest_dir[nx + delta.x[d]][ny + delta.y[d]] == '.') {
            shortest_dir[nx + delta.x[d]][ny + delta.y[d]] = delta.d[d ^ 1];
            que.emplace(nx + delta.x[d], ny + delta.y[d]);
          }
        }
      }
    }

    vector<char> best_path = path;
    double best_score = calc_score();
    for (int k = 0; k < (int) base_ans.size(); k++) {
      path.clear();

      static vector<vector<double> > dp(board_size, vector<double>(board_size));
      for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
          dp[i][j] = 0.0;
        }
      }
      dp[sx][sy] = 1.0;
      double score = 0.0;
      int turn_count = 0;
      while (turn_count < turn_limit) {
        char x;
        if (turn_count < k) x = base_ans[turn_count];
        else {
          int bx = 0;
          int by = 0;
          double best_prob = 0.0;
          for (int i = 0; i < board_size; i++) {
            for (int j = 0; j < board_size; j++) {
              if (dp[i][j] > best_prob) {
                bx = i;
                by = j;
                best_prob = dp[i][j];
              }
            }
          }
          x = shortest_dir[bx][by];
        }
        path.push_back(x);
        for (int d = 0; d < 4; d++) {
          if (delta.d[d] == x) {
            static vector<vector<double> > next_dp(board_size, vector<double>(board_size));
            for (int i = 0; i < board_size; i++) {
              for (int j = 0; j < board_size; j++) {
                next_dp[i][j] = 0.0;
              }
            }
            for (int i = 0; i < board_size; i++) {
              for (int j = 0; j < board_size; j++) {
                if (is_movable(i, j, d)) {
                  next_dp[i + delta.x[d]][j + delta.y[d]] += dp[i][j] * (1.0 - p);
                  next_dp[i][j] += dp[i][j] * p;
                }
                else {
                  next_dp[i][j] += dp[i][j];
                }
              }
            }
            for (int i = 0; i < board_size; i++) {
              for (int j = 0; j < board_size; j++) {
                dp[i][j] = next_dp[i][j];
              }
            }
            break;
          }
        }
        score += dp[tx][ty] * ((double) (400 - turn_count) / 400.0);
        dp[tx][ty] = 0.0;
        turn_count++;
      }
      if (score > best_score) {
        best_path = path;
        best_score = score;
      }
    }
    path = best_path;
  }

  // random fill
  while ((int) path.size() < turn_limit) {
    double roll = theRandom.nextDouble();
         if (roll < 0.1) path.push_back('U');
    else if (roll < 0.5) path.push_back('D');
    else if (roll < 0.6) path.push_back('L');
    else                 path.push_back('R');
  }

  {
    double score = calc_score();
    double last_score = score;
    double best_score = score;

    const double base_temperature = 1e-3;
    const double target_temperature = 1e-5;
    // const double decay_rate = 4e-5;
    double temperature = base_temperature;

    double time_start = theTimer.time();
    const double time_limit = 1.950;
    int iter_count = 0;

    while (theTimer.time() < time_limit) {
      double roll = theRandom.nextDouble();
      if (roll < 0.40) {
        int p = theRandom.nextUIntMod(turn_limit);
        int d = theRandom.nextUIntMod(4);
        if (path[p] == d) continue;

        char prev_d = path[p];
        path[p] = delta.d[d];

        score = calc_score();

        #ifdef DEBUG
        if (iter_count % 100 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          path[p] = prev_d;
          score = last_score;
        }
      }
      else if (roll < 0.50) {
        int p1 = theRandom.nextUIntMod(turn_limit);
        int p2 = theRandom.nextUIntMod(turn_limit);
        if (path[p1] == path[p2]) continue;

        swap(path[p1], path[p2]);

        score = calc_score();

        #ifdef DEBUG
        if (iter_count % 100 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          swap(path[p1], path[p2]);
          score = last_score;
        }
      }
      else if (roll < 1.00) {
        int p1 = theRandom.nextUIntMod(turn_limit);
        int p2 = theRandom.nextUIntMod(turn_limit) + 1;
        if (abs(p1 - p2) <= 1) continue;

        if (p1 < p2) {
          path.insert(path.begin() + p2, path[p1]);
          path.erase(path.begin() + p1);
        }
        else {
          path.insert(path.begin() + p2, path[p1]);
          path.erase(path.begin() + p1 + 1);
        }

        score = calc_score();

        #ifdef DEBUG
        if (iter_count % 100 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          if (p1 < p2) {
            path.insert(path.begin() + p1, path[p2 - 1]);
            path.erase(path.begin() + p2);
          }
          else {
            path.insert(path.begin() + p1 + 1, path[p2]);
            path.erase(path.begin() + p2);
          }
          score = last_score;
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

  string ans;
  for (auto &x: path) ans.push_back(x);

  return ans;
}

void solve() {
  vector<string> ans_candidate;
  ans_candidate.push_back(solve_ricochet_robot());
  ans_candidate.push_back(solve_random_path("", hyper_anneal_time_limit_1, hyper_anneal_base_temperature_1, hyper_anneal_target_temperature_1));
  ans_candidate.push_back(solve_random_path(ans_candidate[1], 1.950, hyper_anneal_base_temperature_2, hyper_anneal_target_temperature_2));
  // ans_candidate.push_back(solve_highest_prob_shortest_path(ans_candidate[1]));

  string best_ans = "";
  double best_score = 0.0;
  for (auto &ans: ans_candidate) {
    double score = simulate(ans);
    if (score > best_score) {
      best_ans = ans;
      best_score = score;
    }
  }

  cerr << "global_score = " << best_score << endl;
  ans_global = best_ans;
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  if (argc > 1) hyper_roll_prob_1 = strtod(argv[1], nullptr);
  if (argc > 2) hyper_roll_prob_2 = strtod(argv[2], nullptr);
  if (argc > 3) hyper_roll_prob_3 = strtod(argv[3], nullptr);
  if (argc > 4) hyper_anneal_time_limit_1 = strtod(argv[4], nullptr);
  if (argc > 5) hyper_anneal_base_temperature_1 = strtod(argv[5], nullptr);
  if (argc > 6) hyper_anneal_target_temperature_1 = strtod(argv[6], nullptr);
  if (argc > 7) hyper_anneal_base_temperature_2 = strtod(argv[7], nullptr);
  if (argc > 8) hyper_anneal_target_temperature_2 = strtod(argv[8], nullptr);

  cin >> sx >> sy >> tx >> ty >> p;
  for (auto &x: wall_h) {
    for (int i = 0; i < (int) x.size(); i++) {
      char c;
      cin >> c;
      x[i] = (c == '1');
    }
  }
  for (auto &x: wall_v) {
    for (int i = 0; i < (int) x.size(); i++) {
      char c;
      cin >> c;
      x[i] = (c == '1');
    }
  }

  init();
  solve();

  cout << ans_global << endl;

  return 0;
}
