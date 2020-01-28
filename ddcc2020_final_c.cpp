#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  vector<int> nextdiff(n);
  int p = 0;
  for (int i = 0; i < n; i++) {
    if (a[p] == a[i]) continue;
    for (int j = p; j < i; j++) {
      nextdiff[j] = i;
    }
    p = i;
  }
  for (int j = p; j < n; j++) {
    nextdiff[j] = n;
  }

  auto smaller = [&](int p1, int p2) {
    while (p1 < n && p2 < n) {
      if (a[p1] < a[p2]) return true;
      else if (a[p1] > a[p2]) return false;
      int delta = min(nextdiff[p1] - p1, nextdiff[p2] - p2);
      p1 += delta;
      p2 += delta;
    }
    if (p2 >= n) return false;
    else return true;
  };

  stack<int> next;
  vector<int> ans(n);
  int prev = a[n-1];
  ans[n-1] = n-1;
  bool same = true;
  for (int i = n-2; i >= 0; i--) {
    if (same) {
      if (a[i] == prev) {
        ans[i] = i;
        continue;
      }
      else if (a[i] > prev) {
        prev = a[i];
        next.push(i+1);
        ans[i] = i;
        continue;
      }
      else {
        same = false;
      }
    }
    if (a[i] > prev) {
      prev = a[i];
      same = true;
      next.push(i+1);
      ans[i] = i;
      continue;
    }
    else {
      while (!next.empty()) {
        auto now = next.top();
        if (smaller(i, now)) next.pop();
        else break;
      }
      if (next.empty()) ans[i] = n-1;
      else ans[i] = next.top() - 1;
      prev = a[i];
    }
  }

  for (auto &x: ans) cout << (x + 1) << endl;

  return 0;
}
