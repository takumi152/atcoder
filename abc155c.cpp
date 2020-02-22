#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>

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

  unordered_map<string, int> count;
  for (auto &x: s) count[x]++;

  int highest = 0;
  for (auto &x: count) {
    if (x.second > highest) highest = x.second;
  }

  vector<string> ans;
  for (auto &x: count) {
    if (x.second == highest) ans.push_back(x.first);
  }

  sort(ans.begin(), ans.end());

  for (auto &x: ans) cout << x << endl;

  return 0;
}
