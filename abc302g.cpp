#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

void get_combination(vector<int> &a, int sz, vector<vector<int>> &res, vector<int> &now, int depth = 0, int pos = 0) {
  while (pos < (int) a.size()) {
    now.push_back(a[pos]);
    if (depth >= sz - 1) res.push_back(now);
    else get_combination(a, sz, res, now, depth + 1, pos + 1);
    now.pop_back();
    pos++;
  }
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  vector<int> a_sorted = a;
  sort(a_sorted.begin(), a_sorted.end());

  vector<vector<int>> swap_matrix(4, vector<int>(4));
  for (int i = 0; i < n; i++) {
    swap_matrix[a[i]-1][a_sorted[i]-1]++;
  }

  auto deduct_from_matrix = [&](vector<int> &comb, int sz) {
    int cycle_min = (int) 1e9;
    for (int i = 0; i < sz - 1; i++) {
      cycle_min = min(cycle_min, swap_matrix[comb[i]][comb[i+1]]);
    }
    cycle_min = min(cycle_min, swap_matrix[comb[sz-1]][comb[0]]);

    for (int i = 0; i < sz - 1; i++) {
      swap_matrix[comb[i]][comb[i+1]] -= cycle_min;
    }
    swap_matrix[comb[sz-1]][comb[0]] -= cycle_min;

    return cycle_min * (sz - 1);
  };

  auto deduct_from_matrix_for_permutation = [&](vector<vector<int>> &comb, int sz) {
    int cost = 0;
    for (auto &c: comb) {
      do {
        cost += deduct_from_matrix(c, sz);
      } while (next_permutation(c.begin(), c.end()));
    }
    return cost;
  };

  int ans = 0;

  vector<int> nums = {0, 1, 2, 3};
  vector<int> temp;

  vector<vector<int>> comb_1;
  get_combination(nums, 1, comb_1, temp);
  ans += deduct_from_matrix_for_permutation(comb_1, 1);

  vector<vector<int>> comb_2;
  get_combination(nums, 2, comb_2, temp);
  ans += deduct_from_matrix_for_permutation(comb_2, 2);

  vector<vector<int>> comb_3;
  get_combination(nums, 3, comb_3, temp);
  ans += deduct_from_matrix_for_permutation(comb_3, 3);

  vector<vector<int>> comb_4;
  get_combination(nums, 4, comb_4, temp);
  ans += deduct_from_matrix_for_permutation(comb_4, 4);

  cout << ans << endl;

  return 0;
}
