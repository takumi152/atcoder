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

  string s;
  cin >> s;

  unordered_set<char> used;

  for (auto &x: s) used.insert(x);

  if (s.length() < 26) {
    for (char c = 'a'; c <= 'z'; c++) {
      if (used.find(c) == used.end()) {
        cout << s << c << endl;
        return 0;
      }
    }
  }
  else {
    auto sx = s;
    if (!next_permutation(sx.begin(), sx.end())) {
      cout << -1 << endl;
      return 0;
    }
    string next;
    for (int i = 0; i < 26; i++) {
      next.push_back(sx[i]);
      if (s[i] != sx[i]) {
        cout << next << endl;
        return 0;
      }
    }
  }

  return 0;
}
