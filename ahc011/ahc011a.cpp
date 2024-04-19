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
typedef unsigned long long int ull;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

timer theTimer;
xorshift64 theRandom;
mt19937 theMersenne(1);

// hyper parameters
double hyper_anneal_1_base_temperature = 2.6854432121290848;
double hyper_anneal_1_target_temperature = 0.19665320545186682;

// structs
struct unionfind {
  vector<int> group;

  unionfind(int n) {
    group = vector<int>(n);
    reset();
  }

  void reset() {
    for (int i = 0; i < (int) group.size(); i++) group[i] = i;
  }

  int root(int x) {
    if (group[x] == x) return x;
    return group[x] = root(group[x]);
  }

  void unite(int x, int y) {
    int rx = root(x);
    int ry = root(y);
    if (rx == ry) return;
    group[rx] = ry;
  }

  bool same(int x, int y) {
    int rx = root(x);
    int ry = root(y);
    return rx == ry;
  }
};

// constants
struct {
  const vector<int>  x = {  0,  -1,   0,   1};
  const vector<int>  y = { -1,   0,   1,   0};
  const vector<char> c = {'L', 'U', 'R', 'D'};
  const vector<char> b = {  1,   2,   4,   8}; // tile representaion
} delta;

constexpr int tile_type_max = 16;

// inputs
int n;
int t;
vector<vector<char> > board_orig;

// outputs
string ans;

// internals
vector<vector<char> > board_target;
vector<vector<pair<char, char> > > tile_assignment;

bool within_board(int x, int y) {
  return 0 <= x && x < n && 0 <= y && y < n;
}

void receive_input() {
  cin >> n >> t;
  board_orig = vector<vector<char> >(n, vector<char>(n));
  for (auto &x: board_orig) {
    for (auto &y: x) {
      char c;
      cin >> c;
      y = stoi(string({c}), nullptr, 16);
    }
  }
}

void init() {
  board_target = board_orig;
  tile_assignment = vector<vector<pair<char, char> > >(n, vector<pair<char, char> >(n));
}

void solve() {
  {
    // for (int i = 0; i < n; i++) {
    //   for (int j = 0; j < n; j++) {
    //     if (board_target[i][j] == 0) {
    //       swap(board_target[i][j], board_target[n-1][n-1]);
    //     }
    //   }
    // }

    auto calc_score = [&]() {
      static unionfind uf(n * n);
      uf.reset();

      int score = 0;

      for (int i = 0; i < n; i++) {
        if (board_target[i][0] & 1) score -= 2;
        if (board_target[i][n-1] & 4) score -= 2;
      }
      for (int j = 0; j < n; j++) {
        if (board_target[0][j] & 2) score -= 2;
        if (board_target[n-1][j] & 8) score -= 2;
      }
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n-1; j++) {
          if (((board_target[i][j] & 4) >> 2) ^ (board_target[i][j+1] & 1)) score -= 2;
        }
      }
      for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n; j++) {
          if (((board_target[i][j] & 8) >> 2) ^ (board_target[i+1][j] & 2)) score -= 2;
        }
      }

      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          if (i < n-1 && ((board_target[i][j] & 8) >> 2) & (board_target[i+1][j] & 2)) {
            uf.unite(i * n + j, (i + 1) * n + j);
          }
          if (j < n-1 && ((board_target[i][j] & 4) >> 2) & (board_target[i][j+1] & 1)) {
            if (uf.same(i * n + j, i * n + j + 1)) score -= 2;
            else uf.unite(i * n + j, i * n + j + 1);
          }
        }
      }

      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          if (uf.same(i * n + j, (n / 2) * n + (n / 2))) score += 3;
        }
      }

      return score;
    };

    auto best_board_target = board_target;

    int score = calc_score();
    int last_score = score;
    int best_score = score;

    const double base_temperature = hyper_anneal_1_base_temperature;
    const double target_temperature = hyper_anneal_1_target_temperature;
    // const double decay_rate = 4e-5;
    double temperature = base_temperature;

    double time_start = theTimer.time();
    const double time_limit = 1.000;
    int iter_count = 0;

    while (theTimer.time() < time_limit) {
      double roll = theRandom.nextDouble();
      if (roll < 1.00) {
        int i1 = theRandom.nextUIntMod(n);
        int j1 = theRandom.nextUIntMod(n);
        int i2 = theRandom.nextUIntMod(n);
        int j2 = theRandom.nextUIntMod(n);
        if (i1 == i2 && j1 == j2) continue;
        // if (i1 == n - 1 && j1 == n - 1) continue;
        // if (i2 == n - 1 && j2 == n - 1) continue;

        swap(board_target[i1][j1], board_target[i2][j2]);

        score = calc_score();

        #ifdef DEBUG
        if (iter_count % 10000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_board_target = board_target;
            best_score = score;
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          swap(board_target[i1][j1], board_target[i2][j2]);
          score = last_score;
        }
      }

      // temperature *= 1.0 - decay_rate;
      temperature = exp(log(base_temperature) - ((log(base_temperature) - log(target_temperature)) * ((theTimer.time() - time_start) * (1.0 / (time_limit - time_start)))));
      iter_count++;
    }

    board_target = best_board_target;

    cerr << "iter_count   = " << iter_count << endl;
    cerr << "score        = " << score << endl;
    cerr << "best_score   = " << best_score << endl;
    cerr << "temperature  = " << temperature << endl;
  }

  {
    vector<vector<pair<char, char> > > target_tile_position_per_type(tile_type_max);
    vector<vector<pair<char, char> > > orig_tile_position_per_type(tile_type_max);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        target_tile_position_per_type[board_target[i][j]].emplace_back(i, j);
        orig_tile_position_per_type[board_orig[i][j]].emplace_back(i, j);
      }
    }
    {
      auto target_tile_position_per_type_que = target_tile_position_per_type;
      int empty_tile_distance = 0;
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          if (board_orig[i][j] == 0) {
            empty_tile_distance = abs(target_tile_position_per_type_que[board_orig[i][j]].back().first - i) + abs(target_tile_position_per_type_que[board_orig[i][j]].back().second - j);
          }
          tile_assignment[i][j] = target_tile_position_per_type_que[board_orig[i][j]].back();
          target_tile_position_per_type_que[board_orig[i][j]].pop_back();
        }
      }
      auto tile_assignment_tmp = tile_assignment;
      int swap_count = 0;
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          while (tile_assignment_tmp[i][j] != pair<char, char>(i, j)) {
            auto tile_to_swap = tile_assignment_tmp[i][j];
            swap(tile_assignment_tmp[i][j], tile_assignment_tmp[tile_to_swap.first][tile_to_swap.second]);
            swap_count++;
          }
        }
      }
      if (empty_tile_distance % 2 != swap_count % 2) {
        // fix parity
        for (int i = 0; i < tile_type_max; i++) {
          if (orig_tile_position_per_type[i].size() >= 2) {
            swap(tile_assignment[orig_tile_position_per_type[i][0].first][orig_tile_position_per_type[i][0].second], tile_assignment[orig_tile_position_per_type[i][1].first][orig_tile_position_per_type[i][1].second]);
            break;
          }
        }
      }
    }
    auto calc_score = [&]() {
      int score = 0;
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          score -= abs(tile_assignment[i][j].first - i) + abs(tile_assignment[i][j].second - j);
        }
      }
      return score;
    };
    int score = calc_score();
    int last_score = score;
    int best_score = score;

    const double base_temperature = 1e1;
    const double target_temperature = 1e-1;
    // const double decay_rate = 4e-5;
    double temperature = base_temperature;

    double time_start = theTimer.time();
    const double time_limit = 1.100;
    int iter_count = 0;

    while (theTimer.time() < time_limit) {
      double roll = theRandom.nextDouble();
      if (roll < 1.00) {
        int t1 = theRandom.nextUIntMod(tile_type_max - 1) + 1;
        int t2 = theRandom.nextUIntMod(tile_type_max - 1) + 1;
        if (orig_tile_position_per_type[t1].size() < 2) continue;
        if (orig_tile_position_per_type[t2].size() < 2) continue;
        int p11 = theRandom.nextUIntMod(orig_tile_position_per_type[t1].size());
        int p12 = theRandom.nextUIntMod(orig_tile_position_per_type[t1].size());
        if (p11 == p12) continue;
        int p21 = theRandom.nextUIntMod(orig_tile_position_per_type[t2].size());
        int p22 = theRandom.nextUIntMod(orig_tile_position_per_type[t2].size());
        if (p21 == p22) continue;

        swap(tile_assignment[orig_tile_position_per_type[t1][p11].first][orig_tile_position_per_type[t1][p11].second], tile_assignment[orig_tile_position_per_type[t1][p12].first][orig_tile_position_per_type[t1][p12].second]);
        swap(tile_assignment[orig_tile_position_per_type[t2][p21].first][orig_tile_position_per_type[t2][p21].second], tile_assignment[orig_tile_position_per_type[t2][p22].first][orig_tile_position_per_type[t2][p22].second]);

        score = calc_score();

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
          swap(tile_assignment[orig_tile_position_per_type[t2][p21].first][orig_tile_position_per_type[t2][p21].second], tile_assignment[orig_tile_position_per_type[t2][p22].first][orig_tile_position_per_type[t2][p22].second]);
          swap(tile_assignment[orig_tile_position_per_type[t1][p11].first][orig_tile_position_per_type[t1][p11].second], tile_assignment[orig_tile_position_per_type[t1][p12].first][orig_tile_position_per_type[t1][p12].second]);
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

  {
    class rolling_hash {
    private:
      const ull base = (1ULL << 8) + 1;
      const ull mask30 = (1ULL << 30) - 1;
      const ull mask31 = (1ULL << 31) - 1;
      const ull mod = (1ULL << 61) - 1;
      const ull mask61 = (1ULL << 61) - 1;
      const ull positivizer = 4ULL * mod;

      vector<ull> base_pow;

      ull mul(ull a, ull b) {
        ull au = a >> 31;
        ull ad = a & mask31;
        ull bu = b >> 31;
        ull bd = b & mask31;
        ull mid = ad * bu + au * bd;
        ull midu = mid >> 30;
        ull midd = mid & mask30;
        return au * bu * 2 + midu + (midd << 31) + ad * bd;
      }

      ull calc_mod(ull x) {
        ull xu = x >> 61;
        ull xd = x & mask61;
        ull res = xu + xd;
        if (res >= mod) res -= mod;
        return res;
      }

      ull _add(ull hash, ull val, int idx) {
        return calc_mod(hash + mul(val, base_pow[idx]));
      }

      ull _subtract(ull hash, ull val, int idx) {
        return calc_mod(positivizer + hash - mul(val, base_pow[idx]));
      }

    public:
      rolling_hash(int n) {
        assert(n > 0);
        base_pow = vector<ull>(n);
        base_pow[0] = 1;
        for (int i = 1; i < n; i++) base_pow[i] = calc_mod(mul(base_pow[i-1], base));
      }

      ull add(ull hash, ull val, int idx) {
        return _add(hash, val + 1ULL, idx);
      }

      ull subtract(ull hash, ull val, int idx) {
        return _subtract(hash, val + 1ULL, idx);
      }

      ull get_hash(vector<unsigned char> &board_status) {
        assert(base_pow.size() == board_status.size());
        ull hash = 0;
        for (int i = 0; i < (int) board_status.size(); i++) hash = add(hash, (ull) board_status[i], i);
        return hash;
      }
    };

    struct search_state {
      vector<unsigned char> board_status;
      ull board_hash;
      string movement;
      int distance;
      int empty_x;
      int empty_y;

      search_state(int n): board_status(vector<unsigned char>(n)), board_hash(0ULL), movement(""), distance(0), empty_x(0), empty_y(0) {}

      bool operator<(const search_state &rhs) const {
        return this->distance < rhs.distance;
      }
    };

    auto calc_score = [&](const vector<unsigned char> &board_status, int empty_x, int empty_y) {
      int score = 0;

      // manhattan distance
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          if (!(i == empty_x && j == empty_y)) score += abs((int) (board_status[i * n + j] >> 4) - i) + abs((int) (board_status[i * n + j] & 0x0f) - j);
        }
      }
      if (score == 0) return score;

      // linear conflict
      static vector<bool> linear_conflict_tile(n * n);
      linear_conflict_tile.assign(n * n, false);
      for (int i = 0; i < n; i++) {
        int last_col = -1;
        for (int j = 0; j < n; j++) {
          if (i == empty_x && j == empty_y) continue;
          if ((board_status[i * n + j] >> 4) == (unsigned char) i) {
            if (last_col > (int) (board_status[i * n + j] & 0x0f)) {
              score += 2;
              linear_conflict_tile[i * n + j] = true;
            }
            last_col = (int) (board_status[i * n + j] & 0x0f);
          }
        }
      }
      for (int j = 0; j < n; j++) {
        int last_row = -1;
        for (int i = 0; i < n; i++) {
          if (i == empty_x && j == empty_y) continue;
          if ((board_status[i * n + j] & 0x0f) == (unsigned char) j) {
            if (last_row > (int) (board_status[i * n + j] >> 4)) {
              score += 2;
              linear_conflict_tile[i * n + j] = true;
            }
            last_row = (int) (board_status[i * n + j] >> 4);
          }
        }
      }

      // incomplete tile connectivity
      static unionfind uf(n * n);
      uf.reset();
      static vector<bool> incomplete_tile(n * n);
      incomplete_tile.assign(n * n, false);
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          if ((board_status[i * n + j] >> 4) != (unsigned char) i || (board_status[i * n + j] & 0x0f) != (unsigned char) j || (i == empty_x && j == empty_y)) {
            incomplete_tile[i * n + j] = true;
            if (i > 0 && incomplete_tile[(i - 1) * n + j]) uf.unite(i * n + j, (i - 1) * n + j);
            if (j > 0 && incomplete_tile[i * n + (j - 1)]) uf.unite(i * n + j, i * n + (j - 1));
          }
        }
      }
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          if (incomplete_tile[i * n + j] && !uf.same(empty_x * n + empty_y, i * n + j)) score += 100;
        }
      }

      // last tile heuristic
      int empty_target_x = (int) (board_status[empty_x * n + empty_y] >> 4);
      int empty_target_y = (int) (board_status[empty_x * n + empty_y] & 0x0f);
      if (abs((board_status[empty_target_x * n + empty_target_y] >> 4) - empty_target_x) + abs((board_status[empty_target_x * n + empty_target_y] & 0x0f) - empty_target_y) != 1) {
        bool conflict_with_linear_conflict = false;
        for (int d = 0; d < 4; d++) {
          if (within_board(empty_target_x + delta.x[d], empty_target_y + delta.y[d]) && linear_conflict_tile[(empty_target_x + delta.x[d]) * n + (empty_target_y + delta.y[d])]) {
            conflict_with_linear_conflict = true;
            break;
          }
        }
        if (!conflict_with_linear_conflict) score += 2;
      }

      return score;
    };

    auto calc_real_score = [&](const vector<unsigned char> &board_status) {
      int score = 0;
      vector<vector<bool> > visited(n, vector<bool>(n));
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          if (!visited[i][j]) {
            int tree_size = 0;
            stack<tuple<int, int, int, int> > st;
            st.emplace(i, j, -1, -1);
            while (!st.empty()) {
              auto [now_x, now_y, prev_x, prev_y] = st.top();
              st.pop();
              if (visited[now_x][now_y]) {
                tree_size = -998244353; // loop
                continue;
              }
              visited[now_x][now_y] = true;
              tree_size++;
              for (int d = 0; d < 4; d++) {
                int next_x = now_x + delta.x[d];
                int next_y = now_y + delta.y[d];
                if (within_board(next_x, next_y) && !(next_x == prev_x && next_y == prev_y) && (board_target[board_status[now_x * n + now_y] >> 4][board_status[now_x * n + now_y] & 0x0f] & delta.b[d]) && (board_target[board_status[next_x * n + next_y] >> 4][board_status[next_x * n + next_y] & 0x0f] & delta.b[d^2])) {
                  st.emplace(next_x, next_y, now_x, now_y);
                }
              }
            }
            score = max(score, tree_size);
          }
        }
      }
      return score;
    };

    rolling_hash rh(n * n);
    search_state first_search_state(n * n);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        first_search_state.board_status[i * n + j] = (tile_assignment[i][j].first << 4) | tile_assignment[i][j].second;
        if (board_orig[i][j] == 0) {
          first_search_state.empty_x = i;
          first_search_state.empty_y = j;
        }
        else {
          // first_search_state.distance += (abs((int) tile_assignment[i][j].first - i) + abs((int) tile_assignment[i][j].second - j));
        }
      }
    }
    first_search_state.distance = calc_score(first_search_state.board_status, first_search_state.empty_x, first_search_state.empty_y);
    first_search_state.board_hash = rh.get_hash(first_search_state.board_status);

    const vector<int> beam_width = {0, 0, 0, 0, 0, 0, 900, 500, 275, 180, 120};
    vector<search_state> beam_state;
    beam_state.push_back(first_search_state);
    unordered_set<ull> searched_hash;
    int best_score = 0;
    for (int turn_count = 0; turn_count < t; turn_count++) {
      cerr << turn_count << " " << beam_state[0].distance << endl;
      if (!beam_state.empty() && beam_state[0].distance == 0) {
        if (calc_real_score(beam_state[0].board_status) > best_score) ans = beam_state[0].movement;
        break;
      }
      else if (!beam_state.empty()) {
        int real_score = calc_real_score(beam_state[0].board_status);
        if (real_score > best_score) {
          ans = beam_state[0].movement;
          best_score = real_score;
        }
      }

      vector<search_state> next_beam_state;

      int searched_board_count = 0;
      for (int idx = 0; idx < (int) beam_state.size(); idx++) {
        if (searched_hash.find(beam_state[idx].board_hash) != searched_hash.end()) continue;
        for (int d = 0; d < 4; d++) {
          if (turn_count > 0 && beam_state[idx].movement[turn_count-1] == delta.c[d ^ 2]) continue; // xor 2 -> 180 degree flip; no "undo"
          else if (within_board(beam_state[idx].empty_x + delta.x[d], beam_state[idx].empty_y + delta.y[d])) {
            auto next_search_state = beam_state[idx];
            next_search_state.board_hash = rh.subtract(next_search_state.board_hash, next_search_state.board_status[ next_search_state.empty_x               * n +  next_search_state.empty_y],                next_search_state.empty_x               * n +  next_search_state.empty_y);
            next_search_state.board_hash = rh.subtract(next_search_state.board_hash, next_search_state.board_status[(next_search_state.empty_x + delta.x[d]) * n + (next_search_state.empty_y + delta.y[d])], (next_search_state.empty_x + delta.x[d]) * n + (next_search_state.empty_y + delta.y[d]));
            swap(next_search_state.board_status[next_search_state.empty_x * n + next_search_state.empty_y], next_search_state.board_status[(next_search_state.empty_x + delta.x[d]) * n + (next_search_state.empty_y + delta.y[d])]);
            next_search_state.board_hash = rh.add(next_search_state.board_hash, next_search_state.board_status[ next_search_state.empty_x               * n +  next_search_state.empty_y],                next_search_state.empty_x               * n +  next_search_state.empty_y);
            next_search_state.board_hash = rh.add(next_search_state.board_hash, next_search_state.board_status[(next_search_state.empty_x + delta.x[d]) * n + (next_search_state.empty_y + delta.y[d])], (next_search_state.empty_x + delta.x[d]) * n + (next_search_state.empty_y + delta.y[d]));
            if (searched_hash.find(next_search_state.board_hash) != searched_hash.end()) continue;
            next_search_state.movement.push_back(delta.c[d]);
            // int distance_sub_base = abs((int) (next_search_state.board_status[next_search_state.empty_x * n + next_search_state.empty_y] >> 4) - (next_search_state.empty_x + delta.x[d])) + abs((int) (next_search_state.board_status[next_search_state.empty_x * n + next_search_state.empty_y] & 0x0f) - (next_search_state.empty_y + delta.y[d]));
            // int distance_add_base = abs((int) (next_search_state.board_status[next_search_state.empty_x * n + next_search_state.empty_y] >> 4) -  next_search_state.empty_x)               + abs((int) (next_search_state.board_status[next_search_state.empty_x * n + next_search_state.empty_y] & 0x0f) -  next_search_state.empty_y);
            // next_search_state.distance -= distance_sub_base;
            // next_search_state.distance += distance_add_base;
            next_search_state.empty_x += delta.x[d];
            next_search_state.empty_y += delta.y[d];
            next_search_state.distance = calc_score(next_search_state.board_status, next_search_state.empty_x, next_search_state.empty_y);
            next_beam_state.push_back(next_search_state);
          }
        }
        searched_hash.insert(beam_state[idx].board_hash);
        searched_board_count++;
        if (searched_board_count >= beam_width[n]) break;
      }

      sort(next_beam_state.begin(), next_beam_state.end());
      beam_state = move(next_beam_state);
    }
    // if (ans == "") ans = beam_state[0].movement;
  }
}

void output_ans() {
  cout << ans << endl;
  for (auto &x: board_target) {
    for (auto &y: x) {
      cerr << hex << (int) y;
    }
    cerr << endl;
  }
  cerr << endl;
  for (auto &x: tile_assignment) {
    for (auto &y: x) {
      cerr << (int) y.first << (int) y.second;
    }
    cerr << endl;
  }
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  if (argc > 1) hyper_anneal_1_base_temperature = strtod(argv[1], nullptr);
  if (argc > 2) hyper_anneal_1_target_temperature = strtod(argv[2], nullptr);

  receive_input();

  init();
  solve();

  output_ans();

  return 0;
}
