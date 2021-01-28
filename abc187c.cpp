#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<string> s(n);
  for (auto &x: s) cin >> x;

  unordered_set<string> pos, neg;

  for (int i = 0; i < n; i++) {
    if (s[i][0] != '!') pos.insert(s[i]);
    else neg.insert(s[i].substr(1));
  }

  for (auto &x: pos) {
    if (neg.find(x) != neg.end()) {
      cout << x << endl;
      return 0;
    }
  }

  cout << "satisfiable" << endl;

  return 0;
}
