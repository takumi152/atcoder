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
}

// structs
struct QueryRecord {
  vector<int> left_items;
  vector<int> right_items;
  char result;

  QueryRecord(vector<int> &left_items, vector<int> &right_items, char result) : left_items(left_items), right_items(right_items), result(result) {}
};

// enums

// constants
constexpr int max_weight_base = 100000;

// inputs
struct Input {
  int item_num;
  int group_num;
  int query_max;

  int max_weight;
};

// outputs
struct Output {
  vector<int> group_of_items;
};

// internals

Input get_input() {
  Input input;
  cin >> input.item_num >> input.group_num >> input.query_max;
  return input;
}

void init(Input& input) {
  input.max_weight = max_weight_base * input.item_num / input.group_num;
}

QueryRecord query(vector<int> left_items, vector<int> right_items) {
  assert(!left_items.empty());
  assert(!right_items.empty());
  
  cout << left_items.size() << " " << right_items.size() << " ";
  for (auto &item: left_items) cout << item << " ";
  for (auto &item: right_items) cout << item << " ";
  cout << endl;

  char result;
  cin >> result;

  return QueryRecord(left_items, right_items, result);
}

void print_comment(string str) {
  cout << "#" << str << endl;
}

void print_output(const Output& output, bool is_comment) {
  if (is_comment) cout << "#c ";
  for (auto &group: output.group_of_items) {
    cout << group << " ";
  }
  cout << endl;
}

Output solve(const Input& input) {
  vector<QueryRecord> query_records;
  for (int q = 0; q < input.query_max; q++) {
    const double use_chance = 0.001 + theRandom.nextDouble() * 0.999;
    vector<int> left_items;
    vector<int> right_items;
    do {
      left_items.clear();
      right_items.clear();
      for (int i = 0; i < input.item_num; i++) {
        double roll = theRandom.nextDouble();
        if (roll < use_chance * 0.5) {
          left_items.push_back(i);
        }
        else if (roll < use_chance) {
          right_items.push_back(i);
        }
      }
    } while (left_items.empty() || right_items.empty());
    auto query_record = query(left_items, right_items);
    query_records.push_back(move(query_record));
  }

  struct SAState1 {
    const Input& input;

    vector<vector<int>> query_score_factor;

    vector<int> item_weight;

    vector<int> query_score;

    ll difference_score = 0;

    const double difference_weight = 1.0;

    SAState1(const Input& input, const vector<QueryRecord>& query_records): input(input) {
      query_score_factor = vector<vector<int>>(input.item_num, vector<int>(input.query_max, 0));
      for (int q = 0; q < input.query_max; q++) {
        // = が来たら処理が面倒なので無視する（！）
        if (query_records[q].result == '<') {
          for (auto &left: query_records[q].left_items) query_score_factor[left][q] = 1;
          for (auto &right: query_records[q].right_items) query_score_factor[right][q] = -1;
        }
        else if (query_records[q].result == '>') {
          for (auto &left: query_records[q].left_items) query_score_factor[left][q] = -1;
          for (auto &right: query_records[q].right_items) query_score_factor[right][q] = 1;
        }
      }

      item_weight = vector<int>(input.item_num);
      for (int i = 0; i < input.item_num; i++) {
        item_weight[i] = theRandom.nextUIntMod(max_weight_base) + 1;
      }

      query_score = vector<int>(input.query_max);
      
      update_score_full();
    }

    void update_score_full() {
      for (int q = 0; q < input.query_max; q++) {
        query_score[q] = 0;
      }
      difference_score = 0;

      for (int i = 0; i < input.item_num; i++) {
        for (int q = 0; q < input.query_max; q++) {
          query_score[q] += item_weight[i] * query_score_factor[i][q];
        }
      }

      for (int q = 0; q < input.query_max; q++) {
        difference_score += max(0, query_score[q]);
      }
    }

    void update_score_partial(int item_id, bool is_after_update) {
      for (int q = 0; q < input.query_max; q++) {
        query_score[q] += item_weight[item_id] * query_score_factor[item_id][q] * (is_after_update ? 1 : -1);
      }

      if (is_after_update) {
        difference_score = 0;
        for (int q = 0; q < input.query_max; q++) {
          difference_score += max(0, query_score[q]);
        }
      }
    }

    double get_score() const {
      return difference_score * difference_weight;
    }
  };

  SAState1 state1(input, query_records);
  {
    double score = state1.get_score();
    double last_score = score;
    double best_score = score;

    const double base_temperature = 1e9;
    const double target_temperature = 1e-1;
    // const double decay_rate = 4e-5;
    double temperature = base_temperature;

    const int base_range = 10000;
    const int target_range = 100;
    int range = base_range;

    int iter_count = 0;
    double time_start = theTimer.time();
    const double time_limit = 1.800;

    while (theTimer.time() < time_limit) {
      double roll = theRandom.nextDouble();
      if (roll < 1.00) {
        int i = theRandom.nextUIntMod(input.item_num);
        int d = theRandom.nextUIntMod(range * 2 + 1) - range;

        state1.update_score_partial(i, false);

        auto orig_weight = state1.item_weight[i];
        state1.item_weight[i] = clamp(state1.item_weight[i] + d, 1, max_weight_base);

        state1.update_score_partial(i, true);

        score = state1.get_score();

        #ifdef DEBUG
        if (iter_count % 10000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << range << " " << theTimer.time() << endl;
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
          state1.update_score_partial(i, false);
          state1.item_weight[i] = orig_weight;
          state1.update_score_partial(i, true);
          score = last_score;
        }
      }

      temperature = exp(log(base_temperature) - ((log(base_temperature) - log(target_temperature)) * ((theTimer.time() - time_start) * (1.0 / (time_limit - time_start)))));
      range = (int) exp(log(base_range) - ((log(base_range) - log(target_range)) * ((theTimer.time() - time_start) * (1.0 / (time_limit - time_start)))));
      iter_count++;
    }

    cerr << "iter_count       = " << iter_count << endl;
    cerr << "score            = " << score << endl;
    cerr << "difference_score = " << state1.difference_score << endl;
    cerr << "best_score       = " << best_score << endl;
    cerr << "temperature      = " << temperature << endl;

    for (int i = 0; i < input.item_num; i++) {
      print_comment("weight[" + to_string(i) + "] = " + to_string(state1.item_weight[i]));
    }
  }

  struct SAState2 {
    const Input& input;

    vector<int> item_weight;
    int item_weight_sum;

    vector<int> group_id;

    vector<int> group_sum;

    double variance_score = 0;

    const double variance_weight = 1.0;

    SAState2(const Input& input, SAState1& state1): input(input) {
      item_weight = state1.item_weight;
      item_weight_sum = accumulate(item_weight.begin(), item_weight.end(), 0);

      group_id = vector<int>(input.item_num);
      for (int i = 0; i < input.item_num; i++) {
        group_id[i] = theRandom.nextUIntMod(input.group_num);
      }

      group_sum = vector<int>(input.group_num);
      
      update_score_full();
    }

    void update_score_full(bool d = false) {
      for (int g = 0; g < input.group_num; g++) {
        group_sum[g] = 0;
      }
      variance_score = 0.0;

      for (int i = 0; i < input.item_num; i++) {
        group_sum[group_id[i]] += item_weight[i];
      }

      ll sum_of_group_squared = 0;
      for (int g = 0; g < input.group_num; g++) {
        sum_of_group_squared += (ll) group_sum[g] * (ll) group_sum[g];
      }

      variance_score = ((double) sum_of_group_squared - (double) ((ll) item_weight_sum * (ll) item_weight_sum) / (double) input.group_num) / (double) input.group_num;
    }

    double get_score() const {
      return variance_score * variance_weight;
    }
  };

  SAState2 state2(input, state1);
  {
    double score = state2.get_score();
    double last_score = score;
    double best_score = score;

    const double base_temperature = 1e7;
    const double target_temperature = 1e-1;
    // const double decay_rate = 4e-5;
    double temperature = base_temperature;

    int iter_count = 0;
    double time_start = theTimer.time();
    const double time_limit = 1.990;

    while (theTimer.time() < time_limit) {
      double roll = theRandom.nextDouble();
      if (roll < 0.50) {
        int i = theRandom.nextUIntMod(input.item_num);
        int g = theRandom.nextUIntMod(input.group_num);

        auto orig_group = state2.group_id[i];
        state2.group_id[i] = g;

        state2.update_score_full();

        score = state2.get_score();

        #ifdef DEBUG
        if (iter_count % 100000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
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
          state2.group_id[i] = orig_group;
          score = last_score;
        }
      }
      else if (roll < 1.00) {
        int i1 = theRandom.nextUIntMod(input.item_num);
        int i2 = theRandom.nextUIntMod(input.item_num);
        if (state2.group_id[i1] == state2.group_id[i2]) continue;

        swap(state2.group_id[i1], state2.group_id[i2]);

        state2.update_score_full();

        score = state2.get_score();

        #ifdef DEBUG
        if (iter_count % 100000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
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
          swap(state2.group_id[i1], state2.group_id[i2]);
          score = last_score;
        }
      }

      temperature = exp(log(base_temperature) - ((log(base_temperature) - log(target_temperature)) * ((theTimer.time() - time_start) * (1.0 / (time_limit - time_start)))));
      iter_count++;
    }

    cerr << "iter_count     = " << iter_count << endl;
    cerr << "score          = " << score << endl;
    cerr << "variance_score = " << state2.variance_score << endl;
    cerr << "best_score     = " << best_score << endl;
    cerr << "temperature    = " << temperature << endl;
  }
  state2.update_score_full(true);

  Output output;
  output.group_of_items = state2.group_id;
  return output;
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  auto input = get_input();

  init(input);

  auto output = solve(input);

  print_output(output, false);

  return 0;
}
