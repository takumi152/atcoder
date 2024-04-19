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

// libs
namespace Lib {
}

// hyper parameters
namespace HyperParameter {
  double base_temperature = 3.1102252027437776;
  double target_temperature = 1.9666082179905622;

  void load_hyper_parameter(int argc, char *argv[]) {
    if (argc > 1) base_temperature = strtod(argv[1], nullptr);
    if (argc > 2) target_temperature = strtod(argv[2], nullptr);
  }
}

// structs

// enums

// constants

// inputs
struct Input {
  int box_num;
  int stack_num;

  vector<vector<int>> initial_stack;
};

// outputs
struct Output {
  vector<Pii> actions;
};

// internals

Input get_input() {
  Input input;

  cin >> input.box_num >> input.stack_num;
  input.initial_stack = vector<vector<int>>(input.stack_num, vector<int>(input.box_num / input.stack_num));
  for (auto &st: input.initial_stack) {
    for (auto &b: st) cin >> b;
  }

  return input;
}

void init(Input& input) {
}

pair<Output, int> solve_greedy(const Input& input, const vector<int>& stack_order, const int bulk_move_difference_limit);

Output solve(const Input& input) {
  vector<int> stack_order(input.stack_num);
  for (int i = 0; i < input.stack_num; i++) stack_order[i] = i;
  int bulk_move_difference_limit = 100;

  auto [output, first_score] = solve_greedy(input, stack_order, bulk_move_difference_limit);
  Output best_output = output;
  {
    int last_score = first_score;
    int best_score = first_score;

    const double base_temperature = HyperParameter::base_temperature;
    const double target_temperature = HyperParameter::target_temperature;
    // const double decay_rate = 4e-5;
    double temperature = base_temperature;

    int iter_count = 0;
    double time_start = theTimer.time();
    const double time_limit = 1.990;

    while (theTimer.time() < time_limit) {
      double roll = theRandom.nextDouble();
      if (roll < 0.40) {
        int i1 = theRandom.nextUIntMod(input.stack_num);
        int i2 = theRandom.nextUIntMod(input.stack_num);
        if (i1 == i2) continue;

        swap(stack_order[i1], stack_order[i2]);

        auto [output, score] = solve_greedy(input, stack_order, bulk_move_difference_limit);

        #ifdef DEBUG
        if (iter_count % 1000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
            best_output = output;
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          swap(stack_order[i1], stack_order[i2]);
        }
      }
      else if (roll < 0.80) {
        int i1 = theRandom.nextUIntMod(input.stack_num);
        int i2 = theRandom.nextUIntMod(input.stack_num);
        if (i1 == i2) continue;

        auto item_to_insert = stack_order[i1];
        stack_order.erase(stack_order.begin() + i1);
        stack_order.insert(stack_order.begin() + i2, item_to_insert);

        auto [output, score] = solve_greedy(input, stack_order, bulk_move_difference_limit);

        #ifdef DEBUG
        if (iter_count % 1000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
            best_output = output;
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          stack_order.erase(stack_order.begin() + i2);
          stack_order.insert(stack_order.begin() + i1, item_to_insert);
        }
      }
      else if (roll < 1.00) {
        int b = theRandom.nextUIntMod(input.box_num + 1);

        auto orig_bulk_move_difference_limit = bulk_move_difference_limit;
        bulk_move_difference_limit = b;

        auto [output, score] = solve_greedy(input, stack_order, bulk_move_difference_limit);

        #ifdef DEBUG
        if (iter_count % 1000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
            best_output = output;
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          bulk_move_difference_limit = orig_bulk_move_difference_limit;
        }
      }
      else if (roll < 1.00) {
        vector<int> shuffled_stack_order = stack_order;
        shuffle(stack_order.begin(), stack_order.end(), theMersenne);
        int b = theRandom.nextUIntMod(input.box_num + 1);

        swap(stack_order, shuffled_stack_order);
        auto orig_bulk_move_difference_limit = bulk_move_difference_limit;
        bulk_move_difference_limit = b;

        auto [output, score] = solve_greedy(input, stack_order, bulk_move_difference_limit);

        #ifdef DEBUG
        if (iter_count % 1000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
            best_output = output;
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          swap(stack_order, shuffled_stack_order);
          bulk_move_difference_limit = orig_bulk_move_difference_limit;
        }
      }

      // temperature *= 1.0 - decay_rate;
      temperature = exp(log(base_temperature) - ((log(base_temperature) - log(target_temperature)) * ((theTimer.time() - time_start) * (1.0 / (time_limit - time_start)))));
      iter_count++;
    }

    cerr << "iter_count  = " << iter_count << endl;
    cerr << "last_score  = " << last_score << endl;
    cerr << "best_score  = " << best_score << endl;
    cerr << "temperature = " << temperature << endl;
  }

  return best_output;
}

pair<Output, int> solve_greedy(const Input& input, const vector<int>& stack_order, const int bulk_move_difference_limit) {
  int score = 10000;
  int collected_box_num = 0;
  Output output;
  vector<vector<int>> current_stack = input.initial_stack;
  vector<int> locked_stack_height(input.stack_num, 0);
  vector<bool> is_sorted(input.stack_num, false);

  for (const auto &sorted_stack_id: stack_order) {
    while (!current_stack[sorted_stack_id].empty()) {
      auto next_box_id = current_stack[sorted_stack_id].back();
      if (next_box_id == collected_box_num + 1) {
        // 回収できる場合は回収する
        output.actions.emplace_back(next_box_id, -1);
        current_stack[sorted_stack_id].pop_back();
        collected_box_num++;
        while (true) {
          bool collected_next_box = false;
          for (int s = 0; s < input.stack_num; s++) {
            if (!current_stack[s].empty() && current_stack[s].back() == collected_box_num + 1) {
              output.actions.emplace_back(current_stack[s].back(), -1);
              current_stack[s].pop_back();
              collected_box_num++;
              collected_next_box = true;
              if (locked_stack_height[s] > 0) locked_stack_height[s]--;
            }
          }
          if (!collected_next_box) break;
        }
        continue;
      }
      if (current_stack[sorted_stack_id].size() == 1) {
        int other_largest_top_box_num = 0;
        for (int s = 0; s < input.stack_num; s++) {
          if (s != sorted_stack_id && !current_stack[s].empty() && current_stack[s].back() > other_largest_top_box_num) {
            other_largest_top_box_num = current_stack[s].back();
          }
        }
        if (next_box_id > other_largest_top_box_num) {
          // 動かしてもソート時にその箱が一番下になって無意味なので、そのままにしておく
          break;
        }
      }
      // ソート済みの山に小さい箱を置いてソート済みの状態が維持できるなら、そちらに動かす
      {
        int best_box_id = 999999;
        int best_stack_id = -1;
        for (int s = 0; s < input.stack_num; s++) {
          if (s != sorted_stack_id && is_sorted[s] && (int) current_stack[s].size() == locked_stack_height[s] && next_box_id < current_stack[s].back() && current_stack[s].back() < best_box_id) {
            best_box_id = current_stack[s].back();
            best_stack_id = s;
            break;
          }
        }
        if (best_stack_id != -1) {
            output.actions.emplace_back(next_box_id, best_stack_id);
            current_stack[best_stack_id].push_back(next_box_id);
            current_stack[sorted_stack_id].pop_back();
            locked_stack_height[best_stack_id]++;
            score -= 2;
            continue;
        }
      }
      int bulk_move_limit = 0;
      for (int s = 0; s < input.stack_num; s++) {
        if (s != sorted_stack_id && is_sorted[s] && (int) current_stack[s].size() == locked_stack_height[s] && bulk_move_limit < current_stack[s].back()) {
          bulk_move_limit = current_stack[s].back();
        }
      }
      int bulk_move_num = 1;
      for (int h = (int) current_stack[sorted_stack_id].size() - 2; h >= 0; h--) {
        if (current_stack[sorted_stack_id][h] < next_box_id && next_box_id - current_stack[sorted_stack_id][h] <= bulk_move_difference_limit && current_stack[sorted_stack_id][h] > bulk_move_limit) {
          // 1つ下の箱との番号の差が小さいならまとめて動かす
          bulk_move_num++;
          next_box_id = current_stack[sorted_stack_id][h];
        }
        else break;
      }
      bool smaller_box_exist = false;
      for (int s = 0; s < input.stack_num; s++) {
        if (s != sorted_stack_id && !current_stack[s].empty() && current_stack[s].back() < next_box_id) smaller_box_exist = true;
      }
      if (smaller_box_exist) {
        int best_box_id = 0;
        int best_stack_id = 0;
        for (int s = 0; s < input.stack_num; s++) {
          // できるだけ差が小さくなるように置く
          if (s != sorted_stack_id && !current_stack[s].empty() && current_stack[s].back() < next_box_id && current_stack[s].back() > best_box_id) {
            best_box_id = current_stack[s].back();
            best_stack_id = s;
          }
        }
        output.actions.emplace_back(next_box_id, best_stack_id);
        current_stack[best_stack_id].insert(current_stack[best_stack_id].end(), current_stack[sorted_stack_id].begin() + (int) current_stack[sorted_stack_id].size() - bulk_move_num, current_stack[sorted_stack_id].end());
        current_stack[sorted_stack_id].erase(current_stack[sorted_stack_id].begin() + (int) current_stack[sorted_stack_id].size() - bulk_move_num, current_stack[sorted_stack_id].end());
        score -= bulk_move_num + 1;
      }
      else {
        int best_box_id = 0;
        int best_stack_id = 0;
        for (int s = 0; s < input.stack_num; s++) {
          // できるだけ大きい箱の上に置く
          if (s != sorted_stack_id && !current_stack[s].empty() && current_stack[s].back() > best_box_id) {
            best_box_id = current_stack[s].back();
            best_stack_id = s;
          }
        }
        output.actions.emplace_back(next_box_id, best_stack_id);
        current_stack[best_stack_id].insert(current_stack[best_stack_id].end(), current_stack[sorted_stack_id].begin() + (int) current_stack[sorted_stack_id].size() - bulk_move_num, current_stack[sorted_stack_id].end());
        current_stack[sorted_stack_id].erase(current_stack[sorted_stack_id].begin() + (int) current_stack[sorted_stack_id].size() - bulk_move_num, current_stack[sorted_stack_id].end());
        score -= bulk_move_num + 1;
      }
    }

    while (true) {
      auto next_box_id = current_stack[sorted_stack_id].empty() ? input.box_num + 1 : current_stack[sorted_stack_id].back();
      bool smaller_box_exist = false;
      int best_box_id = 0;
      int best_stack_id = 0;
      for (int s = 0; s < input.stack_num; s++) {
        if (s != sorted_stack_id && !current_stack[s].empty() && current_stack[s].back() < next_box_id && current_stack[s].back() > best_box_id && (int) current_stack[s].size() > locked_stack_height[s]) {
          smaller_box_exist = true;
          best_box_id = current_stack[s].back();
          best_stack_id = s;
        }
      }
      if (!smaller_box_exist) break;
      // 動かそうとしている山の一番上の箱 < 動かそうとしている山の2番目に上の箱 < ... < 動かそうとしている山のx番目に上の箱 < ソート中の山の一番上の箱 なら、後のソート操作に影響が出ないのでx番目までまとめて動かす
      int bulk_move_num = 1;
      for (int h = (int) current_stack[best_stack_id].size() - 2; h >= 0; h--) {
        if (best_box_id < current_stack[best_stack_id][h] && current_stack[best_stack_id][h] < next_box_id) {
          bulk_move_num++;
          best_box_id = current_stack[best_stack_id][h];
        }
        else break;
      }
      output.actions.emplace_back(best_box_id, sorted_stack_id);
      current_stack[sorted_stack_id].insert(current_stack[sorted_stack_id].end(), current_stack[best_stack_id].begin() + (int) current_stack[best_stack_id].size() - bulk_move_num, current_stack[best_stack_id].end());
      current_stack[best_stack_id].erase(current_stack[best_stack_id].begin() + (int) current_stack[best_stack_id].size() - bulk_move_num, current_stack[best_stack_id].end());
      score -= bulk_move_num + 1;
      while (true) {
        bool collected_next_box = false;
        for (int s = 0; s < input.stack_num; s++) {
          if (!current_stack[s].empty() && current_stack[s].back() == collected_box_num + 1) {
            output.actions.emplace_back(current_stack[s].back(), -1);
            current_stack[s].pop_back();
            collected_box_num++;
            collected_next_box = true;
            if (locked_stack_height[s] > 0) locked_stack_height[s]--;
          }
        }
        if (!collected_next_box) break;
      }
    }

    locked_stack_height[sorted_stack_id] = current_stack[sorted_stack_id].size();
    is_sorted[sorted_stack_id] = true;

    while (true) {
      bool collected_next_box = false;
      for (int s = 0; s < input.stack_num; s++) {
        if (!current_stack[s].empty() && current_stack[s].back() == collected_box_num + 1) {
          output.actions.emplace_back(current_stack[s].back(), -1);
          current_stack[s].pop_back();
          collected_box_num++;
          collected_next_box = true;
          if (locked_stack_height[s] > 0) locked_stack_height[s]--;
        }
      }
      if (!collected_next_box) break;
    }
  }

  // 箱を回収する
  for (int b = 1; b <= input.box_num; b++) {
    bool box_exist = false;
    int box_stack_id = 0;
    int box_stack_height = 0;
    int box_stack_depth = 0;
    for (int s = 0; s < input.stack_num; s++) {
      if (current_stack[s][current_stack[s].size() - 1] == b) {
          box_exist = true;
          box_stack_id = s;
          box_stack_height = (int) current_stack[s].size() - 1;
          box_stack_depth = 0;
        }
    }
    if (!box_exist) {
      for (int s = 0; s < input.stack_num; s++) {
        for (int h = 0; h < (int) current_stack[s].size(); h++) {
          if (current_stack[s][h] == b) {
            box_exist = true;
            box_stack_id = s;
            box_stack_height = h;
            box_stack_depth = (int) current_stack[s].size() - (h + 1);
          }
        }
      }
    }
    if (box_exist) {
      if (box_stack_depth == 0) {
        output.actions.emplace_back(b, -1);
      }
      else {
        output.actions.emplace_back(current_stack[box_stack_id][box_stack_height + 1], (box_stack_id + 1) % input.stack_num);
        output.actions.emplace_back(b, -1);
        output.actions.emplace_back(current_stack[box_stack_id][box_stack_height + 1], box_stack_id);
        score -= (box_stack_depth + 1) * 2;
      }
      assert(*(current_stack[box_stack_id].begin() + box_stack_height) == b);
      current_stack[box_stack_id].erase(current_stack[box_stack_id].begin() + box_stack_height);
    }
  }

  return make_pair(output, score);
}

void print_output(const Output& output) {
  for (auto &[b, s]: output.actions) cout << b << " " << s + 1 << endl;
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  HyperParameter::load_hyper_parameter(argc, argv);

  auto input = get_input();

  init(input);

  auto output = solve(input);

  print_output(output);

  return 0;
}
