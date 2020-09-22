// AC-Library

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>
#include <cassert>

#include <atcoder/convolution>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;
  vector<ll> b(m);
  for (auto &x: b) cin >> x;

  vector<ll> c = atcoder::convolution<998244353>(a, b);

  for (auto &x: c) cout << x << " ";
  cout << endl;

  return 0;
}
