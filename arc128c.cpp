#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <cstdlib>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

struct fenwick_tree {
  int n;
  vector<ll> data;

  fenwick_tree() {
    init(0);
  }

  fenwick_tree(int s) {
    init(s);
  }

  fenwick_tree(vector<ll> &v) {
    int s = v.size();
    init(s);
    for (int i = 0; i < s; i++) add(i, v[i]);
  }

  void init(int s) {
    n = s;
    data = vector<ll>(s);
  }

  void add(int p, ll v) {
    p++;
    while (p <= n) {
      data[p-1] += v;
      p += p & -p;
    }
  }

  ll sum(int l, int r) {
    return sum(r) - sum(l);
  }

  ll sum(int r) {
    ll s = 0;
    while (r > 0) {
      s += data[r-1];
      r -= r & -r;
    }
    return s;
  }
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, m, s;
  cin >> n >> m >> s;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  fenwick_tree a_ft(a);

  // auto solve = [&](auto self, int left, int right, double left_unit_minimum, double right_unit_maximum, double budget, double &res_left_unit) {
  //   if (left >= right) return 0.0;
  //
  //   int best_split_idx = left;
  //   double best_score = (budget / (right - left)) * (double) a_ft.sum(left, right);
  //   double best_split_left_unit = (budget / (right - left));
  //   double best_split_right = budget;
  //   double best_split_right_unit = (budget / (right - left));
  //   for (int split_idx = left + 1; split_idx < right; split_idx++) {
  //     double split_right = min((double) (right_unit_maximum * (right - split_idx)), budget);
  //     double split_left = budget - split_right;
  //     double split_right_unit = split_right / (double) (right - split_idx);
  //     double split_left_unit = split_left / (double) (split_idx - left);
  //     if (split_left_unit < left_unit_minimum) {
  //       split_left = left_unit_minimum * (double) (split_idx - left);
  //       split_right = budget - split_left;
  //       split_left_unit = split_left / (double) (split_idx - left);
  //       split_right_unit = split_right / (double) (right - split_idx);
  //     }
  //     double score = split_left_unit * (double) a_ft.sum(left, split_idx) + split_right_unit * (double) a_ft.sum(split_idx, right);
  //     if (score > best_score) {
  //       best_split_idx = split_idx;
  //       best_score = score;
  //       best_split_left_unit = split_left_unit;
  //       best_split_right = split_right;
  //       best_split_right_unit = split_right_unit;
  //     }
  //   }
  //   cerr << left << " " << right << " " << best_split_idx << " " << best_score << " " << left_unit_minimum << " " << right_unit_maximum << " " << budget << " " << best_split_left_unit << endl;
  //
  //   res_left_unit = best_split_left_unit;
  //   if (best_split_idx == left) return best_score;
  //   else {
  //     double score_to_return = self(self, best_split_idx, right, best_split_left_unit, right_unit_maximum, best_split_right, res_left_unit);
  //     score_to_return += self(self, left, best_split_idx, left_unit_minimum, res_left_unit, budget - best_split_right, res_left_unit);
  //     return score_to_return;
  //   }
  // };
  //
  // double res_left_unit = 0.0;
  // cout << setprecision(20) << solve(solve, 0, n, 0, m, s, res_left_unit) << endl;

  auto solve = [&](auto self, int right, double budget) {
    if (right <= 0) return 0.0;

    double best_idx = right - 1;
    double best_efficiency = 0.0;
    for (int i = right - 1; i >= 0; i--) {
      double efficiency = a_ft.sum(i, right) / (double) (right - i);
      if (efficiency > best_efficiency) {
        best_idx = i;
        best_efficiency = efficiency;
      }
    }

    double score = best_efficiency * min((double) m, budget / (double) (right - best_idx)) * (double) (right - best_idx);
    double budget_remaining = budget - min((double) m, budget / (double) (right - best_idx)) * (double) (right - best_idx);
    return score + self(self, best_idx, budget_remaining);
  };

  cout << setprecision(20) << solve(solve, n, s);

  return 0;
}
