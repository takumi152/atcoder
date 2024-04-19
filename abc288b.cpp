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

  int n, k;
  cin >> n >> k;
  vector<string> s(n);
  for (auto &x: s) cin >> x;

  sort(s.begin(), s.begin() + k);

  for (int i = 0; i < k; i++) cout << s[i] << endl;

  return 0;
}
