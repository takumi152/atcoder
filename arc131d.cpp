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

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, m, d;
  cin >> n >> m >> d;
  vector<ll> r(m+1);
  for (auto &x: r) cin >> x;
  vector<ll> s(m);
  for (auto &x: s) cin >> x;
  s.push_back(0);

  if (n == 1) {
    cout << s[0] << endl;
    return 0;
  }

  vector<ll> start_pos(n);
  for (int i = 0; i < n; i++) start_pos[i] = (i - ((n+1)/2)) * d;

  vector<ll> r_full;
  vector<ll> s_full;
  r_full.push_back((ll) -9e18);
  s_full.push_back(0);
  for (int i = m-1; i >= 0; i--) {
    r_full.push_back(-r[i+1]);
    s_full.push_back(s[i]);
  }
  for (int i = 1; i <= m; i++) {
    r_full.push_back(r[i] + 1);
    s_full.push_back(s[i]);
  }

  ll score = 0;
  vector<pair<ll, ll> > score_event;
  for (int i = 0; i < n; i++) {
    auto it_r = upper_bound(r_full.begin(), r_full.end(), start_pos[i]);
    it_r--;
    auto it_s = s_full.begin() + distance(r_full.begin(), it_r);
    ll score_now = *it_s;
    score += score_now;

    it_r = upper_bound(r_full.begin(), r_full.end(), start_pos[i]);
    it_s = s_full.begin() + distance(r_full.begin(), it_r);
    auto it_sp = it_s - 1;
    while (it_r != r_full.end()) {
      if (*(it_r) - start_pos[i] > d) break;
      score_event.emplace_back(*(it_r) - start_pos[i], *(it_s) - *(it_sp));
      it_r++;
      it_s++;
      it_sp++;
    }
  }

  sort(score_event.begin(), score_event.end());
  ll best_score = score;
  for (auto &[_, x]: score_event) {
    score += x;
    if (score > best_score) best_score = score;
  }

  cout << best_score << endl;

  return 0;
}
