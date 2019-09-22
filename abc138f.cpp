// WIP

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stack>
#include <set>
#include <map>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;

ll count0(ll x) {
  ll zero = 0;
  ll xx = x;
  while (xx > 0) {
    if ((xx & 1) == 0) zero++;
    xx >>= 1;
  }
  return zero;
}

ll count1(ll x) {
  ll zero = 0;
  ll xx = x;
  while (xx > 0) {
    if ((xx & 1) == 1) zero++;
    xx >>= 1;
  }
  return zero;
}

ll floor2(ll x) {
  if (x == 0) return 0;
  ll a = 1;
  ll xx = x >> 1;
  while (xx > 0) {
    a <<= 1;
    xx >>= 1;
  }
  return a;
}

ll bitCount(ll x) {
  ll a = 0;
  ll xx = x;
  while (xx > 0) {
    a++;
    xx >>= 1;
  }
  return a;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll L, R;
  cin >> L >> R;

  // 各iについて2^(iを2進数で表したときの0の個数)個存在

  ll ans = 0;
  vector<ll> pow2;
  vector<ll> pow2r;
  pow2.emplace_back(1);
  pow2r.emplace_back(1);
  for (int i = 1; i < 64; i++) {
    pow2.emplace_back((pow2[i-1] * 2) % MOD);
    pow2r.emplace_back(pow2r[i-1] * 2);
  }
  vector<ll> pow3;
  pow3.emplace_back(1);
  for (int i = 1; i < 64; i++) {
    pow3.emplace_back((pow3[i-1] * 3) % MOD);
  }
  stack<pair<ll, ll> > st;
  st.emplace(0, 62);
  while(!st.empty()) {
    auto now = st.top();
    st.pop();
    cout << now.first << " " << now.second << " " << ans << endl;
    if (L <= now.first && now.first + pow2r[now.second] - 1 <= R) {
      ans = (ans + pow3[now.second] * pow2[count0(now.first)-now.second]) % MOD;
      continue;
    }
    if ((now.first <= L && L <= now.first + pow2r[now.second] - 1) || (now.first <= R && R <= now.first + pow2r[now.second] - 1)) {
      st.emplace(now.first, now.second-1);
      st.emplace(now.first + pow2r[now.second-1], now.second-1);
    }
  }
  L = floor2(R);
  R = R ^ (floor2(R)-1);
  st.emplace(0, 62);
  while(!st.empty()) {
    auto now = st.top();
    st.pop();
    cout << now.first << " " << now.second << " " << ans << endl;
    if (L <= now.first && now.first + pow2r[now.second] - 1 <= R) {
      ans = (ans - pow3[now.second] * pow2[count0(now.first)-now.second]) % MOD;
      if (ans < 0) ans += MOD;
      continue;
    }
    if ((now.first <= L && L <= now.first + pow2r[now.second] - 1) || (now.first <= R && R <= now.first + pow2r[now.second] - 1)) {
      st.emplace(now.first, now.second-1);
      st.emplace(now.first + pow2r[now.second-1], now.second-1);
    }
  }

  cout << ans << endl;

  return 0;
}
