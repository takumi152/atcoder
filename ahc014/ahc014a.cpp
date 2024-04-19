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
int hyper_init_bonus_base = 30000;
double hyper_init_annealing_temperature_min_n_begin = 20785.780726169643;
double hyper_init_annealing_temperature_min_n_end = 2066.544589665905;
double hyper_init_annealing_temperature_max_n_begin = 3679.084567475982;
double hyper_init_annealing_temperature_max_n_end = 232.48946822650458;
double hyper_init_remove_descend_extra_remove_chance = 0.5056632805093586;
double hyper_init_remove_full_extra_remove_chance = 0.14704418795983648;
double hyper_next_annealing_temperature_min_n_begin = 20785.780726169643;
double hyper_next_annealing_temperature_min_n_end = 2066.544589665905;
double hyper_next_annealing_temperature_max_n_begin = 3679.084567475982;
double hyper_next_annealing_temperature_max_n_end = 232.48946822650458;
double hyper_next_remove_descend_extra_remove_chance = 0.5056632805093586;
double hyper_next_remove_full_extra_remove_chance = 0.14704418795983648;

// structs
template <>
struct std::hash<Pii> {
  typedef std::size_t result_type;
  std::size_t operator()(const Pii& p) const noexcept {
    return (p.first << 6) + p.second;
  }
};

template <>
struct std::hash<array<Pii, 4>> {
  typedef std::size_t result_type;
  std::size_t operator()(const array<Pii, 4>& p) const noexcept {
    return (p[0].first << 18) + (p[0].second << 12) + (p[1].first << 6) + p[1].second;
  }
};

// enums
// single direction (dir) / direction set (dirset) bitmask
//  c  4  6   5  4  3
//   \ | /     \ | /
// 8 -   - 2 6 -   - 2
//   / | \     / | \ 
//  9  1  3   7  0  1
enum directions {
  DIR_D  = 0,
  DIR_DR = 1,
  DIR_R  = 2,
  DIR_UR = 3,
  DIR_U  = 4,
  DIR_UL = 5,
  DIR_L  = 6,
  DIR_DL = 7,
};

// constants
// direction set to search rectangles from new point after finding, per direction of rectangle just created
//                    3     1
//                     \ | /
// rectangle dir -->   -   - 0
//                     / | \ 
//                    5     7
// 0b10101011 / 0xab at dir 0
const vector<uchar> dirset_to_search_from_new_point {
  ((0xab << 0) | (0xab >> 8)) & 0xff,
  ((0xab << 1) | (0xab >> 7)) & 0xff,
  ((0xab << 2) | (0xab >> 6)) & 0xff,
  ((0xab << 3) | (0xab >> 5)) & 0xff,
  ((0xab << 4) | (0xab >> 4)) & 0xff,
  ((0xab << 5) | (0xab >> 3)) & 0xff,
  ((0xab << 6) | (0xab >> 2)) & 0xff,
  ((0xab << 7) | (0xab >> 1)) & 0xff,
};

// direction set of existing points to search rectangles from after finding, per direction of rectangle just created
//                       2  1
//                     \ | /
// rectangle dir --> 4 -   - 0
//                     / | \ 
//                       6  7
// 0b11010111 / 0xd7 at dir 0
const vector<uchar> dirset_to_search_existing_point {
  ((0xd7 << 0) | (0xd7 >> 8)) & 0xff,
  ((0xd7 << 1) | (0xd7 >> 7)) & 0xff,
  ((0xd7 << 2) | (0xd7 >> 6)) & 0xff,
  ((0xd7 << 3) | (0xd7 >> 5)) & 0xff,
  ((0xd7 << 4) | (0xd7 >> 4)) & 0xff,
  ((0xd7 << 5) | (0xd7 >> 3)) & 0xff,
  ((0xd7 << 6) | (0xd7 >> 2)) & 0xff,
  ((0xd7 << 7) | (0xd7 >> 1)) & 0xff,
};

// direction set to search rectangles from existing point after finding, per direction from new point
//                    3
//                     \ | /
// new point dir <--   -   -
//                     / | \ 
//                    5
// 0b00101000 / 0x28 at dir 0
const vector<uchar> dirset_to_search_from_existing_point {
  ((0x28 << 0) | (0x28 >> 8)) & 0xff,
  ((0x28 << 1) | (0x28 >> 7)) & 0xff,
  ((0x28 << 2) | (0x28 >> 6)) & 0xff,
  ((0x28 << 3) | (0x28 >> 5)) & 0xff,
  ((0x28 << 4) | (0x28 >> 4)) & 0xff,
  ((0x28 << 5) | (0x28 >> 3)) & 0xff,
  ((0x28 << 6) | (0x28 >> 2)) & 0xff,
  ((0x28 << 7) | (0x28 >> 1)) & 0xff,
};


// utilities for dir and dirset
inline uchar dir_to_dirset(const uchar dir) {
  return (uchar) 0x1 << dir;
}

inline bool dir_in_dirset(const uchar dirset, const uchar dir_to_find) {
  return dirset & ((uchar) 0x1 << dir_to_find);
}

inline bool get_next_dir_from_dirset(uchar& dirset, uchar& next_dir) {
  if (dirset == 0) return false;
  next_dir = __builtin_ctz(dirset);
  dirset &= ~((uchar) 0x1 << next_dir);
  return true;
}

// inputs
int board_size;
unordered_set<Pii> initial_point;

// outputs
vector<array<Pii, 4>> rectangle_to_create; // clockwise

// internals

// board weights
vector<vector<int>> point_weight;

struct state {
  // set of rectangles to create
  int next_rectangle_id = 0;
  unordered_map<int, array<Pii, 4>> rectangle_set_to_create;

  // created rectangles, stored per newly created points
  unordered_map<Pii, array<Pii, 4>> rectangle_per_point;

  // currently existing points
  unordered_map<Pii, int> existing_point;

  // currently existing points, stored per lane for each direction
  vector<ull> existing_point_dl; // / lane = row + col                   , bit = row
  vector<ull> existing_point_d;  // | lane = col                         , bit = row
  vector<ull> existing_point_dr; // \ lane = (board_size - 1) + row - col, bit = row
  vector<ull> existing_point_r;  // - lane = row                         , bit = col

  // currently existing lines, stored per lane for each direction
  vector<ull> existing_line_dl; // / lane = row + col                   , bit = row
  vector<ull> existing_line_d;  // | lane = col                         , bit = row
  vector<ull> existing_line_dr; // \ lane = (board_size - 1) + row - col, bit = row
  vector<ull> existing_line_r;  // - lane = row                         , bit = col

  // dependencies
  unordered_map<Pii, unordered_set<Pii>> point_dependent_to;
  unordered_map<Pii, unordered_set<Pii>> point_dependent_from;
};

state current_state, prev_state;

bool within_board(int px, int py) {
  return 0 <= px && px < board_size && 0 <= py && py < board_size;
}

void get_input() {
  int m;
  cin >> board_size >> m;
  for (int i = 0; i < m; i++) {
    Pii point;
    cin >> point.first >> point.second;
    initial_point.insert(point);
  }
}

void output_ans() {
  cout << rectangle_to_create.size() << endl;
  for (auto &r: rectangle_to_create) {
    for (auto &p: r) cout << p.first << " " << p.second << " ";
    cout << endl;
  }
}

void init() {
  point_weight = vector<vector<int>>(board_size, vector<int>(board_size));
  {
    const int center = (board_size - 1) / 2;
    for (int i = 0; i < board_size; i++) {
      for (int j = 0; j < board_size; j++) {
        point_weight[i][j] = (i - center) * (i - center) + (j - center) * (j - center) + 1;
      }
    }
  }

  current_state.next_rectangle_id = 0;
  current_state.rectangle_set_to_create = unordered_map<int, array<Pii, 4>>();

  current_state.rectangle_per_point = unordered_map<Pii, array<Pii, 4>>();

  current_state.existing_point = unordered_map<Pii, int>();
  for (auto &p: initial_point) current_state.existing_point[p] = -1;

  current_state.existing_point_d  = vector<ull>(board_size);
  current_state.existing_point_dr = vector<ull>(board_size * 2 - 1);
  current_state.existing_point_r  = vector<ull>(board_size);
  current_state.existing_point_dl = vector<ull>(board_size * 2 - 1);

  current_state.existing_line_d  = vector<ull>(board_size);
  current_state.existing_line_dr = vector<ull>(board_size * 2 - 1);
  current_state.existing_line_r  = vector<ull>(board_size);
  current_state.existing_line_dl = vector<ull>(board_size * 2 - 1);

  for (auto &p: initial_point) {
    current_state.existing_point_d [                             p.second] |= 1ULL << p.first;
    current_state.existing_point_dr[(board_size - 1) + p.first - p.second] |= 1ULL << p.first;
    current_state.existing_point_r [                   p.first           ] |= 1ULL << p.second;
    current_state.existing_point_dl[                   p.first + p.second] |= 1ULL << p.first;
  }

  current_state.point_dependent_to = unordered_map<Pii, unordered_set<Pii>>();
  current_state.point_dependent_from = unordered_map<Pii, unordered_set<Pii>>();

  prev_state = current_state;
}

inline int get_prev_bit(ull target, int base) {
  ull bitset_to_search = target & ((1ULL << base) - 1);
  if (bitset_to_search == 0ULL) return -1;
  else return 63 - __builtin_clzll(bitset_to_search);
}

inline int get_next_bit(ull target, int base) {
  ull bitset_to_search = target & ~((1ULL << (base + 1)) - 1);
  if (bitset_to_search == 0ULL) return 64;
  else return __builtin_ctzll(bitset_to_search);
}

inline int get_lane_dr(int row, int col) {
  return (board_size - 1) + row - col;
}

inline int get_lane_dl(int row, int col) {
  return row + col;
}

inline int get_col_from_row_dr(int lane, int row) {
  // lane = (board_size - 1) + row - col
  // col = (board_size - 1) + row - lane
  return (board_size - 1) + row - lane;
}

inline int get_col_from_row_dl(int lane, int row) {
  // lane = row + col
  // col = lane - row
  return lane - row;
}

tuple<int, int, int> get_next_point_d(const Pii point_to_search) {
  const auto lane_d = point_to_search.second;
  const auto row_d = get_next_bit(current_state.existing_point_d[lane_d], point_to_search.first);
  const auto col_d = point_to_search.second;
  return make_tuple(lane_d, row_d, col_d);
}

tuple<int, int, int> get_next_point_dr(const Pii point_to_search) {
  const auto lane_dr = get_lane_dr(point_to_search.first, point_to_search.second);
  const auto row_dr = get_next_bit(current_state.existing_point_dr[lane_dr], point_to_search.first);
  const auto col_dr = get_col_from_row_dr(lane_dr, row_dr);
  return make_tuple(lane_dr, row_dr, col_dr);
}

tuple<int, int, int> get_next_point_r(const Pii point_to_search) {
  const auto lane_r = point_to_search.first;
  const auto row_r = point_to_search.first;
  const auto col_r = get_next_bit(current_state.existing_point_r[lane_r], point_to_search.second);
  return make_tuple(lane_r, row_r, col_r);
}

tuple<int, int, int> get_next_point_ur(const Pii point_to_search) {
  const auto lane_ur = get_lane_dl(point_to_search.first, point_to_search.second);
  const auto row_ur = get_prev_bit(current_state.existing_point_dl[lane_ur], point_to_search.first);
  const auto col_ur = get_col_from_row_dl(lane_ur, row_ur);
  return make_tuple(lane_ur, row_ur, col_ur);
}

tuple<int, int, int> get_next_point_u(const Pii point_to_search) {
  const auto lane_u = point_to_search.second;
  const auto row_u = get_prev_bit(current_state.existing_point_d[lane_u], point_to_search.first);
  const auto col_u = point_to_search.second;
  return make_tuple(lane_u, row_u, col_u);
}

tuple<int, int, int> get_next_point_ul(const Pii point_to_search) {
  const auto lane_ul = get_lane_dr(point_to_search.first, point_to_search.second);
  const auto row_ul = get_prev_bit(current_state.existing_point_dr[lane_ul], point_to_search.first);
  const auto col_ul = get_col_from_row_dr(lane_ul, row_ul);
  return make_tuple(lane_ul, row_ul, col_ul);
}

tuple<int, int, int> get_next_point_l(const Pii point_to_search) {
  const auto lane_l = point_to_search.first;
  const auto row_l = point_to_search.first;
  const auto col_l = get_prev_bit(current_state.existing_point_r[lane_l], point_to_search.second);
  return make_tuple(lane_l, row_l, col_l);
}

tuple<int, int, int> get_next_point_dl(const Pii point_to_search) {
  const auto lane_dl = get_lane_dl(point_to_search.first, point_to_search.second);
  const auto row_dl = get_next_bit(current_state.existing_point_dl[lane_dl], point_to_search.first);
  const auto col_dl = get_col_from_row_dl(lane_dl, row_dl);
  return make_tuple(lane_dl, row_dl, col_dl);
}

tuple<int, int, int> get_next_point(const Pii point_to_search, uchar direction) {
  switch (direction) {
    case DIR_D:  return get_next_point_d(point_to_search);
    case DIR_DR: return get_next_point_dr(point_to_search);
    case DIR_R:  return get_next_point_r(point_to_search);
    case DIR_UR: return get_next_point_ur(point_to_search);
    case DIR_U:  return get_next_point_u(point_to_search);
    case DIR_UL: return get_next_point_ul(point_to_search);
    case DIR_L:  return get_next_point_l(point_to_search);
    case DIR_DL: return get_next_point_dl(point_to_search);
    default:     assert(false);
  }
}

inline ull get_line_bitmask_row(const Pii point1, const Pii point2) {
  return ~((1ULL << min(point1.first, point2.first)) - 1) & ((1ULL << max(point1.first, point2.first)) - 1);
}

inline ull get_line_bitmask_col(const Pii point1, const Pii point2) {
  return ~((1ULL << min(point1.second, point2.second)) - 1) & ((1ULL << max(point1.second, point2.second)) - 1);
}

inline ull get_line_bitmask_d (const Pii point1, const Pii point2) {return get_line_bitmask_row(point1, point2);}
inline ull get_line_bitmask_dr(const Pii point1, const Pii point2) {return get_line_bitmask_row(point1, point2);}
inline ull get_line_bitmask_r (const Pii point1, const Pii point2) {return get_line_bitmask_col(point1, point2);}
inline ull get_line_bitmask_ur(const Pii point1, const Pii point2) {return get_line_bitmask_row(point1, point2);}
inline ull get_line_bitmask_u (const Pii point1, const Pii point2) {return get_line_bitmask_row(point1, point2);}
inline ull get_line_bitmask_ul(const Pii point1, const Pii point2) {return get_line_bitmask_row(point1, point2);}
inline ull get_line_bitmask_l (const Pii point1, const Pii point2) {return get_line_bitmask_col(point1, point2);}
inline ull get_line_bitmask_dl(const Pii point1, const Pii point2) {return get_line_bitmask_row(point1, point2);}

inline uchar get_direction_from_to(Pii source, Pii target) {
  if (source.first == target.first) { // L / R
    if (target.second < source.second) return DIR_L;
    else                               return DIR_R;
  }
  else if (target.first < source.first) { // UL / U / UR
         if (target.second < source.second) return DIR_UL;
    else if (source.second < target.second) return DIR_UR;
    else                                    return DIR_U;
  }
  else { // DL / D / DR
         if (target.second < source.second) return DIR_DL;
    else if (source.second < target.second) return DIR_DR;
    else                                    return DIR_D;
  }
}

int calc_raw_score() {
  int raw_score = 0;
  for (auto &p: initial_point) {
    raw_score += point_weight[p.first][p.second];
  }
  for (auto &r: rectangle_to_create) {
    raw_score += point_weight[r[0].first][r[0].second];
  }
  return raw_score;
}

int calc_actual_score(int raw_score) {
  int maximum_raw_score = 0;
  for (int i = 0; i < board_size; i++) {
    for (int j = 0; j < board_size; j++) {
      maximum_raw_score += point_weight[i][j];
    }
  }
  return (int) (1e6 * ((double) (board_size * board_size) / (double) initial_point.size()) * ((double) raw_score / (double) maximum_raw_score));
}

void add_rectangle(const array<Pii, 4>& rectangle_to_add) {
  current_state.rectangle_set_to_create[current_state.next_rectangle_id] = rectangle_to_add;

  current_state.rectangle_per_point[rectangle_to_add[0]] = rectangle_to_add;
  
  auto p = rectangle_to_add[0];
  current_state.existing_point[p] = current_state.next_rectangle_id;
  current_state.next_rectangle_id++;
  current_state.existing_point_d [                             p.second] |= 1ULL << p.first;
  current_state.existing_point_dr[(board_size - 1) + p.first - p.second] |= 1ULL << p.first;
  current_state.existing_point_r [                   p.first           ] |= 1ULL << p.second;
  current_state.existing_point_dl[                   p.first + p.second] |= 1ULL << p.first;

  for (int i = 0; i < 4; i++) {
    auto next_dir = get_direction_from_to(rectangle_to_add[i], rectangle_to_add[(i+1)%4]);
    switch (next_dir) {
      case DIR_D:
      case DIR_U:
        current_state.existing_line_d[rectangle_to_add[i].second] |= get_line_bitmask_d(rectangle_to_add[i], rectangle_to_add[(i+1)%4]);
        break;
      case DIR_DR:
      case DIR_UL:
        current_state.existing_line_dr[get_lane_dr(rectangle_to_add[i].first, rectangle_to_add[i].second)] |= get_line_bitmask_dr(rectangle_to_add[i], rectangle_to_add[(i+1)%4]);
        break;
      case DIR_R:
      case DIR_L:
        current_state.existing_line_r[rectangle_to_add[i].first] |= get_line_bitmask_r(rectangle_to_add[i], rectangle_to_add[(i+1)%4]);
        break;
      case DIR_UR:
      case DIR_DL:
        current_state.existing_line_dl[get_lane_dl(rectangle_to_add[i].first, rectangle_to_add[i].second)] |= get_line_bitmask_dl(rectangle_to_add[i], rectangle_to_add[(i+1)%4]);
        break;
    }
  }

  for (int i = 1; i < 4; i++) {
    if (initial_point.find(rectangle_to_add[i]) == initial_point.end()) {
      current_state.point_dependent_to[rectangle_to_add[i]].insert(rectangle_to_add[0]);
      current_state.point_dependent_from[rectangle_to_add[0]].insert(rectangle_to_add[i]);
    }
  }
}

void remove_point_descend(const Pii point_to_remove) {
  auto dependent_point = current_state.point_dependent_to[point_to_remove];
  for (auto &p: dependent_point) {
    if (current_state.existing_point.find(p) != current_state.existing_point.end()) {
      remove_point_descend(p);
    }
  }
  
  auto rectangle_to_remove = current_state.rectangle_per_point[point_to_remove];
  for (int i = 1; i < 4; i++) {
    if (initial_point.find(rectangle_to_remove[i]) == initial_point.end()) {
      current_state.point_dependent_to[rectangle_to_remove[i]].erase(rectangle_to_remove[0]);
      current_state.point_dependent_from[rectangle_to_remove[0]].erase(rectangle_to_remove[i]);
    }
  }

  for (int i = 0; i < 4; i++) {
    auto next_dir = get_direction_from_to(rectangle_to_remove[i], rectangle_to_remove[(i+1)%4]);
    switch (next_dir) {
      case DIR_D:
      case DIR_U:
        current_state.existing_line_d[rectangle_to_remove[i].second] &= ~get_line_bitmask_d(rectangle_to_remove[i], rectangle_to_remove[(i+1)%4]);
        break;
      case DIR_DR:
      case DIR_UL:
        current_state.existing_line_dr[get_lane_dr(rectangle_to_remove[i].first, rectangle_to_remove[i].second)] &= ~get_line_bitmask_dr(rectangle_to_remove[i], rectangle_to_remove[(i+1)%4]);
        break;
      case DIR_R:
      case DIR_L:
        current_state.existing_line_r[rectangle_to_remove[i].first] &= ~get_line_bitmask_r(rectangle_to_remove[i], rectangle_to_remove[(i+1)%4]);
        break;
      case DIR_UR:
      case DIR_DL:
        current_state.existing_line_dl[get_lane_dl(rectangle_to_remove[i].first, rectangle_to_remove[i].second)] &= ~get_line_bitmask_dl(rectangle_to_remove[i], rectangle_to_remove[(i+1)%4]);
        break;
    }
  }
  
  const auto p = point_to_remove;
  current_state.existing_point_d [                             p.second] &= ~(1ULL << p.first);
  current_state.existing_point_dr[(board_size - 1) + p.first - p.second] &= ~(1ULL << p.first);
  current_state.existing_point_r [                   p.first           ] &= ~(1ULL << p.second);
  current_state.existing_point_dl[                   p.first + p.second] &= ~(1ULL << p.first);

  current_state.rectangle_set_to_create.erase(current_state.existing_point[point_to_remove]);
  current_state.existing_point.erase(point_to_remove);
  current_state.rectangle_per_point.erase(point_to_remove);
}

void remove_point_full(const Pii point_to_remove, bool can_ascend) {
  auto dependent_point_descend = current_state.point_dependent_to[point_to_remove];
  auto dependent_point_ascend = current_state.point_dependent_from[point_to_remove];
  for (auto &p: dependent_point_descend) {
    if (current_state.existing_point.find(p) != current_state.existing_point.end()) {
      remove_point_full(p, false);
    }
  }
  
  auto rectangle_to_remove = current_state.rectangle_per_point[point_to_remove];
  for (int i = 1; i < 4; i++) {
    if (initial_point.find(rectangle_to_remove[i]) == initial_point.end()) {
      current_state.point_dependent_to[rectangle_to_remove[i]].erase(rectangle_to_remove[0]);
      current_state.point_dependent_from[rectangle_to_remove[0]].erase(rectangle_to_remove[i]);
    }
  }

  for (int i = 0; i < 4; i++) {
    auto next_dir = get_direction_from_to(rectangle_to_remove[i], rectangle_to_remove[(i+1)%4]);
    switch (next_dir) {
      case DIR_D:
      case DIR_U:
        current_state.existing_line_d[rectangle_to_remove[i].second] &= ~get_line_bitmask_d(rectangle_to_remove[i], rectangle_to_remove[(i+1)%4]);
        break;
      case DIR_DR:
      case DIR_UL:
        current_state.existing_line_dr[get_lane_dr(rectangle_to_remove[i].first, rectangle_to_remove[i].second)] &= ~get_line_bitmask_dr(rectangle_to_remove[i], rectangle_to_remove[(i+1)%4]);
        break;
      case DIR_R:
      case DIR_L:
        current_state.existing_line_r[rectangle_to_remove[i].first] &= ~get_line_bitmask_r(rectangle_to_remove[i], rectangle_to_remove[(i+1)%4]);
        break;
      case DIR_UR:
      case DIR_DL:
        current_state.existing_line_dl[get_lane_dl(rectangle_to_remove[i].first, rectangle_to_remove[i].second)] &= ~get_line_bitmask_dl(rectangle_to_remove[i], rectangle_to_remove[(i+1)%4]);
        break;
    }
  }
  
  const auto p = point_to_remove;
  current_state.existing_point_d [                             p.second] &= ~(1ULL << p.first);
  current_state.existing_point_dr[(board_size - 1) + p.first - p.second] &= ~(1ULL << p.first);
  current_state.existing_point_r [                   p.first           ] &= ~(1ULL << p.second);
  current_state.existing_point_dl[                   p.first + p.second] &= ~(1ULL << p.first);

  current_state.rectangle_set_to_create.erase(current_state.existing_point[point_to_remove]);
  current_state.existing_point.erase(point_to_remove);
  current_state.rectangle_per_point.erase(point_to_remove);

  if (can_ascend) {
    for (auto &p: dependent_point_ascend) {
      if (current_state.existing_point.find(p) != current_state.existing_point.end()) {
        remove_point_full(p, true);
      }
    }
  }
}

void create_ans_from_set() {
  rectangle_to_create.clear();
  vector<int> existing_rectangle_ids;
  for (auto &[id, _]: current_state.rectangle_set_to_create) existing_rectangle_ids.push_back(id);
  sort(existing_rectangle_ids.begin(), existing_rectangle_ids.end());
  auto add_to_ans = [&](auto self, const array<Pii, 4>& rect) {
    if (false) return;

    for (auto &p: current_state.point_dependent_from[rect[0]]) {
      if (current_state.existing_point[rect[0]] < current_state.existing_point[p]) {
        self(self, current_state.rectangle_per_point[p]);
      }
    }

    rectangle_to_create.push_back(rect);
  };
  for (auto &id: existing_rectangle_ids) {
    add_to_ans(add_to_ans, current_state.rectangle_set_to_create[id]);
  }
}

void create_next_solution() {
  vector<array<Pii, 4>> rectangle_candidate; // clockwise from target

  auto search_and_add_new_candidate = [&](Pii point_to_search, uchar dirset_to_check) {
    uchar next_dir = 0;
    while (get_next_dir_from_dirset(dirset_to_check, next_dir)) {
      switch (next_dir) {
        case DIR_D:
        {
          const auto [lane_base_dl, row_base_dl, col_base_dl] = get_next_point_dl(point_to_search); // find other base points
          const auto [lane_base_dr, row_base_dr, col_base_dr] = get_next_point_dr(point_to_search);
          if (!(within_board(row_base_dl, col_base_dl) && within_board(row_base_dr, col_base_dr))) continue;
          const auto row_target = row_base_dl + row_base_dr - point_to_search.first;
          const auto col_target = col_base_dl + col_base_dr - point_to_search.second;
          if (!within_board(row_target, col_target)) continue;
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) continue;
          const auto [lane_target_dl, row_target_dl, col_target_dl] = get_next_point_ul(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_dr, row_target_dr, col_target_dr] = get_next_point_ur(make_pair(row_target, col_target));
          if (!(row_base_dl == row_target_dl && col_base_dl == col_target_dl) || !(row_base_dr == row_target_dr && col_base_dr == col_target_dr)) continue;
          const auto origin_to_dl_line_mask = current_state.existing_line_dl[lane_base_dl] & get_line_bitmask_dl(point_to_search, make_pair(row_base_dl, col_base_dl)); // check for existing lines
          const auto origin_to_dr_line_mask = current_state.existing_line_dr[lane_base_dr] & get_line_bitmask_dr(point_to_search, make_pair(row_base_dr, col_base_dr));
          const auto target_to_dl_line_mask = current_state.existing_line_dr[lane_target_dl] & get_line_bitmask_ul(make_pair(row_target, col_target), make_pair(row_target_dl, col_target_dl));
          const auto target_to_dr_line_mask = current_state.existing_line_dl[lane_target_dr] & get_line_bitmask_ur(make_pair(row_target, col_target), make_pair(row_target_dr, col_target_dr));
          if (origin_to_dl_line_mask || origin_to_dr_line_mask || target_to_dl_line_mask || target_to_dr_line_mask) continue;
          rectangle_candidate.push_back(array<Pii, 4>({make_pair(row_target, col_target), make_pair(row_base_dl, col_base_dl), point_to_search, make_pair(row_base_dr, col_base_dr)}));
          break;
        }
        case DIR_DR:
        {
          const auto [lane_base_d, row_base_d, col_base_d] = get_next_point_d(point_to_search); // find other base points
          const auto [lane_base_r, row_base_r, col_base_r] = get_next_point_r(point_to_search);
          if (!(within_board(row_base_d, col_base_d) && within_board(row_base_r, col_base_r))) continue;
          const auto row_target = row_base_d + row_base_r - point_to_search.first;
          const auto col_target = col_base_d + col_base_r - point_to_search.second;
          if (!within_board(row_target, col_target)) continue;
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) continue;
          const auto [lane_target_d, row_target_d, col_target_d] = get_next_point_l(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_r, row_target_r, col_target_r] = get_next_point_u(make_pair(row_target, col_target));
          if (!(row_base_d == row_target_d && col_base_d == col_target_d) || !(row_base_r == row_target_r && col_base_r == col_target_r)) continue;
          const auto origin_to_d_line_mask = current_state.existing_line_d[lane_base_d] & get_line_bitmask_d(point_to_search, make_pair(row_base_d, col_base_d)); // check for existing lines
          const auto origin_to_r_line_mask = current_state.existing_line_r[lane_base_r] & get_line_bitmask_r(point_to_search, make_pair(row_base_r, col_base_r));
          const auto target_to_d_line_mask = current_state.existing_line_r[lane_target_d] & get_line_bitmask_l(make_pair(row_target, col_target), make_pair(row_target_d, col_target_d));
          const auto target_to_r_line_mask = current_state.existing_line_d[lane_target_r] & get_line_bitmask_u(make_pair(row_target, col_target), make_pair(row_target_r, col_target_r));
          if (origin_to_d_line_mask || origin_to_r_line_mask || target_to_d_line_mask || target_to_r_line_mask) continue;
          rectangle_candidate.push_back(array<Pii, 4>({make_pair(row_target, col_target), make_pair(row_base_d, col_base_d), point_to_search, make_pair(row_base_r, col_base_r)}));
          break;
        }
        case DIR_R:
        {
          const auto [lane_base_dr, row_base_dr, col_base_dr] = get_next_point_dr(point_to_search); // find other base points
          const auto [lane_base_ur, row_base_ur, col_base_ur] = get_next_point_ur(point_to_search);
          if (!(within_board(row_base_dr, col_base_dr) && within_board(row_base_ur, col_base_ur))) continue;
          const auto row_target = row_base_dr + row_base_ur - point_to_search.first;
          const auto col_target = col_base_dr + col_base_ur - point_to_search.second;
          if (!within_board(row_target, col_target)) continue;
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) continue;
          const auto [lane_target_dr, row_target_dr, col_target_dr] = get_next_point_dl(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_ur, row_target_ur, col_target_ur] = get_next_point_ul(make_pair(row_target, col_target));
          if (!(row_base_dr == row_target_dr && col_base_dr == col_target_dr) || !(row_base_ur == row_target_ur && col_base_ur == col_target_ur)) continue;
          const auto origin_to_dr_line_mask = current_state.existing_line_dr[lane_base_dr] & get_line_bitmask_dr(point_to_search, make_pair(row_base_dr, col_base_dr)); // check for existing lines
          const auto origin_to_ur_line_mask = current_state.existing_line_dl[lane_base_ur] & get_line_bitmask_ur(point_to_search, make_pair(row_base_ur, col_base_ur));
          const auto target_to_dr_line_mask = current_state.existing_line_dl[lane_target_dr] & get_line_bitmask_dl(make_pair(row_target, col_target), make_pair(row_target_dr, col_target_dr));
          const auto target_to_ur_line_mask = current_state.existing_line_dr[lane_target_ur] & get_line_bitmask_ul(make_pair(row_target, col_target), make_pair(row_target_ur, col_target_ur));
          if (origin_to_dr_line_mask || origin_to_ur_line_mask || target_to_dr_line_mask || target_to_ur_line_mask) continue;
          rectangle_candidate.push_back(array<Pii, 4>({make_pair(row_target, col_target), make_pair(row_base_dr, col_base_dr), point_to_search, make_pair(row_base_ur, col_base_ur)}));
          break;
        }
        case DIR_UR:
        {
          const auto [lane_base_r, row_base_r, col_base_r] = get_next_point_r(point_to_search); // find other base points
          const auto [lane_base_u, row_base_u, col_base_u] = get_next_point_u(point_to_search);
          if (!(within_board(row_base_r, col_base_r) && within_board(row_base_u, col_base_u))) continue;
          const auto row_target = row_base_r + row_base_u - point_to_search.first;
          const auto col_target = col_base_r + col_base_u - point_to_search.second;
          if (!within_board(row_target, col_target)) continue;
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) continue;
          const auto [lane_target_r, row_target_r, col_target_r] = get_next_point_d(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_u, row_target_u, col_target_u] = get_next_point_l(make_pair(row_target, col_target));
          if (!(row_base_r == row_target_r && col_base_r == col_target_r) || !(row_base_u == row_target_u && col_base_u == col_target_u)) continue;
          const auto origin_to_r_line_mask = current_state.existing_line_r[lane_base_r] & get_line_bitmask_r(point_to_search, make_pair(row_base_r, col_base_r)); // check for existing lines
          const auto origin_to_u_line_mask = current_state.existing_line_d[lane_base_u] & get_line_bitmask_u(point_to_search, make_pair(row_base_u, col_base_u));
          const auto target_to_r_line_mask = current_state.existing_line_d[lane_target_r] & get_line_bitmask_d(make_pair(row_target, col_target), make_pair(row_target_r, col_target_r));
          const auto target_to_u_line_mask = current_state.existing_line_r[lane_target_u] & get_line_bitmask_l(make_pair(row_target, col_target), make_pair(row_target_u, col_target_u));
          if (origin_to_r_line_mask || origin_to_u_line_mask || target_to_r_line_mask || target_to_u_line_mask) continue;
          rectangle_candidate.push_back(array<Pii, 4>({make_pair(row_target, col_target), make_pair(row_base_r, col_base_r), point_to_search, make_pair(row_base_u, col_base_u)}));
          break;
        }
        case DIR_U:
        {
          const auto [lane_base_ur, row_base_ur, col_base_ur] = get_next_point_ur(point_to_search); // find other base points
          const auto [lane_base_ul, row_base_ul, col_base_ul] = get_next_point_ul(point_to_search);
          if (!(within_board(row_base_ur, col_base_ur) && within_board(row_base_ul, col_base_ul))) continue;
          const auto row_target = row_base_ur + row_base_ul - point_to_search.first;
          const auto col_target = col_base_ur + col_base_ul - point_to_search.second;
          if (!within_board(row_target, col_target)) continue;
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) continue;
          const auto [lane_target_ur, row_target_ur, col_target_ur] = get_next_point_dr(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_ul, row_target_ul, col_target_ul] = get_next_point_dl(make_pair(row_target, col_target));
          if (!(row_base_ur == row_target_ur && col_base_ur == col_target_ur) || !(row_base_ul == row_target_ul && col_base_ul == col_target_ul)) continue;
          const auto origin_to_ur_line_mask = current_state.existing_line_dl[lane_base_ur] & get_line_bitmask_ur(point_to_search, make_pair(row_base_ur, col_base_ur)); // check for existing lines
          const auto origin_to_ul_line_mask = current_state.existing_line_dr[lane_base_ul] & get_line_bitmask_ul(point_to_search, make_pair(row_base_ul, col_base_ul));
          const auto target_to_ur_line_mask = current_state.existing_line_dr[lane_target_ur] & get_line_bitmask_dr(make_pair(row_target, col_target), make_pair(row_target_ur, col_target_ur));
          const auto target_to_ul_line_mask = current_state.existing_line_dl[lane_target_ul] & get_line_bitmask_dl(make_pair(row_target, col_target), make_pair(row_target_ul, col_target_ul));
          if (origin_to_ur_line_mask || origin_to_ul_line_mask || target_to_ur_line_mask || target_to_ul_line_mask) continue;
          rectangle_candidate.push_back(array<Pii, 4>({make_pair(row_target, col_target), make_pair(row_base_ur, col_base_ur), point_to_search, make_pair(row_base_ul, col_base_ul)}));
          break;
        }
        case DIR_UL:
        {
          const auto [lane_base_u, row_base_u, col_base_u] = get_next_point_u(point_to_search); // find other base points
          const auto [lane_base_l, row_base_l, col_base_l] = get_next_point_l(point_to_search);
          if (!(within_board(row_base_u, col_base_u) && within_board(row_base_l, col_base_l))) continue;
          const auto row_target = row_base_u + row_base_l - point_to_search.first;
          const auto col_target = col_base_u + col_base_l - point_to_search.second;
          if (!within_board(row_target, col_target)) continue;
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) continue;
          const auto [lane_target_u, row_target_u, col_target_u] = get_next_point_r(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_l, row_target_l, col_target_l] = get_next_point_d(make_pair(row_target, col_target));
          if (!(row_base_u == row_target_u && col_base_u == col_target_u) || !(row_base_l == row_target_l && col_base_l == col_target_l)) continue;
          const auto origin_to_u_line_mask = current_state.existing_line_d[lane_base_u] & get_line_bitmask_u(point_to_search, make_pair(row_base_u, col_base_u)); // check for existing lines
          const auto origin_to_l_line_mask = current_state.existing_line_r[lane_base_l] & get_line_bitmask_l(point_to_search, make_pair(row_base_l, col_base_l));
          const auto target_to_u_line_mask = current_state.existing_line_r[lane_target_u] & get_line_bitmask_r(make_pair(row_target, col_target), make_pair(row_target_u, col_target_u));
          const auto target_to_l_line_mask = current_state.existing_line_d[lane_target_l] & get_line_bitmask_d(make_pair(row_target, col_target), make_pair(row_target_l, col_target_l));
          if (origin_to_u_line_mask || origin_to_l_line_mask || target_to_u_line_mask || target_to_l_line_mask) continue;
          rectangle_candidate.push_back(array<Pii, 4>({make_pair(row_target, col_target), make_pair(row_base_u, col_base_u), point_to_search, make_pair(row_base_l, col_base_l)}));
          break;
        }
        case DIR_L:
        {
          const auto [lane_base_ul, row_base_ul, col_base_ul] = get_next_point_ul(point_to_search); // find other base points
          const auto [lane_base_dl, row_base_dl, col_base_dl] = get_next_point_dl(point_to_search);
          if (!(within_board(row_base_ul, col_base_ul) && within_board(row_base_dl, col_base_dl))) continue;
          const auto row_target = row_base_ul + row_base_dl - point_to_search.first;
          const auto col_target = col_base_ul + col_base_dl - point_to_search.second;
          if (!within_board(row_target, col_target)) continue;
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) continue;
          const auto [lane_target_ul, row_target_ul, col_target_ul] = get_next_point_ur(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_dl, row_target_dl, col_target_dl] = get_next_point_dr(make_pair(row_target, col_target));
          if (!(row_base_ul == row_target_ul && col_base_ul == col_target_ul) || !(row_base_dl == row_target_dl && col_base_dl == col_target_dl)) continue;
          const auto origin_to_ul_line_mask = current_state.existing_line_dr[lane_base_ul] & get_line_bitmask_ul(point_to_search, make_pair(row_base_ul, col_base_ul)); // check for existing lines
          const auto origin_to_dl_line_mask = current_state.existing_line_dl[lane_base_dl] & get_line_bitmask_dl(point_to_search, make_pair(row_base_dl, col_base_dl));
          const auto target_to_ul_line_mask = current_state.existing_line_dl[lane_target_ul] & get_line_bitmask_ur(make_pair(row_target, col_target), make_pair(row_target_ul, col_target_ul));
          const auto target_to_dl_line_mask = current_state.existing_line_dr[lane_target_dl] & get_line_bitmask_dr(make_pair(row_target, col_target), make_pair(row_target_dl, col_target_dl));
          if (origin_to_ul_line_mask || origin_to_dl_line_mask || target_to_ul_line_mask || target_to_dl_line_mask) continue;
          rectangle_candidate.push_back(array<Pii, 4>({make_pair(row_target, col_target), make_pair(row_base_ul, col_base_ul), point_to_search, make_pair(row_base_dl, col_base_dl)}));
          break;
        }
        case DIR_DL:
        {
          const auto [lane_base_l, row_base_l, col_base_l] = get_next_point_l(point_to_search); // find other base points
          const auto [lane_base_d, row_base_d, col_base_d] = get_next_point_d(point_to_search);
          if (!(within_board(row_base_l, col_base_l) && within_board(row_base_d, col_base_d))) continue;
          const auto row_target = row_base_l + row_base_d - point_to_search.first;
          const auto col_target = col_base_l + col_base_d - point_to_search.second;
          if (!within_board(row_target, col_target)) continue;
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) continue;
          const auto [lane_target_l, row_target_l, col_target_l] = get_next_point_u(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_d, row_target_d, col_target_d] = get_next_point_r(make_pair(row_target, col_target));
          if (!(row_base_l == row_target_l && col_base_l == col_target_l) || !(row_base_d == row_target_d && col_base_d == col_target_d)) continue;
          const auto origin_to_l_line_mask = current_state.existing_line_r[lane_base_l] & get_line_bitmask_l(point_to_search, make_pair(row_base_l, col_base_l)); // check for existing lines
          const auto origin_to_d_line_mask = current_state.existing_line_d[lane_base_d] & get_line_bitmask_d(point_to_search, make_pair(row_base_d, col_base_d));
          const auto target_to_l_line_mask = current_state.existing_line_d[lane_target_l] & get_line_bitmask_u(make_pair(row_target, col_target), make_pair(row_target_l, col_target_l));
          const auto target_to_d_line_mask = current_state.existing_line_r[lane_target_d] & get_line_bitmask_r(make_pair(row_target, col_target), make_pair(row_target_d, col_target_d));
          if (origin_to_l_line_mask || origin_to_d_line_mask || target_to_l_line_mask || target_to_d_line_mask) continue;
          rectangle_candidate.push_back(array<Pii, 4>({make_pair(row_target, col_target), make_pair(row_base_l, col_base_l), point_to_search, make_pair(row_base_d, col_base_d)}));
          break;
        }
      }
    }
  };

  auto check_and_add_rectangle = [&](const array<Pii, 4> rectangle_to_add) {
    auto rectangle_direction = (get_direction_from_to(rectangle_to_add[2], rectangle_to_add[1]) + 1) % 8;
    bool rectangle_addable = false;
    switch (rectangle_direction) {
      case DIR_D:
      {
        const auto [lane_base_dl, row_base_dl, col_base_dl] = get_next_point_dl(rectangle_to_add[2]); // find other base points
        const auto [lane_base_dr, row_base_dr, col_base_dr] = get_next_point_dr(rectangle_to_add[2]);
        if (!(within_board(row_base_dl, col_base_dl) && within_board(row_base_dr, col_base_dr))) break;
        const auto row_target = row_base_dl + row_base_dr - rectangle_to_add[2].first;
        const auto col_target = col_base_dl + col_base_dr - rectangle_to_add[2].second;
        if (!(row_target == rectangle_to_add[0].first && col_target == rectangle_to_add[0].second)) break;
        if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) break;
        const auto [lane_target_dl, row_target_dl, col_target_dl] = get_next_point_ul(make_pair(row_target, col_target)); // check base points from target
        const auto [lane_target_dr, row_target_dr, col_target_dr] = get_next_point_ur(make_pair(row_target, col_target));
        if (!(row_base_dl == row_target_dl && col_base_dl == col_target_dl) || !(row_base_dr == row_target_dr && col_base_dr == col_target_dr)) break;
        const auto origin_to_dl_line_mask = current_state.existing_line_dl[lane_base_dl] & get_line_bitmask_dl(rectangle_to_add[2], make_pair(row_base_dl, col_base_dl)); // check for existing lines
        const auto origin_to_dr_line_mask = current_state.existing_line_dr[lane_base_dr] & get_line_bitmask_dr(rectangle_to_add[2], make_pair(row_base_dr, col_base_dr));
        const auto target_to_dl_line_mask = current_state.existing_line_dr[lane_target_dl] & get_line_bitmask_ul(make_pair(row_target, col_target), make_pair(row_target_dl, col_target_dl));
        const auto target_to_dr_line_mask = current_state.existing_line_dl[lane_target_dr] & get_line_bitmask_ur(make_pair(row_target, col_target), make_pair(row_target_dr, col_target_dr));
        if (origin_to_dl_line_mask || origin_to_dr_line_mask || target_to_dl_line_mask || target_to_dr_line_mask) break;
        rectangle_addable = true;
        break;
      }
      case DIR_DR:
      {
        const auto [lane_base_d, row_base_d, col_base_d] = get_next_point_d(rectangle_to_add[2]); // find other base points
        const auto [lane_base_r, row_base_r, col_base_r] = get_next_point_r(rectangle_to_add[2]);
        if (!(within_board(row_base_d, col_base_d) && within_board(row_base_r, col_base_r))) break;
        const auto row_target = row_base_d + row_base_r - rectangle_to_add[2].first;
        const auto col_target = col_base_d + col_base_r - rectangle_to_add[2].second;
        if (!(row_target == rectangle_to_add[0].first && col_target == rectangle_to_add[0].second)) break;
        if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) break;
        const auto [lane_target_d, row_target_d, col_target_d] = get_next_point_l(make_pair(row_target, col_target)); // check base points from target
        const auto [lane_target_r, row_target_r, col_target_r] = get_next_point_u(make_pair(row_target, col_target));
        if (!(row_base_d == row_target_d && col_base_d == col_target_d) || !(row_base_r == row_target_r && col_base_r == col_target_r)) break;
        const auto origin_to_d_line_mask = current_state.existing_line_d[lane_base_d] & get_line_bitmask_d(rectangle_to_add[2], make_pair(row_base_d, col_base_d)); // check for existing lines
        const auto origin_to_r_line_mask = current_state.existing_line_r[lane_base_r] & get_line_bitmask_r(rectangle_to_add[2], make_pair(row_base_r, col_base_r));
        const auto target_to_d_line_mask = current_state.existing_line_r[lane_target_d] & get_line_bitmask_l(make_pair(row_target, col_target), make_pair(row_target_d, col_target_d));
        const auto target_to_r_line_mask = current_state.existing_line_d[lane_target_r] & get_line_bitmask_u(make_pair(row_target, col_target), make_pair(row_target_r, col_target_r));
        if (origin_to_d_line_mask || origin_to_r_line_mask || target_to_d_line_mask || target_to_r_line_mask) break;
        rectangle_addable = true;
        break;
      }
      case DIR_R:
      {
        const auto [lane_base_dr, row_base_dr, col_base_dr] = get_next_point_dr(rectangle_to_add[2]); // find other base points
        const auto [lane_base_ur, row_base_ur, col_base_ur] = get_next_point_ur(rectangle_to_add[2]);
        if (!(within_board(row_base_dr, col_base_dr) && within_board(row_base_ur, col_base_ur))) break;
        const auto row_target = row_base_dr + row_base_ur - rectangle_to_add[2].first;
        const auto col_target = col_base_dr + col_base_ur - rectangle_to_add[2].second;
        if (!(row_target == rectangle_to_add[0].first && col_target == rectangle_to_add[0].second)) break;
        if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) break;
        const auto [lane_target_dr, row_target_dr, col_target_dr] = get_next_point_dl(make_pair(row_target, col_target)); // check base points from target
        const auto [lane_target_ur, row_target_ur, col_target_ur] = get_next_point_ul(make_pair(row_target, col_target));
        if (!(row_base_dr == row_target_dr && col_base_dr == col_target_dr) || !(row_base_ur == row_target_ur && col_base_ur == col_target_ur)) break;
        const auto origin_to_dr_line_mask = current_state.existing_line_dr[lane_base_dr] & get_line_bitmask_dr(rectangle_to_add[2], make_pair(row_base_dr, col_base_dr)); // check for existing lines
        const auto origin_to_ur_line_mask = current_state.existing_line_dl[lane_base_ur] & get_line_bitmask_ur(rectangle_to_add[2], make_pair(row_base_ur, col_base_ur));
        const auto target_to_dr_line_mask = current_state.existing_line_dl[lane_target_dr] & get_line_bitmask_dl(make_pair(row_target, col_target), make_pair(row_target_dr, col_target_dr));
        const auto target_to_ur_line_mask = current_state.existing_line_dr[lane_target_ur] & get_line_bitmask_ul(make_pair(row_target, col_target), make_pair(row_target_ur, col_target_ur));
        if (origin_to_dr_line_mask || origin_to_ur_line_mask || target_to_dr_line_mask || target_to_ur_line_mask) break;
        rectangle_addable = true;
        break;
      }
      case DIR_UR:
      {
        const auto [lane_base_r, row_base_r, col_base_r] = get_next_point_r(rectangle_to_add[2]); // find other base points
        const auto [lane_base_u, row_base_u, col_base_u] = get_next_point_u(rectangle_to_add[2]);
        if (!(within_board(row_base_r, col_base_r) && within_board(row_base_u, col_base_u))) break;
        const auto row_target = row_base_r + row_base_u - rectangle_to_add[2].first;
        const auto col_target = col_base_r + col_base_u - rectangle_to_add[2].second;
        if (!(row_target == rectangle_to_add[0].first && col_target == rectangle_to_add[0].second)) break;
        if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) break;
        const auto [lane_target_r, row_target_r, col_target_r] = get_next_point_d(make_pair(row_target, col_target)); // check base points from target
        const auto [lane_target_u, row_target_u, col_target_u] = get_next_point_l(make_pair(row_target, col_target));
        if (!(row_base_r == row_target_r && col_base_r == col_target_r) || !(row_base_u == row_target_u && col_base_u == col_target_u)) break;
        const auto origin_to_r_line_mask = current_state.existing_line_r[lane_base_r] & get_line_bitmask_r(rectangle_to_add[2], make_pair(row_base_r, col_base_r)); // check for existing lines
        const auto origin_to_u_line_mask = current_state.existing_line_d[lane_base_u] & get_line_bitmask_u(rectangle_to_add[2], make_pair(row_base_u, col_base_u));
        const auto target_to_r_line_mask = current_state.existing_line_d[lane_target_r] & get_line_bitmask_d(make_pair(row_target, col_target), make_pair(row_target_r, col_target_r));
        const auto target_to_u_line_mask = current_state.existing_line_r[lane_target_u] & get_line_bitmask_l(make_pair(row_target, col_target), make_pair(row_target_u, col_target_u));
        if (origin_to_r_line_mask || origin_to_u_line_mask || target_to_r_line_mask || target_to_u_line_mask) break;
        rectangle_addable = true;
        break;
      }
      case DIR_U:
      {
        const auto [lane_base_ur, row_base_ur, col_base_ur] = get_next_point_ur(rectangle_to_add[2]); // find other base points
        const auto [lane_base_ul, row_base_ul, col_base_ul] = get_next_point_ul(rectangle_to_add[2]);
        if (!(within_board(row_base_ur, col_base_ur) && within_board(row_base_ul, col_base_ul))) break;
        const auto row_target = row_base_ur + row_base_ul - rectangle_to_add[2].first;
        const auto col_target = col_base_ur + col_base_ul - rectangle_to_add[2].second;
        if (!(row_target == rectangle_to_add[0].first && col_target == rectangle_to_add[0].second)) break;
        if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) break;
        const auto [lane_target_ur, row_target_ur, col_target_ur] = get_next_point_dr(make_pair(row_target, col_target)); // check base points from target
        const auto [lane_target_ul, row_target_ul, col_target_ul] = get_next_point_dl(make_pair(row_target, col_target));
        if (!(row_base_ur == row_target_ur && col_base_ur == col_target_ur) || !(row_base_ul == row_target_ul && col_base_ul == col_target_ul)) break;
        const auto origin_to_ur_line_mask = current_state.existing_line_dl[lane_base_ur] & get_line_bitmask_ur(rectangle_to_add[2], make_pair(row_base_ur, col_base_ur)); // check for existing lines
        const auto origin_to_ul_line_mask = current_state.existing_line_dr[lane_base_ul] & get_line_bitmask_ul(rectangle_to_add[2], make_pair(row_base_ul, col_base_ul));
        const auto target_to_ur_line_mask = current_state.existing_line_dr[lane_target_ur] & get_line_bitmask_dr(make_pair(row_target, col_target), make_pair(row_target_ur, col_target_ur));
        const auto target_to_ul_line_mask = current_state.existing_line_dl[lane_target_ul] & get_line_bitmask_dl(make_pair(row_target, col_target), make_pair(row_target_ul, col_target_ul));
        if (origin_to_ur_line_mask || origin_to_ul_line_mask || target_to_ur_line_mask || target_to_ul_line_mask) break;
        rectangle_addable = true;
        break;
      }
      case DIR_UL:
      {
        const auto [lane_base_u, row_base_u, col_base_u] = get_next_point_u(rectangle_to_add[2]); // find other base points
        const auto [lane_base_l, row_base_l, col_base_l] = get_next_point_l(rectangle_to_add[2]);
        if (!(within_board(row_base_u, col_base_u) && within_board(row_base_l, col_base_l))) break;
        const auto row_target = row_base_u + row_base_l - rectangle_to_add[2].first;
        const auto col_target = col_base_u + col_base_l - rectangle_to_add[2].second;
        if (!(row_target == rectangle_to_add[0].first && col_target == rectangle_to_add[0].second)) break;
        if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) break;
        const auto [lane_target_u, row_target_u, col_target_u] = get_next_point_r(make_pair(row_target, col_target)); // check base points from target
        const auto [lane_target_l, row_target_l, col_target_l] = get_next_point_d(make_pair(row_target, col_target));
        if (!(row_base_u == row_target_u && col_base_u == col_target_u) || !(row_base_l == row_target_l && col_base_l == col_target_l)) break;
        const auto origin_to_u_line_mask = current_state.existing_line_d[lane_base_u] & get_line_bitmask_u(rectangle_to_add[2], make_pair(row_base_u, col_base_u)); // check for existing lines
        const auto origin_to_l_line_mask = current_state.existing_line_r[lane_base_l] & get_line_bitmask_l(rectangle_to_add[2], make_pair(row_base_l, col_base_l));
        const auto target_to_u_line_mask = current_state.existing_line_r[lane_target_u] & get_line_bitmask_r(make_pair(row_target, col_target), make_pair(row_target_u, col_target_u));
        const auto target_to_l_line_mask = current_state.existing_line_d[lane_target_l] & get_line_bitmask_d(make_pair(row_target, col_target), make_pair(row_target_l, col_target_l));
        if (origin_to_u_line_mask || origin_to_l_line_mask || target_to_u_line_mask || target_to_l_line_mask) break;
        rectangle_addable = true;
        break;
      }
      case DIR_L:
      {
        const auto [lane_base_ul, row_base_ul, col_base_ul] = get_next_point_ul(rectangle_to_add[2]); // find other base points
        const auto [lane_base_dl, row_base_dl, col_base_dl] = get_next_point_dl(rectangle_to_add[2]);
        if (!(within_board(row_base_ul, col_base_ul) && within_board(row_base_dl, col_base_dl))) break;
        const auto row_target = row_base_ul + row_base_dl - rectangle_to_add[2].first;
        const auto col_target = col_base_ul + col_base_dl - rectangle_to_add[2].second;
        if (!(row_target == rectangle_to_add[0].first && col_target == rectangle_to_add[0].second)) break;
        if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) break;
        const auto [lane_target_ul, row_target_ul, col_target_ul] = get_next_point_ur(make_pair(row_target, col_target)); // check base points from target
        const auto [lane_target_dl, row_target_dl, col_target_dl] = get_next_point_dr(make_pair(row_target, col_target));
        if (!(row_base_ul == row_target_ul && col_base_ul == col_target_ul) || !(row_base_dl == row_target_dl && col_base_dl == col_target_dl)) break;
        const auto origin_to_ul_line_mask = current_state.existing_line_dr[lane_base_ul] & get_line_bitmask_ul(rectangle_to_add[2], make_pair(row_base_ul, col_base_ul)); // check for existing lines
        const auto origin_to_dl_line_mask = current_state.existing_line_dl[lane_base_dl] & get_line_bitmask_dl(rectangle_to_add[2], make_pair(row_base_dl, col_base_dl));
        const auto target_to_ul_line_mask = current_state.existing_line_dl[lane_target_ul] & get_line_bitmask_ur(make_pair(row_target, col_target), make_pair(row_target_ul, col_target_ul));
        const auto target_to_dl_line_mask = current_state.existing_line_dr[lane_target_dl] & get_line_bitmask_dr(make_pair(row_target, col_target), make_pair(row_target_dl, col_target_dl));
        if (origin_to_ul_line_mask || origin_to_dl_line_mask || target_to_ul_line_mask || target_to_dl_line_mask) break;
        rectangle_addable = true;
        break;
      }
      case DIR_DL:
      {
        const auto [lane_base_l, row_base_l, col_base_l] = get_next_point_l(rectangle_to_add[2]); // find other base points
        const auto [lane_base_d, row_base_d, col_base_d] = get_next_point_d(rectangle_to_add[2]);
        if (!(within_board(row_base_l, col_base_l) && within_board(row_base_d, col_base_d))) break;
        const auto row_target = row_base_l + row_base_d - rectangle_to_add[2].first;
        const auto col_target = col_base_l + col_base_d - rectangle_to_add[2].second;
        if (!(row_target == rectangle_to_add[0].first && col_target == rectangle_to_add[0].second)) break;
        if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) break;
        const auto [lane_target_l, row_target_l, col_target_l] = get_next_point_u(make_pair(row_target, col_target)); // check base points from target
        const auto [lane_target_d, row_target_d, col_target_d] = get_next_point_r(make_pair(row_target, col_target));
        if (!(row_base_l == row_target_l && col_base_l == col_target_l) || !(row_base_d == row_target_d && col_base_d == col_target_d)) break;
        const auto origin_to_l_line_mask = current_state.existing_line_r[lane_base_l] & get_line_bitmask_l(rectangle_to_add[2], make_pair(row_base_l, col_base_l)); // check for existing lines
        const auto origin_to_d_line_mask = current_state.existing_line_d[lane_base_d] & get_line_bitmask_d(rectangle_to_add[2], make_pair(row_base_d, col_base_d));
        const auto target_to_l_line_mask = current_state.existing_line_d[lane_target_l] & get_line_bitmask_u(make_pair(row_target, col_target), make_pair(row_target_l, col_target_l));
        const auto target_to_d_line_mask = current_state.existing_line_r[lane_target_d] & get_line_bitmask_r(make_pair(row_target, col_target), make_pair(row_target_d, col_target_d));
        if (origin_to_l_line_mask || origin_to_d_line_mask || target_to_l_line_mask || target_to_d_line_mask) break;
        rectangle_addable = true;
        break;
      }
    }

    if (rectangle_addable) {
      add_rectangle(rectangle_to_add);
      search_and_add_new_candidate(rectangle_to_add[0], dirset_to_search_from_new_point[rectangle_direction]);
      uchar next_dir = 0;
      uchar dirset_to_check_point = dirset_to_search_existing_point[rectangle_direction];
      while (get_next_dir_from_dirset(dirset_to_check_point, next_dir)) {
        auto [_, next_point_row, next_point_col] = get_next_point(rectangle_to_add[0], next_dir);
        if (within_board(next_point_row, next_point_col)) {
          search_and_add_new_candidate(make_pair(next_point_row, next_point_col), dirset_to_search_from_existing_point[next_dir]);
        }
      }
    }
  };

  for (auto &p: initial_point) {
    search_and_add_new_candidate(p, 0xff);
  }

  while (!rectangle_candidate.empty()) {
    auto idx = theRandom.nextUIntMod(rectangle_candidate.size());
    check_and_add_rectangle(rectangle_candidate[idx]);
    swap(rectangle_candidate[idx], rectangle_candidate[rectangle_candidate.size()-1]);
    rectangle_candidate.pop_back();
  }
}

void create_initial_solution() {
  auto add_to_existing_line = [&](const array<Pii, 4>& rectangle_to_add) {
    for (int i = 0; i < 4; i++) {
      auto next_dir = get_direction_from_to(rectangle_to_add[i], rectangle_to_add[(i+1)%4]);
      switch (next_dir) {
        case DIR_D:
        case DIR_U:
          current_state.existing_line_d[rectangle_to_add[i].second] |= get_line_bitmask_d(rectangle_to_add[i], rectangle_to_add[(i+1)%4]);
          break;
        case DIR_DR:
        case DIR_UL:
          current_state.existing_line_dr[get_lane_dr(rectangle_to_add[i].first, rectangle_to_add[i].second)] |= get_line_bitmask_dr(rectangle_to_add[i], rectangle_to_add[(i+1)%4]);
          break;
        case DIR_R:
        case DIR_L:
          current_state.existing_line_r[rectangle_to_add[i].first] |= get_line_bitmask_r(rectangle_to_add[i], rectangle_to_add[(i+1)%4]);
          break;
        case DIR_UR:
        case DIR_DL:
          current_state.existing_line_dl[get_lane_dl(rectangle_to_add[i].first, rectangle_to_add[i].second)] |= get_line_bitmask_dl(rectangle_to_add[i], rectangle_to_add[(i+1)%4]);
          break;
      }
    }
  };
  auto remove_from_existing_line = [&](const array<Pii, 4>& rectangle_to_remove) {
    for (int i = 0; i < 4; i++) {
      auto next_dir = get_direction_from_to(rectangle_to_remove[i], rectangle_to_remove[(i+1)%4]);
      switch (next_dir) {
        case DIR_D:
        case DIR_U:
          current_state.existing_line_d[rectangle_to_remove[i].second] &= ~get_line_bitmask_d(rectangle_to_remove[i], rectangle_to_remove[(i+1)%4]);
          break;
        case DIR_DR:
        case DIR_UL:
          current_state.existing_line_dr[get_lane_dr(rectangle_to_remove[i].first, rectangle_to_remove[i].second)] &= ~get_line_bitmask_dr(rectangle_to_remove[i], rectangle_to_remove[(i+1)%4]);
          break;
        case DIR_R:
        case DIR_L:
          current_state.existing_line_r[rectangle_to_remove[i].first] &= ~get_line_bitmask_r(rectangle_to_remove[i], rectangle_to_remove[(i+1)%4]);
          break;
        case DIR_UR:
        case DIR_DL:
          current_state.existing_line_dl[get_lane_dl(rectangle_to_remove[i].first, rectangle_to_remove[i].second)] &= ~get_line_bitmask_dl(rectangle_to_remove[i], rectangle_to_remove[(i+1)%4]);
          break;
      }
    }
  };

  unordered_set<Pii> temporary_points;
  auto add_to_existing_point = [&](const Pii p) {
    assert(temporary_points.find(p) == temporary_points.end());
    temporary_points.insert(p);
    current_state.existing_point_d [                             p.second] |= 1ULL << p.first;
    current_state.existing_point_dr[(board_size - 1) + p.first - p.second] |= 1ULL << p.first;
    current_state.existing_point_r [                   p.first           ] |= 1ULL << p.second;
    current_state.existing_point_dl[                   p.first + p.second] |= 1ULL << p.first;
  };
  auto remove_from_existing_point = [&](const Pii p) {
    temporary_points.erase(p);
    current_state.existing_point_d [                             p.second] &= ~(1ULL << p.first);
    current_state.existing_point_dr[(board_size - 1) + p.first - p.second] &= ~(1ULL << p.first);
    current_state.existing_point_r [                   p.first           ] &= ~(1ULL << p.second);
    current_state.existing_point_dl[                   p.first + p.second] &= ~(1ULL << p.first);
  };

  vector<ull> reserved_line_dl(board_size * 2 - 1);          // / lane = row + col                   , bit = row
  vector<ull> reserved_line_d (board_size);                  // | lane = col                         , bit = row
  vector<ull> reserved_line_dr(board_size * 2 - 1);          // \ lane = (board_size - 1) + row - col, bit = row
  vector<ull> reserved_line_r (board_size);                  // - lane = row                         , bit = col
  auto add_to_reserve = [&](const array<Pii, 4>& rectangle_to_add) {
    for (int i = 0; i < 4; i++) {
      auto next_dir = get_direction_from_to(rectangle_to_add[i], rectangle_to_add[(i+1)%4]);
      switch (next_dir) {
        case DIR_D:
        case DIR_U:
          reserved_line_d[rectangle_to_add[i].second] |= get_line_bitmask_d(rectangle_to_add[i], rectangle_to_add[(i+1)%4]);
          break;
        case DIR_DR:
        case DIR_UL:
          reserved_line_dr[get_lane_dr(rectangle_to_add[i].first, rectangle_to_add[i].second)] |= get_line_bitmask_dr(rectangle_to_add[i], rectangle_to_add[(i+1)%4]);
          break;
        case DIR_R:
        case DIR_L:
          reserved_line_r[rectangle_to_add[i].first] |= get_line_bitmask_r(rectangle_to_add[i], rectangle_to_add[(i+1)%4]);
          break;
        case DIR_UR:
        case DIR_DL:
          reserved_line_dl[get_lane_dl(rectangle_to_add[i].first, rectangle_to_add[i].second)] |= get_line_bitmask_dl(rectangle_to_add[i], rectangle_to_add[(i+1)%4]);
          break;
      }
    }
  };
  auto remove_from_reserve = [&](const array<Pii, 4>& rectangle_to_remove) {
    for (int i = 0; i < 4; i++) {
      auto next_dir = get_direction_from_to(rectangle_to_remove[i], rectangle_to_remove[(i+1)%4]);
      switch (next_dir) {
        case DIR_D:
        case DIR_U:
          reserved_line_d[rectangle_to_remove[i].second] &= ~get_line_bitmask_d(rectangle_to_remove[i], rectangle_to_remove[(i+1)%4]);
          break;
        case DIR_DR:
        case DIR_UL:
          reserved_line_dr[get_lane_dr(rectangle_to_remove[i].first, rectangle_to_remove[i].second)] &= ~get_line_bitmask_dr(rectangle_to_remove[i], rectangle_to_remove[(i+1)%4]);
          break;
        case DIR_R:
        case DIR_L:
          reserved_line_r[rectangle_to_remove[i].first] &= ~get_line_bitmask_r(rectangle_to_remove[i], rectangle_to_remove[(i+1)%4]);
          break;
        case DIR_UR:
        case DIR_DL:
          reserved_line_dl[get_lane_dl(rectangle_to_remove[i].first, rectangle_to_remove[i].second)] &= ~get_line_bitmask_dl(rectangle_to_remove[i], rectangle_to_remove[(i+1)%4]);
          break;
      }
    }
  };
  auto check_reserved = [&](const Pii target_point) {
    return ((reserved_line_d[target_point.second] & get_line_bitmask_d(target_point, make_pair(target_point.first + 1, target_point.second)))
         || (reserved_line_dr[get_lane_dr(target_point.first, target_point.second)] & get_line_bitmask_dr(target_point, make_pair(target_point.first + 1, target_point.second + 1)))
         || (reserved_line_r[target_point.first] & get_line_bitmask_r(target_point, make_pair(target_point.first, target_point.second + 1)))
         || (reserved_line_dl[get_lane_dl(target_point.first, target_point.second)] & get_line_bitmask_dl(target_point, make_pair(target_point.first + 1, target_point.second - 1))));
  };

  constexpr int step_limit = 100;
  double skip_chance = 0.2;
  int step_count = 0;
  function<bool(Pii, Pii, uchar, vector<array<Pii, 4>>&)> search_next_rectangle_chain_from_existing_point;
  auto search_next_rectangle_chain_from_single_point = [&](auto self, Pii point_to_search, uchar dirset_to_check_from_new_point, uchar dirset_to_check_existing_point, vector<array<Pii, 4>>& rectangle_to_create) {
    step_count++;
    if (step_count > step_limit) return false;

    vector<uchar> dir_to_search;
    uchar temp_dir = 0;
    double roll = theRandom.nextDouble();

    if (roll < 0.5) {
      dir_to_search.clear();
      while (get_next_dir_from_dirset(dirset_to_check_existing_point, temp_dir)) {
        dir_to_search.push_back(temp_dir);
      }
      shuffle(dir_to_search.begin(), dir_to_search.end(), theMersenne);

      for (auto next_dir: dir_to_search) {
        auto [_, next_row, next_col] = get_next_point(point_to_search, next_dir);
        if (!within_board(next_row, next_col)) continue;
        if (temporary_points.find(make_pair(next_row, next_col)) != temporary_points.end()) continue;
        add_to_existing_point(point_to_search);
        auto success = search_next_rectangle_chain_from_existing_point(make_pair(next_row, next_col), point_to_search, dirset_to_search_from_existing_point[next_dir], rectangle_to_create);
        remove_from_existing_point(point_to_search);
        if (success) return true;
      }
    }
    
    while (get_next_dir_from_dirset(dirset_to_check_from_new_point, temp_dir)) {
      dir_to_search.push_back(temp_dir);
    }
    shuffle(dir_to_search.begin(), dir_to_search.end(), theMersenne);

    for (auto next_dir: dir_to_search) {
      switch (next_dir) {
        case DIR_D:
        {
          const auto [lane_base_dl, row_base_dl, col_base_dl] = get_next_point_dl(point_to_search); // find other base points
          const auto [lane_base_dr, row_base_dr, col_base_dr] = get_next_point_dr(point_to_search);
          if (!(within_board(row_base_dl, col_base_dl) && within_board(row_base_dr, col_base_dr))) continue;
          if (temporary_points.find(make_pair(row_base_dl, col_base_dl)) != temporary_points.end() || temporary_points.find(make_pair(row_base_dr, col_base_dr)) != temporary_points.end()) continue;
          const auto row_target = row_base_dl + row_base_dr - point_to_search.first;
          const auto col_target = col_base_dl + col_base_dr - point_to_search.second;
          if (!within_board(row_target, col_target)) continue;
          if (temporary_points.find(make_pair(row_target, col_target)) != temporary_points.end()) continue;
          if (check_reserved(make_pair(row_target, col_target))) continue;
          const auto [lane_target_dl, row_target_dl, col_target_dl] = get_next_point_ul(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_dr, row_target_dr, col_target_dr] = get_next_point_ur(make_pair(row_target, col_target));
          if (!(row_base_dl == row_target_dl && col_base_dl == col_target_dl) || !(row_base_dr == row_target_dr && col_base_dr == col_target_dr)) continue;
          const auto origin_to_dl_line_mask = current_state.existing_line_dl[lane_base_dl] & get_line_bitmask_dl(point_to_search, make_pair(row_base_dl, col_base_dl)); // check for existing lines
          const auto origin_to_dr_line_mask = current_state.existing_line_dr[lane_base_dr] & get_line_bitmask_dr(point_to_search, make_pair(row_base_dr, col_base_dr));
          const auto target_to_dl_line_mask = current_state.existing_line_dr[lane_target_dl] & get_line_bitmask_ul(make_pair(row_target, col_target), make_pair(row_target_dl, col_target_dl));
          const auto target_to_dr_line_mask = current_state.existing_line_dl[lane_target_dr] & get_line_bitmask_ur(make_pair(row_target, col_target), make_pair(row_target_dr, col_target_dr));
          if (origin_to_dl_line_mask || origin_to_dr_line_mask || target_to_dl_line_mask || target_to_dr_line_mask) continue;
          rectangle_to_create.push_back(array<Pii, 4>({point_to_search, make_pair(row_base_dl, col_base_dl), make_pair(row_target, col_target), make_pair(row_base_dr, col_base_dr)}));
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) {
            if (theRandom.nextDouble() > skip_chance) return true;
          }
          else {
            int rectangle_idx = rectangle_to_create.size()-1;
            add_to_existing_line(rectangle_to_create[rectangle_idx]);
            add_to_existing_point(point_to_search);
            add_to_reserve(rectangle_to_create[rectangle_idx]);
            auto success = self(self, make_pair(row_target, col_target), dirset_to_search_from_new_point[next_dir], dirset_to_search_existing_point[next_dir], rectangle_to_create);
            remove_from_reserve(rectangle_to_create[rectangle_idx]);
            remove_from_existing_point(point_to_search);
            remove_from_existing_line(rectangle_to_create[rectangle_idx]);
            if (success) return true;
          }
          rectangle_to_create.pop_back();
          break;
        }
        case DIR_DR:
        {
          const auto [lane_base_d, row_base_d, col_base_d] = get_next_point_d(point_to_search); // find other base points
          const auto [lane_base_r, row_base_r, col_base_r] = get_next_point_r(point_to_search);
          if (!(within_board(row_base_d, col_base_d) && within_board(row_base_r, col_base_r))) continue;
          if (temporary_points.find(make_pair(row_base_d, col_base_d)) != temporary_points.end() || temporary_points.find(make_pair(row_base_r, col_base_r)) != temporary_points.end()) continue;
          const auto row_target = row_base_d + row_base_r - point_to_search.first;
          const auto col_target = col_base_d + col_base_r - point_to_search.second;
          if (!within_board(row_target, col_target)) continue;
          if (temporary_points.find(make_pair(row_target, col_target)) != temporary_points.end()) continue;
          if (check_reserved(make_pair(row_target, col_target))) continue;
          const auto [lane_target_d, row_target_d, col_target_d] = get_next_point_l(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_r, row_target_r, col_target_r] = get_next_point_u(make_pair(row_target, col_target));
          if (!(row_base_d == row_target_d && col_base_d == col_target_d) || !(row_base_r == row_target_r && col_base_r == col_target_r)) continue;
          const auto origin_to_d_line_mask = current_state.existing_line_d[lane_base_d] & get_line_bitmask_d(point_to_search, make_pair(row_base_d, col_base_d)); // check for existing lines
          const auto origin_to_r_line_mask = current_state.existing_line_r[lane_base_r] & get_line_bitmask_r(point_to_search, make_pair(row_base_r, col_base_r));
          const auto target_to_d_line_mask = current_state.existing_line_r[lane_target_d] & get_line_bitmask_l(make_pair(row_target, col_target), make_pair(row_target_d, col_target_d));
          const auto target_to_r_line_mask = current_state.existing_line_d[lane_target_r] & get_line_bitmask_u(make_pair(row_target, col_target), make_pair(row_target_r, col_target_r));
          if (origin_to_d_line_mask || origin_to_r_line_mask || target_to_d_line_mask || target_to_r_line_mask) continue;
          rectangle_to_create.push_back(array<Pii, 4>({point_to_search, make_pair(row_base_d, col_base_d), make_pair(row_target, col_target), make_pair(row_base_r, col_base_r)}));
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) {
            if (theRandom.nextDouble() > skip_chance) return true;
          }
          else {
            int rectangle_idx = rectangle_to_create.size()-1;
            add_to_existing_line(rectangle_to_create[rectangle_idx]);
            add_to_existing_point(point_to_search);
            add_to_reserve(rectangle_to_create[rectangle_idx]);
            auto success = self(self, make_pair(row_target, col_target), dirset_to_search_from_new_point[next_dir], dirset_to_search_existing_point[next_dir], rectangle_to_create);
            remove_from_reserve(rectangle_to_create[rectangle_idx]);
            remove_from_existing_point(point_to_search);
            remove_from_existing_line(rectangle_to_create[rectangle_idx]);
            if (success) return true;
          }
          rectangle_to_create.pop_back();
          break;
        }
        case DIR_R:
        {
          const auto [lane_base_dr, row_base_dr, col_base_dr] = get_next_point_dr(point_to_search); // find other base points
          const auto [lane_base_ur, row_base_ur, col_base_ur] = get_next_point_ur(point_to_search);
          if (!(within_board(row_base_dr, col_base_dr) && within_board(row_base_ur, col_base_ur))) continue;
          if (temporary_points.find(make_pair(row_base_dr, col_base_dr)) != temporary_points.end() || temporary_points.find(make_pair(row_base_ur, col_base_ur)) != temporary_points.end()) continue;
          const auto row_target = row_base_dr + row_base_ur - point_to_search.first;
          const auto col_target = col_base_dr + col_base_ur - point_to_search.second;
          if (!within_board(row_target, col_target)) continue;
          if (temporary_points.find(make_pair(row_target, col_target)) != temporary_points.end()) continue;
          if (check_reserved(make_pair(row_target, col_target))) continue;
          const auto [lane_target_dr, row_target_dr, col_target_dr] = get_next_point_dl(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_ur, row_target_ur, col_target_ur] = get_next_point_ul(make_pair(row_target, col_target));
          if (!(row_base_dr == row_target_dr && col_base_dr == col_target_dr) || !(row_base_ur == row_target_ur && col_base_ur == col_target_ur)) continue;
          const auto origin_to_dr_line_mask = current_state.existing_line_dr[lane_base_dr] & get_line_bitmask_dr(point_to_search, make_pair(row_base_dr, col_base_dr)); // check for existing lines
          const auto origin_to_ur_line_mask = current_state.existing_line_dl[lane_base_ur] & get_line_bitmask_ur(point_to_search, make_pair(row_base_ur, col_base_ur));
          const auto target_to_dr_line_mask = current_state.existing_line_dl[lane_target_dr] & get_line_bitmask_dl(make_pair(row_target, col_target), make_pair(row_target_dr, col_target_dr));
          const auto target_to_ur_line_mask = current_state.existing_line_dr[lane_target_ur] & get_line_bitmask_ul(make_pair(row_target, col_target), make_pair(row_target_ur, col_target_ur));
          if (origin_to_dr_line_mask || origin_to_ur_line_mask || target_to_dr_line_mask || target_to_ur_line_mask) continue;
          rectangle_to_create.push_back(array<Pii, 4>({point_to_search, make_pair(row_base_dr, col_base_dr), make_pair(row_target, col_target), make_pair(row_base_ur, col_base_ur)}));
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) {
            if (theRandom.nextDouble() > skip_chance) return true;
          }
          else {
            int rectangle_idx = rectangle_to_create.size()-1;
            add_to_existing_line(rectangle_to_create[rectangle_idx]);
            add_to_existing_point(point_to_search);
            add_to_reserve(rectangle_to_create[rectangle_idx]);
            auto success = self(self, make_pair(row_target, col_target), dirset_to_search_from_new_point[next_dir], dirset_to_search_existing_point[next_dir], rectangle_to_create);
            remove_from_reserve(rectangle_to_create[rectangle_idx]);
            remove_from_existing_point(point_to_search);
            remove_from_existing_line(rectangle_to_create[rectangle_idx]);
            if (success) return true;
          }
          rectangle_to_create.pop_back();
          break;
        }
        case DIR_UR:
        {
          const auto [lane_base_r, row_base_r, col_base_r] = get_next_point_r(point_to_search); // find other base points
          const auto [lane_base_u, row_base_u, col_base_u] = get_next_point_u(point_to_search);
          if (!(within_board(row_base_r, col_base_r) && within_board(row_base_u, col_base_u))) continue;
          if (temporary_points.find(make_pair(row_base_r, col_base_r)) != temporary_points.end() || temporary_points.find(make_pair(row_base_u, col_base_u)) != temporary_points.end()) continue;
          const auto row_target = row_base_r + row_base_u - point_to_search.first;
          const auto col_target = col_base_r + col_base_u - point_to_search.second;
          if (!within_board(row_target, col_target)) continue;
          if (temporary_points.find(make_pair(row_target, col_target)) != temporary_points.end()) continue;
          if (check_reserved(make_pair(row_target, col_target))) continue;
          const auto [lane_target_r, row_target_r, col_target_r] = get_next_point_d(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_u, row_target_u, col_target_u] = get_next_point_l(make_pair(row_target, col_target));
          if (!(row_base_r == row_target_r && col_base_r == col_target_r) || !(row_base_u == row_target_u && col_base_u == col_target_u)) continue;
          const auto origin_to_r_line_mask = current_state.existing_line_r[lane_base_r] & get_line_bitmask_r(point_to_search, make_pair(row_base_r, col_base_r)); // check for existing lines
          const auto origin_to_u_line_mask = current_state.existing_line_d[lane_base_u] & get_line_bitmask_u(point_to_search, make_pair(row_base_u, col_base_u));
          const auto target_to_r_line_mask = current_state.existing_line_d[lane_target_r] & get_line_bitmask_d(make_pair(row_target, col_target), make_pair(row_target_r, col_target_r));
          const auto target_to_u_line_mask = current_state.existing_line_r[lane_target_u] & get_line_bitmask_l(make_pair(row_target, col_target), make_pair(row_target_u, col_target_u));
          if (origin_to_r_line_mask || origin_to_u_line_mask || target_to_r_line_mask || target_to_u_line_mask) continue;
          rectangle_to_create.push_back(array<Pii, 4>({point_to_search, make_pair(row_base_r, col_base_r), make_pair(row_target, col_target), make_pair(row_base_u, col_base_u)}));
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) {
            if (theRandom.nextDouble() > skip_chance) return true;
          }
          else {
            int rectangle_idx = rectangle_to_create.size()-1;
            add_to_existing_line(rectangle_to_create[rectangle_idx]);
            add_to_existing_point(point_to_search);
            add_to_reserve(rectangle_to_create[rectangle_idx]);
            auto success = self(self, make_pair(row_target, col_target), dirset_to_search_from_new_point[next_dir], dirset_to_search_existing_point[next_dir], rectangle_to_create);
            remove_from_reserve(rectangle_to_create[rectangle_idx]);
            remove_from_existing_point(point_to_search);
            remove_from_existing_line(rectangle_to_create[rectangle_idx]);
            if (success) return true;
          }
          rectangle_to_create.pop_back();
          break;
        }
        case DIR_U:
        {
          const auto [lane_base_ur, row_base_ur, col_base_ur] = get_next_point_ur(point_to_search); // find other base points
          const auto [lane_base_ul, row_base_ul, col_base_ul] = get_next_point_ul(point_to_search);
          if (!(within_board(row_base_ur, col_base_ur) && within_board(row_base_ul, col_base_ul))) continue;
          if (temporary_points.find(make_pair(row_base_ur, col_base_ur)) != temporary_points.end() || temporary_points.find(make_pair(row_base_ul, col_base_ul)) != temporary_points.end()) continue;
          const auto row_target = row_base_ur + row_base_ul - point_to_search.first;
          const auto col_target = col_base_ur + col_base_ul - point_to_search.second;
          if (!within_board(row_target, col_target)) continue;
          if (temporary_points.find(make_pair(row_target, col_target)) != temporary_points.end()) continue;
          if (check_reserved(make_pair(row_target, col_target))) continue;
          const auto [lane_target_ur, row_target_ur, col_target_ur] = get_next_point_dr(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_ul, row_target_ul, col_target_ul] = get_next_point_dl(make_pair(row_target, col_target));
          if (!(row_base_ur == row_target_ur && col_base_ur == col_target_ur) || !(row_base_ul == row_target_ul && col_base_ul == col_target_ul)) continue;
          const auto origin_to_ur_line_mask = current_state.existing_line_dl[lane_base_ur] & get_line_bitmask_ur(point_to_search, make_pair(row_base_ur, col_base_ur)); // check for existing lines
          const auto origin_to_ul_line_mask = current_state.existing_line_dr[lane_base_ul] & get_line_bitmask_ul(point_to_search, make_pair(row_base_ul, col_base_ul));
          const auto target_to_ur_line_mask = current_state.existing_line_dr[lane_target_ur] & get_line_bitmask_dr(make_pair(row_target, col_target), make_pair(row_target_ur, col_target_ur));
          const auto target_to_ul_line_mask = current_state.existing_line_dl[lane_target_ul] & get_line_bitmask_dl(make_pair(row_target, col_target), make_pair(row_target_ul, col_target_ul));
          if (origin_to_ur_line_mask || origin_to_ul_line_mask || target_to_ur_line_mask || target_to_ul_line_mask) continue;
          rectangle_to_create.push_back(array<Pii, 4>({point_to_search, make_pair(row_base_ur, col_base_ur), make_pair(row_target, col_target), make_pair(row_base_ul, col_base_ul)}));
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) {
            if (theRandom.nextDouble() > skip_chance) return true;
          }
          else {
            int rectangle_idx = rectangle_to_create.size()-1;
            add_to_existing_line(rectangle_to_create[rectangle_idx]);
            add_to_existing_point(point_to_search);
            add_to_reserve(rectangle_to_create[rectangle_idx]);
            auto success = self(self, make_pair(row_target, col_target), dirset_to_search_from_new_point[next_dir], dirset_to_search_existing_point[next_dir], rectangle_to_create);
            remove_from_reserve(rectangle_to_create[rectangle_idx]);
            remove_from_existing_point(point_to_search);
            remove_from_existing_line(rectangle_to_create[rectangle_idx]);
            if (success) return true;
          }
          rectangle_to_create.pop_back();
          break;
        }
        case DIR_UL:
        {
          const auto [lane_base_u, row_base_u, col_base_u] = get_next_point_u(point_to_search); // find other base points
          const auto [lane_base_l, row_base_l, col_base_l] = get_next_point_l(point_to_search);
          if (!(within_board(row_base_u, col_base_u) && within_board(row_base_l, col_base_l))) continue;
          if (temporary_points.find(make_pair(row_base_u, col_base_u)) != temporary_points.end() || temporary_points.find(make_pair(row_base_l, col_base_l)) != temporary_points.end()) continue;
          const auto row_target = row_base_u + row_base_l - point_to_search.first;
          const auto col_target = col_base_u + col_base_l - point_to_search.second;
          if (!within_board(row_target, col_target)) continue;
          if (temporary_points.find(make_pair(row_target, col_target)) != temporary_points.end()) continue;
          if (check_reserved(make_pair(row_target, col_target))) continue;
          const auto [lane_target_u, row_target_u, col_target_u] = get_next_point_r(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_l, row_target_l, col_target_l] = get_next_point_d(make_pair(row_target, col_target));
          if (!(row_base_u == row_target_u && col_base_u == col_target_u) || !(row_base_l == row_target_l && col_base_l == col_target_l)) continue;
          const auto origin_to_u_line_mask = current_state.existing_line_d[lane_base_u] & get_line_bitmask_u(point_to_search, make_pair(row_base_u, col_base_u)); // check for existing lines
          const auto origin_to_l_line_mask = current_state.existing_line_r[lane_base_l] & get_line_bitmask_l(point_to_search, make_pair(row_base_l, col_base_l));
          const auto target_to_u_line_mask = current_state.existing_line_r[lane_target_u] & get_line_bitmask_r(make_pair(row_target, col_target), make_pair(row_target_u, col_target_u));
          const auto target_to_l_line_mask = current_state.existing_line_d[lane_target_l] & get_line_bitmask_d(make_pair(row_target, col_target), make_pair(row_target_l, col_target_l));
          if (origin_to_u_line_mask || origin_to_l_line_mask || target_to_u_line_mask || target_to_l_line_mask) continue;
          rectangle_to_create.push_back(array<Pii, 4>({point_to_search, make_pair(row_base_u, col_base_u), make_pair(row_target, col_target), make_pair(row_base_l, col_base_l)}));
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) {
            if (theRandom.nextDouble() > skip_chance) return true;
          }
          else {
            int rectangle_idx = rectangle_to_create.size()-1;
            add_to_existing_line(rectangle_to_create[rectangle_idx]);
            add_to_existing_point(point_to_search);
            add_to_reserve(rectangle_to_create[rectangle_idx]);
            auto success = self(self, make_pair(row_target, col_target), dirset_to_search_from_new_point[next_dir], dirset_to_search_existing_point[next_dir], rectangle_to_create);
            remove_from_reserve(rectangle_to_create[rectangle_idx]);
            remove_from_existing_point(point_to_search);
            remove_from_existing_line(rectangle_to_create[rectangle_idx]);
            if (success) return true;
          }
          rectangle_to_create.pop_back();
          break;
        }
        case DIR_L:
        {
          const auto [lane_base_ul, row_base_ul, col_base_ul] = get_next_point_ul(point_to_search); // find other base points
          const auto [lane_base_dl, row_base_dl, col_base_dl] = get_next_point_dl(point_to_search);
          if (!(within_board(row_base_ul, col_base_ul) && within_board(row_base_dl, col_base_dl))) continue;
          if (temporary_points.find(make_pair(row_base_ul, col_base_ul)) != temporary_points.end() || temporary_points.find(make_pair(row_base_dl, col_base_dl)) != temporary_points.end()) continue;
          const auto row_target = row_base_ul + row_base_dl - point_to_search.first;
          const auto col_target = col_base_ul + col_base_dl - point_to_search.second;
          if (!within_board(row_target, col_target)) continue;
          if (temporary_points.find(make_pair(row_target, col_target)) != temporary_points.end()) continue;
          if (check_reserved(make_pair(row_target, col_target))) continue;
          const auto [lane_target_ul, row_target_ul, col_target_ul] = get_next_point_ur(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_dl, row_target_dl, col_target_dl] = get_next_point_dr(make_pair(row_target, col_target));
          if (!(row_base_ul == row_target_ul && col_base_ul == col_target_ul) || !(row_base_dl == row_target_dl && col_base_dl == col_target_dl)) continue;
          const auto origin_to_ul_line_mask = current_state.existing_line_dr[lane_base_ul] & get_line_bitmask_ul(point_to_search, make_pair(row_base_ul, col_base_ul)); // check for existing lines
          const auto origin_to_dl_line_mask = current_state.existing_line_dl[lane_base_dl] & get_line_bitmask_dl(point_to_search, make_pair(row_base_dl, col_base_dl));
          const auto target_to_ul_line_mask = current_state.existing_line_dl[lane_target_ul] & get_line_bitmask_ur(make_pair(row_target, col_target), make_pair(row_target_ul, col_target_ul));
          const auto target_to_dl_line_mask = current_state.existing_line_dr[lane_target_dl] & get_line_bitmask_dr(make_pair(row_target, col_target), make_pair(row_target_dl, col_target_dl));
          if (origin_to_ul_line_mask || origin_to_dl_line_mask || target_to_ul_line_mask || target_to_dl_line_mask) continue;
          rectangle_to_create.push_back(array<Pii, 4>({point_to_search, make_pair(row_base_ul, col_base_ul), make_pair(row_target, col_target), make_pair(row_base_dl, col_base_dl)}));
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) {
            if (theRandom.nextDouble() > skip_chance) return true;
          }
          else {
            int rectangle_idx = rectangle_to_create.size()-1;
            add_to_existing_line(rectangle_to_create[rectangle_idx]);
            add_to_existing_point(point_to_search);
            add_to_reserve(rectangle_to_create[rectangle_idx]);
            auto success = self(self, make_pair(row_target, col_target), dirset_to_search_from_new_point[next_dir], dirset_to_search_existing_point[next_dir], rectangle_to_create);
            remove_from_reserve(rectangle_to_create[rectangle_idx]);
            remove_from_existing_point(point_to_search);
            remove_from_existing_line(rectangle_to_create[rectangle_idx]);
            if (success) return true;
          }
          rectangle_to_create.pop_back();
          break;
        }
        case DIR_DL:
        {
          const auto [lane_base_l, row_base_l, col_base_l] = get_next_point_l(point_to_search); // find other base points
          const auto [lane_base_d, row_base_d, col_base_d] = get_next_point_d(point_to_search);
          if (!(within_board(row_base_l, col_base_l) && within_board(row_base_d, col_base_d))) continue;
          if (temporary_points.find(make_pair(row_base_l, col_base_l)) != temporary_points.end() || temporary_points.find(make_pair(row_base_d, col_base_d)) != temporary_points.end()) continue;
          const auto row_target = row_base_l + row_base_d - point_to_search.first;
          const auto col_target = col_base_l + col_base_d - point_to_search.second;
          if (!within_board(row_target, col_target)) continue;
          if (temporary_points.find(make_pair(row_target, col_target)) != temporary_points.end()) continue;
          if (check_reserved(make_pair(row_target, col_target))) continue;
          const auto [lane_target_l, row_target_l, col_target_l] = get_next_point_u(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_d, row_target_d, col_target_d] = get_next_point_r(make_pair(row_target, col_target));
          if (!(row_base_l == row_target_l && col_base_l == col_target_l) || !(row_base_d == row_target_d && col_base_d == col_target_d)) continue;
          const auto origin_to_l_line_mask = current_state.existing_line_r[lane_base_l] & get_line_bitmask_l(point_to_search, make_pair(row_base_l, col_base_l)); // check for existing lines
          const auto origin_to_d_line_mask = current_state.existing_line_d[lane_base_d] & get_line_bitmask_d(point_to_search, make_pair(row_base_d, col_base_d));
          const auto target_to_l_line_mask = current_state.existing_line_d[lane_target_l] & get_line_bitmask_u(make_pair(row_target, col_target), make_pair(row_target_l, col_target_l));
          const auto target_to_d_line_mask = current_state.existing_line_r[lane_target_d] & get_line_bitmask_r(make_pair(row_target, col_target), make_pair(row_target_d, col_target_d));
          if (origin_to_l_line_mask || origin_to_d_line_mask || target_to_l_line_mask || target_to_d_line_mask) continue;
          rectangle_to_create.push_back(array<Pii, 4>({point_to_search, make_pair(row_base_l, col_base_l), make_pair(row_target, col_target), make_pair(row_base_d, col_base_d)}));
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) {
            if (theRandom.nextDouble() > skip_chance) return true;
          }
          else {
            int rectangle_idx = rectangle_to_create.size()-1;
            add_to_existing_line(rectangle_to_create[rectangle_idx]);
            add_to_existing_point(point_to_search);
            add_to_reserve(rectangle_to_create[rectangle_idx]);
            auto success = self(self, make_pair(row_target, col_target), dirset_to_search_from_new_point[next_dir], dirset_to_search_existing_point[next_dir], rectangle_to_create);
            remove_from_reserve(rectangle_to_create[rectangle_idx]);
            remove_from_existing_point(point_to_search);
            remove_from_existing_line(rectangle_to_create[rectangle_idx]);
            if (success) return true;
          }
          rectangle_to_create.pop_back();
          break;
        }
      }
    }

    if (roll >= 0.5) {
      dir_to_search.clear();
      while (get_next_dir_from_dirset(dirset_to_check_existing_point, temp_dir)) {
        dir_to_search.push_back(temp_dir);
      }
      shuffle(dir_to_search.begin(), dir_to_search.end(), theMersenne);

      for (auto next_dir: dir_to_search) {
        auto [_, next_row, next_col] = get_next_point(point_to_search, next_dir);
        if (!within_board(next_row, next_col)) continue;
        if (temporary_points.find(make_pair(next_row, next_col)) != temporary_points.end()) continue;
        add_to_existing_point(point_to_search);
        auto success = search_next_rectangle_chain_from_existing_point(make_pair(next_row, next_col), point_to_search, dirset_to_search_from_existing_point[next_dir], rectangle_to_create);
        remove_from_existing_point(point_to_search);
        if (success) return true;
      }
    }

    return false;
  };
  search_next_rectangle_chain_from_existing_point = [&] (Pii existing_point, Pii new_point, uchar dirset_to_check_from_existing_point, vector<array<Pii, 4>>& rectangle_to_create) {
    step_count++;
    if (step_count > step_limit) return false;

    vector<uchar> dir_to_search;
    uchar temp_dir = 0;
    while (get_next_dir_from_dirset(dirset_to_check_from_existing_point, temp_dir)) {
      dir_to_search.push_back(temp_dir);
    }
    shuffle(dir_to_search.begin(), dir_to_search.end(), theMersenne);

    for (auto next_dir: dir_to_search) {
      switch (next_dir) {
        case DIR_D:
        {
          const auto [lane_base_dl, row_base_dl, col_base_dl] = get_next_point_dl(existing_point); // find other base points
          const auto [lane_base_dr, row_base_dr, col_base_dr] = get_next_point_dr(existing_point);
          if (!(within_board(row_base_dl, col_base_dl) && within_board(row_base_dr, col_base_dr))) continue;
          if ((temporary_points.find(make_pair(row_base_dl, col_base_dl)) != temporary_points.end()) == (temporary_points.find(make_pair(row_base_dr, col_base_dr)) != temporary_points.end())) continue;
          const auto row_target = row_base_dl + row_base_dr - existing_point.first;
          const auto col_target = col_base_dl + col_base_dr - existing_point.second;
          if (!within_board(row_target, col_target)) continue;
          if (temporary_points.find(make_pair(row_target, col_target)) != temporary_points.end()) continue;
          if (check_reserved(make_pair(row_target, col_target))) continue;
          const auto [lane_target_dl, row_target_dl, col_target_dl] = get_next_point_ul(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_dr, row_target_dr, col_target_dr] = get_next_point_ur(make_pair(row_target, col_target));
          if (!(row_base_dl == row_target_dl && col_base_dl == col_target_dl) || !(row_base_dr == row_target_dr && col_base_dr == col_target_dr)) continue;
          const auto origin_to_dl_line_mask = current_state.existing_line_dl[lane_base_dl] & get_line_bitmask_dl(existing_point, make_pair(row_base_dl, col_base_dl)); // check for existing lines
          const auto origin_to_dr_line_mask = current_state.existing_line_dr[lane_base_dr] & get_line_bitmask_dr(existing_point, make_pair(row_base_dr, col_base_dr));
          const auto target_to_dl_line_mask = current_state.existing_line_dr[lane_target_dl] & get_line_bitmask_ul(make_pair(row_target, col_target), make_pair(row_target_dl, col_target_dl));
          const auto target_to_dr_line_mask = current_state.existing_line_dl[lane_target_dr] & get_line_bitmask_ur(make_pair(row_target, col_target), make_pair(row_target_dr, col_target_dr));
          if (origin_to_dl_line_mask || origin_to_dr_line_mask || target_to_dl_line_mask || target_to_dr_line_mask) continue;
          if (row_base_dl == new_point.first && col_base_dl == new_point.second) rectangle_to_create.push_back(array<Pii, 4>({new_point, existing_point, make_pair(row_base_dr, col_base_dr), make_pair(row_target, col_target)}));
          else                                                                   rectangle_to_create.push_back(array<Pii, 4>({new_point, make_pair(row_target, col_target), make_pair(row_base_dl, col_base_dl), existing_point}));
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) {
            if (theRandom.nextDouble() > skip_chance) return true;
          }
          else {
            int rectangle_idx = rectangle_to_create.size()-1;
            add_to_existing_line(rectangle_to_create[rectangle_idx]);
            add_to_reserve(rectangle_to_create[rectangle_idx]);
            auto success = search_next_rectangle_chain_from_single_point(search_next_rectangle_chain_from_single_point, make_pair(row_target, col_target), dirset_to_search_from_new_point[next_dir], dirset_to_search_existing_point[next_dir], rectangle_to_create);
            remove_from_reserve(rectangle_to_create[rectangle_idx]);
            remove_from_existing_line(rectangle_to_create[rectangle_idx]);
            if (success) return true;
          }
          rectangle_to_create.pop_back();
          break;
        }
        case DIR_DR:
        {
          const auto [lane_base_d, row_base_d, col_base_d] = get_next_point_d(existing_point); // find other base points
          const auto [lane_base_r, row_base_r, col_base_r] = get_next_point_r(existing_point);
          if (!(within_board(row_base_d, col_base_d) && within_board(row_base_r, col_base_r))) continue;
          if ((temporary_points.find(make_pair(row_base_d, col_base_d)) != temporary_points.end()) == (temporary_points.find(make_pair(row_base_r, col_base_r)) != temporary_points.end())) continue;
          const auto row_target = row_base_d + row_base_r - existing_point.first;
          const auto col_target = col_base_d + col_base_r - existing_point.second;
          if (!within_board(row_target, col_target)) continue;
          if (temporary_points.find(make_pair(row_target, col_target)) != temporary_points.end()) continue;
          if (check_reserved(make_pair(row_target, col_target))) continue;
          const auto [lane_target_d, row_target_d, col_target_d] = get_next_point_l(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_r, row_target_r, col_target_r] = get_next_point_u(make_pair(row_target, col_target));
          if (!(row_base_d == row_target_d && col_base_d == col_target_d) || !(row_base_r == row_target_r && col_base_r == col_target_r)) continue;
          const auto origin_to_d_line_mask = current_state.existing_line_d[lane_base_d] & get_line_bitmask_d(existing_point, make_pair(row_base_d, col_base_d)); // check for existing lines
          const auto origin_to_r_line_mask = current_state.existing_line_r[lane_base_r] & get_line_bitmask_r(existing_point, make_pair(row_base_r, col_base_r));
          const auto target_to_d_line_mask = current_state.existing_line_r[lane_target_d] & get_line_bitmask_l(make_pair(row_target, col_target), make_pair(row_target_d, col_target_d));
          const auto target_to_r_line_mask = current_state.existing_line_d[lane_target_r] & get_line_bitmask_u(make_pair(row_target, col_target), make_pair(row_target_r, col_target_r));
          if (origin_to_d_line_mask || origin_to_r_line_mask || target_to_d_line_mask || target_to_r_line_mask) continue;
          if (row_base_d == new_point.first && col_base_d == new_point.second) rectangle_to_create.push_back(array<Pii, 4>({new_point, existing_point, make_pair(row_base_r, col_base_r), make_pair(row_target, col_target)}));
          else                                                                 rectangle_to_create.push_back(array<Pii, 4>({new_point, make_pair(row_target, col_target), make_pair(row_base_d, col_base_d), existing_point}));
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) {
            if (theRandom.nextDouble() > skip_chance) return true;
          }
          else {
            int rectangle_idx = rectangle_to_create.size()-1;
            add_to_existing_line(rectangle_to_create[rectangle_idx]);
            add_to_reserve(rectangle_to_create[rectangle_idx]);
            auto success = search_next_rectangle_chain_from_single_point(search_next_rectangle_chain_from_single_point, make_pair(row_target, col_target), dirset_to_search_from_new_point[next_dir], dirset_to_search_existing_point[next_dir], rectangle_to_create);
            remove_from_reserve(rectangle_to_create[rectangle_idx]);
            remove_from_existing_line(rectangle_to_create[rectangle_idx]);
            if (success) return true;
          }
          rectangle_to_create.pop_back();
          break;
        }
        case DIR_R:
        {
          const auto [lane_base_dr, row_base_dr, col_base_dr] = get_next_point_dr(existing_point); // find other base points
          const auto [lane_base_ur, row_base_ur, col_base_ur] = get_next_point_ur(existing_point);
          if (!(within_board(row_base_dr, col_base_dr) && within_board(row_base_ur, col_base_ur))) continue;
          if ((temporary_points.find(make_pair(row_base_dr, col_base_dr)) != temporary_points.end()) == (temporary_points.find(make_pair(row_base_ur, col_base_ur)) != temporary_points.end())) continue;
          const auto row_target = row_base_dr + row_base_ur - existing_point.first;
          const auto col_target = col_base_dr + col_base_ur - existing_point.second;
          if (!within_board(row_target, col_target)) continue;
          if (temporary_points.find(make_pair(row_target, col_target)) != temporary_points.end()) continue;
          if (check_reserved(make_pair(row_target, col_target))) continue;
          const auto [lane_target_dr, row_target_dr, col_target_dr] = get_next_point_dl(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_ur, row_target_ur, col_target_ur] = get_next_point_ul(make_pair(row_target, col_target));
          if (!(row_base_dr == row_target_dr && col_base_dr == col_target_dr) || !(row_base_ur == row_target_ur && col_base_ur == col_target_ur)) continue;
          const auto origin_to_dr_line_mask = current_state.existing_line_dr[lane_base_dr] & get_line_bitmask_dr(existing_point, make_pair(row_base_dr, col_base_dr)); // check for existing lines
          const auto origin_to_ur_line_mask = current_state.existing_line_dl[lane_base_ur] & get_line_bitmask_ur(existing_point, make_pair(row_base_ur, col_base_ur));
          const auto target_to_dr_line_mask = current_state.existing_line_dl[lane_target_dr] & get_line_bitmask_dl(make_pair(row_target, col_target), make_pair(row_target_dr, col_target_dr));
          const auto target_to_ur_line_mask = current_state.existing_line_dr[lane_target_ur] & get_line_bitmask_ul(make_pair(row_target, col_target), make_pair(row_target_ur, col_target_ur));
          if (origin_to_dr_line_mask || origin_to_ur_line_mask || target_to_dr_line_mask || target_to_ur_line_mask) continue;
          if (row_base_dr == new_point.first && col_base_dr == new_point.second) rectangle_to_create.push_back(array<Pii, 4>({new_point, existing_point, make_pair(row_base_ur, col_base_ur), make_pair(row_target, col_target)}));
          else                                                                   rectangle_to_create.push_back(array<Pii, 4>({new_point, make_pair(row_target, col_target), make_pair(row_base_dr, col_base_dr), existing_point}));
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) {
            if (theRandom.nextDouble() > skip_chance) return true;
          }
          else {
            int rectangle_idx = rectangle_to_create.size()-1;
            add_to_existing_line(rectangle_to_create[rectangle_idx]);
            add_to_reserve(rectangle_to_create[rectangle_idx]);
            auto success = search_next_rectangle_chain_from_single_point(search_next_rectangle_chain_from_single_point, make_pair(row_target, col_target), dirset_to_search_from_new_point[next_dir], dirset_to_search_existing_point[next_dir], rectangle_to_create);
            remove_from_reserve(rectangle_to_create[rectangle_idx]);
            remove_from_existing_line(rectangle_to_create[rectangle_idx]);
            if (success) return true;
          }
          rectangle_to_create.pop_back();
          break;
        }
        case DIR_UR:
        {
          const auto [lane_base_r, row_base_r, col_base_r] = get_next_point_r(existing_point); // find other base points
          const auto [lane_base_u, row_base_u, col_base_u] = get_next_point_u(existing_point);
          if (!(within_board(row_base_r, col_base_r) && within_board(row_base_u, col_base_u))) continue;
          if ((temporary_points.find(make_pair(row_base_r, col_base_r)) != temporary_points.end()) == (temporary_points.find(make_pair(row_base_u, col_base_u)) != temporary_points.end())) continue;
          const auto row_target = row_base_r + row_base_u - existing_point.first;
          const auto col_target = col_base_r + col_base_u - existing_point.second;
          if (!within_board(row_target, col_target)) continue;
          if (temporary_points.find(make_pair(row_target, col_target)) != temporary_points.end()) continue;
          if (check_reserved(make_pair(row_target, col_target))) continue;
          const auto [lane_target_r, row_target_r, col_target_r] = get_next_point_d(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_u, row_target_u, col_target_u] = get_next_point_l(make_pair(row_target, col_target));
          if (!(row_base_r == row_target_r && col_base_r == col_target_r) || !(row_base_u == row_target_u && col_base_u == col_target_u)) continue;
          const auto origin_to_r_line_mask = current_state.existing_line_r[lane_base_r] & get_line_bitmask_r(existing_point, make_pair(row_base_r, col_base_r)); // check for existing lines
          const auto origin_to_u_line_mask = current_state.existing_line_d[lane_base_u] & get_line_bitmask_u(existing_point, make_pair(row_base_u, col_base_u));
          const auto target_to_r_line_mask = current_state.existing_line_d[lane_target_r] & get_line_bitmask_d(make_pair(row_target, col_target), make_pair(row_target_r, col_target_r));
          const auto target_to_u_line_mask = current_state.existing_line_r[lane_target_u] & get_line_bitmask_l(make_pair(row_target, col_target), make_pair(row_target_u, col_target_u));
          if (origin_to_r_line_mask || origin_to_u_line_mask || target_to_r_line_mask || target_to_u_line_mask) continue;
          if (row_base_r == new_point.first && col_base_r == new_point.second) rectangle_to_create.push_back(array<Pii, 4>({new_point, existing_point, make_pair(row_base_u, col_base_u), make_pair(row_target, col_target)}));
          else                                                                 rectangle_to_create.push_back(array<Pii, 4>({new_point, make_pair(row_target, col_target), make_pair(row_base_r, col_base_r), existing_point}));
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) {
            if (theRandom.nextDouble() > skip_chance) return true;
          }
          else {
            int rectangle_idx = rectangle_to_create.size()-1;
            add_to_existing_line(rectangle_to_create[rectangle_idx]);
            add_to_reserve(rectangle_to_create[rectangle_idx]);
            auto success = search_next_rectangle_chain_from_single_point(search_next_rectangle_chain_from_single_point, make_pair(row_target, col_target), dirset_to_search_from_new_point[next_dir], dirset_to_search_existing_point[next_dir], rectangle_to_create);
            remove_from_reserve(rectangle_to_create[rectangle_idx]);
            remove_from_existing_line(rectangle_to_create[rectangle_idx]);
            if (success) return true;
          }
          rectangle_to_create.pop_back();
          break;
        }
        case DIR_U:
        {
          const auto [lane_base_ur, row_base_ur, col_base_ur] = get_next_point_ur(existing_point); // find other base points
          const auto [lane_base_ul, row_base_ul, col_base_ul] = get_next_point_ul(existing_point);
          if (!(within_board(row_base_ur, col_base_ur) && within_board(row_base_ul, col_base_ul))) continue;
          if ((temporary_points.find(make_pair(row_base_ur, col_base_ur)) != temporary_points.end()) == (temporary_points.find(make_pair(row_base_ul, col_base_ul)) != temporary_points.end())) continue;
          const auto row_target = row_base_ur + row_base_ul - existing_point.first;
          const auto col_target = col_base_ur + col_base_ul - existing_point.second;
          if (!within_board(row_target, col_target)) continue;
          if (temporary_points.find(make_pair(row_target, col_target)) != temporary_points.end()) continue;
          if (check_reserved(make_pair(row_target, col_target))) continue;
          const auto [lane_target_ur, row_target_ur, col_target_ur] = get_next_point_dr(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_ul, row_target_ul, col_target_ul] = get_next_point_dl(make_pair(row_target, col_target));
          if (!(row_base_ur == row_target_ur && col_base_ur == col_target_ur) || !(row_base_ul == row_target_ul && col_base_ul == col_target_ul)) continue;
          const auto origin_to_ur_line_mask = current_state.existing_line_dl[lane_base_ur] & get_line_bitmask_ur(existing_point, make_pair(row_base_ur, col_base_ur)); // check for existing lines
          const auto origin_to_ul_line_mask = current_state.existing_line_dr[lane_base_ul] & get_line_bitmask_ul(existing_point, make_pair(row_base_ul, col_base_ul));
          const auto target_to_ur_line_mask = current_state.existing_line_dr[lane_target_ur] & get_line_bitmask_dr(make_pair(row_target, col_target), make_pair(row_target_ur, col_target_ur));
          const auto target_to_ul_line_mask = current_state.existing_line_dl[lane_target_ul] & get_line_bitmask_dl(make_pair(row_target, col_target), make_pair(row_target_ul, col_target_ul));
          if (origin_to_ur_line_mask || origin_to_ul_line_mask || target_to_ur_line_mask || target_to_ul_line_mask) continue;
          if (row_base_ur == new_point.first && col_base_ur == new_point.second) rectangle_to_create.push_back(array<Pii, 4>({new_point, existing_point, make_pair(row_base_ul, col_base_ul), make_pair(row_target, col_target)}));
          else                                                                   rectangle_to_create.push_back(array<Pii, 4>({new_point, make_pair(row_target, col_target), make_pair(row_base_ur, col_base_ur), existing_point}));
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) {
            if (theRandom.nextDouble() > skip_chance) return true;
          }
          else {
            int rectangle_idx = rectangle_to_create.size()-1;
            add_to_existing_line(rectangle_to_create[rectangle_idx]);
            add_to_reserve(rectangle_to_create[rectangle_idx]);
            auto success = search_next_rectangle_chain_from_single_point(search_next_rectangle_chain_from_single_point, make_pair(row_target, col_target), dirset_to_search_from_new_point[next_dir], dirset_to_search_existing_point[next_dir], rectangle_to_create);
            remove_from_reserve(rectangle_to_create[rectangle_idx]);
            remove_from_existing_line(rectangle_to_create[rectangle_idx]);
            if (success) return true;
          }
          rectangle_to_create.pop_back();
          break;
        }
        case DIR_UL:
        {
          const auto [lane_base_u, row_base_u, col_base_u] = get_next_point_u(existing_point); // find other base points
          const auto [lane_base_l, row_base_l, col_base_l] = get_next_point_l(existing_point);
          if (!(within_board(row_base_u, col_base_u) && within_board(row_base_l, col_base_l))) continue;
          if ((temporary_points.find(make_pair(row_base_u, col_base_u)) != temporary_points.end()) == (temporary_points.find(make_pair(row_base_l, col_base_l)) != temporary_points.end())) continue;
          const auto row_target = row_base_u + row_base_l - existing_point.first;
          const auto col_target = col_base_u + col_base_l - existing_point.second;
          if (!within_board(row_target, col_target)) continue;
          if (temporary_points.find(make_pair(row_target, col_target)) != temporary_points.end()) continue;
          if (check_reserved(make_pair(row_target, col_target))) continue;
          const auto [lane_target_u, row_target_u, col_target_u] = get_next_point_r(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_l, row_target_l, col_target_l] = get_next_point_d(make_pair(row_target, col_target));
          if (!(row_base_u == row_target_u && col_base_u == col_target_u) || !(row_base_l == row_target_l && col_base_l == col_target_l)) continue;
          const auto origin_to_u_line_mask = current_state.existing_line_d[lane_base_u] & get_line_bitmask_u(existing_point, make_pair(row_base_u, col_base_u)); // check for existing lines
          const auto origin_to_l_line_mask = current_state.existing_line_r[lane_base_l] & get_line_bitmask_l(existing_point, make_pair(row_base_l, col_base_l));
          const auto target_to_u_line_mask = current_state.existing_line_r[lane_target_u] & get_line_bitmask_r(make_pair(row_target, col_target), make_pair(row_target_u, col_target_u));
          const auto target_to_l_line_mask = current_state.existing_line_d[lane_target_l] & get_line_bitmask_d(make_pair(row_target, col_target), make_pair(row_target_l, col_target_l));
          if (origin_to_u_line_mask || origin_to_l_line_mask || target_to_u_line_mask || target_to_l_line_mask) continue;
          if (row_base_u == new_point.first && col_base_u == new_point.second) rectangle_to_create.push_back(array<Pii, 4>({new_point, existing_point, make_pair(row_base_l, col_base_l), make_pair(row_target, col_target)}));
          else                                                                 rectangle_to_create.push_back(array<Pii, 4>({new_point, make_pair(row_target, col_target), make_pair(row_base_u, col_base_u), existing_point}));
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) {
            if (theRandom.nextDouble() > skip_chance) return true;
          }
          else {
            int rectangle_idx = rectangle_to_create.size()-1;
            add_to_existing_line(rectangle_to_create[rectangle_idx]);
            add_to_reserve(rectangle_to_create[rectangle_idx]);
            auto success = search_next_rectangle_chain_from_single_point(search_next_rectangle_chain_from_single_point, make_pair(row_target, col_target), dirset_to_search_from_new_point[next_dir], dirset_to_search_existing_point[next_dir], rectangle_to_create);
            remove_from_reserve(rectangle_to_create[rectangle_idx]);
            remove_from_existing_line(rectangle_to_create[rectangle_idx]);
            if (success) return true;
          }
          rectangle_to_create.pop_back();
          break;
        }
        case DIR_L:
        {
          const auto [lane_base_ul, row_base_ul, col_base_ul] = get_next_point_ul(existing_point); // find other base points
          const auto [lane_base_dl, row_base_dl, col_base_dl] = get_next_point_dl(existing_point);
          if (!(within_board(row_base_ul, col_base_ul) && within_board(row_base_dl, col_base_dl))) continue;
          if ((temporary_points.find(make_pair(row_base_ul, col_base_ul)) != temporary_points.end()) == (temporary_points.find(make_pair(row_base_dl, col_base_dl)) != temporary_points.end())) continue;
          const auto row_target = row_base_ul + row_base_dl - existing_point.first;
          const auto col_target = col_base_ul + col_base_dl - existing_point.second;
          if (!within_board(row_target, col_target)) continue;
          if (temporary_points.find(make_pair(row_target, col_target)) != temporary_points.end()) continue;
          if (check_reserved(make_pair(row_target, col_target))) continue;
          const auto [lane_target_ul, row_target_ul, col_target_ul] = get_next_point_ur(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_dl, row_target_dl, col_target_dl] = get_next_point_dr(make_pair(row_target, col_target));
          if (!(row_base_ul == row_target_ul && col_base_ul == col_target_ul) || !(row_base_dl == row_target_dl && col_base_dl == col_target_dl)) continue;
          const auto origin_to_ul_line_mask = current_state.existing_line_dr[lane_base_ul] & get_line_bitmask_ul(existing_point, make_pair(row_base_ul, col_base_ul)); // check for existing lines
          const auto origin_to_dl_line_mask = current_state.existing_line_dl[lane_base_dl] & get_line_bitmask_dl(existing_point, make_pair(row_base_dl, col_base_dl));
          const auto target_to_ul_line_mask = current_state.existing_line_dl[lane_target_ul] & get_line_bitmask_ur(make_pair(row_target, col_target), make_pair(row_target_ul, col_target_ul));
          const auto target_to_dl_line_mask = current_state.existing_line_dr[lane_target_dl] & get_line_bitmask_dr(make_pair(row_target, col_target), make_pair(row_target_dl, col_target_dl));
          if (origin_to_ul_line_mask || origin_to_dl_line_mask || target_to_ul_line_mask || target_to_dl_line_mask) continue;
          if (row_base_ul == new_point.first && col_base_ul == new_point.second) rectangle_to_create.push_back(array<Pii, 4>({new_point, existing_point, make_pair(row_base_dl, col_base_dl), make_pair(row_target, col_target)}));
          else                                                                   rectangle_to_create.push_back(array<Pii, 4>({new_point, make_pair(row_target, col_target), make_pair(row_base_ul, col_base_ul), existing_point}));
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) {
            if (theRandom.nextDouble() > skip_chance) return true;
          }
          else {
            int rectangle_idx = rectangle_to_create.size()-1;
            add_to_existing_line(rectangle_to_create[rectangle_idx]);
            add_to_reserve(rectangle_to_create[rectangle_idx]);
            auto success = search_next_rectangle_chain_from_single_point(search_next_rectangle_chain_from_single_point, make_pair(row_target, col_target), dirset_to_search_from_new_point[next_dir], dirset_to_search_existing_point[next_dir], rectangle_to_create);
            remove_from_reserve(rectangle_to_create[rectangle_idx]);
            remove_from_existing_line(rectangle_to_create[rectangle_idx]);
            if (success) return true;
          }
          rectangle_to_create.pop_back();
          break;
        }
        case DIR_DL:
        {
          const auto [lane_base_l, row_base_l, col_base_l] = get_next_point_l(existing_point); // find other base points
          const auto [lane_base_d, row_base_d, col_base_d] = get_next_point_d(existing_point);
          if (!(within_board(row_base_l, col_base_l) && within_board(row_base_d, col_base_d))) continue;
          if ((temporary_points.find(make_pair(row_base_l, col_base_l)) != temporary_points.end()) == (temporary_points.find(make_pair(row_base_d, col_base_d)) != temporary_points.end())) continue;
          const auto row_target = row_base_l + row_base_d - existing_point.first;
          const auto col_target = col_base_l + col_base_d - existing_point.second;
          if (!within_board(row_target, col_target)) continue;
          if (temporary_points.find(make_pair(row_target, col_target)) != temporary_points.end()) continue;
          if (check_reserved(make_pair(row_target, col_target))) continue;
          const auto [lane_target_l, row_target_l, col_target_l] = get_next_point_u(make_pair(row_target, col_target)); // check base points from target
          const auto [lane_target_d, row_target_d, col_target_d] = get_next_point_r(make_pair(row_target, col_target));
          if (!(row_base_l == row_target_l && col_base_l == col_target_l) || !(row_base_d == row_target_d && col_base_d == col_target_d)) continue;
          const auto origin_to_l_line_mask = current_state.existing_line_r[lane_base_l] & get_line_bitmask_l(existing_point, make_pair(row_base_l, col_base_l)); // check for existing lines
          const auto origin_to_d_line_mask = current_state.existing_line_d[lane_base_d] & get_line_bitmask_d(existing_point, make_pair(row_base_d, col_base_d));
          const auto target_to_l_line_mask = current_state.existing_line_d[lane_target_l] & get_line_bitmask_u(make_pair(row_target, col_target), make_pair(row_target_l, col_target_l));
          const auto target_to_d_line_mask = current_state.existing_line_r[lane_target_d] & get_line_bitmask_r(make_pair(row_target, col_target), make_pair(row_target_d, col_target_d));
          if (origin_to_l_line_mask || origin_to_d_line_mask || target_to_l_line_mask || target_to_d_line_mask) continue;
          if (row_base_l == new_point.first && col_base_l == new_point.second) rectangle_to_create.push_back(array<Pii, 4>({new_point, existing_point, make_pair(row_base_d, col_base_d), make_pair(row_target, col_target)}));
          else                                                                 rectangle_to_create.push_back(array<Pii, 4>({new_point, make_pair(row_target, col_target), make_pair(row_base_l, col_base_l), existing_point}));
          if (current_state.existing_point.find(make_pair(row_target, col_target)) != current_state.existing_point.end()) {
            if (theRandom.nextDouble() > skip_chance) return true;
          }
          else {
            int rectangle_idx = rectangle_to_create.size()-1;
            add_to_existing_line(rectangle_to_create[rectangle_idx]);
            add_to_reserve(rectangle_to_create[rectangle_idx]);
            auto success = search_next_rectangle_chain_from_single_point(search_next_rectangle_chain_from_single_point, make_pair(row_target, col_target), dirset_to_search_from_new_point[next_dir], dirset_to_search_existing_point[next_dir], rectangle_to_create);
            remove_from_reserve(rectangle_to_create[rectangle_idx]);
            remove_from_existing_line(rectangle_to_create[rectangle_idx]);
            if (success) return true;
          }
          rectangle_to_create.pop_back();
          break;
        }
      }
    }

    return false;
  };

  // down
  for (int i = 0; i < board_size - 2; i += 2) {
    for (int j = 1; j <= board_size / 4; j++) {
      add_to_existing_line(array<Pii, 4>({make_pair(i, j), make_pair(i + 1, j + 1), make_pair(i + 2, j), make_pair(i + 1, j - 1)}));
    }
  }
  for (int j = 0; j < board_size / 4; j++) {
    for (int i = j % 2; i < board_size - 1; i += 2) {
      add_to_existing_line(array<Pii, 4>({make_pair(i, j), make_pair(i, j + 1), make_pair(i + 1, j + 1), make_pair(i + 1, j)}));
    }
  }

  // up
  for (int i = 0; i < board_size - 2; i += 2) {
    for (int j = board_size * 3 / 4; j < board_size - 1; j++) {
      add_to_existing_line(array<Pii, 4>({make_pair(i, j), make_pair(i + 1, j + 1), make_pair(i + 2, j), make_pair(i + 1, j - 1)}));
    }
  }
  for (int j = board_size * 3 / 4; j < board_size - 1; j++) {
    for (int i = j % 2; i < board_size - 1; i += 2) {
      add_to_existing_line(array<Pii, 4>({make_pair(i, j), make_pair(i, j + 1), make_pair(i + 1, j + 1), make_pair(i + 1, j)}));
    }
  }

  // left
  for (int j = 0; j < board_size - 2; j += 2) {
    for (int i = 1; i <= board_size / 4; i++) {
      add_to_existing_line(array<Pii, 4>({make_pair(i, j), make_pair(i - 1, j + 1), make_pair(i, j + 2), make_pair(i + 1, j + 1)}));
    }
  }
  for (int i = 0; i < board_size / 4; i++) {
    for (int j = i % 2; j < board_size - 1; j += 2) {
      add_to_existing_line(array<Pii, 4>({make_pair(i, j), make_pair(i, j + 1), make_pair(i + 1, j + 1), make_pair(i + 1, j)}));
    }
  }

  // right
  for (int j = 0; j < board_size - 2; j += 2) {
    for (int i = board_size * 3 / 4; i < board_size - 1; i++) {
      add_to_existing_line(array<Pii, 4>({make_pair(i, j), make_pair(i - 1, j + 1), make_pair(i, j + 2), make_pair(i + 1, j + 1)}));
    }
  }
  for (int i = board_size * 3 / 4; i < board_size - 1; i++) {
    for (int j = i % 2; j < board_size - 1; j += 2) {
      add_to_existing_line(array<Pii, 4>({make_pair(i, j), make_pair(i, j + 1), make_pair(i + 1, j + 1), make_pair(i + 1, j)}));
    }
  }


  auto calc_bonus_score = [&]() {
    int score = 0;
    int factor = 1;

    // down
    factor = 1;
    for (int i = 0; i < board_size - 2; i += 2) {
      if (current_state.existing_point.find(make_pair(i    , board_size / 4))     != current_state.existing_point.end()
       && current_state.existing_point.find(make_pair(i + 1, board_size / 4))     != current_state.existing_point.end()
       && current_state.existing_point.find(make_pair(i + 1, board_size / 4 + 1)) != current_state.existing_point.end()
       && current_state.existing_point.find(make_pair(i + 2, board_size / 4))     != current_state.existing_point.end()) {
        score += hyper_init_bonus_base * factor;
        factor += 1;
      }
      else {
        factor = 1;
      }
    }
    
    // up
    factor = 1;
    for (int i = 0; i < board_size - 2; i += 2) {
      if (current_state.existing_point.find(make_pair(i    , board_size * 3 / 4))     != current_state.existing_point.end()
       && current_state.existing_point.find(make_pair(i + 1, board_size * 3 / 4))     != current_state.existing_point.end()
       && current_state.existing_point.find(make_pair(i + 1, board_size * 3 / 4 - 1)) != current_state.existing_point.end()
       && current_state.existing_point.find(make_pair(i + 2, board_size * 3 / 4))     != current_state.existing_point.end()) {
        score += hyper_init_bonus_base * factor;
        factor += 1;
      }
      else {
        factor = 1;
      }
    }

    // left
    factor = 1;
    for (int j = 0; j < board_size - 2; j += 2) {
      if (current_state.existing_point.find(make_pair(board_size / 4    , j))     != current_state.existing_point.end()
       && current_state.existing_point.find(make_pair(board_size / 4    , j + 1)) != current_state.existing_point.end()
       && current_state.existing_point.find(make_pair(board_size / 4 + 1, j + 1)) != current_state.existing_point.end()
       && current_state.existing_point.find(make_pair(board_size / 4    , j + 2)) != current_state.existing_point.end()) {
        score += hyper_init_bonus_base * factor;
        factor += 1;
      }
      else {
        factor = 1;
      }
    }
    
    // right
    factor = 1;
    for (int j = 0; j < board_size - 2; j += 2) {
      if (current_state.existing_point.find(make_pair(board_size * 3 / 4    , j))     != current_state.existing_point.end()
       && current_state.existing_point.find(make_pair(board_size * 3 / 4    , j + 1)) != current_state.existing_point.end()
       && current_state.existing_point.find(make_pair(board_size * 3 / 4 - 1, j + 1)) != current_state.existing_point.end()
       && current_state.existing_point.find(make_pair(board_size * 3 / 4    , j + 2)) != current_state.existing_point.end()) {
        score += hyper_init_bonus_base * factor;
        factor += 1;
      }
      else {
        factor = 1;
      }
    }

    return score;
  };

  vector<array<Pii, 4>> rectangle_to_create;

  create_ans_from_set();
  prev_state = current_state;

  auto best_state = current_state;
  int score = calc_actual_score(calc_raw_score()) + calc_bonus_score();
  int last_score = score;
  int best_score = score;

  const double base_temperature = exp(log(hyper_init_annealing_temperature_min_n_begin) - ((log(hyper_init_annealing_temperature_min_n_begin) - log(hyper_init_annealing_temperature_max_n_begin)) * (((double) board_size - 31.0) / 30.0)));
  const double target_temperature = exp(log(hyper_init_annealing_temperature_min_n_end) - ((log(hyper_init_annealing_temperature_min_n_end) - log(hyper_init_annealing_temperature_max_n_end)) * (((double) board_size - 31.0) / 30.0)));
  // const double decay_rate = 4e-5;
  double temperature = base_temperature;

  double time_start = theTimer.time();
  const double time_limit = 3.900 * ((double) (initial_point.size() - board_size) / (double) (board_size * board_size / 12 - board_size));
  int iter_count = 0;

  while (theTimer.time() < time_limit) {
    double roll = theRandom.nextDouble();
    if (roll < 0.80) {
      do {
        vector<int> id_list;
        for (auto &[id, _]: current_state.rectangle_set_to_create) {
          id_list.push_back(id);
        }
        if (!id_list.empty()) {
          int idx = theRandom.nextUIntMod(id_list.size());
          auto id_to_remove = id_list[idx];
          remove_point_descend(current_state.rectangle_set_to_create[id_to_remove][0]);
        }
      } while (theRandom.nextDouble() < hyper_init_remove_descend_extra_remove_chance);

      create_next_solution();
      create_ans_from_set();

      score = calc_actual_score(calc_raw_score()) + calc_bonus_score();

      #ifdef DEBUG
      if (iter_count % 1000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= last_score) {
        last_score = score;
        prev_state = current_state;
        if (score > best_score) {
          best_score = score;
          best_state = current_state;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
        last_score = score;
        prev_state = current_state;
      }
      else { // rollback
        score = last_score;
        current_state = prev_state;
      }
    }
    else if (roll < 1.00) {
      do {
        vector<int> id_list;
        for (auto &[id, _]: current_state.rectangle_set_to_create) {
          id_list.push_back(id);
        }
        if (!id_list.empty()) {
          int idx = theRandom.nextUIntMod(id_list.size());
          auto id_to_remove = id_list[idx];
          remove_point_full(current_state.rectangle_set_to_create[id_to_remove][0], true);
        }
      } while (theRandom.nextDouble() < hyper_init_remove_full_extra_remove_chance);

      create_next_solution();
      create_ans_from_set();

      score = calc_actual_score(calc_raw_score()) + calc_bonus_score();

      #ifdef DEBUG
      if (iter_count % 1000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= last_score) {
        last_score = score;
        prev_state = current_state;
        if (score > best_score) {
          best_score = score;
          best_state = current_state;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
        last_score = score;
        prev_state = current_state;
      }
      else { // rollback
        score = last_score;
        current_state = prev_state;
      }
    }

    // temperature *= 1.0 - decay_rate;
    temperature = exp(log(base_temperature) - ((log(base_temperature) - log(target_temperature)) * ((theTimer.time() - time_start) * (1.0 / (time_limit - time_start)))));
    iter_count++;
  }

  current_state = best_state;

  // down
  for (int i = 0; i < board_size - 2; i += 2) {
    for (int j = 1; j <= board_size / 4; j++) {
      remove_from_existing_line(array<Pii, 4>({make_pair(i, j), make_pair(i + 1, j + 1), make_pair(i + 2, j), make_pair(i + 1, j - 1)}));
    }
  }
  for (int j = 0; j < board_size / 4; j++) {
    for (int i = j % 2; i < board_size - 1; i += 2) {
      remove_from_existing_line(array<Pii, 4>({make_pair(i, j), make_pair(i, j + 1), make_pair(i + 1, j + 1), make_pair(i + 1, j)}));
    }
  }

  // up
  for (int i = 0; i < board_size - 2; i += 2) {
    for (int j = board_size * 3 / 4; j < board_size - 1; j++) {
      remove_from_existing_line(array<Pii, 4>({make_pair(i, j), make_pair(i + 1, j + 1), make_pair(i + 2, j), make_pair(i + 1, j - 1)}));
    }
  }
  for (int j = board_size * 3 / 4; j < board_size - 1; j++) {
    for (int i = j % 2; i < board_size - 1; i += 2) {
      remove_from_existing_line(array<Pii, 4>({make_pair(i, j), make_pair(i, j + 1), make_pair(i + 1, j + 1), make_pair(i + 1, j)}));
    }
  }

  // left
  for (int j = 0; j < board_size - 2; j += 2) {
    for (int i = 1; i <= board_size / 4; i++) {
      remove_from_existing_line(array<Pii, 4>({make_pair(i, j), make_pair(i - 1, j + 1), make_pair(i, j + 2), make_pair(i + 1, j + 1)}));
    }
  }
  for (int i = 0; i < board_size / 4; i++) {
    for (int j = i % 2; j < board_size - 1; j += 2) {
      remove_from_existing_line(array<Pii, 4>({make_pair(i, j), make_pair(i, j + 1), make_pair(i + 1, j + 1), make_pair(i + 1, j)}));
    }
  }

  // right
  for (int j = 0; j < board_size - 2; j += 2) {
    for (int i = board_size * 3 / 4; i < board_size - 1; i++) {
      remove_from_existing_line(array<Pii, 4>({make_pair(i, j), make_pair(i - 1, j + 1), make_pair(i, j + 2), make_pair(i + 1, j + 1)}));
    }
  }
  for (int i = board_size * 3 / 4; i < board_size - 1; i++) {
    for (int j = i % 2; j < board_size - 1; j += 2) {
      remove_from_existing_line(array<Pii, 4>({make_pair(i, j), make_pair(i, j + 1), make_pair(i + 1, j + 1), make_pair(i + 1, j)}));
    }
  }

  skip_chance = 0.0;
  // down
  for (int j = board_size / 4; j >= 0; j--) {
    if (j % 2 != 0) {
      for (int i = 0; i < board_size; i++) {
        if (current_state.existing_point.find(make_pair(i, j)) != current_state.existing_point.end()) continue;
        rectangle_to_create.clear();
        step_count = 0;
        auto result = search_next_rectangle_chain_from_single_point(search_next_rectangle_chain_from_single_point, make_pair(i, j), i % 2 != 0 ? 0x55 : 0xaa, 0x00, rectangle_to_create);
        if (!result) continue;

        for (int idx = rectangle_to_create.size()-1; idx >= 0; idx--) {
          add_rectangle(rectangle_to_create[idx]);
        }
      }
    }
    else {
      for (int i = board_size - 1; i >= 0; i--) {
        if (current_state.existing_point.find(make_pair(i, j)) != current_state.existing_point.end()) continue;
        rectangle_to_create.clear();
        step_count = 0;
        auto result = search_next_rectangle_chain_from_single_point(search_next_rectangle_chain_from_single_point, make_pair(i, j), i % 2 != 0 ? 0x55 : 0xaa, 0x00, rectangle_to_create);
        if (!result) continue;

        for (int idx = rectangle_to_create.size()-1; idx >= 0; idx--) {
          add_rectangle(rectangle_to_create[idx]);
        }
      }
    }
  }
  // up
  for (int j = board_size * 3 / 4; j < board_size; j++) {
    if (j % 2 != 0) {
      for (int i = board_size - 1; i >= 0; i--) {
        if (current_state.existing_point.find(make_pair(i, j)) != current_state.existing_point.end()) continue;
        rectangle_to_create.clear();
        step_count = 0;
        auto result = search_next_rectangle_chain_from_single_point(search_next_rectangle_chain_from_single_point, make_pair(i, j), i % 2 != 0 ? 0x55 : 0xaa, 0x00, rectangle_to_create);
        if (!result) continue;

        for (int idx = rectangle_to_create.size()-1; idx >= 0; idx--) {
          add_rectangle(rectangle_to_create[idx]);
        }
      }
    }
    else {
      for (int i = 0; i < board_size; i++) {
        if (current_state.existing_point.find(make_pair(i, j)) != current_state.existing_point.end()) continue;
        rectangle_to_create.clear();
        step_count = 0;
        auto result = search_next_rectangle_chain_from_single_point(search_next_rectangle_chain_from_single_point, make_pair(i, j), i % 2 != 0 ? 0x55 : 0xaa, 0x00, rectangle_to_create);
        if (!result) continue;

        for (int idx = rectangle_to_create.size()-1; idx >= 0; idx--) {
          add_rectangle(rectangle_to_create[idx]);
        }
      }
    }
  }
  // left
  for (int i = board_size / 4; i >= 0; i--) {
    if (i % 2 != 0) {
      for (int j = 0; j < board_size; j++) {
        if (current_state.existing_point.find(make_pair(i, j)) != current_state.existing_point.end()) continue;
        rectangle_to_create.clear();
        step_count = 0;
        auto result = search_next_rectangle_chain_from_single_point(search_next_rectangle_chain_from_single_point, make_pair(i, j), j % 2 != 0 ? 0x55 : 0xaa, 0x00, rectangle_to_create);
        if (!result) continue;

        for (int idx = rectangle_to_create.size()-1; idx >= 0; idx--) {
          add_rectangle(rectangle_to_create[idx]);
        }
      }
    }
    else {
      for (int j = board_size - 1; j >= 0; j--) {
        if (current_state.existing_point.find(make_pair(i, j)) != current_state.existing_point.end()) continue;
        rectangle_to_create.clear();
        step_count = 0;
        auto result = search_next_rectangle_chain_from_single_point(search_next_rectangle_chain_from_single_point, make_pair(i, j), j % 2 != 0 ? 0x55 : 0xaa, 0x00, rectangle_to_create);
        if (!result) continue;

        for (int idx = rectangle_to_create.size()-1; idx >= 0; idx--) {
          add_rectangle(rectangle_to_create[idx]);
        }
      }
    }
  }
  // right
  for (int i = board_size * 3 / 4; i < board_size; i++) {
    if (i % 2 != 0) {
      for (int j = board_size - 1; j >= 0; j--) {
        if (current_state.existing_point.find(make_pair(i, j)) != current_state.existing_point.end()) continue;
        rectangle_to_create.clear();
        step_count = 0;
        auto result = search_next_rectangle_chain_from_single_point(search_next_rectangle_chain_from_single_point, make_pair(i, j), j % 2 != 0 ? 0x55 : 0xaa, 0x00, rectangle_to_create);
        if (!result) continue;

        for (int idx = rectangle_to_create.size()-1; idx >= 0; idx--) {
          add_rectangle(rectangle_to_create[idx]);
        }
      }
    }
    else {
      for (int j = 0; j < board_size; j++) {
        if (current_state.existing_point.find(make_pair(i, j)) != current_state.existing_point.end()) continue;
        rectangle_to_create.clear();
        step_count = 0;
        auto result = search_next_rectangle_chain_from_single_point(search_next_rectangle_chain_from_single_point, make_pair(i, j), j % 2 != 0 ? 0x55 : 0xaa, 0x00, rectangle_to_create);
        if (!result) continue;

        for (int idx = rectangle_to_create.size()-1; idx >= 0; idx--) {
          add_rectangle(rectangle_to_create[idx]);
        }
      }
    }
  }

  create_ans_from_set();

  cerr << "iter_count   = " << iter_count << endl;
  cerr << "score        = " << score << endl;
  cerr << "best_score   = " << best_score << endl;
  cerr << "temperature  = " << temperature << endl;
}



void solve() {
  create_next_solution();
  create_ans_from_set();
  prev_state = current_state;

  auto best_state = current_state;
  int score = calc_actual_score(calc_raw_score());
  int last_score = score;
  int best_score = score;

  const double base_temperature = exp(log(hyper_next_annealing_temperature_min_n_begin) - ((log(hyper_next_annealing_temperature_min_n_begin) - log(hyper_next_annealing_temperature_max_n_begin)) * (((double) board_size - 31.0) / 30.0)));
  const double target_temperature = exp(log(hyper_next_annealing_temperature_min_n_end) - ((log(hyper_next_annealing_temperature_min_n_end) - log(hyper_next_annealing_temperature_max_n_end)) * (((double) board_size - 31.0) / 30.0)));
  // const double decay_rate = 4e-5;
  double temperature = base_temperature;

  double time_start = theTimer.time();
  const double time_limit = 4.900;
  int iter_count = 0;

  while (theTimer.time() < time_limit) {
    double roll = theRandom.nextDouble();
    if (roll < 0.80) {
      do {
        vector<int> id_list;
        for (auto &[id, _]: current_state.rectangle_set_to_create) {
          id_list.push_back(id);
        }
        if (!id_list.empty()) {
          int idx = theRandom.nextUIntMod(id_list.size());
          auto id_to_remove = id_list[idx];
          remove_point_descend(current_state.rectangle_set_to_create[id_to_remove][0]);
        }
      } while (theRandom.nextDouble() < hyper_next_remove_descend_extra_remove_chance);

      create_next_solution();
      create_ans_from_set();

      score = calc_actual_score(calc_raw_score());

      #ifdef DEBUG
      if (iter_count % 1000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= last_score) {
        last_score = score;
        prev_state = current_state;
        if (score > best_score) {
          best_score = score;
          best_state = current_state;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
        last_score = score;
        prev_state = current_state;
      }
      else { // rollback
        score = last_score;
        current_state = prev_state;
      }
    }
    else if (roll < 1.00) {
      do {
        vector<int> id_list;
        for (auto &[id, _]: current_state.rectangle_set_to_create) {
          id_list.push_back(id);
        }
        if (!id_list.empty()) {
          int idx = theRandom.nextUIntMod(id_list.size());
          auto id_to_remove = id_list[idx];
          remove_point_full(current_state.rectangle_set_to_create[id_to_remove][0], true);
        }
      } while (theRandom.nextDouble() < hyper_next_remove_full_extra_remove_chance);

      create_next_solution();
      create_ans_from_set();

      score = calc_actual_score(calc_raw_score());

      #ifdef DEBUG
      if (iter_count % 1000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= last_score) {
        last_score = score;
        prev_state = current_state;
        if (score > best_score) {
          best_score = score;
          best_state = current_state;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
        last_score = score;
        prev_state = current_state;
      }
      else { // rollback
        score = last_score;
        current_state = prev_state;
      }
    }

    // temperature *= 1.0 - decay_rate;
    temperature = exp(log(base_temperature) - ((log(base_temperature) - log(target_temperature)) * ((theTimer.time() - time_start) * (1.0 / (time_limit - time_start)))));
    iter_count++;
  }

  current_state = best_state;
  create_ans_from_set();

  cerr << "iter_count   = " << iter_count << endl;
  cerr << "score        = " << score << endl;
  cerr << "best_score   = " << best_score << endl;
  cerr << "temperature  = " << temperature << endl;
}

void solve2() {
  create_initial_solution();
  solve();

  cerr << "score = " << calc_actual_score(calc_raw_score()) << endl;
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  if (argc > 1) hyper_init_bonus_base = strtol(argv[1], nullptr, 10);
  if (argc > 2) hyper_init_annealing_temperature_min_n_begin = strtod(argv[2], nullptr);
  if (argc > 3) hyper_init_annealing_temperature_min_n_end = strtod(argv[3], nullptr);
  if (argc > 4) hyper_init_annealing_temperature_max_n_begin = strtod(argv[4], nullptr);
  if (argc > 5) hyper_init_annealing_temperature_max_n_end = strtod(argv[5], nullptr);
  if (argc > 6) hyper_init_remove_descend_extra_remove_chance = strtod(argv[6], nullptr);
  if (argc > 7) hyper_init_remove_full_extra_remove_chance = strtod(argv[7], nullptr);
  if (argc > 8) hyper_next_annealing_temperature_min_n_begin = strtod(argv[8], nullptr);
  if (argc > 9) hyper_next_annealing_temperature_min_n_end = strtod(argv[9], nullptr);
  if (argc > 10) hyper_next_annealing_temperature_max_n_begin = strtod(argv[10], nullptr);
  if (argc > 11) hyper_next_annealing_temperature_max_n_end = strtod(argv[11], nullptr);
  if (argc > 12) hyper_next_remove_descend_extra_remove_chance = strtod(argv[12], nullptr);
  if (argc > 13) hyper_next_remove_full_extra_remove_chance = strtod(argv[13], nullptr);

  get_input();

  init();
  solve2();

  output_ans();

  return 0;
}
