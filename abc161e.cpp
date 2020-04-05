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

  int n, k, c;
  cin >> n >> k >> c;
  string s;
  cin >> s;

  unordered_set<int> segment_front;
  int p = 0;
  while (p < n) {
    if (s[p] == 'o') {
      segment_front.insert(p);
      p += c + 1;
    }
    else p++;
  }
  unordered_set<int> segment_back;
  p = n-1;
  while (p >= 0) {
    if (s[p] == 'o') {
      segment_back.insert(p);
      p -= c + 1;
    }
    else p--;
  }

  if ((int) segment_front.size() > k) {
    cout << endl;
    return 0;
  }

  vector<int> ans;
  for (auto &x: segment_front) {
    if (segment_back.find(x) != segment_back.end()) ans.push_back(x);
  }

  sort(ans.begin(), ans.end());

  for (auto &x: ans) cout << x + 1 << endl;

  return 0;
}
