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

  ll n, d;
  cin >> n >> d;

  if (n * d > n * (n - 1) / 2) {
    cout << "No" << endl;
    return 0;
  }

  vector<Pii> ans;
  for (int i = 0; i < d; i++) {
    int inc = i + 1;
    vector<bool> visited(n);
    for (int j = 0; j < n; j++) {
      if (visited[j]) break;
      int now = j;
      while (!visited[now]) {
        visited[now] = true;
        int next = (now + inc) % n;
        ans.emplace_back(now + 1, next + 1);
        now = next;
      }
    }
  }

  cout << "Yes" << endl;
  for (auto &x: ans) {
    cout << x.first << " " << x.second << endl;
  }

  return 0;
}
