#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx2")

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
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <random>
#include <functional>
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
typedef unsigned char uchar;

const ll mod = 1000000007;

timer theTimer;
xorshift64 theRandom;
mt19937 theMersenne(1);

// hyper parameters
double hyper_init_increase_factor = 3.9166373254218074;
double hyper_destruct_base_hardness_factor = 0.9357325032510382;
double hyper_destruct_hardness_increase_factor = 1.0769442563685678;
double hyper_cost_estimation_hardness_factor = 1.2076692814852833e-05;
double hyper_cost_estimation_action_count_factor = 1.9437159815521752;

// structs
struct unionfind {
  vector<int> group;
  vector<int> rank;

  unionfind() {
    unionfind(0);
  }

  unionfind(int n) {
    group = vector<int>(n);
    rank = vector<int>(n);
    for (int i = 0; i < n; i++) group[i] = i;
  }

  int root(int x) {
    if (group[x] == x) return x;
    return group[x] = root(group[x]);
  }

  void unite(int x, int y) {
    int rx = root(x);
    int ry = root(y);
    if (rank[rx] > rank[ry]) group[ry] = rx;
    else if (rank[ry] > rank[rx]) group[rx] = ry;
    else if (rx != ry) {
      group[ry] = rx;
      rank[rx]++;
    }
  }

  bool same(int x, int y) {
    int rx = root(x);
    int ry = root(y);
    return rx == ry;
  }
};

struct position {
  int x;
  int y;

  position() = default;
  position(int x, int y): x(x), y(y) {}
  position(Pii p): x(p.first), y(p.second) {}
};

// enums
enum excavation_result {
  INVALID = -1,
  NOT_DESTROYED = 0,
  DESTROYED = 1,
  COMPLETED = 2
};

// constants
struct {
  const vector<int> x = {  0,  -1,   0,   1};
  const vector<int> y = { -1,   0,   1,   0};
} delta;

constexpr int map_size = 200;

// inputs
int water_num;
int house_num;
int base_stamina_consumption;

vector<position> water_position;
vector<position> house_position;

// internals
int total_score;

vector<vector<bool>> bedrock_destroyed;
unionfind destroyed_group;

vector<vector<double>> estimated_hardness;

int target_house;
vector<vector<int>> distance_from_water;
priority_queue<pair<double, Pii>, vector<pair<double, Pii>>, greater<pair<double, Pii>>> destruction_que;

void get_first_input() {
  int _map_size;
  cin >> _map_size >> water_num >> house_num >> base_stamina_consumption;
  water_position = vector<position>(water_num);
  for (int i = 0; i < water_num; i++) cin >> water_position[i].x >> water_position[i].y;
  house_position = vector<position>(house_num);
  for (int i = 0; i < house_num; i++) cin >> house_position[i].x >> house_position[i].y;
}

// psytester用のメタデータを出力する
void output_psytester_metadata() {
  auto output_metadata = [](string name, int value) {
    cerr << "[DATA] " << name << " = " << value << endl;
  };

  output_metadata("W", water_num);
  output_metadata("H", house_num);
  output_metadata("C", base_stamina_consumption);
}

inline bool within_board(position pos) {
  return 0 <= pos.x && pos.x < map_size && 0 <= pos.y && pos.y < map_size;
}

void recalculate_distance_from_water() {
  queue<Pii> que;
  for (int i = 0; i < map_size; i++) {
    for (int j = 0; j < map_size; j++) {
      distance_from_water[i][j] = 999999;
      for (auto &w: water_position) {
        if (destroyed_group.same(i * map_size + j, w.x * map_size + w.y)) {
          que.emplace(i, j);
          distance_from_water[i][j] = 0;
        }
      }
    }
  }
  while (!que.empty()) {
    auto [cx, cy] = que.front();
    que.pop();
    for (int d = 0; d < 4; d++) {
      if (within_board(position(cx + delta.x[d], cy + delta.y[d])) && distance_from_water[cx + delta.x[d]][cy + delta.y[d]] == 999999) {
        distance_from_water[cx + delta.x[d]][cy + delta.y[d]] = distance_from_water[cx][cy] + 1;
        que.emplace(cx + delta.x[d], cy + delta.y[d]);
      }
    }
  }
}

void init() {
  output_psytester_metadata();

  total_score = 0;

  bedrock_destroyed = vector<vector<bool>>(map_size, vector<bool>(map_size));
  destroyed_group = unionfind(map_size * map_size);

  estimated_hardness = vector<vector<double>>(map_size, vector<double>(map_size, -1.0));

  target_house = -1;
  distance_from_water = vector<vector<int>>(map_size, vector<int>(map_size));
  recalculate_distance_from_water();
}

excavation_result excavate(position pos, int power) {
  cout << pos.x << " " << pos.y << " " << power << endl;
  total_score += power + base_stamina_consumption;

  int result_code;
  cin >> result_code;
  return (excavation_result) result_code;
}

void comment(string str) {
  cout << "#" << str << endl;
}

double destroy_bedrock(position pos) {
  if (estimated_hardness[pos.x][pos.y] == -1.0) {
    const double base_hardness = 10.0;
    const double hardness_increase_factor = hyper_init_increase_factor;
    double current_hardness = base_hardness;
    int next_power = (int) current_hardness;
    while (true) {
      if (next_power <= 0) {
        double next_hardness = min(5000.0, current_hardness * hardness_increase_factor);
        next_power = min(5000, (int) next_hardness - (int) current_hardness);
        current_hardness = next_hardness;
        continue;
      }

      auto res = excavate(pos, next_power);
      if (res == INVALID) {
        cerr << "[Error] Invalid excavation at x = " << pos.x << ", y = " << pos.y << ", power = " << next_power << endl;
        return -1.0;
      }
      else if (res == COMPLETED) {
        return -1.0;
      }
      else if (res == NOT_DESTROYED) {
        double next_hardness = min(5000.0, current_hardness * hardness_increase_factor);
        next_power = min(5000, (int) next_hardness - (int) current_hardness);
        current_hardness = next_hardness;
      }
      else {
        bedrock_destroyed[pos.x][pos.y] = true;
        for (int d = 0; d < 4; d++) {
          if (within_board(position(pos.x + delta.x[d], pos.y + delta.y[d])) && bedrock_destroyed[pos.x + delta.x[d]][pos.y + delta.y[d]]) {
            destroyed_group.unite(pos.x * map_size + pos.y, (pos.x + delta.x[d]) * map_size + (pos.y + delta.y[d]));
          }
        }
        return current_hardness;
      }
    }
  }
  else {
    const double base_hardness_factor = hyper_destruct_base_hardness_factor;
    const double hardness_increase_factor = hyper_destruct_hardness_increase_factor;
    double current_hardness = clamp(estimated_hardness[pos.x][pos.y] * base_hardness_factor, 10.0, 5000.0);
    int next_power = (int) current_hardness;
    while (true) {
      if (next_power <= 0) {
        double next_hardness = min(5000.0, current_hardness * hardness_increase_factor);
        next_power = min(5000, (int) next_hardness - (int) current_hardness);
        current_hardness = next_hardness;
        continue;
      }

      auto res = excavate(pos, next_power);
      if (res == INVALID) {
        cerr << "[Error] Invalid excavation at x = " << pos.x << ", y = " << pos.y << ", power = " << next_power << endl;
        return -1.0;
      }
      else if (res == COMPLETED) {
        return -1.0;
      }
      else if (res == NOT_DESTROYED) {
        double next_hardness = min(5000.0, current_hardness * hardness_increase_factor);
        next_power = min(5000, (int) next_hardness - (int) current_hardness);
        current_hardness = next_hardness;
      }
      else {
        bedrock_destroyed[pos.x][pos.y] = true;
        for (int d = 0; d < 4; d++) {
          if (within_board(position(pos.x + delta.x[d], pos.y + delta.y[d])) && bedrock_destroyed[pos.x + delta.x[d]][pos.y + delta.y[d]]) {
            destroyed_group.unite(pos.x * map_size + pos.y, (pos.x + delta.x[d]) * map_size + (pos.y + delta.y[d]));
          }
        }
        return current_hardness;
      }
    }
  }
}

double calculate_estimated_cost(position pos) {
  const double hardness_factor = hyper_cost_estimation_hardness_factor;
  const double action_count_factor = hyper_cost_estimation_action_count_factor;
  return (estimated_hardness[pos.x][pos.y] * hardness_factor + base_stamina_consumption * action_count_factor) * distance_from_water[pos.x][pos.y];
}

position decide_destruction_position() {
  // まずは水源と家の直下を調べる
  if (target_house == -1) {
    for (auto &w: water_position) {
      if (estimated_hardness[w.x][w.y] == -1.0) {
        return w;
      }
    }
    for (auto &h: house_position) {
      if (estimated_hardness[h.x][h.y] == -1.0) {
        return h;
      }
    }
    target_house = 0;
    recalculate_distance_from_water();
    destruction_que = priority_queue<pair<double, Pii>, vector<pair<double, Pii>>, greater<pair<double, Pii>>>();
    const auto &h = house_position[target_house];
    for (int d = 0; d < 4; d++) {
      if (within_board(position(h.x + delta.x[d], h.y + delta.y[d])) && !bedrock_destroyed[h.x + delta.x[d]][h.y + delta.y[d]]) {
        destruction_que.emplace(calculate_estimated_cost(position(h.x + delta.x[d], h.y + delta.y[d])), Pii(h.x + delta.x[d], h.y + delta.y[d]));
      }
    }
  }

  while (target_house < house_num) {
    const auto &h = house_position[target_house];
    bool already_connected = false;
    for (const auto &w: water_position) {
      if (destroyed_group.same(h.x * map_size + h.y, w.x * map_size + w.y)) {
        already_connected = true;
        break;
      }
    }
    if (already_connected) {
      target_house++;
      if (target_house >= house_num) {
        cerr << "[Error] All house expected to be connected, but actually not" << endl;
        exit(0);
      }
      recalculate_distance_from_water();
      destruction_que = priority_queue<pair<double, Pii>, vector<pair<double, Pii>>, greater<pair<double, Pii>>>();
      const auto &h = house_position[target_house];
      for (int d = 0; d < 4; d++) {
        if (within_board(position(h.x + delta.x[d], h.y + delta.y[d])) && !bedrock_destroyed[h.x + delta.x[d]][h.y + delta.y[d]]) {
          destruction_que.emplace(calculate_estimated_cost(position(h.x + delta.x[d], h.y + delta.y[d])), Pii(h.x + delta.x[d], h.y + delta.y[d]));
        }
      }
      continue;
    }
    break;
  }

  // 各家から一番良さそうな水源を目指す
  while (!destruction_que.empty()) {
    auto [estimated_cost, pos] = destruction_que.top();
    destruction_que.pop();
    if (bedrock_destroyed[pos.first][pos.second]) continue;
    if (estimated_cost != calculate_estimated_cost(position(pos))) {
      destruction_que.emplace(calculate_estimated_cost(position(pos)), pos);
      continue;
    }
    comment("pos = (" + to_string(pos.first) + ", " + to_string(pos.second) + "), estimated hardness = " + to_string(estimated_hardness[pos.first][pos.second]) + ", score = " + to_string(estimated_cost));
    return position(pos);
  }
  cerr << "[Error] Destruction queue is empty" << endl;
  exit(0);
}

void update_hardness_estimation(position pos, double used_power) {
  if (estimated_hardness[pos.x][pos.y] == -1.0) {
    estimated_hardness[pos.x][pos.y] = used_power;
    for (int d = 0; d < 4; d++) {
      if (within_board(position(pos.x + delta.x[d], pos.y + delta.y[d])) && !bedrock_destroyed[pos.x + delta.x[d]][pos.y + delta.y[d]] && estimated_hardness[pos.x + delta.x[d]][pos.y + delta.y[d]] == -1.0) {
        estimated_hardness[pos.x + delta.x[d]][pos.y + delta.y[d]] = used_power;
        destruction_que.emplace(calculate_estimated_cost(position(pos.x + delta.x[d], pos.y + delta.y[d])), Pii(pos.x + delta.x[d], pos.y + delta.y[d]));
      }
    }
    return;
  }

  estimated_hardness[pos.x][pos.y] = used_power;
  for (int d1 = 0; d1 < 4; d1++) {
    if (within_board(position(pos.x + delta.x[d1], pos.y + delta.y[d1]))) {
      if (bedrock_destroyed[pos.x + delta.x[d1]][pos.y + delta.y[d1]]) {
        double hardness_delta = used_power - estimated_hardness[pos.x + delta.x[d1]][pos.y + delta.y[d1]];
        for (int d2 = 0; d2 < 4; d2++) {
          if (within_board(position(pos.x + delta.x[d1] + delta.x[d2], pos.y + delta.y[d1] + delta.y[d2])) && within_board(position(pos.x + delta.x[d2], pos.y + delta.y[d2])) && !bedrock_destroyed[pos.x + delta.x[d2]][pos.y + delta.y[d2]]) {
            if (estimated_hardness[pos.x + delta.x[d2]][pos.y + delta.y[d2]] == -1.0) {
              estimated_hardness[pos.x + delta.x[d2]][pos.y + delta.y[d2]] = estimated_hardness[pos.x + delta.x[d1] + delta.x[d2]][pos.y + delta.y[d1] + delta.y[d2]] + hardness_delta;
            }
            else {
              estimated_hardness[pos.x + delta.x[d2]][pos.y + delta.y[d2]] += hardness_delta;
            }
            destruction_que.emplace(calculate_estimated_cost(position(pos.x + delta.x[d2], pos.y + delta.y[d2])), Pii(pos.x + delta.x[d2], pos.y + delta.y[d2]));
          }
        }
      }
    }
  }
}

void solve() {
  while (true) {
    auto position_to_destruct = decide_destruction_position();
    auto used_power = destroy_bedrock(position_to_destruct);
    if (used_power == -1.0) break;
    update_hardness_estimation(position_to_destruct, used_power);
  }
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  // ifstream params_file("params.txt");
  // params_file >> hyper_init_increase_factor >> hyper_destruct_base_hardness_factor >> hyper_destruct_hardness_increase_factor >> hyper_cost_estimation_hardness_factor >> hyper_cost_estimation_action_count_factor;

  get_first_input();

  init();
  solve();

  return 0;
}
