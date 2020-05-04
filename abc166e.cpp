#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  unordered_map<int, ll> left_count, right_count;
  for (int i = 0; i < n; i++) {
    left_count[i-a[i]]++;
    right_count[i+a[i]]++;
  }

  ll ans = 0;
  for (auto &x: left_count) {
    ans += x.second * right_count[x.first];
  }

  cout << ans << endl;

  return 0;
}
