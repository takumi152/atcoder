// WIP

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

  vector<int> p(n);
  for (int i = 0; i < n; i++) p[i] = s[i] - '0' - 1;

  while (true) {
    for (int i = 0; i < n - p.size(); i++) cerr << " ";
    for (auto &x: p) cerr << x;
    cerr << endl;
    if (p.size() == 1) break;
    vector<int> next(p.size()-1);
    for (int i = 0; i < (int) p.size() - 1; i++) next[i] = abs(p[i] - p[i+1]);
    p = next;
  }

  return 0;
}
