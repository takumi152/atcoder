#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx2")

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <cmath>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, k;
  cin >> n >> k;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  for (int i = 0; i < min(k, 50); i++) {
    vector<ll> next(n);
    vector<ll> imos(n+1);
    for (int j = 0; j < n; j++) {
      imos[max(0, (int)(j-a[j]))]++;
      imos[min(n, (int)(j+a[j]+1))]--;
    }
    int now = 0;
    for (int j = 0; j < n; j++) {
      now += imos[j];
      next[j] = now;
    }
    a = next;
  }

  for (int i = 0; i < n; i++) {
    cout << a[i] << " ";
  }
  cout << endl;

  return 0;
}
