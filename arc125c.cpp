#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, k;
  cin >> n >> k;
  vector<int> a(k);
  for (auto &x: a) {
    cin >> x;
    x--;
  }

  // 9
  // 1 3 6 7 9
  // 1 3 2 6 4 7 5 9 8
  //
  // 9
  // 3 8
  // 3 1 9 8 7 6 5 4 2

  vector<bool> appeared(n);
  for (int i = 0; i < k; i++) appeared[a[i]] = true;
  set<int> unappeared;
  for (int i = 0; i < n; i++) {
    if (!appeared[i]) unappeared.insert(i);
  }

  vector<bool> used(n);
  vector<int> ans;
  for (int i = 0; i < k; i++) {
    if (i < k - 1) {
      ans.push_back(a[i]);
      used[a[i]] = true;
      if (!unappeared.empty()) {
        auto it = unappeared.begin();
        if (*it < a[i]) {
          ans.push_back(*it);
          used[*it] = true;
          unappeared.erase(it);
        }
      }
    }
    else {
      for (int j = n-1; j >= 0; j--) {
        if (!used[j]) {
          ans.push_back(j);
        }
      }
    }
  }

  for (auto &x: ans) cout << x + 1 << " ";
  cout << endl;

  return 0;
}
