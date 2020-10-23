#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<Pii> ab(n);
  for (auto &x: ab) cin >> x.first >> x.second;

  // sanity check
  vector<bool> visited(2*n);
  bool good = true;
  for (auto &x: ab) {
    if (x.first != -1) {
      x.first--;
      if (visited[x.first]) good = false;
      else visited[x.first] = true;
    }
    if (x.second != -1) {
      x.second--;
      if (visited[x.second]) good = false;
      else visited[x.second] = true;
    }
    if (x.first != -1 && x.second != -1) {
      if (x.first >= x.second) good = false;
    }
  }
  if (!good) {
    cout << "No" << endl;
    return 0;
  }

  vector<int> c(2*n, -1);
  vector<int> delta(2*n);
  vector<int> id(2*n, -1);
  int d = 0;
  for (auto &x: ab) {
    if (x.first != -1) {
      delta[x.first] = 1;
      id[x.first] = d;
    }
    if (x.second != -1) {
      delta[x.second] = -1;
      id[x.second] = d;
    }
    if (x.first != -1 && x.second != -1) {
      for (int i = x.first; i <= x.second; i++) {
        if (c[i] == -1 || c[i] == (x.second - x.first - 1)) c[i] = x.second - x.first - 1;
        else {
          cout << "No" << endl;
          return 0;
        }
      }
    }
    d++;
  }

  vector<bool> dp(2*n+1);
  dp[0] = true;
  for (int i = 0; i < 2*n; i++) {
    if (dp[i]) {
      for (int j = 0; j < n; j++) {
        int p = i;
        bool good = true;
        for (int k = 0; k < j+1; k++) {
          if (p >= 2*n) {
            good = false;
            break;
          }
          if (delta[p] == -1) {
            good = false;
            break;
          }
          if (c[p] != -1 && c[p] != j) {
            good = false;
            break;
          }
          p++;
        }
        if (!good) continue;
        for (int k = 0; k < j+1; k++) {
          if (p >= 2*n) {
            good = false;
            break;
          }
          if (delta[p] == 1) {
            good = false;
            break;
          }
          if (c[p] != -1 && c[p] != j) {
            good = false;
            break;
          }
          if (!(id[p] == -1 || id[p-(j+1)] == -1) && id[p] != id[p-(j+1)]) {
            good = false;
            break;
          }
          p++;
        }
        if (good) dp[p] = true;
      }
    }
  }

  if (dp[2*n]) cout << "Yes" << endl;
  else cout << "No" << endl;

  return 0;
}
