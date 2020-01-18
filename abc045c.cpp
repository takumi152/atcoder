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

  string s;
  cin >> s;

  int n = s.length();

  vector<ll> t;
  for (auto &x: s) t.push_back(x - '0');

  ll total = 0;
  for (int i = 0; i < (1 << (n-1)); i++) {
    int m = i;
    ll a = t[0];
    for (int j = 1; j < n; j++) {
      if (m & 1) {
        total += a;
        a = t[j];
      }
      else {
        a *= 10;
        a += t[j];
      }
      m >>= 1;
    }
    total += a;
  }

  cout << total << endl;

  return 0;
}
