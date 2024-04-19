#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

#include <math.h>

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

/*
アルゴリズム概要
1. measurement costが小さくなるようにBaseを決める
2. Baseを500 + max(150, S * 3.0), その他のマスを 500 - max(150, S * 3.0)とする
3. Baseを中心とした盤面半分を均すことで、placement costを小さくする
4. 「対応している確率が高い(入口,出口)の組をBaseを使って検証」を繰り返す
*/

int L, N, S;

struct Pos {
  int x_, y_, idx_;
  Pos() {}
  // _x, _yがマイナスの場合はプラスに変換しておく
  Pos(int _x, int _y) : x_(_x), y_(_y), idx_((_y + L) % L * L + (_x + L) % L) {}
  int X() const { return x_; }
  int Y() const { return y_; }
  operator int() const { return idx_; }
  operator size_t() const { return idx_; }
  int Manhattan(const Pos& other) const {
    // トーラスなので、正の方向と負の方向のうち近いほう
    return min(abs(X() - other.X()), L - abs(X() - other.X())) +
           min(abs(Y() - other.Y()), L - abs(Y() - other.Y()));
  }
  int Cost(const Pos& other) const { return (Manhattan(other) + 10) * 100; }
  int DeltaCost() const { return Cost(Pos(0, 0)); }
  void Add(const Pos other) { *this = Pos(x_ + other.x_, y_ + other.y_); }
  bool operator<(const Pos& other) const { return this->idx_ < other.idx_; }
  bool operator==(const Pos& other) const { return this->idx_ == other.idx_; }
  bool operator!=(const Pos& other) const { return this->idx_ != other.idx_; }
};

vector<Pos> holes;

// 平均mean, 標準偏差stdの正規分布におけるvalueの確率密度
double ProbDensity(const double mean, const double std, const double value) {
  constexpr double PI = acos(-1);
  const double var = std * std;
  return 1.0 / sqrt(2.0 * PI * var) *
         exp(-(value - mean) * (value - mean) / (2.0 * var));
}

// value以下となる確率
double ProbLEThan(const double mean, const double std, const double value) {
  return 0.5 * (1.0 + std::erf((value - mean) / (sqrt(2.0) * std)));
}

// value以上となる確率
double ProbGEThan(const double mean, const double std, const double value) {
  return 1.0 - ProbLEThan(mean, std, value);
}

// 入口iをdeltaずらして測定
int Execute(const int i, const Pos& delta) {
  // マイナス方向に移動したほうが近いならマイナス方向に移動する
  const int plus_dy = delta.Y();
  const int minus_dy = delta.Y() - L;
  const int dy = plus_dy < abs(minus_dy) ? plus_dy : minus_dy;

  const int plus_dx = delta.X();
  const int minus_dx = delta.X() - L;
  const int dx = plus_dx < abs(minus_dx) ? plus_dx : minus_dx;

  cout << i << " " << dy << " " << dx << endl;
  int ret;
  cin >> ret;
  return ret;
}

struct OneBaseSolver {
  Pos DecideBase() const {
    // measurement costが最小になるようにBaseを決める
    Pos best_base;
    long long int min_cost = (long long int)1e18;
    for (int y = 0; y < L; y++) {
      for (int x = 0; x < L; x++) {
        Pos base(x, y);
        // holeのposとそのmeasurement costのpair
        vector<pair<Pos, long long int>> hole_costs;
        for (int j = 0; j < N; j++) {
          hole_costs.push_back({holes[j], holes[j].Cost(base)});
        }
        // measurement costの昇順にソート
        sort(hole_costs.begin(), hole_costs.end(),
             [](const auto& l, const auto& r) { return l.second < r.second; });
        // measurement costが小さい順に出口を試すことを考えると、
        // だんだんと候補の入口が減っていくので、
        // measurement costが小さいものほどたくさんの入口で試すことになる
        long long int cost_sum = 0;
        for (int idx = 0; idx < N; idx++) {
          long long int remain_count = N - idx;
          cost_sum += remain_count * hole_costs[idx].second;
        }
        if (min_cost > cost_sum) {
          min_cost = cost_sum;
          best_base = base;
        }
      }
    }

    return best_base;
  }

  // 盤面に配置した各値を出力する
  void PrintBoard(const vector<int>& board) const {
    for (int y = 0; y < L; y++) {
      for (int x = 0; x < L; x++) {
        Pos p(x, y);
        cout << board[p] << " ";
      }
      cout << endl;
    }
  }

  struct Predictor {
    // ベイズの定理に概ね基づいているつもりですが自信はないです

    // f(i,j) = ∏ P(測定結果k' | 入口i == 出口j)
    // Q[i][j] = f(i,j) / Σ f(i, j')
    // g(i,j) = Q[i][j] * (Π_{i != i'} 1.0 - Q[i'][j])
    // Prob(i,j) = g(i,j) / Σ g(i',j)

    // f(i,j)：「入口iと出口jが対応している場合に、測定結果がサンプルされる確率」を、
    //          入口iの各測定結果について掛け合わせた値。
    // Q[i][j]:
    //          f(i,j)をiを固定して各jで足し合わせた場合の和が1になるようにした値。
    //          入口iだけ考えた時に、出口jに対応している確率。
    // g(i,j):
    //          入口iが出口jに対応して、それ以外の入口が出口jに対応しない確率。(の近似？)
    //          Q[i][j] と　(1.0 - Q[i'][j]) を掛け合わせている。
    // Prob(i,j):
    //          g(i,j)をjを固定して各iで足し合わせた場合の和が1になるようにした値。
    //          入口iと出口jが対応する確率の推定値。

    vector<vector<double>> Q;
    // Q[i][j] / (1.0 - Q[i][j])のsum
    vector<double> Q_j_sums;

    Predictor()
        : Q(N, vector<double>(N, 1.0 / N)),
          Q_j_sums(N, 1.0 / N / (1.0 - 1.0 / N) * N) {}

    // Q[i][j]を受け取ってg(i,j)を返す
    double G(const double q) const {
      // 1e-5は0除算ケア
      return q / (1.0 - q + 1e-5);
    }

    // 測定値に基づいて更新
    void Update(const vector<int>& board, const int i, const Pos delta,
                const int measured_value) {
      double q_i_sums = 0.0;
      for (int j = 0; j < N; j++) {
        // 差分更新
        Q_j_sums[j] -= G(Q[i][j]);

        Pos p = holes[j];
        p.Add(delta);
        const int mean = board[p];
        // 平均mean, 標準偏差Sの正規分布から
        // 測定値がサンプルされる確率
        if (measured_value == 0) {
          // 0以下のどれかがサンプルされる確率
          Q[i][j] *= ProbLEThan(mean, S, 0);
        } else if (measured_value == 1000) {
          // 1000以上のどれかがサンプルされる確率
          Q[i][j] *= ProbGEThan(mean, S, 1000);
        } else {
          // measured_valueがサンプルされる確率
          Q[i][j] *= ProbDensity(mean, S, measured_value);
        }

        q_i_sums += Q[i][j];
      }

      for (int j = 0; j < N; j++) {
        // f(i,j)の和が1になるようにする
        Q[i][j] /= q_i_sums;
        // 丸め誤差ケアで一応0.0 <= Q[i][j] <= 1.0を保証する
        Q[i][j] = std::clamp(Q[i][j], 0.0, 1.0);
        // 差分更新
        Q_j_sums[j] += G(Q[i][j]);
      }
    }

    double Prob(const int i, const int j) const {
      // g(i,j)を和が1になるようにしてから返す
      return this->G(Q[i][j]) / Q_j_sums[j];
    }

    // 予測結果を出力する
    void PrintResult() const {
      cout << "-1 -1 -1" << endl;
      for (int i = 0; i < N; i++) {
        // 最も確率が高い出口j
        int best_j = 0;
        double max_prob = 0.0;
        for (int j = 0; j < N; j++) {
          const double prob = this->Prob(i, j);
          if (prob > max_prob) {
            best_j = j;
            max_prob = prob;
          }
        }
        cout << best_j << endl;
      }
    }
  };

  void Run() const {
    // 1. measurement costが小さくなるようにBaseを決める
    Pos base = DecideBase();

    // 2. Baseを500 + S * 2.0, その他のマスを 500 - S * 2.0とする
    // このパラメータをN, L, Sごとに調整するだけでもさらにスコア上がると思います
    const int high_value = 500 + std::clamp((int)(S * 3.0), 150, 500);
    const int low_value = 1000 - high_value;
    vector<int> board(L * L, low_value);
    board[base] = high_value;

    // 3. Baseを中心とした盤面半分を均すことで、placement costを小さくする
    // これによってplacement costだけでなくmeasurement costも減る
    // マスの値に多様性が生まれることで、測定で得られる情報も増えてるっぽい？
    for (int _t = 0; _t < 10000; _t++) {
      auto new_board = board;
      for (int y = 0; y < L; y++) {
        for (int x = 0; x < L; x++) {
          const Pos p(x, y);
          // Base中心の半分のみが対称
          if (base.Manhattan(p) >= L / 2) continue;
          // Baseは固定
          if (p == base) continue;
          // そのセルと4方向の差の2乗を最小化する
          double sum = 0.0;
          for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
              if (abs(dx) + abs(dy) != 1) continue;
              auto adj_p = p;
              Pos delta(dx, dy);
              adj_p.Add(delta);
              sum += board[adj_p];
            }
          }
          new_board[p] = std::clamp((int)std::round(sum / 4.0), 0, 1000);
        }
      }

      board = std::move(new_board);
    }

    PrintBoard(board);

    Predictor predictor;

    // 出口jからbaseに到達するためのdelta
    vector<Pos> deltas;
    for (int j = 0; j < N; j++) {
      Pos delta = Pos(base.X() - holes[j].X(), base.Y() - holes[j].Y());
      deltas.push_back(delta);
    }

    // 4. 「対応している確率が高い(入口,出口)の組をBaseを使って検証」を繰り返す
    for (int t = 0; t < 10000; t++) {
      // Prob(i,j) >= kProbThreshold となった入口iと出口jの組を確定とみなす
      constexpr double kProbThreshold = 0.99;

      // (入口i, 出口j)の対応関係について、出口jからbaseを測定した場合に、
      // 「対応関係が正しい確率 /  測定コスト」が最大の組を使用する
      // ただし、対応関は除外する
      double max_eval = 0.0;
      int best_i = -1;
      int best_j = -1;
      for (int i = 0; i < N; i++) {
        // 出口が確定している入口は除外
        bool skip = false;
        for (int j = 0; j < N; j++) {
          const double prob = predictor.Prob(i, j);
          if (prob >= kProbThreshold) {
            skip = true;
            break;
          }
        }
        if (skip) continue;

        // 「対応関係が正しい確率 /  測定コスト」を評価
        for (int j = 0; j < N; j++) {
          const double prob = predictor.Prob(i, j);
          const double eval = prob / deltas[j].DeltaCost();

          if (eval > max_eval) {
            max_eval = eval;
            best_i = i;
            best_j = j;
          }
        }
      }

      // 全ての入口iが確定していたら終わり
      if (best_i == -1) break;

      // 実行
      const int measured_value = Execute(best_i, deltas[best_j]);
      // 更新
      predictor.Update(board, best_i, deltas[best_j], measured_value);
    }

    predictor.PrintResult();
  }
};

// 最初の入力を受け取る
void Input() {
  cin >> L >> N >> S;
  for (int i = 0; i < N; i++) {
    int y, x;
    cin >> y >> x;
    holes.push_back(Pos(x, y));
  }
}

int main(void) {
  Input();
  OneBaseSolver solver;
  solver.Run();
}