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
        return __rdtsc() * 3.333333333333333e-10; // 1 / 3.0e9, for AWS EC2 C5 (Xeon Platinum 8124M / Xeon Platinum 8275CL)
        //return __rdtsc() * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C6i (Xeon Platinum 8375C)
      #endif
    #else
      unsigned long long l, h;
        __asm__ ("rdtsc" : "=a"(l), "=d"(h));
      #ifdef LOCAL
        return (double)(l | h << 32) * 2.857142857142857e-10; // 1 / 3.5e9, for local (Ryzen 9 3950X)
      #else
        //return (double)(l | h << 32) * 3.5714285714285715e-10; // 1 / 2.8e9, for AWS EC2 C3 (Xeon E5-2680 v2)
        //return (double)(l | h << 32) * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C4 (Xeon E5-2666 v3)
        return (double)(l | h << 32) * 3.333333333333333e-10; // 1 / 3.0e9, for AWS EC2 C5 (Xeon Platinum 8124M / Xeon Platinum 8275CL)
        //return (double)(l | h << 32) * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C6i (Xeon Platinum 8375C)
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
mt19937 theMersenne(1);

// hyper parameters

// structs

// enums

// constants
constexpr int contest_type_num = 26;
constexpr int turn_limit = 365;

// inputs

// outputs

// internals

// --------------------------------
// Beam search library
// 参考: https://qiita.com/rhoo/items/2f647e32f6ff2c6ee056

class NotImplementedException : public std::logic_error
{
public:
    NotImplementedException() : std::logic_error("Function not yet implemented") { };
};

// 【問題固有】入力や前計算を管理する構造体
struct Input {
  vector<int> satisfaction_loss_factor;
  vector<vector<int>> satisfaction_gain;
  vector<vector<ull>> zobrist;
  
  Input() {
    int _turn_limit;
    cin >> _turn_limit;
    satisfaction_loss_factor = vector<int>(contest_type_num);
    for (auto &x: satisfaction_loss_factor) cin >> x;
    satisfaction_gain = vector<vector<int>>(turn_limit, vector<int>(contest_type_num));
    for (auto &x: satisfaction_gain) {
      for (auto &y: x) cin >> y;
    }

    zobrist = vector<vector<ull>>(turn_limit + 1, vector<ull>(contest_type_num));
    for (int t = 0; t < turn_limit + 1; t++) {
      for (int c = 0; c < contest_type_num; c++) {
        zobrist[t][c] = theRandom.nextULL();
      }
    }
  }
};

struct Cand {
  uchar op; // Nodeからの操作
  ll parent; // 親のindex
  int eval_score; // この大小で上位Mが決定される
  ull hash; // 重複除去に用いられる

  Cand(uchar op, ll parent, int eval_score, ull hash) : op(op), parent(parent), eval_score(eval_score), hash(hash) {}

  // 【問題固有】問題の生スコア、これを最終的に最大化したい
  int raw_score(const Input &input) {
    return this->eval_score;
  }
};

// 確定した状態を管理する構造体
struct Node {
  ll track_id; // 履歴のID

  // 【問題固有】状態をここに持たせる
  int score;
  ull hash;
  vector<int> last_selected;

  Node() {
    track_id = -1;
    score = 0;
    hash = 0;
    last_selected = vector<int>(contest_type_num);
  }
  Node(int score, ull hash, vector<int> &&last_selected) : track_id(-1), score(score), hash(hash), last_selected(last_selected) {}

  // 【問題固有】thisからcandの遷移をしたときの新たなNodeを返す
  Node new_node(const Input &input, const int turn, const Cand &cand) {
    auto last_selected = this->last_selected;
    last_selected[cand.op] = turn;
    return Node(cand.eval_score, cand.hash, move(last_selected));
  }
};

struct BeamSearch {
  const int MAX_WIDTH = 1000; // 【問題固有】ビーム幅
  const int TURN = turn_limit; // 【問題固有】探索を行うターン数

  vector<pair<ll, uchar>> track; // 履歴を管理
  vector<Node> nodes;
  vector<Node> next_nodes;

  BeamSearch(const Node node) {
    track = vector<pair<ll, uchar>>();
    nodes = vector<Node>{node};
    next_nodes = vector<Node>();
  }

  // 【問題固有】self.nodes[idx]からの候補をcandsに積む
  void append_cands(const Input &input, const int turn, const ll idx, vector<Cand> &cands) {
    auto node = this->nodes[idx];

    int base_score_loss = 0;
    for (int c = 0; c < contest_type_num; c++) {
      base_score_loss += input.satisfaction_loss_factor[c] * (turn - node.last_selected[c] + 1);
    }
    
    for (int op = 0; op < contest_type_num; op++) {
      int score = node.score;

      score += input.satisfaction_gain[turn][op];
      score -= base_score_loss;
      score += input.satisfaction_loss_factor[op] * (turn - node.last_selected[op] + 1);

      int hash = node.hash;
      hash ^= input.zobrist[node.last_selected[op]][op];
      hash ^= input.zobrist[turn + 1][op];

      cands.push_back(Cand(op, idx, score, hash));
    }
  }

  // 候補の列挙
  void enum_cands(const Input &input, const int turn, vector<Cand> &cands) {
    for (int i = 0; i < (int) this->nodes.size(); i++) {
      this->append_cands(input, turn, i, cands);
    }
  }

  // 候補の採用を反映する
  void update(const Input &input, const int turn, const vector<Cand> &cands) {
    this->next_nodes.clear();
    for (auto &cand: cands) {
      auto node = this->nodes[cand.parent];
      auto new_node = node.new_node(input, turn, cand);
      this->track.push_back(make_pair(node.track_id, cand.op));
      new_node.track_id = (ll) this->track.size() - 1;
      this->next_nodes.push_back(new_node);
    }

    swap(this->nodes, this->next_nodes);
  }

  // 復元
  vector<uchar> restore(ll idx) {
    idx = this->nodes[idx].track_id;
    vector<uchar> ret;
    while (idx != -1) {
      ret.push_back(this->track[idx].second);
      idx = this->track[idx].first;
    }
    reverse(ret.begin(), ret.end());
    return ret;
  }

  // ビームサーチを実行する
  vector<uchar> solve(Input &input) {
    vector<Cand> cands;
    unordered_set<ull> set;
    for (int t = 0; t < TURN; t++) {
      if (t != 0) {
        sort(cands.rbegin(), cands.rend(), [](auto &a, auto &b){return a.eval_score < b.eval_score;});
        set.clear();
        // 重複除去をして上位M個を取り出す
        vector<Cand> cands_to_update;
        for (auto &cand: cands) {
          auto [_, success] = set.insert(cand.hash);
          if (success) {
            cands_to_update.push_back(cand);
            if ((int) cands_to_update.size() >= this->MAX_WIDTH) {
              break;
            }
          }
        }
        this->update(input, t, cands_to_update);
      }
      cands.clear();
      this->enum_cands(input, t, cands);
      assert(!cands.empty() && "次の合法手がありません");
    }

    // 一番いい状態を取り出す
    auto best = max_element(cands.begin(), cands.end(), [&input](auto &a, auto &b){return a.raw_score(input) < b.raw_score(input);});
    cerr << "score = " << best->raw_score(input) << endl;

    auto ret = this->restore(best->parent);
    ret.push_back(best->op);

    return ret;
  }
};

// --------------------------------

vector<uchar> solve() {
  Input input;
  Node node;
  BeamSearch solver(node);
  return solver.solve(input);
}

void output_ans(vector<uchar> ans) {
  for (auto &x: ans) cout << (int) x + 1 << endl;
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  auto ans = solve();
  output_ans(ans);

  return 0;
}
