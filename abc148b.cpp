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
  string s, t;
  cin >> s >> t;

  vector<char> x;
  for (int i = 0; i < n; i++) {
    x.push_back(s[i]);
    x.push_back(t[i]);
  }

  for (auto &y: x) cout << y;
  cout << endl;

  return 0;
}
