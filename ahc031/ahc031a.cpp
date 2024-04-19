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
#include <memory>
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
        //return __rdtsc() * 3.333333333333333e-10; // 1 / 3.0e9, for AWS EC2 C5 (Xeon Platinum 8124M / Xeon Platinum 8275CL)
        return __rdtsc() * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C6i (Xeon Platinum 8375C)
      #endif
    #else
      unsigned long long l, h;
        __asm__ ("rdtsc" : "=a"(l), "=d"(h));
      #ifdef LOCAL
        return (double)(l | h << 32) * 2.857142857142857e-10; // 1 / 3.5e9, for local (Ryzen 9 3950X)
      #else
        //return (double)(l | h << 32) * 3.5714285714285715e-10; // 1 / 2.8e9, for AWS EC2 C3 (Xeon E5-2680 v2)
        //return (double)(l | h << 32) * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C4 (Xeon E5-2666 v3)
        //return (double)(l | h << 32) * 3.333333333333333e-10; // 1 / 3.0e9, for AWS EC2 C5 (Xeon Platinum 8124M / Xeon Platinum 8275CL)
        return (double)(l | h << 32) * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C6i (Xeon Platinum 8375C)
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
mt19937_64 theMersenne(1);

// libraries
namespace Lib {
}

template <typename T>
T ceil_div(T a, T b) {
  return (a + b - 1) / b;
}

// hyper parameters
namespace HyperParameter {
  void load_hyper_parameter(int argc, char *argv[]) {
    // do nothing
  }
}

// enums

// structs
struct Rectangle {
  int x_min, y_min, x_max, y_max;
  Rectangle(): x_min(0), y_min(0), x_max(0), y_max(0) {}
  Rectangle(int x_min, int y_min, int x_max, int y_max): x_min(x_min), y_min(y_min), x_max(x_max), y_max(y_max) {}
};

// constants
struct Consts {
  static constexpr ll insufficient_area_penalty_factor = 100;
};

// inputs
struct Input {
  int grid_size;
  int day_num;
  int reservation_per_day;
  vector<vector<int>> required_area;
};

// outputs
struct Output {
  vector<vector<Rectangle>> reservation_area;
};

// internals
struct Environment {
  int grid_size;
  int day_num;
  int reservation_per_day;
  vector<vector<int>> required_area;
};

Input get_input() {
  Input input;

  cin >> input.grid_size >> input.day_num >> input.reservation_per_day;
  input.required_area = vector<vector<int>>(input.day_num, vector<int>(input.reservation_per_day));
  for (int i = 0; i < input.day_num; i++) {
    for (int j = 0; j < input.reservation_per_day; j++) {
      cin >> input.required_area[i][j];
    }
  }

  return input;
}

Environment create_environment(const Input &input) {
  Environment env;
  env.grid_size = input.grid_size;
  env.day_num = input.day_num;
  env.reservation_per_day = input.reservation_per_day;
  env.required_area = input.required_area;

  return env;
}

ll calculate_score(const Environment &env, const Output &output) {
  ll insufficient_area_score = 0;
  ll separator_score = 0;
  unordered_map<int, vector<pair<int, int>>> sep_horizontal_prev;
  unordered_map<int, vector<pair<int, int>>> sep_vertical_prev;

  const auto get_total_sep_length = [](const vector<pair<int, int>> &vec) {
    int total_sep_length = 0;
    for (const auto &[a, b] : vec) {
      total_sep_length += b - a;
    }
    return total_sep_length;
  };

  const auto get_total_sep_diff_length = [](const vector<pair<int, int>> &vec1, const vector<pair<int, int>> &vec2) {
    int total_sep_diff_length = 0;
    int p1 = 0;
    bool next_is_max1 = false;
    int p2 = 0;
    bool next_is_max2 = false;
    int prev_val = 0;
    bool is_different = false;
    while (p1 < (int) vec1.size() && p2 < (int) vec2.size()) {
      int val1 = next_is_max1 ? vec1[p1].second : vec1[p1].first;
      int val2 = next_is_max2 ? vec2[p2].second : vec2[p2].first;
      if (is_different) {
        total_sep_diff_length += min(val1, val2) - prev_val;
      }
      prev_val = min(val1, val2);
      is_different = (val1 == val2) ? is_different : !is_different;
      if (val1 <= val2) {
        p1 += next_is_max1 ? 1 : 0;
        next_is_max1 = !next_is_max1;
      }
      if (val2 <= val1) {
        p2 += next_is_max2 ? 1 : 0;
        next_is_max2 = !next_is_max2;
      }
    }
    for (; p1 < (int) vec1.size(); p1++) {
      if (is_different) {
        total_sep_diff_length += vec1[p1].second - prev_val;
      }
      else {
        total_sep_diff_length += vec1[p1].second - vec1[p1].first;
      }
    }
    for (; p2 < (int) vec2.size(); p2++) {
      if (is_different) {
        total_sep_diff_length += vec2[p2].second - prev_val;
      }
      else {
        total_sep_diff_length += vec2[p2].second - vec2[p2].first;
      }
    }
    return total_sep_diff_length;
  };

  for (int i = 0; i < (int) output.reservation_area.size(); i++) {
    const auto &reservation_area_of_day = output.reservation_area[i];

    for (int j = 0; j < (int) reservation_area_of_day.size(); j++) {
      const auto &rect = reservation_area_of_day[j];
      const int rect_area = (rect.x_max - rect.x_min) * (rect.y_max - rect.y_min);
      const int required_area = env.required_area[i][j];
      if (rect_area < required_area) {
        insufficient_area_score += (required_area - rect_area) * Consts::insufficient_area_penalty_factor;
      }
    }

    unordered_map<int, vector<pair<int, int>>> sep_horizontal_now_before_merge;
    unordered_map<int, vector<pair<int, int>>> sep_vertical_now_before_merge;
    for (int j = 0; j < (int) reservation_area_of_day.size(); j++) {
      const auto &rect = reservation_area_of_day[j];
      if (rect.x_min > 0) {
        sep_vertical_now_before_merge[rect.x_min].emplace_back(rect.y_min, rect.y_max);
      }
      if (rect.x_max < env.grid_size) {
        sep_vertical_now_before_merge[rect.x_max].emplace_back(rect.y_min, rect.y_max);
      }
      if (rect.y_min > 0) {
        sep_horizontal_now_before_merge[rect.y_min].emplace_back(rect.x_min, rect.x_max);
      }
      if (rect.y_max < env.grid_size) {
        sep_horizontal_now_before_merge[rect.y_max].emplace_back(rect.x_min, rect.x_max);
      }
    }

    unordered_map<int, vector<pair<int, int>>> sep_horizontal_now_after_merge;
    unordered_map<int, vector<pair<int, int>>> sep_vertical_now_after_merge;
    for (auto &[x, vec] : sep_horizontal_now_before_merge) {
      sort(vec.begin(), vec.end());
      vector<pair<int, int>> merged;
      for (const auto &[y_min, y_max] : vec) {
        if (merged.empty() || merged.back().second < y_min) {
          merged.emplace_back(y_min, y_max);
        } else {
          merged.back().second = max(merged.back().second, y_max);
        }
      }
      sep_horizontal_now_after_merge[x] = merged;
    }
    for (auto &[y, vec] : sep_vertical_now_before_merge) {
      sort(vec.begin(), vec.end());
      vector<pair<int, int>> merged;
      for (const auto &[x_min, x_max] : vec) {
        if (merged.empty() || merged.back().second < x_min) {
          merged.emplace_back(x_min, x_max);
        } else {
          merged.back().second = max(merged.back().second, x_max);
        }
      }
      sep_vertical_now_after_merge[y] = merged;
    }

    if (i > 0) {
      for (const auto &[x, vec_prev] : sep_horizontal_prev) {
        if (sep_horizontal_now_after_merge.find(x) != sep_horizontal_now_after_merge.end()) {
          separator_score += get_total_sep_diff_length(vec_prev, sep_horizontal_now_after_merge[x]);
        } else {
          separator_score += get_total_sep_length(vec_prev);
        }
      }
      for (const auto &[x, vec_now] : sep_horizontal_now_after_merge) {
        if (sep_horizontal_prev.find(x) == sep_horizontal_prev.end()) {
          separator_score += get_total_sep_length(vec_now);
        }
      }
      for (const auto &[y, vec_prev] : sep_vertical_prev) {
        if (sep_vertical_now_after_merge.find(y) != sep_vertical_now_after_merge.end()) {
          separator_score += get_total_sep_diff_length(vec_prev, sep_vertical_now_after_merge[y]);
        } else {
          separator_score += get_total_sep_length(vec_prev);
        }
      }
      for (const auto &[y, vec_now] : sep_vertical_now_after_merge) {
        if (sep_vertical_prev.find(y) == sep_vertical_prev.end()) {
          separator_score += get_total_sep_length(vec_now);
        }
      }
    }

    sep_horizontal_prev = sep_horizontal_now_after_merge;
    sep_vertical_prev = sep_vertical_now_after_merge;
  }

  return insufficient_area_score + separator_score + 1LL;
}

pair<vector<Rectangle>, bool> find_packing(const vector<int> &required_area, const int height, const int width) {
  vector<Rectangle> rectangle_to_reserve(required_area.size());
  vector<bool> reserved(required_area.size());
  int remaining_height = height;
  int remaining_width = width;
  for (int i = 0; i < (int) required_area.size(); i++) {
    if (remaining_height == 0 || remaining_width == 0) return make_pair(vector<Rectangle>(), false); // fail
    int best_spare_area = height * width;
    int best_idx = -1;
    int best_width_or_height = -1;
    bool best_is_vertical = false;
    for (int j = 0; j < (int) required_area.size(); j++) {
      if (reserved[j]) continue;
      int width_on_vertical = ceil_div(required_area[j], remaining_height);
      if (width_on_vertical <= remaining_width) {
        int spare_area_on_vertical = remaining_height * width_on_vertical - required_area[j];
        if (spare_area_on_vertical < best_spare_area) {
          best_spare_area = spare_area_on_vertical;
          best_idx = j;
          best_width_or_height = width_on_vertical;
          best_is_vertical = true;
        }
      }
      int height_on_horizontal = ceil_div(required_area[j], remaining_width);
      if (height_on_horizontal <= remaining_height) {
        int spare_area_on_horizontal = remaining_width * height_on_horizontal - required_area[j];
        if (spare_area_on_horizontal < best_spare_area) {
          best_spare_area = spare_area_on_horizontal;
          best_idx = j;
          best_width_or_height = height_on_horizontal;
          best_is_vertical = false;
        }
      }
    }
    if (best_idx == -1) return make_pair(vector<Rectangle>(), false); // fail
    reserved[best_idx] = true;
    if (best_is_vertical) {
      rectangle_to_reserve[best_idx] = Rectangle(0, remaining_width - best_width_or_height, remaining_height, remaining_width);
      remaining_width -= best_width_or_height;
    }
    else {
      rectangle_to_reserve[best_idx] = Rectangle(remaining_height - best_width_or_height, 0, remaining_height, remaining_width);
      remaining_height -= best_width_or_height;
    }
  }

  return make_pair(rectangle_to_reserve, true);
};

pair<Output, bool> solve_zero_cost(const Environment &env) {
  vector<int> max_required_area(env.reservation_per_day);
  for (int i = 0; i < env.day_num; i++) {
    for (int j = 0; j < env.reservation_per_day; j++) {
      max_required_area[j] = max(max_required_area[j], env.required_area[i][j]);
    }
  }

  auto [rectangle_to_reserve, success] = find_packing(max_required_area, env.grid_size, env.grid_size);
  if (!success) return make_pair(Output(), false);

  Output output;
  output.reservation_area = vector<vector<Rectangle>>(env.day_num, rectangle_to_reserve);
  return make_pair(output, true);
}

Output solve_greedy1(const Environment &env, const int bin_size) {
  vector<vector<Rectangle>> reservation_area(env.day_num, vector<Rectangle>(env.reservation_per_day));
  for (int i = 0; i < env.day_num; i++) {
    vector<vector<int>> bin_idx = vector<vector<int>>(bin_size);
    vector<int> bin_area_sum = vector<int>(bin_size);
    int area_sum = 0;
    for (int j = env.reservation_per_day - 1; j >= 0; j--) {
      int smallest_bin_area_idx = distance(bin_area_sum.begin(), min_element(bin_area_sum.begin(), bin_area_sum.end()));
      bin_idx[smallest_bin_area_idx].push_back(j);
      bin_area_sum[smallest_bin_area_idx] += env.required_area[i][j];
      area_sum += env.required_area[i][j];
    }
    vector<int> bin_height = vector<int>(bin_size);
    int bin_height_sum = 0;
    for (int j = 0; j < bin_size - 1; j++) {
      bin_height[j] = clamp((int) round((double) (env.grid_size * bin_area_sum[j]) / (double) area_sum), 1, env.grid_size - bin_height_sum - (bin_size - (j + 1)));
      bin_height_sum += bin_height[j];
    }
    bin_height[bin_size - 1] = env.grid_size - bin_height_sum;
    for (int j = 0; j < bin_size; j++) {
      int x_min = accumulate(bin_height.begin(), bin_height.begin() + j, 0);
      int x_max = x_min + bin_height[j];
      int next_y_min = 0;
      for (int k = 0; k < (int) bin_idx[j].size() - 1; k++) {
        int y_min = next_y_min;
        int y_max = min(env.grid_size - ((int) bin_idx[j].size() - (k + 1)), y_min + ceil_div(env.required_area[i][bin_idx[j][k]], bin_height[j]));
        next_y_min = y_max;
        reservation_area[i][bin_idx[j][k]] = Rectangle(x_min, y_min, x_max, y_max);
      }
      if (!bin_idx[j].empty()) {
        int y_min = next_y_min;
        int y_max = env.grid_size;
        reservation_area[i][bin_idx[j].back()] = Rectangle(x_min, y_min, x_max, y_max);
      }
    }
  }

  Output output;
  output.reservation_area = reservation_area;
  return output;
}

pair<Output, bool> solve_greedy2(const Environment &env) {
  Output output;
  output.reservation_area = vector<vector<Rectangle>>(env.day_num);
  for (int i = 0; i < env.day_num; i++) {
    auto [rectangle_to_reserve, success] = find_packing(env.required_area[i], env.grid_size, env.grid_size);
    if (!success) return make_pair(Output(), false);
    output.reservation_area[i] = rectangle_to_reserve;
  }
  return make_pair(output, true);
}

pair<Output, bool> solve_greedy3(const Environment &env) {
  vector<vector<bool>> use_for_fixed_packing(env.day_num, vector<bool>(env.reservation_per_day, false));
  vector<vector<int>> fixed_pack_idx_for_fixed_packing(env.day_num, vector<int>(env.reservation_per_day, -1));
  vector<vector<int>> reservation_idxes_of_fixed_packing; // fixed_pack_idx, day_idx
  vector<int> reservation_area_of_fixed_packing;

  Output best_output;
  ll best_score = 1LL << 60;
  for (int i = 0; i < env.reservation_per_day - 1; i++) {
    {
      vector<int> reservation_idx_of_fixed_packing(env.day_num);
      priority_queue<pair<int, int>> area_day_idx_que; 
      int max_area = 0;
      int min_area = 1 << 30;
      for (int j = 0; j < env.day_num; j++) {
        for (int k = env.reservation_per_day - 1; k >= 0; k--) {
          if (use_for_fixed_packing[j][k]) continue;
          reservation_idx_of_fixed_packing[j] = k;
          area_day_idx_que.emplace(env.required_area[j][k], j);
          max_area = max(max_area, env.required_area[j][k]);
          min_area = min(min_area, env.required_area[j][k]);
          break;
        }
      }
      vector<int> best_reservation_idx_of_fixed_packing = reservation_idx_of_fixed_packing;
      int best_max_area = max_area;
      int best_area_range = max_area - min_area;
      while (!area_day_idx_que.empty()) {
        auto [area, day_idx] = area_day_idx_que.top();
        area_day_idx_que.pop();
        int reservation_idx = reservation_idx_of_fixed_packing[day_idx];
        do {
          reservation_idx--;
        } while (use_for_fixed_packing[day_idx][reservation_idx] && reservation_idx >= 0);
        if (reservation_idx < 0) break;
        reservation_idx_of_fixed_packing[day_idx] = reservation_idx;
        int new_area = env.required_area[day_idx][reservation_idx];
        area_day_idx_que.emplace(new_area, day_idx);
        auto [next_area, next_day_idx] = area_day_idx_que.top();
        max_area = next_area;
        min_area = min(min_area, new_area);
        if (max_area - min_area < best_area_range) {
          best_reservation_idx_of_fixed_packing = reservation_idx_of_fixed_packing;
          best_max_area = max_area;
          best_area_range = max_area - min_area;
        }
      }
      for (int j = 0; j < env.day_num; j++) {
        use_for_fixed_packing[j][best_reservation_idx_of_fixed_packing[j]] = true;
        fixed_pack_idx_for_fixed_packing[j][best_reservation_idx_of_fixed_packing[j]] = i;
      }
      reservation_idxes_of_fixed_packing.push_back(best_reservation_idx_of_fixed_packing);
      reservation_area_of_fixed_packing.push_back(best_max_area);
    }
    int width_used_for_fixed_packing;
    vector<Rectangle> fixed_packing_rectangle_to_reserve;
    {
      int ok = env.grid_size + 1;
      int ng = 0;
      while (ok - ng > 1) {
        int mid = (ok + ng) / 2;
        auto [rectangle_to_reserve, success] = find_packing(reservation_area_of_fixed_packing, env.grid_size, mid);
        if (success) {
          ok = mid;
          fixed_packing_rectangle_to_reserve = rectangle_to_reserve;
        } else {
          ng = mid;
        }
      }
      if (ok > env.grid_size) break;
      width_used_for_fixed_packing = ok;
    }
    vector<vector<Rectangle>> reservation_area(env.day_num, vector<Rectangle>(env.reservation_per_day));
    {
      for (int j = 0; j < env.day_num; j++) {
        int height_used = 0;
        int non_fixed_reservation_remaining = env.reservation_per_day - (i + 1);
        for (int k = 0; k < env.reservation_per_day; k++) {
          if (use_for_fixed_packing[j][k]) {
            reservation_area[j][k] = fixed_packing_rectangle_to_reserve[fixed_pack_idx_for_fixed_packing[j][k]];
          }
          else {
            int x_min = height_used;
            int x_max = non_fixed_reservation_remaining > 1 ? min(env.grid_size - non_fixed_reservation_remaining, x_min + ceil_div(env.required_area[j][k], env.grid_size - width_used_for_fixed_packing)) : env.grid_size;
            reservation_area[j][k] = Rectangle(x_min, width_used_for_fixed_packing, x_max, env.grid_size);
            height_used = x_max;
            non_fixed_reservation_remaining--;
          }
        }
        assert(non_fixed_reservation_remaining == 0);
      }
    }
    Output output;
    output.reservation_area = reservation_area;
    ll score = calculate_score(env, output);
    if (score < best_score) {
      best_score = score;
      best_output = output;
    }
  }

  if (best_score == 1LL << 60) return make_pair(Output(), false);
  return make_pair(best_output, true);
}

Output solve(const Environment &env) {
  Output best_output;
  ll best_score = 1LL << 60;
  {
    auto [output, success] = solve_zero_cost(env);
    if (success) return output;
  }
  for (int bin_size = 1; bin_size <= env.reservation_per_day; bin_size++) {
    auto output = solve_greedy1(env, bin_size);
    ll score = calculate_score(env, output);
    if (score < best_score) {
      best_score = score;
      best_output = output;
    }
  }
  {
    auto [output, success] = solve_greedy2(env);
    if (success) {
      ll score = calculate_score(env, output);
      if (score < best_score) {
        best_score = score;
        best_output = output;
      }
    }
  }
  {
    auto [output, success] = solve_greedy3(env);
    if (success) {
      ll score = calculate_score(env, output);
      if (score < best_score) {
        best_score = score;
        best_output = output;
      }
    }
  }
  return best_output;
}

void print_output(const Output &output) {
  for (int i = 0; i < (int) output.reservation_area.size(); i++) {
    for (int j = 0; j < (int) output.reservation_area[i].size(); j++) {
      cout << output.reservation_area[i][j].x_min << " " << output.reservation_area[i][j].y_min << " " << output.reservation_area[i][j].x_max << " " << output.reservation_area[i][j].y_max << endl;
    }
  }
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  HyperParameter::load_hyper_parameter(argc, argv);

  const auto input = get_input();

  const auto env = create_environment(input);

  const auto output = solve(env);

  print_output(output);

  return 0;
}
