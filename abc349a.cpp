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

  int n;
  cin >> n;
  vector<int> a(n-1);
  for (auto &x: a) cin >> x;

  int sum = 0;
  for (int i = 0; i < n-1; i++) {
    sum += a[i];
  }

  cout << -sum << endl;

  return 0;
}
