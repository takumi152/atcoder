// TLE

#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("sse4.2")

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <list>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, p;
  cin >> n >> p;
  string s;
  cin >> s;

  vector<int> t(n);
  for (int i = 0; i < n; i++) t[i] = s[i] - '0';

  vector<vector<int> > modtable(p, vector<int>(10));
  for (int i = 0; i < p; i++) {
    for (int j = 0; j < 10; j++) {
      modtable[i][j] = (i * 10 + j) % p;
    }
  }

  ll ans = 0;
  vector<ll> now1(p), now2(p);
  for (int i = 0; i < n; i++) {
    if (i % 2 == 0) {
      if (p == 2 || p == 5) {
        for (int j = 0; j < p; j++) now1[j] = 0;
        for (int j = 0; j < p; j++) now1[modtable[j][t[i]]] += now2[j];
        now1[t[i] % p]++;
        ans += now1[0];
      }
      else {
        for (int j = 0; j < p; j++) now1[modtable[j][t[i]]] = now2[j];
        now1[t[i] % p]++;
        ans += now1[0];
      }
    }
    else {
      if (p == 2 || p == 5) {
        for (int j = 0; j < p; j++) now2[j] = 0;
        for (int j = 0; j < p; j++) now2[modtable[j][t[i]]] += now1[j];
        now2[t[i] % p]++;
        ans += now2[0];
      }
      else {
        for (int j = 0; j < p; j++) now2[modtable[j][t[i]]] = now1[j];
        now2[t[i] % p]++;
        ans += now2[0];
      }
    }
  }

  cout << ans << endl;

  return 0;
}
