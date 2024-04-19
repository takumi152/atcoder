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

  int n, m;
  cin >> n >> m;
  vector<int> a(n), b(m);
  for (auto &x: a) cin >> x;
  for (auto &x: b) cin >> x;

  int score = 0;
  for (auto &x: b) {
    score += a[x-1];
  }

  cout << score << endl;

  return 0;
}
