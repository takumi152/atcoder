#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a(n), b(n);
  for (auto &x: a) cin >> x;
  for (auto &x: b) cin >> x;

  vector<tuple<int, int, int> > abi(n);
  for (int i = 0; i < n; i++) abi[i] = make_tuple(a[i], b[i], i);
  sort(abi.begin(), abi.end());

  vector<int> ans;
  int has_b_lt_head = false;
  int has_b_gt_head = false;
  int head = 1000000007;
  int tail = -1;
  int p = 0;
  while (p < n) {
    if (get<2>(abi[p]) <= tail) {
      p++;
      continue;
    }

    if (tail == -1 && get<0>(abi[p]) >= get<1>(abi[p])) {
      ans.push_back(get<2>(abi[p]));
      break;
    }

    if (tail != -1) {
      if (!has_b_lt_head && !has_b_gt_head && get<0>(abi[p]) >= b[head]) break;
      else if (!has_b_lt_head && has_b_gt_head && get<0>(abi[p]) > b[head]) break;
      else if (has_b_lt_head && !has_b_gt_head && get<0>(abi[p]) >= b[head]) break;
    }

    int a_target = get<0>(abi[p]);
    int old_p = p;
    int old_tail = tail;
    bool first_num = (tail == -1);
    while (p < n) {
      if (get<0>(abi[p]) == a_target) {
        if (get<2>(abi[p]) > old_tail) {
          ans.push_back(get<2>(abi[p]));
          tail = max(tail, get<2>(abi[p]));
          if (first_num) head = min(head, get<2>(abi[p]));
        }
        p++;
      }
      else break;
    }

    if (!has_b_lt_head && !has_b_gt_head) {
      int first_lt = 1000000007;
      int first_gt = 1000000007;
      for (int i = old_p; i < p; i++) {
        if (get<2>(abi[i]) != head && get<2>(abi[i]) > old_tail && get<1>(abi[i]) < b[head]) first_lt = min(first_lt, get<2>(abi[i]));
        if (get<2>(abi[i]) != head && get<2>(abi[i]) > old_tail && get<1>(abi[i]) > b[head]) first_gt = min(first_gt, get<2>(abi[i]));
      }
      if (first_lt < first_gt) has_b_lt_head = true;
      if (first_lt > first_gt) has_b_gt_head = true;
    }
  }

  sort(ans.begin(), ans.end());

  for (auto &x: ans) cout << a[x] << " ";
  for (auto &x: ans) cout << b[x] << " ";
  cout << endl;

  return 0;
}
