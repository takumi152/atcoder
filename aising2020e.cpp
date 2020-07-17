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

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int t;
  cin >> t;
  vector<int> n(t);
  vector<vector<ll> > k(t), l(t), r(t);
  for (int i = 0; i < t; i++) {
    cin >> n[i];
    k[i] = vector<ll>(n[i]);
    l[i] = vector<ll>(n[i]);
    r[i] = vector<ll>(n[i]);
    for (int j = 0; j < n[i]; j++) cin >> k[i][j] >> l[i][j] >> r[i][j];
  }

  vector<ll> ans(t);
  for (int i = 0; i < t; i++) {
    vector<pair<ll, ll> > forward, backward;
    for (int j = 0; j < n[i]; j++) {
      if (l[i][j] > r[i][j]) forward.push_back(pair<ll, ll>(k[i][j], l[i][j] - r[i][j]));
      else if (l[i][j] < r[i][j]) backward.push_back(pair<ll, ll>(n[i] - k[i][j], r[i][j] - l[i][j]));
      ans[i] += max(l[i][j], r[i][j]);
    }

    sort(forward.begin(), forward.end());
    multiset<ll> forwardPlaced;
    int forwardCount = 0;
    for (auto &x: forward) {
      if (forwardCount < x.first) {
        forwardPlaced.insert(x.second);
        forwardCount++;
      }
      else if (!forwardPlaced.empty()) {
        if (x.second <= *(forwardPlaced.begin())) {
          ans[i] -= x.second;
        }
        else {
          ans[i] -= *(forwardPlaced.begin());
          forwardPlaced.erase(forwardPlaced.find(*(forwardPlaced.begin())));
          forwardPlaced.insert(x.second);
        }
      }
      else {
        ans[i] -= x.second;
      }
    }

    sort(backward.begin(), backward.end());
    multiset<ll> backwardPlaced;
    int backwardCount = 0;
    for (auto &x: backward) {
      if (backwardCount < x.first) {
        backwardPlaced.insert(x.second);
        backwardCount++;
      }
      else if (!backwardPlaced.empty()) {
        if (x.second <= *(backwardPlaced.begin())) {
          ans[i] -= x.second;
        }
        else {
          ans[i] -= *(backwardPlaced.begin());
          backwardPlaced.erase(backwardPlaced.find(*(backwardPlaced.begin())));
          backwardPlaced.insert(x.second);
        }
      }
      else {
        ans[i] -= x.second;
      }
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
