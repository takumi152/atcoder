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

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  string s, t;
  cin >> s;
  cin >> t;

  ll length = 0;
  vector<set<ll> > sap(256);
  for (int i = 0; i < s.size(); i++) {
    sap[s[i]].emplace(i);
  }

  int pointer = -1;
  for (int i = 0; i < t.size(); i++) {
    auto it1 = sap[t[i]].upper_bound(pointer);
    if (it1 != sap[t[i]].end()) {
      length += (*it1) - pointer;
      pointer = (*it1);
      continue;
    }
    length += s.size() - pointer - 1;
    pointer = -1;
    auto it2 = sap[t[i]].upper_bound(pointer);
    if (it2 != sap[t[i]].end()) {
      length += (*it2) - pointer;
      pointer = (*it2);
      continue;
    }
    cout << -1 << endl; // impossible
    return 0;
  }

  cout << length << endl;

  return 0;
}
