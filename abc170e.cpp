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

  int n, q;
  cin >> n >> q;
  vector<int> a(n), b(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i] >> b[i];
    b[i]--;
  }
  vector<int> c(q), d(q);
  for (int i = 0; i < q; i++) {
    cin >> c[i] >> d[i];
    c[i]--;
    d[i]--;
  }

  vector<multiset<int> > now(200000);
  for (int i = 0; i < n; i++) {
    now[b[i]].insert(a[i]);
  }
  multiset<int> highest;
  for (int i = 0; i < 200000; i++) {
    if (!now[i].empty()) highest.insert(*(now[i].rbegin()));
  }

  vector<int> ans(q);
  for (int i = 0; i < q; i++) {
    // cerr << "A " << i << " ";
    // for (auto &x: highest) cerr << x << " ";
    // cerr << endl;
    // for (int j = 0; j < 200000; j++) {
    //   if (!now[j].empty()) {
    //     cerr << "B " << j << " ";
    //     for (auto &x: now[j]) cerr << x << " ";
    //     cerr << endl;
    //   }
    // }
    // cerr << "C ";
    // for (auto &x: b) cerr << x << " ";
    // cerr << endl;
    if (*(now[b[c[i]]].rbegin()) == a[c[i]]) {
      auto ith = highest.find(a[c[i]]);
      highest.erase(ith);
      auto itn = now[b[c[i]]].find(a[c[i]]);
      now[b[c[i]]].erase(itn);
      if (!now[b[c[i]]].empty()) highest.insert(*(now[b[c[i]]].rbegin()));
    }
    else {
      auto itn = now[b[c[i]]].find(a[c[i]]);
      now[b[c[i]]].erase(itn);
    }

    if (!now[d[i]].empty()) {
      if (*(now[d[i]].rbegin()) < a[c[i]]) {
        auto ith = highest.find(*(now[d[i]].rbegin()));
        highest.erase(ith);
        highest.insert(a[c[i]]);
      }
    }
    else {
      highest.insert(a[c[i]]);
    }
    now[d[i]].insert(a[c[i]]);
    b[c[i]] = d[i];

    ans[i] = *(highest.begin());
  }

  // cerr << "A " << q << " ";
  // for (auto &x: highest) cerr << x << " ";
  // cerr << endl;
  // for (int j = 0; j < 200000; j++) {
  //   if (!now[j].empty()) {
  //     cerr << "B " << j << " ";
  //     for (auto &x: now[j]) cerr << x << " ";
  //     cerr << endl;
  //   }
  // }
  // cerr << "C ";
  // for (auto &x: b) cerr << x << " ";
  // cerr << endl;

  for (auto &x: ans) cout << x << endl;

  return 0;
}
