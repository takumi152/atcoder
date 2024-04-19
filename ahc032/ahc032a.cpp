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

#include <atcoder/all>

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

const ll mod = 998244353;

timer theTimer;
xorshift64 theRandom;
mt19937_64 theMersenne(1);

// libraries
namespace Lib {
}

// hyper parameters
namespace HyperParameter {
  int beam_extra_action_num = 3;
  ll beam_extra_action_cost_factor = 32237269;
  void load_hyper_parameter(int argc, char *argv[]) {
    if (argc > 1) beam_extra_action_num = atoi(argv[1]);
    if (argc > 2) beam_extra_action_cost_factor = atoll(argv[2]);
  }
}

// enums

// structs
struct Action {
  int stamp_id;
  int x;
  int y;

  Action(): stamp_id(0), x(0), y(0) {}
  Action(int stamp_id, int x, int y): stamp_id(stamp_id), x(x), y(y) {}
};

// constants
struct Consts {
  static constexpr int board_size = 9;
  static constexpr int stamp_num = 20;
  static constexpr int max_action = 81;

  static constexpr int stamp_size = 3;
};

// inputs
struct Input {
  int board_size;
  int stamp_num;
  int max_action;

  vector<vector<int>> initial_board;
  vector<vector<vector<int>>> stamps;
};

// outputs
struct Output {
  vector<Action> actions;
};

// internals
struct Environment {
  int board_size;
  int stamp_num;
  int max_action;

  vector<vector<int>> initial_board;
  vector<vector<vector<int>>> stamps;

  vector<vector<vector<int>>> combined_stamps;
  vector<vector<int>> combined_stamp_ids;
};

Input get_input() {
  Input input;

  cin >> input.board_size >> input.stamp_num >> input.max_action;
  input.initial_board = vector<vector<int>>(input.board_size, vector<int>(input.board_size));
  for (int i = 0; i < input.board_size; i++) {
    for (int j = 0; j < input.board_size; j++) {
      cin >> input.initial_board[i][j];
    }
  }
  input.stamps = vector<vector<vector<int>>>(input.stamp_num, vector<vector<int>>(Consts::stamp_size, vector<int>(Consts::stamp_size)));
  for (int i = 0; i < input.stamp_num; i++) {
    for (int j = 0; j < Consts::stamp_size; j++) {
      for (int k = 0; k < Consts::stamp_size; k++) {
        cin >> input.stamps[i][j][k];
      }
    }
  }

  return input;
}

Environment create_environment(const Input &input) {
  Environment env;

  env.board_size = input.board_size;
  env.stamp_num = input.stamp_num;
  env.max_action = input.max_action;
  env.initial_board = input.initial_board;
  env.stamps = input.stamps;

  vector<vector<int>> combined_stamp(Consts::stamp_size, vector<int>(Consts::stamp_size));
  auto combine_stamp = [&](const int stamp_id, const bool is_add) {
    for (int i = 0; i < Consts::stamp_size; i++) {
      for (int j = 0; j < Consts::stamp_size; j++) {
        combined_stamp[i][j] = (combined_stamp[i][j] + (is_add ? env.stamps[stamp_id][i][j] : -env.stamps[stamp_id][i][j] + mod)) % mod;
      }
    }
  };
  env.combined_stamps.push_back(combined_stamp);
  env.combined_stamp_ids.push_back({});
  for (int i = 0; i < env.stamp_num; i++) {
    combine_stamp(i, true);
    env.combined_stamps.push_back(combined_stamp);
    env.combined_stamp_ids.push_back({i});
    for (int j = i; j < env.stamp_num; j++) {
      combine_stamp(j, true);
      env.combined_stamps.push_back(combined_stamp);
      env.combined_stamp_ids.push_back({i, j});
      for (int k = j; k < env.stamp_num; k++) {
        combine_stamp(k, true);
        env.combined_stamps.push_back(combined_stamp);
        env.combined_stamp_ids.push_back({i, j, k});
        // for (int l = k; l < env.stamp_num; l++) {
        //   combine_stamp(l, true);
        //   env.combined_stamps.push_back(combined_stamp);
        //   env.combined_stamp_ids.push_back({i, j, k, l});
        //   combine_stamp(l, false);
        // }
        combine_stamp(k, false);
      }
      combine_stamp(j, false);
    }
    combine_stamp(i, false);
  }

  return env;
}

Output solve_sa(const Environment &env) {
  struct SAState {
    const Environment &env;

    vector<Action> actions;
    int initial_solution_size;

    vector<vector<int>> board;

    ll score;

    SAState(const Environment &env): env(env) {
      actions = vector<Action>();
      initial_solution_size = 0;
      board = env.initial_board;
      score = 0;

      create_initial_solution();

      update_score_full();
    }

    void create_initial_solution() {
      for (int i = 0; i < env.board_size - Consts::stamp_size; i++) {
        for (int j = 0; j < env.board_size - Consts::stamp_size; j++) {
          int best_stamp_id = 0;
          int best_upper_left_value = 0;
          for (int k = 0; k < env.stamp_num; k++) {
            int upper_left_value = (board[i][j] + env.stamps[k][0][0]) % mod;
            if (upper_left_value > best_upper_left_value) {
              best_stamp_id = k;
              best_upper_left_value = upper_left_value;
            }
          }
          Action new_action(best_stamp_id, i, j);
          actions.push_back(new_action);
          update_score_partial(new_action, true);
        }
      }
      initial_solution_size = actions.size();
    }

    void update_score_full() {
      for (int i = 0; i < env.board_size; i++) {
        for (int j = 0; j < env.board_size; j++) {
          board[i][j] = env.initial_board[i][j];
        }
      }

      for (const auto &action: actions) {
        update_board(action, true);
      }

      score = 0;
      for (int i = 0; i < env.board_size; i++) {
        for (int j = 0; j < env.board_size; j++) {
          score += board[i][j];
        }
      }
    }

    void update_score_partial(const Action &action, const bool is_add) {
      for (int i = 0; i < Consts::stamp_size; i++) {
        for (int j = 0; j < Consts::stamp_size; j++) {
          score -= board[action.x + i][action.y + j];
        }
      }
      update_board(action, is_add);
      for (int i = 0; i < Consts::stamp_size; i++) {
        for (int j = 0; j < Consts::stamp_size; j++) {
          score += board[action.x + i][action.y + j];
        }
      }
    }

    void update_board(const Action &action, const bool is_add) {
      for (int i = 0; i < Consts::stamp_size; i++) {
        for (int j = 0; j < Consts::stamp_size; j++) {
          board[action.x + i][action.y + j] = (board[action.x + i][action.y + j] + (is_add ? env.stamps[action.stamp_id][i][j] : -env.stamps[action.stamp_id][i][j] + mod)) % mod;
        }
      }
    }

    ll get_score() {
      return score;
    }

    Output get_output() {
      Output output;
      output.actions = actions;
      return output;
    }
  };

  SAState state(env);
  {
    ll score = state.get_score();
    ll last_score = score;
    ll best_score = score;

    const double base_temperature = 1e9;
    const double target_temperature = 5e7;
    // const double decay_rate = 4e-5;
    double temperature = base_temperature;

    int iter_count = 0;
    double time_start = theTimer.time();
    const double time_limit = 1.970;

    while (true) {
      if (iter_count % 1024 == 0 && theTimer.time() > time_limit) break;
      const double roll = theRandom.nextDouble();
      if (roll < 0.3) {
        if ((int) state.actions.size() >= env.max_action) continue;
        const int i = theRandom.nextUIntMod(env.stamp_num);
        const int x = theRandom.nextUIntMod(env.board_size - Consts::stamp_size + 1);
        const int y = theRandom.nextUIntMod(env.board_size - Consts::stamp_size + 1);
        if (!(x == env.board_size - Consts::stamp_size || y == env.board_size - Consts::stamp_size)) continue;

        Action new_action(i, x, y);
        state.actions.push_back(new_action);

        state.update_score_partial(new_action, true);

        score = state.get_score();

        #ifdef DEBUG
        if (iter_count % 100000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
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
          state.update_score_partial(new_action, false);
          state.actions.pop_back();

          // state.update_score_full();
          // assert(abs(last_score - state.get_score()) == 0);
          score = last_score;
        }
      }
      else if (roll < 0.6) {
        if (state.actions.size() == 0) continue;
        const int i = theRandom.nextUIntMod(state.actions.size());
        if (i < state.initial_solution_size) continue;

        Action action_to_remove = state.actions[i];
        swap(state.actions[i], state.actions.back());
        state.actions.pop_back();

        state.update_score_partial(action_to_remove, false);

        score = state.get_score();

        #ifdef DEBUG
        if (iter_count % 100000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
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
          state.update_score_partial(action_to_remove, true);
          state.actions.push_back(action_to_remove);

          // state.update_score_full();
          // assert(abs(last_score - state.get_score()) == 0);
          score = last_score;
        }
      }
      else if (roll < 1.0) {
        if (state.actions.size() == 0) continue;
        const int ia = theRandom.nextUIntMod(state.actions.size());
        if (ia < state.initial_solution_size) continue;
        const int is = theRandom.nextUIntMod(env.stamp_num);
        const int x = theRandom.nextUIntMod(env.board_size - Consts::stamp_size + 1);
        const int y = theRandom.nextUIntMod(env.board_size - Consts::stamp_size + 1);
        if (!(x == env.board_size - Consts::stamp_size || y == env.board_size - Consts::stamp_size)) continue;

        Action action_to_change = state.actions[ia];
        Action new_action(is, x, y);
        state.actions[ia] = new_action;
        
        state.update_score_partial(action_to_change, false);
        state.update_score_partial(new_action, true);

        score = state.get_score();

        #ifdef DEBUG
        if (iter_count % 100000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
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
          state.update_score_partial(new_action, false);
          state.update_score_partial(action_to_change, true);
          state.actions[ia] = action_to_change;

          // state.update_score_full();
          // assert(abs(last_score - state.get_score()) == 0);
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
  }

  Output output = state.get_output();
  return output;
}

// source: https://eijirou-kyopro.hatenablog.com/entry/2024/02/01/115639
namespace beam_search {

// メモリの再利用を行いつつ集合を管理するクラス
template<class T>
class ObjectPool {
    public:
        // 配列と同じようにアクセスできる
        T& operator[](int i) {
            return data_[i];
        }

        // 配列の長さを変更せずにメモリを確保する
        void reserve(size_t capacity) {
            data_.reserve(capacity);
        }

        // 要素を追加し、追加されたインデックスを返す
        int push(const T& x) {
            if (garbage_.empty()) {
                data_.push_back(x);
                return data_.size() - 1;
            } else {
                int i = garbage_.top();
                garbage_.pop();
                data_[i] = x;
                return i;
            }
        }

        // 要素を（見かけ上）削除する
        void pop(int i) {
            garbage_.push(i);
        }

        // 使用した最大のインデックス(+1)を得る
        // この値より少し大きい値をreserveすることでメモリの再割り当てがなくなる
        size_t size() {
            return data_.size();
        }

    private:
        vector<T> data_;
        stack<int> garbage_;
};

// 連想配列
// Keyにハッシュ関数を適用しない
// open addressing with linear probing
// unordered_mapよりも速い
// nは格納する要素数よりも4~16倍ほど大きくする
template <class Key, class T>
struct HashMap {
    public:
        explicit HashMap(uint32_t n) {
            n_ = n;
            valid_.resize(n_, false);
            data_.resize(n_);
        }

        // 戻り値
        // - 存在するならtrue、存在しないならfalse
        // - index
        pair<bool,int> get_index(Key key) const {
            Key i = key % n_;
            while (valid_[i]) {
                if (data_[i].first == key) {
                    return {true, i};
                }
                if (++i == n_) {
                    i = 0;
                }
            }
            return {false, i};
        }

        // 指定したindexにkeyとvalueを格納する
        void set(int i, Key key, T value) {
            valid_[i] = true;
            data_[i] = {key, value};
        }

        // 指定したindexのvalueを返す
        T get(int i) const {
            assert(valid_[i]);
            return data_[i].second;
        }

        void clear() {
            fill(valid_.begin(), valid_.end(), false);
        }

    private:
        uint32_t n_;
        vector<bool> valid_;
        vector<pair<Key,T>> data_;
};

using Hash = uint32_t; // TODO

vector<Hash> hash_table_stamp_id;
vector<Hash> hash_table_stamp_x;
vector<Hash> hash_table_stamp_y;

inline void init_hash(const Environment &env) {
  hash_table_stamp_id = vector<Hash>(env.combined_stamp_ids.size());
  hash_table_stamp_x = vector<Hash>(Consts::board_size - Consts::stamp_size + 1);
  hash_table_stamp_y = vector<Hash>(Consts::board_size - Consts::stamp_size + 1);
  for (int i = 0; i < (int) env.combined_stamp_ids.size(); i++) {
    hash_table_stamp_id[i] = theRandom.nextUInt();
  }
  for (int i = 0; i < Consts::board_size - Consts::stamp_size + 1; i++) {
    hash_table_stamp_x[i] = theRandom.nextUInt();
    hash_table_stamp_y[i] = theRandom.nextUInt();
  }
}

inline Hash get_hash(const char stamp_id, const char stamp_x, const char stamp_y) {
  return hash_table_stamp_id[stamp_id] ^ hash_table_stamp_x[stamp_x] ^ hash_table_stamp_y[stamp_y];
}

// 状態遷移を行うために必要な情報
// メモリ使用量をできるだけ小さくしてください
struct Action {
    int combined_stamp_id;
    char stamp_x;
    char stamp_y;

    Action(const int combined_stamp_id, const char stamp_x, const char stamp_y) :
        combined_stamp_id(combined_stamp_id),
        stamp_x(stamp_x),
        stamp_y(stamp_y) {}
};

using Cost = ll;

// 状態のコストを評価するための構造体
// メモリ使用量をできるだけ小さくしてください
struct Evaluator {
    Cost score;
    char extra_action_remaining;

    Evaluator(const Cost score, const char extra_action_remaining) : score(score), extra_action_remaining(extra_action_remaining) {}

    // 低いほどよい
    Cost evaluate() const {
      return -score + ((Cost) -extra_action_remaining) * HyperParameter::beam_extra_action_cost_factor;
    }
};

// 展開するノードの候補を表す構造体
struct Candidate {
    Action action;
    Evaluator evaluator;
    Hash hash;
    int parent;
    Cost cost;

    Candidate(Action action, Evaluator evaluator, Hash hash, int parent, Cost cost) :
        action(action),
        evaluator(evaluator),
        hash(hash),
        parent(parent),
        cost(cost) {}
};

// ビームサーチの設定
struct Config {
    int max_turn;
    size_t beam_width;
    size_t nodes_capacity;
    uint32_t hash_map_capacity;
};

// 削除可能な優先度付きキュー
using MaxSegtree = atcoder::segtree<
    pair<Cost,int>,
    [](pair<Cost,int> a, pair<Cost,int> b){
        if (a.first >= b.first) {
            return a;
        } else {
            return b;
        }
    },
    []() { return make_pair(numeric_limits<Cost>::min(), -1); }
>;

// ノードの候補から実際に追加するものを選ぶクラス
// ビーム幅の個数だけ、評価がよいものを選ぶ
// ハッシュ値が一致したものについては、評価がよいほうのみを残す
class Selector {
    public:
        explicit Selector(const Config& config) :
            hash_to_index_(config.hash_map_capacity)
        {
            beam_width = config.beam_width;
            candidates_.reserve(beam_width);
            full_ = false;
            st_original_.resize(beam_width);
        }

        // 候補を追加する
        // ターン数最小化型の問題で、candidateによって実行可能解が得られる場合にのみ finished = true とする
        // ビーム幅分の候補をCandidateを追加したときにsegment treeを構築する
        void push(Action action, const Evaluator& evaluator, Hash hash, int parent, bool finished) {
            Cost cost = evaluator.evaluate();
            if (finished) {
                finished_candidates_.emplace_back(Candidate(action, evaluator, hash, parent, cost));
                return;
            }
            if (full_ && cost >= st_.all_prod().first) {
                // 保持しているどの候補よりもコストが小さくないとき
                return;
            }
            auto [valid, i] = hash_to_index_.get_index(hash);

            if (valid) {
                int j = hash_to_index_.get(i);
                if (hash == candidates_[j].hash) {
                    // ハッシュ値が等しいものが存在しているとき
                    if (cost < candidates_[j].cost) {
                        // 更新する場合
                        candidates_[j] = Candidate(action, evaluator, hash, parent, cost);
                        if (full_) {
                            st_.set(j, {cost, j});
                        }
                    }
                    return;
                }
            }
            if (full_) {
                // segment treeが構築されている場合
                int j = st_.all_prod().second;
                hash_to_index_.set(i, hash, j);
                candidates_[j] = Candidate(action, evaluator, hash, parent, cost);
                st_.set(j, {cost, j});
            } else {
                // segment treeが構築されていない場合
                hash_to_index_.set(i, hash, candidates_.size());
                candidates_.emplace_back(Candidate(action, evaluator, hash, parent, cost));

                if (candidates_.size() == beam_width) {
                    // 保持している候補がビーム幅分になったとき
                    construct_segment_tree();
                }
            }
        }

        // 選んだ候補を返す
        const vector<Candidate>& select() const {
            return candidates_;
        }

        // 実行可能解が見つかったか
        bool have_finished() const {
            return !finished_candidates_.empty();
        }

        // 実行可能解に到達する「候補」を返す
        vector<Candidate> get_finished_candidates() const {
            return finished_candidates_;
        }

        void clear() {
            candidates_.clear();
            hash_to_index_.clear();
            full_ = false;
        }

    private:
        size_t beam_width;
        vector<Candidate> candidates_;
        HashMap<Hash,int> hash_to_index_;
        bool full_;
        vector<pair<Cost,int>> st_original_;
        MaxSegtree st_;
        vector<Candidate> finished_candidates_;

        void construct_segment_tree() {
            full_ = true;
            for (size_t i = 0; i < beam_width; ++i) {
                st_original_[i] = {candidates_[i].cost, i};
            }
            st_ = MaxSegtree(st_original_);
        }
};

// 深さ優先探索に沿って更新する情報をまとめたクラス
class State {
    public:
        explicit State(const Environment& env) : env(env) {
            turn_num = 0;
            board = env.initial_board;
            extra_action_available = HyperParameter::beam_extra_action_num;
        }

        // 次の状態候補を全てselectorに追加する
        // 引数
        //   evaluator : 今の評価器
        //   hash      : 今のハッシュ値
        //   parent    : 今のノードID（次のノードにとって親となる）
        void expand(const Evaluator& evaluator, Hash hash, int parent, Selector& selector) {
          // const int next_stamp_x = turn_num % (Consts::board_size - Consts::stamp_size + 1);
          // const int next_stamp_y = turn_num / (Consts::board_size - Consts::stamp_size + 1);
          const int next_stamp_x = stamp_xy[turn_num].first;
          const int next_stamp_y = stamp_xy[turn_num].second;
          const int base_stamp_num = get_base_action_num(next_stamp_x, next_stamp_y);
          const int max_stamp_num = min(base_stamp_num + extra_action_available, 3);
          const bool is_final_turn = (next_stamp_x == Consts::board_size - Consts::stamp_size && next_stamp_y == Consts::board_size - Consts::stamp_size - 1);
          for (int combined_stamp_id = 0; combined_stamp_id < (int) env.combined_stamp_ids.size(); combined_stamp_id++) {
            if ((int) env.combined_stamp_ids[combined_stamp_id].size() > max_stamp_num) continue;

            update_board(combined_stamp_id, next_stamp_x, next_stamp_y, true);
            Hash next_hash = hash ^ get_hash(combined_stamp_id, next_stamp_x, next_stamp_y);
            ll score = get_partial_score(next_stamp_x, next_stamp_y);

            Action next_action = Action(combined_stamp_id, next_stamp_x, next_stamp_y);
            Evaluator next_evaluator = Evaluator(evaluator.score + score, is_final_turn ? extra_action_available + base_stamp_num - (int) env.combined_stamp_ids[combined_stamp_id].size() : 0);
            selector.push(next_action, next_evaluator, next_hash, parent, false);

            update_board(combined_stamp_id, next_stamp_x, next_stamp_y, false);
          }
        }

        // actionを実行して次の状態に遷移する
        void move_forward(Action action) {
          turn_num++;
          update_board(action.combined_stamp_id, action.stamp_x, action.stamp_y, true);
          extra_action_available -= (int) env.combined_stamp_ids[action.combined_stamp_id].size() - get_base_action_num(action.stamp_x, action.stamp_y);
        }

        // actionを実行する前の状態に遷移する
        // 今の状態は、親からactionを実行して遷移した状態である
        void move_backward(Action action) {
          turn_num--;
          update_board(action.combined_stamp_id, action.stamp_x, action.stamp_y, false);
          extra_action_available += (int) env.combined_stamp_ids[action.combined_stamp_id].size() - get_base_action_num(action.stamp_x, action.stamp_y);
        }

    private:
        const Environment &env;

        const vector<Pii> stamp_xy = {
          Pii(0, 0),
          Pii(0, 1),
          Pii(0, 2),
          Pii(0, 3),
          Pii(0, 4),
          Pii(0, 5),
          Pii(0, 6),
          Pii(1, 0),
          Pii(2, 0),
          Pii(3, 0),
          Pii(4, 0),
          Pii(5, 0),
          Pii(6, 0),
          Pii(1, 1),
          Pii(1, 2),
          Pii(1, 3),
          Pii(1, 4),
          Pii(1, 5),
          Pii(1, 6),
          Pii(2, 1),
          Pii(3, 1),
          Pii(4, 1),
          Pii(5, 1),
          Pii(6, 1),
          Pii(2, 2),
          Pii(2, 3),
          Pii(2, 4),
          Pii(2, 5),
          Pii(2, 6),
          Pii(3, 2),
          Pii(4, 2),
          Pii(5, 2),
          Pii(6, 2),
          Pii(3, 3),
          Pii(3, 4),
          Pii(3, 5),
          Pii(3, 6),
          Pii(4, 3),
          Pii(5, 3),
          Pii(6, 3),
          Pii(4, 4),
          Pii(4, 5),
          Pii(4, 6),
          Pii(5, 4),
          Pii(6, 4),
          Pii(5, 5),
          Pii(5, 6),
          Pii(6, 5),
          Pii(6, 6),
          Pii(-1, -1),
        };

        int turn_num;
        vector<vector<int>> board;
        int extra_action_available;

        int get_base_action_num(const int stamp_x, const int stamp_y) {
          return (stamp_x == Consts::board_size - Consts::stamp_size || stamp_y == Consts::board_size - Consts::stamp_size) ? 3 : 1;
        }

        void update_board(const int combined_stamp_id, const char stamp_x, const char stamp_y, const bool is_add) {
            for (int i = 0; i < Consts::stamp_size; i++) {
                for (int j = 0; j < Consts::stamp_size; j++) {
                    board[stamp_x + i][stamp_y + j] = (board[stamp_x + i][stamp_y + j] + (is_add ? env.combined_stamps[combined_stamp_id][i][j] : -env.combined_stamps[combined_stamp_id][i][j] + mod)) % mod;
                }
            }
        }

        ll get_partial_score(const char x, const char y) {
          int x_sz = x == Consts::board_size - Consts::stamp_size ? Consts::stamp_size : 1;
          int y_sz = y == Consts::board_size - Consts::stamp_size ? Consts::stamp_size : 1;
          ll score = 0;
          for (int i = 0; i < x_sz; i++) {
            for (int j = 0; j < y_sz; j++) {
              score += board[x + i][y + j];
            }
          }
          return score;
        }
};

// 探索木（二重連鎖木）のノード
struct Node {
    Action action;
    Evaluator evaluator;
    Hash hash;
    int parent, child, left, right;

    // 根のコンストラクタ
    Node(Action action, const Evaluator& evaluator, Hash hash) :
        action(action),
        evaluator(evaluator),
        hash(hash),
        parent(-1),
        child(-1),
        left(-1),
        right(-1) {}

    // 通常のコンストラクタ
    Node(const Candidate& candidate, int right) :
        action(candidate.action),
        evaluator(candidate.evaluator),
        hash(candidate.hash),
        parent(candidate.parent),
        child(-1),
        left(-1),
        right(right) {}
};

// 二重連鎖木に対する操作をまとめたクラス
class Tree {
    public:
        explicit Tree(const State& state, size_t nodes_capacity, const Node& root) :
            state_(state)
        {
            nodes_.reserve(nodes_capacity);
            root_ = nodes_.push(root);
        }

        // 状態を更新しながら深さ優先探索を行い、次のノードの候補を全てselectorに追加する
        void dfs(Selector& selector) {
            update_root();

            int v = root_;
            while (true) {
                v = move_to_leaf(v);
                state_.expand(nodes_[v].evaluator, nodes_[v].hash, v, selector);
                v = move_to_ancestor(v);
                if (v == root_) {
                    break;
                }
                v = move_to_right(v);
            }
        }

        // 根からノードvまでのパスを取得する
        vector<Action> get_path(int v) {
            // cerr << nodes_.size() << endl;

            vector<Action> path;
            while (nodes_[v].parent != -1) {
                path.push_back(nodes_[v].action);
                v = nodes_[v].parent;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        // 新しいノードを追加する
        int add_leaf(const Candidate& candidate) {
            int parent = candidate.parent;
            int sibling = nodes_[parent].child;
            int v = nodes_.push(Node(candidate, sibling));

            nodes_[parent].child = v;

            if (sibling != -1) {
                nodes_[sibling].left = v;
            }
            return v;
        }

        // ノードvに子がいなかった場合、vと不要な先祖を削除する
        void remove_if_leaf(int v) {
            if (nodes_[v].child == -1) {
                remove_leaf(v);
            }
        }

        // 最も評価がよいノードを返す
        int get_best_leaf(const vector<int>& last_nodes) {
            assert(!last_nodes.empty());
            int ret = last_nodes[0];
            for (int v : last_nodes) {
                if (nodes_[v].evaluator.evaluate() < nodes_[ret].evaluator.evaluate()) {
                    ret = v;
                }
            }
            return ret;
        }

    private:
        State state_;
        ObjectPool<Node> nodes_;
        int root_;

        // 根から一本道の部分は往復しないようにする
        void update_root() {
            int child = nodes_[root_].child;
            while (child != -1 && nodes_[child].right == -1) {
                root_ = child;
                state_.move_forward(nodes_[child].action);
                child = nodes_[child].child;
            }
        }

        // ノードvの子孫で、最も左にある葉に移動する
        int move_to_leaf(int v) {
            int child = nodes_[v].child;
            while (child != -1) {
                v = child;
                state_.move_forward(nodes_[child].action);
                child = nodes_[child].child;
            }
            return v;
        }

        // ノードvの先祖で、右への分岐があるところまで移動する
        int move_to_ancestor(int v) {
            while (v != root_ && nodes_[v].right == -1) {
                state_.move_backward(nodes_[v].action);
                v = nodes_[v].parent;
            }
            return v;
        }

        // ノードvの右のノードに移動する
        int move_to_right(int v) {
            state_.move_backward(nodes_[v].action);
            v = nodes_[v].right;
            state_.move_forward(nodes_[v].action);
            return v;
        }

        // 不要になった葉を再帰的に削除する
        void remove_leaf(int v) {
            while (true) {
                int left = nodes_[v].left;
                int right = nodes_[v].right;
                if (left == -1) {
                    int parent = nodes_[v].parent;

                    if (parent == -1) {
                        cerr << "ERROR: root is removed" << endl;
                        exit(-1);
                    }
                    nodes_.pop(v);
                    nodes_[parent].child = right;
                    if (right != -1) {
                        nodes_[right].left = -1;
                        return;
                    }
                    v = parent;
                } else {
                    nodes_.pop(v);
                    nodes_[left].right = right;
                    if (right != -1) {
                        nodes_[right].left = left;
                    }
                    return;
                }
            }
        }
};

// ビームサーチを行う関数
vector<Action> beam_search(const Config& config, State state, Node root) {
    Tree tree(state, config.nodes_capacity, root);

    // 探索中のノード集合
    vector<int> curr_nodes;
    curr_nodes.reserve(config.beam_width);
    // 本来は curr_nodes = {state.root_} とすべきだが, 省略しても問題ない

    // 新しいノードの集合
    vector<int> next_nodes;
    next_nodes.reserve(config.beam_width);

    // 新しいノード候補の集合
    Selector selector(config);

    for (int turn = 0; turn < config.max_turn; ++turn) {
        // Euler Tour で selector に候補を追加する
        tree.dfs(selector);

        if (selector.have_finished()) {
            // ターン数最小化型の問題で実行可能解が見つかったとき
            Candidate candidate = selector.get_finished_candidates()[0];
            vector<Action> ret = tree.get_path(candidate.parent);
            ret.push_back(candidate.action);
            return ret;
        }
        // 新しいノードを追加する
        for (const Candidate& candidate : selector.select()) {
            next_nodes.push_back(tree.add_leaf(candidate));
        }
        if (next_nodes.empty()) {
            // 新しいノードがないとき
            cerr << "ERROR: Failed to find any valid solution" << endl;
            return {};
        }
        // 不要なノードを再帰的に削除する
        for (int v : curr_nodes) {
            tree.remove_if_leaf(v);
        }
        // ダブルバッファリングで配列を使い回す
        swap(curr_nodes, next_nodes);
        next_nodes.clear();

        selector.clear();
    }
    // ターン数固定型の問題で全ターンが終了したとき
    int best_leaf = tree.get_best_leaf(curr_nodes);
    return tree.get_path(best_leaf);
}

} // namespace beam_search

Output solve_beam(const Environment &env) {
  const int max_turn = (env.board_size - Consts::stamp_size + 1) * (env.board_size - Consts::stamp_size + 1) - 1;
  const size_t beam_width = 1024;
  const size_t nodes_capacity = 16 * beam_width;
  const uint32_t hash_map_capacity = 64 * beam_width;
  beam_search::Config config = {
    max_turn,
    beam_width,
    nodes_capacity,
    hash_map_capacity
  };
  beam_search::init_hash(env);
  beam_search::State state(env);
  beam_search::Node root(beam_search::Action({}, 0, 0), beam_search::Evaluator(0, 0), 0);
  auto actions = beam_search::beam_search(config, state, root);

  Output output;
  for (const auto &action: actions) {
    for (const auto &stamp_id: env.combined_stamp_ids[action.combined_stamp_id]) {
      output.actions.push_back(Action(stamp_id, action.stamp_x, action.stamp_y));
    }
  }
  return output;
}

Output solve_sa_after_beam(const Environment &env, const Output &output_beam) {
  struct SAState {
    const Environment &env;

    vector<Action> actions;

    vector<vector<int>> board;

    ll score;

    SAState(const Environment &env, const Output &output_beam): env(env) {
      actions = output_beam.actions;
      board = env.initial_board;
      score = 0;

      update_score_full();
    }

    void update_score_full() {
      for (int i = 0; i < env.board_size; i++) {
        for (int j = 0; j < env.board_size; j++) {
          board[i][j] = env.initial_board[i][j];
        }
      }

      for (const auto &action: actions) {
        update_board(action, true);
      }

      score = 0;
      for (int i = 0; i < env.board_size; i++) {
        for (int j = 0; j < env.board_size; j++) {
          score += board[i][j];
        }
      }
    }

    void update_score_partial(const Action &action, const bool is_add) {
      for (int i = 0; i < Consts::stamp_size; i++) {
        for (int j = 0; j < Consts::stamp_size; j++) {
          score -= board[action.x + i][action.y + j];
        }
      }
      update_board(action, is_add);
      for (int i = 0; i < Consts::stamp_size; i++) {
        for (int j = 0; j < Consts::stamp_size; j++) {
          score += board[action.x + i][action.y + j];
        }
      }
    }

    void update_board(const Action &action, const bool is_add) {
      for (int i = 0; i < Consts::stamp_size; i++) {
        for (int j = 0; j < Consts::stamp_size; j++) {
          board[action.x + i][action.y + j] = (board[action.x + i][action.y + j] + (is_add ? env.stamps[action.stamp_id][i][j] : -env.stamps[action.stamp_id][i][j] + mod)) % mod;
        }
      }
    }

    ll get_score() {
      return score;
    }

    Output get_output() {
      Output output;
      output.actions = actions;
      return output;
    }
  };

  SAState state(env, output_beam);
  vector<Action> best_actions = state.actions;
  {
    ll score = state.get_score();
    ll last_score = score;
    ll best_score = score;

    const double base_temperature = 1e10;
    const double target_temperature = 1e8;
    // const double decay_rate = 4e-5;
    double temperature = base_temperature;

    int iter_count = 0;
    double time_start = theTimer.time();
    #ifdef LOCAL
    const double time_limit = 2.970;
    #else
    const double time_limit = 1.970;
    #endif

    while (true) {
      if (iter_count % 1024 == 0 && theTimer.time() > time_limit) break;
      const double roll = theRandom.nextDouble();
      if (roll < 0.3) {
        if ((int) state.actions.size() >= env.max_action) continue;
        const int i = theRandom.nextUIntMod(env.stamp_num);
        const int x = env.board_size - Consts::stamp_size;
        const int y = env.board_size - Consts::stamp_size;

        Action new_action(i, x, y);
        state.actions.push_back(new_action);

        state.update_score_partial(new_action, true);

        score = state.get_score();

        #ifdef DEBUG
        if (iter_count % 100000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
            best_actions = state.actions;
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          state.update_score_partial(new_action, false);
          state.actions.pop_back();

          // state.update_score_full();
          // assert(abs(last_score - state.get_score()) == 0);
          score = last_score;
        }
      }
      else if (roll < 0.6) {
        if (state.actions.size() == 0) continue;
        const int i = theRandom.nextUIntMod(state.actions.size());

        Action action_to_remove = state.actions[i];
        if (!(action_to_remove.x == env.board_size - Consts::stamp_size && action_to_remove.y == env.board_size - Consts::stamp_size)) continue;
        swap(state.actions[i], state.actions.back());
        state.actions.pop_back();

        state.update_score_partial(action_to_remove, false);

        score = state.get_score();

        #ifdef DEBUG
        if (iter_count % 100000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
            best_actions = state.actions;
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          state.update_score_partial(action_to_remove, true);
          state.actions.push_back(action_to_remove);

          // state.update_score_full();
          // assert(abs(last_score - state.get_score()) == 0);
          score = last_score;
        }
      }
      else if (roll < 1.0) {
        if (state.actions.size() == 0) continue;
        const int ia = theRandom.nextUIntMod(state.actions.size());
        const int is = theRandom.nextUIntMod(env.stamp_num);
        const int x = env.board_size - Consts::stamp_size;
        const int y = env.board_size - Consts::stamp_size;

        Action action_to_change = state.actions[ia];
        if (!(action_to_change.x == env.board_size - Consts::stamp_size && action_to_change.y == env.board_size - Consts::stamp_size)) continue;
        Action new_action(is, x, y);
        state.actions[ia] = new_action;
        
        state.update_score_partial(action_to_change, false);
        state.update_score_partial(new_action, true);

        score = state.get_score();

        #ifdef DEBUG
        if (iter_count % 100000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
        #endif

        if (score >= last_score) {
          last_score = score;
          if (score > best_score) {
            best_score = score;
            best_actions = state.actions;
          }
        }
        else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
          last_score = score;
        }
        else { // rollback
          state.update_score_partial(new_action, false);
          state.update_score_partial(action_to_change, true);
          state.actions[ia] = action_to_change;

          // state.update_score_full();
          // assert(abs(last_score - state.get_score()) == 0);
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
  }

  Output output;
  output.actions = best_actions;
  return output;
}

Output solve_greedy_after_beam(const Environment &env, const Output &output_beam) {
  vector<vector<int>> board = env.initial_board;
  for (auto &action: output_beam.actions) {
    for (int i = 0; i < Consts::stamp_size; i++) {
      for (int j = 0; j < Consts::stamp_size; j++) {
        board[action.x + i][action.y + j] = (board[action.x + i][action.y + j] + env.stamps[action.stamp_id][i][j] + mod) % mod;
      }
    }
  }
  vector<vector<int>> right_bottom_board(Consts::stamp_size, vector<int>(Consts::stamp_size));
  for (int i = 0; i < Consts::stamp_size; i++) {
    for (int j = 0; j < Consts::stamp_size; j++) {
      right_bottom_board[i][j] = board[env.board_size - Consts::stamp_size + i][env.board_size - Consts::stamp_size + j];
    }
  }
  int best_combined_stamp_id_1 = 0;
  int best_combined_stamp_id_2 = 0;
  ll best_score = 0;
  for (int combined_stamp_id_1 = 0; combined_stamp_id_1 < (int) env.combined_stamp_ids.size(); combined_stamp_id_1++) {
    for (int combined_stamp_id_2 = 0; combined_stamp_id_2 < (int) env.combined_stamp_ids.size(); combined_stamp_id_2++) {
      if ((int) (output_beam.actions.size() + env.combined_stamp_ids[combined_stamp_id_1].size() + env.combined_stamp_ids[combined_stamp_id_2].size()) > Consts::max_action) continue;
      ll score = 0;
      for (int i = 0; i < Consts::stamp_size; i++) {
        for (int j = 0; j < Consts::stamp_size; j++) {
          score += ((ll) right_bottom_board[i][j] + (ll) env.combined_stamps[combined_stamp_id_1][i][j] + (ll) env.combined_stamps[combined_stamp_id_2][i][j]) % mod;
        }
      }
      if (score > best_score) {
        best_score = score;
        best_combined_stamp_id_1 = combined_stamp_id_1;
        best_combined_stamp_id_2 = combined_stamp_id_2;
      }
    }
  }
  Output output_final = output_beam;
  for (const auto &stamp_id: env.combined_stamp_ids[best_combined_stamp_id_1]) {
    output_final.actions.push_back(Action(stamp_id, env.board_size - Consts::stamp_size, env.board_size - Consts::stamp_size));
  }
  for (const auto &stamp_id: env.combined_stamp_ids[best_combined_stamp_id_2]) {
    output_final.actions.push_back(Action(stamp_id, env.board_size - Consts::stamp_size, env.board_size - Consts::stamp_size));
  }
  return output_final;
}

Output solve(const Environment &env) {
  // Output output = solve_sa(env);
  Output output_beam = solve_beam(env);
  Output output_final = solve_sa_after_beam(env, output_beam);
  // Output output_final = solve_greedy_after_beam(env, output_beam);
  return output_final;
}

void print_output(const Output &output) {
  cout << output.actions.size() << endl;
  for (const auto &action: output.actions) {
    cout << action.stamp_id << " " << action.x << " " << action.y << endl;
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
