#include <iostream>
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
  vector<ll> c(n);
  for (auto &x: c) cin >> x;
  int q;
  cin >> q;
  vector<vector<ll> > s(q, vector<ll>(3));
  for (int i = 0; i < q; i++) {
    cin >> s[i][0] >> s[i][1];
    if (s[i][0] == 1) {
      s[i][1]--; // zero index
      cin >> s[i][2];
    }
  }

  ll minimumAll = 1000000007;
  ll minimumEven = 1000000007;
  for (int i = 0; i < n; i++) {
    if (c[i] < minimumAll) minimumAll = c[i];
    if (i % 2 == 0 && c[i] < minimumEven) minimumEven = c[i];
  }

  ll sold = 0;
  ll packEven = 0;
  ll packAll = 0;
  for (int i = 0; i < q; i++) {
    if (s[i][0] == 1) {
      ll need = s[i][2] + packAll;
      if (s[i][1] % 2 == 0) need += packEven;
      if (c[s[i][1]] >= need) {
        sold += s[i][2];
        c[s[i][1]] -= s[i][2];
        if (c[s[i][1]] < minimumAll) minimumAll = c[s[i][1]];
        if (s[i][1] % 2 == 0 && c[s[i][1]] < minimumEven) minimumEven = c[s[i][1]];
      }
    }
    else if (s[i][0] == 2) {
      ll need = s[i][1] + packAll + packEven;
      if (minimumEven >= need) {
        sold += s[i][1] * ((n + 1) / 2);
        packEven += s[i][1];
      }
    }
    else if (s[i][0] == 3) {
      ll needEven = s[i][1] + packAll + packEven;
      ll needOdd = s[i][1] + packAll;
      if (minimumEven >= needEven && minimumAll >= needOdd) {
        sold += s[i][1] * n;
        packAll += s[i][1];
      }
    }
  }

  cout << sold << endl;

  return 0;
}
