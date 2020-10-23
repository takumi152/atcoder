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

  int n, m;
  cin >> n >> m;
  vector<ll> w(n);
  for (auto &x: w) cin >> x;
  vector<ll> l(m), v(m);
  for (int i = 0; i < m; i++) cin >> l[i] >> v[i];



  return 0;
}
