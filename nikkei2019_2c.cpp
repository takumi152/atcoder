// WA

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stack>
#include <set>
#include <map>
#include <list>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;
const ll inf = (ll) 1e18;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a(n), b(n);
  for (auto &x: a) cin >> x;
  for (auto &x: b) cin >> x;

  // possibility check
  vector<int> as = a, bs = b;
  sort(as.begin(), as.end());
  sort(bs.begin(), bs.end());
  for (int i = 0; i < n; i++) {
    if (as[i] > bs[i]) {
      cout << "No" << endl; // impossible
      return 0;
    }
  }

  map<int, set<int> > ab, abs;
  for (int i = 0; i < n; i++) {
    ab[a[i]].insert(b[i]);
    abs[as[i]].insert(bs[i]);
  }
  for (auto it = ab.begin(); it != ab.end(); it++) {
    auto itab = ab[(*it).first].begin();
    auto itabs = abs[(*it).first].begin();
    while (itab != ab[(*it).first].end() && itabs != abs[(*it).first].end()) {
      if ((*itab) < (*itabs)) {
        itab++;
      }
      else if ((*itab) > (*itabs)) {
        itabs++;
      }
      else {
        cout << "Yes" << endl; // possible
        return 0;
      }
    }
  }

  int ap = 0, bp = 0;
  while (ap < n && bp < n) {
    if (as[ap] <= bs[bp]) {
      if (ap > bp) {
        cout << "Yes" << endl; // possible
        return 0;
      }
      else {
        ap++;
      }
    }
    else {
      bp++;
    }
  }

  vector<Pii> abi(n);
  for (int i = 0; i < n; i++) {
    abi[i].first = b[i];
    abi[i].second = a[i];
  }
  sort(abi.begin(), abi.end());
  int smallestNum = 1000000007;
  int smallestIdx = 0;
  int largestNum = 0;
  int largestIdx = 0;
  for (int i = 0; i < n; i++) {
    if (abi[i].second < smallestNum) {
      smallestNum = abi[i].second;
      smallestIdx = i;
    }
    if (abi[i].second > largestNum) {
      largestNum = abi[i].second;
      largestIdx = i;
    }
  }

  if (largestIdx == 0 && smallestIdx == 1) {
    cout << "No" << endl;
  }
  else {
    cout << "Yes" << endl;
  }

  return 0;
}
