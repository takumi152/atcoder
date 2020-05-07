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
  vector<Pii> ba(n);
  for (auto &x: ba) cin >> x.second >> x.first;

  sort(ba.begin(), ba.end());

  int elapsed = 0;
  for (int i = 0; i < n; i++) {
    elapsed += ba[i].second;
    if (elapsed > ba[i].first) {
      cout << "No" << endl;
      return 0;
    }
  }

  cout << "Yes" << endl;

  return 0;
}
