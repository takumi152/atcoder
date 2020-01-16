#include <iostream>
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

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> p(n);
  for (auto &x: p) {
    cin >> x;
    x--;
  }

  vector<int> q(n);
  for (int i = 0; i < n; i++) q[i] = i;
  sort(q.begin(), q.end(), [&](auto &a, auto &b){ return p[a] < p[b]; });

  vector<int> a(n), b(n);
  for (int i = 0; i < n; i++) a[i] = (i + 1) * 30000;
  for (int i = n-1; i >= 0; i--) b[i] = (n - i) * 30000 + q[i];

  for (auto &x: a) cout << x << " ";
  cout << endl;
  for (auto &x: b) cout << x << " ";
  cout << endl;

  return 0;
}
