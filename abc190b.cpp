#include <iostream>
#include <iomanip>
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

  int n, s, d;
  cin >> n >> s >> d;
  vector<Pii> xy(n);
  for (auto &x: xy) cin >> x.first >> x.second;

  for (auto &x: xy) {
    if (x.first < s && x.second > d) {
      cout << "Yes" << endl;
      return 0;
    }
  }

  cout << "No" << endl;

  return 0;
}
