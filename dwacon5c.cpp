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

  int n;
  cin >> n;
  string s;
  cin >> s;
  int q;
  cin >> q;
  vector<int> k(q);
  for (auto &x: k) cin >> x;

  vector<ll> ans(q);
  for (int i = 0; i < q; i++) {
    ll d = 0;
    ll m = 0;
    ll m_total = 0;
    queue<ll> m_delta;
    for (int j = 0; j < n; j++) {
      if (j-k[i] >= 0) {
        if (s[j-k[i]] == 'D') {
          d--;
          auto m_first = m_delta.front();
          m_delta.pop();
          m -= m_total - m_first;
        }
      }
      if (s[j] == 'D') {
        d++;
        m_delta.push(m_total);
      }
      else if (s[j] == 'M') {
        m += d;
        m_total++;
      }
      else if (s[j] == 'C') {
        ans[i] += m;
      }
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
