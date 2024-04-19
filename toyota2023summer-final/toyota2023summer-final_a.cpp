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
mt19937_64 theMersenne(1);

// libs

// hyper parameters

// structs
struct Position {
  int x, y;
  Position() = default;
  Position(int x, int y): x(x), y(y) {}

  bool operator==(const Position& that) const {
    return this->x == that.x && this->y == that.y;
  }

  bool operator!=(const Position& that) const {
    return !(*this == that);
  }
};

struct Input;
bool within_board(const Input &input, int x, int y);

// enums

// constants

// inputs
struct Input {
  int board_size;
  int wall_num;
  vector<Position> wall_positions;

  int turn_max;
  vector<vector<bool>> has_wall;
};

// internals
struct State {
  const Input &input;

  vector<int> container_num_history;

  vector<vector<bool>> can_place;
  vector<vector<int>> container_number;

  State (const Input &input) : input(input) {
    this->can_place = vector<vector<bool>>(this->input.board_size, vector<bool>(this->input.board_size, true));
    for (auto &w: this->input.wall_positions) {
      this->can_place[w.x][w.y] = false;
    }
    this->can_place[0][(this->input.board_size - 1) / 2] = false; // entrance

    container_number = vector<vector<int>>(input.board_size, vector<int>(input.board_size));
  }
};

// outputs
struct Output {
  vector<Position> container_withdraw_order;

  Output(vector<uchar> &beam_res, const Input &input) {
    for (auto &op: beam_res) {
      int x = op / input.board_size;
      int y = op % input.board_size;
      container_withdraw_order.push_back(Position(x, y));
    }
  }
};

struct BeamSearch {
  // 【問題固有】入力や前計算を管理する構造体
  struct Input {
    int board_size;
    vector<vector<int>> container_number;
    vector<ull> zobrist;
    
    Input(const ::Input &input, const ::State &state) {
      this->board_size = input.board_size;
      this->container_number = state.container_number;

      this->zobrist = vector<ull>(board_size * board_size);
      for (int i = 0; i < board_size * board_size; i++) {
        this->zobrist[i] = theRandom.nextULL();
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
    int track_id; // 履歴のID

    // 【問題固有】状態をここに持たせる
    int score;
    ull hash;
    vector<bool> is_removed;
    vector<bool> can_remove;

    Node(const ::Input &input, ::State &state) {
      track_id = -1;
      score = 0;
      hash = 0;

      is_removed = vector<bool>(input.board_size * input.board_size, false);
      for (auto &w: input.wall_positions) {
        is_removed[w.x * input.board_size + w.y] = true;
      }
      queue<int> que;
      que.push((input.board_size - 1) / 2);
      while (!que.empty()) {
        auto pos = que.front();
        que.pop();
        int i = pos / input.board_size;
        int j = pos % input.board_size;
        if (is_removed[pos]) continue;
        is_removed[pos] = true;
        const vector<int> dx = {-1, 1, 0, 0};
        const vector<int> dy = {0, 0, -1, 1};
        for (int d = 0; d < 4; d++) {
          if (within_board(input, i + dx[d], j + dy[d]) && !is_removed[(i + dx[d]) * input.board_size + (j + dy[d])] && state.can_place[i + dx[d]][j + dy[d]]) {
            que.push((i + dx[d]) * input.board_size + (j + dy[d]));
          }
        }
      }

      can_remove = vector<bool>(input.board_size * input.board_size, false);
      for (int i = 0; i < input.board_size * input.board_size; i++) {
        if (i % input.board_size > 0                    && is_removed[i - 1]                && !input.has_wall[i / input.board_size][(i - 1) % input.board_size]) can_remove[i] = true;
        if (i % input.board_size < input.board_size - 1 && is_removed[i + 1]                && !input.has_wall[i / input.board_size][(i + 1) % input.board_size]) can_remove[i] = true;
        if (i / input.board_size > 0                    && is_removed[i - input.board_size] && !input.has_wall[(i - input.board_size) / input.board_size][i % input.board_size]) can_remove[i] = true;
        if (i / input.board_size < input.board_size - 1 && is_removed[i + input.board_size] && !input.has_wall[(i + input.board_size) / input.board_size][i % input.board_size]) can_remove[i] = true;
      }
    }
    Node(int score, ull hash, vector<bool> &is_removed, vector<bool> &can_remove) : track_id(-1), score(score), hash(hash), is_removed(is_removed), can_remove(can_remove) {}

    // 【問題固有】thisからcandの遷移をしたときの新たなNodeを返す
    Node new_node(const Input &input, const int turn, const Cand &cand) {
      Node next_node(cand.eval_score, cand.hash, is_removed, can_remove);
      next_node.is_removed[cand.op] = true;
      next_node.can_remove[cand.op] = false;
      if (cand.op % input.board_size > 0                    && !is_removed[cand.op - 1])                next_node.can_remove[cand.op - 1] = true;
      if (cand.op % input.board_size < input.board_size - 1 && !is_removed[cand.op + 1])                next_node.can_remove[cand.op + 1] = true;
      if (cand.op / input.board_size > 0                    && !is_removed[cand.op - input.board_size]) next_node.can_remove[cand.op - input.board_size] = true;
      if (cand.op / input.board_size < input.board_size - 1 && !is_removed[cand.op + input.board_size]) next_node.can_remove[cand.op + input.board_size] = true;
      return next_node;
    }
  };

  const int MAX_WIDTH; // 【問題固有】ビーム幅
  const int TURN; // 【問題固有】探索を行うターン数

  vector<pair<ll, uchar>> track; // 履歴を管理
  vector<Node> nodes;
  vector<Node> next_nodes;

  BeamSearch(const int width, const int turn, const Node node, const ::Input &input): MAX_WIDTH(width), TURN(turn) {
    track = vector<pair<ll, uchar>>();
    nodes = vector<Node>{node};
    next_nodes = vector<Node>();
  }

  // 【問題固有】self.nodes[idx]からの候補をcandsに積む
  void append_cands(const Input &input, const int turn, const int idx, vector<Cand> &cands) {
    auto node = this->nodes[idx];

    for (int op = 0; op < input.board_size * input.board_size; op++) {
      if (!node.can_remove[op]) continue;

      int score = node.score;
      score -= abs(turn - input.container_number[op / input.board_size][op % input.board_size]);

      int hash = node.hash;
      hash ^= input.zobrist[op];

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
      new_node.track_id = (int) this->track.size() - 1;
      this->next_nodes.push_back(new_node);
    }

    swap(this->nodes, this->next_nodes);
  }

  // 復元
  vector<uchar> restore(int idx) {
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
  pair<ll, vector<uchar>> solve(Input &input) {
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

    return make_pair(best->raw_score(input), ret);
  }
};

Input get_first_input() {
  Input input;
  cin >> input.board_size >> input.wall_num;
  input.wall_positions = vector<Position>(input.wall_num);
  for (auto &w: input.wall_positions) cin >> w.x >> w.y;
  return input;
}

void init(Input &input) {
  input.turn_max = input.board_size * input.board_size - 1 - input.wall_num;

  input.has_wall = vector<vector<bool>>(input.board_size, vector<bool>(input.board_size));
  for (auto &w: input.wall_positions) {
    input.has_wall[w.x][w.y] = true;
  }
}

int get_turn_input() {
  int container_num;
  cin >> container_num;
  return container_num;
}

void print_turn_output(Position pos) {
  cout << pos.x << " " << pos.y << endl;
}

bool within_board(const Input &input, int x, int y) {
  return 0 <= x && x < input.board_size && 0 <= y && y < input.board_size;
}

Output solve(const Input &input) {
  State state(input);
  for (int turn = 0; turn < input.turn_max; turn++) {
    auto next_container_num = get_turn_input();
    state.container_num_history.push_back(next_container_num);

    vector<vector<int>> dist_from_entrance(input.board_size, vector<int>(input.board_size, 999));
    {
      queue<pair<int, Position>> que;
      que.emplace(0, Position(0, (input.board_size - 1) / 2));
      while (!que.empty()) {
        auto [dist, pos] = que.front();
        que.pop();
        if (dist >= dist_from_entrance[pos.x][pos.y]) continue;
        dist_from_entrance[pos.x][pos.y] = dist;
        const vector<int> dx = {-1, 1, 0, 0};
        const vector<int> dy = {0, 0, -1, 1};
        for (int d = 0; d < 4; d++) {
          if (within_board(input, pos.x + dx[d], pos.y + dy[d])
           && dist + 1 < dist_from_entrance[pos.x + dx[d]][pos.y + dy[d]]) {
            que.emplace(dist + 1, Position(pos.x + dx[d], pos.y + dy[d]));
          }
        }
      }
    }

    vector<int> distance_histogram(input.board_size * input.board_size);
    {
      for (int i = 0; i < input.board_size; i++) {
        for (int j = 0; j < input.board_size; j++) {
          if (dist_from_entrance[i][j] < input.board_size * input.board_size) distance_histogram[dist_from_entrance[i][j]]++;
        }
      }
    }

    vector<vector<int>> degree(input.board_size, vector<int>(input.board_size));
    {
      for (int i = 0; i < input.board_size; i++) {
        for (int j = 0; j < input.board_size; j++) {
          int deg = 0;
          const vector<int> dx = {-1, 1, 0, 0};
          const vector<int> dy = {0, 0, -1, 1};
          for (int d = 0; d < 4; d++) {
            if (within_board(input, i + dx[d], j + dy[d]) && state.can_place[i + dx[d]][j + dy[d]]) {
              deg++;
            }
          }
          degree[i][j] = deg;
        }
      }
    }

    vector<vector<bool>> is_blocking(input.board_size, vector<bool>(input.board_size));
    {
      for (int i = 0; i < input.board_size; i++) {
        for (int j = 0; j < input.board_size; j++) {
          if (!state.can_place[i][j]) {
            is_blocking[i][j] = true;
            continue;
          }
          vector<vector<bool>> visited(input.board_size, vector<bool>(input.board_size));
          queue<Position> que;
          que.push(Position(0, (input.board_size - 1) / 2));
          while (!que.empty()) {
            auto pos = que.front();
            que.pop();
            if (visited[pos.x][pos.y]) continue;
            visited[pos.x][pos.y] = true;
            const vector<int> dx = {-1, 1, 0, 0};
            const vector<int> dy = {0, 0, -1, 1};
            for (int d = 0; d < 4; d++) {
              if (within_board(input, pos.x + dx[d], pos.y + dy[d]) && state.can_place[pos.x + dx[d]][pos.y + dy[d]] && !visited[pos.x + dx[d]][pos.y + dy[d]] && !(pos.x + dx[d] == i && pos.y + dy[d] == j)) {
                que.push(Position(pos.x + dx[d], pos.y + dy[d]));
              }
            }
          }
          bool good = true;
          for (int p = 0; p < input.board_size; p++) {
            for (int q = 0; q < input.board_size; q++) {
              if (!(p == i && q == j) && state.can_place[p][q] && !visited[p][q]) {
                good = false;
                break;
              }
            }
          }
          is_blocking[i][j] = !good;
        }
      }
    }

    Position best_position;
    int best_score = -1e9;
    for (int i = 0; i < input.board_size; i++) {
      for (int j = 0; j < input.board_size; j++) {
        if (is_blocking[i][j]) continue;
        state.can_place[i][j] = false;
        state.container_number[i][j] = next_container_num;
        BeamSearch::Input beam_input(input, state);
        BeamSearch::Node beam_node(input, state);
        BeamSearch beam_solver(1, turn + 1, beam_node, input);
        state.can_place[i][j] = true;
        state.container_number[i][j] = 0;
        auto [beam_score, _] =  beam_solver.solve(beam_input);
        int distance_score = 0;
        {
          int order_num = next_container_num;
          {
            for (auto &n: state.container_num_history) {
              if (n < next_container_num) order_num--;
            }
          }

          int optimal_distance = 0;
          {
            int item_checked = 0;
            for (int i = 0; i < (int) distance_histogram.size(); i++) {
              item_checked += distance_histogram[i];
              if (item_checked >= order_num) {
                optimal_distance = i;
                break;
              }
            }
          }
          distance_score = -abs(optimal_distance - dist_from_entrance[i][j]);
        }
        int score = beam_score + distance_score * 100;
        if (score > best_score) {
          best_score = score;
          best_position = Position(i, j);
        }
      }
    }

    state.can_place[best_position.x][best_position.y] = false;
    state.container_number[best_position.x][best_position.y] = next_container_num;
    print_turn_output(best_position);
    cout << "#" << best_score << endl;
  }

  BeamSearch::Input beam_input(input, state);
  BeamSearch::Node beam_node(input, state);
  BeamSearch beam_solver(5000, input.turn_max, beam_node, input);
  auto [beam_score, ans] = beam_solver.solve(beam_input);
  return Output(ans, input);
}

void print_final_output(const Output &output) {
  for (auto &p: output.container_withdraw_order) {
    cout << p.x << " " << p.y << endl;
  }
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  auto input = get_first_input();

  init(input);
  auto output = solve(input);

  print_final_output(output);

  return 0;
}
