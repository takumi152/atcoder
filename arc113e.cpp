// WIP

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

  int t;
  cin >> t;
  vector<string> s(t);
  for (auto &x: s) cin >> x;

  vector<string> ans(t);
  for (int k = 0; k < t; k++) {
    int n;
    int a, b;
    vector<bool> remain;

    {
      n = s[k].size();
      remain = vector<bool>(n, true);
      for (int i = 0; i < n; i++) {
        if (s[k][i] == 'a') a++;
        else b++;
      }
    }

    if (a == 0 || b == 0) {
      ans[k] = s[k];
      continue;
    }

    {
      int c = 0;
      bool f = false;
      int p1 = 0;
      for (int i = 0; i < n; i++) {
        if (s[k][i] == 'b') f = true;
        if (f && c % 2 == 0) break;
        if (s[k][i] == 'a') {
          c++;
          if (c % 2 != 0) p1 = i;
          else {
            remain[p1] = false;
            remain[i] = false;
          }
        }
      }
    }

    {
      string new_s;
      for (int i = 0; i < n; i++) {
        if (remain[i]) new_s.push_back(s[k][i]);
      }
      s[k] = new_s;
    }

    {
      n = s[k].size();
      remain = vector<bool>(n, true);
      for (int i = 0; i < n; i++) {
        if (s[k][i] == 'a') a++;
        else b++;
      }
    }

    if (a == 0 || b == 0) {
      ans[k] = s[k];
      continue;
    }

    if (s[k][n-1] == 'a') {
      int ra = 0;
      int i = n-1;
      for (; i >= 0; i--) {
        if (s[k][i] == 'a') ra++;
        else break;
      }

      int ca = 0;
      for (; i >= 0; i--) {
        if (s[k][i] == 'a') ca++;
        else {
          if (ca == 0) {

          }
        }
      }
    }

    ans[k] = s[k];
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
