// WA

#include <iostream>
#include <iomanip>
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

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<ll> a(2*n);
  for (auto &x: a) cin >> x;

  vector<ll> left(n), right(n);
  for (int i = 0; i < n; i++) {
    left[i] = a[i];
    right[i] = a[n+i];
  }
  reverse(left.begin(), left.end());

  set<pair<ll, ll> > left_deck, right_deck;
  for (int i = 0; i < n; i++) {
    left_deck.emplace(left[i], i);
    right_deck.emplace(right[i], i);
  }

  vector<bool> left_used(n), right_used(n);
  int lp = 0, rp = 0;

  ll ans = 0;

  for (int i = 0; i < n; i++) {
    auto [left_max, left_idx] = *left_deck.rbegin();
    auto [right_max, right_idx] = *right_deck.rbegin();

    ll left_take_score = left_max - right[rp];
    ll right_take_score = right_max - left[lp];

    if (left_take_score > right_take_score) {
      ans += left_max;
      left_used[left_idx] = true;
      left_deck.erase(pair<ll, ll>(left_max, left_idx));
      right_used[rp] = true;
      right_deck.erase(pair<ll, ll>(right[rp], rp));
    }
    else {
      ans += right_max;
      right_used[right_idx] = true;
      right_deck.erase(pair<ll, ll>(right_max, right_idx));
      left_used[lp] = true;
      left_deck.erase(pair<ll, ll>(left[lp], lp));
    }

    while (lp < n) {
      if (left_used[lp]) lp++;
      else break;
    }
    while (rp < n) {
      if (right_used[rp]) rp++;
      else break;
    }

    cerr << i << " " << ans << endl;
  }

  cout << ans << endl;

  return 0;
}
