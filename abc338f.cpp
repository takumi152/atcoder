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

// hyper parameters
namespace HyperParameter {
  void load_hyper_parameter(int argc, char *argv[]) {
    // do nothing
  }
}

// enums

// structs
struct Edge {
  int from, to;
  ll cost;
};

// constants
namespace Consts {
  constexpr ll inf = (ll) 1e12;
  constexpr ll inf_threshold = (ll) 1e9;
};

// inputs
struct Input {
  int vertex_num, edge_num;
  vector<Edge> edges;

  vector<vector<ll>> cost_from_to;
};

// outputs
struct Output {
  ll score;
};

// internals

struct SAState {
  const Input& input;
  vector<int> visit_order;

  ll score;

  SAState(const Input& input): input(input) {
    visit_order = vector<int>(input.vertex_num);
    iota(visit_order.begin(), visit_order.end(), 0);

    update_score_full();
  }

  void update_score_full() {
    score = 0;
    for (int i = 0; i < input.vertex_num - 1; i++) {
      score += input.cost_from_to[visit_order[i]][visit_order[i + 1]];
    }
  }

  ll get_score() {
    return score;
  }
};

Input get_input() {
  Input input;
  cin >> input.vertex_num >> input.edge_num;
  input.edges = vector<Edge>(input.edge_num);
  for (auto &x: input.edges) {
    cin >> x.from >> x.to >> x.cost;
    x.from--;
    x.to--;
  }

  return input;
}

void init(Input& input) {
  input.cost_from_to = vector<vector<ll>>(input.vertex_num, vector<ll>(input.vertex_num, Consts::inf));
  for (auto &x: input.edges) {
    input.cost_from_to[x.from][x.to] = x.cost;
  }
  for (int k = 0; k < input.vertex_num; k++) {
    for (int i = 0; i < input.vertex_num; i++) {
      for (int j = 0; j < input.vertex_num; j++) {
        input.cost_from_to[i][j] = min(input.cost_from_to[i][j], input.cost_from_to[i][k] + input.cost_from_to[k][j]);
      }
    }
  }
}

Output solve(const Input& input) {
  SAState state(input);
  Output output;
  {
    ll score = state.get_score();
    ll last_score = score;
    ll best_score = score;

    const double base_temperature = 1e15;
    const double target_temperature = 1e-3;
    // const double decay_rate = 4e-5;
    double temperature = base_temperature;

    int iter_count = 0;
    double time_start = theTimer.time();
    const double time_limit = 5.900;

    while (true) {
      if (iter_count % 1024 == 0 && theTimer.time() > time_limit) break;
      const double roll = theRandom.nextDouble();
      if (roll < 0.3) {
        const int v1 = theRandom.nextUIntMod(input.vertex_num);
        const int v2 = theRandom.nextUIntMod(input.vertex_num);
        if (v1 == v2) continue;

        swap(state.visit_order[v1], state.visit_order[v2]);

        state.update_score_full();
        score = state.get_score();

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
          swap(state.visit_order[v1], state.visit_order[v2]);
          score = last_score;
        }
      }
      else if (roll < 0.6) {
        const int v1 = theRandom.nextUIntMod(input.vertex_num);
        const int v2 = theRandom.nextUIntMod(input.vertex_num);
        if (v1 == v2) continue;

        const int orig_v = state.visit_order[v1];
        state.visit_order.erase(state.visit_order.begin() + v1);
        state.visit_order.insert(state.visit_order.begin() + v2, orig_v);

        state.update_score_full();
        score = state.get_score();

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
          state.visit_order.erase(state.visit_order.begin() + v2);
          state.visit_order.insert(state.visit_order.begin() + v1, orig_v);
          score = last_score;
        }
      }
      else if (roll < 1.0) {
        const int len = theRandom.nextUIntMod(input.vertex_num);
        const int v1 = theRandom.nextUIntMod(input.vertex_num - len + 1);
        const int v2 = theRandom.nextUIntMod(input.vertex_num - len + 1);
        if (v1 == v2) continue;

        const vector<int> orig_v(state.visit_order.begin() + v1, state.visit_order.begin() + v1 + len);
        state.visit_order.erase(state.visit_order.begin() + v1, state.visit_order.begin() + v1 + len);
        state.visit_order.insert(state.visit_order.begin() + v2, orig_v.begin(), orig_v.end());

        state.update_score_full();
        score = state.get_score();

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
          state.visit_order.erase(state.visit_order.begin() + v2, state.visit_order.begin() + v2 + len);
          state.visit_order.insert(state.visit_order.begin() + v1, orig_v.begin(), orig_v.end());
          score = last_score;
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

    output.score = best_score;
  }

  return output;
}

void print_output(const Output& output) {
  if (output.score > Consts::inf_threshold) cout << "No" << endl;
  else cout << output.score << endl;
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
