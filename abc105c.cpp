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

  ll n;
  cin >> n;

  vector<bool> plus(24);
  vector<bool> minus(24);
  vector<ll> pos(24);
  vector<ll> neg(24);
  pos[0] = 1;
  neg[0] = 2;
  for (int i = 1; i < 24; i++) {
    pos[i] = pos[i-1] * 4;
    neg[i] = neg[i-1] * 4;
  }

  ll a = 0;
  for (int i = 0; i < 24; i++) {
    a -= neg[i];
    minus[i] = true;
  }
  for (int i = 23; i >= 0; i--) {
    if (a + neg[i] <= n) {
      a += neg[i];
      minus[i] = false;
    }
    if (a + pos[i] <= n) {
      a += pos[i];
      plus[i] = true;
    }
  }

  vector<int> ans;
  for (int i = 23; i >= 0; i--) {
    if (minus[i]) ans.push_back(1);
    else if (ans.size() > 0) ans.push_back(0);
    if (plus[i]) ans.push_back(1);
    else if (ans.size() > 0) ans.push_back(0);
  }
  if (ans.size() == 0) ans.push_back(0);

  for (auto &x: ans) cout << x;
  cout << endl;

  return 0;
}
